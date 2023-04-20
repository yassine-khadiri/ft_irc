/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:07:12 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/20 22:21:45 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Client.hpp"
#include "Ircserv.hpp"

typedef enum
{
	USER,
	NICK,
	PASS,
	PRIVMSG,
	JOIN,
	PART,
	QUIT,
	MSG,
} e_Command;


class Command
{
	private :
		std::string 				_command;
		std::vector<std::string> 	_args;
		std::vector<std::string>	_basicCommand;
		int							_indexCmd;
		Client						_client;
		std::string					_pass;
	public :
		Command();
		Command(Client &client,std::string &msg,std::string &pass);
		void						toUpper(std::string &str);
		int							splitParams(std::string msg, std::vector<std::string> &args, std::string &cmd);
		void						initBasicCommand();
		int							findCommand(std::string cmd);
		std::string 				getCommand() const;
		std::vector<std::string>	getArgs() const;
		std::string					getPass() const;
		Client						&getClient();
		void						setCommand(std::string Command);
		void						setArgs(std::vector<std::string> args);
		void						setPass(std::string pass);
		void						setClient(Client &client);	
		void						passCommand();
};


// int		splitCommand(std::string str, std::vector<std::string> &v);
// int		is_number(std::string str);