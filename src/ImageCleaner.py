from typing import Sequence
import cv2
import numpy as np

from src.model import OCRBlock
from src.utility.show_debug import show_debug

class ImageCleaner:
    @classmethod
    def process(cls, image, textBlockList: Sequence[OCRBlock]):
        mask = np.zeros(image.shape[:2], dtype="uint8")
        # Build suppression mask
        for block in textBlockList:
            if block.text == "":
                continue
            poly = block.polygon
            cv2.fillPoly(mask, [poly], color=(255, 255, 255))
        # Apply mask on image
        return cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)