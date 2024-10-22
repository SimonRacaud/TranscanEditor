from typing import Sequence, Tuple
from src.ocr.OCR_AWSRekognition import OCR_AWSRekognition
from src.ocr.OCR_AWSTextract import OCR_AWSTextract
from src.ocr.default.OCRCraftTesseract import OCRCraftTesseract
from src.model.model import OCRConfig, OCRPage, OCRService

class OCRManager:
    def __init__(self, config: OCRConfig) -> None:
        ocr_class = {
            OCRService.LOCAL_CRAFTTESSERACT : OCRCraftTesseract,
            OCRService.AWS_REKOGNITION : OCR_AWSRekognition,
            OCRService.AWS_TEXTRACT : OCR_AWSTextract
        }
        self.ocr = ocr_class[config.ocr_service](config)
        
    def process_batch(self, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        return self.ocr.process_batch(img_path_list)


    def process_img(self, img_path: str) -> Tuple[OCRPage, any]:
        """ Process an image and return a list of text and bouncing boxes """
        return self.ocr.process_img(img_path)
