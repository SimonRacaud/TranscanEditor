from typing import Sequence

from src.model import OCRConfig, OCRPage
from src.ocr.IOpticalCharacterRecognition import IOpticalCharacterRecognition
from src.ocr.default.charaterRecognition.CharacterDetector import CharacterDetector
from src.ocr.default.TextExtractor import TextExtractor

class OCRCraftTesseract(IOpticalCharacterRecognition):

    @classmethod
    def setup(cls, config: OCRConfig):
        config.craft_config = CharacterDetector.setup(config.craft_config)
        cls.config = config

    @classmethod
    def process_batch(cls, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        result = []
        for image, bboxes, image_path in CharacterDetector.process(cls.config.craft_config, img_path_list):
            textBlockList = TextExtractor.character_extraction(image, bboxes)
            result.append(OCRPage(textBlockList, image_path, image))
        return result

    @classmethod
    def process_img(cls, img_path: str) -> OCRPage:
        """ Process an image and return a list of text and bouncing boxes """
        image, bboxes, image_path = CharacterDetector.process_one(cls.config.craft_config, img_path)
        textBlockList = TextExtractor.character_extraction(image, bboxes)
        return OCRPage(textBlockList, image_path, image)
