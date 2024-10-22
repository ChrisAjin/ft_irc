#include "../include/utils.hpp"

bool	server::_parse_privmsg(user *sender, t_IRCMessage cmd)
{
    if (cmd.params.empty())																				// Check if command takes parameters
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
	if (cmd.params.size() == 1)																			// Check if I have minimum 2 parameters (target + message)
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
    if (cmd.params[0][0] == ':')																		// Check if any target is specified
    {
        _Output_client(sender->Get_fd_client(), ERR_NORECIPIENT(_name_serveur));
        return true;
    }
    if (cmd.params[1][0] != ':')																		// Check message start with a ":"
    {
        _Output_client(sender->Get_fd_client(), ERR_NOTEXTTOSEND(_name_serveur));
        return true;
    }	
	return false;
}
int _Output_client(int fd, std::string msg) {
    ssize_t ret;

    msg.append("\r\n");
    std::cout << fd << " >> " << msg << std::endl;  // print send message
    ret = send(fd, msg.c_str(), msg.size(), 0);
    return 0;
};