# Kole Configuration Fields

This document explains all the available configuration options in `KoleConfig.yaml` and what each one does. You can customize these settings to fit your project's needs.

### General Notes:
- Properties can be set to `'none'`, which makes their value empty.
- Both `extension` and `platform` can be set to `'auto'`, which will automatically determine the correct value for your operating system.
- `output`, `platform`, and `compiler` cannot be empty.
- Only `gcc` and `g++` are supported for now.

## General Settings

### `output`
- **Type**: `string`
- **Default**: `"main"`
- **Description**: The name of the output binary file (without extension). For example, setting this to `"app"` will result in a binary named `app`.

### `extension`
- **Type**: `string`
- **Default**: `"auto"`
- **Description**: The file extension for the output binary. Set this to `"auto"` to automatically choose the appropriate extension based on the platform (`.exe` for Windows, `.out` for Linux). You can also specify a custom extension if needed.

### `platform`
- **Type**: `string`
- **Default**: `"auto"`
- **Description**: The platform for which the program will be built. Set to `"auto"` to automatically detect the current platform. Valid options are `"windows"`, `"linux"`, `"macos"`, and `"unix"`.

## Directories

### `directories`
- **Type**: `map<string, vector<string>>`
- **Description**: Defines key directories used during the build process. Every directory can be a list, and you can include multiple directories. However, for `obj` and `bin`, only the first value in the list will be used.
  - **`src`**: Source files directory. Typically where your `.cpp` or `.c` files are located.
  - **`ui`**: Directory for Qt UI files, if you’re using Qt.
  - **`obj`**: Directory for object files created during compilation. Only the first directory in the list is used.
  - **`bin`**: Directory for the final binary output. Only the first directory in the list is used.
  - **`include`**: Directory for header files.

## Compiler and Flags

### `flags`
- **Type**: `map<string, string>`
- **Description**: Compiler flags used for different platforms.
  - **`common`**: Flags applied to all platforms.
  - **`windows`**: Flags applied when building for Windows.
  - **`linux`**: Flags applied when building for Linux.
  - **`macos`**: Flags applied when building for macOS.
  - **`unix`**: Flags applied when building for Unix-like systems.

## Qt Support

### `qtSupport`
- **Type**: `map<string, string>`
- **Description**: Configuration options for Qt projects. If you're not using Qt, you can ignore these settings.
  - **`compile_ui`**: Set to `"true"` to compile `.ui` files into header files.
  - **`compile_moc`**: Set to `"true"` to compile Qt MOC files.
  - **`ui_prefix`**: Prefix for generated UI header files. For example, setting this to `"ui_"` will generate files like `ui_mainwindow.h`.
  - **`ui_extension`**: File extension for the generated UI header files (e.g., `.hpp` or `.h`).
  - **`ui_output_dir`**: Directory to store compiled UI header files.
  - **`moc_prefix`**: Prefix for generated MOC files.

## Compiler and Language Versions

### `compiler`
- **Type**: `string`
- **Default**: `"g++"`
- **Description**: Specifies the compiler to use. Kole currently supports only `gcc` and `g++`.

### `languageVersion`
- **Type**: `string`
- **Default**: `"c++17"`
- **Description**: Specifies the C++ language version to use. You can change this to any version supported by your compiler, such as `"c++14"`, `"c++20"`, etc.

## Optimization

### `optimization`
- **Type**: `string`
- **Default**: `"debug"`
- **Description**: Specifies the optimization level. The following options are available:
  - `"none"`: No optimization (`-O0`).
  - `"opt1"`: Optimization Level 1 (`-O1`).
  - `"opt2"`: Optimization Level 2 (`-O2`).
  - `"release"`: Full optimization for release builds (`-O3`).
  - `"fast"`: Aggressive optimization (`-Ofast`).
  - `"debug"`: Optimization for debugging (`-Og`).
  - `"size"`: Optimization for reducing binary size (`-Os`).
