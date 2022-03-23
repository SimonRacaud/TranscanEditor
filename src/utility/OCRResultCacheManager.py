import json
import os
import hashlib

class OCRResultCacheManager:
    @staticmethod
    def load_result(image, cache_path: str):
        try:
            if not os.path.isdir(cache_path):
                os.mkdir(cache_path)
            file_hash = hashlib.md5(image).hexdigest()
            src_filepath = OCRResultCacheManager.__get_file_path(file_hash, cache_path)
            with open(src_filepath, 'r') as file:
                data = json.load(file)
                return data
        except:
            raise FileNotFoundError()

    @staticmethod
    def save_result(image, data, cache_path: str):
        try:
            if not os.path.isdir(cache_path):
                os.mkdir(cache_path)
            file_hash = hashlib.md5(image).hexdigest()
            dest_filepath = OCRResultCacheManager.__get_file_path(file_hash, cache_path)
            json_object = json.dumps(data, indent = 4)
            with open(dest_filepath, 'w') as file:
                file.write(json_object)
        except:
            raise RuntimeError()

    @staticmethod
    def __get_file_path(file_hash, cache_path):
        return cache_path + '/' + file_hash + '.json'
