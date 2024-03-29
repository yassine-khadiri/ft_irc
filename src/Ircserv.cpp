/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 12:46:13 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Ircserv.hpp"
#include "../inc/Client.hpp"

Ircserv::Ircserv()
{
};

Ircserv::Ircserv( int port, std::string password ): Tcp(port), password(password),cmd(Command(password))
{
    recvString = "";
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

        if (fcntl(this->socket_fd, F_SETFL, O_NONBLOCK) == -1)
        {
            std::cerr << "Error Making Server Socket non-blocking!" << std::endl;
            return EXIT_FAILURE;
        }

        if (listen(this->socket_fd, SOMAXCONN))
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
    int socketClient, sd, num_clients = 0;
    std::vector<int>  client_sockets(MAX_CLIENTS);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    fd_set readfds;
    char buff[1024] = {0};
    int max_sd;

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
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

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
            if (fcntl(socketClient, F_SETFL, O_NONBLOCK) == -1)
            {
                std::cerr << "Error Making Client Socket non-blocking!" << std::endl;
                return EXIT_FAILURE;
            }
            if (num_clients < MAX_CLIENTS)
            {
                this->_clients.push_back(Client(socketClient));
                client_sockets[num_clients] = socketClient;
                num_clients++;

            }
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
                        if (str.find('\n'))
                        {
                            std::stringstream ss(str);
                            while(std::getline(ss,str))
                            {
                                str.erase(str.find_last_not_of("\r") + 1);
                                if (!str.empty())
                                    cmd.exec(i, str, this->_clients, client_sockets, num_clients);
                            }
                        }
                    }
                    memset(buff, 0, sizeof(buff));
                }
                else
                {
                    std::string msg = "QUIT Leaving...";
                    if (!this->_clients[i].getNickname().empty())
                        cmd.exec(i, msg, this->_clients,client_sockets,num_clients);
                }
            }
        }

    }
    return EXIT_SUCCESS;
};
