#include "Utils/Logger/LogTypes.hpp"

namespace LogTypes
{
    std::map<LogType, std::string> LogTypePrefix = {
        { LogType::Info,      "INFO"  },
        { LogType::Debug,     "DEBUG" },
        { LogType::Warning,   "WARN"  },
        { LogType::Error,     "ERROR" },
        { LogType::Fatal,     "FATAL" },
    };

    std::map<LogType, bool> LogTypeStatus = {
        { LogType::Info,      true },
        { LogType::Debug,     true },
        { LogType::Warning,   true },
        { LogType::Error,     true },
    };
}