/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 18:11:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2023/05/24 16:11:56 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

Command::Command()
{
};

Command::Command(std::string &pass)
{
	_pass = pass;
	initBasicCommand();
};

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

std::string getMachineHostName()
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

void Command::exec(int nbClient,std::string &msg ,std::vector<Client> &clients)
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
	// std::cout << "command : " << _args[0] << std::endl;
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

void Command::passCommand()
{
	if (this->_client.getIsRegistered())
		sendReply(":" + getMachineHostName() + " 462 * :You are already registered\r\n");
	else if(this->_client.getPassword() != "")
		sendReply(":" + getMachineHostName() + " 462 * :You are already give me password\r\n");
	else
	{
		if(this->_args.size() != 1 || this->_args[0] == "")
			sendReply(":" + getMachineHostName() + " 461 * :PASS <password>\r\n");
		else if (this->_args[0] == this->_pass)
			this->_client.setPassword(this->_args[0]);
		else
			sendReply(":" + getMachineHostName() + " 464 * :Password incorrect\r\n");
	}
};

std::string getChannelKey(const std::vector<std::string>& channelKeys, int channelMapSize)
{
    std::string chKey = "";

    if (channelKeys.size() > 0)
    {
        int index = channelMapSize % channelKeys.size();
        chKey = channelKeys[index];
    }
    return chKey;
};

// void Command::getAllChannelUser()
// {
// 	this->_args.clear();
// 	this->_args.push_back("");
// 	channelMap::iterator it = this->_channelObj._channelMap.begin();

// 	while (it != this->_channelObj._channelMap.end())
// 	{
// 		if (this->_client.isMemberOfChannel(it->first, this->_client.getFd()) == 1)
// 			this->_args[0] += it->first + ",";
// 		++it;
// 	}
// };

