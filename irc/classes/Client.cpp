/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:42:27 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/28 13:33:35 by hbouqssi         ###   ########.fr       */
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
	if(isRegistered == true)
	{
		std::cout << "Client " << this->_nickname << " is now registered" << std::endl;
	}
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

void Client::setOpPrivilegePermission(int OpPrivilege)
{
	this->Operator = OpPrivilege;
}

int Client::getOpPriviligePermission() const
{
	return(this->Operator);
}

void Client::joinChannel(Channel &_Channel)
{
	joined.insert(std::make_pair(_Channel.getChannelName(), _Channel));
}

void Client::channelSegment(Channel &_Channel)
{
	joined.erase(_Channel.getChannelName());
}

int Client::isMemberOfChannel(std::string channelName)
{
	MAP::iterator it = joined.begin();
	while(it != joined.end())
	{
		if(channelName == it->first)
			return 1;
		it++;
	}
	return 0;
}
