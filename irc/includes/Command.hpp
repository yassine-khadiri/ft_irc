/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:07:12 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/19 16:35:29 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <ctime>
#include "Ircserv.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// #define channelMap std::map<std::string, Channel>
// #define Map		   std::map<int, Client>
#define _iterator   Map::iterator
typedef enum
{
	USER,
	NICK,
	PASS,
	PRIVMSG,
	NOTICE,
	JOIN,
	PART,
	QUIT,
	MODE,
	PONG,
	KICK,
	TOPIC,
	INVITE,
	BOT,
} e_Command;

class Command
{
		std::string 				_command;
		std::vector<std::string> 	_args;
		std::vector<std::string>	_basicCommand;
		int							_indexCmd;
		Channel						_channelObj;
		Client&						_client;
		std::vector<Client>			&_clients;
		std::vector<std::string>	modes;
		std::string					_pass;
	public :
		Command(int nbClient,std::string &msg,std::string &pass,std::vector<Client> &clients);
		void						toUpper(std::string &str);
		int							splitParams(std::string msg, std::vector<std::string> &args, std::string &cmd);
		void						initBasicCommand();
		int							findCommand(std::string cmd);
		userMap::iterator			searchForUser(std::string name, int fd);
		std::string					joinVectorValues();
		std::string 				getCommand() const;
		std::vector<std::string>	getArgs() const;
		std::string					getPass() const;
		Client						&getClient();
		void						setCommand(std::string Command);
		void						setArgs(std::vector<std::string> args);
		void						setPass(std::string pass);
		void						setClient(Client &client);
		void						sendReply(std::string msg);
		int							nickExist(std::string nick);
		void						passCommand();
		void						userCommand();
		void						nickCommand();
		void						privmsgCommand();
		void						joinCommand();
		void						partCommand();
		void						noticeCommand();
		void						quitCommand();
		void						kickCommand();
		void						topicCommand();
		void						inviteCommand();
		void						botCommand();
		void						modeCommand();
		void						broadcast( std::string const &channel, std::string const &msg );
		std::string					getCurrentUnixTimestamp();
		int							searchClientByName( std::string clientName );
		int 						modeAnalyzer();
};
// int		splitCommand(std::string str, std::vector<std::string> &v);
// int		is_number(std::string str);