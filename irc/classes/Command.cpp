/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:11:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/15 18:48:05 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

int   Command::splitParams(std::string msg, std::vector<std::string> &arg, std::string &cmd)
{
	if(msg == "")
		return (-1);
	msg.erase(0, msg.find_first_not_of(' '));
	std::string tmp = msg;
	int tab = msg.find(':');
	if(tab > 0)
		tmp = msg.substr(0, tab);
	std::stringstream ss(tmp);
	std::getline(ss, cmd, ' '); 
	this->toUpper(cmd);
	if(cmd.empty() || this->findCommand(cmd) == -1)
		return (-1);
	while(std::getline(ss, tmp, ' ') && tmp.empty() == false)
		arg.push_back(tmp);
	if(tab > 0)
		arg.push_back(msg.substr(tab , msg.length()));
	return (arg.size()); 
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

Command::Command(int nbClient,std::string &msg ,std::string &pass,std::vector<Client> &clients):_client(clients[nbClient]), _clients(clients)
{
	this->_pass = pass;
	initBasicCommand();
	// std::cout << "command : " << msg << std::endl;
	if (splitParams(msg, _args, _command) == -1)
	{
		sendReply(":localhost 421 * : " + _command + " Unknown command\r\n");
		return ;
	}
	if (!_client.getIsRegistered() &&  this->_indexCmd != USER  && this->_indexCmd != NICK && this->_indexCmd != PASS)
	{
		sendReply(":localhost 451 * :You have not registered\r\n");
		return ;
	}
	// std::cout << "Command: " << _command << std::endl;
	// std::cout << "BEFORE: " << this->_clients.size() << std::endl;
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
	case (QUIT):
		quitCommand();
		break;
	case (MODE):
		modeCommand();
		break;
	}
	// std::cout << "AFTER: " << this->_clients.size() << std::endl;
};

// std::string Command::joinVectorValues()
// {
// 	std::string joinedValues = "\"";

// 	if (this->_args.size() > 2)
// 	{
// 		std::vector<std::string>::iterator it = this->_args.begin() + 1;
// 		while (it != this->_args.end())
// 		{
// 			joinedValues += *it + ' ';
// 			++it;
// 		}
// 	}
// 	else
// 		joinedValues = this->_args[1];
// 	return joinedValues;
// }

std::string Command::getCommand() const
{
	return _command;
};

std::vector<std::string> Command::getArgs() const
{
	return _args;
};

