#include "../include/utils.hpp"

bool	server::_parse_notice(t_IRCMessage cmd)
{
    if (cmd.params.empty()) {					// Check if command takes parameters
        std::cerr << "ERROR : No parameter from NOTICE" << std::endl;	
        return true;
	}
	if (cmd.params.size() == 1)	{				// Check if I have minimum 2 parameters (target + message)
        std::cerr << "ERROR : Not enough from NOTICE" << std::endl;	
        return true;
	}
    if (cmd.params[0][0] == ':') {				// Check if any target is specified
        std::cerr << "ERROR : No targets from NOTICE" << std::endl;	
        return true;
	}
    if (cmd.params[1][0] != ':') {				// Check message start with a ":"
        std::cerr << "ERROR : No message starting with ':' from NOTICE" << std::endl;	
        return true;
	}
	return false;
}