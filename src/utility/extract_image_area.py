import cv2
import numpy as np

from src.model import Vector2I
from src.utility.show_debug import show_debug

def __compute_angle(pointl: Vector2I, pointh: Vector2I):
    """ Compute angle with trigonometry (arc sinus) """
    pointm = Vector2I(pointl.x, pointh.y)
    hypotenuse = round(np.sqrt(np.power((pointl.x - pointh.x), 2) + np.power((pointl.y - pointh.y), 2)))
    opposite = round(np.sqrt(np.power((pointl.x - pointm.x), 2) + np.power((pointl.y - pointm.y), 2)))

    angle = np.rad2deg(np.arcsin(opposite / hypotenuse))
    return angle, hypotenuse

def __get_angle_rectangle_horizontal(poly):
    """
        Compute polygon inclination. The larger side will become the horizontal one
        return: angle, pivot, width, height
    """
    # Coordinate analyzis
    point_left = Vector2I(-1, 0)
    point_top = Vector2I(0, -1) 
    point_right = Vector2I(-1, 0) 
    point_bottom = Vector2I(0, -1)
    for coord in poly:
        if point_left.x == -1 or coord[0] < point_left.x:
            point_left = Vector2I.fromarray(coord)
        if point_top.y == -1 or coord[1] < point_top.y:
            point_top = Vector2I.fromarray(coord)
        if point_right.x == -1 or coord[0] > point_right.x:
            point_right = Vector2I.fromarray(coord)
        if point_bottom.y == -1 or coord[1] > point_bottom.y:
            point_bottom = Vector2I.fromarray(coord)
    
    distance_tl = round(np.sqrt(np.power((point_top.x - point_left.x), 2) + np.power((point_left.y - point_top.y), 2)))
    distance_tr = round(np.sqrt(np.power((point_right.x - point_top.x), 2) + np.power((point_right.y - point_top.y), 2)))
    if distance_tl == 0 or distance_tr == 0:
        # Null angle
        pivot = Vector2I(point_left.x, point_top.y)
        width = distance_tl if distance_tl != 0 else distance_tr
        height = round(np.sqrt(np.power((point_bottom.y - point_top.y), 2)))
        return 0, pivot, width, height
    if distance_tl >= distance_tr:
        # inclinaison vers la gauche
        pivot = point_left
        angle, width = __compute_angle(point_left, point_top)
        return -angle, pivot, width, distance_tr
    else:
        # inclinaison vers la droite
        pivot = point_top
        angle, width = __compute_angle(point_right, point_top)
        return angle, pivot, width, distance_tl

def __get_angle_upper_points(poly):
    """
        Compute polygon inclination. The upper vertex will become the horizontal one
        return: angle, pivot, width, height
    """
    poly_sort_y = poly[np.lexsort((poly[:,0],poly[:,1]))]
    point_top = Vector2I.fromarray(poly_sort_y[0])
    point_top2 = Vector2I.fromarray(poly_sort_y[1])
    point_bot = Vector2I.fromarray(poly_sort_y[3])

    angle, width = __compute_angle(point_top2, point_top)
    height = round(np.sqrt(np.power(abs(point_bot.x - point_top2.x), 2) + np.power((point_bot.y - point_top2.y), 2)))
    if point_top.x < point_top2.x:
        pivot = point_top
        angle = -angle
    else:
        pivot = point_top2
    return angle, pivot, width, height

def extract_image_area(poly, image, is_rectangle=True):
    """ Extract a segment delimited by a polygon in an image """
    shape = image.shape[:2]

    # calcul angle
    if is_rectangle:
        (angle, pivot, width, height) = __get_angle_rectangle_horizontal(poly)
    else:
        (angle, pivot, width, height) = __get_angle_upper_points(poly)
    # rotate image
    if angle != 0:
        rotate_matrix = cv2.getRotationMatrix2D(center=pivot.data(), angle=-angle, scale=1)
        rotated_image = cv2.warpAffine(src=image, M=rotate_matrix, dsize=(shape[1], shape[0]))
    else:
        rotated_image = image
    # crop image
    rotated_area = rotated_image[pivot.y:pivot.y+height, pivot.x:pivot.x+width]
    return rotated_area, angle, pivot, Vector2I(width, height)
