/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 12:07:29 by hbouqssi          #+#    #+#             */
/*   Updated: 2023/06/13 16:05:13 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

channelMap Channel::_channelMap;

Channel::Channel() : _channelName(""), _topic(""),_limitUsers(0), _key("")
{
};

Channel::Channel( Channel const &channel )
{
    *this = channel;
};

Channel::Channel( std::string _channelName, std::string _topic, std::string _key, Client _member )
{
    this->_channelName = _channelName;
    this->_topic = _topic;
    this->_key = _key;
    this->_member = _member;
    this->_limitUsers = 0;
};

Channel&  Channel::operator=( Channel const & channel )
{
    this->_channelName = channel._channelName;
    this->_topic = channel._topic ;
    this->_modes = channel._modes;
    this->_topicTime = channel._topicTime;
    this->_channelCreationTime = channel._channelCreationTime;
    this->_key = channel._key;
    this->_invitedUsers = channel._invitedUsers;
    this->_operator = channel._operator;
    this->_member = channel._member;
    this->_userMap = channel._userMap;
    return *this;
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

std::string Channel::getModes() const
{
    int i = -1;
    std::string mode = "+";

    while (++i < (int)this->_modes.size())
        mode +=  this->_modes[i];
    if (_limitUsers)
    {
        std::stringstream ss;
        ss << _limitUsers;
        mode += " " + ss.str();
    }
    return mode;
};

int Channel::findMode( std::string mode )
{
    std::vector<char>::iterator it = std::find(this->_modes.begin(), this->_modes.end(), mode[1]);

    if (it == this->_modes.end())
        return 0;
    return 1;
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

std::vector<std::string> Channel::getInvitedUsers() const
{
    return this->_invitedUsers;
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
    std::vector<char>::iterator it = std::find(this->_modes.begin(), this->_modes.end(), _mode[1]);

    if (_mode[0] == '-')
    {
        if (it != this->_modes.end())
            this->_modes.erase(it);
    }
    else
    {
        if (it == this->_modes.end())
            this->_modes.push_back(_mode[1]);
    }
};

int Channel::getLimitUsers() const
{
    return _limitUsers;
};

void Channel::setLimitUsers( int limit )
{
    _limitUsers = limit;
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

void Channel::setInvitedUsers( std::string userName )
{
    this->_invitedUsers.push_back(userName);
};

int Channel::isAnInvitedUser( std::string userName )
{
    std::vector<std::string>::iterator it = std::find(this->_invitedUsers.begin(), this->_invitedUsers.end(), userName);

    return (it != this->_invitedUsers.end()) ? 1 : 0;
};

void Channel::removeInvitedUser(std::string userName)
{
    this->_invitedUsers.erase(std::remove(this->_invitedUsers.begin(), this->_invitedUsers.end(), userName), this->_invitedUsers.end());
};

void Channel::addChannelToChannelMap()
{
	this->_channelMap[this->getChannelName()] = new Channel(*this);
};

int Channel::verifyKey( std::string &_key ) const
{
    return (this->_key == _key) ? 1 : 0;
};

void Channel::addUserToUserMap( Client &_client, int privilege )
{
    this->_userMap.insert(std::make_pair(_client.getFd(), _client));
    this->_userMap[_client.getFd()].setOpPrivilegePermission(privilege);
    _client.setOpPrivilegePermission(privilege);
    if (privilege)
        this->_operator = _client;
};

int Channel::removeUserFromUserMap( std::string channelName, int clientFd )
{
	channelMap::iterator it = this->_channelMap.find(channelName);

	if (it != this->_channelMap.end())
	{
		it->second->_userMap.erase(clientFd);
        if (this->_channelMap[channelName]->_userMap.size() == 0)
        {
            delete this->_channelMap[channelName];
            this->_channelMap.erase(channelName);
            this->_operator = NULL;
        }
		return 1;
	}
	return 0;
};

std::string Channel::usersList() const
{
    std::string userList = ":",
                userOper = "";
    userMap::const_iterator it;

    for (it = this->_userMap.begin(); it != this->_userMap.end(); ++it)
    {
        if (it->second.getOpPriviligePermission())
            userOper = it->second.getNickname();
        else
            userList += it->second.getNickname() + " ";
    }
    if (!userOper.empty())
        userList += "@" + userOper;

    return userList;
};
