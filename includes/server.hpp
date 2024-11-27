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
#include "channel.hpp" // Include the Channel class.
#include "errorcode.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RARROW  "\u2B9E "
#define LARROW  "\u2B9C "

class User;
class Channel;

class Server
{
private:
    // Server properties
    int sock_fd;                            // Socket file descriptor for the server
    int port;                               // Port the server listens on
    std::string password;                   // Server password
    int poll_nb;                            // Number of poll events to monitor
    int status;                             // Status of the poll operation
    int user_max;                           // Maximum number of users allowed
    int sock_opt;                           // Socket options
    int addr_size;                          // Address size for sockaddr structures
    static bool signal;                     // Signal flag for handling termination

    struct pollfd new_user;                 // Pollfd structure for a new user
    std::vector<struct pollfd> poll_fd;     // List of pollfd structures
    std::vector<User> sock_user;            // List of connected users
    std::vector<Channel>	channel;
    std::map<int, std::string> buff;        // Buffers for user input

    // Channel management
    std::map<std::string, Channel> channels; // Map of channels by name
    // pour quand les channels seront crees
public:
    // Constructors and destructor
    Server();
    Server(Server const &obj);
    Server &operator=(Server const &obj);
    ~Server();

    // Getters
    int getSockFd();
    int getPort();
    int getFdByNick(std::string nickname);
    std::string getPassword();
    User *getClientByFd(int fd);
    User *getClientByNickname(std::string nickname);
    // Channel *getChannelByName(const std::string &name); // Get a channel by name
    //pareil
    // Setters
    void setSockFd(int sock_fd);
    void setPort(int port);
    void setPassword(std::string password);
    void setNewUser(User newuser);
    void setPollfd(pollfd fd);

    // User and socket management
    void removeClient(int fd);
    void removeFd(int fd);

    // Notification methods
    void notifyUsers(std::string message, int fd);
    void notifyClient2(int errnum, std::string user, std::string channel, int fd, std::string message);
    void notifyClient3(int errnum, std::string user, int fd, std::string message);

    // Server lifecycle
    void closeFd();
    bool isPortValid(std::string port);
    bool isRegistered(int fd);

    Channel* getChannelByName(const std::string& name);


    // Server and client command handling
    void Init_Serveur(int port, std::string pass);
    void checkPollEvents();
    void acceptNewClient();
    void handleData(int fd);
    static void handleSignal(int signum);

    // Command parsing utilities
    int splitMessage(std::string message, std::string split_mess[3]);
    int splitParams(std::string params, std::string split_params[3]);
    std::vector<std::string> extractParams(std::string &message);
    std::vector<std::string> dissectMessage(std::string &message);
    std::vector<std::string> splitBuffer(std::string buff);
    void processCommand(std::string &command, int fd);

    // IRC-specific commands
    void kickUser(int fd, const std::string &channelName, const std::string &targetUserName, const std::string &reason);
    void inviteUser(int fd, const std::string &channelName, const std::string &targetUserName);
    void topic(int fd, const std::string &channelName, const std::string &topic);
    void mode(int fd, const std::string &channelName, const std::string &mode, const std::string &param);

    // Command handlers
    void	PASS(std::string message, int fd);

    void	NICK(std::string message, int fd);
    bool	isNicknameUsed(std::string &nickname);
    bool	validNickname(std::string &nickname);
    void	updateNicknameChannel(std::string old, std::string n_nick);

    void	USER(std::string &message, int fd);

    void	    QUIT(std::string message, int fd);
    std::string	quitReason(std::string message);
    void	    quitReason2(std::string message, std::string str, std::string &reason);

    void clearChannel(int fd);

    void	PING(std::string &message, int fd);

    void	JOIN(std::string message, int fd);
    int	    splitJoinParams(std::vector<std::pair<std::string, std::string> > &param, std::string message, int fd);
    void	addClientToExistChannel(std::vector<std::pair<std::string, std::string> > &param, int i , int j, int fd);
    void	createAndAddToNewChannel(std::vector<std::pair<std::string, std::string> >&param, int i, int fd);
    int	    channelUserCount(std::string user);
    bool	isUserInvited(User *user, std::string channel, int flag);

    void	TOPIC(std::string message, int fd);
    Channel	*getChannel(std::string name);
};

#endif // SERVER_HPP
