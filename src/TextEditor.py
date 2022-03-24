import string
from PIL import Image, ImageDraw
from typing import Sequence, Tuple
import cv2

import numpy as np
from src.utility.TextManager import TextManager
from src.model import AppConfig, BlockCluster, Vector2I
from src.utility.extract_image_area import extract_image_area

from src.utility.show_debug import show_debug

class TextEditor:
    @classmethod
    def process_img(cls, config: AppConfig, blockList: Sequence[BlockCluster], image: np.ndarray, raw_image: np.ndarray) -> np.ndarray:
        for block in blockList:
            area, angle, pivot, size = extract_image_area(block.polygon, raw_image)
            if not area.size:
                continue
            color = cls.__get_auto_text_color(area)
            text_padding = config.text_padding_y
            font, lines, text_height = TextManager.compute(config.default_font, block.sentence, size, text_padding)
            if len(lines) == 0:
                continue
            position: Vector2I = cls.__align_text_vertical(text_height, size, pivot)
            # Create text segment
            segment, background_color = cls.__create_text_segment(lines, image, position,
                color, font, size.x, text_padding)
            # Rotate segment
            if abs(angle) > 2:
                segment = cls.__rotate_segment(pivot, angle, segment, background_color)
            # Apply segment on image
            image = cls.__apply_segment_on_image(segment, image, background_color)
        return image

    @staticmethod
    def __create_text_segment(lines: Sequence[str], image, position: Vector2I, color: Tuple[int, int, int], 
            font, area_width: int, text_padding: int):
        background = (0, 0, 0)
        segment = np.zeros(image.shape, dtype="uint8")
        if color == (0, 0, 0):
            background = (255, 255, 255)
            segment = cv2.bitwise_not(segment)
        # Draw text with Pillow
        segment = cv2.cvtColor(segment, cv2.COLOR_BGR2RGB)
        pil_img = Image.fromarray(segment)
        draw = ImageDraw.Draw(pil_img)
        for line in lines:
            w, h = draw.textsize(line, font=font)
            pos = (position.x + ((area_width - w) / 2), position.y)
            draw.text(pos, line, font=font, fill=color, stroke_width=0, stroke_fill=(0, 0, 0))
            position.y += h + text_padding
        segment = cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)
        #
        return segment, background
    
    @staticmethod
    def __rotate_segment(pivot, angle, segment, backgroundColor):
        rotate_matrix = cv2.getRotationMatrix2D(center=pivot.data(), angle=-angle, scale=1)
        shape = segment.shape[:2]
        return cv2.warpAffine(src=segment, M=rotate_matrix, dsize=(shape[1], shape[0]), borderValue=backgroundColor)
    
    @staticmethod
    def __apply_segment_on_image(segment, image, background_color):
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
    def __align_text_vertical(text_height: int, bbox_size: Vector2I, pivot: Vector2I) -> Vector2I:
        """ return optimal text coordinates. centered vertically """
        pos_x = pivot.x
        pos_y = pivot.y + round((bbox_size.y - text_height) / 2)
        return Vector2I(pos_x, pos_y)

    @staticmethod
    def __get_auto_text_color(area):
        """ Define text color based on the background border's color """
        size = Vector2I(area.shape[1], area.shape[0])

        # if abs(angle) > 2 and len(block.sentence) > 1:
        #     block.angle = angle
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
        return (int(255 - color[0]), int(255 -color[1]), int(255 - color[2]))
            