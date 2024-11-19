#include "Core/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string ConfigReader::configPath = "./KoleConfig.yaml";
std::shared_ptr<BuildConfig> ConfigReader::buildConfig = nullptr;

// Set of recognized top-level keys in the YAML config
std::unordered_set<std::string> ConfigReader::recognizedKeys = {
    "output",
    "extension",
    "platform",
    "directories",
    "autocreate",
    "flags",
    "qt_support",
    "compiler_version",
    "language_version",
    "optimization"
};

void ConfigReader::CreateConfig()
{
    if (fs::exists(configPath))
    {
        Logger::Warning("The config flag was specified, but a config file already exists");
        return;
    }

    try
    {
        std::ofstream configFile(configPath);
        if (!configFile.is_open())
        {
            Logger::Error(fmt::format("Failed to open config file at '{}'", configPath));
            return;
        }

        configFile << defaultConfig;
        configFile.close();

        Logger::Info(fmt::format("Successfully created config file at '{}'", configPath));
    }
    catch (const std::exception& e)
    {
        Logger::Error(fmt::format("Error creating config file: {}", e.what()));
    }
}

void ConfigReader::ReadConfig()
{
    buildConfig = std::make_shared<BuildConfig>();

    if (!std::filesystem::exists(configPath))
    {
        Logger::Info("Config file doesn't exist. Using default values");
        return;
    }

    try
    {
        YAML::Node config = YAML::LoadFile(configPath);

        // Check for unrecognized properties
        for (const auto& property : config)
        {
            std::string key = property.first.as<std::string>();

            if (recognizedKeys.find(key) == recognizedKeys.end())
            {
                Logger::Warning(fmt::format("Property '{}' was not recognized. Ignoring...", key));
            }
        }

        // Check if the property is specified in the config before saving it
        // Else, the default value in the config object is used
        if (config["output"])
        {
            std::string property = config["output"].as<std::string>();
            buildConfig->output = ProcessProperty(property);
        }

        if (config["extension"])
        {
            std::string property = config["extension"].as<std::string>();
            buildConfig->extension = ProcessProperty(property);
        }

        if (config["platform"])
        {
            std::string property = config["platform"].as<std::string>();
            buildConfig->platform = ProcessProperty(property);
        }

        if (config["directories"])
        {
            const auto& dirs = config["directories"];

            for (const auto& dir : dirs)
            {
                std::string key = dir.first.as<std::string>();

                // If a directory isn't recognized in the config property, ignore it and warn the user
                if (buildConfig->directories.count(key) == 0)
                {
                    Logger::Warning(fmt::format("Directory '{}' was not recognized. Ignoring...", key));
                    continue;
                }

                if (dir.second.IsSequence())
                {
                    std::vector<std::string> value = dir.second.as<std::vector<std::string>>();
                    buildConfig->directories[key] = ProcessProperty(value);
                }
                else if (dir.second.IsScalar())
                {
                    std::string value = dir.second.as<std::string>();
                    buildConfig->directories[key] = { ProcessProperty(value) };
                }
            }
        }

        if (config["autocreate"])
        {
            const auto& autocreated = config["autocreate"];

            for (const auto& autocreate : autocreated)
            {
                std::string value = autocreate.as<std::string>();
                buildConfig->autocreate.push_back(ProcessProperty(value));
            }
        }

        if (config["flags"])
        {
            const auto& flags = config["flags"];

            for (const auto& flag : flags)
            {
                std::string key = flag.first.as<std::string>();
                std::string value = flag.second.as<std::string>();

                if (buildConfig->flags.count(key) > 0)
                    buildConfig->flags[key] = ProcessProperty(value);
                else
                    Logger::Warning(fmt::format("Flag '{}' was not recognized. Ignoring...", key));
            }
        }

        if (config["qt_support"])
        {
            const auto& qtSupport = config["qt_support"];

            for (const auto& property : qtSupport)
            {
                std::string key = property.first.as<std::string>();
                std::string value = property.second.as<std::string>();

                if (buildConfig->qtSupport.count(key) > 0)
                    buildConfig->qtSupport[key] = ProcessProperty(value);
                else
                    Logger::Warning(fmt::format("QT Support property '{}' was not recognized. Ignoring...", key));
            }
        }

        if (config["compiler_version"])
        {
            std::string property = config["compiler_version"].as<std::string>();
            buildConfig->compilerVersion = ProcessProperty(property);
        }

        if (config["language_version"])
        {
            std::string property = config["language_version"].as<std::string>();
            buildConfig->languageVersion = ProcessProperty(property);
        }

        if (config["optimization"])
        {
            std::string property = config["optimization"].as<std::string>();
            buildConfig->optimization = ProcessProperty(property);
        }

        Logger::Info("Successfully read config file");
    }
    catch (const YAML::Exception& e)
    {
        Logger::Debug(e.what());
        Logger::Fatal("Couldn't read config file. Aborting...");
    }
}

void ConfigReader::PostProcess()
{
    Logger::Assert("Output name in config can't be empty", !buildConfig->output.empty());
    Logger::Assert("Platform in config can't be empty", !buildConfig->platform.empty());
    Logger::Assert("Compiler version in config can't be empty", !buildConfig->compilerVersion.empty());

    if (buildConfig->platform == "auto")
    {
        buildConfig->platform = Platform::GetPlatform();
    }
    else
    {
        Platform::SetPlatform(buildConfig->platform);
    }

    // NOTE: The extension check needs to be after the platform,
    // or else the extension will be calculated for the user's os
    if (buildConfig->extension == "auto")
    {
        buildConfig->extension = Platform::GetOutputExtension();
    }

    const std::string compileUi = buildConfig->qtSupport.at("compile_ui");
    const std::string uiExtension = buildConfig->qtSupport.at("ui_extension");

    if (compileUi == "true" && uiExtension != "hpp" && uiExtension != "h")
    {
        Logger::Warning(fmt::format("UI extension '{}' is not valid and may cause issues.", uiExtension));
    }
}

std::string ConfigReader::ProcessProperty(std::string property)
{
    std::string propertyLowercase = property;
    transform(propertyLowercase.begin(), propertyLowercase.end(), propertyLowercase.begin(), ::tolower);

    if (propertyLowercase == "none")
        return "";

    return property;
}

std::vector<std::string> ConfigReader::ProcessProperty(std::vector<std::string> properties)
{
    std::vector<std::string> result;
    result.reserve(properties.size());

    for (const auto& property : properties)
    {
        std::string propertyLowercase = property;
        transform(propertyLowercase.begin(), propertyLowercase.end(), propertyLowercase.begin(), ::tolower);

        if (propertyLowercase != "none")
        {
            result.push_back(property);
        }
    }

    return result;
}

std::shared_ptr<BuildConfig> ConfigReader::GetBuildConfig()
{
    if (buildConfig == nullptr)
        ReadConfig();

    return buildConfig;
}
