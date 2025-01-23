#pragma once

#include <memory>

#include "Core/ConfigReader.hpp"
#include "Utils/FlagManager.hpp"

class BuildEngine
{
public:
    BuildEngine(std::shared_ptr<BuildConfig> config);

    /**
     * @brief Generates the output path, given a source file.
     *
     * Constructs the output path for a file depending on its' file name and extension.
     * Supports source, header and UI files.
     *
     * @param sourceFileName The source file name.
     * @param sourceExtension The source file extension.
     *
     * @return The respective output path.
     */
    std::string GetOutputPath(std::string sourceFileName, const std::string& sourceExtension);

    /**
     * @brief Generates the command to compile a file.
     *
     * Constructs the compile command for a file depending on its' extension.
     * Supports source, header and UI files.
     *
     * @param sourceExtension The source file extension.
     * @param sourcePath The source file path.
     * @param outputPath The output file path.
     *
     * @return The formatted compile command.
     */
    std::string GetCompileCommandForFile(const std::string& sourceExtension, const std::string& sourcePath, const std::string& outputPath);

    /**
     * @brief Generates the command to link object files into a final binary.
     *
     * Constructs the link command using the compiler version, flags, object files,
     * and output binary name.
     *
     * @param files Vector of object files to link.
     * @param output The output binary name.
     *
     * @return The formatted link command.
     */
    std::string GetLinkCommandForProject(const std::vector<std::string>& files, const std::string& output);

private:
    /**
     * @brief Generates the command to compile a source file to an object file.
     */
    std::string GetCompileCommandForSourceFile(const std::string& source, const std::string& output);

    /**
     * @brief Generates the command to compile a header file to a moc file.
     */
    std::string GetCompileCommandForHeaderFile(const std::string& source, const std::string& output);

    /**
     * @brief Generates the command to compile a UI file to a UI header file.
     */
    std::string GetCompileCommandForUIFile(const std::string& source, const std::string& output);

private:
    std::shared_ptr<BuildConfig> m_config;

    std::unique_ptr<FlagManager> m_flagManager;
};
