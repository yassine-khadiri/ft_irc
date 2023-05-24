/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:04:42 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 17:31:25 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::nickCommand()
{
	std::string hostname = getMachineHostName();

	if(this->_client.getPassword() == "")
	{
		sendReply(":"+ getMachineHostName() +" ERROR * :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 1 || this->_args[0] == "")
		sendReply(":"+ getMachineHostName() +" 431 " + _client.getNickname() + ": NICK <nickname>\r\n");
	else if (this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendReply(":"+ getMachineHostName() +" 432 " + _client.getNickname() + ": Nickname invalid\r\n");
	else if(nickExist(this->_args[0]) != -1)
		sendReply(":"+ getMachineHostName() +" 433 " + _client.getNickname()+ " Nickname already in use\r\n");
	else
	{
		if (_client.getNickname() == "" && _client.getUsername() != "")
		{
			sendReply(":"+ hostname + " 001 " + this->_args[0] + " :Welcome to the Internet Relay Network " + this->_args[0] + "!\r\n");
			sendReply(":"+ hostname + " 002 " + this->_args[0] + " :Your host is "+ getMachineHostName() +", running version 0.1\r\n");
			sendReply(":"+ hostname + " 003 " + this->_args[0] + " :This server was created 2019-10-10\r\n");
			sendReply(":"+ hostname + " 004 " + this->_args[0] + " :"+ getMachineHostName() +" 0.1\r\n");
			sendReply(":"+ hostname + " 251 " + this->_args[0] + " :There are 1 users and 1 server\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":"+ hostname + " 372 " + this->_args[0] + " :                        Please enjoy your stay!\r\n");
			sendReply(":"+ hostname + " 372 " + this->_args[0] + " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":"+ hostname + " 376 " + this->_args[0] + " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");
			this->_client.setIsRegistered(true);
		}
		else if(_client.getNickname() != "" && _client.getUsername() != "")
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@"+ getMachineHostName() +"" + " NICK " + this->_args[0] + "\r\n"); 
		this->_client.setNickname(this->_args[0]); 
	}
};
