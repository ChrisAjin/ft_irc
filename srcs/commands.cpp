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
    std::vector<std::string> commandParams;
    std::vector<std::string> commandParts;

    if (command.empty())
        return;
    commandParts = dissectMessage(command);

    commandParams = extractParams(command);
    if (commandParams.size() < 2)
    {
        notifyUsers(ERR_NOTENOUGHPARAMETERS(std::string("*")), fd);
        return;
    }
    size_t nonspace = command.find_first_not_of(" \t\v");
    if (nonspace != std::string::npos)
        command = command.substr(nonspace);
    
    //COMMANDE PARSE
    if ((commandParts[1] == "PASS" || commandParts[1] == "pass") && commandParams.size())
        PASS(command, fd);
    else if ((commandParts[1] == "NICK" || commandParts[1] == "nick") && commandParams.size())
        NICK(command, fd);
    else if ((commandParts[1] == "USER" || commandParts[1] == "user") && commandParams.size())
        USER(command, fd);
    else if ((commandParts[1] == "QUIT" || commandParts[1] == "quit") && commandParams.size())
        QUIT(command, fd);
    else if ((commandParts[1] == "PING" || commandParts[1] == "ping") && commandParams.size())
        PING(command, fd);
    else if ((commandParts[1] == "CAP" || commandParts[1] == "cap") && commandParams.size())
        return;
    else if (isRegistered(fd) && commandParams.size())
    {
        if (commandParts[1] == "JOIN" || commandParts[1] == "join")
            JOIN(command, fd);
        else if (commandParts[1] == "INVITE" || commandParts[1] == "invite")
            inviteUser(fd, commandParams[0], commandParams[1]);
        else if (commandParts[1] == "WHO" || commandParts[1] == "who")
            return;
        else if (commandParts[1] == "WHOIS" || commandParts[1] == "whois")
            return;
        else if (commandParts[1] == "TOPIC" || commandParts[1] == "topic")
            topic(fd, commandParams[0], commandParams.size() > 1 ? commandParams[1] : "");
        // else if (commandParts[1] == "PART" || commandParts[1] == "part")
        //     PART(command, fd);
        else if (commandParts[1] == "KICK" || commandParts[1] == "kick")
            kickUser(fd, commandParams[0], commandParams[1], commandParams.size() > 2 ? commandParams[2] : "");
        else if ((commandParts[1] == "MODE" || commandParts[1] == "mode") && commandParams[1][0] == '#')
            mode(fd, commandParams[0], commandParams[1], commandParams.size() > 2 ? commandParams[2] : "");
        else if ((commandParts[1] == "MODE" || commandParts[1] == "mode") && commandParams[1][0] != '#')
            return;
        // else if (commandParts[1] == "PRIVMSG" || commandParts[1] == "privmsg")
        //     PRIVMSG(command, fd);
        else
            notifyUsers(ERR_UNKNOWNCOMMAND(getClientByFd(fd)->getNickname(), commandParts[1]), fd);
    }
    else if (!isRegistered(fd))
        notifyUsers(ERR_NOTREGISTERED(std::string("*")), fd);
}
