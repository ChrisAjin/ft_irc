#include "../include/user.hpp"

user::user(){}

user::user(int socket)
{
    _client_pollfd.fd = socket;
    _client_pollfd.revents = 0;
    _client_pollfd.events = POLLIN;
    _login_status = 0;
    _isOperator = false;
    _isAuthenticated = false;
    std::cout << "User created with fd :"<< _client_pollfd.fd << std::endl;
}
user::~user(){
    std::cout << "User destroy with fd :"<< _client_pollfd.fd << std::endl;
    close(_client_pollfd.fd);
}
/********/
/*GETTER*/
/********/
int user::Get_fd_client() const
{
    return _client_pollfd.fd;
};

int user::Get_login_status() const
{
    return _login_status;
};

std::string user::Get_nickname() const
{
    return _nickname;
};

std::string user::Get_username() const
{
    return _username;
};

std::string user::Get_realname() const
{
    return _realname;
};

std::string user::Get_hostname() const
{
    return _hostname;
};

std::vector<channel *> user::Get_channels_register() const
{
    return _channels_register;
};

/********/
/*SETTER*/
/********/
void user::Set_login_status(int status)
{
    _login_status = status;
};
void user::Set_nickname(std::string nickname)
{
    _nickname = nickname;
};
void user::Set_username(std::string username)
{
    _username = username;
};
void user::Set_realname(std::string realname)
{
    _realname = realname;
};
void user::Set_hostname(std::string hostname)
{
    _hostname = hostname;
};