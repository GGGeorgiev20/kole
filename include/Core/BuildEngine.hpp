#pragma once

#include <memory>

#include "Utils/ConfigReader.hpp"

class BuildEngine
{
public:
    BuildEngine(std::shared_ptr<BuildConfig> config);

    // TODO: Add comments to project

    std::string GetCompileCommandForFile(std::string path, std::string name);

    std::string GetLinkCommandForProject(std::vector<std::string> files);

    std::string GetFlags();

    std::string GetIncludePaths();

private:
    std::shared_ptr<BuildConfig> config;

    std::string flags = std::string();

    std::map<std::string, std::string> optimizationLevels = {
        { "none",         "-O0"    },  // No optimization
        { "opt1",         "-O1"    },  // Optimization Level 1
        { "opt2",         "-O2"    },  // Optimization Level 2
        { "release",      "-O3"    },  // Optimization Level 3
        { "fast",         "-Ofast" },  // Aggressive optimization
        { "debug",        "-Og"    },  // Optimization perfected for debugging
        { "size",         "-Os"    },  // Optimization for size
    };
};