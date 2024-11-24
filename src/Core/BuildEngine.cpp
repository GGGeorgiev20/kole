#include "Core/BuildEngine.hpp"

#include <map>
#include <algorithm>
#include <fmt/core.h>
#include <filesystem>
#include <functional>

#include "Utils/Logger/Logger.hpp"
#include "Utils/Platform.hpp"

namespace fs = std::filesystem;

std::string BuildEngine::GetOutputPath(std::string sourceFileName, const std::string& sourceExtension)
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
        sourceFileName = m_config->qtSupport.at("moc_prefix") + sourceFileName;
    }
    else if (sourceExtension == "ui")
    {
        outputExtension = m_config->qtSupport.at("ui_extension");
        outputDirectory = m_config->qtSupport.at("ui_output_dir");
        sourceFileName = m_config->qtSupport.at("ui_prefix") + sourceFileName;
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

std::string BuildEngine::GetCompileCommandForFile(const std::string& sourceExtension, const std::string& sourcePath, const std::string& outputPath)
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

    Logger::Warning(fmt::format("Unrecognized source extension: '{}'", sourceExtension));
    return "";
}

std::string BuildEngine::GetLinkCommandForProject(const std::vector<std::string>& files, const std::string& output)
{
    std::string flags = GetFlags();
    std::string objectFiles = "";

    for (const auto& file : files)
    {
        objectFiles += file + " ";
    }

    std::string command = fmt::format(
        "{} {} -o {} {}",
        m_config->compiler,
        objectFiles,
        output,
        flags
    );

    return command;
}

std::string BuildEngine::GetCompileCommandForSourceFile(const std::string& source, const std::string& output)
{
    std::string flags = GetFlags();
    std::string includePaths = GetIncludePaths();

    std::string command = fmt::format(
        "{} {}{} -c {} -o {} {} {}",
        m_config->compiler,
        m_config->languageVersion != "" ? "-std=" : "",
        m_config->languageVersion,
        source,
        output,
        includePaths,
        flags
    );

    return command;
}

std::string BuildEngine::GetCompileCommandForHeaderFile(const std::string& source, const std::string& output)
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

std::string BuildEngine::GetCompileCommandForUIFile(const std::string& source, const std::string& output)
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
    if (!m_flags.empty()) return m_flags;

    // Set default optimization to 'debug'
    std::string optimization = m_optimizationLevels.at("debug");

    std::string optimizationLowercase = m_config->optimization;
    transform(optimizationLowercase.begin(), optimizationLowercase.end(), optimizationLowercase.begin(), ::tolower);

    // Check if user-given optimization is valid and present in optimizations map
    if (m_optimizationLevels.contains(optimizationLowercase))
    {
        optimization = m_optimizationLevels.at(optimizationLowercase);
        Logger::Debug(fmt::format("Setting optimization level to '{}' ({})", optimizationLowercase, optimization));
    }
    else
    {
        Logger::Warning(fmt::format("Optimization level '{}' not recognized", optimizationLowercase));
        Logger::Debug("Defaulting to debug optimization level");
    }

    std::string commonFlags = m_config->flags.at("common");
    std::string platformName = Platform::GetPlatformName();
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);

    // If the platform specified was found, apply both common and platform-specific flags, if not then only common flags
    if (m_config->flags.contains(platformName))
    {
        std::string platformSpecificFlags = m_config->flags.at(platformName);
        m_flags = fmt::format("{} {} {}", optimization, commonFlags, platformSpecificFlags);
    }
    else
    {
        // I am calling GetPlatformName here again, because I want the name to not be only lowercase
        Logger::Warning(fmt::format("Configuration for platform '{}' was not found", Platform::GetPlatformName()));
        Logger::Info("Applying only common (platform-independent) flags");
        m_flags = fmt::format("{} {}", optimization, commonFlags);
    }

    return m_flags;
}

std::string BuildEngine::GetIncludePaths()
{
    if (!m_includePaths.empty()) return m_includePaths;

    for (const auto& includeDir : m_config->directories.at("include"))
    {
        m_includePaths += " -I" + includeDir;
    }

    // Add directories holding compiled ui files to include paths
    if (m_config->qtSupport.at("compile_ui") == ConfigConstants::TRUE)
        m_includePaths += " -I{}" + m_config->qtSupport.at("ui_output_dir");

    Logger::Debug(fmt::format("Include paths are '{}'", m_includePaths));

    return m_includePaths;
}
