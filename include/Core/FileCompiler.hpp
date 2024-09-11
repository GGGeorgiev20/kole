#pragma once

#include <filesystem>

#include "Core/BuildEngine.hpp"
#include "Utils/ConfigReader.hpp"

class FileCompiler
{
public:
    FileCompiler(std::shared_ptr<BuildConfig> config);

    void CompileObjectFiles();

    void LinkObjectFiles();

private:
    std::shared_ptr<BuildConfig> config;
    std::shared_ptr<BuildEngine> buildEngine;
};