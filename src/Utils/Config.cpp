#include "Core/ConfigReader.hpp"

std::string ConfigReader::defaultConfig = R"(output: main
extension: auto

directories:
  src: [ src ]
  obj: obj
  bin: bin
  include: [ include ]

flags:
  common: none
  windows: none
  linux: none

language_version: c++17

optimization: debug
)";
