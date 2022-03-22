from dataclasses import dataclass
import string
from typing import Sequence
import cv2

@dataclass
class CraftConfig:
    cuda = False            # Use cuda for inference
    trained_model = './weights/craft_mlt_25k.pth' # pretrained model
    text_threshold = 0.7    # text confidence threshold
    low_text = 0.4          # text low-bound score
    link_threshold = 0.4    # link confidence threshold
    canvas_size = 1300      # image size for inference
    mag_ratio = 1.5         # image magnification ratio
    poly = False            # enable polygon type
    show_time = True        # show processing time
    refine = True           # enable link refiner
    refiner_model = './weights/craft_refiner_CTW1500.pth' # pretrained refiner model

@dataclass
class OCRConfig:
    craft_config: CraftConfig = CraftConfig()

@dataclass
class AppConfig:
    input_folder: string
    output_folder = './result/'
    default_font: string = 'Chilanka-Regular.otf'
    ocr_config: OCRConfig = OCRConfig()

@dataclass
class Vector2I:
    x: int
    y: int

    @classmethod
    def fromarray(cls, array) -> None:
        return cls(array[0], array[1])

    def data(self):
        return (self.x, self.y)

@dataclass
class OCRBlock:
    """ Result of an OCR treatment """
    box: any # Rectangle bouncing box
    polygon: any # Polygon bouncing box
    text: string
    pivot: Vector2I # box top left coord
    size: Vector2I # box size
    angle: float = 0.0 # box rotation angle

@dataclass
class OCRPage:
    blocks: Sequence[OCRBlock]
    image_path: string # page file path
    image: any