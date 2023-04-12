/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:40:58 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/12 22:08:27 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Ircserv.hpp"

int isNumber(std::string str)
{
    int i = -1;

    while (str[++i])
    {
        if (!isdigit(str[i]))
            return 0;   
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        int port  = isNumber(argv[1]);
		if (!port)
			std::cerr << "The Port Must Be A Positive Integer :)" << std::endl;
		else
		{
	        Ircserv ircserv = Ircserv(atoi(argv[1]), argv[2]);
			ircserv.waitForConnection();
		}
    }
    else
        std::cerr << "Try Enter Two Args!" << std::endl;
    return 0;
}