void Command::joinCommand() 
{
    if (this->_args.size() <= 1)
    {
		if (this->_args.size() == 1 && this->_args[0] == "0")
		{
			if (!leaveAllChannels())
				sendReply(":" + getMachineHostName() + " 476 " + _client.getNickname() + " 0 :Invalid channel name\r\n");
        	return;
		}
		else if (this->_args.size() < 1)
		{	
        	sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " JOIN :Not enough parameters\r\n");
			return ;
		}
    }
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;
    std::vector<std::string> channelKeys;

    if (this->_args.size() > 1)
    {
        std::stringstream keySplitter(this->_args[1]);
        std::string key;

        while (std::getline(keySplitter, key, ','))
		{
            channelKeys.push_back(key);
		}
    }

    while (std::getline(channelSplitter, channelName, ','))
    {
        if (channelName.empty() || channelName[0] != '#')
        {
            sendReply(":" + getMachineHostName() + " 476 " + channelName + " Invalid channel name\r\n");
            continue;
        }
		if (this->_client.isMemberOfChannel(channelName, this->_client.getFd()) == -1)
		{
			this->_channelObj = Channel(channelName, "", getChannelKey(channelKeys, this->_channelObj._channelMap.size()), _client);
			this->_channelObj.addUserToUserMap(_client, OPERATOR);
			this->_channelObj.setChannelCreationTime(this->getCurrentUnixTimestamp());
			this->_channelObj.setMode("+n");
			this->_channelObj.setMode("+t");
			if (!getChannelKey(channelKeys, this->_channelObj._channelMap.size()).empty())
				this->_channelObj.setMode("+k");
			this->_channelObj.addChannelToChannelMap();
			// this->_channelObj = this->_channelObj._channelMap[this->_args[0]];
			//  std::cout << "operator :" <<   this->_channelObj.getOperator().getNickname() << std::endl;
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");
        	sendReply(":" + getMachineHostName() + " MODE " + channelName + " " + this->_channelObj.getModes() + "\r\n");
        	sendReply(":" + getMachineHostName() + " 353 " + _client.getNickname() + " = " + channelName + " :@" + this->_client.getNickname() + "\r\n");
			sendReply(":" + getMachineHostName() + " 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
		}
		else if (!this->_client.isMemberOfChannel(channelName, this->_client.getFd())) // 0
		{
			this->_channelObj = this->_channelObj._channelMap[channelName];

			if (this->_channelObj.getModes().find("i") != std::string::npos)
			{
				std::cout << "Mode Channel: " << this->_channelObj.getModes() << std::endl;
				std::cout << "founded! " << this->_channelObj.getModes().find("i") << std::endl;
				sendReply(":" + getMachineHostName() + " 473 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n"); //ERR_INVITEONLYCHAN (473)
			}
			else if (this->_channelObj.getModes().find("l") != std::string::npos && (int)this->_channelObj._userMap.size() >= this->_channelObj.getLimitUsers())
				sendReply(":" + getMachineHostName() + " 471 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n"); //ERR_CHANNELISFULL (471)
			else
			{
				std::string  checkKey = getChannelKey(channelKeys, this->_channelObj._channelMap.size());

				if (!this->_channelObj.verifyKey(checkKey))
				{
					sendReply(":" + getMachineHostName() + " 474 " + this->_client.getNickname() + " " + channelName + " :Cannot join channel\r\n");
					return ;
				}
				this->_channelObj.addUserToUserMap(_client, CLIENT);
				this->_channelObj._channelMap[channelName] = this->_channelObj;
				// sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");

				this->broadcast(channelName, ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " JOIN " + channelName + "\r\n");
				sendReply(":" + getMachineHostName() + " 353 " + _client.getNickname() + " @ " + channelName + " " + this->_channelObj.usersList() + "\r\n");
				sendReply(":" + getMachineHostName() + " 366 " + _client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
			}
		}
    }
};

void Command::partCommand()
{
	// For NetCat FuCking Tests:
	if (!this->_args.size())
	{
		// ERR_NEEDMOREPARAMS (461)
        sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " PART :Not enough parameters\r\n");
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
			message = ":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
		{
			// Parting A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
			if (channelName[0] == ':' && !channelName[1])
				message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " * No such channel\r\n";
			else
				message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName.substr(1) + " :No such channel\r\n";
		}			
		else
		{
			// Parting With No Reason (Already Joined!):
			message = ":" + this->_client.getNickname() + "!@" + getMachineHostName() + " PART " + channelName + "\r\n";
			_channelObj.removeUserFromUserMap(channelName, _client.getFd());
			// Parting With A Reason:
			if (this->_args[1][0] == ':')
				message = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " PART " + channelName + " " + this->_args[1] + "\r\n";
		}																			
		broadcast(channelName,message);
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
	int i = -1;
	while(++i < (int)this->_args.size())
	{
		std::cout << _args[i] <<std::endl;
	}
	// Set A Topic To A Non-existent Channel: (ERR_NOSUCHCHANNEL (403))
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
	{
		sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[0] + " No such channel\r\n");
		return;
	}
	// Setting A Topic
	if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == 1
		&& this->_args.size() > 1)
	{
		if (!this->_channelObj._channelMap[this->_args[0]]._userMap[_client.getFd()].getOpPriviligePermission() && this->_channelObj.findMode("+t"))
		{
			sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not channel operator\r\n"); //ERR_CHANOPRIVSNEEDED (482)
			return;
		}
		// Add The Topic To The Channel Map
		std::string topic;
		if (this->_args[1].length() == 1 && this->_args[1][0] == ':')
			topic = ":";
		else
			topic = this->_args[1];
		_channelObj._channelMap[this->_args[0]].setTopic(topic);
		_channelObj._channelMap[this->_args[0]].setTopicTime(this->getCurrentUnixTimestamp());
		sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " TOPIC " + this->_args[0] + " " + this->_args[1] + "\r\n");
	}
	else if (this->_args.size() == 1)
	{
		if (_channelObj._channelMap[this->_args[0]].getTopic().empty())
			sendReply(":" + getMachineHostName() + " 331 " + this->_client.getNickname() + " " + this->_args[0] + " :No topic is set\r\n");
		else
		{
			// RPL_TOPIC (332)
			sendReply(":" + getMachineHostName() + " 332 " + this->_client.getNickname() + " " + this->_args[0] + " " + _channelObj._channelMap[this->_args[0]].getTopic() + "\r\n");
			// RPL_TOPICTIME (333)
			sendReply(":" + getMachineHostName() + " 333 " + this->_client.getNickname() + " " + this->_args[0] + " " + this->_client.getNickname() + " " + _channelObj._channelMap[this->_args[0]].getTopicTime() + "\r\n");
		}
	}
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

