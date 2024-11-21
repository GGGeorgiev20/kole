#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <fmt/core.h>

#include "Utils/Logger/LogTypes.hpp"

namespace Logger
{
    void Log(LogTypes::LogType type, std::string message);

    void Info(std::string message);

    void Debug(std::string message);

    void Warning(std::string message);

    void Error(std::string message);

    void Fatal(std::string message);

    void Assert(bool condition, std::string message);
};