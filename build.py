import os
import sys
import yaml
import platform
import subprocess

config_file = "config.yaml"

def load_config():
    with open(config_file, 'r') as file:
        try:
            data = yaml.safe_load(file)
            return data
        except yaml.YAMLError as e:
            print(f"FATAL: Couldn't read config: {e}")
            return None

def create_dirs():
    for key in config['directories']:
        dir = config['directories'][key]

        if key in config['ignore']:
            continue
        if dir == [] or dir == None or dir == "":
            continue

        dirs = dir if type(dir) == list else [ dir ]
        for dir in dirs:
            if not os.path.exists(dir):
                os.makedirs(dir)
        
def get_flag(flag):
    result = config['flags'][flag]
    
    if result != None:
        return result
    else:
        return ""

def get_flags():
    flags = f"{get_flag('error_flags')} "

    print(f"INFO: Operating system is {platform.system()}")
    if platform.system() == "Windows":
        print(f"INFO: Applying windows compiler flags")
        flags += get_flag('windows_lib_flags')
    else:
        print(f"INFO: Applying unix compiler flags")
        flags += get_flag('unix_lib_flags')

    if config['build_type'].lower() == "debug":
        flags += " -O0"
    elif config['build_type'].lower() == "release":
        print("INFO: Building in release mode")
        flags += " -O3"
    else:
        print("FATAL: Invalid build type in config. Exiting...")
        exit(1)

    return flags

def get_args():
    args = {
        "clear": False,
        "run": False
    }

    default_args = True

    for arg in sys.argv[1:]:
        if arg in args:
            args[arg] = True
            default_args = False
        else:
            print(f"WARNING: Invalid flag '{arg}'. Ignoring...")

    if default_args:
        print("INFO: No flags specified, using default flags")

    return args

def get_output_extension():
    if platform.system() == "Windows":
        return "exe"
    else:
        return "out"
    
def clear():
    print("INFO: Rebuilding all files...")

    for file in os.listdir(config['directories']['obj']):
        file_path = os.path.join(config['directories']['obj'], file)
        os.remove(file_path)

def run(output_path):
    print("BUILD: Running build...")

    command = output_path

    if platform.system() == "Windows":
        command = output_path.replace('/', '\\')
    else:
        command = f"./{output_path}"
    
    print()
    os.system(command)

def run_command(*args):
    try:
        return subprocess.check_output(" ".join(args), shell=True).strip().decode("utf-8")
    except Exception as e:
        print(f'FATAL: {e}')
        print("FATAL: Exiting...")
        exit(1)

def compile(dir, file):
    if (not file.endswith(".cpp")):
        return
    
    compiled_file = os.path.join(dir, file)
    object_file = os.path.join(config['directories']['obj'], file.split('.')[0]) + '.o'

    global compiled
    compiled.append(object_file)

    if os.path.exists(object_file) and os.path.getmtime(compiled_file) < os.path.getmtime(object_file):
        return
    
    run_command(f"{config['compiler_version']} {flags} -c {compiled_file} -o {object_file} -I{include_paths}")

    print(f'LOG: Compiled {compiled_file}')

def main():
    global config
    config = load_config()

    if not config:
        print("ERROR: No config file found. Exiting...")
        exit(1)
    
    create_dirs()

    global sys_args
    sys_args = get_args()

    global flags
    flags = get_flags()

    global compiled
    compiled = []

    if sys_args['clear']:
        clear()

    global include_paths
    include = config['directories']['include']
    include_paths = ' '.join(include) if type(include) == list else include

    dirs = config['directories']['src']
    source_dirs = dirs if type(dirs) == list else [ dirs ]
    for source_dir in source_dirs:
        for [dir, _, files] in os.walk(source_dir):
            for file in files:
                compile(dir, file)

    output_extension = get_output_extension()
    output_path = f"{config['directories']['bin']}/{config['output']}.{output_extension}"   

    main_command = f"{config['compiler_version']} -std={config['language_version']} {flags} -I{include_paths} -o {output_path} {' '.join(compiled)} {get_flag('end_flags')}"

    run_command(main_command)

    print("BUILD: Build successful")

    if sys_args['run']:
        run(output_path)

if __name__ == "__main__":
    main()