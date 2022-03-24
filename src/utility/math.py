import numpy as np

def rotate_point(p, origin=(0, 0), degrees=0):
    """ rotate point """
    angle = np.deg2rad(degrees)
    R = np.array([[np.cos(angle), -np.sin(angle)],
                  [np.sin(angle),  np.cos(angle)]])
    o = np.atleast_2d(origin)
    p = np.atleast_2d(p)
    return np.squeeze((R @ (p.T-o.T) + o.T).T)

def middle_point(point_list):
    x = 0
    y = 0
    size = len(point_list)
    for point in point_list:
        x += point[0]
        y += point[1]
    return (int(x / size), int(y / size))