
import os
import string
from typing import Sequence
import cv2
import numpy as np
from PIL import Image
import io

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
    def load_image(image_path: str) -> np.ndarray:
        with open(image_path, 'rb') as img_file:
            ## Read binary
            img_bytes = img_file.read()
            ## Format and convert to numpy array
            img = Image.open(io.BytesIO(img_bytes))
            image = np.asarray(img)
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
            return image

    @staticmethod
    def save_image(image: np.ndarray, filename: string, output_folder: str) -> str:
        """
            Write image to file in the output folder
        """
        if not output_folder or len(output_folder) == 0:
            raise RuntimeError()
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
        return res_img_file
    
    @staticmethod
    def filename_from_filepath(filepath: str) -> str:
        return os.path.basename(filepath)
    
    @staticmethod
    def path_from_filepath(filepath: str) -> str:
        if filepath[-1] != '/':
            filepath += '/'
        return os.path.dirname(filepath)
    
    @staticmethod
    def add_file_suffix(filepath: str, suffix: str) -> str:
        filename, extension = filepath.rsplit('.', 1)
        return filename + suffix + "." + extension