#include "../include/utils.hpp"

/*********************/
/****** PARSING ******/
/*********************/

int parsing(int argc, char **argv) {

   // 3 Arguments
    if (argc != 3) {
        std::cerr << "The executable will run as follows:\n\t./ircserv <port> <password>" << std::endl;
        return 1;
    }

    // Port only contain number (0 - 9)
    // Port's range (1024 - 65535) 16-bits
    std::string valid_port(argv[1]);
    if (!isNumber(valid_port)) {
        std::cerr << "The port must be an int" << std::endl;
        return 1;
    }
    int range_port = std::atoi(argv[1]);
    if (range_port < 1024 || range_port > 65535) {
        std::cerr << "The port's range must be between 1024 and 65535" << std::endl;
        return 1;
    }

    // password lenght: 1 - 12 char
    // only 0-9 a-z A-Z
    std::string password(argv[2]);
    if (password.length() < 1 || password.length() > 12) {
        std::cerr << "Password's lenght must be between 1 and 12" << std::endl;
        return 1;
    }
    for(size_t i = 0; i < password.length(); i++) {
		if (!isalnum(password[i])) {
			std::cerr << "Password doesn't contains only alphanumerics characters :\n(0 - 9)\t(a - z)\t(A - Z)" << std::endl;
			return 1;
		}
    }
    return 0;
};

bool isNumber(const std::string& str) {
    for (size_t i = 0; i < (str.size()); i++) {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
};

bool Compare_case_sensitive(std::string str1, std::string str2) {
    if (str1.size() != str2.size())
        return false;
    for (size_t i = 0; i < str1.size(); i++) {
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;
    }
    return true;
};

int Stoi(std::string str) {
    int res = 0;
    for (size_t i = 0; i < str.size(); i++) {
        res = res * 10 + (str[i] - '0');
    }
    return res;
};

std::string Itos(int nb) {
    std::string rev;

    while ((nb / 10) > 0) {
        rev += (nb % 10) + '0';
        nb = nb / 10;
    }
    rev += (nb % 10) + '0';

    std::string res;
    for (int i = rev.size(); i >= 0; i--) {
        res += rev[i];
    }

    return res;
}
