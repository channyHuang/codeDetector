import cv2
import numpy as np
import random

def combineImageHor(img1, img2):
    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]

    new_height = max(h1, h2)
    new_width = w1 + w2

    result = np.zeros((new_height, new_width, 3), dtype=np.uint8)

    result[0:h1, 0:w1] = img1 
    result[0:h2, w1:w1+w2] = img2  

    cv2.imwrite('combine.jpg', result)
    return result

def irregular_parallelogram(image, intensity=0.2):
    """
    Create an irregular parallelogram distortion
    """
    h, w = image.shape[:2]
    
    # Source points (original image corners)
    src_points = np.float32([[0, 0], [w, 0], [w, h], [0, h]])
    
    # Calculate maximum displacement based on intensity
    max_displacement_x = int(w * intensity)
    max_displacement_y = int(h * intensity)
    
    # Create irregular parallelogram by shifting points randomly
    # Ensure it remains a parallelogram (opposite sides remain parallel)
    shift_x1 = random.randint(-max_displacement_x, max_displacement_x)
    shift_y1 = random.randint(-max_displacement_y, max_displacement_y)
    
    shift_x2 = random.randint(-max_displacement_x, max_displacement_x)
    shift_y2 = random.randint(-max_displacement_y, max_displacement_y)
    
    # Destination points for irregular parallelogram
    dst_points = np.float32([
        [0 + shift_x1, 0 + shift_y1],                      # Top-left
        [w + shift_x2, 0 + shift_y2],                      # Top-right
        [w - shift_x1, h - shift_y1],                      # Bottom-right (maintain parallelogram property)
        [0 - shift_x2, h - shift_y2]                       # Bottom-left (maintain parallelogram property)
    ])
    
    # Alternative: More irregular version (not strictly parallelogram)
    if random.random() > 0.5:
        dst_points = np.float32([
            [random.randint(-max_displacement_x, max_displacement_x), 
             random.randint(-max_displacement_y, max_displacement_y)],
            [w + random.randint(-max_displacement_x, max_displacement_x), 
             random.randint(-max_displacement_y, max_displacement_y)],
            [w + random.randint(-max_displacement_x, max_displacement_x), 
             h + random.randint(-max_displacement_y, max_displacement_y)],
            [random.randint(-max_displacement_x, max_displacement_x), 
             h + random.randint(-max_displacement_y, max_displacement_y)]
        ])
    
    # Get perspective transform matrix
    matrix = cv2.getPerspectiveTransform(src_points, dst_points)
    
    # Apply the transformation
    distorted_image = cv2.warpPerspective(image, matrix, (w, h), 
                                        borderMode=cv2.BORDER_CONSTANT, 
                                        borderValue=(255, 255, 255))
    
    distorted_image = cv2.copyMakeBorder(distorted_image, 10, 10, 10, 10, cv2.BORDER_CONSTANT, value=(255, 255, 255))  # add border
    
    return distorted_image

if __name__ == '__main__':
    img1 = cv2.imread('./../data/tag/tag36_11_00168.png')
    img2 = cv2.imread('./../data/tag/tag36_11_00568.png')

    height, width = img1.shape[0], img1.shape[1]
    scale = 5
    img1 = cv2.resize(img1, (width * scale, height * scale))
    img2 = cv2.resize(img2, (width * scale, height * scale))

    parallelogram_intensity = 0.2
    img1 = irregular_parallelogram(img1, parallelogram_intensity)
    img2 = irregular_parallelogram(img2, parallelogram_intensity)

    combineImageHor(img1, img2)