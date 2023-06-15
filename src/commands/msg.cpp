/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:47:38 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/15 17:35:32 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void	Command::privmsgCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + ": PRIVMSG <nick/channel> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == - 1)
		sendReply(":" + getMachineHostName() + " 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":" + getMachineHostName() + " 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " PRIVMSG " + this->_args[0] + " " + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0], tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);
		}
	}
};

void Command::noticeCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + ": NOTICE <nickname> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
		sendReply(":" + getMachineHostName() + " 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":" + getMachineHostName() + " 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + "" + " NOTICE " + this->_args[0] + " :" + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0],tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);	
		}
	}
};
