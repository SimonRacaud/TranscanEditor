from PIL import Image, ImageDraw
from typing import Sequence, Tuple
import cv2
import numpy as np

from src.TextManager import TextManager
from src.model.model import BlockCluster, Vector2I, EditConfig
from src.utility.extract_image_area import extract_image_area

#from src.utility.show_debug import show_debug

class TextEditor:
    @classmethod
    def process_img(cls, config: EditConfig, blockList: Sequence[BlockCluster], image: np.ndarray, raw_image: np.ndarray) -> np.ndarray:
        for block in blockList:
            area, angle, pivot, size = extract_image_area(block.polygon, raw_image, False)
            if not area.size:
                continue
            color = cls.__get_auto_text_color(area)
            text_padding = config.line_height
            font, lines, text_height = TextManager.compute(config.font, block.translation, size, text_padding)
            if len(lines) == 0:
                continue
            # Create text segment
            segment = cls.__create_text_segment(lines, image, pivot,
                color, font, size, text_padding, config.stroke_width, text_height)
            # Rotate segment
            if abs(angle) > 2:
                segment = cls.__rotate_segment(pivot, angle, segment)
            # Apply segment on image
            image = cls.__apply_segment_on_image(segment, image)
        return image

    @staticmethod
    def __create_text_segment(lines: Sequence[str], image, position: Vector2I, color: Tuple[int, int, int], 
            font, area_size: Vector2I, text_padding, text_stroke_width: int, text_height: int):
        padding_y = round((area_size.y - text_height) / len(lines) * 0.8)
        stroke_color = TextEditor.__reverse_color(color)
        segment = Image.new('RGBA', (image.shape[1], image.shape[0]), (0, 0, 0, 0))
        seg_draw = ImageDraw.Draw(segment)
        # Draw text with Pillow
        position_y = position.y
        for line in lines:
            position_y += padding_y
            w, h = seg_draw.textsize(line, font=font)
            pos = (position.x + ((area_size.x - w) / 2), position_y)
            seg_draw.text(pos, line, font=font, fill=color, stroke_width=text_stroke_width, stroke_fill=stroke_color)
            position_y += h + text_padding
        #
        return segment
    
    @staticmethod
    def __rotate_segment(pivot, angle, segment):
        return segment.rotate(angle, center=pivot.data())
    
    @staticmethod
    def __apply_segment_on_image(segment, image):
        pil_img = Image.fromarray(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
        sx, sy = segment.size
        pil_img.paste(segment, (0, 0, sx, sy), segment)
        return cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)
    
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
        return TextEditor.__reverse_color(color)
    
    @staticmethod
    def __reverse_color(color):
        return (int(255 - color[0]), int(255 -color[1]), int(255 - color[2]))
            