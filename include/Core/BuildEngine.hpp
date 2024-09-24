#pragma once

#include <memory>

#include "Utils/ConfigReader.hpp"

class BuildEngine
{
public:
    BuildEngine(std::shared_ptr<BuildConfig> config);

    std::string GetCompileCommandForFile(std::string sourcePath, std::string sourceFileName, std::string sourceExtension);

    std::string GetLinkCommandForProject(std::vector<std::string> files, std::string output);

    std::string GetCompileCommandForSourceFile(std::string source, std::string output);

    std::string GetCompileCommandForHeaderFile(std::string source, std::string output);

    std::string GetCompileCommandForUIFile(std::string source, std::string output);

    std::string GetFlags();

    std::string GetIncludePaths();

private:
    std::shared_ptr<BuildConfig> config;

    std::string flags = std::string();
    std::string includePaths = std::string();

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