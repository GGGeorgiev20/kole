#include "Utils/Platform.hpp"
#include "Utils/Logger/Logger.hpp"

#include <map>

std::map<int, std::string> Platform::platformNames = {
    { WINDOWS,      "Windows" },
    { LINUX,        "Linux"   },
    { MACOS,        "MacOS"   },
    { UNIX,         "Unix"    },
    { UNKNOWN,      "Unknown" },
};

int Platform::savedPlatform = -1;
int Platform::GetPlatform()
{
    if (savedPlatform != -1) return savedPlatform;
    
    int platform;
    
    #if defined(_WIN32) || defined(_WIN64)
        platform = WINDOWS;
    #elif defined(__linux__)
        platform = LINUX;
    #elif defined(__APPLE__) || defined(__MACH__)
        platform = MACOS;
    #elif defined(__unix__)
        platform = UNIX;
    #else
        Logger::Debug("Unable to determine the user's operating system.");
        return UNKNOWN;
    #endif

    savedPlatform = platform;

    Logger::Debug(fmt::format("Operating system is {}", GetPlatformName(platform)));
    return platform;
}

std::string Platform::GetOutputExtension()
{
    int platform = GetPlatform();

    switch (platform)
    {
    case WINDOWS:
        return ".exe";
    case LINUX:
        return ".out";
    case MACOS:
        return "";
    case UNIX:
        return ".out";
    default:
        return "";
    }
}

std::string Platform::GetPlatformName(int platform)
{
    return platformNames.at(platform);
}
