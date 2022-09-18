
import os
import string
import cv2
from numpy import ndarray

from src.model import AppConfig

class FileManager:
    @staticmethod
    def get_files(img_dir):
        img_files = []
        for (dirpath, dirnames, filenames) in os.walk(img_dir):
            for file in filenames:
                filename, ext = os.path.splitext(file)
                ext = str.lower(ext)
                if ext == '.jpg' or ext == '.jpeg' or ext == '.gif' or ext == '.png' or ext == '.pgm':
                    img_files.append(os.path.join(dirpath, file))
        img_files.sort()
        return img_files

    @staticmethod
    def setup(config: AppConfig):
        if not os.path.isdir(config.output_folder):
            os.mkdir(config.output_folder)

    @staticmethod
    def save_image(image: ndarray, image_path: string, config: AppConfig):
        filename = os.path.basename(image_path)
        res_img_file = config.output_folder + filename
        cv2.imwrite(res_img_file, image)