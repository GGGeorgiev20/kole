#include "Utils/RegexHelper.hpp"

std::string RegexHelper::EscapeRegexSpecialChars(const std::string& pattern)
{
    static const std::regex specialChars{ R"([-[\]{}()*+?.,\^$|#])" };
    return std::regex_replace(pattern, specialChars, R"(\$&)");
}

std::string RegexHelper::HandlePathSeparators(const std::string& pattern)
{
    std::string result;
    result.reserve(pattern.size());

    std::string backslashBuffer;

    for (std::size_t i = 0; i < pattern.size(); ++i)
    {
        const char& c = pattern[i];

        if (c == '\\')
        {
            backslashBuffer += '\\';
        }
        else
        {
            if (c == '/' || c == '\\')
            {
                if (backslashBuffer.length() % 2 == 0)
                {
                    result += backslashBuffer;
                    result += "[\\\\/]";
                }
                else
                {
                    result += backslashBuffer;
                    result += c;
                }
            }
            else
            {
                result += backslashBuffer;
                result += c;
            }

            backslashBuffer.clear();
        }
    }

    result += backslashBuffer;

    return result;
}

std::string RegexHelper::ConvertPatternToRegex(const std::string& pattern)
{
    std::string escapedPattern = EscapeRegexSpecialChars(pattern);

    // Replace escaped wildcard `\*` with regex equivalent `.*`
    escapedPattern = std::regex_replace(escapedPattern, std::regex(R"(\\\*)"), R"(.*)");

    // Replace `/` or `\` with `[\\/]` to handle platform independence
    escapedPattern = HandlePathSeparators(escapedPattern);

    // Add the already converted pattern to the map containing all converted patterns
    convertedPatterns.insert({ pattern, escapedPattern });

    return escapedPattern;
}

bool RegexHelper::MatchesRegex(const std::string& string, const std::vector<std::string>& pattern)
{
    for (const auto& excludePattern : pattern)
    {
        try
        {
            bool patternConverted = convertedPatterns.find(excludePattern) != convertedPatterns.end();

            std::string regexPattern = patternConverted
                ? convertedPatterns.at(excludePattern)
                : ConvertPatternToRegex(excludePattern);

            std::regex patternRegex(regexPattern, std::regex_constants::ECMAScript | std::regex_constants::icase);

            if (std::regex_match(string, patternRegex)) return true;
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
