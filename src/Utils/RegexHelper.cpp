#include "Utils/RegexHelper.hpp"

std::string RegexHelper::EscapeRegexSpecialChars(const std::string& pattern)
{
    static const std::regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s])" };
    return std::regex_replace(pattern, specialChars, R"(\$&)");
}

std::string RegexHelper::ConvertPatternToRegex(const std::string& pattern)
{
    std::string escapedPattern = EscapeRegexSpecialChars(pattern);

    // Replace escaped wildcard `\*` with regex equivalent `.*`
    escapedPattern = std::regex_replace(escapedPattern, std::regex(R"(\\\*)"), R"(.*)");

    // Handle path separators to be platform-independent
    escapedPattern = std::regex_replace(escapedPattern, std::regex(R"(\\|/)"), R"([\\/])");

    return escapedPattern;
}

bool RegexHelper::MatchesRegex(const std::string& string, const std::vector<std::string>& pattern)
{
    for (const auto& excludePattern : pattern)
    {
        try
        {
            bool patternConverted = convertedPatterns.find(excludePattern) != convertedPatterns.end();
            std::cout << "Pattern already converted: " << patternConverted << std::endl;

            std::string regexPattern = patternConverted ? convertedPatterns.at(excludePattern) : ConvertPatternToRegex(excludePattern);

            std::regex patternRegex(regexPattern, std::regex_constants::ECMAScript | std::regex_constants::icase);

            if (std::regex_match(string, patternRegex))
            {
                Logger::Debug(fmt::format("Excluding '{}' due to pattern '{}'", string, excludePattern));
                return true;
            }
        }
        catch (const std::regex_error& e)
        {
            Logger::Error(fmt::format("Invalid exclude pattern '{}': {}", excludePattern, e.what()));
        }
    }

    return false;
}

bool RegexHelper::MatchesRegex(const fs::path& path, const std::vector<std::string>& pattern)
{
    std::string pathStr = path.generic_string();
    return MatchesRegex(pathStr, pattern);
}

// NAMESPACE VARIABLES

std::unordered_map<std::string, std::string> RegexHelper::convertedPatterns;
