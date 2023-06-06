/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:44:32 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/06 13:48:46 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::inviteCommand()
{
	if (this->_args.size() == 1)
		sendReply(":" + getMachineHostName() + " 461 " + this->_client.getNickname() + " " + this->_args[1] + " :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
	else if (this->_args.size() == 2)
	{
		if (this->_client.isMemberOfChannel(this->_args[1], this->_client.getFd()) == -1)
			sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[1] + " :No such channel\r\n"); //ERR_NOSUCHCHANNEL (403)
		else if (!this->_client.isMemberOfChannel(this->_args[1], this->_client.getFd()))
			sendReply(":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not on that channel\r\n"); //ERR_NOTONCHANNEL (442)
		else if (this->searchClientByName(this->_args[0]))
		{
			if (this->_client.isMemberOfChannel(this->_args[1], this->searchClientByName(this->_args[0])) == 1)
				sendReply(":" + getMachineHostName() + " 443 " + this->_client.getNickname() + " " + this->_args[1] + " :is already on channel\r\n"); //ERR_USERONCHANNEL (443)
			else if (!this->_client.isMemberOfChannel(this->_args[1], this->searchClientByName(this->_args[0])))
			{
				if (this->_channelObj._channelMap[this->_args[1]]->findMode("+i") && this->_client.getNickname() != this->_channelObj.getOperator().getNickname())
					sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not channel operator\r\n"); //ERR_CHANOPRIVSNEEDED (482)
				else
					sendReply(":" + _client.getNickname() + "!" + this->_args[0] + "@" + getMachineHostName() + " INVITE " + this->_args[0] + " " + this->_args[1] + "\r\n"); //RPL_INVITING (341)
			}
		}
	}
};
