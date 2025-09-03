import cv2
import numpy as np
import time

def detect_pyzbar(image):
    from pyzbar.pyzbar import decode

    decoded_objects = decode(image)

    texts = []
    boxes = []

    for obj in decoded_objects:
        texts.append(obj.data.decode('utf-8'))
    return texts, boxes

def detect_qreader(image):
    from qreader import QReader
    
    qreader = QReader()
    texts = []
    boxes = []
    detections = qreader.detect(image=image)
    if detections is not None:
        for det in detections:
            decoded_text = qreader.decode(image, det)
            decoded_box = det['bbox_xyxy']
            texts.append(decoded_text)
            boxes.append(decoded_box)
    return texts, boxes

def detect_opencv(image):
    detector = cv2.QRCodeDetector()
    
    decodeed_text, points, _ = detector.detectAndDecode(image)
    texts = [decodeed_text]
    boxes = []
    drawn_img = image.copy() # Create a copy of the image to draw on

    if points is not None:
        print(f"points not none")
        points = points.astype(np.int32)
        for i, qr_points in enumerate(points):
            # The points for one QR code are a 2D array of shape (4, 2).
            # We need to reshape it to the format expected by the decode function.
            qr_points_reshaped = qr_points.reshape((-1, 1, 2))
            # Draw the bounding box on the image (in green, thickness of 2)
            cv2.polylines(drawn_img, [qr_points_reshaped], isClosed=True, color=(0, 255, 0), thickness=2)
            
            # Optional: Add a label number
            centroid = np.mean(qr_points, axis=0).astype(int).flatten()
            cv2.putText(drawn_img, str(i+1), tuple(centroid), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            
    return texts, boxes

# need opencv-contrib-python instead opencv-python
def detect_wechat(image):
    model_dir = "../opencv_3rdparty/" 
    detector = cv2.wechat_qrcode_WeChatQRCode(
        model_dir + "detect.prototxt",
        model_dir + "detect.caffemodel",
        model_dir + "sr.prototxt",
        model_dir + "sr.caffemodel"
    )
    texts = []
    boxes = []
    res, points = detector.detectAndDecode(image)

    print(len(res))
    if len(res) > 0:
        for i, (text, point_set) in enumerate(zip(res, points)):
            texts.append(text)
            boxes.append(point_set)
            
            if point_set.ndim == 3 and point_set.shape[1] == 4 and point_set.shape[2] == 2:
                points_reshaped = point_set.reshape(4, 2).astype(int)
            else:
                points_reshaped = point_set.astype(int)
            
            for j in range(4):
                start_point = tuple(points_reshaped[j])
                end_point = tuple(points_reshaped[(j + 1) % 4])
                cv2.line(image, start_point, end_point, (0, 255, 0), 2)
            
            cv2.putText(image, f"QR: {text}", tuple(points_reshaped[0]),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    return texts, boxes

def unit_test(image, func):
    texts, boxes = func(image)
    print(texts, boxes)

if __name__ == '__main__':
    start_time = time.time()

    # failed
    # unit_test(cv2.imread('../data/1_distort.jpg'), detect_pyzbar)

    # failed
    # unit_test(cv2.imread('../data/1_distort.jpg'), detect_opencv)

    # multi_code success
    # unit_test(cv2.imread('1_distort.jpg'), detect_qreader)
    # unit_test(cv2.imread('../data/2_multicode.png'), detect_qreader)
    # unit_test(cv2.imread('../data/2_multicode_normal.jpg'), detect_qreader)
    # multi_code failed, miss one text 
    # unit_test(cv2.imread('../data/3_selfdefine.jpg'), detect_qreader)
    '''
    [None, 'This is Image 2'] [array([      30.58,      9.8294,      197.52,      171.39], dtype=float32), array([     257.03,      37.019,      388.42,      178.48], dtype=float32)]
    '''

    # multi_code success
    # unit_test(cv2.imread('../data/2_multicode_normal.jpg'), detect_wechat)
    # multi_code failed, only one detect 
    # unit_test(cv2.imread('../data/2_multicode.png'), detect_wechat)
