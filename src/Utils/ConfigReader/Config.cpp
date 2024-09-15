#include "Utils/ConfigReader.hpp"

std::string ConfigReader::defaultConfig = R"(
    output: main
extension: auto

directories:
  src: [ src ]
  obj: obj
  bin: bin
  include: [ include ]

exclude: []

flags:
  common: none
  windows: none
  linux: none

compiler_version: g++
language_version: c++17

optimization: debug
)";
