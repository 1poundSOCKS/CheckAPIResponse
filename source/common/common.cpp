#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "common_defs.h"

auto LoadDefaultConfig() -> config
{
    config config;

    std::ifstream inputFile("default.config");

    std::string line;
    while( std::getline(inputFile, line) )
    {
        std::istringstream f(line);
        std::string key, value;

        if( std::getline(f, key, '=') && std::getline(f, value, '\n') )
        {
            if( key.compare("url") == 0 )
            {
                config.url = value;
            }
            else if( key.compare("database_user") == 0 )
            {
                config.databaseUser = value;
            }
            else if( key.compare("password") == 0 )
            {
                config.password = value;
            }
            else if( key.compare("sp_user") == 0 )
            {
                config.spUser = value;
            }
        }
    }

    return config;
}
