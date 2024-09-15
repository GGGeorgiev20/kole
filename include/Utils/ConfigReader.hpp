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

    std::map<std::string, std::vector<std::string>> directories = {
        { "src",              {   "src"   } },
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

    std::string compilerVersion = "g++";
    std::string languageVersion = "c++20";

    std::string optimization = "debug";
};

namespace ConfigReader
{
    void CreateConfig();

    void ReadConfig();

    void ValidateProperties();

    std::string ProcessProperty(std::string property);

    std::vector<std::string> ProcessProperty(std::vector<std::string> property);

    std::shared_ptr<BuildConfig> GetBuildConfig();

    // Variables

    extern std::string configPath;
    extern std::shared_ptr<BuildConfig> buildConfig;
    extern std::unordered_set<std::string> recognizedKeys;

    extern std::string defaultConfig;
};