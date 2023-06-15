/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:42:27 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/06/15 17:29:16 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"

Client::Client()
{	
};

Client::Client( int fdSocket ): _password(""), _nickname(""), _username(""), _isRegistered(false), _fdsocket(fdSocket), _privilige(0)
{
};

Client::Client(std::string pass, std::string nick, std::string user): _password(pass), _nickname(nick), _username(user), _isRegistered(true)
{
};

Client::Client(Client const &src)
{
	*this = src;
};

Client& Client::operator=(Client const &src)
{
	if (this != &src)
	{
		this->_password = src._password;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_isRegistered = src._isRegistered;
		this->_fdsocket = src._fdsocket;
		this->_privilige = src._privilige;
	}
	return (*this);
};

Client::~Client()
{
};

void Client::setPassword( std::string pass )
{
	this->_password = pass;
};

std::string	Client::getPassword() const
{
	return (this->_password);
};

void Client::setNickname( std::string nick )
{
	this->_nickname = nick;
};

std::string	Client::getNickname() const
{
	return (this->_nickname);
};

void Client::setUsername( std::string user )
{
	this->_username = user;
};

std::string	Client::getUsername() const
{
	return (this->_username);
};

void Client::setIsRegistered( bool isRegistered )
{
	this->_isRegistered = isRegistered;
	if (isRegistered == true)
		std::cout << "Welcome: " << this->_nickname << " To Our IRC Server!" << std::endl;
	
};

bool Client::getIsRegistered() const
{
	return (this->_isRegistered);
};

void Client::setFd( int fdsocket )
{
	this->_fdsocket = fdsocket;
};

int Client::getFd() const
{
	return (this->_fdsocket);
};

void Client::setOpPrivilegePermission( int privilige )
{
	this->_privilige = privilige;
};

int Client::getOpPriviligePermission() const
{
	return(this->_privilige);
};

int Client::isMemberOfChannel( std::string channelName, int clientFd )
{
	Channel channelObj;

	channelMap::iterator it1 = channelObj._channelMap.find(channelName);
	if (it1 != channelObj._channelMap.end())
	{
		userMap::iterator it2 = it1->second->_userMap.find(clientFd);
		if (it2 != it1->second->_userMap.end())
			return 1;
	}
	else
		return -1; // channel not found
	return 0; // channel found but the client doesn't belong to this channel
};
