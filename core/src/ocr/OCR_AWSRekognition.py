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


class OCR_AWSRekognition(IOpticalCharacterRecognition):
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
        with OCR_AWSRekognition.boto3_client_lock:
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
                result=[]
                try:
                    result = OCRResultCacheManager.load_result(img_bytes, self.config.cache_path)
                    done = True
                    print("Info: OCRAmazonAWS::process_img OCR result loaded from cache")
                except FileNotFoundError:
                    result = self.__process_through_api(img, img_bytes)
                ### Format data
                return OCR_AWSRekognition.__format_page(result, img_path, image), image 
        except BaseException as err:
            print("Error: OCRAmazonAWS::process_img -", err)
            raise err
        
    def __process_through_api(self, img, img_bytes):
        imgBytes2 = img_bytes
        maxIteration = 4 # Max number of time an image can be split
        imgSlice = img
        done = False
        counter = 0
        result = []
        deltaY = 0 # Cursor on Y where the image has been split
        while (not done) and (counter < maxIteration):
            print("(info) OCR : Image processing iteration ", counter)
            ## AWS network call
            response = self.client.detect_text(Image={'Bytes': imgBytes2})
            blocks = response['TextDetections']
            done = not OCR_AWSRekognition.__check_word_limit(blocks)
            blocks = OCR_AWSRekognition.__filter_low_confidence_blocks(blocks)
            if not done:
                # The API reached the limitation of 100 words, image split needed
                imgSlice, deltaY = OCR_AWSRekognition.__split_image(img, blocks, deltaY)
                # Convert image slice to an array of bytes
                imgBytes2 = io.BytesIO()
                imgSlice.save(imgBytes2, format='PNG')
                imgBytes2 = imgBytes2.getvalue()
            result = OCR_AWSRekognition.__merge_result(result, blocks, deltaY, imgSlice.size[1], img.size)
            if done:
                OCRResultCacheManager.save_result(img_bytes, result, self.config.cache_path)
            counter += 1
        return result
    
    @staticmethod
    def __filter_low_confidence_blocks(blocks):
        return list(filter(lambda b : b['Confidence'] >= 40.0, blocks))

    @staticmethod
    def __merge_result(blocks1, blocks2, deltaY: int, imgSliceHeight: int, imgSize):
        """ Merge two API results """ 
        result=blocks1
        
        if blocks1 == []:
            return blocks2
        
        def filterResult(block):
            if block['Type'] == 'LINE':
                bPoly = block['Geometry']['Polygon']
                poly = OCR_AWSRekognition.__format_polygon(bPoly, imgSize[0], imgSize[1])
                for pt in poly:
                    if pt[1] > deltaY:
                        return False
            else:
                return False
            return True
        result = list(filter(filterResult, result))

        for b in blocks2:
            if b['Type'] == 'LINE':
                bPoly = b['Geometry']['Polygon']
                # move on Y (to make the coordinates relative to the main image, not the slice)
                for i in range(0, len(bPoly)):
                    bPoly[i]['Y'] = (bPoly[i]['Y'] * imgSliceHeight + deltaY) / imgSize[1]
                #
                result.append(b)
        return result

    @staticmethod
    def __split_image(img,  blocks, deltaY: int):
        width, height = img.size # Size of the whole image
        # Get previous detection limit
        maxY=0
        for b in blocks:
            if b['Type'] == 'LINE':
                bbox = b['Geometry']['BoundingBox']
                maxY = max(maxY, bbox['Top'])
        maxY = int(maxY * height) # convert from fraction to absolute size
        if maxY > 50:
            maxY -= 50 # security margin
        # Splitting image horizontally
        subImg = img.crop((0, maxY, width, height))
        return subImg, (maxY + deltaY)

    @staticmethod
    def __check_word_limit(blocks) -> bool:
        """ AWS rekognition have a limit of 100 words per page """
        counter=0
        for b in blocks:
            if b['Type'] == 'WORD':
                counter += 1
        return counter == 100

    @staticmethod
    def __format_page(blocks, img_path, image) -> OCRPage:
        textBlockList = []
        width = image.shape[1]
        height = image.shape[0]
        for block in blocks:
            if block['Type'] == 'LINE':
                polygon = OCR_AWSRekognition.__format_polygon(block['Geometry']['Polygon'], width, height)
                #bounding_box = OCRAmazonAWS.__format_bounding_box(block['Geometry']['BoundingBox'], width, height)
                text = block['DetectedText']
                text = cyrillic_to_latin(text)
                area, angle, pivot, size = extract_image_area(polygon, image, True) # TODO: to simplify ?
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