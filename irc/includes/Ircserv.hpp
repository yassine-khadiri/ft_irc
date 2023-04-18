/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:02 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/18 20:39:11 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#define MAX_CLIENTS 30
#include "../../tcp/includes/Tcp.hpp"
#include <sys/select.h>

class Ircserv : public Tcp
{
    std::string password;
    int waitForConnection();
    int checkMessageInfos( std::string recvMessage );
public:
    Ircserv( int port, std::string password );
    ~Ircserv();
    int createServerSocket();
};

#endif
