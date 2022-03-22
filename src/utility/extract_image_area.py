from typing import Sequence
import cv2
import numpy as np

from src.model import Vector2I

def get_angle(poly):
    # Coordinate analyzis
    point_left = Vector2I(-1, 0) # point to the extreme left
    point_top = Vector2I(0, -1) # point on the top
    point_bot = Vector2I(0, -1) # point on the bottom
    for coord in poly:
        if point_left.x == -1 or coord[0] < point_left.x:
            point_left = Vector2I.fromarray(coord)
        if point_top.y == -1 or coord[1] < point_top.y:
            point_top = Vector2I.fromarray(coord)
        if point_bot.y == -1 or coord[1] > point_bot.y:
            point_bot = Vector2I.fromarray(coord)
    horizontal_len = round(np.sqrt(np.power((point_top.x - point_left.x), 2) + np.power((point_left.y - point_top.y), 2)))
    vertical_len = round(np.sqrt(np.power((point_bot.x - point_left.x), 2) + np.power((point_bot.y - point_left.y), 2)))
    max_x = max(poly[:,0])
    # Compute angle
    if horizontal_len == 0 or horizontal_len > vertical_len:
        adjacent = point_top.x - point_left.x
        opposite = point_left.y - point_top.y
        pivot = Vector2I(point_left.x, point_left.y)
        if adjacent == 0:
            return (0, Vector2I(point_left.x, point_top.y), int(max_x - point_left.x), int(point_bot.y - point_top.y))
        angle = np.arctan(opposite / adjacent) * (180/np.pi)
        return (-angle, pivot, horizontal_len, vertical_len)
    else:
        adjacent = point_bot.x - point_left.x # horizontal axis
        opposite = point_bot.y - point_left.y # vertical axis
        pivot = Vector2I(point_top.x, point_top.y)
        if adjacent == 0:
            return (0, Vector2I(point_left.x, point_top.y), int(point_bot.y - point_top.y), int(max_x - point_left.x))
        angle = np.arctan(opposite / adjacent) * (180/np.pi)
        return (angle, pivot, vertical_len, horizontal_len)

def extract_image_area(poly, image):
    shape = image.shape[:2]

    # Extract text segment
    mask = np.zeros(shape, dtype="uint8")
    cv2.fillPoly(mask, pts=[poly], color=(255, 255, 255))
    area = cv2.bitwise_and(image, image, mask=mask)
    # calcul angle
    (angle, pivot, width, height) = get_angle(poly)
    # rotate image
    rotate_matrix = cv2.getRotationMatrix2D(center=pivot.data(), angle=angle, scale=1)
    rotated_image = cv2.warpAffine(src=area, M=rotate_matrix, dsize=(shape[1], shape[0]))
    # crop image
    rotated_area = rotated_image[pivot.y:pivot.y+height, pivot.x:pivot.x+width]
    return rotated_area, angle, pivot, Vector2I(width, height)
