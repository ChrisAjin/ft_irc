#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#define PORT 6667
#define MAX_CLIENTS 100
struct Client {
    int fd;
    std::string nickname;
    std::string username;
    bool isAuthenticated;
    bool isOperator;
};
struct Channel {
    std::string name;
    std::vector<Client*> members;
};
std::vector<struct pollfd> poll_fds;
std::map<int, Client> clients;
std::map<std::string, Channel> channels;
void setupServer(int &server_fd);
void handleNewConnection(int server_fd);
void handleClientMessage(int client_fd);
void authenticateClient(Client &client, const std::string &message);
void joinChannel(Client &client, const std::string &channelName);
void sendMessageToChannel(const std::string &channelName, const std::string &message, int sender_fd);
void handleOperatorCommand(Client &client, const std::string &command);
#endif