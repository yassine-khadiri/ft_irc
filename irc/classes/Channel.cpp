/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 12:07:29 by hbouqssi          #+#    #+#             */
/*   Updated: 2023/05/15 18:46:45 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

channelMap Channel::_channelMap;

Channel::Channel() : _channelName(""), _topic(""), _key("")
{
};

Channel::Channel( std::string _channelName, std::string _topic, std::string _key, Client _member )
{
    this->_channelName = _channelName;
    this->_topic = _topic;
    this->_key = _key;
    this->_member = _member;
};

Channel::~Channel()
{
};

std::string Channel::getChannelName() const
{
    return this->_channelName;
};

std::string Channel::getTopic() const
{
    return this->_topic;
};

std::string Channel::getMode() const
{
    return this->_mode;
};

std::string Channel::getTopicTime() const
{
    return this->_topicTime;  
};

std::string Channel::getChannelCreationTime() const
{
	return this->_channelCreationTime;
};

std::string Channel::getKey() const
{
    return this->_key;
};

Client Channel::getOperator() const
{
    return this->_operator;
};

userMap Channel::getUserMap() const
{
    return this->_userMap;
};

void Channel::setChannelName( std::string _ChannelName )
{
    this->_channelName = _ChannelName;
};

void Channel::setTopic( std::string _topic )
{
    this->_topic = _topic;
};

void Channel::setMode( std::string _mode )
{
	this->_mode = _mode;
};

void Channel::setTopicTime( std::string _topicTime )
{
    this->_topicTime = _topicTime;
};

void Channel::setChannelCreationTime( std::string _channelCreationTime )
{
	this->_channelCreationTime = _channelCreationTime;
};

void Channel::setKey( std::string _key )
{
    this->_key = _key;
};

void Channel::addChannelToChannelMap()
{
	this->_channelMap.insert(std::make_pair(this->getChannelName(), *this));
};

int Channel::verifyKey( std::string &_key ) const
{
   return (this->_key == _key) ? 1 : 0;
};

void Channel::addUserToUserMap( Client &_client, int privilege )
{
    this->_channelMap[this->getChannelName()]._userMap.insert(std::make_pair(_client.getFd(), _client));
    
    _client.setOpPrivilegePermission(privilege);
    if (privilege)
        this->_operator = _client;
};

int Channel::removeUserFromUserMap( std::string channelName, int clientFd )
{
	channelMap::iterator it = this->_channelMap.find(channelName);

	if (it != this->_channelMap.end())
	{
		it->second._userMap.erase(clientFd);
		return 1;
	}
	return 0;
};

// void Channel::removeUser( Client &_client )
// {
//     this->_userMap.erase(_client.getFd());
//     _client.channelSegment(*this);
// };

// int Channel::channelFound( std::string channelName )
// {
// 	channelMap::iterator it = this->_channelMap.begin();

// 	while(it != this->_channelMap.end())
// 	{
// 		// std::cout << "key: " << it->first << std::endl;
// 		// std::cout <<  "value: " << it->second.getChannelName() << std::endl;
// 		if (channelName == it->first)
// 			return std::distance(this->_channelMap.begin(), it);
// 		it++;
// 	}
// 	return -1;
// };

std::string Channel::usersList() const
{
    std::string userList = ":",
                userOper;
    userMap::const_iterator it;

    for (it = this->_channelMap[this->getChannelName()]._userMap.begin(); it != this->_channelMap[this->getChannelName()]._userMap.end(); ++it)
    {
        if (it->second.getOpPriviligePermission())
            userOper = it->second.getNickname();
        else
            userList += it->second.getNickname() + " ";
    }
    userList += "@" + userOper;
    std::cout << userList << std::endl;
    return userList;
}
