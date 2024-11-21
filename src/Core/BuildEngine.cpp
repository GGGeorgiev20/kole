#include "Core/BuildEngine.hpp"

#include "Utils/Logger/Logger.hpp"
#include "Utils/Platform.hpp"

#include <map>
#include <algorithm>
#include <fmt/core.h>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

BuildEngine::BuildEngine(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
}

std::string BuildEngine::GetOutputPath(std::string sourceFileName, std::string sourceExtension)
{
    std::string outputExtension;
    std::string outputDirectory;

    // Determine output extension and directory based on source file extension
    if (sourceExtension == "cpp" || sourceExtension == "c")
    {
        outputExtension = "o";
        outputDirectory = "obj";
    }
    else if (sourceExtension == "h" || sourceExtension == "hpp")
    {
        outputExtension = "cpp";
        outputDirectory = "obj";
        sourceFileName = config->qtSupport.at("moc_prefix") + sourceFileName;
    }
    else if (sourceExtension == "ui")
    {
        outputExtension = config->qtSupport.at("ui_extension");
        outputDirectory = config->qtSupport.at("ui_output_dir");
        sourceFileName = config->qtSupport.at("ui_prefix") + sourceFileName;
    }
    else
    {
        Logger::Warning(fmt::format("Unsupported file extension: {}", sourceExtension));
        return "";
    }

    std::string outputPath = fmt::format(
        "./{}/{}.{}",
        outputDirectory,
        sourceFileName,
        outputExtension
    );

    return outputPath;
}

std::string BuildEngine::GetCompileCommandForFile(std::string sourceExtension, std::string sourcePath, std::string outputPath)
{
    if (sourceExtension == "cpp" || sourceExtension == "c")
    {
        return GetCompileCommandForSourceFile(sourcePath, outputPath);
    }
    else if (sourceExtension == "h" || sourceExtension == "hpp")
    {
        return GetCompileCommandForHeaderFile(sourcePath, outputPath);
    }
    else if (sourceExtension == "ui")
    {
        return GetCompileCommandForUIFile(sourcePath, outputPath);
    }

    return "";
}

std::string BuildEngine::GetLinkCommandForProject(std::vector<std::string> files, std::string output)
{
    std::string flags = GetFlags();
    std::string objectFiles = "";

    for (const auto& file : files)
    {
        objectFiles += fmt::format("{} ", file);
    }

    objectFiles = objectFiles.substr(0, objectFiles.length() - 1);

    std::string command = fmt::format(
        "{} {} -o {} {}",
        config->compiler,
        objectFiles,
        output,
        flags
    );

    return command;
}

std::string BuildEngine::GetCompileCommandForSourceFile(std::string source, std::string output)
{
    std::string flags = GetFlags();
    std::string includePaths = GetIncludePaths();

    std::string command = fmt::format(
        "{} {}{} -c {} -o {} {} {}",
        config->compiler,
        config->languageVersion != "" ? "-std=" : "",
        config->languageVersion,
        source,
        output,
        includePaths,
        flags
    );

    return command;
}

std::string BuildEngine::GetCompileCommandForHeaderFile(std::string source, std::string output)
{
    std::string command = fmt::format(
        "moc {} -o {} >/dev/null 2>&1",
        source,
        output
    );

    // NOTE: The reason this specific flag is passed to the moc command
    // is to remove the message if a certain header file doesn't need to be compiled.
    // Personally, I fucking love that feature as it saves me a lot of trouble
    // but I don't want it to show up (it's great for debugging purposes, it's just not aesthetic),

    return command;
}

std::string BuildEngine::GetCompileCommandForUIFile(std::string source, std::string output)
{
    std::string command = fmt::format(
        "uic {} -o {}",
        source,
        output
    );

    return command;
}

std::string BuildEngine::GetFlags()
{
    if (!flags.empty()) return flags;

    std::string platformName = Platform::GetPlatformName();
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

    flags = fmt::format("{} {} {}", optimization, commonFlags, platformSpecificFlags);

    return flags;
}

std::string BuildEngine::GetIncludePaths()
{
    if (!includePaths.empty()) return includePaths;

    for (const auto& includeDir : config->directories.at("include"))
    {
        includePaths += fmt::format("-I{} ", includeDir);
    }

    if (config->qtSupport.at("compile_ui") == "true")
        includePaths += fmt::format("-I{} ", config->qtSupport.at("ui_output_dir"));

    // Remove trailing whitespace
    includePaths = includePaths.substr(0, includePaths.length() - 1);

    Logger::Debug(fmt::format("Include paths are '{}'", includePaths));

    return includePaths;
}
