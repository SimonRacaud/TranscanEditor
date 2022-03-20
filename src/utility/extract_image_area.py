import cv2
import numpy as np

def get_angle(poly):
    # Coordinate analyzis
    point_left = (-1, 0) # point to the extreme left
    point_top = (0, -1) # point on the top
    point_bot = (0, -1) # point on the bottom
    for coord in poly:
        # nota: coord[0] => coord->x // coord[1] => coord->y
        if point_left[0] == -1 or coord[0] < point_left[0]:
            point_left = coord
        if point_top[1] == -1 or coord[1] < point_top[1]:
            point_top = coord
        if point_bot[1] == -1 or coord[1] > point_bot[1]:
            point_bot = coord
    horizontal_len = round(np.sqrt(np.power((point_top[0] - point_left[0]), 2) + np.power((point_left[1] - point_top[1]), 2)))
    vertical_len = round(np.sqrt(np.power((point_bot[0] - point_left[0]), 2) + np.power((point_bot[1] - point_left[1]), 2)))
    max_x = max(poly[:,0])
    # Compute angle
    if horizontal_len == 0 or horizontal_len > vertical_len:
        adjacent = point_top[0] - point_left[0]
        opposite = point_left[1] - point_top[1]
        pivot = (point_left[0], point_left[1])
        if adjacent == 0:
            return (0, pivot, (max_x - point_left[0]), (point_bot[1] - point_top[1]))
        angle = np.arctan(opposite / adjacent) * (180/np.pi)
        return (-angle, pivot, horizontal_len, vertical_len)
    else:
        adjacent = point_bot[0] - point_left[0] # horizontal axis
        opposite = point_bot[1] - point_left[1] # vertical axis
        pivot = (point_top[0], point_top[1])
        if adjacent == 0:
            return (0, pivot, (point_bot[1] - point_top[1]), (max_x - point_left[0]))
        angle = np.arctan(opposite / adjacent) * (180/np.pi)
        return (-angle, pivot, vertical_len, horizontal_len)

def extract_image_area(poly, image):
    shape = image.shape[:2]

    mask = np.zeros(shape, dtype="uint8")
    cv2.fillPoly(mask, [poly.reshape((-1, 1, 2))], color=(255, 255, 255))
    area = cv2.bitwise_and(image, image, mask=mask)
    # calcul angle
    (angle, pivot, width, height) = get_angle(poly)
    # rotate image
    rotate_matrix = cv2.getRotationMatrix2D(center=pivot, angle=angle, scale=1)
    rotated_image = cv2.warpAffine(src=area, M=rotate_matrix, dsize=(shape[1], shape[0]))
    #if angle != 0:
    # print(pivot, width, height, angle)
    # result = rotated_image[pivot[1]:pivot[1]+height, pivot[0]:pivot[0]+width]
    # cv2.imshow("area", cv2.resize(area, (562, 800)))
    # cv2.imshow("rotate", cv2.resize(rotated_image, (562, 800)))
    # cv2.waitKey(0)
    # cv2.imshow("test", result)
    # cv2.waitKey(0)
    # crop image
    return rotated_image[pivot[1]:pivot[1]+height, pivot[0]:pivot[0]+width]
