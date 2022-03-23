from typing import Sequence
import cv2
import numpy as np
from src.model import BlockCluster, OCRBlock

def draw_bouncing_box(img, blocks: Sequence[OCRBlock], color=(0, 0, 255), thickness=1):
    """ save text detection result one by one
    Args:
        img (array): raw image context
        boxes (array): array of result file
            Shape: [num_detections, 4] for BB output / [num_detections, 4] for QUAD output
    Return:
        None
    """
    for block in blocks:
        poly = block.polygon
        cv2.polylines(img, [poly.reshape((-1, 1, 2))], True, color=color, thickness=thickness)
    return img

def draw_cluster_box(img, blocks: Sequence[BlockCluster], color=(0, 255, 0), thickness=1):
    for block in blocks:
        poly = []
        for point in block.polygon:
            poly.append(np.array([point.x, point.y]))
        poly = np.array(poly)
        cv2.polylines(img, [poly.reshape((-1, 1, 2))], True, color=color, thickness=thickness)
    return img