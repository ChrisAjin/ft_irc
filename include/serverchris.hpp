#pragma once

# include "utils.hpp"

#define PORT 6667
#define MAX_CLIENTS 100

class user;
class channel;

class server
{

    private:
        const std::string   _name_serveur;
        // List des utilisateurs connecter au serveur
        std::vector<user *> _list_user;
        /* COMMAND*/

        bool                     _parse_privmsg(user *sender, struct s_IRCMessage cmd);
                    /* Notice */
        bool                     _parse_notice(struct s_IRCMessage cmd);
                    /* Invite */
        bool	                 _parse_invite(user *sender, struct s_IRCMessage cmd);
                    /* Kick */
        bool	                 _parse_kick(user *sender, struct s_IRCMessage cmd);
        
                    /* BOT */
        //void                     _Bot_main(user *sender, struct s_IRCMessage cmd);

        /* GET USER */
        user                     *_Get_user_by_fd(int fd);

       
    public:

        //COMMAND
        int Check_command(user *user, std::string str);

};