std::string Command::getPass() const
{
	return _pass;
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

void Command::setPass(std::string pass)
{
	this->_pass = pass;
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

void Command::passCommand()
{
	if (this->_client.getIsRegistered())
		sendReply(":localhost 462 * :You are already registered\r\n");
	else if(this->_client.getPassword() != "")
		sendReply(":localhost 462 * :You are already give me password\r\n");
	else
	{
		if(this->_args.size() != 1 || this->_args[0] == "")
			sendReply(":localhost 461 * :PASS <password>\r\n");
		else if (this->_args[0] == this->_pass)
			this->_client.setPassword(this->_args[0]);
		else
			sendReply(":localhost 464 * :Password incorrect\r\n");
	}
};

// void Command::joinCommand()
// {
// 	if (this->_args.size() == 1 && _client.isMemberOfChannel(this->_args[0], _client.getFd()) != 1) // Joining A New Channel
// 	{
//         sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + this->_args[0] + "\r\n");
// 		int privilege;
// 		if (_client.isMemberOfChannel(this->_args[0], _client.getFd()) == -1)
// 		{
//         	sendReply(":localhost MODE " + this->_args[0] + " +nt\r\n");
//         	sendReply(":localhost 353 " + _client.getNickname() + " = " + this->_args[0] + " :@" + this->_client.getNickname() + "\r\n");
// 			_channelObj = Channel(this->_args[0], "", "", _client);
// 			_channelObj.joinChannel();
// 			privilege = OPERATOR;
// 		}
// 		else // 0
// 		{
// 			this->_channelObj._channelMap[this->_args[0]].setChannelCreationTime(this->getCurrentUnixTimestamp());
//         	sendReply(":localhost 333 " + _client.getNickname() + " " + this->_args[0] + " " + _channelObj.getOperator().getNickname() + "!localhost " + this->_channelObj._channelMap[this->_args[0]].getChannelCreationTime() + "\r\n");
//         	sendReply(":localhost 353 " + _client.getNickname() + " @ " + this->_args[0] + " :" + this->_client.getNickname() + " " + _channelObj.getOperator().getNickname() + "\r\n");
// 			privilege = CLIENT;
// 		}
// 		sendReply(":localhost 366 " + _client.getNickname() + " " + this->_args[0] + " :End of /NAMES list.\r\n");
// 		_channelObj.addUserToChannelMap(this->_client, privilege);
// 	}
// };

std::string getChannelKey(const std::vector<std::string>& channelKeys, int channelMapSize)
{
    std::string chKey = "";
    if (channelKeys.size() > 0)
    {
        int index = channelMapSize % channelKeys.size();
        chKey = channelKeys[index];
    }
    return chKey;
}

void Command::joinCommand() 
{
    if (this->_args.size() < 1)
    {
        sendReply(":localhost 461 " + _client.getNickname() + " JOIN :Not enough parameters\r\n");
        return;
    }
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;
    std::vector<std::string> channelKeys;

    if (this->_args.size() > 1)
    {
        std::stringstream keySplitter(this->_args[1]);
        std::string key;
        while (std::getline(keySplitter, key, ','))
            channelKeys.push_back(key);
    }

    while (std::getline(channelSplitter, channelName, ','))
    {
        if (channelName.empty() || channelName[0] != '#')
        {
            sendReply(":localhost 476 " + channelName + " Invalid channel name\r\n");
            continue;
        }
		if (_client.isMemberOfChannel(channelName, _client.getFd()) == -1)
		{
			_channelObj = Channel(channelName, "", getChannelKey(channelKeys, _channelObj._channelMap.size()), _client);
			_channelObj.addChannelToChannelMap();
			_channelObj.addUserToUserMap(_client, OPERATOR);
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + channelName + "\r\n");
        	sendReply(":localhost MODE " + channelName + " +nt\r\n");
        	sendReply(":localhost 353 " + _client.getNickname() + " = " + channelName + " :@" + this->_client.getNickname() + "\r\n");
			sendReply(":localhost 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");

			// _channelObj._channelMap[channelName].setChannelCreationTime(this->getCurrentUnixTimestamp());
		}
		else if (!_client.isMemberOfChannel(channelName, _client.getFd())) // 0
		{
			_channelObj = _channelObj._channelMap[channelName];
			std::string  checkKey = getChannelKey(channelKeys, _channelObj._channelMap.size());
			if (!_channelObj.verifyKey(checkKey))
			{
				sendReply(":localhost 474 " + _client.getNickname() + " " + channelName + " :Cannot join channel\r\n");
				return ;
			}
			_channelObj.addUserToUserMap(_client, CLIENT);
			// sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + channelName + "\r\n");

			this->broadcast(channelName, ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + channelName + "\r\n");
			sendReply(":localhost 353 " + _client.getNickname() + " @ " + channelName + " " + _channelObj.usersList() + "\r\n");
			sendReply(":localhost 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
			// YASSIN : We have to add the broadcast here to inform other users that a new user is joined !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
    }
};

void Command::partCommand()
{
	// For NetCat FuCking Tests:
	if (!this->_args.size())
	{
		// ERR_NEEDMOREPARAMS (461)
        sendReply(":localhost 461 " + _client.getNickname() + " PART :Not enough parameters\r\n");
		return;
	}
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;
	//####################################################=====> CHECK THE ARGUMENTS | if the user typed just KICK or if he typed a lot of Params !!!!!!!!
	// int i = -1;
	// while (++i < (int)this->_args.size())
	// 	std::cout << this->_args[i] << std::endl;
	// std::cout << this->_args.size() << std::endl;
 	while (std::getline(channelSplitter, channelName, ','))
    {
		// Parting A Channel You’re Not Joined To: (ERR_NOTONCHANNEL (442))
		if (!this->getClient().isMemberOfChannel(channelName, _client.getFd()))
			message = ":localhost 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
		{
			// Parting A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
			if (channelName[0] == ':' && !channelName[1])
				message = ":localhost 403 " + this->_client.getNickname() + " * No such channel\r\n";
			else
				message = ":localhost 403 " + this->_client.getNickname() + " " + channelName.substr(1) + " :No such channel\r\n";
		}
		else
		{
			// Parting With No Reason (Already Joined!):
			message = ":" + this->_client.getNickname() + "!@localhost PART " + channelName + "\r\n";
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
			// Parting With A Reason:
			if (this->_args[1][0] == ':')
				message = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@localhost PART " + channelName + " " + this->_args[1] + "\r\n";
		}																			
		sendReply(message);
	}
};

std::string Command::getCurrentUnixTimestamp()
{
	std::time_t now = std::time(NULL);
	std::stringstream ss;
	ss << now;
	return ss.str();
};

void Command::topicCommand()
{
	// Set A Topic To A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
	{
		sendReply(":localhost 403 " + this->_client.getNickname() + " " + this->_args[0] + " No such channel\r\n");
		return;
	}
	// Setting A Topic
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == 1
		&& this->_args.size() > 1)
	{
		// Add The Topic To The Channel Map
		std::string topic;
		if (this->_args[1].length() == 1 && this->_args[1][0] == ':')
			topic = ":";
		else
			topic = this->_args[1];
		_channelObj._channelMap[this->_args[0]].setTopic(topic);
		_channelObj._channelMap[this->_args[0]].setTopicTime(this->getCurrentUnixTimestamp());
		sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@localhost TOPIC " + this->_args[0] + " " + this->_args[1] + "\r\n");
	}
	else if (this->_args.size() == 1)
	{
		if (_channelObj._channelMap[this->_args[0]].getTopic().empty())
			sendReply(":localhost 331 " + this->_client.getNickname() + " " + this->_args[0] + " :No topic is set\r\n");
		else
		{
			//RPL_TOPIC (332)
			sendReply(":localhost 332 " + this->_client.getNickname() + " " + this->_args[0] + " " + _channelObj._channelMap[this->_args[0]].getTopic() + "\r\n");
			// RPL_TOPICTIME (333)
			sendReply(":localhost 333 " + this->_client.getNickname() + " " + this->_args[0] + " " + this->_client.getNickname() + " " + _channelObj._channelMap[this->_args[0]].getTopicTime() + "\r\n");
		}
	}
};

void Command::kickCommand()
{
	if (this->_args.size() < 3)
    {
        sendReply(":localhost 461 " + _client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;
	
	if(this->_client.getOpPriviligePermission() == CLIENT)
		sendReply(":localhost 482" + _client.getNickname() + " " + channelName + " :You're not channel operator");
	while (std::getline(channelSplitter, channelName, ','))
    {
		if (!this->getClient().isMemberOfChannel(channelName, _client.getFd()))
			message = ":localhost 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
			message = ":localhost 403 " + this->_client.getNickname() + " " + channelName + " :No such channel\r\n";
		else
		{
			message = ":" + this->_client.getNickname() + "!@localhost PART " + channelName + "\r\n";
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
			//kick a user by specifying the reason why ...
		}																			
		sendReply(message);
	}
};

std::string getJokeQuote()
{
	std::vector<std::string>nokat_7amdin; // link : http://nokat-mawadi3-amtal.blogspot.com/2011/10/nokat-maghribiya-bi-darija-l-maghribiya.html
	nokat_7amdin.push_back("galek hadou wahed jouj bnat mchaw ychoufou natija nta3 lbac wehda l9at rasseha sa9ta wlokhra saroute");
	nokat_7amdin.push_back("galk hada wa7d zizoun ila bgha igoul gar3a tigoul liha ga3a");
	nokat_7amdin.push_back("hada wahed dar ouhowa itekra");
	nokat_7amdin.push_back("Hada wahd 9ta3 chanti .. ohowa ykhrej lih zaft");
	nokat_7amdin.push_back("hadi wahed nokta b me9lob iwa dehako o ne3awedalkom hhhhhhh loool");
	nokat_7amdin.push_back("aaa ch77aaal 7amd hhhhhhhhh");
    std::srand(std::time(0));
    int randIndex = std::rand() % nokat_7amdin.size();
    return nokat_7amdin[randIndex];
};

std::string getTime()
{
    std::time_t t = std::time(NULL);
    char time_str[20];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&t));
    return std::string(time_str) + " \r\n";
};

void Command::botCommand()
{
	// std::cout << this->_args.size() << std::endl;
	std::string message = "The current time is " + getTime();
	if(this->_args.size() < 1)
	{
        sendReply(":localhost 461 " + _client.getNickname() + " BOT :Not enough parameters\r\n");
        return;
    }
	std::stringstream channelSplitter(this->_args[0]);
	std::string cmd = channelSplitter.str();
	std::cout << cmd << std::endl;
	if (cmd != "time")
	{
		sendReply(":localhost Available Bots Now : [time - nokta]>\r\n");
	}
	if (cmd == "time")
		sendReply("PRIVMSG" + message);
	if(cmd == "nokta")
		sendReply(":" + getJokeQuote() + "\r\n");	 
};

void Command::nickCommand()
{
	if(this->_client.getPassword() == "")
	{
		sendReply(":localhost ERROR * :You must enter a password first\r\n");
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
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost" + " NICK " + this->_args[0] + "\r\n"); 
		this->_client.setNickname(this->_args[0]); 
	}
};

void	Command::userCommand()
{
	if(this->_client.getPassword() == "")
	{
		sendReply(":localhost ERROR * :You must enter a password first\r\n");
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
};

void	Command::privmsgCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":localhost 461 " + _client.getNickname() + ": PRIVMSG <nick/channel> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == - 1)
		sendReply(":localhost 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":localhost 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost" + " PRIVMSG " + this->_args[0] + " :" + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0],tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);
		}
		// sendReply(":" + this->_args[0] + "!" + _clients[nickExist(this->_args[0])].getUsername() + "@localhost" + " PRIVMSG " + _client.getNickname() + " :" + this->_args[1] + "\r\n");
	}
}

void	Command::noticeCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":localhost 461 " + _client.getNickname() + ": NOTICE <nickname> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
		sendReply(":localhost 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":localhost 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost" + " NOTICE " + this->_args[0] + " :" + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0],tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);	
		}
		// sendReply(":" + this->_args[0] + "!" + _clients[nickExist(this->_args[0])].getUsername() + "@localhost" + " NOTICE " + _client.getNickname() + " :" + this->_args[1] + "\r\n");
	}
};
void	Command::quitCommand()
{
	std::vector<Client>::iterator it = this->_clients.begin();

	sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost" + " QUIT " + this->_args[0] + "\r\n");
	while(it != this->_clients.end())
	{
		if (it->getNickname() == _client.getNickname())
		{
			it->setIsRegistered(false);
			it->setNickname("");
			it->setPassword("");
			it->setUsername("");
			break;
		}
		++it;
	}
};

