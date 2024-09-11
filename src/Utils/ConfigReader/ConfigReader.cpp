#include "Utils/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"
#include <unordered_set>

namespace ConfigReader
{
    std::string configPath = "./KoleConfig.yaml";
    std::shared_ptr<BuildConfig> buildConfig = nullptr;

    // Set of recognized top-level keys in the YAML config
    std::unordered_set<std::string> recognizedKeys = {
        "output",
        "extension",
        "directories",
        "exclude",
        "flags",
        "compiler_version",
        "language_version",
        "optimization"
    };

    void ReadConfig()
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

            if (config["output"])
                buildConfig->output = config["output"].as<std::string>();

            if (config["extension"])
                buildConfig->extension = config["extension"].as<std::string>();

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
                        std::vector<std::string> values = dir.second.as<std::vector<std::string>>();
                        buildConfig->directories[key] = values;
                    }
                    else if (dir.second.IsScalar())
                    {
                        buildConfig->directories[key] = { dir.second.as<std::string>() };
                    }
                }
            }

            if (config["exclude"])
            {
                const auto& excluded = config["exclude"];
                for (const auto& exclude : excluded)
                {
                    buildConfig->exclude.push_back(exclude.as<std::string>());
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
                        buildConfig->flags[key] = value;
                    else
                        Logger::Warning(fmt::format("Flag '{}' was not recognized. Ignoring...", key));
                }
            }

            if (config["compilerVersion"])
                buildConfig->compilerVersion = config["compilerVersion"].as<std::string>();

            if (config["languageVersion"])
                buildConfig->languageVersion = config["languageVersion"].as<std::string>();

            if (config["optimization"])
                buildConfig->optimization = config["optimization"].as<std::string>();

            Logger::Debug("Successfully read config file");
        }
        catch (const YAML::Exception& e)
        {
            Logger::Debug(e.what());
            Logger::Fatal("Couldn't read config file. Aborting...");
        }
    }

    std::shared_ptr<BuildConfig> GetBuildConfig()
    {
        if (buildConfig == nullptr)
            ReadConfig();

        return buildConfig;
    }
};
