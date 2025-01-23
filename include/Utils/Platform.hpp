#pragma once

#include <iostream>
#include <string>
#include <map>

namespace Platform
{
    enum Platforms
    {
        WINDOWS,
        LINUX,
        MACOS,
        UNIX,
        UNKNOWN,
    };

    /**
     * @brief Set the platform based on the user's configuration.
     *
     * This function sets the platform cache according to the specified platform
     * in the config. If the platform is unrecognized, it defaults to the user's
     * detected operating system.
     *
     * @param platformName The platform specified in the config (e.g., "Windows").
     */
    void SetPlatform(std::string platformName);

    /**
     * @brief Get the platform of the user's operating system.
     *
     * Uses compile-time macros to detect the user's OS and caches the result.
     * If the platform isn't recognized, returns UNKNOWN.
     *
     * @return The platform identifier (e.g., WINDOWS, LINUX).
     */
    int GetPlatform();

    /**
     * @brief Get the appropriate file extension for the platform.
     */
    std::string GetOutputExtension();

    /**
     * @brief Get the name of the detected platform.
     */
    std::string GetPlatformName();

    // NAMESPACE VARIABLES

    extern int savedPlatform;
    extern std::map<int, std::string> platformNames;
};
