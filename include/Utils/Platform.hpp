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

    void SetPlatform(std::string platformName);

    int GetPlatform();

    std::string GetOutputExtension();

    std::string GetPlatformName();

    // Variables

    extern int savedPlatform;
    extern std::map<int, std::string> platformNames;
};