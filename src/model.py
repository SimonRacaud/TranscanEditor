from dataclasses import dataclass
from enum import Enum
import string
from typing import Sequence

import numpy as np

class OCRService(Enum):
    LOCAL_CRAFTTESSERACT = 'LOCAL_CRAFT_TESSERACT'
    AWS_REKOGNITION = 'AWS_REKOGNITION'

    def __str__(self) -> str:
        return self.value

class TranslatorService(Enum):
    DISABLE = "DISABLE"
    GOOGLE = "GOOGLE"
    MICROSOFT = "MICROSOFT"
    DEEPL = 'DEEPL'
    LINGUEE = 'LINGUEE'
    AMAZON = 'AWS'
    PONS = 'PONS'
    MYMEMORY = 'MYMEMORY'
    YANDEX = 'YANDEX'
    PAPAGO = 'PAPAGO'
    LIBRE = 'LIBRE'

    def __str__(self) -> str:
        return self.value

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
    cache_path = './cache'

@dataclass
class AppConfig:
    input_folder: string
    translation_service: TranslatorService
    output_folder = './result/'
    default_font: string = 'Chilanka-Regular.otf'
    ocr_service: OCRService = OCRService.LOCAL_CRAFTTESSERACT
    ocr_config: OCRConfig = OCRConfig()
    box_cluster_search_range = 20   # Max distance width the text dialogue bubble
    box_cluster_search_step = 5     # Shifts to research the next bubble
    text_padding_y = 8              # Default text padding (y axis)
    text_stroke_width = 3           # Text stroke border width

@dataclass
class Vector2I:
    x: int
    y: int

    @classmethod
    def fromarray(cls, array) -> None:
        return cls(array[0], array[1])

    def data(self):
        return (self.x, self.y)
    
    def round(self):
        self.x = round(self.x)
        self.y = round(self.y)

@dataclass
class OCRBlock:
    """ Result of an OCR treatment """
    box: any # Rectangle bouncing box
    polygon: any # Polygon bouncing box
    text: string
    pivot: Vector2I # box top left coord
    size: Vector2I # box size
    angle: float # box rotation angle

    def __eq__(self, __o: object) -> bool:
        return np.array_equal(self.polygon, __o.polygon) and self.text == __o.text

@dataclass
class BlockCluster:
    blocks: Sequence[OCRBlock]
    polygon: Sequence[Vector2I] # 4 points
    sentence: str
    translation: str = None

@dataclass
class OCRPage:
    blocks: Sequence[OCRBlock]
    image_path: string # page file path
    image: any
    clusters: Sequence[BlockCluster] = None
