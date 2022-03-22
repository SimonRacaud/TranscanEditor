from argparse import ArgumentError
from PIL import ImageFont

from src.model import Vector2I

class FontManager:
    @classmethod
    def get(cls, font_path: str, text: str, areaSize: Vector2I):
        """ Get font from filepath. Support tff and otf files """
        if text == "":
            return ImageFont.load_default()
        try:
            font = ImageFont.truetype(font_path, size=1)
            scale = cls.__get_optimal_font_scale(text, areaSize, font, font_path)
            if scale > 1:
                return ImageFont.truetype(font_path, size=scale)
            return font
        except OSError as err:
            print("(FontManager) Warning: fail to load font", font_path, "Using default.", err)
            return ImageFont.load_default()
    
    @staticmethod
    def __get_optimal_font_scale(text: str, areaSize: Vector2I, font, font_path: str) -> int:
        """ Calcul font size. Based on the bouncing box width """
        scale = 1
        size = font.getsize(text)
        while size[0] < areaSize.x and size[1] < areaSize.y:
            scale += 1
            font = ImageFont.truetype(font_path, scale)
            size = font.getsize(text)
        return (scale - 1)
    
    @staticmethod
    def get_font_size(font, text: str):
        return font.getsize(text)
        