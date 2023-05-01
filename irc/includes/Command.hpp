/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:07:12 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/01 01:54:07 by hbouqssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Ircserv.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define channelMap std::map<std::string, Channel>
#define Map		   std::map<int, Client>
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
} e_Command;

class Command
{
	private :
		std::string 				_command;
		std::vector<std::string> 	_args;
		std::vector<std::string>	_basicCommand;
		int							_indexCmd;
		Client						_client;
		std::vector<Client>			_clients;
		std::string					_pass;
		channelMap					_channelMap;
		Map							_users;
	public :
		Command();
		Command(int nbClient,std::string &msg,std::string &pass,std::vector<Client> &clients);
		void						toUpper(std::string &str);
		int							splitParams(std::string msg, std::vector<std::string> &args, std::string &cmd);
		void						initBasicCommand();
		int							findCommand(std::string cmd);
		_iterator					searchForUser(std::string name, int fd);
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
};

// int		splitCommand(std::string str, std::vector<std::string> &v);
// int		is_number(std::string str);