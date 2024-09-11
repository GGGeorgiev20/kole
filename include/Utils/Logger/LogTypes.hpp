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

    extern std::map<LogType, std::string> LogTypePrefixes;
    extern std::map<LogType, bool> LogTypeStatuses;

    void EnableDebug();
}