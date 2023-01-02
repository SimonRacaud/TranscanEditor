from argparse import ArgumentError
from typing import Tuple, List
from PIL import ImageFont

from src.model.model import Vector2I

class TextManager:
    @staticmethod
    def get_font_size(font, text: str):
        return font.getsize(text)

    @classmethod
    def compute(cls, font_path: str, text: str, areaSize: Vector2I, text_padding: int):
        """ 
            Get font from filepath. Support tff and otf files. 
            Format the (text) in input to fit inside the rectangle (areaSize)
            return:
                - Pillow font object
                - Line array
                - Text total height
        """
        if text == "":
            return ImageFont.load_default(), [], 0
        try:
            scale, lines, text_height = cls.__get_optimal_font_scale(text, areaSize, font_path, text_padding)
            return ImageFont.truetype(font_path, size=scale), lines, text_height
        except OSError as err:
            print("(FontManager) Warning: fail to load font", font_path, "Using default.", err)
            raise ArgumentError(None, message="Fail to load font "+font_path)
    
    @staticmethod
    def __get_optimal_font_scale(text: str, areaSize: Vector2I, font_path: str, text_padding: int) -> Tuple[int, List[str], int]:
        """ Calcul font size. Based on the bouncing box width and height """
        scale = 1
        word_array = text.split(' ')
        res_text_height = 0
        res_lines = None
        ## Compute font scale and lines
        while True:
            font = ImageFont.truetype(font_path, size=scale)
            lines, text_height, max_line_width = TextManager.__split_text(font, areaSize, word_array, text_padding)
            if text_height >= areaSize.y or max_line_width >= areaSize.x:
                break
            res_text_height = text_height
            res_lines = lines
            scale += 1
        if res_lines == None:
            res_lines = lines
            res_text_height = text_height
        ## Assemble lines
        text_lines = []
        for line in res_lines:
            tmp = ''
            for word in line:
                if tmp != '' and tmp[-1] != '\n':
                    tmp += ' '
                tmp += word
            text_lines.append(tmp)
        return (scale - 1), text_lines, res_text_height

    @staticmethod
    def __split_text(font, areaSize: Vector2I, word_array: list, text_padding: int):
        space_width, _ = TextManager.__get_text_dimensions(' ', font)
        words_data = []
        for word in word_array:
            width, height = TextManager.__get_text_dimensions(word, font)
            words_data.append({'word': word, 'width': width, 'height': height})
        #
        lines = [[]]
        text_height = 0
        max_line_width = 0

        line_idx = 0
        line_width = 0
        line_height = 0
        for data in words_data:
            if (line_width + data['width'] + space_width) >= areaSize.x and len(lines[line_idx]) > 0:
                line_idx += 1
                max_line_width = max(max_line_width, line_width)
                text_height += (line_height + text_padding)
                line_height = 0
                line_width = 0
                lines.append([])
            line_height = max(line_height, data['height'])
            lines[line_idx].append(data['word'])
            line_width += data['width'] + space_width
        text_height += line_height
        max_line_width = max(max_line_width, line_width)
        return lines, text_height, max_line_width
    
    @staticmethod
    def __get_text_dimensions(text_string, font):
        if text_string == ' ':
            return font.getsize(text_string)
        else:
            #ascent, descent = font.getmetrics()
            bbox = font.getmask(text_string).getbbox()
            text_width = bbox[2]
            text_height = bbox[3]
            return (text_width, text_height)
        