#include "Utils/ConfigReader/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

namespace ConfigReader
{
    std::string configPath = "./config.yaml";
    std::shared_ptr<BuildConfig> buildConfig = nullptr;

    void ReadConfig()
    {
        try
        {
            YAML::Node config = YAML::LoadFile(configPath);

            buildConfig = std::make_shared<BuildConfig>();

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
            
            if (config["ignore"])
            {
                const auto& ignores = config["ignore"];
                for (const auto& ignore : ignores)
                {
                    buildConfig->ignore.push_back(ignore.as<std::string>());
                }
            }
            
            if (config["flags"])
            {
                const auto& flags = config["flags"];
                for (const auto& flag : flags)
                {
                    std::string key = flag.first.as<std::string>();
                    std::string value = flag.second.as<std::string>();
                    buildConfig->flags[key] = value;
                }
            }
            
            if (config["compilerVersion"])
                buildConfig->compilerVersion = config["compilerVersion"].as<std::string>();

            if (config["languageVersion"])
                buildConfig->languageVersion = config["languageVersion"].as<std::string>();

            if (config["buildType"])
                buildConfig->buildType = config["buildType"].as<std::string>();

            Logger::Info("Successfully read config file");
        }
        catch (const YAML::Exception& e)
        {
            std::cerr << "ERROR: Couldn't read config file: " << e.what() << std::endl;
            exit(1);
        }
    }

    std::shared_ptr<BuildConfig> GetBuildConfig()
    {
        if (buildConfig == nullptr)
            ReadConfig();

        return buildConfig;
    }
};