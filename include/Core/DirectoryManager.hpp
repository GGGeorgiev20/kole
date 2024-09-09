#pragma once

#include <memory>

#include "Utils/ConfigReader/ConfigReader.hpp"

class DirectoryManager
{
public:
    DirectoryManager(std::shared_ptr<BuildConfig> config);

    void CreateDirectories();

    void ClearObjectDirectory();
    
private:
    std::shared_ptr<BuildConfig> config;
};