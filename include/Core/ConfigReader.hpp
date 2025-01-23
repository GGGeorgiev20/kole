#pragma once

#include <iostream>
#include <fstream>
#include <variant>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>

#include <yaml-cpp/yaml.h>

#include "Utils/Platform.hpp"

namespace ConfigConstants
{
    inline constexpr const char* AUTO = "auto";
    inline constexpr const char* FALSE = "false";
    inline constexpr const char* TRUE = "true";
}

struct BuildConfig
{
    std::string output = "main";
    std::string extension = ConfigConstants::AUTO;

    std::string platform = ConfigConstants::AUTO;

    // NOTE: The value of the map is a vector, not an array because these values (if not default) are got from the config and can have more than 1 elements
    std::map<std::string, std::vector<std::string>> directories = {
        { "src",          { "src" }     },
        { "ui",           { "" }        },
        { "obj",          { "obj" }     },
        { "bin",          { "bin" }     },
        { "include",      { "include" } },
    };

    std::vector<std::string> autocreate = { "obj", "bin" };
    std::vector<std::string> exclude;

    std::map<std::string, std::string> flags = {
        { "common",       "" },
        { "windows",      "" },
        { "linux",        "" },
        { "macos",        "" },
        { "unix",         "" },
    };

    // NOTE: The value type can actually be a variant,
    // meaning it can be either a string or a bool,
    // but in that case I will have to check if they're a specific type
    // every time they're cast (and that's annoying so I just won't bother)
    std::map<std::string, std::string> qtSupport = {
        { "compile_ui",         ConfigConstants::FALSE },
        { "compile_moc",        ConfigConstants::FALSE },
        { "ui_prefix",          "ui_"                  },
        { "ui_extension",       "h"                    },
        { "ui_output_dir",      "ui"                   },
        { "moc_prefix",         "moc_"                 },
    };

    std::string compiler = "g++";
    std::string languageVersion = "c++17";

    std::string optimization = "debug";
};

class ConfigReader
{
public:
    static std::shared_ptr<ConfigReader> GetInstance()
    {
        if (m_instance == nullptr)
            m_instance = std::make_shared<ConfigReader>();

        return m_instance;
    }

    /**
     * @brief Creates a default configuration file if one doesn't exist.
     */
    void CreateConfig();

    /**
     * @brief Reads and caches the config file.
     *
     * Handles unrecognized properties and errors, then creates a config object.
     */
    void ReadConfig();

    /**
     * @brief Validates and finalizes config values.
     *
     * Ensures required properties are valid and calculates 'auto' values.
     */
    void PostProcess();

    /**
     * @brief Processes a single property value.
     *
     * @param property The raw property value.
     * @return The processed value.
     */
    std::string ProcessProperty(const std::string& property);

    /**
     * @brief Processes a list of property values.
     *
     * @param property The raw list of property values.
     * @return The processed list of values.
     */
    std::vector<std::string> ProcessProperty(const std::vector<std::string>& property);

    /**
     * @brief Retrieves the cached config or reads a new one.
     *
     * Returns a shared pointer to the cached `BuildConfig` object, or reads
     * and creates a new one if it's the first time loading the config.
     */
    std::shared_ptr<BuildConfig> GetBuildConfig();

private:
    static std::shared_ptr<ConfigReader> m_instance;

    std::shared_ptr<BuildConfig> m_buildConfig = nullptr;

    std::string m_defaultConfigPath = "./assets/KoleConfig.default.yaml";

    std::string m_configPath = "./KoleConfig.yaml";
    std::array<std::string, 11> m_recognizedKeys = {
        "output",
        "extension",
        "platform",
        "directories",
        "autocreate",
        "exclude",
        "flags",
        "qt_support",
        "compiler",
        "language_version",
        "optimization"
    };
};
