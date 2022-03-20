from typing import Sequence
import cv2

import numpy as np
from src.model import TextBlock

class TextEditor:
    @classmethod
    def process(cls, textBlockList: Sequence[TextBlock], image: np.ndarray) -> np.ndarray:
        for block in textBlockList:
            # Create text segment
            segment = cls.__create_text_segment(block, image)
            # Rotate segment
            if block.angle != 0:
                segment = cls.__rotate_segment(block, segment)
            # Apply segment on image
            image = cls.__apply_segment_on_image(segment, block, image)
        return image

    @staticmethod
    def __create_text_segment(block: TextBlock, image):
        segment = np.zeros(image.shape, dtype="uint8")
        if block.color == (0, 0, 0):
            segment = cv2.bitwise_not(segment)
        cv2.putText(segment, "{}".format(block.str), block.position, 
            block.font, block.font_scale, block.color, thickness=block.thickness)
        return segment
    
    @staticmethod
    def __rotate_segment(block: TextBlock, segment):
        rotate_matrix = cv2.getRotationMatrix2D(center=block.pivot, angle=-block.angle, scale=1)
        shape = segment.shape[:2]
        return cv2.warpAffine(src=segment, M=rotate_matrix, dsize=(shape[1], shape[0]), borderValue=(255,255,255))
    
    @staticmethod
    def __apply_segment_on_image(segment, block: TextBlock, image):
        mask = cv2.inRange(segment, block.color, block.color)
        mask = cv2.bitwise_not(mask)
        image = cv2.bitwise_or(image, image, mask=mask)
        if block.color != (0, 0, 0):
            image = cv2.bitwise_or(segment, image)
        return image