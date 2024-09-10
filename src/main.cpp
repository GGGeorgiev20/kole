#include "Core/ArgumentManager.hpp"
#include "Core/DirectoryManager.hpp"
#include "Core/FlagManager.hpp"

#include "Utils/ConfigReader.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ArgumentManager> argumentManager = std::make_shared<ArgumentManager>(argc, argv);
    argumentManager->ProcessArguments();

    std::shared_ptr<BuildConfig> config = ConfigReader::GetBuildConfig();
    
    std::shared_ptr<DirectoryManager> directoryManager = std::make_shared<DirectoryManager>(config);
    directoryManager->CreateDirectories();

    if (argumentManager->GetArgumentState(Argument::Clear))
        directoryManager->ClearObjectDirectory();
    
    std::shared_ptr<FlagManager> flagManager = std::make_shared<FlagManager>(config);
    std::string flags = flagManager->GetFlags();
}