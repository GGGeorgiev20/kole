#pragma once

#include <filesystem>

#include "Core/BuildEngine.hpp"
#include "Core/ConfigReader.hpp"

namespace fs = std::filesystem;

class FileCompiler
{
public:
    FileCompiler(std::shared_ptr<BuildConfig> config)
        : m_config(config)
    {
        m_buildEngine = std::make_shared<BuildEngine>(m_config);
        this->SetupDirectories();
    }

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
     * Iterates through configured directories (e.g., 'src')
     * and calls the compile object file function for each one.
     *
     * @param rebuild Whether to rebuild all files.
     */
    void CompileObjectFiles(bool rebuild);

    /**
     * @brief Compiles a source file to an object file.
     *
     * Compiles source files into object files.
     * Includes UI and header files if QT support is enabled.
     *
     * @param sourcePath The path to the source file.
     * @param rebuild Whether to rebuild the file.
     */
    void CompileObjectFile(fs::path parentDirectory, fs::path childPath, bool rebuild);

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
    void RunBinaryExecutable(const std::string& arguments);

private:
    std::shared_ptr<BuildConfig> m_config;
    std::shared_ptr<BuildEngine> m_buildEngine;

    // NOTE: Usually, only files in the src directories are compiled.
    // But if the user is using Qt, UI & header files also need to be compiled.
    // So instead of checking 3 different arrays of directories,
    // just add the UI, include & src directories to one array
    std::vector<std::string> m_directoriesForCompilation;

    // NOTE: The output of the LinkObjectFiles is saved, so that it can be ran later by RunBinaryExecutable if needed
    std::string m_output;
};
