#include "Core/ArgumentManager.hpp"
#include "Core/DirectoryManager.hpp"
#include "Core/FileCompiler.hpp"

#include "Core/ConfigReader.hpp"
#include "Utils/Logger/Logger.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ArgumentManager> argumentManager = std::make_shared<ArgumentManager>(argc, argv);
    argumentManager->ProcessArguments();

    if (argumentManager->GetArgumentState(Argument::Debug))
        LogTypes::EnableDebug();

    std::shared_ptr<ConfigReader> configReader = ConfigReader::GetInstance();

    if (argumentManager->GetArgumentState(Argument::Config))
        configReader->CreateConfig();

    std::shared_ptr<BuildConfig> config = configReader->GetBuildConfig();
    configReader->PostProcess();

    std::shared_ptr<DirectoryManager> directoryManager = std::make_shared<DirectoryManager>(config);

    if (argumentManager->GetArgumentState(Argument::Initialize))
        directoryManager->InitializeProject();
    else
        directoryManager->CreateDirectories();

    if (argumentManager->GetArgumentState(Argument::Config) || argumentManager->GetArgumentState(Argument::Initialize))
        return 0;

    std::shared_ptr<FileCompiler> fileCompiler = std::make_shared<FileCompiler>(config);

    bool rebuild = argumentManager->GetArgumentState(Argument::Rebuild);
    if (rebuild)
        Logger::Info("Rebuilding all files...");

    fileCompiler->CompileObjectFiles(rebuild);
    fileCompiler->LinkObjectFiles();

    if (argumentManager->GetArgumentState(Argument::Autorun))
        fileCompiler->RunBinaryExecutable(argumentManager->GetAutorunArguments());
}
