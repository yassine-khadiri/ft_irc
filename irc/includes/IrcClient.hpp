#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "../../tcp/includes/Tcp.hpp"

class Client
{
    private:
    std::string username;
    std::string nickname;
    std::string password;
    int port;
    int fd;

    public:
    Client();
    ~Client();
    int getPort() const;
    int getFd() const;
    std::string getUsername() const;
    std::string getNickname() const;
    std::string getPassword() const;

    void setPort(int _port);
    void setFd(int _fd);
    void setUsername(std::string _username);
    void setNickname(std::string _nickname);
    void setPassword(std::string _password);
};

#endif