#pragma once

#include <iostream>
#include <string>
#include <map>

namespace Platform
{
    enum Platforms
    {
        WINDOWS,
        LINUX,
        MACOS,
        UNIX,
        UNKNOWN,
    };

    extern std::map<int, std::string> platformNames;

    extern int savedPlatform;
    int GetPlatform();

    std::string GetOutputExtension();

    std::string GetPlatformName(int platform);
};