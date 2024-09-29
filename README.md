# Kole

Kole is a small cross-platform, open-source build system written in C++. It's made to be highly customizable by storing all build settings in the configuration file (`KoleConfig.yaml`). You can use it if you are compiling any C/C++ projects.

I want to note that this is a project is not indended for use on professional or large projects. I don't intend on updating it a lot or making it suitable for any type of projects. I have neither the time nor the will to do so.

What I intend to use this project for is, mainly, projects which share a similar structure to mine (this project acts as an example). If you share a similar structure for your projects and you like this program, I'd be glad if you use it.

If you run into issues or need something fixed, feel free to reach out, and I’ll help out when I can. You’re also welcome to contribute — just let me know :)

## Prerequisites

Before using Kole, make sure you have:

- GCC (GNU Compiler Collection)

## Installation

Kole currently only supports Linux as I am not booting Windows at the moment. While the config file does let you specify a target platform, the program doesn’t run on Windows at the moment (although it does compile). If you're on Windows, you can use another build system to compile it, and it should work. I plan to fix the issue and add support for other operating systems in the future tho.

### How to Install (Linux)

Since there’s no package available yet, you’ll need to install it manually. Luckily, it’s just a single binary:

1. Start by downloading the `kole` binary from the Release section.

2. Move the binary to your `/usr/local/bin` directory using the following command:

```bash
sudo mv kole /usr/local/bin
```

After that, you're ready to go!

## Usage

### Configuration

The configuration file (`KoleConfig.yaml`) is essential for specifying various settings required during the build process. For a detailed explanation of every field, please check the `docs` folder. For an example, check the config file of this project.

### Running the program

#### Starting a new project

To start up a new project, navigate to an empty directory and run:
```bash
kole --config --createdirs
```
This creates a default config file and sets up all necessary directories for your project.

#### Building an existing project

To build an already existing project, go to its' directory and follow these instructions:

1. Create a default config file using this command:
```bash
kole --config
```
2. Change the settings in it according to your projects' needs and dependencies
3. Run the command:
```bash
kole
```

This should hopefully compile the project and build a binary executable for you to run.

### Command-Line Arguments

- **`--help`**: Displays the help menu and exits the program.
- **`--clear`**: Deletes EVERYTHING in the `obj` directory. Useful when you need to rebuild all object files.
- **`--debug`**: Prints debug logs. Helps when the program doesn't compile and you're debugging it.
- **`--autorun`**: Runs the final executable after a successful build. All arguments after it are passed to the executable.
- **`--config`**: Creates a default config file, if one doesn't already exist and doesn't compile the project. Mainly used when starting a new project.
- **`--createdirs`**: Creates all necessary directories, if they doesn't already exist and doesn't compile the project. Mainly used when starting a new project.
