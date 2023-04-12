/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:44:07 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/12 22:00:24 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <sys/types.h> 
#include <sys/socket.h>
// For the sockaddr_in structure
#include <netinet/in.h>
//  For the inet_addr function
#include <arpa/inet.h>

class Ircserv
{
        int socket_fd;
        int port;
        std::string password;
        const char *ipAdress;
        struct sockaddr_in addr;
        int initialize();
        int createServerSocket();
    public:
        Ircserv( int port, std::string password );
        ~Ircserv();
        int waitForConnection();
};

#endif