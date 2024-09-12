#include "Utils/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

std::string ConfigReader::configPath = "./KoleConfig.yaml";
std::shared_ptr<BuildConfig> ConfigReader::buildConfig = nullptr;

// Set of recognized top-level keys in the YAML config
std::unordered_set<std::string> ConfigReader::recognizedKeys = {
    "output",
    "extension",
    "directories",
    "exclude",
    "flags",
    "compiler_version",
    "language_version",
    "optimization"
};

void ConfigReader::ReadConfig()
{
    try
    {
        YAML::Node config = YAML::LoadFile(configPath);

        buildConfig = std::make_shared<BuildConfig>();

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

        if (config["directories"])
        {
            const auto& dirs = config["directories"];

            for (const auto& dir : dirs)
            {
                std::string key = dir.first.as<std::string>();

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

        if (config["exclude"])
        {
            const auto& excluded = config["exclude"];

            for (const auto& exclude : excluded)
            {
                std::string value = exclude.as<std::string>();
                buildConfig->exclude.push_back(ProcessProperty(value));
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

        Logger::Debug("Successfully read config file");
    }
    catch (const YAML::Exception& e)
    {
        Logger::Debug(e.what());
        Logger::Fatal("Couldn't read config file. Aborting...");
    }
}

void ConfigReader::ValidateProperties()
{
    if (buildConfig->output == "")
    {
        Logger::Fatal("Output name in config can't be empty");
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
