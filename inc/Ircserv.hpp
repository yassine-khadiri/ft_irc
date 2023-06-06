/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:02 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 17:49:24 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#define MAX_CLIENTS 30

#include "Tcp.hpp"
#include <sys/select.h>
#include "Client.hpp"
#include "Command.hpp"
#include <vector>

class Ircserv : public Tcp
{
    std::string password;
    std::vector<Client> _clients;
    int waitForConnection();
    Command     cmd;
public:
    Ircserv();
    Ircserv( int port, std::string password );
    ~Ircserv();
    int createServerSocket();
};

#endif