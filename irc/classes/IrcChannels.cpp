/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannels.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 20:30:49 by hbouqssi          #+#    #+#             */
/*   Updated: 2023/04/27 20:30:50 by hbouqssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcChannels.hpp"
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

void Channel::setChannelName(std::string &_ChannelName)
{
    this->channelName = _ChannelName;
};

void Channel::setTopic(std::string &_topic)
{
    this->topic = _topic;
};

void Channel::setKey(std::string &_key)
{
    this->key = _key;
};


