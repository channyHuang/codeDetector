import cv2
import time

def detect_apriltag(image):
    ap = True
    if ap == True:
        # python3 -m pip install pupil_apriltags -i Https://pypi.tuna.tsinghua.edu.cn/simple/
        import pupil_apriltags as apriltag
    else:
        # python3 -m pip install apriltag -i Https://pypi.tuna.tsinghua.edu.cn/simple/
        import apriltag as apriltag

    detector = None
    if ap == True:
        detector = apriltag.Detector(families='tag36h11')
    else:
        detector = apriltag.Detector(apriltag.DetectorOptions(families='tag36h11'))

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    results = detector.detect(gray)
    ids = []
    corners = []
    for r in results:
        tagID = str(r.tag_id)
        ids.append(tagID)
        b = (tuple(r.corners[0].astype(int))[0], tuple(r.corners[0].astype(int))[1])
        c = (tuple(r.corners[1].astype(int))[0], tuple(r.corners[1].astype(int))[1])
        d = (tuple(r.corners[2].astype(int))[0], tuple(r.corners[2].astype(int))[1])
        a = (tuple(r.corners[3].astype(int))[0], tuple(r.corners[3].astype(int))[1])
        corners.append(a)        
        cv2.line(image, a, b, (255, 0, 255), 2, lineType=cv2.LINE_AA)
        cv2.line(image, b, c, (255, 0, 255), 2, lineType=cv2.LINE_AA)
        cv2.line(image, c, d, (255, 0, 255), 2, lineType=cv2.LINE_AA)
        cv2.line(image, d, a, (255, 0, 255), 2, lineType=cv2.LINE_AA)

    return image, ids, corners

def detect_aruco(image):
    import cv2.aruco as aruco

    # Convert to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # Define the ArUco dictionary
    aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_APRILTAG_36H11)
    
    # Initialize detector parameters
    parameters = aruco.DetectorParameters()
    
    # Create detector
    detector = aruco.ArucoDetector(aruco_dict, parameters)
    
    # Detect markers (NEW API)
    corners, ids, rejected = detector.detectMarkers(gray)
    
    # Draw detected markers
    if ids is not None:
        image = aruco.drawDetectedMarkers(image, corners, ids)
        print(f"Detected marker IDs: {ids.flatten()}")
    
    return image, ids, corners

def unit_test(image, func):
    image, ids, corners = func(image)
    print('Detect ids: ', ids)

if __name__ == '__main__':
    start_time = time.time()

    image = cv2.imread('../data/tag/tag36_11_00568.png')
    image = cv2.resize(image, (40, 40))  
    unit_test(image, detect_aruco)

    image = cv2.imread('../data/tag/tag36_11_00568.png')
    image = cv2.resize(image, (40, 40))  
    unit_test(image, detect_apriltag)
