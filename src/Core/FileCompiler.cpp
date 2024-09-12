#include "Core/FileCompiler.hpp"
#include "Utils/Logger/Logger.hpp"

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
            // TODO: Don't compile files if the last modified timestamp of the source file is older than the creation date of its object file (if it exists)
            
            // Only compile files, not directories
            if (fs::is_regular_file(entry.status()))
            {
                std::string path = entry.path().string();
                std::string name = entry.path().filename().string();

                // Remove extension, leave only name
                name = name.substr(0, name.find("."));

                std::string object = fmt::format("{}/{}.o", config->directories.at("obj")[0], name);

                try
                {
                    std::string command = buildEngine->GetCompileCommandForFile(path, object);
                    system(command.c_str());
                }
                catch (...)
                {
                    Logger::Fatal(fmt::format("Failed when compiling {}", path));
                }

                Logger::Info(fmt::format("Compiled {}", path));
                objects.push_back(object);
            }
        }
    }
}

void FileCompiler::LinkObjectFiles()
{
    try
    {
        std::string command = buildEngine->GetLinkCommandForProject(objects);
        system(command.c_str());

        Logger::Info("Build successful");
    }
    catch (...)
    {
        Logger::Fatal("Failed when linking project");
    }
}
