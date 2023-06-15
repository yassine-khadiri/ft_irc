/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:07:12 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/06/15 18:39:05 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <ctime>
#include <sstream>
#include <curl/curl.h>
#include "Client.hpp"
#include "Channel.hpp"

class Ircserv;

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
}	e_Command;

class Command
{
		std::string					_pass;
		std::string					_msg;
		std::vector<std::string>	_basicCommand ;
		int							_indexCmd;
		std::string 				_command;
		std::vector<std::string> 	_args;
		Channel						_channelObj;
		Client						_client;
		std::vector<Client>			_clients;
		std::vector<std::string>	modes;
	public :
		Command();
		Command( std::string &pass );
		~Command();
		void						exec( int nbClient,std::string &msg,std::vector<Client> &clients,std::vector<int> &socketClients, int &nbClients);
		void						toUpper( std::string &str );
		int							splitParams( std::string msg, std::vector<std::string> &args, std::string &cmd );
		void						initBasicCommand();
		int							findCommand( std::string cmd );
		std::string 				getCommand() const;
		std::vector<std::string>	getArgs() const;
		Client						&getClient();
		void						setCommand( std::string Command );
		void						setArgs( std::vector<std::string> args );
		void						setClient( Client &client );
		void						sendReply( std::string msg );
		int							nickExist( std::string nick );
		void						passCommand();
		void						userCommand();
		void						nickCommand();
		void						privmsgCommand();
		void						joinCommand();
		void						partCommand();
		void						noticeCommand();
		void						quitCommand();
		int							leaveAllChannels();
		void						kickCommand();
		void						topicCommand();
		void						inviteCommand();
		void						botCommand();
		void						modeCommand();
		void						broadcast( std::string const &channel, std::string const &msg );
		std::string					getCurrentUnixTimestamp();
		int							searchClientByName( std::string clientName );
		int 						modeAnalyzer();
		void						communClients( std::string msg );
		std::string					getMachineHostName();
};

#endif