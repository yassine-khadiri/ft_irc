/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:11:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/06/10 15:47:35 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"

Command::Command()
{
};

Command::Command(std::string &pass)
{
	_pass = pass;
	initBasicCommand();
};

Command::~Command()
{
};

int   Command::splitParams(std::string msg, std::vector<std::string> &arg, std::string &cmd)
{
	if (msg == "")
		return (-1);
	msg.erase(0, msg.find_first_not_of(' '));
	std::string tmp = msg;
	int semicolon = msg.find(':');
	if(semicolon > 0)
		tmp = msg.substr(0, semicolon);
	std::stringstream ss(tmp);
	std::getline(ss, cmd, ' '); 
	this->toUpper(cmd);
	if (cmd.empty() || this->findCommand(cmd) == -1)
		return (-1);
	while(std::getline(ss, tmp, ' ') && tmp.empty() == false)
		arg.push_back(tmp);
	if (semicolon > 0)
		arg.push_back(msg.substr(semicolon, msg.length()));
	return (arg.size()); 
};

std::string Command::getMachineHostName()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        // Error occurred while retrieving the hostname
        return "";
    }
    return hostname;
};

void Command::toUpper(std::string &str)
{
	for (int i = 0; i < (int)str.length(); i++)
		str[i] = toupper(str[i]);
};

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
	_basicCommand.push_back("KICK");
	_basicCommand.push_back("TOPIC");
	_basicCommand.push_back("INVITE");
	_basicCommand.push_back("BOT");
};

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
};

void Command::exec(int nbClient, std::string &msg ,std::vector<Client> &clients)
{
	
	_clients = clients;
	_client = _clients[nbClient];
	_args.clear();
	_command = "";
	this->modes.clear();
	if (splitParams(msg, _args, _command) == -1)
	{
		sendReply(":" + getMachineHostName() + " 421 * : " + _command + " Unknown command\r\n");
		return ;
	}
	if (!_client.getIsRegistered() &&  this->_indexCmd != USER  && this->_indexCmd != NICK && this->_indexCmd != PASS)
	{
		sendReply(":" + getMachineHostName() + " 451 * :You have not registered\r\n");
		return ;
	}
	std::cout << "Command: " << msg << std::endl;
	
	switch (this->_indexCmd)
	{
	case(BOT):
		botCommand();
		break;
	case (KICK):
		kickCommand();
		break;
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
	case (PART):
		partCommand();
		break;
	case (PRIVMSG):
		privmsgCommand();
		break;
	case (NOTICE):
		noticeCommand();
		break;
	case (PONG):
		break;
	case (TOPIC):
		topicCommand();
		break;
	case (INVITE):
		inviteCommand();
		break;
	case (QUIT):
		quitCommand();
		break;
	case (MODE):
		modeCommand();
		break;
	}
	_clients[nbClient] = _client; 
	clients = _clients;
};

std::string Command::getCommand() const
{
	return _command;
};

std::vector<std::string> Command::getArgs() const
{
	return _args;
};

Client& Command::getClient()
{
	return _client;
};

void Command::setCommand(std::string command)
{
	this->_command = command;
};

void Command::setArgs(std::vector<std::string> args)
{
	this->_args = args;
};

void Command::setClient( Client &client )
{
	this->_client = client;
};

void	Command::sendReply(std::string msg)
{
	send(this->_client.getFd(), msg.c_str(), msg.length(), 0);
};

int	Command::nickExist(std::string nick)
{
	int i = 0;

	if (nick.empty())
		return (-1);
	while (i < (int)_clients.size())
	{
		if (_clients[i].getNickname() == nick)
			return (i);
		i++;
	}
	return (-1);
};

std::string Command::getCurrentUnixTimestamp()
{
	std::time_t now = std::time(NULL);
	std::stringstream ss;

	ss << now;
	return ss.str();
};

int	Command::searchClientByName( std::string clientName )
{
	std::vector<Client>::iterator it = this->_clients.begin();

	while (it != this->_clients.end())
	{
		if (it->getNickname() == clientName)
			return it->getFd();
		++it;
	}
	return 0;
};

int Command::leaveAllChannels()
{
	this->_args.clear();
	this->_args.push_back("");
	channelMap::iterator it = this->_channelObj._channelMap.begin();

	while (it != this->_channelObj._channelMap.end())
	{
		channelMap::iterator itTmp = it;
		itTmp++;
		if (this->_client.isMemberOfChannel(it->first, this->_client.getFd()) == 1)
			it->second->removeUserFromUserMap(it->second->getChannelName(),_client.getFd());
		it = itTmp;
	}
	return 1;
};

void Command::broadcast( std::string const &channel, std::string const &msg)
{
	userMap::iterator it = this->_channelObj._channelMap[channel]->_userMap.begin();

	while (it != this->_channelObj._channelMap[channel]->_userMap.end())
	{
		if(!((this->_indexCmd == PRIVMSG || this->_indexCmd == NOTICE) && _client.getFd() == it->second.getFd()))	
			send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		++it;
	}
};
