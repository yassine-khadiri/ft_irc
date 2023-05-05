/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:39:54 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/05 17:54:35 by ykhadiri         ###   ########.fr       */
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
		int			Operator;
	public :
		Client();
		Client( int fdSocket );
		Client( Client const &src) ;
		Client &operator=( Client const &src );
		Client( std::string pass, std::string nick, std::string user );
		~Client();
		std::string	getPassword();
		std::string	getNickname();
		std::string	getUsername();
		bool		getIsRegistered();
		int			getFd();
		int			getOpPriviligePermission() const;
		void		setPassword( std::string pass );
		void		setNickname( std::string nick );
		void		setUsername( std::string user);
		void		setIsRegistered( bool isRegistered );
		void		setFd( int fdsocket );
		void		setOpPrivilegePermission( int privilege );
		void		channelSegment( Channel &_channel );
		void		joinChannel( Channel &_channel );
		int			isMemberOfChannel( std::string channelName, int clientFd );
};
