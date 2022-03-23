import io
from typing import Sequence
import os
import numpy as np
from PIL import Image
import os

from src.utility.OCRResultCacheManager import OCRResultCacheManager
from .IOpticalCharacterRecognition import IOpticalCharacterRecognition
from src.model import OCRBlock, OCRConfig, OCRPage, Vector2I

import boto3
from botocore.config import Config

class OCRAmazonAWS(IOpticalCharacterRecognition):
    @classmethod
    def setup(cls, config: OCRConfig):
        """ Init OCR """
        cls.config = config
        if (not "AWS_ACCESS_KEY_ID" in os.environ) or (not "AWS_SECRET_ACCESS_KEY" in os.environ):
            print("Error: missing env variable AWS_ACCESS_KEY_ID or/and AWS_SECRET_ACCESS_KEY. exit")
            exit(1)
        my_config = Config(
            region_name='eu-west-3',
            signature_version='v4',
            retries = {
                'max_attempts': 5,
                'mode': 'standard'
            }
        )
        cls.client = boto3.client('textract', config=my_config)

    @classmethod
    def process_batch(cls, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        raise NotImplementedError

    @classmethod
    def process_img(cls, img_path: str) -> OCRPage:
        """ Process an image and return a list of text and bouncing boxes """
        try:
            with open(img_path, 'rb') as img_file:
                ## Read binary
                img_bytes = img_file.read()
                ## Format and convert to numpy array
                img = Image.open(io.BytesIO(img_bytes))
                image = np.asarray(img)
                ## AWS network call
                try:
                    response = OCRResultCacheManager.load_result(img_bytes, cls.config.cache_path)
                    print("Info: OCRAmazonAWS::process_img OCR result loaded from cache")
                except FileNotFoundError:
                    response = cls.client.detect_document_text(Document={'Bytes': img_bytes})
                    try:
                        OCRResultCacheManager.save_result(img_bytes, response, cls.config.cache_path)
                        print("Info: OCRAmazonAWS::process_img OCR result saved in cache")
                    except RuntimeError:
                        print("Warning: OCRAmazonAWS::process_img fail to save result in cache")
                blocks = response['Blocks']
                page_count = response['DocumentMetadata']['Pages']
                ### Format data
                print("AWS request success. Page count", page_count)
                return cls.__format_page(blocks, img_path, image) 
        except BaseException as err:
            print("Error: OCRAmazonAWS::process_img -", err)
            raise
    
    @staticmethod
    def __format_page(blocks, img_path, image) -> OCRPage:
        textBlockList = []
        width = image.shape[1]
        height = image.shape[0]
        for block in blocks:
            if block['BlockType'] == 'LINE':
                polygon = OCRAmazonAWS.__format_polygon(block['Geometry']['Polygon'], width, height)
                bounding_box = OCRAmazonAWS.__format_bounding_box(block['Geometry']['BoundingBox'], width, height)
                text = block['Text']
                pivot = Vector2I(bounding_box['left'], bounding_box['top'])
                size = Vector2I(bounding_box['width'], bounding_box['height'])

                textBlockList.append(OCRBlock(bounding_box, polygon, text, pivot, size))

        return OCRPage(textBlockList, img_path, image)
    
    @staticmethod
    def __format_bounding_box(bbox, width, height):
        return {
            'width': int(bbox['Width'] * width),
            'height': int(bbox['Height'] * height),
            'left': int(bbox['Left'] * width),
            'top': int(bbox['Top'] * height)
        }
    
    @staticmethod
    def __format_polygon(poly, width, height):
        points = []
        for pt in poly:
            points.append([
                pt['X'] * width,
                pt['Y'] * height
            ])
        return np.array(points).astype(np.int32)