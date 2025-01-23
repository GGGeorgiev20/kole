#include "Utils/FlagManager.hpp"

std::string FlagManager::GetFlags()
{
    if (!m_flags.empty()) return m_flags;

    Logger::Debug("Generating flags for the first time...");

    const std::string optimization = GetOptimization();
    const std::string commonFlags = m_config->flags.at("common");
    const std::string platformFlags = GetPlatformFlags();

    m_flags = fmt::format("{} {} {}", optimization, commonFlags, platformFlags);

    Logger::Debug(fmt::format("Generated flags: '{}'", m_flags));

    return m_flags;
}

std::string FlagManager::GetIncludePaths()
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

std::string FlagManager::GetOptimization()
{
    // Set default optimization to 'debug'
    std::string optimization = m_optimizationLevels.at("debug");

    std::string optimizationLowercase = m_config->optimization;
    transform(optimizationLowercase.begin(), optimizationLowercase.end(), optimizationLowercase.begin(), ::tolower);

    // Check if user-given optimization is valid and present in optimizations map
    if (m_optimizationLevels.find(optimizationLowercase) != m_optimizationLevels.end())
    {
        optimization = m_optimizationLevels.at(optimizationLowercase);
        Logger::Debug(fmt::format("Setting optimization level to '{}' ({})", optimizationLowercase, optimization));
    }
    else
    {
        Logger::Warning(fmt::format("Optimization level '{}' not recognized", optimizationLowercase));
        Logger::Debug("Defaulting to debug optimization level");
    }

    return optimization;
}

std::string FlagManager::GetPlatformFlags()
{
    std::string platformName = Platform::GetPlatformName();
    transform(platformName.begin(), platformName.end(), platformName.begin(), ::tolower);

    if (m_config->flags.contains(platformName))
    {
        return m_config->flags.at(platformName);
    }
    else
    {
        Logger::Warning(fmt::format("Configuration for platform '{}' was not found", platformName));
        Logger::Info("Applying only common (platform-independent) flags");
    }

    return "";
}

std::string FlagManager::ProcessPlatformFlags(const std::string& flags)
{
    const char special = '$';
    std::string pkg = "`pkg-config --libs --cflags`";

    std::string newFlags = "";
    for (std::size_t i = 0; i < flags.length(); i++)
    {
        if (flags[i] == special)
        {
            std::size_t start = i + 1;

            std::size_t end = flags.find(' ', start);
            if (end == std::string::npos)
                end = flags.length();
            
            std::string flag = flags.substr(start, end - start);

            pkg.insert(pkg.length() - 1, ' ' + flag);

            i += end - start + 1;
            continue;
        }

        newFlags += flags[i];
    }

    return newFlags + ' ' + pkg;
}
