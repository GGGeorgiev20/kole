import os
import platform

class BuildRunner:
    def __init__(self, config, compiler):
        self.config = config
        self.compiler = compiler

    def build_project(self, end_flags):
        output_extension = self.compiler.get_output_extension()
        output_path = f"{self.config['directories']['bin']}/{self.config['output']}.{output_extension}"

        main_command = f"{self.config['compiler_version']} -std={self.config['language_version']} {self.compiler.flags} {'-I' if self.compiler.include_paths else ''}{self.compiler.include_paths} -o {output_path} {' '.join(self.compiler.compiled)} {end_flags}"

        self.compiler.run_command(main_command)

        print("BUILD: Build successful")
        return output_path

    def run_build(self, output_path):
        print("BUILD: Running build...")

        command = output_path.replace('/', '\\') if platform.system() == "Windows" else f"./{output_path}"
        os.system(command)
