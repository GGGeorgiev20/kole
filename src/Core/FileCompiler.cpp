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
            // Only compile files, not directories
            if (fs::is_regular_file(entry.status()))
            {
                std::string path = entry.path().string();
                std::string name = entry.path().filename().string();

                // Remove extension, leave only name
                name = name.substr(0, name.find("."));
                
                Logger::Info(fmt::format("Compiled {}", path));
                
                std::string command = buildEngine->GetCompileCommandForFile(path, name);
                system(command.c_str());
            }
        }
    }
}

void FileCompiler::LinkObjectFiles()
{

}
