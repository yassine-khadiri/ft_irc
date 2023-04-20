/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:02 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/20 17:43:30 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#define MAX_CLIENTS 30
#include "../../tcp/includes/Tcp.hpp"
#include <sys/select.h>
#include <vector>

class Ircserv : public Tcp
{
    std::string password;
    std::vector<Client> _clients;
    int waitForConnection();
    int checkMessageInfos( std::string recvMessage );
public:
    Ircserv( int port, std::string password );
    ~Ircserv();
    int createServerSocket();
};

#endif
