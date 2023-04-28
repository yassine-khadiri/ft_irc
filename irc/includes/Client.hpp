/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:39:54 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/20 18:13:50 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <string>
#include <vector>

class Client
{
	private :
		std::string	_password;
		std::string	_nickname;
		std::string	_username;
		bool		_isRegistered;
		int			_fdsocket;
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
		// void		eraseClient();
		// void		joinChannel(std::string channel);
		void		setFd(int fdsocket);
		int			getFd();
};
