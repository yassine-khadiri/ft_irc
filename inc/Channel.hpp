/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 17:55:40 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/09 15:46:10 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define channelMap std::map<std::string, Channel* >
#define userMap std::map<int, Client >
#define OPERATOR 1
#define CLIENT 0

#include "Tcp.hpp"
#include "Client.hpp"

class Channel
{
        std::string                 _channelName;
        std::string                 _topic;
        std::vector<char>           _modes;
        int                         _limitUsers;
        std::string                 _topicTime;
        std::string                 _channelCreationTime;
        std::string                 _key;
        Client                      _operator;
        Client                      _member;
    public:
        userMap                     _userMap;
        static channelMap           _channelMap;

		// ############################################################### //

        Channel();
        Channel( std::string channelName, std::string _topic, std::string key, Client _member);
        Channel& operator=( Channel const &channel );
        Channel( Channel const &channel );
        ~Channel();
        int getLimitUsers() const ;
        void setLimitUsers(int limit);
        std::string getChannelName() const;
        std::string getTopic() const;
        std::string getModes() const;
        int         findMode( std::string mode );
        std::string getTopicTime() const;
        std::string getChannelCreationTime() const;
        std::string getKey() const;
        Client getOperator() const;
        userMap getUserMap() const;
        std::string	usersList() const;
        void setChannelName( std::string _channelName );
        void setTopic( std::string _topic );
        void setMode( std::string _mode );
        void setTopicTime( std::string _topicTime );
        void setChannelCreationTime( std::string _channelCreationTime );
        void setKey( std::string _key );
        void addChannelToChannelMap();
        int  verifyKey( std::string &key )const;
        void addUserToUserMap( Client &_client, int Privilege );
        int removeUserFromUserMap( std::string channelName, int clientFd );
};

#endif