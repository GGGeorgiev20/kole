#include "Core/ArgumentManager.hpp"
#include "Core/DirectoryManager.hpp"
#include "Core/FileCompiler.hpp"

#include "Core/ConfigReader.hpp"
#include "Utils/Logger/LogTypes.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ArgumentManager> argumentManager = std::make_shared<ArgumentManager>(argc, argv);
    argumentManager->ProcessArguments();

    if (argumentManager->GetArgumentState(Argument::Debug))
        LogTypes::EnableDebug();

    if (argumentManager->GetArgumentState(Argument::Config))
        ConfigReader::CreateConfig();

    std::shared_ptr<BuildConfig> config = ConfigReader::GetBuildConfig();
    ConfigReader::PostProcess();

    std::shared_ptr<DirectoryManager> directoryManager = std::make_shared<DirectoryManager>(config);

    if (argumentManager->GetArgumentState(Argument::Setup))
        directoryManager->CreateDirectories();

    if (argumentManager->GetArgumentState(Argument::Config) || argumentManager->GetArgumentState(Argument::CreateDirs))
        return 0;

    if (argumentManager->GetArgumentState(Argument::Clear))
        directoryManager->ClearObjectDirectory();

    std::shared_ptr<FileCompiler> fileCompiler = std::make_shared<FileCompiler>(config);

    fileCompiler->CompileObjectFiles();
    fileCompiler->LinkObjectFiles();

    if (argumentManager->GetArgumentState(Argument::Autorun))
        fileCompiler->RunBinaryExecutable(argumentManager->GetArgumentsForAutorun());
}
