from __future__ import annotations
from dataclasses import dataclass
from enum import Enum
from typing import Sequence, Tuple

import numpy as np

from src.utility.check import check_argument
from src.utility.exception import InvalidJson

## ENUM

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

## CONFIGURATION

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
    src_img_path: str = "" # Target image file path
    output_folder: str = ""
    ocr_service: OCRService = OCRService.LOCAL_CRAFTTESSERACT

    cache_path = './cache'
    box_cluster_search_range = 20   # Max distance width the text dialogue bubble
    box_cluster_search_step = 5     # Shifts to research the next bubble
    craft_config: CraftConfig = CraftConfig()

    @staticmethod
    def deserialize(data: dict) -> OCRConfig:
        REQUIRED = ['srcImgPath', 'outputFolder', 'ocrService']
        check_argument(data, REQUIRED)

        obj = OCRConfig()
        obj.src_img_path = data['srcImgPath']
        obj.output_folder = data['outputFolder']
        try:
            obj.ocr_service = OCRService[data['ocrService']]
        except:
            raise InvalidJson("Invalid OCR type, " + data['ocrService'])
        if 'cachePath' in data:
            obj.cache_path = data['cachePath']
        if 'boxClusterSearchRange' in data:
            obj.box_cluster_search_range = data['boxClusterSearchRange']
        if 'boxClusterSearchStep' in data:
            obj.box_cluster_search_step = data['boxClusterSearchRange']
        return obj

# @dataclass
# class AppConfig:
#     translation_service: TranslatorService
#     default_font: str = 'Chilanka-Regular.otf'
#     ocr_config: OCRConfig = OCRConfig()
#     text_padding_y = 8              # Default text padding (y axis)
#     text_stroke_width = 3           # Text stroke border width

#     @staticmethod
#     def deserialize(data):
#         required = ['input_folder', 'translation_service', 'output_folder', 'default_font', 'ocr_service', 'line_height', 'text_stroke']
#         check_argument(data, required)
#         config = AppConfig(data['input_folder'], data['translation_service'])
#         config.translation_service = TranslatorService[data['translation_service']]
#         config.output_folder = data['output_folder']
#         config.default_font = data['default_font']
#         config.ocr_service = OCRService[data['ocr_service']]
#         config.text_padding_y = data['line_height']
#         config.text_stroke_width = data['text_stroke']
#         return config

## UTILITY

@dataclass
class Vector2I:
    x: int = 0
    y: int = 0

    @classmethod
    def fromarray(cls, array) -> None:
        return cls(array[0], array[1])

    def data(self):
        return (int(self.x), int(self.y))
    
    def round(self):
        self.x = round(self.x)
        self.y = round(self.y)
    
    def serialize(self):
        return {
            "x": int(self.x),
            "y": int(self.y)
        }
    @staticmethod
    def deserialize(data) -> Vector2I:
        check_argument(data, ['x', 'y'])
        return Vector2I(int(data["x"]), int(data["y"]))

## ENTITIES

@dataclass
class OCRBlock:
    """ Result of an OCR treatment """
    #box: any # Rectangle bouncing box
    polygon: Sequence[Sequence[int]] # Polygon bouncing box
    text: str
    pivot: Vector2I # box top left coord
    size: Vector2I # box size
    angle: float # box rotation angle

    def __eq__(self, __o: object) -> bool:
        return np.array_equal(self.polygon, __o.polygon) and self.text == __o.text
    
    def serialize(self):
        return {
            "polygon": self.polygon.astype(int).tolist(),
            "text": self.text,
            "pivot": self.pivot.serialize(),
            "size": self.size.serialize(),
            "angle": self.angle
        }
    @staticmethod
    def deserialize(data) -> OCRBlock:
        check_argument(data, ['polygon', 'text', 'pivot', 'size', 'angle'])

        return OCRBlock(
            polygon=np.array(data['polygon']),
            text=data['text'],
            pivot=Vector2I.deserialize(data['pivot']),
            size=Vector2I.deserialize(data['size']),
            angle=float(data['angle'])
        )

@dataclass
class BlockCluster:
    blocks: Sequence[Tuple[int, OCRBlock]]
    polygon: np.ndarray # 4 points
    # box: Rect
    sentence: str
    translation: str = None
    cleanBox: bool = True
    font: str = None
    color: int = None
    line_height: float = 0

    def serialize(self):
        return {
            "blocks": [], # TODO: export blocks indexes ?
            "polygon": self.polygon.astype(int).tolist() if self.polygon.shape == (4, 2) else [],
            "sentence": self.sentence,
            "translation": self.translation,
            "clean_box": self.cleanBox,
            "font": self.font,
            "color": self.color,
            "line_height": self.line_height
        }
    @staticmethod
    def deserialize(data) -> BlockCluster:
        check_argument(data, ['blocks', 'polygon', 'sentence', 'translation', 'clean_box', 'font', 'color', 'line_height'])

        return BlockCluster(
            blocks=list(map(lambda b: OCRBlock.deserialize(b), data['blocks'])),
            polygon=np.array(data['polygon']),
            sentence=data["sentence"],
            translation=data["translation"],
            cleanBox=data["clean_box"],
            font=data["font"],
            color=data["color"],
            line_height=float(data["line_height"]))

@dataclass
class OCRPage:
    src_path: str # Source image file path
    blocks: Sequence[OCRBlock]
    clusters: Sequence[BlockCluster] = None
    clean_path: str = None # Clean image path
    render_path: str = None # Render image path
    index: int = 0 # Page index

    def serialize(self):
        return {
            "index": self.index,
            "srcImgPath": self.src_path,
            "cleanImgPath": self.clean_path,
            "renderImgPath": self.render_path,

            "blocks": list(map(lambda b: b.serialize(), self.blocks)),
            "clusters": list(map(lambda c: c.serialize(), self.clusters)),
        }
    @staticmethod
    def deserialize(data) -> OCRPage:
        check_argument(data, ['srcImgPath', "cleanImgPath", "renderImgPath", 'blocks', 'clusters'])

        page = OCRPage(
            index=int(data['index']),
            src_path=data['srcImgPath'],
            clean_path=data['cleanImgPath'],
            render_path=data['renderImgPath'],
            blocks=[],
            clusters=[])
        page.blocks=list(map(lambda b: OCRBlock.deserialize(b), data['blocks']))
        page.clusters=list(map(lambda c: BlockCluster.deserialize(c), data['clusters']))
        return page