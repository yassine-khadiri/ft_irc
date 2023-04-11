/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:44:07 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/11 21:50:55 by ykhadiri         ###   ########.fr       */
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
        std::string port;
        std::string password;
    public:
        Ircserv( std::string port, std::string password );
        ~Ircserv();
};

#endif