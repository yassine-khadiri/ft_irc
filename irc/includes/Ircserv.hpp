/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:02 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/15 21:13:02 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "../../tcp/includes/Tcp.hpp"

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
