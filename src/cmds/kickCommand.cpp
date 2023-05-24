/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:48:05 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 17:00:48 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::kickCommand()
{
	if (this->_args.size() < 3)
    {
        sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;

		if(this->_client.getOpPriviligePermission() == CLIENT)
			sendReply(":" + getMachineHostName() + " 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator");
	while (std::getline(channelSplitter, channelName, ','))
    {
		if (!this->getClient().isMemberOfChannel(channelName, _client.getFd()))
			message = ":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
			message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName + " :No such channel\r\n";
		else
		{
			message = ":" + this->_client.getNickname() + "!@" + getMachineHostName() + " PART " + channelName + "\r\n";
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
			//kick a user by specifying the reason why ...
		}																			
		sendReply(message);
	}
};
