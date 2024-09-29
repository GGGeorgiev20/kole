#pragma once

#include <memory>

#include "Core/ConfigReader.hpp"

class BuildEngine
{
public:
    BuildEngine(std::shared_ptr<BuildConfig> config);

    /**
     * @brief Generates the command to compile a file.
     *
     * Constructs the compile command for a file depending on its' extension.
     * Supports source, header and UI files.
     *
     * @param sourcePath: The source file path.
     * @param sourceFileName: The source file name.
     * @param sourceExtension: The source file extension.
     *
     * @return The formatted compile command.
     */
    std::string GetCompileCommandForFile(std::string sourcePath, std::string sourceFileName, std::string sourceExtension);

    /**
     * @brief Generates the command to link object files into a final binary.
     *
     * Constructs the link command using the compiler version, flags, object files,
     * and output binary name.
     *
     * @param files: Vector of object files to link.
     * @param output: The output binary name.
     *
     * @return The formatted link command.
     */
    std::string GetLinkCommandForProject(std::vector<std::string> files, std::string output);

    /**
     * @brief Generates the command to compile a source file to an object file.
     *
     * @param source: The source file to compile.
     * @param object: The output file path.
     *
     * @return The formatted compile command.
     */
    std::string GetCompileCommandForSourceFile(std::string source, std::string output);

    /**
     * @brief Generates the command to compile a header file to a moc file.
     *
     * @param source: The source file to compile.
     * @param object: The output file path.
     *
     * @return The formatted compile command.
     */
    std::string GetCompileCommandForHeaderFile(std::string source, std::string output);

    /**
     * @brief Generates the command to compile a UI file to a UI header file.
     *
     * @param source: The source file to compile.
     * @param object: The output file path.
     *
     * @return The formatted compile command.
     */
    std::string GetCompileCommandForUIFile(std::string source, std::string output);

    /**
     * @brief Retrieves and caches the necessary compile flags.
     *
     * Gathers platform-specific and common flags, along with optimization levels,
     * and caches them for future use.
     *
     * @return The formatted compile flags.
     */
    std::string GetFlags();

    /**
     * @brief Retrieves and caches the necessary include paths.
     *
     * Collects include directories from the configuration and caches them for future use.
     *
     * @return The formatted include paths.
     */
    std::string GetIncludePaths();

private:
    std::shared_ptr<BuildConfig> config;

    std::string flags = std::string();
    std::string includePaths = std::string();

    std::map<std::string, std::string> optimizationLevels = {
        { "none",         "-O0"    },  // No optimization
        { "opt1",         "-O1"    },  // Optimization Level 1
        { "opt2",         "-O2"    },  // Optimization Level 2
        { "release",      "-O3"    },  // Optimization Level 3
        { "fast",         "-Ofast" },  // Aggressive optimization
        { "debug",        "-Og"    },  // Optimization perfected for debugging
        { "size",         "-Os"    },  // Optimization for size
    };
};