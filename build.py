import os
import sys
import platform
import subprocess

config_file = "config.yaml"

def check_imports():
    try:
        import yaml
    except ImportError:
        print("WARNING: PyYAML not found. Installing...")
        subprocess.check_call(['pip', 'install', 'PyYAML'])

def load_config():
    check_imports()

    import yaml

    with open(config_file, 'r') as file:
        try:
            data = yaml.safe_load(file)
            return data
        except yaml.YAMLError as e:
            print(f"ERROR: Couldn't read config: {e}")
            return None

def create_dirs():
    for key in config['directories']:
        dir = config['directories'][key]

        if dir == [] or dir == "":
            continue

        if not key in config['ignore'] and not os.path.exists(dir):
            os.makedirs(dir)

def get_flags():
    flags = f"{config['flags']['error_flags']} "

    if platform.system() == "Windows":
        flags += config['flags']['windows_lib_flags']
    else:
        flags += config['flags']['unix_lib_flags']

    if config['build_type'] == "debug":
        flags += " -O0"
    elif config['build_type'] == "release":
        print("INFO: Building in release mode")
        flags += " -O3"
    else:
        print("ERROR: Invalid build type in config. Exiting...")
        exit(1)

    return flags

def get_args():
    args = {
        "clear": False
    }

    default_args = False

    for arg in sys.argv[1:]:
        if arg in args:
            args[arg] = True
            default_args = True
        else:
            print(f"WARNING: Invalid flag '{arg}'. Ignoring...")

    if default_args:
        print("INFO: No flags specified, using default flags")
    elif args['clear']:
        print("INFO: Rebuilding all files...")

    return args

def get_output_extension():
    if platform.system() == "Windows":
        return "exe"
    else:
        return "out"
    
def clear():
    for file in os.listdir(config['directories']['obj']):
        file_path = os.path.join(config['directories']['obj'], file)
        os.remove(file_path)

def run_command(*args):
    try:
        return subprocess.check_output(" ".join(args), shell=True).strip().decode("utf-8")
    except Exception as e:
        print(f'ERROR: Failed to run command:')
        print(f'ERROR: {e}')
        print("ERROR: Exiting...")
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
    
    run_command(f"{config['compiler_version']} {flags} -c {compiled_file} -o {object_file}")

    post_index = len(config['directories']['src']) + 1
    print(f'BUILD: Compiled {compiled_file[post_index:]}')

def main():
    global config
    config = load_config()

    if not config:
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

    for [dir, _, files] in os.walk(config['directories']['src']):
        for file in files:
            compile(dir, file)

    output_extension = get_output_extension()
    output_path = f"{config['directories']['bin']}/{config['output']}.{output_extension}"

    run_command(f"{config['compiler_version']} -std={config['language_version']} {flags} -o {output_path} {' '.join(compiled)} {config['flags']['end_flags']}")

    print("INFO: Build successful")

if __name__ == "__main__":
    main()