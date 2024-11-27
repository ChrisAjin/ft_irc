#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

#include "server.hpp"
#include "user.hpp"

class User;

class Channel {
private:
    std::string name;                   // Name of the channel
    std::string topic;                  // Topic of the channel
    std::string key;                    // Channel key (password)
    std::string created_at;             // Date and time the channel was created
    int userLimit;                      // Maximum number of users allowed
    bool inviteOnly;                    // Is the channel invite-only?
    bool topicRestricted;               // Is the topic restricted to operators?
    std::set<int> members;              // List of user file descriptors in the channel
    std::set<int> operators;            // List of operator file descriptors
    std::set<int> invitees;             // List of users invited to the channel
    // You
    std::vector<User> sock_user;
    std::vector<User> admin;
    std::vector<std::pair<char, bool> > channelMode;

public:
    std::vector<User> &getUsers();
    std::vector<User> &getOperators();
    // Constructors
    Channel();
    Channel(Channel const &src);
    ~Channel();
    // Getters
    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::string &getKey() const;
    int getUserLimit() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    bool getTopicRestriction() const;
    std::string getCreationDate();

    // Setters
    void setTopic(const std::string &newTopic);
    void setKey(const std::string &newKey);
    void removeKey();
    void setUserLimit(int limit);
    void removeUserLimit();
    void setInviteOnly(bool enable);
    void setTopicRestricted(bool enable);

    // Member management
    void addMember(int fd);
    void removeMember(int fd);
    bool isMember(int fd) const;

    // Operator management
    void addOperator(int fd);
    void removeOperator(int fd);
    bool isOperator(int fd) const;

    // Invitation management
    void addInvite(int fd);
    bool isInvited(int fd) const;
    void removeInvite(int fd);

    User *getUserByFd(int fd);
    User *getOperatorByFd(int fd);

    void removeUserByFd(int fd);
    void removeOperatorByFd(int fd);

    size_t getUserCount();
    bool hasOperators();

    void promoteFirstUserToOperator();

    std::string getChannelName();

    void broadcastMessage(std::string reply);
    void broadcastMessage2(std::string reply, int fd);
    User *getFindUserByName(std::string name);
    std::string getChannelPassword();
    int getInviteOnlyStatus();
    void addUser(User user);
    std::string getTopicName();
    std::string getUserList();
    void setCreationDate();
    void setChannelName(std::string name);
    void addOperatorOnChannel(User user);
    bool isUserOperator(std::string &name);
};

#endif // CHANNEL_HPP
