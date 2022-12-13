"""
    Description: data Models related to the API endpoints
"""

from dataclasses import dataclass

from src.model.model import OCRConfig, OCRPage, EditConfig
from src.utility.check import check_argument
from src.utility.exception import InvalidJson

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

@dataclass
class RenderInput:
    # Config
    config: EditConfig
    # Page
    page: OCRPage

    @staticmethod
    def deserialize(data):
        check_argument(data, ["config", "page"])
        if type(data["config"]) is not dict:
            raise InvalidJson("Invalid config field type")
        #
        return RenderInput(
            config=EditConfig.deserialize(data["config"]),
            page=OCRPage.deserialize(data["page"])
        )
