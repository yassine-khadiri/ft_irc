/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 17:55:40 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/23 18:08:09 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../../tcp/includes/Tcp.hpp"

class Channel
{
        std::string channelName;
        std::string topic;
        std::string key;
    public:
        Channel();
        ~Channel();
        std::string getChannelName() const;
        std::string getTopic() const;
        std::string getKey() const;
        void setChannelName(std::string _channelName);
        void setTopic(std::string _topic);
        void setKey(std::string _key);  
};

#endif