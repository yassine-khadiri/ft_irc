/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/19 21:30:39 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ircserv.hpp"

Ircserv::Ircserv( int port, std::string password ): Tcp(port), password(password)
{
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

int Ircserv::checkMessageInfos( std::string recvMessage )
{
    std::cout << "content: " << recvMessage << std::endl;
    return 0;  
};

int Ircserv::waitForConnection()
{
    int socketClient, client_sockets[MAX_CLIENTS] = {0}, sd, num_clients = 0;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    fd_set readfds;
    char buff[1024] = {0};
    int max_sd;

    std::cout << "Waiting For Incoming IRC Connections...!" << std::endl;
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

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
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
                client_sockets[num_clients++] = socketClient;
            else
                close(socketClient);
        }
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            // if (client_sockets[i] == 0)
            // {
            //     client_sockets[i] = socketClient;
            //     break;
            // }
            if (FD_ISSET(client_sockets[i], &readfds))
            {                
                if (recv(client_sockets[i], buff, sizeof(buff), 0) > 0)
                {
                    std::cout << buff << std::endl;
                    memset(buff, 0, sizeof(buff));
                }
            }
        }
    }
    
    return EXIT_SUCCESS;
};
