/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 12:07:29 by hbouqssi          #+#    #+#             */
/*   Updated: 2023/04/28 16:42:19 by hbouqssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel()
{
    this->channelName = "";
    this->topic = "";
    this->key = "";
};

Channel::~Channel()
{

};

std::string Channel::getChannelName() const
{
    return this->channelName;
};

std::string Channel::getTopic() const
{
    return this->topic;
};

std::string Channel::getKey() const
{
    return this->key;
};

void Channel::setChannelName(std::string _ChannelName)
{
    this->channelName = _ChannelName;
};

void Channel::setTopic(std::string _topic)
{
    this->topic = _topic;
};

void Channel::setKey(std::string _key)
{
    this->key = _key;
};

void Channel::addUser(Client &_client, int privilege)
{
    users.insert(std::make_pair(_client.getFd(), _client));
    _client.joinChannel(*this);
    _client.setOpPrivilegePermission(privilege ? OPERATOR : CLIENT);
    
}
void Channel::removeUser(Client &_client)
{
    users.erase(_client.getFd());
    _client.channelSegment(*this);
}
