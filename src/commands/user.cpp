/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:46:32 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/16 19:59:20 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::userCommand()
{
	std::string nickname = (_client.getNickname() == "") ? "*" : _client.getNickname();
	
	if(this->_client.getPassword().empty())
	{
		sendReply(":" + getMachineHostName() + " ERROR * :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 4)
		sendReply(":" + getMachineHostName() + " 461 " + nickname + " :USER <username> <hostname> <servername> <realname>\r\n");
	else if(this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
        sendReply(":" + getMachineHostName() + " 432 " + nickname + " : error username\r\n");
	else if (this->_client.getUsername() != "" && this->_client.getNickname() != "")
		sendReply(":" + getMachineHostName() + " 462 " + nickname + " :You are already registered\r\n");
	else
	{
		if (_client.getNickname() != "" && _client.getUsername() == "")
		{
			sendReply(":" + getMachineHostName() + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!\r\n");
			sendReply(":" + getMachineHostName() + " 002 " + nickname + " :Your host is " + getMachineHostName() + ", running version 0.1\r\n");
			sendReply(":" + getMachineHostName() + " 003 " + nickname + " :This server was created 2019-10-10\r\n");
			sendReply(":" + getMachineHostName() + " 251 " + nickname + " :There are 1 users and 1 server\r\n");
			sendReply(":" + getMachineHostName() + " 004 " + nickname + " :" + getMachineHostName() + " 0.1\r\n");                                                                                 
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :                        Please enjoy your stay!\r\n");
			sendReply(":" + getMachineHostName() + " 372 " + nickname + " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":" + getMachineHostName() + " 376 " + nickname + " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");
			this->_client.setIsRegistered(true);
		}
		this->_client.setUsername(this->_args[0]); 
	}
};
