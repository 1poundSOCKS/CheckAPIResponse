#pragma once

struct config
{
    std::string url;
    std::string databaseUser;
    std::string password;
    std::string spUser;
};

auto LoadDefaultConfig() -> config;
