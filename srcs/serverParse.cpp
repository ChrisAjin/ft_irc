#include "../includes/server.hpp"

/*Avance l'index jusqu'au prochain espace dans la chaîne de caractères*/
void goToNextSpace(std::string s, size_t *i)
{
    while (s[*i] != 0 && s[*i] != ' ')
    {
        ++(*i);
    }
}
/*J'en sais rien j'ai copier coller*/
/*Sépare les paramètres d'une commande en trois parties maximum*/
int Server::splitParams(std::string params, std::string splitParams[3])
{
    size_t i = 0;
    size_t k = 0;
    for (int j = 0; i < params.length() && j < 3; j++)
    {
        k = i;
        i = params.find_first_of(" ", k);
        if (i == std::string::npos
            || (j >= 1 && params[k] == ':'))
        {
            splitParams[j] = params.substr(k);
            break;
        }
        else if (i < params.length()) 
        {
            splitParams[j] = params.substr(k, i - k);
        }
        i += (1 & (params[i] == ' '));
    }
    return (0);
}

/*Sépare un message en trois parties : préfixe, commande, et paramètres*/
int Server::splitMessage(std::string message, std::string split_mess[3])
{
    std::string s_tmp;
    size_t i = 0;

    if (message[i] == ':')
    {
        s_tmp = message.substr(0, message.find_first_of(" "));
        split_mess[0] = s_tmp;
        goToNextSpace(message, &i);
        i += (1 & (message[i] == ' '));
        if (message[i] == 0)
            return (1);
    }
    else
    {
        split_mess[0] = std::string();
    }

    s_tmp = message.substr(i, message.find_first_of(" ", i) - i);
    split_mess[1] = s_tmp;
    goToNextSpace(message, &i);
    i += (1 & (message[i] == ' '));
    if (message[i] == 0)
        return (1);
    else
    {
        s_tmp = message.substr(i);
        split_mess[2].append(s_tmp);
    }
    return (0);
}

/*Analyse le message et le divise en trois parties : préfixe, commande et paramètres*/
std::vector<std::string> Server::dissectMessage(std::string &message)
{
    std::vector<std::string> commandParams;
    std::string split_mess[3] = {std::string(), std::string(), std::string()};

    splitMessage(message, split_mess);

    for (size_t i = 0; i < 3; i++)
    {
        commandParams.push_back(split_mess[i]);
    }

    return commandParams;
}

/*Sépare les paramètres d'un message en utilisant l'espace comme délimiteur*/
std::vector<std::string> Server::extractParams(std::string& message)
{
    std::vector<std::string> param;
    std::stringstream ss(message);
    std::string line;

    while (std::getline(ss, line, ' '))
    {
        param.push_back(line);
    }

    return param;
}

/*Sépare le buff en lignes de commande en utilisant les délimiteurs de fin de ligne (\r\n) car client de ref irssi*/
std::vector<std::string> Server::splitBuffer(std::string buff)
{
    std::vector<std::string> commandParams;
    std::istringstream stm(buff);
    std::string line;
    size_t end;

    while (std::getline(stm, line))
    {
        end = line.find_first_of("\r\n");
        if (end != std::string::npos)
            line.erase(end);

        commandParams.push_back(line);
    }

    return commandParams;
}
// A finir 
// /*Analyse et exécute une commande IRC reçue par le serveur*/
// void Server::processCommand(std::string &message, int fd)
// {
//     std::vector<std::string> commandParams;
//     std::vector<std::string> commandParts;

//     if (message.empty())
//         return;

//     commandParts = dissectMessage(message);

//     commandParams = extractParams(message);

//     size_t nonspace = message.find_first_not_of(" \t\v");
//     if (nonspace != std::string::npos)
//         message = message.substr(nonspace);

//     //COMMANDE PARSE
//     else if (isRegistered(fd) && commandParams.size())
//     {
//         //COMMANDE PARSE
//         //mettre un else if pour chaque commande
//         notifyUsers(ERR_UNKNOWNCOMMAND(getClientByFd(fd)->getNickname(), commandParts[1]), fd);
//     }
//     else if (!isRegistered(fd))
//         notifyUsers(ERR_NOTREGISTERED(std::string("*")), fd);
// }
