# pragma once

#include "utils.hpp"

class user
{   
    private:
        /*********/
        /*PRIVATE*/
        /******* */
        //int _fd;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;
        bool _isOperator;
        bool _isAuthenticated;
        std::vector<channel *> _channels_register;
        struct pollfd _client_pollfd;
        int _login_status;


    public:
        user();
        user(int socket);
        ~user();
        int Get_fd_client() const;
        int Get_login_status() const;
        std::string Get_nickname() const;
        std::string Get_username() const;
        std::string Get_realname() const;
        std::string Get_hostname() const;
        std::vector<channel *> Get_channels_register() const;

        //Set
        void Set_login_status(int status);
        void Set_nickname(std::string nickname);
        void Set_username(std::string username);
        void Set_realname(std::string realname);
        void Set_hostname(std::string hostname);

};