#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

class Channel {
private:
    std::string name;                   // Name of the channel
    std::string topic;                  // Topic of the channel
    std::string key;                    // Channel key (password)
    int userLimit;                      // Maximum number of users allowed
    bool inviteOnly;                    // Is the channel invite-only?
    bool topicRestricted;               // Is the topic restricted to operators?
    std::set<int> members;              // List of user file descriptors in the channel
    std::set<int> operators;            // List of operator file descriptors
    std::set<int> invitees;             // List of users invited to the channel

public:
    // Constructors
    Channel(const std::string &name);

    // Getters
    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::string &getKey() const;
    int getUserLimit() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;

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
};

#endif // CHANNEL_HPP
