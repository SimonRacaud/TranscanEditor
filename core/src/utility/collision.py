from src.model.model import Vector2I

def check_collide_point_poly(poly, point: Vector2I) -> bool:
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

def check_collide_poly_poly(poly1, poly2):
    """ Check partial collision between two polygon """
    for p in poly1:
        if check_collide_point_poly(p, poly2):
            return True
    return False