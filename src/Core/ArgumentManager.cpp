#include "Core/ArgumentManager.hpp"
#include <iomanip>

ArgumentManager::ArgumentManager(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

void ArgumentManager::ProcessArguments()
{
    // Skip the first argument, as it's the name of the executable
    for (int i = 1; i < argc; i++)
    {
        char* argument = argv[i];

        bool argumentIsFound = false;
        
        for (const auto& [key, value] : argumentIdentifiers)
        {
            for (const auto& identifier : value)
            {
                if (identifier == argument)
                {
                    argumentFlags[key] = true;
                    argumentIsFound = true;
                    break;
                }
            }
            
            if (argumentIsFound)
            {
                if (key == Argument::Help)
                {
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
    
    size_t longestOption = 0;
    std::map<Argument, size_t> argumentLengths;

    for (const auto& [key, value] : argumentIdentifiers)
    {
        // Arguments can have multiple identifiers (e.g., "-h, --help").
        // These identifiers are separated by a comma and a space (2 characters total).
        // Instead of conditionally adding 2 characters after the first identifier, 
        // we initialize the length at -2 and always add 2 during each iteration.
        size_t currentOptionLength = -2;

        for (const auto& identifier : value)
        {
            currentOptionLength += identifier.length() + 2;
        }

        argumentLengths[key] = currentOptionLength;

        if (currentOptionLength > longestOption)
            longestOption = currentOptionLength;            
    }

    for (const auto& [key, value] : argumentIdentifiers)
    {
        // Print the first identifier without a leading comma.
        // Any additional identifiers will be printed with a preceding comma and space.
        printf("  ");
        printf("%s", value[0].c_str());
        
        for (size_t i = 1; i < value.size(); i++)
        {
            printf(", %s", value[i].c_str());
        }

        size_t remainingLength = longestOption - argumentLengths.at(key);
        for (size_t i = 0; i < remainingLength; i++)
            printf(" ");
        
        printf("  ");
        
        printf("%s\n", argumentDescriptions.at(key).c_str());
    }
}

void ArgumentManager::PrintUsage()
{
    printf("usage: kole");
    for (const auto& [key, value] : argumentIdentifiers)
    {
        printf(" [%s]", value[0].c_str());
    }
    printf("\n");
}

void ArgumentManager::PrintUnrecognizedArgument(std::string argument)
{
    printf("kole: error: unrecognized arguments: %s\n", argument.c_str());
}