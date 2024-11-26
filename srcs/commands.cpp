#include "../includes/server.hpp"
#include"../includes/errorcode.hpp"
#include"../includes/channel.hpp"
//des erreurs car no channel atm

void Server::kickUser(int fd, const std::string &channelName, const std::string &targetUserName, const std::string &reason)
{
    Channel *channel = getChannelByName(channelName);
    if (!channel)
    {
        notifyUsers(ERR_NOSUCHCHANNEL(targetUserName, channelName), fd);
        return;
    }

    if (!channel->isOperator(fd))
    {
        notifyUsers(ERR_CHANOPRIVSNEEDED(channelName), fd);
        return;
    }

    int targetFd = getFdByNick(targetUserName);
    if (targetFd == -1 || !channel->isMember(targetFd))
    {
        notifyUsers(ERR_USERNOTINCHANNEL(targetUserName, channelName), fd);
        return;
    }

    channel->removeMember(targetFd);
    notifyUsers(":" + getClientByFd(fd)->getNickname() + " KICK " + channelName + " " + targetUserName + " :" + reason, -1);
    close(targetFd); // Optionally close the socket if disconnecting the user entirely.
}

void Server::inviteUser(int fd, const std::string &channelName, const std::string &targetUserName)
{
    Channel *channel = getChannelByName(channelName);
    if (!channel)
    {
        notifyUsers(ERR_NOSUCHCHANNEL(targetUserName, channelName), fd);
        return;
    }

    if (!channel->isOperator(fd))
    {
        notifyUsers(ERR_CHANOPRIVSNEEDED(channelName), fd);
        return;
    }

    int targetFd = getFdByNick(targetUserName);
    if (targetFd == -1)
    {
        notifyUsers(ERR_NOSUCHNICK(targetUserName), fd);
        return;
    }

    channel->addInvite(targetFd);
    notifyUsers(":" + getClientByFd(fd)->getNickname() + " INVITE " + targetUserName + " " + channelName + "\r\n", -1);
}

void Server::notifyUsers(std::string message, int fd)
{
    std::cout << RESET << message;
    if (send(fd, message.c_str(), message.size(), 0) == -1)
        std::cerr << "send() failed" << std::endl;
}

void Server::topic(int fd, const std::string &channelName, const std::string &topic)
{
    Channel *channel = getChannelByName(channelName);
    User *user = this->getClientByFd(fd);

    if (!channel)
    {
        notifyUsers(ERR_NOSUCHCHANNEL(user->getNickname(), channelName), fd);
        return;
    }

    if (topic.empty())
    {
        notifyUsers(":" + channelName + " :" + channel->getTopic() + "\r\n", fd);
    }
    else
    {
        if (channel->isTopicRestricted() && !channel->isOperator(fd))
        {
            notifyUsers(ERR_CHANOPRIVSNEEDED(channelName), fd);
            return;
        }
        channel->setTopic(topic);
        notifyUsers(":" + getClientByFd(fd)->getNickname() + " TOPIC " + channelName + " :" + topic + "\r\n", -1);
    }
}

void Server::mode(int fd, const std::string &channelName, const std::string &mode, const std::string &param)
{
    Channel *channel = getChannelByName(channelName);
    User    *user = getClientByFd(fd);

    if (!channel)
    {
        notifyUsers(ERR_NOSUCHCHANNEL(user->getNickname(), channelName), fd);
        return;
    }

    if (!channel->isOperator(fd))
    {
        notifyUsers(ERR_CHANOPRIVSNEEDED(channelName), fd);
        return;
    }

    if (mode == "+i")
    {
        channel->setInviteOnly(true);
        notifyUsers(":" + channelName + " :Invite-only mode enabled\r\n", fd);
    }
    else if (mode == "-i")
    {
        channel->setInviteOnly(false);
        notifyUsers(":" + channelName + " :Invite-only mode disabled\r\n", fd);
    }
    else if (mode == "+t")
    {
        channel->setTopicRestricted(true);
        notifyUsers(":" + channelName + " :Topic restriction enabled\r\n", fd);
    }
    else if (mode == "-t")
    {
        channel->setTopicRestricted(false);
        notifyUsers(":" + channelName + " :Topic restriction disabled\r\n", fd);
    }
    else if (mode == "+k")
    {
        channel->setKey(param);
        notifyUsers(":" + channelName + " :Channel key set\r\n", fd);
    }
    else if (mode == "-k")
    {
        channel->removeKey();
        notifyUsers(":" + channelName + " :Channel key removed\r\n", fd);
    }
    else if (mode == "+l")
    {
        int limit = std::atoi(param.c_str()); // C++98-compatible conversion
        channel->setUserLimit(limit);
        std::ostringstream oss;
        oss << limit; // Convert limit to string
        notifyUsers(":" + channelName + " :User limit set to " + oss.str() + "\r\n", fd);
    }
    else if (mode == "-l")
    {
        channel->removeUserLimit();
        notifyUsers(":" + channelName + " :User limit removed\r\n", fd);
    }
    else if (mode == "+o")
    {
        int targetFd = getFdByNick(param);
        if (targetFd == -1)
        {
            notifyUsers(ERR_NOSUCHNICK(param), fd);
            return;
        }
        channel->addOperator(targetFd);
        notifyUsers(":" + param + " :Granted operator status\r\n", fd);
    }
    else if (mode == "-o")
    {
        int targetFd = getFdByNick(param);
        if (targetFd == -1)
        {
            notifyUsers(ERR_NOSUCHNICK(param), fd);
            return;
        }
        channel->removeOperator(targetFd);
        notifyUsers(":" + param + " :Removed operator status\r\n", fd);
    }
    else
    {
        notifyUsers(ERR_UNKNOWNMODE(user->getNickname(), channelName,  mode), fd);
    }
}

void Server::processCommand(std::string &command, int fd)
{
    std::vector<std::string> commandParts = dissectMessage(command);
    std::vector<std::string> commandParams = extractParams(command);

    if (commandParts.size() < 2)
    {
        notifyUsers("Invalid command.\r\n", fd);
        return;
    }

    std::string action = commandParts[1]; // The actual command (e.g., KICK, INVITE, etc.)

    if (action == "KICK" && commandParams.size() >= 3)
    {
        kickUser(fd, commandParams[0], commandParams[1], commandParams.size() > 2 ? commandParams[2] : "");
    }
    else if (action == "INVITE" && commandParams.size() >= 2)
    {
        inviteUser(fd, commandParams[0], commandParams[1]);
    }
    else if (action == "TOPIC" && commandParams.size() >= 1)
    {
        topic(fd, commandParams[0], commandParams.size() > 1 ? commandParams[1] : "");
    }
    else if (action == "MODE" && commandParams.size() >= 2)
    {
        mode(fd, commandParams[0], commandParams[1], commandParams.size() > 2 ? commandParams[2] : "");
    }
    else
    {
        notifyUsers("Unknown command: " + action + "\r\n", fd);
    }
}
