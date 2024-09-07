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

    extern std::map<LogType, std::string> LogTypePrefix;
    extern std::map<LogType, bool> LogTypeStatus;
}