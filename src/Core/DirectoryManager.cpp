#include "Core/DirectoryManager.hpp"
#include "Utils/Logger/Logger.hpp"

#include <filesystem>

namespace fs = std::filesystem;

void DirectoryManager::InitializeProject()
{
    // A config file should have already been created

    for (const auto& [directoryKey, directories] : m_config->directories)
    {
        for (const auto& directory : directories)
        {
            CreateDirectory(directory);
        }
    }

    // TODO: Create .gitignore file
}

void DirectoryManager::CreateDirectories()
{
    for (const auto& directory : m_config->autocreate)
    {
        CreateDirectory(directory);
    }
}

void DirectoryManager::CreateDirectory(const std::string& directory)
{
    if (directory.empty()) return;

    try
    {
        const fs::path directoryPath = directory;

        if (fs::exists(directoryPath)) return;

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
