from dataclasses import replace
from typing import Sequence, Text
import pytesseract
import numpy as np
import cv2

from model import TextBlock

def get_optimal_font_scale(text, width):
    height = 1
    for scale in reversed(np.arange(0, 60, 0.5)):
        textSize = cv2.getTextSize(text, fontFace=cv2.FONT_HERSHEY_DUPLEX, fontScale=scale/10, thickness=1)
        if scale == 1:
            height = textSize[0][1]
        new_width = textSize[0][0]
        if (new_width <= width):
            return scale/10, textSize[0][1]
    return 1, height

def filter_non_printable(str):
    filtered_characters = list(s for s in str if s.isprintable())
    return ''.join(filtered_characters)

def get_optimal_text_position(box, text_height):
    pos_x = box[3][0]
    box_height = (box[2][1] - box[0][1])
    pos_y = box[3][1] - round((box_height - text_height) / 2)
    return (pos_x, pos_y)

def character_extraction(image, bboxes) -> Sequence[TextBlock]:
    result = []

    for i, box in enumerate(bboxes):
        poly = np.array(box).astype(np.int32).reshape((-1))
        poly = poly.reshape(-1, 2)
        # 
        area = image[poly[0][1]:poly[2][1], poly[0][0]:poly[2][0]]
        gray = cv2.cvtColor(area, cv2.COLOR_BGR2GRAY)
        #
        text = pytesseract.image_to_string(gray, lang='eng', config='--psm 6 --oem 1')
        text = filter_non_printable(text)
        #
        width = (poly[2][0] - poly[0][0])
        font_scale, text_height = get_optimal_font_scale(text, width)
        position = get_optimal_text_position(poly, text_height)

        result.append(TextBlock(text, position, font_scale, box))
    return result

def text_insert(textBlockList: Sequence[TextBlock], image):
    for block in textBlockList:
        cv2.putText(image, "{}".format(block.str), block.position, 
            block.font, block.font_scale, block.color, thickness=block.thickness)
    return image
        