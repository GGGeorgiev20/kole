#pragma once

#include "Core/BuildEngine.hpp"
#include "Utils/ConfigReader.hpp"

class FileCompiler
{
public:
    FileCompiler(std::shared_ptr<BuildConfig> config);

    /**
     * @brief Adds relevant directories to an array for compilation.
     *
     * E.g. [ src ] -> [ ui, src ] if UI compilation is enabled,
     * [ src ] -> [ include, src ] if header compilation is enabled.
     */
    void SetupDirectories();

    /**
     * @brief Compiles source files to object files.
     *
     * Iterates through configured directories (e.g., 'src') and compiles source
     * files into object files. Includes UI and header files if QT support is enabled.
     */
    void CompileObjectFiles();

    /**
     * @brief Links object files into a binary executable.
     *
     * Links compiled object files into an executable binary, applying platform-specific
     * flags. Saves the output path for optional execution.
     */
    void LinkObjectFiles();

    /**
     * @brief Runs the compiled binary executable with optional arguments.
     *
     * Adjusts path separators for compatibility with the target platform (Linux/MacOS/Windows).
     *
     * @param arguments Arguments to pass to the executable.
     */
    void RunBinaryExecutable(std::string arguments);

private:
    std::shared_ptr<BuildConfig> config;
    std::shared_ptr<BuildEngine> buildEngine;

    // NOTE: Usually, only files in the src directories are compiled.
    // But if the user is using Qt, UI & header files also need to be compiled.
    // So instead of checking 3 different arrays of directories,
    // just add the UI, include & src directories to one array
    std::vector<std::string> directoriesForCompilation;

    // NOTE: The output of the LinkObjectFiles is saved
    // so that it can be ran later by RunBinaryExecutable if needed
    std::string output;
};