/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:45:18 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/16 19:58:39 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::nickCommand()
{
	std::string hostname = getMachineHostName();
	std::string nickname = (_client.getNickname() == "") ? "*" : _client.getNickname();

	if(this->_client.getPassword() == "")
	{
		sendReply(":" + hostname + " ERROR * : You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 1 || this->_args[0] == "")
		sendReply(":" + hostname + " 431 " + nickname + " : NICK <nickname>\r\n");
	else if(this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
        sendReply(":" + hostname + " 432 " + nickname + " : Error nickname\r\n");
	else if(nickExist(this->_args[0]) != -1)
		sendReply(":" + hostname + " 433 " + nickname + " : Nickname already in use\r\n");
	else
	{
		if (_client.getNickname() == "" && _client.getUsername() != "")
		{
			sendReply(":" + hostname + " 001 " + this->_args[0] + " :Welcome to the Internet Relay Network " + this->_args[0] + "!\r\n");
			sendReply(":" + hostname + " 002 " + this->_args[0] + " :Your host is " + hostname + ", running version 0.1\r\n");
			sendReply(":" + hostname + " 003 " + this->_args[0] + " :This server was created 2019-10-10\r\n");
			sendReply(":" + hostname + " 004 " + this->_args[0] + " :" + hostname + " 0.1\r\n");
			sendReply(":" + hostname + " 251 " + this->_args[0] + " :There are 1 users and 1 server\r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :                        Please enjoy your stay!\r\n");
			sendReply(":" + hostname + " 372 " + this->_args[0] + " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":" + hostname + " 376 " + this->_args[0] + " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");

			this->_client.setIsRegistered(true);
		}
		else if(_client.getIsRegistered())
			communClients(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + hostname + " NICK " + this->_args[0] + "\r\n"); 
		this->_client.setNickname(this->_args[0]);
	}
};
