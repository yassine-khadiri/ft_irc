/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:39:54 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/03 19:14:57 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Channel;
#define MAP std::map<std::string, Channel>

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
		Client(Client const &src);
		Client &operator=(Client const &src);
		Client(std::string pass, std::string nick, std::string user);
		~Client();
		void		setPassword(std::string pass);
		std::string	getPassword();
		void		setNickname(std::string nick);
		std::string	getNickname();
		void		setUsername(std::string user);
		std::string	getUsername();
		void		setIsRegistered(bool isRegistered);
		bool		getIsRegistered();
		void		setOpPrivilegePermission(int privilege);
		int			getOpPriviligePermission() const;
		void		channelSegment(Channel &_channel);
		void		joinChannel(Channel &_channel);
		void		setFd(int fdsocket);
		int			getFd();
		MAP			joined;
		int			isExistedChannel( std::string channelName );
		int			isMemberOfChannel( std::string nickName, std::string channelName );
};
