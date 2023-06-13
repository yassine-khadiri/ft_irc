/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:21:13 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/13 16:33:57 by ykhadiri         ###   ########.fr       */
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

// int Tcp::createClientSocket()
// {
//     std::string _val;
//     char buff[1024];

//     if (!this->initialize())
//     {
//         if (!(connect(this->socket_fd, (const sockaddr *)&this->addr, sizeof(this->addr))))
//         {
//             while (1)
//             {
//                 if(getline(std::cin, _val) && _val.length())
//                 {   
//                     if (send(this->socket_fd, _val.c_str(), _val.length() + 1, 0) > 0)
//                     {
//                         if (recv(this->socket_fd, buff, 1024, 0) > 0)
//                             std::cout << "SERVER SAID: " << buff << std::endl;
//                     }
//                 }
//             }
//         }
//     }
//     return EXIT_FAILURE;
// };
