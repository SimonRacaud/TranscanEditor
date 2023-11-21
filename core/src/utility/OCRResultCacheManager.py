import json
import os
import hashlib

from src.model.model import OCRService

class OCRResultCacheManager:
    @staticmethod
    def load_result(image, cache_path: str, service: OCRService):
        try:
            if not os.path.isdir(cache_path):
                os.mkdir(cache_path)
            file_hash = hashlib.md5(image).hexdigest()
            src_filepath = OCRResultCacheManager.__get_file_path(file_hash, service, cache_path)
            with open(src_filepath, 'r') as file:
                data = json.load(file)
                return data
        except:
            raise FileNotFoundError()

    @staticmethod
    def save_result(image, data, cache_path: str, service: OCRService):
        try:
            if not os.path.isdir(cache_path):
                os.mkdir(cache_path)
            file_hash = hashlib.md5(image).hexdigest()

            dest_filepath = OCRResultCacheManager.__get_file_path(file_hash, service, cache_path)
            json_object = json.dumps(data, indent = 4)
            with open(dest_filepath, 'w') as file:
                file.write(json_object)
            print("Info: OCR result saved in cache.")
        except BaseException as err:
            print("Warning: Fail to save ocr result in cache.", err)

    @staticmethod
    def __get_file_path(file_hash: str, service: OCRService, cache_path: str):
        return cache_path + '/' + str(service) + '_' + file_hash + '.json'
