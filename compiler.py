import os
import platform
import subprocess

class Compiler:
    def __init__(self, config, flags, include_paths):
        self.config = config
        self.flags = flags
        self.include_paths = include_paths
        self.compiled = []

    def compile_file(self, dir, file):
        if not file.endswith(".cpp"):
            return

        compiled_file = os.path.join(dir, file)
        object_file = os.path.join(self.config['directories']['obj'], file.split('.')[0]) + '.o'

        self.compiled.append(object_file)

        if os.path.exists(object_file) and os.path.getmtime(compiled_file) < os.path.getmtime(object_file):
            return

        self.run_command(f"{self.config['compiler_version']} {self.flags} -c {compiled_file} -o {object_file} {'-I' if len(self.include_paths) > 0 else ''}{self.include_paths}")

        print(f'LOG: Compiled {compiled_file}')

    def run_command(self, *args):
        try:
            return subprocess.check_output(" ".join(args), shell=True).strip().decode("utf-8")
        except Exception as e:
            print(f'FATAL: {e}')
            print("FATAL: Exiting...")
            exit(1)

    def get_output_extension(self):
        return "exe" if platform.system() == 'Windows' else "out"
