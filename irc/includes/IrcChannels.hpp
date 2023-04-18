#ifndef CHANNELS_HPP
#define CHANNELS_HPP
#include "../../tcp/includes/Tcp.hpp"
class Channel
{

    std::string channelName;
    std::string topic;
    std::string key;
    public:
    Channel();
    ~Channel();
    
    //
    std::string getChannelName() const;
    std::string getTopic() const;
    std::string getKey() const;

    void setChannelName(std::string &_channelName);
    void setTopic(std::string &_topic);
    void setKey(std::string &_key);
};

#endif