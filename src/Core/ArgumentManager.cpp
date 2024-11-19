#include "Core/ArgumentManager.hpp"
#include "Utils/Logger/Logger.hpp"

#include <fmt/core.h>
#include <iomanip>

ArgumentManager::ArgumentManager(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

void ArgumentManager::ProcessArguments()
{
    // TODO: Fix arguments. Very bugged.

    // Skip the first argument, as it's the name of the executable
    for (int i = 1; i < argc; i++)
    {
        char* argument = argv[i];
        Logger::Debug(fmt::format("Processing argument '{}'", argument));

        bool argumentIsFound = false;

        if (argumentStates.at(Argument::Autorun) == true)
        {
            argumentsForAutorun += fmt::format("{} ", argument);
            continue;
        }

        for (const auto& [key, value] : argumentIdentifiers)
        {
            for (const auto& identifier : value)
            {
                if (identifier == argument)
                {
                    argumentStates[key] = true;
                    argumentIsFound = true;
                    break;
                }
            }

            if (argumentIsFound)
            {
                if (key == Argument::Help)
                {
                    Logger::Debug("Found help argument. Stopping and displaying help menu");
                    PrintHelp();
                    exit(0);
                }

                break;
            }
        }

        if (!argumentIsFound)
        {
            PrintUsage();
            PrintUnrecognizedArgument(argument);
            exit(1);
        }
    }
}

void ArgumentManager::PrintHelp()
{
    PrintUsage();

    printf("\n%s\n", description.c_str());

    printf("\noptions:\n");

    // Determine the longest option string to align the argument descriptions for formatting.
    // For example:
    // -h, --help   Shows this help menu
    // --clear      Clears the object files

    uint16_t longestOption = 0;

    for (const auto& [key, value] : argumentIdentifiers)
    {
        // Set default to -2 to simplify calculations (add 2 for every identifier (comma and space) instead of checking if this is the last identifier)
        uint16_t optionLength = -2;

        for (const auto& identifier : value)
        {
            auto length = identifier.length();

            optionLength += length + 2;

            // Add 1 or 2 to account for the dashes in front of the argument
            optionLength += length > 1 ? 2 : 1;
        }

        if (optionLength > longestOption)
            longestOption = optionLength;
    }

    for (const auto& [key, value] : argumentIdentifiers)
    {
        for (size_t i = 0; i < value.size(); i++)
        {
            if (i > 0) printf(", ");

            printf("-%c%s", value[i].length() > 1 && '-', value[i].c_str());
        }

        std::cout << std::left << std::setw(longestOption + 2) << argumentDescriptions.at(key) << std::endl;
    }
}

void ArgumentManager::PrintUsage()
{
    printf("usage: kole");
    for (const auto& [key, value] : argumentIdentifiers)
    {
        // The arguments in the argument map are sorted so that the first element is always the short-hand, if it has one.
        // Because of that, we can check if the first element has a length longer than 1 (is long-hand)
        // and if it is, add an extra '-'.
        printf(" [-%c%s]", value[0].length() > 1 && '-', value[0].c_str());
    }
    printf("\n");
}

void ArgumentManager::PrintUnrecognizedArgument(std::string argument)
{
    printf("kole: error: unrecognized arguments: %s\n", argument.c_str());
}

std::string ArgumentManager::GetArgumentsForAutorun()
{
    if (this->argumentsForAutorun.empty())
    {
        Logger::Debug("No arguments for autorun were found");
    }
    else
    {
        Logger::Debug(fmt::format("Arguments for binary execution: [{}]", this->argumentsForAutorun));
    }

    return this->argumentsForAutorun;
}

bool ArgumentManager::GetArgumentState(Argument argument)
{
    return argumentStates.at(argument);
}
