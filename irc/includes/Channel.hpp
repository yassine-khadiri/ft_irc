/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 17:55:40 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/28 16:43:17 by hbouqssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../../tcp/includes/Tcp.hpp"
#include "Client.hpp"
class Client;
#define USERMAP std::map<int, Client>
#define OPERATOR 1
#define CLIENT  0

class Channel
{
        std::string channelName;
        std::string topic;
        std::string key;
        USERMAP       users;
    public:
        Channel();
        ~Channel();
        std::string getChannelName() const;
        std::string getTopic() const;
        std::string getKey() const;
        void setChannelName(std::string _channelName);
        void setTopic(std::string _topic);
        void setKey(std::string _key);
        void addUser(Client &_client, int Privilege);
        void removeUser(Client &_client);
        
};

#endif