#include "Core/BuildEngine.hpp"

#include "Utils/Logger/Logger.hpp"
#include "Utils/Platform.hpp"

#include <map>
#include <algorithm>
#include <fmt/core.h>

BuildEngine::BuildEngine(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
}

std::string BuildEngine::GetCompileCommandForFile(std::string path, std::string name)
{
    std::string flags = GetFlags();
    std::string includePaths = GetIncludePaths();
    
    std::string command = fmt::format(
        "{} -std={} {} -c {} -o {} {}",
        config->compilerVersion,
        config->languageVersion,
        flags,
        path,
        fmt::format("{}/{}.o", config->directories.at("obj")[0], name),
        includePaths
    );

    return command;
}

std::string BuildEngine::GetLinkCommandForProject(std::vector<std::string> files)
{
    return files[0];
}

std::string BuildEngine::GetFlags()
{
    if (flags != "") return flags;

    int platform = Platform::GetPlatform();

    std::string platformName = Platform::GetPlatformName(platform);
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);

    std::string optimization = optimizationLevels.at("debug");
    bool optimizationLevelIsValid = optimizationLevels.count(config->optimization) > 0;

    if (!optimizationLevelIsValid)
    {
        Logger::Warning(fmt::format("Optimization level '{}' not recognized", config->optimization));
        Logger::Warning("Defaulting to debug optimization level");
    }
    else
    {
        optimization = optimizationLevels.at(config->optimization);
        Logger::Debug(fmt::format("Setting optimization level to '{}' ({})", config->optimization, optimization));
    }

    std::string commonFlags = config->flags.at("common");
    std::string platformSpecificFlags = config->flags.at(platformName);

    std::string flags = fmt::format("{} {} {}", commonFlags, platformSpecificFlags, optimization);
    this->flags = flags;

    return flags;
}

std::string BuildEngine::GetIncludePaths()
{
    std::string includePaths = "";

    for (const auto& includeDir : config->directories.at("include"))
    {
        includePaths += fmt::format("-I{} ", includeDir);
    }

    return includePaths;
}
