#include "Core/ArgumentManager.hpp"
#include "Core/DirectoryManager.hpp"
#include "Core/FileCompiler.hpp"

#include "Utils/ConfigReader.hpp"
#include "Utils/Logger/LogTypes.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ArgumentManager> argumentManager = std::make_shared<ArgumentManager>(argc, argv);
    argumentManager->ProcessArguments();

    if (argumentManager->GetArgumentState(Argument::Debug))
        LogTypes::EnableDebug();

    std::shared_ptr<BuildConfig> config = ConfigReader::GetBuildConfig();
    ConfigReader::ValidateProperties();
    
    std::shared_ptr<DirectoryManager> directoryManager = std::make_shared<DirectoryManager>(config);
    directoryManager->CreateDirectories();

    if (argumentManager->GetArgumentState(Argument::Clear))
        directoryManager->ClearObjectDirectory();
    
    std::shared_ptr<FileCompiler> fileCompiler = std::make_shared<FileCompiler>(config);

    fileCompiler->CompileObjectFiles();
    fileCompiler->LinkObjectFiles();
}