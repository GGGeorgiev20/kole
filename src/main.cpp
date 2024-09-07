#include <iostream>
#include <cstdlib>

int main()
{
    std::shared_ptr<BuildConfig> config = ConfigReader::GetBuildConfig();
}