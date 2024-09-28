#pragma once

#include <iostream>
#include <map>

namespace LogTypes
{
    enum class LogType
    {
        Info,
        Debug,
        Warning,
        Error,
        Fatal,
        Assert,
    };

    void EnableDebug();

    // NAMESPACE VARIABLES

    extern std::map<LogType, bool> LogTypeStatuses;
    extern std::map<LogType, std::string> LogTypePrefixes;
}