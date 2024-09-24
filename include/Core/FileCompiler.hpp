#pragma once

#include "Core/BuildEngine.hpp"
#include "Utils/ConfigReader.hpp"

class FileCompiler
{
public:
    FileCompiler(std::shared_ptr<BuildConfig> config);

    void SetupDirectories();

    void CompileObjectFiles();

    void LinkObjectFiles();

    void RunBinaryExecutable(std::string arguments);

private:
    std::shared_ptr<BuildConfig> config;
    std::shared_ptr<BuildEngine> buildEngine;

    // NOTE: Usually, only files in the src directories are compiled
    // But if the user is using qt, ui and header files also need to be compiled
    // So instead of checking 3 different arrays of directories, I just add
    // the ui and include directories to the src directories if
    // compiling ui or compiling headers are enabled accordingly
    std::vector<std::string> directoriesForCompilation;

    // NOTE: The output of the LinkObjectFiles is saved
    // so that it can be later ran by RunBinaryExecutable if needed
    std::string output;
};