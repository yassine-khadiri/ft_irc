/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/12 16:27:33 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Ircserv.hpp"
#include "../inc/Client.hpp"

Ircserv::Ircserv()
{
};

Ircserv::Ircserv( int port, std::string password ): Tcp(port), password(password),cmd(Command(password))
{
    recvString ="";
};

Ircserv::~Ircserv()
{
};

int Ircserv::createServerSocket()
{
    if (!this->initialize())
    {
        if (bind(this->socket_fd, (sockaddr *)&this->addr, sizeof(this->addr)))
        {
            std::cerr << "Error Binding Socket...!" << std::endl;
            return EXIT_FAILURE;
        }

        if (listen(this->socket_fd, 1))
        {
            std::cerr << "Error Listening...!" << std::endl;
            return EXIT_FAILURE;
        }
        this->waitForConnection();
    }
    return EXIT_FAILURE;
};

int Ircserv::waitForConnection()
{
    // int socketClient, client_sockets[MAX_CLIENTS] = {0}, sd, num_clients = 0;
    int socketClient, sd, num_clients = 0;
    std::vector<int> client_sockets(MAX_CLIENTS, 0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    fd_set readfds;
    char buff[1024] = {0};
    int max_sd;

    std::cout << "Waiting For Incoming IRC Connections...!" << std::endl;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(this->socket_fd, &readfds);

        max_sd = this->socket_fd;
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_sockets[i];

            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }
        int activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
        if (activity < 0)
        {
            std::cout << "Select Error!" << std::endl;
            return EXIT_FAILURE;
        }

        if (FD_ISSET(this->socket_fd, &readfds))
        {
            if ((socketClient = accept(this->socket_fd, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrSize)) < 0)
            {
                std::cout << "Accept Failed!" << std::endl;
                return EXIT_FAILURE;
            }
            if (num_clients < MAX_CLIENTS)
            {
                this->_clients.push_back(Client(socketClient));
                client_sockets.push_back(socketClient);
                num_clients++;
            }
            else
                close(socketClient);
        }
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (FD_ISSET(client_sockets[i], &readfds))
            {                
                if (recv(client_sockets[i], buff, sizeof(buff), 0) > 0)
                {
                    std::string str(buff);  
                    if (str.find('\n') == std::string::npos)
                        recvString += str;
                    else
                    {
                        if (!recvString.empty())
                        {
                            str = recvString + str;
                            recvString = "";
                        }
                        str.erase(str.find_last_not_of("\r\n") + 1);
                        if (str.find('\n'))
                        {
                            std::stringstream ss(str);
                            while(std::getline(ss,str))
                            {
                                if (!str.empty())
                                    cmd.exec(i, str, this->_clients);
                            }
                        }
                        else if (!str.empty())
                            cmd.exec(i, str, this->_clients);
                    }
                    memset(buff, 0, sizeof(buff));
                }
                else
                {
                    std::cout << "HERE\n";
                    close(client_sockets[i]);
                    // std::vector<Client>::ite[r]it = this->_clients.begin();
                    // std::cout << (it + i)->getNickname() << std::endl;
                    // this->_clients.erase(it + i);
                    // client_sockets.erase(std::remove(client_sockets->begin(), client_sockets->end(), client_sockets-[i]), client_sockets->end());
                    client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_sockets[i]), client_sockets.end());
                    // client_sockets[i] = 0;
                    // if( i != num_clients - 1)
                    // {
                    //     int tmp = i;
                    //     while(tmp < num_clients - 1)
                    //     {
                    //         client_sockets[tmp] = client_sockets[tmp + 1];
                    //     }
                    // }
                    num_clients = num_clients - 1;
                    // if(num_clients < 0)
                    //     num_clients = 0;
                    // FD_CLR(client_sockets[i], &readfds);
                }
            }
        }
    }
    return EXIT_SUCCESS;
};
