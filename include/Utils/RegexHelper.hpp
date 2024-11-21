#pragma once

#include <regex>
#include <string>
#include <vector>
#include <filesystem>

#include "Utils/Logger/Logger.hpp"

namespace fs = std::filesystem;

namespace RegexHelper
{
    std::string EscapeRegexSpecialChars(const std::string& pattern);

    std::string ConvertPatternToRegex(const std::string& pattern);

    bool MatchesRegex(const std::string& string, const std::vector<std::string>& pattern);

    bool MatchesRegex(const fs::path& path, const std::vector<std::string>& pattern);
}