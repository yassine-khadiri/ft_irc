/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:45:37 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/06/17 13:07:43 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

void Command::passCommand()
{
	if (this->_client.getIsRegistered())
		sendReply(":" + getMachineHostName() + " 462 * :You are already registered\r\n");
	else if(this->_client.getPassword() != "")
		sendReply(":" + getMachineHostName() + " 462 * :You are already give me password\r\n");
	else
	{
		if(this->_args.size() < 1 || this->_args[0] == "")
			sendReply(":" + getMachineHostName() + " 461 * :PASS <password>\r\n");
		else if (this->_args[0] == this->_pass)
			this->_client.setPassword(this->_args[0]);
		else
			sendReply(":" + getMachineHostName() + " 464 * :Password incorrect\r\n");
	}
};
