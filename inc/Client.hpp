/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:39:54 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/06/13 14:26:00 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Channel;

class Client
{
		std::string	_password;
		std::string	_nickname;
		std::string	_username;
		bool		_isRegistered;
		int			_fdsocket;
		int			_privilige;
	public:
		Client();
		Client( int fdSocket );
		Client( Client const &src );
		Client &operator=( Client const &src );
		Client( std::string pass, std::string nick, std::string user );
		~Client();
		std::string	getPassword() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		bool		getIsRegistered() const;
		int			getFd() const;
		int			getOpPriviligePermission() const;
		void		setPassword( std::string pass );
		void		setNickname( std::string nick );
		void		setUsername( std::string user);
		void		setIsRegistered( bool isRegistered );
		void		setFd( int fdsocket );
		void		setOpPrivilegePermission( int privilege );
		int			isMemberOfChannel( std::string channelName, int clientFd );
};
