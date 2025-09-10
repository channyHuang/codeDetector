import cv2
import os

class ApriltagDetector:
    def __init__(self) -> None:
        self.ap = True
        if self.ap == True:
            # python3 -m pip install pupil_apriltags -i Https://pypi.tuna.tsinghua.edu.cn/simple/
            import pupil_apriltags as apriltag
        else:
            # python3 -m pip install apriltag -i Https://pypi.tuna.tsinghua.edu.cn/simple/
            import apriltag as apriltag

        self.detector = None
        if self.ap == True:
            self.detector = apriltag.Detector(families='tag36h11',
                                              nthreads = 1,
                                              quad_decimate=1.6, # 2.0
                                              quad_sigma=2.0, # 0.0
                                              refine_edges=True, # True
                                              decode_sharpening = 0.5,# 0.25
                                              debug = False
                                              )
        else:
            self.detector = apriltag.Detector(apriltag.DetectorOptions(families='tag36h11'))

    def detect_frame(self, image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        results = self.detector.detect(gray)
        ids = []
        corners = []
        for r in results:
            tagID = str(r.tag_id)
            ids.append(tagID)

            b = (tuple(r.corners[0].astype(int))[0], tuple(r.corners[0].astype(int))[1])
            c = (tuple(r.corners[1].astype(int))[0], tuple(r.corners[1].astype(int))[1])
            d = (tuple(r.corners[2].astype(int))[0], tuple(r.corners[2].astype(int))[1])
            a = (tuple(r.corners[3].astype(int))[0], tuple(r.corners[3].astype(int))[1])
            corners.append({"ld": b, "rd": c, "ru": d, "lu": a})        
            cv2.line(image, a, b, (255, 0, 255), 2, lineType=cv2.LINE_AA)
            cv2.line(image, b, c, (255, 0, 255), 2, lineType=cv2.LINE_AA)
            cv2.line(image, c, d, (255, 0, 255), 2, lineType=cv2.LINE_AA)
            cv2.line(image, d, a, (255, 0, 255), 2, lineType=cv2.LINE_AA)
        return ids, corners, image

if __name__ == '__main__':
    folder = '../data/tag/frame_SwinIR'

    detector = ApriltagDetector()

    image_name_list = os.listdir(folder)
    total_image = len(image_name_list)
    count = 0
    try:
        for image_name in image_name_list:
            full_name = os.path.join(folder, image_name)
            image = cv2.imread(full_name)

            ids, corners, image_display = detector.detect_frame(image)
            if ids is not None and len(ids) > 0:
                count += 1
    except Exception as e:
        print(e)

    print(f"Detect {count} / {total_image}, rate = {count / total_image}")