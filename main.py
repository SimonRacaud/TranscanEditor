import os

from src.model import CraftConfig
from src.charaterRecognition.CharacterDetector import CharacterDetector
import src.charaterRecognition.craft.file_utils as file_utils

if __name__ == '__main__':
    config = CraftConfig()

    if not os.path.isdir(config.result_folder):
        os.mkdir(config.result_folder)

    image_path_list, _, _ = file_utils.get_files(config.test_folder)

    detector = CharacterDetector()
    detector.process(config, image_path_list)
