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
        std::string argument(argv[i]);
        Logger::Debug(fmt::format("Processing argument '{}'", argument));

        bool argumentIsFound = false;

        if (argumentStates.at(Argument::Autorun) == true)
        {
            argumentsForAutorun += fmt::format("{} ", argument);
            continue;
        }
        
        if (argument[0] != '-')
            ArgumentNotFound(argument);

        bool longhand = false;

        if (argument[1] == '-')
            longhand = true;

        // Skip the 2 first characters of the argument (in this case '--') if the argument is long-hand
        // and just 1 if it's short-hand
        std::string sequence = argument.substr(1 + longhand);

        for (const auto& [key, value] : argumentIdentifiers)
        {
            if (argument.find(value[longhand]) != std::string::npos)
            {
                argumentStates[key] = true;
                argumentIsFound = true;
            }

            if (argumentIsFound)
            {
                if (key == Argument::Help)
                {
                    Logger::Debug("Help argument found. Stopping program and displaying help menu");
                    PrintHelp();
                    exit(0);
                }

                // Multiple short-hand arguments can be packed in one sequence,
                // but only one long-hand can be passed at once
                // e.g. '-hrc' as opposed to '--clear'
                if (longhand)
                    break;
            }
        }

        if (!argumentIsFound)
            ArgumentNotFound(argument);
    }


    for (const auto& [key, value] : argumentIdentifiers)
    {
        if (argumentStates.at(key) == true)
            Logger::Debug(fmt::format("Argument with identifier {} has state of true.", value[1]));
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

    const int optionIndentation = 2;
    const int minimalSpaceToDesc = 3;

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
        std::string identifiers = "";

        for (size_t i = 0; i < value.size(); i++)
        {
            if (i > 0) identifiers += ", ";

            if (value[i].length() > 1) identifiers += "-";
            identifiers += "-" + value[i];
        }

        std::cout << std::setw(optionIndentation) << " ";
        std::cout << std::left << std::setw(longestOption + minimalSpaceToDesc) << identifiers;
        std::cout << argumentDescriptions.at(key) << std::endl;
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
        printf(" [");
        for (size_t i = 0; i < value.size(); i++)
        {
            const auto& identifier = value[i];

            if (i > 0) printf(", ");
            if (identifier.length() > 1) printf("-");
            printf("-%s", identifier.c_str());
        }
        printf("]");
    }
    printf("\n");
}

void ArgumentManager::PrintUnrecognizedArgument(std::string argument)
{
    printf("kole: error: unrecognized arguments: %s\n", argument.c_str());
}

void ArgumentManager::ArgumentNotFound(std::string argument)
{
    PrintUsage();
    PrintUnrecognizedArgument(argument);
    exit(1);
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
