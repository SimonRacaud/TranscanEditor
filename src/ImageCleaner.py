from typing import Sequence
import cv2
import numpy as np

from src.model import TextBlock
from src.utility.show_debug import show_debug

class ImageCleaner:
    @classmethod
    def process(cls, image, textBlockList: Sequence[TextBlock]):
        mask = np.zeros(image.shape[:2], dtype="uint8")
        # Build suppression mask
        for block in textBlockList:
            if block.str == "":
                continue
            box = block.bbox
            poly = np.array(box[1]).astype(np.int32)
            cv2.fillPoly(mask, [poly.reshape((-1, 1, 2))], color=(255, 255, 255))

            #image = cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)
            #show_debug(image)
        # Apply mask on image
        return cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)