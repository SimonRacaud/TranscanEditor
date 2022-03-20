from typing import Sequence
import pytesseract
import numpy as np
import cv2

from src.model import TextBlock
from src.utility.extract_image_area import extract_image_area

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

def get_optimal_text_position(text_height, box_size, pivot):
    pos_x = pivot[0]
    pos_y = pivot[1] + box_size[1] - round((box_size[1] - text_height) / 2)
    return (pos_x, pos_y)

def character_extraction(image, bboxes) -> Sequence[TextBlock]:
    result = []

    for box in enumerate(bboxes):
        poly = np.array(box[1]).astype(np.int32).reshape((-1))
        poly = poly.reshape(-1, 2)

        area, angle, pivot, size = extract_image_area(poly, image)
        if not area.size:
            continue
        gray = cv2.cvtColor(area, cv2.COLOR_BGR2GRAY)
        text = pytesseract.image_to_string(gray, lang='eng', config='--psm 6 --oem 1')
        text = filter_non_printable(text)

        font_scale, text_height = get_optimal_font_scale(text, size[0])
        position = get_optimal_text_position(text_height, size, pivot)

        result.append(TextBlock(text, position, font_scale, box, pivot, angle, size))
    return result

def text_insert(textBlockList: Sequence[TextBlock], image):
    for block in textBlockList:
        temporary = np.zeros(image.shape, dtype="uint8")
        if block.color == (0, 0, 0):
            temporary = cv2.bitwise_not(temporary)
        cv2.putText(temporary, "{}".format(block.str), block.position, 
            block.font, block.font_scale, block.color, thickness=block.thickness)
        # Rotate
        if block.angle != 0:
            rotate_matrix = cv2.getRotationMatrix2D(center=block.pivot, angle=-block.angle, scale=1)
            shape = temporary.shape[:2]
            temporary = cv2.warpAffine(src=temporary, M=rotate_matrix, dsize=(shape[1], shape[0]))
        #
        mask = cv2.inRange(temporary, block.color, block.color)
        mask = cv2.bitwise_not(mask)
        image = cv2.bitwise_or(image, image, mask=mask)
        if block.color != (0, 0, 0):
            image = cv2.bitwise_or(temporary, image)
    return image
        