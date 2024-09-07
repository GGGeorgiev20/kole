#pragma once

#include <iostream>
#include <string>

namespace Platform
{
    enum Platforms
    {
        WINDOWS,
        LINUX,
        MAC,
        UNIX,
        UNKNOWN,
    };
    
    int GetPlatform();

    std::string GetOutputExtension();
};