import string
from typing import Sequence
import pytesseract
import numpy as np
import cv2

from src.model.model import OCRBlock
from src.utility.extract_image_area import extract_image_area

class TextExtractor:
    @classmethod
    def character_extraction(cls, image: np.ndarray, bboxes) -> Sequence[OCRBlock]:
        result = []

        for poly in bboxes:
            # Extract text area
            area, angle, pivot, size = extract_image_area(poly, image, True)
            if not area.size:
                continue
            # Extract text
            gray = cv2.cvtColor(area, cv2.COLOR_BGR2GRAY)
            text: string = pytesseract.image_to_string(gray, lang='eng', config='--psm 9') # or 6?
            text = cls.__filter_non_printable(text)
            text = text.encode('latin1', "replace").decode('latin1')
            
            # Save data
            result.append(OCRBlock(poly, text, pivot, size, angle))
        return result

    @staticmethod
    def __filter_non_printable(str: string):
        """ remove non printable characters """
        filtered_characters = list(s for s in str if s.isprintable())
        return ''.join(filtered_characters)