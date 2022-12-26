import io
from typing import Sequence, Tuple
import os
import cv2
import numpy as np
from PIL import Image
import os

from src.utility.OCRResultCacheManager import OCRResultCacheManager
from src.utility.extract_image_area import extract_image_area
from src.ocr.IOpticalCharacterRecognition import IOpticalCharacterRecognition
from src.model.model import OCRBlock, OCRConfig, OCRPage
from src.utility.cyrillic_to_latin import cyrillic_to_latin
from src.utility.exception import InternalError

from boto3 import client as AWSClient
from botocore.config import Config as AWSConfig

from numpy import ndarray
import threading


class OCRAmazonAWS(IOpticalCharacterRecognition):
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
            region_name=os.environ["AWS_REGION"], # Irland
            signature_version='v4',
            retries = {
                'max_attempts': 5,
                'mode': 'standard'
            }
        )
        with OCRAmazonAWS.boto3_client_lock:
            try:
                self.client = AWSClient(
                    'rekognition',
                    aws_access_key_id=os.environ["AWS_ACCESS_KEY_ID"],
                    aws_secret_access_key=os.environ["AWS_SECRET_ACCESS_KEY"],
                    config=my_config)
            except BaseException as e:
                print("Error: AWS connection - ", e)
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
                ## AWS network call
                try:
                    response = OCRResultCacheManager.load_result(img_bytes, self.config.cache_path)
                    print("Info: OCRAmazonAWS::process_img OCR result loaded from cache")
                except FileNotFoundError:
                    response = self.client.detect_text(Image={'Bytes': img_bytes})
                    OCRResultCacheManager.save_result(img_bytes, response, self.config.cache_path)
                blocks = response['TextDetections']
                ### Format data
                return OCRAmazonAWS.__format_page(blocks, img_path, image), image 
        except BaseException as err:
            print("Error: OCRAmazonAWS::process_img -", err)
            raise err
    
    @staticmethod
    def __format_page(blocks, img_path, image) -> OCRPage:
        textBlockList = []
        width = image.shape[1]
        height = image.shape[0]
        for block in blocks:
            if block['Type'] == 'LINE':
                polygon = OCRAmazonAWS.__format_polygon(block['Geometry']['Polygon'], width, height)
                bounding_box = OCRAmazonAWS.__format_bounding_box(block['Geometry']['BoundingBox'], width, height)
                text = block['DetectedText']
                text = cyrillic_to_latin(text)
                #pivot = Vector2I(polygon[0][0], polygon[0][1])
                #size = Vector2I(polygon[1][0] - polygon[0][0], polygon[3][1] - polygon[0][1])
                area, angle, pivot, size = extract_image_area(polygon, image, True)
                textBlockList.append(OCRBlock(polygon=polygon, text=text, pivot=pivot, size=size, angle=angle))
        return OCRPage(blocks=textBlockList, src_path=img_path)
    
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