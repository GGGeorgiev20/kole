#pragma once

#include <memory>

#include "Utils/ConfigReader.hpp"

class FlagManager
{
public:
    FlagManager(std::shared_ptr<BuildConfig> config);

    std::string GetFlags();

private:
    std::shared_ptr<BuildConfig> config;
};