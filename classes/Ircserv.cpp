/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/12 21:30:08 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ircserv.hpp"

Ircserv::Ircserv( int port, std::string password ): port(port), password(password)
{
};

Ircserv::~Ircserv()
{
};

int Ircserv::initialize()
{
    if ((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error Creating Socket...!" << std::endl;
        return EXIT_FAILURE;
    }

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = inet_addr(this->ipAdress);

    return EXIT_SUCCESS;
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
                std::cout << "httpResponse" << std::endl;
                // return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
};

