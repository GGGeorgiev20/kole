#pragma once

#include <memory>

#include "Core/ConfigReader.hpp"

class DirectoryManager
{
public:
    DirectoryManager(std::shared_ptr<BuildConfig> config);

    /**
     * @brief Create missing directories based on the config.
     *
     * Iterates through the configured directories and creates them unless they are excluded or already exist.
     */
    void CreateDirectories();

    /**
     * @brief Creates a directory if it doesn't exist.
     */
    void CreateDirectory(std::string directory);

    /**
     * @brief Clears all files in the object directory.
     */
    void ClearObjectDirectory();

private:
    std::shared_ptr<BuildConfig> config;
};
