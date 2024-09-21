#include "Core/FileCompiler.hpp"
#include "Utils/Logger/Logger.hpp"

#include <algorithm>
#include <fmt/core.h>

namespace fs = std::filesystem;

FileCompiler::FileCompiler(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
    this->buildEngine = std::make_shared<BuildEngine>(config);
}

/**
 * @brief Compiles files in every directories specified in the 'src' property of the config to object files.
 *
 * Iterate through every directory in the 'src' property and every file and subdirectory in it and compile them to object files.
 * By default, if the object file is modified earlier than the last time its source file has been modified, skip recompiling it.
 * (The object directory can be cleared using the 'clear' flag)
 *
 * Every object file is saved in the 'objects' array for linking after that.
 */
void FileCompiler::CompileObjectFiles()
{
    for (const auto& dir : config->directories.at("src"))
    {
        const fs::path dirPath = dir;

        if (fs::is_empty(dir))
        {
            Logger::Warning(fmt::format("Source directory '{}' is empty. No files to process, skipping...", dir));  
            continue;
        }

        for (const auto& entry : fs::recursive_directory_iterator(dirPath))
        {
            // Only compile files, not directories
            if (fs::is_regular_file(entry.status()))
            {
                std::string sourcePath = entry.path().string();
                std::string sourceFileName = entry.path().filename().string();

                // Remove extension, leave only the file name
                sourceFileName = sourceFileName.substr(0, sourceFileName.find("."));

                std::string objectPath = fmt::format("{}/{}.o", config->directories.at("obj")[0], sourceFileName);
                fs::path objectFile = objectPath;

                if (fs::exists(objectFile))
                {
                    auto sourceLastModified = fs::last_write_time(entry.path());
                    auto objectLastModified = fs::last_write_time(objectFile);

                    // Skip compilation if the source file is older than the object file
                    // To recompile every file, the clear flag needs to be ran which just clears the obj dir
                    if (sourceLastModified <= objectLastModified)
                    {
                        Logger::Debug(fmt::format("Skipping {} (up to date)", sourcePath));
                        continue;
                    }
                }

                        try
                        {
                            std::string command = buildEngine->GetCompileCommandForFile(sourcePath, objectPath);
                            const int exitStatus = system(command.c_str());

                            if (exitStatus != 0)
                                throw (command);
                        }
                        catch (std::string command)
                        {
                            Logger::Error(fmt::format("Failed when compiling {}", sourcePath));
                            Logger::Fatal(fmt::format("Command: ", command));
                        }

                Logger::Info(fmt::format("Compiled {}", sourcePath));
            }
        }
    }
}

/**
 * @brief Link saved object files to a binary executable.
 */
void FileCompiler::LinkObjectFiles()
{
    const fs::path objPath = config->directories.at("obj")[0];

    if (fs::is_empty(objPath))
    {
        Logger::Warning("No object files were found, skipping linking phase...");
        return;
    }

    std::vector<std::string> objects;

    const fs::directory_iterator objDir (objPath);

    for (auto const& file : objDir)
    {
        const std::string filePath = file.path().string();
        objects.push_back(filePath);
    }

            try
            {
                output = fmt::format(
                    "{}/{}{}{}",
                    config->directories.at("bin")[0],
                    config->output,
                    config->extension != "" ? "." : "",
                    config->extension
                );

                std::string command = buildEngine->GetLinkCommandForProject(objects, output);
                const int exitStatus = system(command.c_str());

                if (exitStatus != 0)
                    throw (command);

                Logger::Info("Build successful");
            }
            catch (std::string command)
            {
                Logger::Error("Failed when linking project");
                Logger::Fatal(fmt::format("Command: {}", command));
            }
}

/**
 * @brief Run the compiled binary executable with arguments.
 *
 * Executes the compiled binary.
 * On Linux, paths use '/' while on Windows, paths use '\'.
 * Adjust accordingly for compatibility.
 *
 * @param arguments: Arguments to pass to the executable.
 */
void FileCompiler::RunBinaryExecutable(std::string arguments)
{
    Logger::Assert("Binary executable wasn't found when trying to run it. Something has gone wrong", !output.empty());

    printf("\n");
    if (arguments.empty())
        Logger::Info("Executing compiled binary..");
    else
        Logger::Info(fmt::format("Executing binary with arguments: '{}'", arguments.substr(0, arguments.length() - 1)));

    std::string command = fmt::format("./{} {}", output, arguments);

    int platform = Platform::GetPlatform();

    if (platform == Platform::Platforms::WINDOWS)
    {
        std::replace(command.begin(), command.end(), '/', '\\');
    }

    system(command.c_str());
}
