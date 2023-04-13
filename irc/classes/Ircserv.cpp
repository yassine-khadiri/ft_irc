/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/13 21:54:59 by ykhadiri         ###   ########.fr       */
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

int Ircserv::waitForConnection()
{
    int socketClient;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buff[1024];

    std::cout << "Waiting For Uncoming Connection...!" << std::endl;
    socketClient = accept(this->socket_fd, (sockaddr *)&clientAddr, &clientAddrSize);
    while(1)
    {
        if (recv(socketClient, buff, 1024, 0) > 0)
        {
            if (send(socketClient, buff, 1024, 0) < 0)
            {
                std::cout << "response" << std::endl;
                // return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
};
