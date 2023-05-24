/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:18:09 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 17:24:40 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::communClients( std::vector<int>&clients )
{
	channelMap::iterator it1 = this->_channelObj._channelMap.begin();
	// std::vector<int> clients;

	while (it1 != this->_channelObj._channelMap.end())
	{
		this->_channelObj = this->_channelObj._channelMap[it1->first];
		if (this->_channelObj._userMap[_client.getFd()].isMemberOfChannel(it1->first,_client.getFd()) == 1)
		{
			userMap::iterator it2 = this->_channelObj._userMap.begin();
			while (it2 != this->_channelObj._userMap.end())
			{
				if (!std::count (clients.begin(), clients.end(), it2->first))
					clients.push_back(_client.getFd());
				it2++;
			}
		}
		it1++;
	}
	// return clients;
};

void sendReplyToCommunClient(std::vector<int> &clients, std::string msg)
{
	std::vector<int>::iterator it = clients.begin();

	while (it != clients.end())
	{
		send(*it, msg.c_str(), msg.length(), 0);
		it++;
	}
};

void Command::quitCommand()
{
	std::string msg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " QUIT " + this->_args[0] + "\r\n";
	std::vector<int> clients;
    communClients(clients);

	leaveAllChannels();
	sendReplyToCommunClient(clients,msg);
	_client.setIsRegistered(false);
	_client.setNickname("");
	_client.setPassword("");
	_client.setUsername("");
};
