"""
    Description: data Models related to the API endpoints
"""

from dataclasses import dataclass

from src.model.model import OCRConfig
from src.utility.check import check_argument

@dataclass
class OCRInput:
    index: int = 0
    ocr_config: OCRConfig = OCRConfig()

    @staticmethod
    def deserialize(data):
        REQUIRED = ['index', 'config']
        check_argument(data, REQUIRED)

        obj = OCRInput(int(data['index']))
        obj.ocr_config = OCRConfig.deserialize(data['config'])
        return obj
