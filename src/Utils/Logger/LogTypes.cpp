#include "Utils/Logger/LogTypes.hpp"

std::map<LogTypes::LogType, std::string> LogTypes::LogTypePrefixes = {
    { LogType::Info,      "INFO"  },
    { LogType::Debug,     "DEBUG" },
    { LogType::Warning,   "WARN"  },
    { LogType::Error,     "ERROR" },
    { LogType::Fatal,     "FATAL" },
};

std::map<LogTypes::LogType, bool> LogTypes::LogTypeStatuses = {
    { LogType::Info,      true },
    { LogType::Debug,     true },
    { LogType::Warning,   true },
    { LogType::Error,     true },
};
