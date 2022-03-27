from typing import Sequence
from statistics import mean
import numpy as np

from src.model import AppConfig, OCRPage, BlockCluster, OCRBlock, Vector2I
from src.utility.mymath import middle_point, rotate_point

def create_block_cluster(page: OCRPage, config: AppConfig) -> OCRPage:
    """ Create cluster of bounding boxes close from each others on the y axis """
    block_buffer = list(page.blocks)
    page.clusters = []
    search_range = config.box_cluster_search_range
    search_step = config.box_cluster_search_step

    while len(block_buffer) > 0:
        block = block_buffer[0]
        pack = []
        point_target = __get_init_point(block.polygon)
        pack.append(block)
        block_buffer.remove(block)
        # Process lower boxes
        pack, block_buffer = __find_neightbour_points(point_target, block_buffer, block, pack, search_range, search_step)
        page.clusters.append(__pack_boxes(pack))
    return page

def __find_neightbour_points(target_point, block_buffer, block, pack, search_range, search_step):
    stop = False
    while not stop:
        stop = True
        # Research for the next poly
        for pt in __browse_line(target_point, search_step, search_range):
            size = len(pack)
            # Search block which match the current point
            for bck in list(block_buffer):
                if bck != block and __check_collide(bck.polygon, pt):
                    pack.append(bck)
                    target_point = __get_init_point(bck.polygon)
                    block_buffer.remove(bck)
                    break
            # A block has been found
            if len(pack) != size:
                stop = False
                break
    return pack, block_buffer

def __get_init_point(poly: np.array) -> Vector2I:
    if len(poly) != 4:
        print("__get_init_point: warning wrong polygon size")
    data = sorted(poly.tolist(), key=lambda point: point[0], reverse=True)
    right = [data[0], data[1]]
    left = [data[2], data[3]]
    right = sorted(right, key=lambda point: point[1], reverse=True)
    left = sorted(left, key=lambda point: point[1], reverse=True)
    point1 = right[0]
    point2 = left[0]
    max_y = Vector2I(int((point1[0] + point2[0]) / 2), int((point1[1] + point2[1]) / 2))
    return max_y

def __browse_line(point: Vector2I, delta_y, end_y) -> Vector2I:
    if end_y < 0: # go upper
        end_y += point.y
        while point.y - delta_y >= end_y:
            point.y -= delta_y
        yield point
    else: # go lower
        end_y += point.y
        while point.y + delta_y <= end_y:
            point.y += delta_y
            yield point

def __check_collide(poly, point) -> bool:
    """ check point collision in polygon : Jordan curve theorem. """
    poly_size = len(poly)
    j = poly_size - 1
    inside = False
    for i in range(0, poly_size, 1):
        if (poly[i][1] > point.y) != (poly[j][1] > point.y) and \
            (point.x < (poly[j][0] - poly[i][0]) * (point.y - poly[i][1]) / (poly[j][1] - poly[i][1]) + poly[i][0]):
            inside = not inside
        j = i
    return inside

def __pack_boxes(block_list: Sequence[OCRBlock]) -> BlockCluster:
    polygon = __get_cluster_rect(block_list)
    sentence = __make_sentence(block_list)
    return BlockCluster(block_list, polygon, sentence)

def __get_cluster_rect(block_list: Sequence[OCRBlock]) -> Sequence[Vector2I]:
    pack_points = []
    pack_angle_list = []

    for block in block_list:
        pack_angle_list.append(block.angle)
        for point in block.polygon:
            pack_points.append(point)
    global_rect = __get_global_rect(pack_points)
    avg_angle = mean(pack_angle_list)
    if abs(avg_angle) < 5:
        return global_rect # The polygon is ~~horizontal
    center_coord = middle_point(pack_points)
    for i in range(0, len(pack_points), 1):
        pack_points[i] = rotate_point(pack_points[i], origin=center_coord, degrees=-avg_angle)
    global_rect = __get_global_rect(pack_points)
    for i in range(0, len(global_rect), 1):
        global_rect[i] = np.array(rotate_point(global_rect[i], origin=center_coord, degrees=avg_angle))
    return global_rect

def __get_global_rect(point_list) -> Sequence[Vector2I]:
    list_x = []
    list_y = []
    for point in point_list:
        list_x.append(point[0])
        list_y.append(point[1])
    min_x = min(list_x)
    min_y = min(list_y)
    max_x = max(list_x)
    max_y = max(list_y)
    return np.array([
        [int(min_x), int(max_y)], # top left
        [int(max_x), int(max_y)], # top right
        [int(max_x), int(min_y)], # bottom right
        [int(min_x), int(min_y)], # bottom left
    ])

def __make_sentence(block_list: Sequence[OCRBlock]) -> str:
    buffer = ""
    for block in block_list:
        text = block.text.strip()
        if len(buffer) > 0:
            if buffer[-1] == '-':
                buffer = buffer[:-1]
            else:
                buffer += ' '
        buffer += text
    return buffer