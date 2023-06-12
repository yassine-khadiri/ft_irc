/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:44:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/12 15:26:45 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

std::string getChannelKey(const std::vector<std::string>& channelKeys, int channelMapSize)
{
    std::string chKey = "";

    if (channelKeys.size() > 0)
    {
        int index = channelMapSize % channelKeys.size();
        chKey = channelKeys[index];
    }
    return chKey;
};

void Command::joinCommand() 
{
    if (this->_args.size() <= 1)
    {
		if (this->_args.size() == 1 && this->_args[0] == "0")
		{
			if (!leaveAllChannels())
				sendReply(":" + getMachineHostName() + " 476 " + _client.getNickname() + " 0 :Invalid channel name\r\n");
			return;
		}
		else if (this->_args.size() < 1)
		{	
			sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " JOIN :Not enough parameters\r\n");
			return ;
		}
    }
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;
    std::vector<std::string> channelKeys;

    if (this->_args.size() > 1)
    {
        std::stringstream keySplitter(this->_args[1]);
        std::string key;

        while (std::getline(keySplitter, key, ','))
            channelKeys.push_back(key);
    }

    while (std::getline(channelSplitter, channelName, ','))
    {
        if (channelName.empty() || channelName[0] != '#')
        {
            sendReply(":" + getMachineHostName() + " 476 " + channelName + " :Invalid channel name\r\n");
            continue;
        }
		if (this->_client.isMemberOfChannel(channelName, this->_client.getFd()) == -1)
		{
			this->_channelObj = Channel(channelName, "", getChannelKey(channelKeys, this->_channelObj._channelMap.size()), _client);
			this->_channelObj.addUserToUserMap(_client, OPERATOR);
			this->_channelObj.setChannelCreationTime(this->getCurrentUnixTimestamp());
			this->_channelObj.setMode("+n");
			this->_channelObj.setMode("+t");
			if (!getChannelKey(channelKeys, this->_channelObj._channelMap.size()).empty())
				this->_channelObj.setMode("+k");
			this->_channelObj.addChannelToChannelMap();
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");
			sendReply(":" + getMachineHostName() + " MODE " + channelName + " " + this->_channelObj.getModes() + "\r\n");
			sendReply(":" + getMachineHostName() + " 353 " + _client.getNickname() + " = " + channelName + " :@" + this->_client.getNickname() + "\r\n");
			sendReply(":" + getMachineHostName() + " 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
		}
		else if (!this->_client.isMemberOfChannel(channelName, this->_client.getFd())) // 0
		{
			this->_channelObj = *this->_channelObj._channelMap[channelName];
			std::cout << "Invited for cha2: " << this->_channelObj.isAnInvitedUser(this->_client.getNickname()) << std::endl;
			std::cout << "Is invited: " << this->_channelObj.isAnInvitedUser(this->_client.getNickname()) << std::endl;
			if (this->_channelObj.getModes().find("i") != std::string::npos && !this->_channelObj.isAnInvitedUser(this->_client.getNickname()))
				sendReply(":" + getMachineHostName() + " 473 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n"); //ERR_INVITEONLYCHAN (473)
			else if (this->_channelObj.getModes().find("l") != std::string::npos && (int)this->_channelObj._userMap.size() >= this->_channelObj.getLimitUsers())
				sendReply(":" + getMachineHostName() + " 471 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n"); //ERR_CHANNELISFULL (471)
			else
			{
				std::string  checkKey = getChannelKey(channelKeys, this->_channelObj._channelMap.size());

				if (!this->_channelObj.verifyKey(checkKey))
				{
					sendReply(":" + getMachineHostName() + " 475 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel (+k) - bad key\r\n");
					return ;
				}
				this->_channelObj.addUserToUserMap(_client, CLIENT);
				if (this->_channelObj.isAnInvitedUser(this->_client.getNickname()))
					this->_channelObj.removeInvitedUser(this->_client.getNickname());
				this->_channelObj._channelMap[channelName] = &this->_channelObj;
				// sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");

				this->broadcast(channelName, ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");
				sendReply(":" + getMachineHostName() + " 353 " + _client.getNickname() + " @ " + channelName + " " + this->_channelObj.usersList() + "\r\n");
				sendReply(":" + getMachineHostName() + " 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
			}
		}
    }
};
