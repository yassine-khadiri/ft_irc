/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:20:55 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/13 17:37:18 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCP_HPP
#define TCP_HPP

#include <iostream>
#include <string>
#include <cstring>
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

class Tcp
{
protected:
    int socket_fd;
    int port;
    struct sockaddr_in addr;
public:
    Tcp( int port );
    ~Tcp();
    int initialize();
    int createServerSocket();
    // int createClientSocket();
    virtual int waitForConnection() = 0;
};

#endif