/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:43:41 by ykhadiri          #+#    #+#             */
/*   Updated: 2023/04/20 12:46:25 by hbouqssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ircserv.hpp"
#include "../includes/IrcClient.hpp"

Ircserv::Ircserv( int port, std::string password ): Tcp(port), password(password)
{
};

Ircserv::~Ircserv()
{
};

int Ircserv::createServerSocket()
{
    if (!this->initialize())
    {
        if (bind(this->socket_fd, (sockaddr *)&this->addr, sizeof(this->addr)))
        {
            std::cerr << "Error Binding Socket...!" << std::endl;
            return EXIT_FAILURE;
        }

        if (listen(this->socket_fd, 1))
        {
            std::cerr << "Error Listening...!" << std::endl;
            return EXIT_FAILURE;
        }
        this->waitForConnection();
    }
    return EXIT_FAILURE;
};

int Ircserv::checkMessageInfos( std::string recvMessage )
{
    std::cout << "content: " << recvMessage << std::endl;
    return 0;  
};
void getClientInfos(Client &_client, char *buff)
{
    std::string buffer(buff);
    size_t NickIndex = buffer.find("NICK");
    size_t PassIndex = buffer.find("PASS");
    if(NickIndex != -1)
    {
        std::string  nickname = buffer.substr(NickIndex + 5);
        _client.setNickname(nickname);
    }
    if(PassIndex != -1)
    {
        std::string password = buffer.substr(PassIndex + 5);
        _client.setPassword(password);
    }
    // std::cout << "NICK IS: " << _client.getNickname() << "\n" << "PASS IS: " << _client.getPassword();
}
std::string replyToServer(Client& client, const std::string& str)
{
    return ":" + client.getNickname() + " JOIN " + str + "\r\n";
}

int Ircserv::waitForConnection()
{
    Client client;
    int socketClient;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buff[1024] = {0};

    std::cout << "Waiting For Incoming Connection...!" << std::endl;
    socketClient = accept(this->socket_fd, (sockaddr *)&clientAddr, &clientAddrSize);
    while(1)
    {
        if (recv(socketClient, buff, sizeof(buff), 0) > 0)
        {
            std::cout << buff << std::endl;
            getClientInfos(client, buff);
            
            if(strncmp(buff,"JOIN",4) == 0)
            {
                 std::string tmp1 = "Hello";
                if (send(socketClient, tmp1.c_str(), tmp1.length(), 0) < 0)  return EXIT_FAILURE;
            }

            // std::string tmp1 = ":127.0.0.1 001 rgatnaou Welcome to the Internet Relay Network rgatnaou!rgatnaou@127.0.0.1";
            // this->checkMessageInfos(buff);
            // exit(0);
        }
            // exit(0);
    }
    return EXIT_SUCCESS;
};

