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
    void CreateConfig();

    void ReadConfig();

    void PostProcess();

    std::string ProcessProperty(std::string property);

    std::vector<std::string> ProcessProperty(std::vector<std::string> property);

    std::shared_ptr<BuildConfig> GetBuildConfig();

    // Variables

    extern std::string configPath;
    extern std::shared_ptr<BuildConfig> buildConfig;
    extern std::unordered_set<std::string> recognizedKeys;

    extern std::string defaultConfig;
};