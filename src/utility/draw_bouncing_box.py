from typing import Sequence
import cv2
from src.model import OCRBlock

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
        # Save result image
        return img