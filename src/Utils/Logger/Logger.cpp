#include "Utils/Logger/Logger.hpp"

using namespace Logger;

void Logger::Log(LogTypes::LogType type, std::string message)
{
    if (!LogTypes::LogTypeStatus[type] && type != LogTypes::LogType::Fatal)
        return;

    printf("%s: %s\n", LogTypes::LogTypePrefix[type].c_str(), message.c_str());
}

void Logger::Info(std::string message)
{
    Log(LogTypes::LogType::Info, message);
}

void Logger::Debug(std::string message)
{
    Log(LogTypes::LogType::Debug, message);
}

void Logger::Warning(std::string message)
{
    Log(LogTypes::LogType::Warning, message);
}

void Logger::Error(std::string message)
{
    Log(LogTypes::LogType::Error, message);
}

void Logger::Fatal(std::string message)
{
    Log(LogTypes::LogType::Fatal, message);
    Log(LogTypes::LogType::Fatal, "Exiting program...");
    exit(1);
}

void Logger::Assert(std::string message, bool condition, bool isFatal)
{
    if (condition)
        return;

    message = "ASSERT: " + message;

    if (isFatal)
        Fatal(message);
    else
        Error(message);
}