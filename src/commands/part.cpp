/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:45:30 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 13:07:54 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::partCommand()
{
	// For NetCat Tests
	if (!this->_args.size())
	{
		// ERR_NEEDMOREPARAMS (461)
        sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " PART :Not enough parameters\r\n");
		return;
	}
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;

	//####################################################=====> CHECK THE ARGUMENTS | if the user typed just KICK or if he typed a lot of Params !!!!!!!!

	while (std::getline(channelSplitter, channelName, ','))
    {
		// Parting A Channel You’re Not Joined To: (ERR_NOTONCHANNEL (442))
		if (!this->getClient().isMemberOfChannel(channelName, _client.getFd()))
			message = ":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
		{
			// Parting A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
			if (channelName[0] == ':' && !channelName[1])
				message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " * No such channel\r\n";
			else
				message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName.substr(1) + " :No such channel\r\n";
			sendReply(message);
		}			
		else
		{
			// Parting With A Reason:
			if (this->_args[1][0] == ':')
				message = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " PART " + channelName + " " + this->_args[1] + "\r\n";
			// Parting With No Reason (Already Joined!):
			else
				message = ":" + this->_client.getNickname() + "!@" + getMachineHostName() + " PART " + channelName + "\r\n";
			broadcast(channelName, message);
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
		}																			
	}
};
