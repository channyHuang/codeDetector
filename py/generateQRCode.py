import qrcode
import cv2
import numpy as np
import random
from PIL import Image

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

def combineImageVer(img1, img2):
    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]

    new_width = max(w1, w2)
    new_height = h1 + h2

    result = np.zeros((new_height, new_width, 3), dtype=np.uint8)

    result[0:h1, 0:w1] = img1 
    result[h1:h1+h2, 0:w2] = img2  

    cv2.imwrite('combine.jpg', result)
    return result

def create_qr_with_logo(data, logo_path = '../data/UAV.png'):
    qr = qrcode.QRCode(
        error_correction=qrcode.constants.ERROR_CORRECT_H)
    qr.add_data(data)
    qr.make()
    
    qr_img = qr.make_image().convert('RGB')
    
    logo = Image.open(logo_path)
    
    qr_width, qr_height = qr_img.size
    logo_size = qr_width // 4
    
    logo = logo.resize((logo_size, logo_size), Image.Resampling.LANCZOS)
    pos = ((qr_width - logo_size) // 2, (qr_height - logo_size) // 2)
    qr_img.paste(logo, pos)
    
    qr_img = qr_img.rotate(45, expand=True)
    qr_array = np.array(qr_img.convert('RGB'))
    qr_cv = cv2.cvtColor(qr_array, cv2.COLOR_RGB2BGR)

    cv2.imshow('Multiple QR Codes', qr_cv)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return qr_cv

def create_qr(text = "hello, haige"):
    border = 5
    box_size = 1
    qr = qrcode.QRCode(
        version = None,
        box_size = box_size,
        border = border,
        error_correction=qrcode.constants.ERROR_CORRECT_H
        )
    qr.add_data(text)
    qr.make(fit=True)

    qr_img = qr.make_image().convert('RGB')
    qr_array = np.array(qr_img.convert('RGB'))
    qr_cv = cv2.cvtColor(qr_array, cv2.COLOR_RGB2BGR)

    return qr_cv

def create_origin(text):
    border = 5
    box_size = 5
    qr = qrcode.QRCode(
        version = None,
        box_size = box_size,
        border = border,
        error_correction=qrcode.constants.ERROR_CORRECT_H
        )
    qr.add_data(text)
    qr.make(fit=True)

    qr_img = qr.make_image().convert('RGB')
    return qr_img

def create_add_logo(qr_img, logo_path = '../data/UAV.png'):
    if logo_path is not None:
        logo = Image.open(logo_path)
        qr_width, qr_height = qr_img.size
        logo_size = qr_width // 4
        logo = logo.resize((logo_size, logo_size), Image.Resampling.LANCZOS)
        pos = ((qr_width - logo_size) // 2, (qr_height - logo_size) // 2)
        qr_img.paste(logo, pos)

    return qr_img
    
def create_rotate(qr_img, rotate = np.random.randint(0, 360)):
    qr_img = qr_img.rotate(rotate, expand=True)

def create_tocv(qr_img):
    qr_array = np.array(qr_img.convert('RGB'))
    qr_cv = cv2.cvtColor(qr_array, cv2.COLOR_RGB2BGR)

    cv2.imwrite('selfcode.jpg', qr_cv)
    return qr_cv

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

def create(text, rotate = np.random.randint(0, 360), logo_path = '../data/UAV.png'):
    qr_img = create_origin(text)
    qr_img = create_add_logo(qr_img, logo_path)
    
    qr_array = np.array(qr_img.convert('RGB'))
    parallelogram_intensity = 0.2
    qr_array = irregular_parallelogram(qr_array, parallelogram_intensity)
    
    qr_cv = cv2.cvtColor(qr_array, cv2.COLOR_RGB2BGR)
    return qr_cv

if __name__ == '__main__':
    img1 = create("This is a distort image")
    # img2 = create("This is Image 2")
    # combineImageHor(img1, img2)
    cv2.imwrite("1_distort.jpg", img1)
    