void Command::inviteCommand()
{
	if (this->_args.size() == 1)
		sendReply(":" + getMachineHostName() + " 461 " + this->_client.getNickname() + " " + this->_args[1] + " :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
	else if (this->_args.size() == 2)
	{
		if (this->_client.isMemberOfChannel(this->_args[1], this->_client.getFd()) == -1)
			sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[1] + " :No such channel\r\n"); //ERR_NOSUCHCHANNEL (403)
		else if (!this->_client.isMemberOfChannel(this->_args[1], this->_client.getFd()))
			sendReply(":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not on that channel\r\n"); //ERR_NOTONCHANNEL (442)
		else if (this->searchClientByName(this->_args[0]))
		{
			if (this->_client.isMemberOfChannel(this->_args[1], this->searchClientByName(this->_args[0])) == 1)
				sendReply(":" + getMachineHostName() + " 443 " + this->_client.getNickname() + " " + this->_args[1] + " :is already on channel\r\n"); //ERR_USERONCHANNEL (443)
			else if (!this->_client.isMemberOfChannel(this->_args[1], this->searchClientByName(this->_args[0])))
			{
				if (this->_channelObj._channelMap[this->_args[1]].findMode("+i") && this->_client.getNickname() != this->_channelObj.getOperator().getNickname())
					sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " " + this->_args[1] + " :You're not channel operator\r\n"); //ERR_CHANOPRIVSNEEDED (482)
				else
					sendReply(":" + _client.getNickname() + "!" + this->_args[0] + "@" + getMachineHostName() + " INVITE " + this->_args[0] + " " + this->_args[1] + "\r\n"); //RPL_INVITING (341)
			}
		}
	}
};

void Command::kickCommand()
{
	if (this->_args.size() < 3)
    {
        sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }
	std::string message = "";
    std::stringstream channelSplitter(this->_args[0]);
    std::string channelName;

		if(this->_client.getOpPriviligePermission() == CLIENT)
			sendReply(":" + getMachineHostName() + " 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator");
	while (std::getline(channelSplitter, channelName, ','))
    {
		if (!this->getClient().isMemberOfChannel(channelName, _client.getFd()))
			message = ":" + getMachineHostName() + " 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		else if (this->getClient().isMemberOfChannel(channelName, _client.getFd()) == -1)
			message = ":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + channelName + " :No such channel\r\n";
		else
		{
			message = ":" + this->_client.getNickname() + "!@" + getMachineHostName() + " PART " + channelName + "\r\n";
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

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
};

void Command::botCommand()
{
	if(this->_args.size() < 1)
	{
        sendReply(":"+ getMachineHostName() +" 461 " + _client.getNickname() + " BOT :Not enough parameters\r\n");
        return;
    }
	std::stringstream channelSplitter(this->_args[0]);
	std::string cmd = channelSplitter.str();

	if (cmd != "time" && cmd != "nokta"  && cmd != "jokes")
	{
		sendReply("300 RPL_NONE: Available Bots Now : [time - nokta]\r\n");
		return ;
	}
	if (cmd == "time")
		sendReply("300 RPL_NONE :" + getTime());
	if (cmd == "nokta")
		sendReply("300 RPL_NONE :" + getJokeQuote() + "\r\n");	
	if (cmd == "jokes")
	{
		std::string url = "https://api.api-ninjas.com/v1/jokes?limit";

		// Initialize cURL
		CURL* curl = curl_easy_init();

		if (curl)
		{
			// Set the URL
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			// Set the headers
			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, "X-Api-Key: KjtJWjMZe7WLG0ucyGyHvcOuhssaEopZZKXnCUhu");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			// Set the response callback
			std::string response;
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

			// Perform the request
			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK) {
				std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
			} else {
				std::string clean = response.erase(response.size() - 3);
				std::string output = clean.substr(11, -1);
				sendReply("300 RPL_NONE: " + output + "\r\n");
			}

			// Clean up
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
		}
	} 
};

