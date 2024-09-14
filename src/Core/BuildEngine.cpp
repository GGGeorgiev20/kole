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

/**
 * @brief Generates the command to compile a source file to an object file.
 *
 * Constructs the compile command using the compiler version, language standard,
 * flags, source file, object file, and include paths.
 *
 * @param source: The source file to compile.
 * @param object: The object file output.
 * @return The formatted compile command.
 */
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

/**
 * @brief Generates the command to link object files into a final binary.
 *
 * Constructs the link command using the compiler version, flags, object files,
 * and output binary name.
 *
 * @param files: Vector of object files to link.
 * @param output: The output binary name.
 * @return The formatted link command.
 */
std::string BuildEngine::GetLinkCommandForProject(std::vector<std::string> files, std::string output)
{
    std::string flags = GetFlags();
    std::string objectFiles = "";

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

/**
 * @brief Retrieves and caches the necessary compile flags.
 *
 * Gathers platform-specific and common flags, along with optimization levels,
 * and caches them for future use.
 *
 * @return The formatted compile flags.
 */
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

/**
 * @brief Retrieves and caches the necessary include paths.
 *
 * Collects include directories from the configuration and caches them for future use.
 *
 * @return The formatted include paths.
 */
std::string BuildEngine::GetIncludePaths()
{
    if (!includePaths.empty()) return includePaths;

    for (const auto& includeDir : config->directories.at("include"))
    {
        includePaths += fmt::format("-I{} ", includeDir);
    }

    return includePaths;
}
