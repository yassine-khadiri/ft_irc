/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:45:01 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 16:58:09 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

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
	Channel *channelobj = NULL;

	if (this->_args.size() && this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == -1)
	{
		sendReply(":" + getMachineHostName() + " 403 " + this->_client.getNickname() + " " + this->_args[0] + " :No such channel\r\n"); //ERR_NOSUCHCHANNEL (403)
		return ;
	}
	else
		channelobj = channelobj->_channelMap[this->_args[0]];
	if (this->_args.size() == 1)
	{
		sendReply(":" + getMachineHostName() + " 324 " + this->_client.getNickname() + " " + this->_args[0] + " " + channelobj->getModes() + "\r\n"); //RPL_CHANNELMODEIS (324)
		sendReply(":" + getMachineHostName() + " 329 " + this->_client.getNickname() + " " + this->_args[0] + " " + channelobj->getChannelCreationTime() + "\r\n"); //RPL_CREATIONTIME (329)
	}
	else if (this->_args.size() >= 2)
	{
		if (this->_client.isMemberOfChannel(this->_args[0], this->_client.getFd()) == 1)
		{
			if (!channelobj->_userMap[this->_client.getFd()].getOpPriviligePermission())
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
							if (*it == "+i" && !channelobj->findMode("+i"))
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +i\r\n");
								channelobj->setMode(*it);
							}
							if (*it == "-i" && channelobj->findMode("+i"))
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -i\r\n");
								channelobj->setMode(*it);
							}
						}
						else if ((*it)[1] == 'l') // Limit-Channel Mode
						{
							if (this->_args.size() <= 2 && (*it) == "+l")
							{
								sendReply( ":" + getMachineHostName() + " 482 " + this->_client.getNickname() + " MODE :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
								return ;
							}
							if (this->_args.size() > 1)
							{
								int tmp = atoi(this->_args[2].c_str());
								std::stringstream limitClients(tmp);

								if (*it == "+l" && tmp > 0)
								{
									broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +l " + limitClients.str() + "\r\n");
									channelobj->setMode("+l");
									channelobj->setLimitUsers(tmp);
								}
								if (*it == "-l" && channelobj->findMode("+l"))
								{
									broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -l\r\n");
									channelobj->setMode("-l");
									channelobj->setLimitUsers(0);
								}
							}
						}
						else if ((*it)[1] == 'o') // Set/Remove Operator Privileges
						{
							int fdClient = this->searchClientByName(this->_args[2]);

							if (this->_args.size() <  3)
							{
								sendReply(":" + getMachineHostName() + " 461 " + this->_client.getNickname() + " MODE :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS (461)
								return ;
							}
							if (!fdClient || this->_client.isMemberOfChannel(this->_args[0], fdClient) != 1)
							{
								sendReply(":" + getMachineHostName() + " 401 " + this->_client.getNickname() + " " + this->_args[2] + " :No such nick/channel\r\n"); //ERR_NOSUCHNICK (401)
								return;
							}
							if (*it == "+o" && !channelobj->_userMap[this->searchClientByName(this->_args[2])].getOpPriviligePermission())
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +o " + this->_args[2] + "\r\n");
								channelobj->_userMap[this->searchClientByName(this->_args[2])].setOpPrivilegePermission(OPERATOR);
							}
							else if (*it == "-o" && channelobj->_userMap[this->_client.getFd()].getOpPriviligePermission() && channelobj->_userMap[this->searchClientByName(this->_args[2])].getOpPriviligePermission())
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + getMachineHostName() + " MODE " + this->_args[0] + " -o " +  this->_args[2] + "\r\n");
								channelobj->_userMap[this->searchClientByName(this->_args[2])].setOpPrivilegePermission(CLIENT);
							}
						}
						else if ((*it)[1] == 't') // Set/Remove Topic Restrictions
						{
							if (*it == "+t" && !channelobj->findMode("+t"))
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +t\r\n");
								channelobj->setMode("+t");
							}
							else if (*it == "-t" && channelobj->findMode("+t"))
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -t\r\n");
								channelobj->setMode("-t");
							}
						}
						else if ((*it)[1] == 'k') // Set/Remove Channel Key
						{
							if (*it == "+k" && this->_args.size() > 2)
							{
								std::string channelKey = this->_args[2];

								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " +k " + channelKey + "\r\n");
								channelobj->setKey(channelKey);
								channelobj->setMode("+k");
							}
							else if (*it == "-k" && channelobj->findMode("+k"))
							{
								broadcast(channelobj->getChannelName(), ":" + this->_client.getNickname() + "!" + this->_client.getUsername()+ "@" + getMachineHostName() + " MODE " + this->_args[0] + " -k\r\n");
								channelobj->setKey("");
								channelobj->setMode("-k");
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
	channelobj->_channelMap[this->_args[0]] = channelobj;
};
