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
    };

    void EnableDebug();

    // Variables

    extern std::map<LogType, bool> LogTypeStatuses;
    extern std::map<LogType, std::string> LogTypePrefixes;
}