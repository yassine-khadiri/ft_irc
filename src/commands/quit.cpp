/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:45:45 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/09 18:46:50 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::communClients( std::string msg )
{
	channelMap::iterator it1 = this->_channelObj._channelMap.begin();
	// std::vector<int> clients;

	while (it1 != this->_channelObj._channelMap.end())
	{
		this->_channelObj = *this->_channelObj._channelMap[it1->first];
		if (this->_channelObj._userMap[_client.getFd()].isMemberOfChannel(it1->first,_client.getFd()) == 1)
			broadcast(this->_channelObj.getChannelName(), msg);
		it1++;
	}
	// return clients;
};



void Command::quitCommand()
{
	std::string msg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " QUIT " + this->_args[0] + "\r\n";
    communClients(msg);

	leaveAllChannels();
	_client.setIsRegistered(false);
	_client.setNickname("");
	_client.setPassword("");
	_client.setUsername("");
};
