/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:46:24 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 17:33:22 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::topicCommand()
{
	// Set A Topic To A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
	{
		sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[0] + " No such channel\r\n");
		return;
	}
	// Setting A Topic
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == 1
		&& this->_args.size() > 1)
	{
		if (!this->_channelObj._channelMap[this->_args[0]]->_userMap[_client.getFd()].getOpPriviligePermission() && this->_channelObj._channelMap[this->_args[0]]->findMode("+t"))
		{
			sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not channel operator\r\n"); //ERR_CHANOPRIVSNEEDED (482)
			return;
		}
		// Add The Topic To The Channel Map
		std::string topic;
		if (this->_args[1].length() == 1 && this->_args[1][0] == ':')
			topic = ":";
		else
			topic = this->_args[1];
		_channelObj._channelMap[this->_args[0]]->setTopic(topic);
		_channelObj._channelMap[this->_args[0]]->setTopicTime(this->getCurrentUnixTimestamp());
		sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " TOPIC " + this->_args[0] + " " + this->_args[1] + "\r\n");
	}
	else if (this->_args.size() == 1)
	{
		if (_channelObj._channelMap[this->_args[0]]->getTopic().empty())
			sendReply(":" + getMachineHostName() + " 331 " + this->_client.getNickname() + " " + this->_args[0] + " :No topic is set\r\n");
		else
		{
			// RPL_TOPIC (332)
			sendReply(":" + getMachineHostName() + " 332 " + this->_client.getNickname() + " " + this->_args[0] + " " + _channelObj._channelMap[this->_args[0]]->getTopic() + "\r\n");
			// RPL_TOPICTIME (333)
			sendReply(":" + getMachineHostName() + " 333 " + this->_client.getNickname() + " " + this->_args[0] + " " + this->_client.getNickname() + " " + _channelObj._channelMap[this->_args[0]]->getTopicTime() + "\r\n");
		}
	}
};
