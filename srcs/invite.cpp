#include "../include/utils.hpp"

// bool server::_parse_invite(user *sender, t_IRCMessage cmd)
// {
//     if (cmd.params.empty() || cmd.params.size() < 2)												// Check if command takes parameters
//     {
// 		_Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "INVITE"));
//         return true;
//     }
//     if (Check_valid_channel(cmd.params[1]) == false)													// Check if 2nd parameter is a Channel
//     {
//         _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "INVITE"));
//         return true;
//     }
// 	return false;
// }
int _Output_client(int fd, std::string msg) {
    ssize_t ret;

    msg.append("\r\n");
    std::cout << fd << " >> " << msg << std::endl;  // print send message
    ret = send(fd, msg.c_str(), msg.size(), 0);
    return 0;
};