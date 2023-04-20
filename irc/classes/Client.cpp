/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:42:27 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/20 22:07:31 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

Client::Client()
{	
}

Client::Client( int fdSocket ): _password(""), _nickname(""), _username(""), _isRegistered(false), _fdsocket(fdSocket)
{
	
}

Client::Client(std::string pass, std::string nick, std::string user): _password(pass), _nickname(nick), _username(user), _isRegistered(true)
{
	
}

Client::Client(Client const &src)
{
	*this = src;
}

Client& Client::operator=(Client const &src)
{
	if (this != &src)
	{
		this->_password = src._password;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_isRegistered = src._isRegistered;
		this->_fdsocket = src._fdsocket;
	}
	return (*this);
}

Client::~Client()
{
	
}

void		Client::setPassword(std::string pass)
{
	this->_password = pass;
}

std::string	Client::getPassword()
{
	return (this->_password);
}

void		Client::setNickname(std::string nick)
{
	this->_nickname = nick;
}

std::string	Client::getNickname()
{
	return (this->_nickname);
}

void		Client::setUsername(std::string user)
{
	this->_username = user;
}

std::string	Client::getUsername()
{
	return (this->_username);
}

void Client::setIsRegistered(bool isRegistered)
{
	this->_isRegistered = isRegistered;
}

bool Client::getIsRegistered()
{
	return (this->_isRegistered);
}

void Client::setFd(int fdsocket)
{
	this->_fdsocket = fdsocket;
}

int Client::getFd()
{
	return (this->_fdsocket);
}
