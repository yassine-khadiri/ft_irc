/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:02 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/20 17:53:44 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#define MAX_CLIENTS 30
#include "../../tcp/includes/Tcp.hpp"
#include <sys/select.h>
#include "Client.hpp"
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
