# pragma once

# include "utils.hpp"
class user;
class channel
{
    private:
        /*********/
        /*PRIVATE*/
        /*********/
        std::string _name;
        std::vector<user *> _list_channel_user;
        std::vector<user *> _list_invited_user;
        std::vector<user *> _list_operator;
        std::string _key;
        std::string _topic;

        //channel mode
        bool _invite_only;
        bool _topic_settable;
        bool _channel_private;

        public:



    
};