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

    # Build suppression mask
    for block in textBlockList:
        if block.str == "":
            continue
        box = block.bbox
        poly = np.array(box[1]).astype(np.int32)
        cv2.fillPoly(mask, [poly.reshape((-1, 1, 2))], color=(255, 255, 255))
    gray = cv2.bitwise_not(gray)
    mask = cv2.bitwise_and(gray, gray, mask=mask)
    # Apply mask on image
    return cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)