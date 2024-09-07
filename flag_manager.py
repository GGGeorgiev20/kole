import platform

class FlagManager:
    def __init__(self, config):
        self.config = config

    def get_flag(self, flag):
        result = self.config['flags'][flag]
        return result if result is not None else ''

    def get_flags(self):
        flags = f"{self.get_flag('error_flags')} "

        print(f"INFO: Operating system is {platform.system()}")
        if platform.system() == "Windows":
            print(f"INFO: Applying windows compiler flags")
            flags += self.get_flag('windows_lib_flags')
        else:
            print(f"INFO: Applying unix compiler flags")
            flags += self.get_flag('unix_lib_flags')

        build_type = self.config['build_type'].lower()
        if build_type == "debug":
            flags += " -O0"
        elif build_type == "release":
            print("INFO: Building in release mode")
            flags += " -O3"
        else:
            print("FATAL: Invalid build type in config. Exiting...")
            exit(1)

        return flags
