#pragma once

#include <iostream>
#include <map>
#include <variant>
#include <vector>
#include <string>

enum class Argument
{
    Help,
    Clear,
    Autorun,
    Debug,
    Config,
    CreateDirs,
};

class ArgumentManager
{
public:
    ArgumentManager(int argc, char** argv);

    /**
     * @brief Processes the command-line arguments.
     *
     * Loops through the provided arguments and updates the argument states based on matching identifiers.
     * Exits the program if unrecognized arguments are found or if help is requested.
     */
    void ProcessArguments();

    /**
     * @brief Prints the help menu and exits the program.
     */
    void PrintHelp();

    /**
     * @brief Prints the usage instructions.
     */
    void PrintUsage();

    /**
     * @brief Prints an error message for unrecognized arguments.
     *
     * @param argument: The unrecognized argument.
     */
    void PrintUnrecognizedArgument(std::string argument);

    /**
     * @brief Retrieves the collected arguments for autorun.
     *
     * @return The string of arguments meant for autorun, or an empty string if none were found.
     */
    std::string GetArgumentsForAutorun();

    /**
     * @brief Checks the boolean state of a specific argument.
     *
     * @param argument: The argument to check.
     * @return The boolean state of the argument.
     */
    bool GetArgumentState(Argument argument);

private:
    int argc;
    char** argv;

    const std::string description = "Build system for compiling and running C++ projects.";
    const Argument defaultHelpArgument = Argument::Help;

    std::string argumentsForAutorun = "";

    // Map of arguments and their possible identifiers (flags)
    const std::map<Argument, std::vector<std::string>> argumentIdentifiers = {
        { Argument::Help,             { "-h", "--help" } },
        { Argument::Clear,            { "--clear" }      },
        { Argument::Autorun,          { "--autorun" }    },
        { Argument::Debug,            { "--debug" }      },
        { Argument::Config,           { "--config" }     },
        { Argument::CreateDirs,       { "--createdirs" } },
    };

    // Map of arguments and their descriptions
    const std::map<Argument, std::string> argumentDescriptions = {
        { Argument::Help,             "Show this help message and exit"              },
        { Argument::Clear,            "Clear all object files before building"       },
        { Argument::Autorun,          "Autorun the compiled binary after building"   },
        { Argument::Debug,            "Show logs used for debugging"                 },
        { Argument::Config,           "Create a default config if one doesn't exist" },
        { Argument::CreateDirs,       "Creates all missing directories" },
    };

    // Map to track the state (whether the argument was provided or not)
    std::map<Argument, bool> argumentStates = {
        { Argument::Help,             false },
        { Argument::Clear,            false },
        { Argument::Autorun,          false },
        { Argument::Debug,            false },
        { Argument::Config,           false },
        { Argument::CreateDirs,       false },
    };
};
