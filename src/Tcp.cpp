/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:21:13 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/16 12:47:27 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Tcp.hpp"

Tcp::Tcp()
{  
};

Tcp::Tcp( int port ): port(port)
{
};

Tcp::~Tcp()
{
};

int Tcp::initialize()
{
    if ((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error Creating Socket...!" << std::endl;
        return EXIT_FAILURE;
    }
    int optval = 1;
    setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    // this->addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->addr.sin_addr.s_addr = INADDR_ANY;
    return EXIT_SUCCESS;
};

int Tcp::createServerSocket()
{
    if (!this->initialize())
    {
        if (bind(this->socket_fd, (sockaddr *)&this->addr, sizeof(this->addr)))
        {
            std::cerr << "Error Binding Socket...!" << std::endl;
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
