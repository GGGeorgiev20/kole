from config_loader import ConfigLoader
from dir_manager import DirManager
from arg_manager import ArgManager
from flag_manager import FlagManager
from compiler import Compiler
from build_runner import BuildRunner

import os

def main():
    config = ConfigLoader.load_config()

    if not config:
        print("ERROR: No config file found. Exiting...")
        exit(1)

    args = ArgManager.parse_args()
    
    flag_manager = FlagManager(config)
    flags = flag_manager.get_flags()

    dir_manager = DirManager(config)
    dir_manager.create_dirs()

    if args.clear:
        dir_manager.clear()

    include_paths = config['directories']['include']
    include_paths = ' '.join(include_paths) if isinstance(include_paths, list) else include_paths

    compiler = Compiler(config, flags, include_paths)

    dirs = config['directories']['src']
    source_dirs = dirs if isinstance(dirs, list) else [dirs]

    for source_dir in source_dirs:
        for dir, _, files in os.walk(source_dir):
            for file in files:
                compiler.compile_file(dir, file)

    if compiler.compiled == []:
        print('ERROR: No files were compiled. Aborting linking phase...')
        exit(1)

    build_runner = BuildRunner(config, compiler)
    output_path = build_runner.build_project(flag_manager.get_flag('end_flags'))

    if args.autorun:
        build_runner.run_build(output_path)

if __name__ == "__main__":
    main()