void	Command::modeCommand()
{
	// std::cout << "nick :" << this->_args[0] << ":" << std::endl;
	// std::cout << "nick :" <<nickExist(this->_args[0])<< ":" << std::endl;
	std::string mode;
	if (this->_args[0] != _client.getNickname()  && _client.isMemberOfChannel(this->_args[0], _client.getFd()) == -1)
	{
		sendReply(":localhost 403 " + this->_client.getNickname() + " " + this->_args[0] + " * No such channel\r\n");
		return ;	
	}
	if (this->_args.size() == 1)
	{
		if (this->_args[0][0] == '#' ) // Channel Modes
		{
			sendReply(":localhost 324 " + this->_client.getNickname() + " " + this->_args[0] + " " + _channelObj._channelMap[this->_args[0]].getMode() + " \r\n");
			sendReply(":localhost 329 " + this->_client.getNickname() + " " + this->_args[0] + " " + _channelObj._channelMap[this->_args[0]].getChannelCreationTime() + " \r\n");
		}
		else
			sendReply(":localhost 221 " + this->_client.getNickname() + " " + _channelObj._channelMap[this->_args[0]].getMode() + " \r\n");
		return;
	}
	else if (this->_args.size() > 1)
	{
		
		if (this->_args[1][0] == '+')
		{
			int i = 1;
			while(this->_args[1][i] && this->_args[1][i] == '+')
				i++;
			if ((this->_args[1].substr(i)).find_first_not_of("b") != std::string::npos)
			{
				// Error
				return;
			}
			mode = "+b";
			_channelObj._channelMap[this->_args[0]].setMode(mode);
			sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@localhost MODE " + this->_args[0] + " +b " + this->_args[2] + "!*@*\r\n");
			// sendReply(":Guest45756!~usr@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #rgatnaou +b usr2!*@*");
		}
		else if (this->_args[1][0] == '-')
		{
			int i = 1;
			while(this->_args[1][i] && this->_args[1][i] == '-')
				i++;
			if ((this->_args[1].substr(i)).find_first_not_of("b") != std::string::npos)
			{
				// Error
				return;
			}
			mode = "-b";
			_channelObj._channelMap[this->_args[0]].setMode(mode);
			sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@localhost MODE " + this->_args[0] + " -b " + this->_args[2] + "!*@*\r\n");
			// sendReply(":Guest45756!~usr@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #rgatnaou +b usr2!*@*");
		}
	}
};

void Command::broadcast( std::string const &channel, std::string const &msg)
{
	userMap::iterator it = this->_channelObj._channelMap[channel]._userMap.begin();

	while (it != this->_channelObj._channelMap[channel]._userMap.end())
	{
			send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		++it;
	}
};
