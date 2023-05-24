/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:05:30 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 17:06:04 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::userCommand()
{
	if(this->_client.getPassword() == "")
	{
		sendReply(":"+ getMachineHostName() +" ERROR * :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 4)
		sendReply(":"+ getMachineHostName() +" 461 " + _client.getNickname() + ": USER <username> <hostname> <servername> <realname>\r\n");
	else if (this->_client.getUsername() != "" && this->_client.getNickname() != "")
		sendReply(":"+ getMachineHostName() +" 462 " + _client.getNickname() + ":You are already registered\r\n");
	else
	{
		if (_client.getNickname() != "" && _client.getUsername() == "")
		{
			sendReply(":"+ getMachineHostName() +" 001 " + _client.getNickname() + " :Welcome to the Internet Relay Network " + _client.getNickname() + "!\r\n");
			sendReply(":"+ getMachineHostName() +" 002 " + _client.getNickname() + " :Your host is "+ getMachineHostName() +", running version 0.1\r\n");
			sendReply(":"+ getMachineHostName() +" 003 " + _client.getNickname() + " :This server was created 2019-10-10\r\n");
			sendReply(":"+ getMachineHostName() +" 251 " + _client.getNickname() +" :There are 1 users and 1 server\r\n");
			sendReply(":"+ getMachineHostName() +" 004 " + _client.getNickname() + " :"+ getMachineHostName() +" 0.1\r\n");                                                                                 
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :                        Please enjoy your stay!\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":"+ getMachineHostName() +" 376 " + _client.getNickname() +  " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");

			this->_client.setIsRegistered(true);
		}
		this->_client.setUsername(this->_args[0]);
	}
};
