#include "Core/FileCompiler.hpp"
#include "Utils/Logger/Logger.hpp"

#include <algorithm>
#include <fmt/core.h>

#include "Utils/RegexHelper.hpp"

void FileCompiler::SetupDirectories()
{
    std::vector<std::string> tempDirs;

    // Add directories from config to tempDirs
    auto AddDirsFromConfig = [&](const std::vector<std::string>& dirs)
    {
        if (dirs.empty()) return;

        tempDirs.insert(tempDirs.end(), dirs.begin(), dirs.end());
    };

    // UI and headers need to be added to the array first so that they're
    // compiled first. This is because these files need to be included
    // and so source files won't compile without them.

    if (m_config->qtSupport.at("compile_ui") == ConfigConstants::TRUE)
    {
        AddDirsFromConfig(m_config->directories.at("ui"));
    }

    if (m_config->qtSupport.at("compile_moc") == ConfigConstants::TRUE)
    {
        AddDirsFromConfig(m_config->directories.at("include"));
    }

    AddDirsFromConfig(m_config->directories.at("src"));

    // Remove duplicates
    for (const auto& dir : tempDirs)
    {
        if (std::find(m_directoriesForCompilation.begin(), m_directoriesForCompilation.end(), dir) == m_directoriesForCompilation.end())
        {
            m_directoriesForCompilation.push_back(dir);
        }
    }
}

void FileCompiler::CompileObjectFiles(bool rebuild)
{
    for (const auto& dir : m_directoriesForCompilation)
    {
        const fs::path dirPath = dir;

        if (RegexHelper::MatchesRegex(dirPath, m_config->exclude)) {
            Logger::Info(fmt::format("Skipping excluded directory '{}'", dirPath.string()));
            continue;
        }

        if (fs::is_empty(dirPath))
        {
            Logger::Warning(fmt::format("Source directory '{}' is empty. No files to process, skipping...", dir));
            continue;
        }

        for (auto it = fs::recursive_directory_iterator(dirPath); it != fs::recursive_directory_iterator(); ++it) {
            const fs::directory_entry& entry = *it;
            const fs::path sourcePath = entry.path();

            if (RegexHelper::MatchesRegex(sourcePath, m_config->exclude)) {
                Logger::Info(fmt::format("Skipping excluded '{}'", sourcePath.string()));

                // If it's a directory, skip recursion into it
                if (fs::is_directory(sourcePath)) {
                    it.disable_recursion_pending();
                }
                continue;
            }

            if (!fs::is_regular_file(entry))
                continue;

            this->CompileObjectFile(sourcePath, rebuild);

            Logger::Info(fmt::format("Compiled {}", sourcePath.string()));
        }
    }
}

void FileCompiler::CompileObjectFile(const fs::path& sourcePath, bool rebuild)
{
    // Get filename without path
    const std::string sourceFileName = sourcePath.stem().string();
    const std::string sourceExtension = sourcePath.extension().string().substr(1);

    try
    {
        const std::string outputPath = m_buildEngine->GetOutputPath(sourceFileName, sourceExtension);

        if (outputPath == "")
        {
            Logger::Warning(fmt::format("Skipping compilation of file '{}'", sourcePath.string()));
            return;
        }

        const fs::path outputFile = outputPath;

        // If the rebuild flag is passed, just skip this check
        if (fs::exists(outputFile) && !rebuild)
        {
            auto sourceLastModified = fs::last_write_time(sourcePath);
            auto outputLastModified = fs::last_write_time(outputFile);

            // Skip compilation if the source file is older than the object file (up-to-date)
            if (sourceLastModified <= outputLastModified)
            {
                Logger::Debug(fmt::format("Skipping {} (up to date)", sourcePath.string()));
                return;
            }
        }

        const std::string command = m_buildEngine->GetCompileCommandForFile(sourceExtension, sourcePath.string(), outputPath);

        // Safety check
        if (command.empty())
        {
            Logger::Error(fmt::format("Empty compile command was returned for file {}", sourcePath.string()));
            return;
        }

        const int exitStatus = system(command.c_str());

        if (exitStatus != 0)
            throw std::runtime_error(command);
    }
    catch (const std::runtime_error& command)
    {
        Logger::Error(fmt::format("Failed to compile '{}'", sourcePath.string()));
        Logger::Fatal(fmt::format("Command: {}", command.what()));
    }
}

void FileCompiler::LinkObjectFiles()
{
    const fs::path objPath = m_config->directories.at("obj")[0];

    if (fs::is_empty(objPath))
    {
        Logger::Warning("No object files were found, skipping linking phase...");
        return;
    }

    std::vector<std::string> objects;

    const fs::directory_iterator objDir (objPath);

    for (auto const& file : objDir)
    {
        const std::string filePath = file.path().string();
        objects.push_back(filePath);
    }

    m_output = fmt::format(
        "{}/{}{}{}",
        m_config->directories.at("bin")[0],
        m_config->output,
        m_config->extension != "" ? "." : "",
        m_config->extension
    );

    try
    {
        const std::string command = m_buildEngine->GetLinkCommandForProject(objects, m_output);

        const int exitStatus = system(command.c_str());

        if (exitStatus != 0)
            throw std::runtime_error(command);
    }
    catch (const std::runtime_error& command)
    {
        Logger::Error("Failed when linking project");
        Logger::Fatal(fmt::format("Command: {}", command.what()));
    }

    Logger::Info("Build successful");
}

void FileCompiler::RunBinaryExecutable(const std::string& arguments)
{
    Logger::Assert(!m_output.empty(), "Binary executable wasn't found when trying to run it. Something has gone wrong");

    printf("\n");
    if (arguments.empty())
        Logger::Info("Executing compiled binary...");
    else
        Logger::Info(fmt::format("Executing binary with arguments: '{}'", arguments.substr(0, arguments.length() - 1)));

    try
    {
        std::string command = fmt::format("./{} {}", m_output, arguments);

        int platform = Platform::GetPlatform();

        if (platform == Platform::Platforms::WINDOWS)
        {
            std::replace(command.begin(), command.end(), '/', '\\');
        }

        const int exitStatus = system(command.c_str());

        if (exitStatus != 0)
            throw (command);
    }
    catch (std::string command)
    {
        Logger::Error("Failed when running binary executable");
        Logger::Fatal(fmt::format("Command: {}", command));
    }
}
