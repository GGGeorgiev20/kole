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

struct BuildConfig
{
    std::string output = "main";
    std::string extension = "auto";

    std::string platform = "auto";

    std::map<std::string, std::vector<std::string>> directories = {
        { "src",              {   "src"   } },
        { "ui",               {    ""     } },
        { "obj",              {   "obj"   } },
        { "bin",              {   "bin"   } },
        { "include",          { "include" } },
    };

    std::vector<std::string> exclude;

    std::map<std::string, std::string> flags = {
        { "common",       "" },
        { "windows",      "" },
        { "linux",        "" },
        { "macos",        "" },
        { "unix",         "" },
    };

    // NOTE: The value type can actually be a variant meaning
    // it can be either a string or a bool but that means that
    // then I will have to check if they're a specific type
    // every time they're cast and that's annoying so I just won't bother
    std::map<std::string, std::string> qtSupport = {
        { "compile_ui",         "false" },
        { "compile_moc",        "false" },
        { "ui_prefix",            ""    },
        { "ui_extension",         ""    },
        { "ui_output_dir",        ""    },
        { "moc_prefix",           ""    },
    };

    std::string compilerVersion = "g++";
    std::string languageVersion = "c++17";

    std::string optimization = "debug";
};

namespace ConfigReader
{
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
    std::string ProcessProperty(std::string property);

    /**
     * @brief Processes a list of property values.
     *
     * @param property The raw list of property values.
     * @return The processed list of values.
     */
    std::vector<std::string> ProcessProperty(std::vector<std::string> property);

    /**
     * @brief Retrieves the cached config or reads a new one.
     *
     * Returns a shared pointer to the cached `BuildConfig` object, or reads
     * and creates a new one if it's the first time loading the config.
     */
    std::shared_ptr<BuildConfig> GetBuildConfig();

    // NAMESPACE VARIABLES

    extern std::string configPath;
    extern std::shared_ptr<BuildConfig> buildConfig;
    extern std::unordered_set<std::string> recognizedKeys;

    extern std::string defaultConfig;
};