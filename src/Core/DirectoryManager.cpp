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
    for (const auto& [directoryKey, directories] : config->directories)
    {
        for (const auto& directory : directories)
        {
            CreateDirectory(directory);
        }
    }
}

void DirectoryManager::CreateDirectory(std::string directory)
{
    if (directory.empty()) return;

    try
    {
        const fs::path directoryPath = directory;

        if (fs::exists(directoryPath))
        {
            Logger::Warning(fmt::format("Directory '{}' already exists. Skipping...", directory));
            return;
        }

        if (fs::create_directory(directoryPath))
            Logger::Info(fmt::format("Created empty directory '{}'", directory));
        else
            Logger::Error(fmt::format("Failed to create directory '{}'", directory));
    }
    catch (const std::exception& e)
    {
        Logger::Error(fmt::format("Failed to create directory '{}'", directory));
        std::cerr << e.what() << '\n';
    }
}

void DirectoryManager::ClearObjectDirectory()
{
    try
    {
        const fs::path objPath = config->directories.at("obj")[0];
        const fs::directory_iterator objDir (objPath);

        if (fs::is_empty(objPath))
        {
            Logger::Debug("Object directory is empty. Nothing to clear");
            return;
        }

        for (auto const& file : objDir)
        {
            const fs::path filePath = file.path();
            fs::remove(filePath);
        }

        Logger::Debug("Cleared object directory");
    }
    catch (const std::exception& e)
    {
        Logger::Error("Failed to clear object directory");
        std::cerr << e.what() << '\n';
    }
}
