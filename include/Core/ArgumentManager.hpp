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
    Config
};

class ArgumentManager
{
public:
    ArgumentManager(int argc, char** argv);

    void ProcessArguments();

    void PrintHelp();

    void PrintUsage();

    void PrintUnrecognizedArgument(std::string argument);

    std::string GetArgumentForAutorun();

    bool GetArgumentState(Argument argument);

private:
    int argc;
    char** argv;

    const std::string description = "Build system for compiling and running C++ projects.";
    const Argument defaultHelpArgument = Argument::Help;

    std::string argumentsForAutorun = "";

    // Map of arguments and their possible identifiers (flags)
    const std::map<Argument, std::vector<std::string>> argumentIdentifiers = {
        { Argument::Help,         { "-h", "--help" } },
        { Argument::Clear,        { "--clear" }      },
        { Argument::Autorun,      { "--autorun" }    },
        { Argument::Debug,        { "--debug" }      },
        { Argument::Config,       { "--config" }     },
    };

    // Map of arguments and their descriptions
    const std::map<Argument, std::string> argumentDescriptions = {
        { Argument::Help,         "Show this help message and exit"              },
        { Argument::Clear,        "Clear all object files before building"       },
        { Argument::Autorun,      "Autorun the compiled binary after building"   },
        { Argument::Debug,        "Show logs used for debugging"                 },
        { Argument::Config,       "Create a default config if one doesn't exist" },
    };

    // Map to track the state (whether the argument was provided or not)
    std::map<Argument, bool> argumentStates = {
        { Argument::Help,         false },
        { Argument::Clear,        false },
        { Argument::Autorun,      false },
        { Argument::Debug,        false },
        { Argument::Config,       false },
    };
};
