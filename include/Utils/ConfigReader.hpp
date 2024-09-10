#pragma once

#include <iostream>
#include <fstream>
#include <variant>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include <yaml-cpp/yaml.h>

#include "Utils/Platform.hpp"

struct BuildConfig
{
    std::string output = "main";
    std::string extension = Platform::GetOutputExtension();

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
    void ReadConfig();

    std::shared_ptr<BuildConfig> GetBuildConfig();
};