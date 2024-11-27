#include "../includes/channel.hpp"

// Constructor

Channel::Channel()
{
    this->name = "";
    this->topic = "";
    this->created_at = "";
    this->key = "";
    this->userLimit = -1;
    this->inviteOnly = false;
    this->topicRestricted = false;
    
    char mode[5] = {'i', 't', 'k', 'o', 'l'};
    for (int i = 0; i < 5; i++)
        channelMode.push_back(std::make_pair(mode[i], false));
}
// Constructeur de copie de la classe Channel
Channel::Channel(Channel const &src)
{
    *this = src;
}

// Destructeur de la classe Channel
Channel::~Channel()
{
}
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

bool Channel::getTopicRestriction() const
{
    return (this->topicRestricted);
}

// Retourne une référence à la liste des utilisateurs non-opérateurs du canal
std::vector<User> &Channel::getUsers()
{
    return (sock_user);
}
// Retourne une référence à la liste des opérateurs du canal
std::vector<User> &Channel::getOperators()
{
    return (admin);
}
User *Channel::getUserByFd(int fd)
{
    for (std::vector<User>::iterator it = sock_user.begin(); it != sock_user.end(); ++it)
    {
        if (it->getFduser() == fd)
            return &(*it);
    }
    return (NULL);
}

void Channel::removeUserByFd(int fd)
{
    for (std::vector<User>::iterator it = sock_user.begin(); it != sock_user.end(); ++it)
    {
        if (it->getFduser() == fd)
        {
            sock_user.erase(it);
            break;
        }
    }
}

void Channel::removeOperatorByFd(int fd)
{
    for (std::vector<User>::iterator it = admin.begin(); it != admin.end(); ++it)
    {
        if (it->getFduser() == fd)
        {
            admin.erase(it);
            break;
        }
    }
}

size_t Channel::getUserCount()
{
    size_t num = this->sock_user.size() + this->admin.size();
    return (num);
}

bool Channel::hasOperators()
{
    if (admin.empty())
        return (false);
    return (true);
}

void Channel::promoteFirstUserToOperator()
{
    admin.push_back(sock_user[0]);
    sock_user.erase(sock_user.begin());
}

std::string Channel::getChannelName()
{
    return (name);
}

// Envoie un message à tous les utilisateurs et opérateurs présents dans le canal
void Channel::broadcastMessage(std::string reply)
{
    for (size_t i = 0; i < admin.size(); i++)
    {
        if (send(admin[i].getFduser(), reply.c_str(), reply.size(), 0) == -1)
            std::cerr << "send() failed" << std::endl;
    }
    for (size_t i = 0; i < sock_user.size(); i++)
    {
        if (send(sock_user[i].getFduser(), reply.c_str(), reply.size(), 0) == -1)
            std::cerr << "send() failed" << std::endl;
    }
}

// Envoie un message à tous les utilisateurs et opérateurs présents dans le canal sauf un
void Channel::broadcastMessage2(std::string reply, int fd)
{
    for (size_t i = 0; i < admin.size(); i++)
    {
        if (admin[i].getFduser() != fd)
        {
            if (send(admin[i].getFduser(), reply.c_str(), reply.size(), 0) == -1)
                std::cerr << "send() failed" << std::endl;
        }
    }
    for (size_t i = 0; i < sock_user.size(); i++)
    {
        if (sock_user[i].getFduser() != fd)
        {
            if (send(sock_user[i].getFduser(), reply.c_str(), reply.size(), 0) == -1)
                std::cerr << "send() failed" << std::endl;
        }
    }
}

User *Channel::getFindUserByName(std::string name)
{
    for (std::vector<User>::iterator it = sock_user.begin(); it != sock_user.end(); ++it)
    {
        if (it->getNickname() == name)
            return &(*it);
    }
    for (std::vector<User>::iterator it = admin.begin(); it != admin.end(); ++it)
    {
        if (it->getNickname() == name)
            return &(*it);
    }
    return (NULL);
}

std::string Channel::getChannelPassword()
{
    return (this->key);
}

int Channel::getInviteOnlyStatus()
{
    return this->inviteOnly;
}

void Channel::addUser(User user)
{
    sock_user.push_back(user);
}

std::string Channel::getTopicName()
{
    return (topic);
}

std::string Channel::getUserList()
{
    std::string list;

    for (size_t i = 0; i < admin.size(); i++)
    {
        list += "@" + admin[i].getNickname();
        if ((i + 1) < admin.size())
            list += " ";
    }
    if (sock_user.size())
        list += " ";
    for (size_t i = 0; i < sock_user.size(); i++)
    {
        list += sock_user[i].getNickname();
        if ((i + 1) < sock_user.size())
            list += " ";
    }

    return (list);
}

void Channel::setCreationDate()
{
    std::time_t _time = std::time(NULL);
    std::ostringstream oss;
    oss << _time;
    this->created_at = std::string(oss.str());
}
void Channel::setChannelName(std::string name)
{
    this->name = name;
}
void Channel::addOperatorOnChannel(User user)
{
    admin.push_back(user);
}

User *Channel::getOperatorByFd(int fd)
{
    for (std::vector<User>::iterator it = admin.begin(); it != admin.end(); ++it)
    {
        if (it->getFduser() == fd)
            return &(*it);
    }
    return (NULL);
}

bool Channel::isUserOperator(std::string &name)
{
    for (std::vector<User>::iterator it = admin.begin(); it != admin.end(); ++it)
    {
        if (it->getNickname() == name)
            return (true);
    }
    return (false);
}

std::string Channel::getCreationDate()
{
    return (this->created_at);
}