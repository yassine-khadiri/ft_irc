#include "IrcClient.hpp"
Client::Client()
{

};

Client::~Client()
{

};

void Client::setPort(int _port)
{
    this->port = _port;
};

void Client::setFd(int _fd)
{
    this->fd = _fd;
};

void Client::setUsername(std::string _username)
{
    this->username = _username;
};

void Client::setNickname(std::string _nickname)
{
    this->nickname = _nickname;   
};

void Client::setPassword(std::string _password)
{
    this->password = _password;
};

int Client::getPort() const
{
    return this->port;

};

int Client::getFd() const
{
    return this->fd;
};

std::string Client::getUsername() const
{
    return this->username;
};

std::string Client::getNickname() const
{
    return this->nickname;
};

std::string Client::getPassword() const
{
    return this->password;
};