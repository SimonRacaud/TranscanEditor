
import os
import string
from typing import Sequence
import cv2
from numpy import ndarray

from src.model.model import OCRConfig

class FileManager:
    supported_format = ['.jpg', '.jpeg', '.png']

    @staticmethod
    def get_files(img_dir) -> Sequence[str]:
        """
            Get images file path in directory
        """
        img_files = []
        for (dirpath, dirnames, filenames) in os.walk(img_dir):
            for file in filenames:
                filename, ext = os.path.splitext(file)
                ext = str.lower(ext)
                if ext in FileManager.supported_format:
                    img_files.append(os.path.join(dirpath, file))
        img_files.sort()
        return img_files

    @staticmethod
    def setup(config: OCRConfig):
        """
            If it doesn't exist, create the destination directory
        """
        if not os.path.isdir(config.output_folder):
            os.mkdir(config.output_folder)

    @staticmethod
    def save_image(image: ndarray, src_image_path: string, output_folder: str):
        """
            Write image to file in the output folder
        """
        filename = os.path.basename(src_image_path)
        if output_folder[-1] != '/':
            res_img_file = output_folder + "/" + filename    
        else:
            res_img_file = output_folder + filename
        # Create output folder:
        if not os.path.isdir(output_folder):
            os.mkdir(output_folder)
        # Write image
        cv2.imwrite(res_img_file, image)
        print("Image save at {}".format(res_img_file))