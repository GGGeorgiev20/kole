#include "Core/FileCompiler.hpp"
#include "Utils/Logger/Logger.hpp"

#include <algorithm>

namespace fs = std::filesystem;

FileCompiler::FileCompiler(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
    this->buildEngine = std::make_shared<BuildEngine>(config);
}

void FileCompiler::CompileObjectFiles()
{
    for (const auto& dir : config->directories.at("src"))
    {
        const fs::path dirPath = dir;
        
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
                    system(command.c_str());
                }
                catch (...)
                {
                    Logger::Fatal(fmt::format("Failed when compiling {}", sourcePath));
                }

                Logger::Info(fmt::format("Compiled {}", sourcePath));
                objects.push_back(objectPath);
            }
        }
    }
}

void FileCompiler::LinkObjectFiles()
{
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
        system(command.c_str());

        Logger::Info("Build successful");
    }
    catch (...)
    {
        Logger::Fatal("Failed when linking project");
    }
}

void FileCompiler::RunBinaryExecutable(std::string arguments)
{
    if (output.empty())
    {
        Logger::Fatal("Binary executable wasn't found when trying to run it. Something has gone wrong");
    }
    
    std::string command = fmt::format("./{} {}", output, arguments);

    int platform = Platform::GetPlatform();

    if (platform == Platform::Platforms::WINDOWS)
    {
        std::replace(command.begin(), command.end(), '/', '\\');
    }

    system(command.c_str());
}
