/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:11:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/04/23 19:21:24 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

int   Command::splitParams(std::string msg, std::vector<std::string> &arg, std::string &cmd)
{
	if(msg == "")
		return (-1);
	std::string tmp = msg;
	int tab = msg.find(':');
	if(tab > 0)
		tmp = msg.substr(0, tab -1);
	std::stringstream ss(tmp);
	std::getline(ss, cmd, ' '); 
	this->toUpper(cmd);
	if(cmd.empty() || this->findCommand(cmd) == -1)
		return (-1);
	while(std::getline(ss, tmp, ' ') && tmp.empty() == false)
		arg.push_back(tmp);
	if(tab > 0)
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
	_basicCommand.push_back("NOTICE");
	_basicCommand.push_back("JOIN");
	_basicCommand.push_back("PART");
	_basicCommand.push_back("QUIT");
	_basicCommand.push_back("MODE");
	_basicCommand.push_back("PONG");
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
	if (i > (int)_basicCommand.size() -1)
		i = -1;
	return (i);
}

Command::Command()
{
}

Command::Command(int nbClient,std::string &msg ,std::string &pass,std::vector<Client> &clients)
{
	this->_pass = pass;
	this->_client = clients[nbClient];
	this->_clients = clients;
	initBasicCommand();
	std::cout << "command : " << msg << std::endl;
	if (splitParams(msg, _args, _command) == -1)
	{
		std::cout << "error" << std::endl;
		send(_client.getFd(), "ERROR :Invalid command\r\n", 24, 0);
		return ;
	}
	// std::cout << "Command: " << _command << std::endl;
	switch (this->_indexCmd)
	{
	case (USER):
		userCommand();
		break;
	case (NICK):
		nickCommand();
		break;
	case (PASS):
		passCommand();
		break;
	case (JOIN):
		joinCommand();
		break;
	case (PRIVMSG):
		// privmsgCommand();
		break;
	case (PONG):
		break;
	}
	clients[nbClient] = this->_client;
	// case (PRIVMSG):
		
	// 	break;
	// case (JOIN):
		
	// 	break;
	// case (PART):
		
	// 	break;
	// case (QUIT):
		
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

void	Command::sendReply(std::string msg)
{
	send(this->_client.getFd(), msg.c_str(), msg.length(), 0);
}

int	Command::nickExist(std::string nick)
{
	int i = 0;
	while (i < (int)_clients.size())
	{
		if (_clients[i].getNickname() == nick)
			return (i);
		i++;
	}
	return (-1);
}

void Command::passCommand()
{
	if (this->_client.getIsRegistered())
		sendReply(":localhost 462 You are already registered\r\n");
	else if(this->_client.getPassword() != "")
		sendReply(":localhost 462 You are already give me password\r\n");
	else
	{
		if(this->_args.size() != 1 || this->_args[0] == "")
			sendReply(":localhost 461 PASS <password>\r\n");
		else if (this->_args[0] == this->_pass)
			this->_client.setPassword(this->_args[0]);
		else
			sendReply(":localhost 464 Password incorrect\r\n");
	}
}

void Command::joinCommand()
{
	std::cout << this->_command << std::endl;
	std::cout << this->_args[0] << std::endl;
	std::cout << "okhh " << this->_args.size() << std::endl;
};

void Command::nickCommand()
{
	std::cout<< this->_args.size() << std::endl;
	if(this->_client.getPassword() == "")
	{
		sendReply("ERROR :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 1 || this->_args[0] == "")
		sendReply(":localhost 431 " + _client.getNickname() + ": NICK <nickname>\r\n");
	else if (this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendReply(":localhost 432 " + _client.getNickname() + ": Nickname invalid\r\n");
	else if(nickExist(this->_args[0]) != -1)
		sendReply(":localhost 433 " + _client.getNickname()+ " Nickname already in use\r\n");
	else
	{
		if (_client.getNickname() == "" && _client.getUsername() != "")
		{
			sendReply(":localhost 001 " + this->_args[0] + " :Welcome to the Internet Relay Network " + this->_args[0] + "!\r\n");
			sendReply(":localhost 002 " + this->_args[0] + " :Your host is localhost, running version 0.1\r\n");
			sendReply(":localhost 003 " + this->_args[0] + " :This server was created 2019-10-10\r\n");
			sendReply(":localhost 004 " + this->_args[0] + " :localhost 0.1\r\n");
			this->_client.setIsRegistered(true);
		}
		else if(_client.getNickname() != "" && _client.getUsername() != "")
			sendReply(":" + this->_args[0] + "!" + _client.getUsername() + "@localhost" + " NICK " + this->_args[0] + "\r\n"); 
		this->_client.setNickname(this->_args[0]); 
	}
}

void	Command::userCommand()
{
	if(this->_client.getPassword() == "")
	{
		sendReply("ERROR :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 4)
		sendReply(":localhost 461 " + _client.getNickname() + ": USER <username> <hostname> <servername> <realname>\r\n");
	else if (this->_client.getUsername() != "" && this->_client.getNickname() != "")
		sendReply(":localhost 462 " + _client.getNickname() + ":You are already registered\r\n");
	else
	{
		if (_client.getNickname() != "" && _client.getUsername() == "")
		{
			sendReply(":localhost 001 " + _client.getNickname() + " :Welcome to the Internet Relay Network " + _client.getNickname() + "!\r\n");
			sendReply(":localhost 002 " + _client.getNickname() + " :Your host is localhost, running version 0.1\r\n");
			sendReply(":localhost 003 " + _client.getNickname() + " :This server was created 2019-10-10\r\n");
			sendReply(":localhost 004 " + _client.getNickname() + " :localhost 0.1\r\n");
			this->_client.setIsRegistered(true);
		}
		this->_client.setUsername(this->_args[0]);
	}
}

void	Command::privmsgCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":localhost 461 " + _client.getNickname() + ": PRIVMSG <nickname> <message>\r\n");
	else if (this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendReply(":localhost 432 " + _client.getNickname() + ": Nickname invalid\r\n");
	else if(nickExist(this->_args[0]) == -1)
		sendReply(":localhost 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost" + " PRIVMSG " + this->_args[0] + " :" + this->_args[1] + "\r\n");
		sendReply(":" + this->_args[0] + "!" + _clients[nickExist(this->_args[0])].getUsername() + "@localhost" + " PRIVMSG " + _client.getNickname() + " :" + this->_args[1] + "\r\n");
	}
}