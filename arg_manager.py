import argparse

class ArgManager:
    @staticmethod
    def parse_args():
        parser = argparse.ArgumentParser(description="Build script for compiling and running C++ projects.")
        parser.add_argument('--clear', action='store_true', help='Clear all object files before building.')
        parser.add_argument('--autorun', action='store_true', help='Run the compiled binary after building.')
        return parser.parse_args()