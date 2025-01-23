#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

class FlagManager
{
public:
    FlagManager(std::shared_ptr<BuildConfig> config) : m_config(config) {}

    /**
     * @brief Retrieves and caches the necessary compile flags.
     *
     * Gathers platform-specific and common flags, along with optimization levels,
     * and caches them for future use.
     *
     * @return The formatted compile flags.
     */
    std::string GetFlags();

    /**
     * @brief Retrieves and caches the necessary include paths.
     *
     * Collects include directories from the configuration and caches them for future use.
     *
     * @return The formatted include paths.
     */
    std::string GetIncludePaths();

private:
    std::string GetOptimization();

    std::string GetPlatformFlags();

    std::string ProcessPlatformFlags(const std::string& flags);

private:
    std::shared_ptr<BuildConfig> m_config;
    std::string m_flags;
    std::string m_includePaths;

    std::map<std::string, std::string> m_optimizationLevels = {
        { "none",         "-O0"    },  // No optimization
        { "opt1",         "-O1"    },  // Optimization Level 1
        { "opt2",         "-O2"    },  // Optimization Level 2
        { "release",      "-O3"    },  // Optimization Level 3
        { "fast",         "-Ofast" },  // Aggressive optimization
        { "debug",        "-Og"    },  // Optimization for the best debugging experience
        { "size",         "-Os"    },  // Optimization for size
    };
};
