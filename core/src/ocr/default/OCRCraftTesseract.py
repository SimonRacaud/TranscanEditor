from typing import Sequence, Tuple

from src.model.model import OCRConfig, OCRPage
from src.ocr.IOpticalCharacterRecognition import IOpticalCharacterRecognition
from src.ocr.default.characterRecognition.CharacterDetector import CharacterDetector
from src.ocr.default.TextExtractor import TextExtractor
from numpy import ndarray

class OCRCraftTesseract(IOpticalCharacterRecognition):

    def __init__(self, config: OCRConfig) -> None:
        config.craft_config = CharacterDetector.setup(config.craft_config)
        self.config = config

    def process_batch(self, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        result = []
        for image, bboxes, image_path in CharacterDetector.process(self.config.craft_config, img_path_list):
            textBlockList = TextExtractor.character_extraction(image, bboxes)
            result.append(OCRPage(src_path=image_path, blocks=textBlockList))
        return result

    def process_img(self, img_path: str) -> Tuple[OCRPage, ndarray]:
        """ Process an image and return a list of text and bouncing boxes """
        image, bboxes, image_path = CharacterDetector.process_one(self.config.craft_config, img_path)
        textBlockList = TextExtractor.character_extraction(image, bboxes)
        return OCRPage(src_path=image_path, blocks=textBlockList), image
