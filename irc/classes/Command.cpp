/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:11:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/20 18:06:50 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

int   Command::splitParams(std::string msg, std::vector<std::string> &arg, std::string &cmd)
{
	if(msg == "")
		return (-1);
	std::string tmp ;
	int tab = msg.find(':');
	if(tab > 0)
		tmp = msg.substr(0, tab);
	std::stringstream ss(tmp);
	std::getline(ss, cmd, ' ');
	if(cmd.empty() || this->findCommand(cmd) == -1)
		return (-1);
	this->toUpper(cmd);
	while(std::getline(ss, tmp, ' ') && tmp.empty() == false)
		arg.push_back(tmp);
	arg.push_back(msg.substr(tab + 1, msg.length()));
	return (arg.size()); 
}

void Command::toUpper(std::string &str)
{
	for (int i = 0; i < (int)str.length(); i++)
		str[i] = toupper(str[i]);
}

void	Command::initBasicCommand()
{
	_basicCommand.push_back("USER");
	_basicCommand.push_back("NICK");
	_basicCommand.push_back("PASS");
	_basicCommand.push_back("PRIVMSG");
	_basicCommand.push_back("JOIN");
	_basicCommand.push_back("PART");
	_basicCommand.push_back("QUIT");
}

int Command::findCommand(std::string cmd)
{
	int i = 0;
	while (i < (int)_basicCommand.size())
	{
		if (cmd == _basicCommand[i])
			break;
		i++;
	}
	this->_indexCmd = i;
	if (i > 6)
		i = -1;
	return (i);
}

Command::Command()
{
}

Command::Command( Client &client,std::string &msg )
{
	this->_client = client;
	if (splitParams(msg, _args, _command) == -1)
		send(_client.getFd(), "ERROR :Invalid command\r\n", 24, 0);
	// switch (expression)
	// {
	// case (USER):
		
	// 	break;
	
	// default:
	// 	break;
	// }
	
}

std::string Command::getCommand() const
{
	return _command;
}

std::vector<std::string> Command::getArgs() const
{
	return _args;
}

std::string Command::getPass() const
{
	return _pass;
}


Client& Command::getClient()
{
	return _client;
}

void Command::setCommand(std::string command)
{
	this->_command = command;
}

void Command::setArgs(std::vector<std::string> args)
{
	this->_args = args;
}

void Command::setPass(std::string pass)
{
	this->_pass = pass;
}

void Command::setClient( Client &client )
{
	this->_client = client;
}

// void Command::passCommand()
// {
// 	if (this->_client.getIsRegistered())
// 		send(this->_client.getFd(), ":loclahost 462 :You are already registered\r\n", 41, 0);
// 	else
// 	{
// 		if (this->_args.size() < 1)
// 			send(this->_client.getFd(), ":localhost 460 : PASS <password>\r\n", 33, 0);
// 		else if (pass == this->_args[0])
// 			this->_client.setPassword(this->_args[0]);
// 		else
// 			send(this->_client.getFd(), ":localhost 464 :Password incorrect\r\n", 36, 0);
// 	}
// }

// void Command::nickCommand()
// {
// 	if(this->_client.getPassword() == "")
// 	{
// 		send(this->_client.getFd(), "You must enter a password first\r\n", 33, 0);
// 		return ;
// 	}
// 	if (this->_args.size() < 1)
// 		send(this->_client.getFd(), ":localhost 431 : NICK <nickname>\r\n", 33, 0);
// 	else if (this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
// 		send(this->_client.getFd(), ":localhost 432 :Nickname invalid\r\n", 34, 0);
// 	else
// 	{
// 		if (_client->getNickname() == "" && _client->getUsername() != "")
// 		{
// 			this->_client.setNickname(this->_args[0]);
// 			registerClient();
// 		}
// 		else
// 		{
// 			this->_client.setNickname(this->_args[0]);
// 			send(this->_client.getFd(), ":you are now known as " + this->_args[0] + "\r\n", 41, 0);
// 		}
// 	}
			
// }

