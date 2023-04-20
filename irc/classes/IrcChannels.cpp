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

