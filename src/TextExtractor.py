from typing import Sequence
import pytesseract
import numpy as np
import cv2

from src.model import TextBlock
from src.utility.extract_image_area import extract_image_area
from src.utility.show_debug import show_debug

class TextExtractor:
    @classmethod
    def character_extraction(cls, image: np.ndarray, bboxes) -> Sequence[TextBlock]:
        result = []

        for box in enumerate(bboxes):
            # Extract text area
            poly = np.array(box[1]).astype(np.int32).reshape((-1))
            poly = poly.reshape(-1, 2)
            area, angle, pivot, size = extract_image_area(poly, image)
            if not area.size:
                continue
            # Extract text
            gray = cv2.cvtColor(area, cv2.COLOR_BGR2GRAY)
            text = pytesseract.image_to_string(gray, lang='eng', config='--psm 9') # or 6?
            text = cls.__filter_non_printable(text)
            
            font_scale, text_height = cls.__get_optimal_font_scale(text, size[0], size[1])
            position = cls.__get_optimal_text_position(text_height, size, pivot)
            color = cls.__get_text_color(area, size)
            # Save data
            result.append(TextBlock(text, position, font_scale, box, pivot, angle, size, color))
        return result

    @staticmethod
    def __get_optimal_font_scale(text, width, height):
        """ Based on the bouncing box width """
        new_height = 1
        for scale in reversed(np.arange(0, 60, 0.5)):
            textSize = cv2.getTextSize(text, fontFace=cv2.FONT_HERSHEY_DUPLEX, fontScale=scale/10, thickness=1)
            new_height = textSize[0][1]
            new_width = textSize[0][0]
            if (new_width <= width and new_height <= height):
                return scale/10, new_height
        return 1, new_height

    @staticmethod
    def __filter_non_printable(str):
        """ remove non printable characters """
        filtered_characters = list(s for s in str if s.isprintable())
        return ''.join(filtered_characters)

    @staticmethod
    def __get_optimal_text_position(text_height, box_size, pivot):
        """ return optimal text coordinates. center vertically """
        pos_x = pivot[0]
        pos_y = pivot[1] + box_size[1] - round((box_size[1] - text_height) / 2)
        return (pos_x, pos_y)

    @staticmethod
    def __get_text_color(image_area, size):
        """ Define text color based on the background border's color """
        max_y = size[1] - 1
        max_x = size[0] - 1
        color_buffer = []

        # pick border colors
        for x in range(0, size[0], 1):
            color_buffer.append(image_area[0][x])
            color_buffer.append(image_area[max_y][x])
        for y in range(0, size[1], 1):
            color_buffer.append(image_area[y][0])
            color_buffer.append(image_area[y][max_x])
        # filter colors
        freq_dict = dict()
        for el in color_buffer:
            key = el.tostring()
            if key in freq_dict:
                freq_dict[key][0] += 1
            else:
                freq_dict[key] = [1, el]
        values = freq_dict.values()
        # Get most used color
        max = None
        for el in values:
            if max == None or el[0] > max[0]:
                max = el
        # inverse color
        return (int(255 - max[1][0]), int(255 - max[1][1]), int(255 - max[1][2]))
            