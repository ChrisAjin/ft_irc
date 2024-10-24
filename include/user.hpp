# pragma once

#include "utils.hpp"

class user
{   
    private:
        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool _isOperator;
        bool _isAuthenticated;
        std::vector<channel *> _channels_register;
        struct pollfd _client_pollfd;
    public:
        //getter setting relou de bz 
};