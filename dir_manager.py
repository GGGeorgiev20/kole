import os

class DirManager:
    def __init__(self, config):
        self.config = config

    def create_dirs(self):
        for key in self.config['directories']:
            dir = self.config['directories'][key]

            if key in self.config['ignore']:
                continue

            dirs = dir if isinstance(dir, list) else [dir]          
            for dir in dirs:
                if not os.path.exists(dir):
                    os.makedirs(dir)

    def clear(self):
        print("INFO: Rebuilding all files...")

        try:
            for file in os.listdir(self.config['directories']['obj']):
                file_path = os.path.join(self.config['directories']['obj'], file)
                os.remove(file_path)
        except:
            print("ERROR: The 'obj' directory is missing. It may have been ignored or something has gone wrong.")
