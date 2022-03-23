import string
from PIL import Image, ImageDraw
from typing import Sequence, Tuple
import cv2

import numpy as np
from src.utility.FontManager import FontManager
from src.model import AppConfig, OCRBlock, Vector2I
from src.utility.extract_image_area import extract_image_area

from src.utility.show_debug import show_debug

class TextEditor:
    @classmethod
    def process_img(cls, config: AppConfig, textBlockList: Sequence[OCRBlock], image: np.ndarray, raw_image: np.ndarray) -> np.ndarray:
        for block in textBlockList:
            color, box_size = cls.__get_auto_text_color(raw_image, block)
            font = FontManager.get(config.default_font, block.text, box_size) # TODO get font path
            text_size = FontManager.get_font_size(font, block.text)

            position: Vector2I = cls.__get_optimal_text_position(text_size[1], box_size, block.pivot)
            # Create text segment
            segment, background_color = cls.__create_text_segment(block.text, image, position,
                color, font)
            # Rotate segment
            if abs(block.angle) > 2:
                segment = cls.__rotate_segment(block, segment, background_color)
            # Apply segment on image
            image = cls.__apply_segment_on_image(segment, image, color, background_color)
        return image

    @staticmethod
    def __create_text_segment(text: string, image, position: Vector2I, color: Tuple[int, int, int], 
            font):
        background = (0, 0, 0)
        segment = np.zeros(image.shape, dtype="uint8")
        if color == (0, 0, 0):
            background = (255, 255, 255)
            segment = cv2.bitwise_not(segment)
        # Draw text with Pillow
        segment = cv2.cvtColor(segment, cv2.COLOR_BGR2RGB)
        pil_img = Image.fromarray(segment)
        draw = ImageDraw.Draw(pil_img)
        draw.text(position.data(), text, font=font, fill=color, stroke_width=0, stroke_fill=(0, 0, 0))
        segment = cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)
        #
        return segment, background
    
    @staticmethod
    def __rotate_segment(block: OCRBlock, segment, backgroundColor):
        rotate_matrix = cv2.getRotationMatrix2D(center=block.pivot.data(), angle=-block.angle, scale=1)
        shape = segment.shape[:2]
        return cv2.warpAffine(src=segment, M=rotate_matrix, dsize=(shape[1], shape[0]), borderValue=backgroundColor)
    
    @staticmethod
    def __apply_segment_on_image(segment, image, color: Tuple[int, int, int], background_color):
        # Create mask
        mask = cv2.inRange(segment, background_color, background_color)
        mask = cv2.bitwise_not(mask)
        # Apply mask
        pil_segment = Image.fromarray(cv2.cvtColor(segment, cv2.COLOR_BGR2RGB))
        pil_mask = Image.fromarray(mask)
        pil_img = Image.fromarray(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
        res = Image.composite(pil_segment, pil_img, pil_mask)
        image = cv2.cvtColor(np.array(res), cv2.COLOR_RGB2BGR)
        return image
    
    @staticmethod
    def __get_optimal_text_position(text_height: int, bbox_size: Vector2I, pivot: Vector2I) -> Vector2I:
        """ return optimal text coordinates. centered vertically """
        pos_x = pivot.x
        pos_y = pivot.y + round((bbox_size.y - text_height) / 2)
        return Vector2I(pos_x, pos_y)

    @staticmethod
    def __get_auto_text_color(image, block: OCRBlock):
        """ Define text color based on the background border's color """
        area, angle, _, _ = extract_image_area(block.polygon, image)
        size = Vector2I(area.shape[1], area.shape[0])

        if abs(angle) > 2 and len(block.text) > 1:
            block.angle = angle
        max_y = size.y - 1
        max_x = size.x - 1
        color_buffer = []

        # pick border colors
        for x in range(0, size.x, 1):
            color_buffer.append(area[0][x])
            color_buffer.append(area[max_y][x])
        for y in range(0, size.y, 1):
            color_buffer.append(area[y][0])
            color_buffer.append(area[y][max_x])
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
        color = max[1]
        # inverse color
        return (int(255 - color[0]), int(255 -color[1]), int(255 - color[2])), size
            