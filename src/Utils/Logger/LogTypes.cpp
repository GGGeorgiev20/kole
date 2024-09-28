#include "Utils/Logger/LogTypes.hpp"

void LogTypes::EnableDebug()
{
    LogTypeStatuses[LogType::Debug] = true;
}

// NAMESPACE VARIABLES

// This variable is used to disable any log types
// Debug is disabled by default, it can be enabled using the '--debug' flag
std::map<LogTypes::LogType, bool> LogTypes::LogTypeStatuses = {
    { LogType::Debug,     false },
    { LogType::Info,      true  },
    { LogType::Warning,   true  },
    { LogType::Error,     true  },
};

// Prefixes for all log types
std::map<LogTypes::LogType, std::string> LogTypes::LogTypePrefixes = {
    { LogType::Debug,      "DEBUG"  },
    { LogType::Info,       "INFO"   },
    { LogType::Warning,    "WARN"   },
    { LogType::Error,      "ERROR"  },
    { LogType::Fatal,      "FATAL"  },
    { LogType::Assert,     "ASSERT" },
};
