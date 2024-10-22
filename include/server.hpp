#pragma once

# include "utils.hpp"

class user;
class channel;

class server
{

    private:
        const std::string   _name_serveur;
        /* COMMAND*/

        bool                     _parse_privmsg(user *sender, struct s_IRCMessage cmd);
                    /* Notice */
        bool                     _parse_notice(struct s_IRCMessage cmd);
                    /* Invite */
        bool	                 _parse_invite(user *sender, struct s_IRCMessage cmd);
                    /* Kick */
        bool	                 _parse_kick(user *sender, struct s_IRCMessage cmd);
        
                    /* BOT */
        void                     _Bot_main(user *sender, struct s_IRCMessage cmd);
       
    public:
};