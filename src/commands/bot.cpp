/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:44:22 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/05/24 18:44:23 by ykhadiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Command.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
};

std::string getTime()
{
    std::time_t t = std::time(NULL);
    char time_str[20];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&t));
    return std::string(time_str) + " \r\n";
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
			if (res != CURLE_OK)
				std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
			else
			{
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
