/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/18 21:24:53 by ykhadiri         ###   ########.fr       */
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
    int socketClient, client_sockets[MAX_CLIENTS] = {0}, sd;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    fd_set readfds;
    char buff[1024] = {0};

    std::cout << "Waiting For Incoming IRC Connections...!" << std::endl;
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(this->socket_fd, &readfds);

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_sockets[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
        }

        int activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
            std::cout << "Select Error!" << std::endl;

        if (FD_ISSET(this->socket_fd, &readfds))
        {
            if ((socketClient = accept(this->socket_fd, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrSize)) < 0)
            {
                std::cout << "Accept Failed!" << std::endl;
                return EXIT_FAILURE;
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client_sockets[i] == 0)
            {
                client_sockets[i] = socketClient;
                break;
            }
        }

        if (recv(socketClient, buff, sizeof(buff), 0) > 0)
            std::cout << buff << std::endl;
    }
    return EXIT_SUCCESS;
};
