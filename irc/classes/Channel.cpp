/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 12:07:29 by hbouqssi          #+#    #+#             */
/*   Updated: 2023/05/11 18:51:20 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

channelMap Channel::_channelMap;

Channel::Channel() : _channelName(""), _topic(""), _key("")
{
};

Channel::Channel( std::string _channelName, std::string _topic, std::string _key, Client _operator )
{
    this->_channelName = _channelName;
    this->_topic = _topic;
    this->_key = _key;
    this->_operator = _operator;
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

void Channel::joinChannel()
{
	this->_channelMap.insert(std::make_pair(this->getChannelName(), *this));
};

int Channel::verifyKey( std::string &_key ) const
{
   return (this->_key == _key) ? 1 : 0;
};

void Channel::addUserToChannelMap( Client &_client, int privilege )
{
    this->_userMap.insert(std::make_pair(_client.getFd(), _client));
    // userMap::iterator it = this->_userMap.begin();
    // while (it != this->_userMap.end())
    // {
    //     std::cout << "fd: " << it->first << std::endl;
    //     std::cout << "Clinet Name: " << it->second.getNickname() << std::endl;
    //     ++it;
    // }
    // std::cout << "Users Map Size: " << this->_userMap.size() << std::endl;
    _client.setOpPrivilegePermission(privilege ? OPERATOR : CLIENT);
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
	std::string userslist = ":";
	userMap::const_iterator it = _userMap.begin();
    while(it != _userMap.end())
    {
        if(it->second.getNickname() == _operator.getNickname())
            userslist += "@" + it->second.getNickname() + " ";
        else
            userslist += it->second.getNickname() + " ";
    }
    userslist += "\r\n";
    return userslist;
}
