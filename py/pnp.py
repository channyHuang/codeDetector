import cv2
import numpy as np
import os

from config import *
from ArucoDetector import ArucoDetector
from ApriltagDetector import ApriltagDetector

def prepare(ids, corners):
    data_size = len(ids)
    points_2d = np.zeros([4 * data_size, 2], dtype=np.double)
    points_3d = np.zeros([4 * data_size, 3], dtype=np.double)
    index = 0

    for id, corner in zip(ids, corners):
        if id in g_apPoints:
            qrpos = g_apPoints[id]

            points_2d[0 + (index << 2), :] = corner["ld"]
            points_2d[1 + (index << 2), :] = corner["rd"]
            points_2d[2 + (index << 2), :] = corner["ru"]
            points_2d[3 + (index << 2), :] = corner["lu"]

            offset = qrpos["w"] / 2

            points_3d[0 + (index << 2), 0] = qrpos["x"] - offset
            points_3d[0 + (index << 2), 1] = -qrpos["y"] + offset
            points_3d[1 + (index << 2), 0] = qrpos["x"] + offset
            points_3d[1 + (index << 2), 1] = -qrpos["y"] + offset
            points_3d[2 + (index << 2), 0] = qrpos["x"] + offset
            points_3d[2 + (index << 2), 1] = -qrpos["y"] - offset
            points_3d[3 + (index << 2), 0] = qrpos["x"] - offset
            points_3d[3 + (index << 2), 1] = -qrpos["y"] - offset

            index += 1

    return points_2d, points_3d

def pnp(points_2d, points_3d):

    found, rvec, t_camera = cv2.solvePnP(points_3d, points_2d, g_camera_matrix, g_dist_coefs
                                         , flags = cv2.SOLVEPNP_IPPE_SQUARE
                                        #  , flags = cv2.SOLVEPNP_IPPE
                                        )
    print('found= ', found, rvec, t_camera)


if __name__ == '__main__':
    image_name = '../data/tag/frame_SwinIR/frame0016.jpg_SwinIR.png'

    frame = cv2.imread(image_name)
    detector = ArucoDetector()
    ids, corners, frame_display = detector.detect_frame(frame)
    
    # cv2.imshow('res', frame_display)
    # cv2.waitKey(1000)
    # cv2.imwrite('res.png', frame_display)
    points_2d, points_3d = prepare(ids, corners)
    pnp(points_2d, points_3d)
