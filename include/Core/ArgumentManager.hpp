#pragma once

#include <iostream>
#include <map>
#include <variant>
#include <string>
#include <array>

enum class Argument
{
    Help,
    Rebuild,
    Autorun,
    Debug,
    Config,
    Initialize,
};

struct ArgumentInfo
{
    std::string shortName;
    std::string longName;
    std::string description;
};

class ArgumentManager
{
public:
    ArgumentManager(int argc, char** argv);

    /**
     * @brief Processes the command-line arguments.
     *
     * Loops through the provided arguments and updates the argument states based on matching identifiers.
     * Exits the program if unrecognized arguments are found or the help paramter is passed.
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
     * @param argument The unrecognized argument.
     */
    void PrintUnrecognizedArgument(const std::string& argument);

    /**
     * @brief Prints usage, unrecognized argument and exits.
     *
     * @param argument The unrecognized argument.
     */
    void ArgumentNotFound(const std::string& argument);

    /**
     * @brief Retrieves the collected arguments for autorun.
     *
     * @return The string of arguments meant for autorun, or an empty string if none were found.
     */
    std::string GetAutorunArguments();

    /**
     * @brief Checks the boolean state of a specific argument.
     *
     * @param argument The argument to check.
     * @return The boolean state of the argument.
     */
    bool GetArgumentState(const Argument& argument);

private:
    int m_argc;
    char** m_argv;

    const std::string m_description = "Build system for compiling and running C++ projects.";

    std::string m_autorunArguments = "";

    // NOTE: I tried making argumentIdentifiers and argumentDescriptions to be inline static constexpr and replace std::string with const char*
    // because that would make them get evaluated at compile time which is never bad and actually faster
    // but because of its absance of methods, it's not worth enough to use it, so I just reverted to std::string

    // Map of arguments and their possible identifiers (flags)
    const std::map<Argument, std::array<std::string, 2>> m_argumentIdentifiers = {
        { Argument::Help,              { "h", "help" }    },
        { Argument::Rebuild,           { "r", "rebuild" } },
        { Argument::Autorun,           { "a", "autorun" } },
        { Argument::Debug,             { "d", "debug" }   },
        { Argument::Config,            { "c", "config" }  },
        { Argument::Initialize,        { "i", "init" }    },
    };

    // Map of arguments and their descriptions
    const std::map<Argument, std::string> m_argumentDescriptions = {
        { Argument::Help,              "Display this help message and exit"    },
        { Argument::Rebuild,           "Rebuild all object files"              },
        { Argument::Autorun,           "Run the compiled binary automatically" },
        { Argument::Debug,             "Enable debugging logs"                 },
        { Argument::Config,            "Generate a default config if missing"  },
        { Argument::Initialize,        "Sets up an empty project"              },
    };

    // Map to track the state (whether the argument was provided or not)
    std::map<Argument, bool> m_argumentStates = {
        { Argument::Help,              false },
        { Argument::Rebuild,           false },
        { Argument::Autorun,           false },
        { Argument::Debug,             false },
        { Argument::Config,            false },
        { Argument::Initialize,        false },
    };
};
