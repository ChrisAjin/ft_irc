#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include "user.hpp"
#include "errorcode.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RARROW  "\u2B9E "
#define LARROW  "\u2B9C "

class User;

class Server
{
private:
    int sock_fd;
    int  port;
    std::string	password;
    int	poll_nb;
    int	status;
    int	user_max;
    int sock_opt;
    int	addr_size;
    static bool signal;
    struct pollfd	new_user;
    std::vector<struct pollfd>	poll_fd;
    std::vector<User>	sock_user;
    std::map<int, std::string>	buff;

public:
    Server();
    Server(Server const &obj);
    Server &operator=(Server const &obj);
    ~Server();

    int	getSockFd();
    int	getPort();
    int	getFdByNick(std::string nickname);
    std::string	getPassword();
    User	*getClientByFd(int fd);
    User	*getClientByNickname(std::string nickname);

    void	setSockFd(int sock_fd);
    void	setPort(int port);
    void	setPassword(std::string password);
    void	setNewUser(User newuser);
    void	setPollfd(pollfd fd);

    void	removeClient(int fd);
    void	removeFd(int fd);

    void	notifyUsers(std::string message, int fd);
    void	notifyClient3(int errnum, std::string user, int fd, std::string message);

    void	closeFd();
    bool	isPortValid(std::string port);
    bool	isRegistered(int fd);

    void	Init_Serveur(int port, std::string pass);
    void	checkPollEvents();
    void	acceptNewClient();
    void	handleData(int fd);
    static void	handleSignal(int signum);

    int	splitMessage(std::string message, std::string split_mess[3]);
    int	splitParams(std::string params, std::string split_params[3]);
    std::vector<std::string>	extractParams(std::string &message);
    std::vector<std::string>	dissectMessage(std::string &message);
    std::vector<std::string>	splitBuffer(std::string buff);
    void	processCommand(std::string &command, int fd);
};

#endif