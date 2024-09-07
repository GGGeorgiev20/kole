import yaml
import copy

class ConfigLoader:
    @staticmethod
    def load_config(config_file='config.yaml'):
        """Load config file content and process it. Default is config.yaml."""
        with open(config_file, 'r') as file:
            try:
                content = yaml.safe_load(file)
                processed = ConfigLoader.process_config(content)
                return processed
            except yaml.YAMLError as e:
                print(f"FATAL: Couldn't read config: {e}")
                return None

    @staticmethod
    def get_all_keys(d, parent_key=''):
        """Recursively retrieve all keys, including nested ones."""
        keys = []

        for k, v in d.items():
            full_key = f'{parent_key}/{k}' if parent_key else k

            if isinstance(v, dict):
                # Recurse into the dictionary
                keys.extend(ConfigLoader.get_all_keys(v, full_key))
            else:
                keys.append(full_key)

        return keys
    
    @staticmethod
    def process_config(content):
        """Process the content of the config by replacing empty property values and raising errors for non-empty properties."""
        ACCEPTABLE_EMPTY = [
            'ignore',
            'flags/error_flags',
            'flags/windows_lib_flags',
            'flags/unix_lib_flags',
            'flags/end_flags',
            'directories/include',
        ]

        EMPTY_VALUES = [ '', 'None', '-', '[]' ]

        content_copy = copy.deepcopy(content)
        keys = ConfigLoader.get_all_keys(content_copy)

        for key in keys:
            key_parts = key.split('/')

            value = content_copy
            for part in key_parts:
                value = value.get(part, '')

            if value in EMPTY_VALUES:
                if key in ACCEPTABLE_EMPTY:
                    # Reset empty values for acceptable keys
                    ref = content_copy

                    for part in key_parts[:-1]:
                        ref = ref[part]

                    ref[key_parts[-1]] = ''
                else:
                    print(f"FATAL: Key '{key}' is not allowed to be empty")
                    exit(1)

        return content_copy
