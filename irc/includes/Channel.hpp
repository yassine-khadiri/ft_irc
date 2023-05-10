/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 17:55:40 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/10 17:47:14 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define channelMap std::map<std::string, Channel>
#define userMap std::map<int, Client>
#define OPERATOR 1
#define CLIENT 0

#include "../../tcp/includes/Tcp.hpp"
#include "Client.hpp"

class Channel
{
        std::string         _channelName;
        std::string         _topic;
        std::string         _topicTime;
        std::string         _key;
        Client              _operator;
    public:
        userMap              _userMap;
        static channelMap   _channelMap;

		// ############################################################### //

        Channel();
        Channel( std::string channelName, std::string _topic, std::string key, Client _operator);
        ~Channel();
        std::string getChannelName() const;
        std::string getTopic() const;
        std::string getTopicTime();
        std::string getKey() const;
        userMap getUserMap() const;
        std::string	usersList() const;
        void setChannelName( std::string _channelName );
        void setTopic( std::string _topic );
        void setTopicTime( std::string _topicTime );
        void setKey( std::string _key );
        void joinChannel();
        int  verifyKey( std::string &key )const;
        void addUserToChannelMap( Client &_client, int Privilege );
        int removeUserFromUserMap( std::string channelName, int clientFd );
        // void removeUser( Client &_client );
        // int channelFound( std::string channelName );
};

#endif