#include "../includes/channel.hpp"

// Constructor
Channel::Channel(const std::string &name) : name(name), userLimit(-1), inviteOnly(false), topicRestricted(false) {}

// Getters
const std::string &Channel::getName() const {
    return name;
}

const std::string &Channel::getTopic() const {
    return topic;
}

const std::string &Channel::getKey() const {
    return key;
}

int Channel::getUserLimit() const {
    return userLimit;
}

bool Channel::isInviteOnly() const {
    return inviteOnly;
}

bool Channel::isTopicRestricted() const {
    return topicRestricted;
}

// Setters
void Channel::setTopic(const std::string &newTopic) {
    topic = newTopic;
}

void Channel::setKey(const std::string &newKey) {
    key = newKey;
}

void Channel::removeKey() {
    key.clear();
}

void Channel::setUserLimit(int limit) {
    userLimit = limit;
}

void Channel::removeUserLimit() {
    userLimit = -1; // No limit
}

void Channel::setInviteOnly(bool enable) {
    inviteOnly = enable;
}

void Channel::setTopicRestricted(bool enable) {
    topicRestricted = enable;
}

// Member management
void Channel::addMember(int fd) {
    members.insert(fd);
}

void Channel::removeMember(int fd) {
    members.erase(fd);
}

bool Channel::isMember(int fd) const {
    return members.find(fd) != members.end();
}

// Operator management
void Channel::addOperator(int fd) {
    operators.insert(fd);
}

void Channel::removeOperator(int fd) {
    operators.erase(fd);
}

bool Channel::isOperator(int fd) const {
    return operators.find(fd) != operators.end();
}

// Invitation management
void Channel::addInvite(int fd) {
    invitees.insert(fd);
}

bool Channel::isInvited(int fd) const {
    return invitees.find(fd) != invitees.end();
}

void Channel::removeInvite(int fd) {
    invitees.erase(fd);
}