void Command::nickCommand()
{
	std::string hostname = getMachineHostName();
	std::cout << "\nHOOOO: "  << hostname << std::endl;
	if(this->_client.getPassword() == "")
	{
		sendReply(":"+ getMachineHostName() +" ERROR * :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 1 || this->_args[0] == "")
		sendReply(":"+ getMachineHostName() +" 431 " + _client.getNickname() + ": NICK <nickname>\r\n");
	else if (this->_args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendReply(":"+ getMachineHostName() +" 432 " + _client.getNickname() + ": Nickname invalid\r\n");
	else if(nickExist(this->_args[0]) != -1)
		sendReply(":"+ getMachineHostName() +" 433 " + _client.getNickname()+ " Nickname already in use\r\n");
	else
	{
		if (_client.getNickname() == "" && _client.getUsername() != "")
		{
			sendReply(":"+ hostname + " 001 " + this->_args[0] + " :Welcome to the Internet Relay Network " + this->_args[0] + "!\r\n");
			sendReply(":"+ hostname + " 002 " + this->_args[0] + " :Your host is "+ getMachineHostName() +", running version 0.1\r\n");
			sendReply(":"+ hostname + " 003 " + this->_args[0] + " :This server was created 2019-10-10\r\n");
			sendReply(":"+ hostname + " 004 " + this->_args[0] + " :"+ getMachineHostName() +" 0.1\r\n");
			sendReply(":"+ hostname + " 251 " + this->_args[0] + " :There are 1 users and 1 server\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":"+ hostname + " 372 " + this->_args[0] + " :                        Please enjoy your stay!\r\n");
			sendReply(":"+ hostname + " 372 " + this->_args[0] + " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":"+ hostname + " 376 " + this->_args[0] + " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");
			this->_client.setIsRegistered(true);
		}
		else if(_client.getNickname() != "" && _client.getUsername() != "")
			sendReply(":" + _client.getNickname() + "!" + _client.getUsername() + "@"+ getMachineHostName() +"" + " NICK " + this->_args[0] + "\r\n"); 
		this->_client.setNickname(this->_args[0]); 
	}
};

void	Command::userCommand()
{
	if(this->_client.getPassword() == "")
	{
		sendReply(":"+ getMachineHostName() +" ERROR * :You must enter a password first\r\n");
		return ;
	}
	if (this->_args.size() != 4)
		sendReply(":"+ getMachineHostName() +" 461 " + _client.getNickname() + ": USER <username> <hostname> <servername> <realname>\r\n");
	else if (this->_client.getUsername() != "" && this->_client.getNickname() != "")
		sendReply(":"+ getMachineHostName() +" 462 " + _client.getNickname() + ":You are already registered\r\n");
	else
	{
		if (_client.getNickname() != "" && _client.getUsername() == "")
		{
			sendReply(":"+ getMachineHostName() +" 001 " + _client.getNickname() + " :Welcome to the Internet Relay Network " + _client.getNickname() + "!\r\n");
			sendReply(":"+ getMachineHostName() +" 002 " + _client.getNickname() + " :Your host is "+ getMachineHostName() +", running version 0.1\r\n");
			sendReply(":"+ getMachineHostName() +" 003 " + _client.getNickname() + " :This server was created 2019-10-10\r\n");
			sendReply(":"+ getMachineHostName() +" 251 " + _client.getNickname() +" :There are 1 users and 1 server\r\n");
			sendReply(":"+ getMachineHostName() +" 004 " + _client.getNickname() + " :"+ getMachineHostName() +" 0.1\r\n");                                                                                 
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :         	       _       _                            ___ ____   ____   ____                           \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :              | | ___ (_)_ __     ___  _   _ _ __  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           _  | |/ _ \\| | '_ \\   / _ \\| | | | '__|  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :          | |_| | (_) | | | | | | (_) | |_| | |     | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :           \\___/ \\___/|_|_| |_|  \\___/ \\__,_|_|    |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|   \r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :                        Please enjoy your stay!\r\n");
			sendReply(":"+ getMachineHostName() +" 372 " + _client.getNickname() +  " :you can use bot command (BOT) to get time && jokes && nokta!\r\n");
			sendReply(":"+ getMachineHostName() +" 376 " + _client.getNickname() +  " :Made by hbouqssi && ykhadiri && rgatnaou\r\n");

			this->_client.setIsRegistered(true);
		}
		this->_client.setUsername(this->_args[0]);
	}
};

void	Command::privmsgCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + ": PRIVMSG <nick/channel> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == - 1)
		sendReply(":" + getMachineHostName() + " 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":" + getMachineHostName() + " 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + "" + " PRIVMSG " + this->_args[0] + " " + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0],tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);
		}
		// sendReply(":" + this->_args[0] + "!" + _clients[nickExist(this->_args[0])].getUsername() + "@" + getMachineHostName() + "" + " PRIVMSG " + _client.getNickname() + " :" + this->_args[1] + "\r\n");
	}
}

void	Command::noticeCommand()
{
	if (this->_args.size() != 2 || this->_args[0] == "" )
		sendReply(":" + getMachineHostName() + " 461 " + _client.getNickname() + ": NOTICE <nickname> <message>\r\n");
	else if(nickExist(this->_args[0]) == -1 && _client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
		sendReply(":" + getMachineHostName() + " 401 " + _client.getNickname() + " " + this->_args[0] + " :No such nick/channel\r\n");
	else
	{
		if ((this->_args[0][0] == '#') && !_client.isMemberOfChannel(this->_args[0], this->_client.getFd()))
				sendReply(":" + getMachineHostName() + " 404 " + _client.getNickname() + " " + this->_args[0] + " :Cannot send to channel\r\n");
		else
		{
			std::string tmp(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + "" + " NOTICE " + this->_args[0] + " :" + this->_args[1] + "\r\n");
			if (this->_args[0][0] == '#')
				this->broadcast(this->_args[0],tmp);
			else
				send(_clients[nickExist(this->_args[0])].getFd(), tmp.c_str(), tmp.length(), 0);	
		}
		// sendReply(":" + this->_args[0] + "!" + _clients[nickExist(this->_args[0])].getUsername() + "@" + getMachineHostName() + "" + " NOTICE " + _client.getNickname() + " :" + this->_args[1] + "\r\n");
	}
};

std::vector<int> Command::communClients()
{
	channelMap::iterator it1 = this->_channelObj._channelMap.begin();
	std::vector<int> clients;

	while (it1 != this->_channelObj._channelMap.end())
	{
		this->_channelObj = this->_channelObj._channelMap[it1->first];
		if (this->_channelObj._userMap[_client.getFd()].isMemberOfChannel(it1->first,_client.getFd()) == 1)
		{
			userMap::iterator it2 = this->_channelObj._userMap.begin();
			while (it2 != this->_channelObj._userMap.end())
			{
				if (!std::count (clients.begin(), clients.end(), it2->first))
					clients.push_back(_client.getFd());
				it2++;
			}
		}
		it1++;
	}
	return clients;
}

void sendReplyToCommunClient(std::vector<int> &clients, std::string msg)
{
	std::vector<int>::iterator it = clients.begin();

	while (it != clients.end())
	{
		send(*it, msg.c_str(), msg.length(), 0);
		it++;
	}
}

void Command::quitCommand()
{
	std::string msg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@" + getMachineHostName() + " QUIT " + this->_args[0] + "\r\n";
	std::vector<int> clients = communClients();

	leaveAllChannels();
	sendReplyToCommunClient(clients,msg);
	_client.setIsRegistered(false);
	_client.setNickname("");
	_client.setPassword("");
	_client.setUsername("");
};

int Command::leaveAllChannels()
{
	this->_args.clear();
	this->_args.push_back("");
	channelMap::iterator it = this->_channelObj._channelMap.begin();

	while (it != this->_channelObj._channelMap.end())
	{
		if (this->_client.isMemberOfChannel(it->first, this->_client.getFd()) == 1)
			this->_args[0] += it->first + ",";
		++it;
	}
	if (!this->_args[0].empty())
		this->partCommand();
	else
		return 0;
	return 1;
};

int Command::modeAnalyzer()
{
	if (this->_args[1].size() == 1 || this->_args[1].find_first_not_of("+-sn") == std::string::npos)
		return -1;	
	if ((this->_args[1].find_first_not_of("+-itkol") == std::string::npos))
	{
		char sign = '\0';
		std::string mode;
		int i = -1;

		while (this->_args[1][++i])
		{
			if (!isalpha(this->_args[1][i]))
				sign = this->_args[1][i];
			else
			{
				if (sign)
				{
					mode = std::string(1, sign) + this->_args[1][i];
					std::vector<std::string>::iterator it = std::find(this->modes.begin(), this->modes.end(), mode);
					// std::vector<std::string>::iterator modeFound = std::find(this->_channelObj.getModes(), this->modes.end(), mode);
					if (it == this->modes.end())
						this->modes.push_back(mode);
				}
			}
		}
		return 1;
	}
	return 0;
};

void Command::modeCommand()
{
	if (this->_args.size() && this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
	{
		sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[0] + " :No such channel\r\n"); //ERR_NOSUCHCHANNEL (403)
		return ;
	}
	else
		this->_channelObj = this->_channelObj._channelMap[this->_args[0]];
	if (this->_args.size() == 1)
	{
		sendReply(":" + getMachineHostName() + " 324 " + this->_client.getNickname() + " " + this->_args[0] + " " + this->_channelObj.getModes() + "\r\n"); //RPL_CHANNELMODEIS (324)
		sendReply(":" + getMachineHostName() + " 329 " + this->_client.getNickname() + " " + this->_args[0] + " " + this->_channelObj.getChannelCreationTime() + "\r\n"); //RPL_CREATIONTIME (329)
	}
	else if (this->_args.size() >= 2)
	{
		if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == 1)
		{
			std::cout << this->_client.getNickname() << " : " << this->_channelObj._userMap[this->_client.getFd()].getOpPriviligePermission() <<std::endl;
			if (!this->_channelObj._userMap[this->_client.getFd()].getOpPriviligePermission())
				sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " " + this->_args[0] + " :You must have channel halfop access or above to set channel mode " + this->_args[1][1] + "\r\n"); //ERR_CHANOPRIVSNEEDED (482)
			else
			{
				if (this->modeAnalyzer() == 1)
				{
					std::vector<std::string>::iterator it = this->modes.begin();

					while (it != this->modes.end())
					{
						if ((*it)[1] == 'i') // Invite-Only Mode
						{
							if (*it == "+i" && !this->_channelObj.findMode("+i"))
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +i\r\n");
								this->_channelObj.setMode(*it);
							}
							if (*it == "-i" && this->_channelObj.findMode("+i"))
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -i\r\n");
								this->_channelObj.setMode(*it);
							}
						}
						else if ((*it)[1] == 'l') // Limit-Channel Mode
						{
							if (this->_args.size() == 2 && (*it) == "+l")
							{
								sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " MODE :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
								return ;
							}
							if (this->_args.size() > 1)
							{
								int tmp = atoi(this->_args[2].c_str());
								std::stringstream limitClients(tmp);

								if (*it == "+l" && tmp > 0)
								{
									sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +l " + limitClients.str() + "\r\n");
									// std::string mode = "+l " + limitClients.str();;
									this->_channelObj.setMode("+l");
									this->_channelObj.setLimitUsers(tmp);
								}
								if (*it == "-l" && this->_channelObj.findMode("+l"))
								{
									sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -l\r\n");
									this->_channelObj.setMode("-l");
									this->_channelObj.setLimitUsers(0);
								}
							}
						}
						else if ((*it)[1] == 'o') // Set/Remove Operator Privileges
						{
							int fdClient = this->searchClientByName(this->_args[2]);

							if (this->_args.size() <  3)
							{
								sendReply(":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " MODE :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
								return ;
							}
							if (!fdClient || this->_client.isMemberOfChannel(this->_args[0], fdClient) != 1)
							{
								sendReply(":" + getMachineHostName() + " 401 " + this->_client.getNickname() + " " + this->_args[2] + " :No such nick/channel\r\n"); //ERR_NOSUCHNICK (401)
								return;
							}
							if (*it == "+o")
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +o " + this->_args[2] + "\r\n");
								this->_channelObj._userMap[this->searchClientByName(this->_args[2])].setOpPrivilegePermission(OPERATOR);
							}
							else if (*it == "-o" && this->_client.getOpPriviligePermission())
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -o\r\n");
								this->_channelObj._userMap[this->searchClientByName(this->_args[2])].setOpPrivilegePermission(CLIENT);
							}
						}
						else if ((*it)[1] == 't') // Set/Remove Topic Restrictions
						{
							if (*it == "+t" && !this->_channelObj.findMode("+t"))
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +t\r\n");
								this->_channelObj.setMode("+t");
							}
							else if (*it == "-t" && this->_channelObj.findMode("+t"))
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -t\r\n");
								this->_channelObj.setMode("-t");
							}
						}
						else if ((*it)[1] == 'k') // Set/Remove Channel Key
						{
							if (*it == "+k" && this->_args.size() > 2)
							{
								std::string channelKey = this->_args[2];

								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +k " + channelKey + "\r\n");
								this->_channelObj.setKey(channelKey);
								this->_channelObj.setMode("+k");
							}
							else if (*it == "-k" && this->_channelObj.findMode("+k"))
							{
								sendReply(":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -k\r\n");
								this->_channelObj.setKey("");
								this->_channelObj.setMode("-k");
							}
						}
						
						++it;
					}
				}
				else if (!this->modeAnalyzer())
					sendReply(":" + getMachineHostName() + " 472 " + this->_client.getNickname() + " " + this->_args[1] + " :is not a recognised channel mode.\r\n"); //ERR_UNKNOWNMODE (472)
			}
		}	
	}
	this->_channelObj._channelMap[this->_args[0]] = this->_channelObj;
};

void Command::broadcast( std::string const &channel, std::string const &msg)
{
	userMap::iterator it = this->_channelObj._channelMap[channel]._userMap.begin();

	if (this->_indexCmd == PART)
		 send(_client.getFd(), msg.c_str(), msg.length(), 0);
	while (it != this->_channelObj._channelMap[channel]._userMap.end())
	{
			if((this->_indexCmd == PRIVMSG || this->_indexCmd == NOTICE) && _client.getFd() == it->second.getFd())	
				continue;
			else
				 send(it->second.getFd(), msg.c_str(), msg.length(), 0);
		++it;
	}
};
