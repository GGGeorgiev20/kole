#include "Utils/Platform/Platform.hpp"

int Platform::GetPlatform()
{
    #if defined(_WIN32) || defined(_WIN64)
        return WINDOWS;
    #elif defined(__linux__)
        return LINUX;
    #elif defined(__APPLE__) || defined(__MACH__)
        return MAC;
    #elif defined(__unix__)
        return UNIX;
    #else
        return UNKNOWN;
    #endif
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
    case MAC:
        return "";
    case UNIX:
        return ".out";
    default:
        return "";
    }
}