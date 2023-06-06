/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:44:53 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/06 18:57:29 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::kickCommand()
{
	if (this->_args.size() < 3)
    {
        sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " KICK :Not enough parameters <channel> <nick> [<reason>]\r\n");
        return;
    }
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;

	while (std::getline(channelSplitter, channelName, ','))
    {
		if (this->_client.isMemberOfChannel(channelName, _client.getFd()) == -1)
		{
			std::cout << "case0\n";
			sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName + " :No such channel\r\n");
			continue;
		}
		else
			this->_channelObj = *this->_channelObj._channelMap[this->_args[0]];
		if (!this->_client.isMemberOfChannel(channelName, _client.getFd()))
		{
			std::cout << "case1\n";
			sendReply(":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
		}
		else if(!this->_channelObj._channelMap[channelName]->_userMap[this->_client.getFd()].getOpPriviligePermission())
		{
			std::cout << "case2\n";
			sendReply(":" + getMachineHostName() + " 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator\r\n");
		}
		else if (!this->_client.isMemberOfChannel(this->_args[0], this->searchClientByName(this->_args[1])))
		{
			std::cout << "case3\n";
			sendReply(":" + getMachineHostName() + " 441 " + this->_args[1] + " " + channelName + " :They are not on that channel\r\n");
		}
		else
		{
			std::cout << "case4\n";
			this->broadcast(channelName, ":" + this->_client.getNickname() + "!@" + getMachineHostName() + " KICK " + channelName + " " + this->_args[1] + " " + this->_args[2] + "\r\n");
			_channelObj.removeUserFromUserMap(channelName, searchClientByName(this->_args[1]));
			//kick a user by specifying the reason why ...
		}
	}
};
