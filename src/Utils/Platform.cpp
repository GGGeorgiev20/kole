#include "Utils/Platform.hpp"
#include "Utils/Logger/Logger.hpp"

#include <map>
#include <algorithm>

void Platform::SetPlatform(std::string platformName)
{
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);

    // Get the value from the enum for the specified platform name
    for (const auto& [key, value] : platformNames)
    {
        std::string mapPlatformName = value;
        transform(mapPlatformName.begin(), mapPlatformName.end(), mapPlatformName.begin(), ::tolower);

        if (platformName == mapPlatformName)
        {
            Logger::Debug(fmt::format("Platform '{}' specified. Adjusting build settings.", platformName));
            
            savedPlatform = key;
            return;
        }
    }

    Logger::Warning(fmt::format("Platform '{}' not recognized. Defaulting to '{}'.", platformName, GetPlatformName()));
}

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

    Logger::Debug(fmt::format("Operating system is {}", GetPlatformName()));
    return platform;
}

std::string Platform::GetOutputExtension()
{
    const int platform = GetPlatform();

    switch (platform)
    {
    case WINDOWS:
        return "exe";
    case LINUX:
        return "out";
    case MACOS:
        return "";
    case UNIX:
        return "out";
    default:
        return "";
    }
}

std::string Platform::GetPlatformName()
{
    const int platform = GetPlatform();
    return platformNames.at(platform);
}

// NAMESPACE VARIABLES

int Platform::savedPlatform = -1;

std::map<int, std::string> Platform::platformNames = {
    { WINDOWS,      "Windows" },
    { LINUX,        "Linux"   },
    { MACOS,        "MacOS"   },
    { UNIX,         "Unix"    },
    { UNKNOWN,      "Unknown" },
};
