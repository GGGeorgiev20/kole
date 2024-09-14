#include "Core/DirectoryManager.hpp"
#include "Utils/Logger/Logger.hpp"
#include <filesystem>

namespace fs = std::filesystem;

DirectoryManager::DirectoryManager(std::shared_ptr<BuildConfig> config)
{
    this->config = config;
}

void DirectoryManager::CreateDirectories()
{
    for (const auto& [key, value] : config->directories)
    {
        bool dirIsExcluded = false;

        for (const auto& dir : config->exclude)
        {
            if (key == dir)
            {
                dirIsExcluded = true;
                break;
            }
        }

        try
        {
            const std::filesystem::path directoryPath = key;

            if (dirIsExcluded) continue;
            if (std::filesystem::exists(directoryPath)) continue;

            const bool directory = fs::create_directory(directoryPath);
            if (directory) {
                Logger::Debug(fmt::format("Successfully created directory '{}'", key));
            }
            else {
                Logger::Error(fmt::format("Failed to create directory '{}'", key));
            }
        }
        catch (const std::exception& e)
        {
            Logger::Error(fmt::format("Failed to create directory '{}'", key));
            std::cerr << e.what() << '\n';
        }
    }
}

void DirectoryManager::ClearObjectDirectory()
{
    Logger::Debug("Clearing object directory");

    try
    {
        const fs::path objPath = config->directories.at("obj")[0];
        const fs::directory_iterator objDir (objPath);
        
        for (auto const& file : objDir)
        {
            const fs::path filePath = file.path();
            fs::remove(filePath);
        }
    }
    catch (const std::exception& e)
    {
        Logger::Error("Failed to clear object directory");
        std::cerr << e.what() << '\n';
    }
}
