from typing import Sequence, Tuple, List
from statistics import mean
import numpy as np

from src.model.model import OCRPage, BlockCluster, OCRBlock, Vector2I, EditConfig
from src.utility.mymath import middle_point, rotate_point

def create_block_cluster(page: OCRPage, search_range: int, search_step: int, style: EditConfig) -> OCRPage:
    """ Create cluster of bounding boxes close from each others on the y axis """
    # Create a list of tuple with a couple (index, block)
    block_buffer = list(page.blocks)
    # Sort by position on Y axis. The research will be done from to top to the bottom.
    block_buffer.sort(key=lambda b : __get_init_point(b.polygon).y, reverse=True)
    page.clusters = []

    while len(block_buffer) > 0:
        block = block_buffer.pop() # get last element
        pack = []
        point_target = __get_init_point(block.polygon)
        pack.append(block)
        # Find lower neighbours:
        pack, block_buffer = __find_neighbour_points(point_target, block_buffer, block, pack, search_range, search_step)
        page.clusters.append(__pack_boxes(pack, style))
    return page

def __find_neighbour_points(
    target_point: Vector2I, 
    block_buffer: List[OCRBlock], 
    block: OCRBlock, 
    pack: List[OCRBlock], 
    search_range: int, 
    search_step: int) -> Tuple[List[OCRBlock], List[OCRBlock]]:
    stop = False
    while not stop:
        stop = True
        # Research for the next poly
        for pt in __browse_line(target_point, search_step, search_range):
            size = len(pack)
            # Search block matching the current point
            for bck in block_buffer:
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
    """ compute the coordinate of a point at the bottom (on Y) and middle (on X) of the polygone """
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
    """ Return the next point, moving delta_y pixels """
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

def __pack_boxes(block_list: Sequence[OCRBlock], style_config: EditConfig) -> BlockCluster:
    polygon = __get_cluster_rect(block_list)
    sentence = __make_sentence(block_list)
    return BlockCluster(
        blocks=block_list, 
        polygon=polygon, 
        sentence=sentence,
        font=style_config.font,
        color=style_config.color,
        line_height=style_config.line_height,
        stroke_width=style_config.stroke_width)

def __get_cluster_rect(block_list: Sequence[OCRBlock]) -> np.array:
    """ 
        Compute a polygon which include all sub-blocks polygons 
        Take into account the angle of every blocks
    """
    pack_points = []
    pack_angle_list = []

    # Compute average angle
    for block in block_list:
        pack_angle_list.append(block.angle)
        for point in block.polygon:
            pack_points.append(point)
    global_rect = __get_global_rect(pack_points)
    avg_angle = mean(pack_angle_list)
    if abs(avg_angle) < 5:
        return global_rect # The polygon is ~horizontal~
    center_coord = middle_point(pack_points)
    for i in range(0, len(pack_points), 1):
        pack_points[i] = rotate_point(pack_points[i], origin=center_coord, degrees=-avg_angle)
    global_rect = __get_global_rect(pack_points)
    for i in range(0, len(global_rect), 1):
        global_rect[i] = np.array(rotate_point(global_rect[i], origin=center_coord, degrees=avg_angle))
    return global_rect

def __get_global_rect(point_list) -> np.array:
    """ compute a polygon which include all sub-blocks polygons """
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
    """ Concat the text of the member blocks to create a sentence """
    buffer = ""
    for block in block_list:
        text = block.text.strip()
        if len(buffer) > 0:
            if buffer[-1] == '-':
                buffer = buffer[:-1]
            else:
                buffer += ' '
        buffer += text.replace("\n", "") # Remove all new line
    return buffer