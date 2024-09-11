#include "Utils/Logger/LogTypes.hpp"

std::map<LogTypes::LogType, std::string> LogTypes::LogTypePrefixes = {
    { LogType::Debug,     "DEBUG" },
    { LogType::Info,      "INFO"  },
    { LogType::Warning,   "WARN"  },
    { LogType::Error,     "ERROR" },
    { LogType::Fatal,     "FATAL" },
};

std::map<LogTypes::LogType, bool> LogTypes::LogTypeStatuses = {
    { LogType::Debug,     false },
    { LogType::Info,      true  },
    { LogType::Warning,   true  },
    { LogType::Error,     true  },
};

void LogTypes::EnableDebug()
{
    LogTypeStatuses[LogType::Debug] = true;
}
