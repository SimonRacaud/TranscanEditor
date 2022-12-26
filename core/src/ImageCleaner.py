from typing import Sequence
import cv2
import numpy as np

from src.model.model import OCRBlock, BlockCluster
from src.utility.mymath import order_points

from src.utility.show_debug import show_debug

class ImageCleaner:
    @classmethod
    def process(cls, image, clusterList: Sequence[BlockCluster]):
        mask = np.zeros(image.shape[:2], dtype="uint8")
        # Build suppression mask
        for cluster in clusterList:
            if cluster.cleanBox == False:
                continue # Skip those blocks
            for block in cluster.blocks:
                if block.text == "":
                    continue # Skip this block
                distendedPoly = ImageCleaner.__poly_distension(block.polygon, block.angle, 3)
                cv2.fillPoly(mask, [distendedPoly], color=(255, 255, 255))
        # Apply mask on image
        return cv2.inpaint(image, mask, 7, cv2.INPAINT_NS)
    
    @staticmethod
    def __poly_distension(poly, angle, shift):
        if abs(angle) > 2:
            return poly
        poly_sort = poly[np.lexsort((poly[:,0],poly[:,1]))]
        # Lower vertex
        poly_sort[2][1] += shift
        poly_sort[3][1] += shift
        poly_sort = poly_sort[np.lexsort((poly_sort[:,1],poly_sort[:,0]))]
        # Vertex on the left
        poly_sort[2][0] += shift
        poly_sort[3][0] += shift
        return order_points(poly_sort)
