#pragma once

# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <cerrno>
# include <fcntl.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstdlib>
# include <string.h>
# include <sstream>
# include <poll.h>
# include <vector>
# include <csignal>
# include <algorithm>
# include <cctype>
# include <ctime>

# include "server.hpp"
# include "user.hpp"
# include "error_msg.hpp"

typedef struct s_IRCMessage {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
}				t_IRCMessage;

// Fonctions de parsing
int     parsing(int argc, char **argv);
bool    isNumber(const std::string& s);
bool    Compare_case_sensitive(std::string str1, std::string str2);
int     Stoi(std::string str);
std::string Itos(int nb);
