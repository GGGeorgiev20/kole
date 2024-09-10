#include "Core/FlagManager.hpp"
#include "Utils/Platform.hpp"

#include <map>
#include <algorithm>
#include <fmt/core.h>

FlagManager::FlagManager(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
}

std::string FlagManager::GetFlags()
{
    int platform = Platform::GetPlatform();

    std::string platformName = Platform::GetPlatformName(platform);
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);
    
    std::string platformSpecificFlags = config->flags.at(platformName);

    std::map<std::string, std::string> optimizationLevels = {
        { "none",         "-O0"    },  // No optimization
        { "opt1",         "-O1"    },  // Optimization Level 1
        { "opt2",         "-O2"    },  // Optimization Level 2
        { "release",      "-O3"    },  // Optimization Level 3
        { "fast",         "-Ofast" },  // Aggressive optimization
        { "debug",        "-Og"    },  // Optimization perfected for debugging
        { "size",         "-Os"    },  // Optimization for size
    };

    std::string optimization = optimizationLevels.at(config->optimization);

    std::string flags = fmt::format("{} {}", platformSpecificFlags, optimization);
    return flags;
}