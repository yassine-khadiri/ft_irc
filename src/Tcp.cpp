/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:21:13 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 11:24:39 by ykhadiri         ###   ########.fr       */
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
