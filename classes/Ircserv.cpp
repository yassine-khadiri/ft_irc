/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/11 21:51:27 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ircserv.hpp"

Ircserv::Ircserv( std::string port, std::string password ): port(port), password(password)
{
};

Ircserv::~Ircserv()
{
};

