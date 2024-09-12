#include "Utils/Logger/LogTypes.hpp"

void LogTypes::EnableDebug()
{
    LogTypeStatuses[LogType::Debug] = true;
}

// Variables

std::map<LogTypes::LogType, bool> LogTypes::LogTypeStatuses = {
    { LogType::Debug,     false },
    { LogType::Info,      true  },
    { LogType::Warning,   true  },
    { LogType::Error,     true  },
};

std::map<LogTypes::LogType, std::string> LogTypes::LogTypePrefixes = {
    { LogType::Debug,     "DEBUG" },
    { LogType::Info,      "INFO"  },
    { LogType::Warning,   "WARN"  },
    { LogType::Error,     "ERROR" },
    { LogType::Fatal,     "FATAL" },
};
