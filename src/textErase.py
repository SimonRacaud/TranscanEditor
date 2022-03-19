from typing import Sequence
import cv2
import numpy as np

from src.model import TextBlock

def inverte(imagem, name):
    imagem = (255-imagem)
    cv2.imwrite(name, imagem)

def textErase(image, textBlockList: Sequence[TextBlock]):
    mask = np.zeros(image.shape[:2], dtype="uint8")
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    for block in textBlockList:
        if block.str == "":
            continue
        box = block.bbox
        poly = np.array(box).astype(np.int32)
        area = gray[poly[0][1]:poly[2][1], poly[0][0]:poly[2][0]]
        if area == []:
            continue
        area = cv2.bitwise_not(area)
        try:
            mask[poly[0][1]:poly[2][1], poly[0][0]:poly[2][0]] = area
        except:
            print(area) # TODO : temp
    return cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)