import io
from typing import Sequence, Tuple
import os
import cv2
import numpy as np
from PIL import Image
import os

from src.utility.OCRResultCacheManager import OCRResultCacheManager
from src.ocr.IOpticalCharacterRecognition import IOpticalCharacterRecognition
from src.model.model import OCRConfig, OCRPage, Vector2I, OCRBlock
from src.utility.exception import InternalError
from src.ocr.OCR_AWSRekognition import OCR_AWSRekognition
from src.utility.cyrillic_to_latin import cyrillic_to_latin

from boto3 import client as AWSClient
from botocore.config import Config as AWSConfig

from numpy import ndarray
import threading

class OCR_AWSTextract(IOpticalCharacterRecognition):
    boto3_client_lock = threading.Lock()

    def __init__(self, config: OCRConfig) -> None:
        """ Init OCR """
        self.config = config
        if (not "AWS_ACCESS_KEY_ID" in os.environ) or (not "AWS_SECRET_ACCESS_KEY" in os.environ):
            print("Error: missing env variable AWS_ACCESS_KEY_ID or/and AWS_SECRET_ACCESS_KEY. exit")
            exit(1)
        if (not "AWS_REGION" in os.environ or os.environ["AWS_REGION"] == ""):
            print("Error: missing env variable AWS_REGION.")
            exit(1)
        my_config = AWSConfig(
            region_name=os.environ["AWS_REGION"],
            signature_version='v4',
            retries = {
                'max_attempts': 5,
                'mode': 'standard'
            }
        )
        with OCR_AWSTextract.boto3_client_lock:
            try:
                self.client = AWSClient(
                    'textract',
                    aws_access_key_id=os.environ["AWS_ACCESS_KEY_ID"],
                    aws_secret_access_key=os.environ["AWS_SECRET_ACCESS_KEY"],
                    config=my_config)
            except BaseException as e:
                print("Error: AWS connection (textract) - ", e)
                raise InternalError("Unable to connect to AWS. Did you provide the right credentials?")

    def process_batch(self, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        result = []
        for img_path in img_path_list:
            result.append(self.process_img(img_path))
        return result


    def process_img(self, img_path: str) -> Tuple[OCRPage, ndarray]:
        """ Process an image and return a list of text and bouncing boxes """
        try:
            with open(img_path, 'rb') as img_file:
                ## Read binary
                img_bytes = img_file.read()
                ## Format and convert to numpy array
                img = Image.open(io.BytesIO(img_bytes))
                image = np.asarray(img)
                image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
                result=[]
                try:
                    raise FileNotFoundError() # TODO: DEBUG
                    result = OCRResultCacheManager.load_result(img_bytes, self.config.cache_path)
                    print("Info: OCR_AWSTextract::process_img OCR result loaded from cache")
                except FileNotFoundError:
                    ## AWS network call
                    response = self.client.detect_document_text(Document={'Bytes': img_bytes})
                    ##
                    result = response['Blocks']
                    # https://docs.aws.amazon.com/textract/latest/dg/sync-calling.html
                    result = OCR_AWSRekognition.filter_low_confidence_blocks(result)
                    #OCRResultCacheManager.save_result(img_bytes, result, self.config.cache_path) # TODO : module specific
                ### Format data
                return OCR_AWSTextract.__format_page(result, img_path, image), image
        except BaseException as err:
            print("Error: OCR_AWSTextract::process_img -", err)
            raise err

    @staticmethod
    def __format_page(blocks, img_path, image) -> OCRPage:
        textBlockList = []
        width = image.shape[1]
        height = image.shape[0]
        for block in blocks:
            if block['BlockType'] == 'LINE':
                polygon = OCR_AWSRekognition.format_polygon(block['Geometry']['Polygon'], width, height)
                text = block['Text']
                text = cyrillic_to_latin(text)
                boundingBox = block['Geometry']['BoundingBox']
                size = Vector2I(boundingBox['Width'] * width, boundingBox['Height'] * height)
                pivot = Vector2I(boundingBox['Left'] * width, boundingBox['Top'] * height)
                #area, angle, pivot, size = extract_image_area(polygon, image, True)
                textBlockList.append(OCRBlock(polygon=polygon, text=text, pivot=pivot, size=size, angle=0.0))
        return OCRPage(blocks=textBlockList, src_path=img_path)