#include "Core/ArgumentManager.hpp"
#include "Utils/Logger/Logger.hpp"

#include <fmt/core.h>
#include <iomanip>

ArgumentManager::ArgumentManager(int argc, char** argv)
{
    m_argc = argc;
    m_argv = argv;
}

void ArgumentManager::ProcessArguments()
{
    // NOTE: No debug logs can be used here as they won't be enabled until the debug argument is processed.

    // Skip the first argument, as it's the name of the executable
    for (int i = 1; i < m_argc; i++)
    {
        std::string argument(m_argv[i]);

        // If the autorun argument has been 
        if (m_argumentStates.at(Argument::Autorun))
        {
            m_autorunArguments += fmt::format("{} ", argument);
            continue;
        }

        if (argument.length() == 1)
        {
            if (argument[0] == '-')
                this->PrintUnspecifiedArgument(false);
            else
                this->PrintUnrecognizedArgument(argument);
        }

        if (argument.length() == 2 && argument[1] == '-')
        {
            if (argument[1] == '-')
                this->PrintUnspecifiedArgument(true);
            else
                this->PrintUnrecognizedArgument(argument);
        }

        bool longhand = argument[1] == '-';

        // Skip the 2 first characters of the argument (in this case '--') if the argument is long-hand
        // and just 1 if it's short-hand
        std::string sequence = argument.substr(longhand ? 2 : 1);

        bool argumentIsFound = false;

        for (const auto& [key, value] : m_argumentIdentifiers)
        {
            if (longhand)
            {
                // The first element of the value array is the short-hand, the second is the long-hand
                if (sequence == value[1])
                {
                    m_argumentStates[key] = true;
                    argumentIsFound = true;
                }
            }
            else
            {
                for (char c : sequence)
                {
                    // This converts the char to a string to use the string comparison methods
                    std::string option(1, c);

                    if (option == value[0])
                    {
                        m_argumentStates[key] = true;
                        argumentIsFound = true;
                        break;
                    }
                }
            }

            if (argumentIsFound && key == Argument::Help)
            {
                this->PrintHelp();
                exit(0);
            }
            else if (!argumentIsFound) this->PrintUnrecognizedArgument(argument);
        }
    }
}

void ArgumentManager::PrintHelp()
{
    PrintUsage();

    printf("\n%s\n", m_description.c_str());

    printf("\noptions:\n");

    // Determine the longest option string to align the argument descriptions for formatting.
    // For example:
    // -h, --help   Shows this help menu
    // --clear      Clears the object files

    const int optionIndentation = 2;
    const int minimalSpaceToDesc = 3;

    uint16_t longestOption = 0;

    for (const auto& [key, value] : m_argumentIdentifiers)
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

    for (const auto& [key, value] : m_argumentIdentifiers)
    {
        std::string identifiers = "";

        for (std::size_t i = 0; i < value.size(); i++)
        {
            if (i > 0) identifiers += ", ";
            if (value[i].length() > 1) identifiers += "-";

            identifiers += "-" + value[i];
        }

        std::cout << std::setw(optionIndentation) << " ";
        std::cout << std::left << std::setw(longestOption + minimalSpaceToDesc) << identifiers;
        std::cout << m_argumentDescriptions.at(key) << std::endl;
    }
}

void ArgumentManager::PrintUsage()
{
    printf("usage: kole");

    for (const auto& [key, value] : m_argumentIdentifiers)
    {
        // The arguments in the argument map are sorted so that the first element is always the short-hand, if it has one.
        // Because of that, we can check if the first element has a length longer than 1 (is long-hand)
        // and if it is, add an extra '-'.
        printf(" [");
        for (std::size_t i = 0; i < value.size(); i++)
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

void ArgumentManager::PrintUnrecognizedArgument(const std::string& argument)
{
    printf("%s\n", fmt::format("kole: error: unrecognized argument '{}'", argument).c_str());
    printf("info: use -h or --help for help\n");
    exit(1);
}

void ArgumentManager::PrintUnspecifiedArgument(bool longhand)
{
    printf("%s\n", fmt::format("kole: error: unspecified argument after '{}'", longhand ? "--" : "-").c_str());
    printf("info: use -h or --help for help\n");
    exit(1);
}

std::string ArgumentManager::GetAutorunArguments()
{
    if (m_autorunArguments.empty())
    {
        Logger::Debug("No arguments for autorun were found");
    }
    else
    {
        Logger::Debug(fmt::format("Arguments for binary execution: [{}]", m_autorunArguments));
    }

    return m_autorunArguments;
}

bool ArgumentManager::GetArgumentState(const Argument& argument)
{
    return m_argumentStates.at(argument);
}
