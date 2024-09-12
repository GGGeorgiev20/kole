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

std::string BuildEngine::GetCompileCommandForFile(std::string source, std::string object)
{
    std::string flags = GetFlags();
    std::string includePaths = GetIncludePaths();
    
    std::string command = fmt::format(
        "{} {}{} {} -c {} -o {} {}",
        config->compilerVersion,
        config->languageVersion != "" ? "-std=" : "",
        config->languageVersion,
        flags,
        source,
        object,
        includePaths
    );

    return command;
}

std::string BuildEngine::GetLinkCommandForProject(std::vector<std::string> files)
{
    std::string flags = GetFlags();
    std::string objectFiles = "";

    std::string output = fmt::format(
        "{}/{}{}{}",
        config->directories.at("bin")[0],
        config->output,
        config->extension != "" ? "." : "",
        config->extension
    );

    for (const auto& file : files)
    {
        objectFiles += fmt::format("{} ", file);
    }
    
    std::string command = fmt::format(
        "{} {} {} -o {}",
        config->compilerVersion,
        flags,
        objectFiles,
        output
    );

    return command;
}

std::string BuildEngine::GetFlags()
{
    if (!flags.empty()) return flags;

    int platform = Platform::GetPlatform();

    std::string platformName = Platform::GetPlatformName(platform);
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);

    std::string optimization = optimizationLevels.at("debug");

    std::string optimizationLowercase = config->optimization;
    transform(optimizationLowercase.begin(), optimizationLowercase.end(), optimizationLowercase.begin(), ::tolower);
    
    if (optimizationLevels.count(optimizationLowercase) > 0)
    {
        optimization = optimizationLevels.at(optimizationLowercase);
        Logger::Debug(fmt::format("Setting optimization level to '{}' ({})", optimizationLowercase, optimization));
    }
    else
    {
        Logger::Warning(fmt::format("Optimization level '{}' not recognized", optimizationLowercase));
        Logger::Warning("Defaulting to debug optimization level");
    }

    std::string commonFlags = config->flags.at("common");
    std::string platformSpecificFlags = config->flags.at(platformName);

    flags = fmt::format("{} {} {}", commonFlags, platformSpecificFlags, optimization);

    return flags;
}

std::string BuildEngine::GetIncludePaths()
{
    if (!includePaths.empty()) return includePaths;

    for (const auto& includeDir : config->directories.at("include"))
    {
        includePaths += fmt::format("-I{} ", includeDir);
    }

    return includePaths;
}
