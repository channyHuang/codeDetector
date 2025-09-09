
import numpy as np

g_apPoints = {
    168: {"x": 0, "y": 0, "w": 20},
    568: {"x": 20, "y": 0, "w": 20}
}

g_family = "tag36h11"
g_width = 1920
g_height = 1080
g_f = g_height / 2
g_camera_matrix = np.array([190.97847715128717, 0, 254.93170605935475,
                   0, 190.9733070521226, 256.8974428996504,
                   0, 0, 1])
g_camera_matrix = g_camera_matrix.reshape(3, 3)
g_dist_coefs = np.array([0.0034823894022493434, 0.0007150348452162257, -0.0020532361418706202, 0.00020293673591811182, 0])
