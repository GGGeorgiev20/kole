#include <iostream>
#include "Utils/ConfigReader/ConfigReader.hpp"

int main()
{
    std::shared_ptr<BuildConfig> config = ConfigReader::GetBuildConfig();
}