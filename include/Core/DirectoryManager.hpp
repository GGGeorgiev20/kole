#pragma once

#include <memory>

#include "Utils/ConfigReader.hpp"

class DirectoryManager
{
public:
    DirectoryManager(std::shared_ptr<BuildConfig> config);

    void CreateDirectories();

    void CreateDirectory(std::string directory);

    void ClearObjectDirectory();

private:
    std::shared_ptr<BuildConfig> config;
};
