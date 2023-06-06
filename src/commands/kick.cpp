/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:44:53 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/06 14:33:44 by rgatnaou         ###   ########.fr       */
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
			sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName + " :No such channel\r\n");
			continue;
		}
		else
			this->_channelObj = *this->_channelObj._channelMap[this->_args[0]];
		if (!this->_client.isMemberOfChannel(channelName, _client.getFd()))
			sendReply(":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
		else if(this->_channelObj._userMap[this->_client.getFd()].getOpPriviligePermission())
			sendReply(":" + getMachineHostName() + " 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator");
		else if (!this->_client.isMemberOfChannel(this->_args[0], this->searchClientByName(this->_args[1])))
			sendReply(":" + getMachineHostName() + " 441 " + this->_args[1] + " " + channelName + " :They are not on that channel");
		else
		{
			std::cout << "kick" << std::endl;
			this->broadcast(channelName,":" + this->_client.getNickname() + "!@" + getMachineHostName() + " KICK " + channelName + " " + this->_args[1] + " " + this->_args[2] + "\r\n");
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
			//kick a user by specifying the reason why ...
		}																			
		this->_channelObj._channelMap[this->_args[0]] = &this->_channelObj ;
	}
};
