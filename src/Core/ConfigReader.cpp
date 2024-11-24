#include "Core/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::shared_ptr<ConfigReader> ConfigReader::m_instance = nullptr;

void ConfigReader::CreateConfig()
{
    if (fs::exists(m_configPath))
    {
        Logger::Warning("The config flag was specified, but a config file already exists");
        return;
    }

    try
    {
        std::ofstream configFile(m_configPath);
        if (!configFile.is_open())
        {
            Logger::Error(fmt::format("Failed to open config file at '{}'", m_configPath));
            return;
        }

        // TODO: Remake this to use a file from the 'assets' folder
        // configFile << m_defaultConfig;
        configFile.close();

        Logger::Info(fmt::format("Successfully created config file at '{}'", m_configPath));
    }
    catch (const std::exception& e)
    {
        Logger::Error(fmt::format("Error creating config file: {}", e.what()));
    }
}

void ConfigReader::ReadConfig()
{
    m_buildConfig = std::make_shared<BuildConfig>();

    if (!std::filesystem::exists(m_configPath))
    {
        Logger::Info("Config file doesn't exist. Using default values");
        return;
    }

    try
    {
        YAML::Node config = YAML::LoadFile(m_configPath);

        // Check for unrecognized properties
        for (const auto& property : config)
        {
            std::string key = property.first.as<std::string>();

            if (std::find(m_recognizedKeys.begin(), m_recognizedKeys.end(), key) == m_recognizedKeys.end())
            {
                Logger::Warning(fmt::format("Property '{}' was not recognized in the configuration. Ignoring...", key));
            }
        }

        // Check if the property is specified in the config before saving it
        // Else, the default value in the config object is used
        if (config["output"])
        {
            std::string property = config["output"].as<std::string>();
            m_buildConfig->output = ProcessProperty(property);
        }

        if (config["extension"])
        {
            std::string property = config["extension"].as<std::string>();
            m_buildConfig->extension = ProcessProperty(property);
        }

        if (config["platform"])
        {
            std::string property = config["platform"].as<std::string>();
            m_buildConfig->platform = ProcessProperty(property);
        }

        if (config["directories"])
        {
            const auto& dirs = config["directories"];

            for (const auto& dir : dirs)
            {
                std::string key = dir.first.as<std::string>();

                // If a directory isn't recognized in the config property, ignore it and warn the user
                if (!m_buildConfig->directories.contains(key))
                {
                    Logger::Warning(fmt::format("Directory '{}' was not recognized. Ignoring...", key));
                    continue;
                }

                if (dir.second.IsSequence())
                {
                    std::vector<std::string> value = dir.second.as<std::vector<std::string>>();
                    m_buildConfig->directories[key] = ProcessProperty(value);
                }
                else if (dir.second.IsScalar())
                {
                    std::string value = dir.second.as<std::string>();
                    m_buildConfig->directories[key] = { ProcessProperty(value) };
                }
            }
        }

        if (config["autocreate"])
        {
            const auto& autocreated = config["autocreate"];

            for (const auto& autocreate : autocreated)
            {
                std::string value = autocreate.as<std::string>();
                m_buildConfig->autocreate.push_back(ProcessProperty(value));
            }
        }

        if (config["exclude"])
        {
            const auto& excluded = config["exclude"];

            for (const auto& exclude : excluded)
            {
                std::string value = exclude.as<std::string>();
                m_buildConfig->exclude.push_back(ProcessProperty(value));
            }
        }

        if (config["flags"])
        {
            const auto& flags = config["flags"];

            for (const auto& flag : flags)
            {
                std::string key = flag.first.as<std::string>();
                std::string value = flag.second.as<std::string>();

                if (!m_buildConfig->flags.contains(key))
                {
                    Logger::Warning(fmt::format("Flag '{}' was not recognized. Ignoring...", key));
                    continue;
                }

                m_buildConfig->flags[key] = ProcessProperty(value);
            }
        }

        if (config["qt_support"])
        {
            const auto& qtSupport = config["qt_support"];

            for (const auto& property : qtSupport)
            {
                std::string key = property.first.as<std::string>();
                std::string value = property.second.as<std::string>();

                if (!m_buildConfig->qtSupport.contains(key))
                {
                    Logger::Warning(fmt::format("QT Support property '{}' was not recognized. Ignoring...", key));
                    continue;
                }

                m_buildConfig->qtSupport[key] = ProcessProperty(value);
            }
        }

        if (config["compiler"])
        {
            std::string property = config["compiler"].as<std::string>();
            m_buildConfig->compiler = ProcessProperty(property);
        }

        if (config["language_version"])
        {
            std::string property = config["language_version"].as<std::string>();
            m_buildConfig->languageVersion = ProcessProperty(property);
        }

        if (config["optimization"])
        {
            std::string property = config["optimization"].as<std::string>();
            m_buildConfig->optimization = ProcessProperty(property);
        }

        Logger::Debug("Successfully read config file");
    }
    catch (const YAML::Exception& e)
    {
        Logger::Error(fmt::format("Failed when parsing config file '{}': {}", m_configPath, e.what()));
        Logger::Info("Using default configuration values.");
    }
}

void ConfigReader::PostProcess()
{
    Logger::Assert(!m_buildConfig->output.empty(), "Output name in config can't be empty");
    Logger::Assert(!m_buildConfig->platform.empty(), "Platform in config can't be empty");
    Logger::Assert(!m_buildConfig->compiler.empty(), "Compiler version in config can't be empty");

    if (m_buildConfig->platform == ConfigConstants::AUTO)
    {
        m_buildConfig->platform = Platform::GetPlatform();
    }
    else
    {
        Platform::SetPlatform(m_buildConfig->platform);
    }

    // NOTE: The extension check needs to be after the platform,
    // or else the extension will be calculated for the user's os
    if (m_buildConfig->extension == ConfigConstants::AUTO)
    {
        m_buildConfig->extension = Platform::GetOutputExtension();
    }

    const std::string compileUi = m_buildConfig->qtSupport.at("compile_ui");
    const std::string uiExtension = m_buildConfig->qtSupport.at("ui_extension");

    if (compileUi == ConfigConstants::TRUE && uiExtension != "hpp" && uiExtension != "h")
    {
        Logger::Warning(fmt::format("UI extension '{}' is not valid and may cause issues.", uiExtension));
    }
}

std::string ConfigReader::ProcessProperty(const std::string& property)
{
    // The reason I create a copy instead of simply passing by value is that I want to convert it to lowercase.
    // Then I return the original value (in most cases), and so it must be preserved.
    std::string copy = property;
    transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

    if (copy == "none")
        return "";

    return property;
}

std::vector<std::string> ConfigReader::ProcessProperty(const std::vector<std::string>& properties)
{
    std::vector<std::string> result;
    result.reserve(properties.size());

    for (auto property : properties)
    {
        transform(property.begin(), property.end(), property.begin(), ::tolower);

        if (property != "none")
        {
            result.push_back(property);
        }
    }

    return result;
}

std::shared_ptr<BuildConfig> ConfigReader::GetBuildConfig()
{
    if (m_buildConfig == nullptr)
        ReadConfig();

    return m_buildConfig;
}
