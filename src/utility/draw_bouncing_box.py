import cv2
import numpy as np

def draw_bouncing_box(img, boxes, color=(0, 0, 255), thickness=1):
        """ save text detection result one by one
        Args:
            img (array): raw image context
            boxes (array): array of result file
                Shape: [num_detections, 4] for BB output / [num_detections, 4] for QUAD output
        Return:
            None
        """
        for box in enumerate(boxes):
            poly = np.array(box[1]).astype(np.int32).reshape((-1))
            poly = poly.reshape(-1, 2)
            cv2.polylines(img, [poly.reshape((-1, 1, 2))], True, color=color, thickness=thickness)
        # Save result image
        return img