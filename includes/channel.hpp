#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "server.hpp"
#include "user.hpp"

class User;

class Channel
{

private:
    std::string channelName;
    std::string topicName;
    std::string password;
    std::string created_date;
    int topic;
    int key;
    int limit;
    int invitOnly;
    bool topicRestriction;

    std::vector<User> sock_user;
    std::vector<User> admin;
    std::vector<std::pair<char, bool> > channelMode;

public:
    Channel();
    Channel(Channel const &src);
    Channel &operator=(Channel const &rhs);
    ~Channel();

    std::string getChannelName();
    std::string getTopicName();
    std::string getChannelPassword();
    std::string getUserList();
    std::string getCreationDate();
    std::vector<User> &getUsers();
    std::vector<User> &getOperators();
    User *getUserByFd(int fd);
    User *getOperatorByFd(int fd);
    void setChannelName(std::string name);
    void setTopicName(std::string topic);
    void setChannelPassword(std::string password);
    void setTopicStatus(int topic);
    void setKeyStatus(int key);
    void setUserLimit(int limit);
    void setInviteOnlyStatus(int invitOnly);
    void setTopicRestriction(bool restriction);
    void setChannelMode(size_t i, bool mode);
    void setCreationDate();

    void removeUserByFd(int fd);
    void removeOperatorByFd(int fd);

    size_t getUserCount();
    void checkChannelName(std::string channelName);
    void addOperatorOnChannel(User user);
    void addUser(User user);
    void notifyUsers(std::string msg, User &author);

};

#endif
