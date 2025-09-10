import cv2
import os
import numpy as np

class ArucoDetector:
    def __init__(self) -> None:
        pass

    @staticmethod
    def outputArucoParam():
        import cv2; [print(f"{attr}: {getattr(cv2.aruco.DetectorParameters(), attr)}") for attr in dir(cv2.aruco.DetectorParameters()) if not attr.startswith('_')]

    @staticmethod
    def create_custom_aruco_detector():
        # ==================== 1. 选择字典 ====================
        dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_APRILTAG_36H11)
        
        # ==================== 2. 自定义检测器参数 ====================
        detector_params = cv2.aruco.DetectorParameters()
        # outputArucoParam()
        # 自适应阈值参数 - 处理光照问题
        detector_params.adaptiveThreshWinSizeMin = 5      # 最小窗口大小（必须是奇数且>=3） 3
        detector_params.adaptiveThreshWinSizeMax = 23     # 最大窗口大小（奇数）23
        detector_params.adaptiveThreshWinSizeStep = 10     # 窗口大小步长 10
        detector_params.adaptiveThreshConstant = 7        # 从平均值中减去的常数 7.0
        
        # 标记尺寸过滤参数
        detector_params.minMarkerPerimeterRate = 0.03     # 最小周长比率（检测更小的标记）0.03
        detector_params.maxMarkerPerimeterRate = 4.0      # 最大周长比率（检测更大的标记）4.0
        detector_params.minMarkerDistanceRate = 0.125      # 标记间最小距离比率 0.125
        detector_params.minCornerDistanceRate = 0.05      # 角点间最小距离比率 0.05
        
        # 轮廓处理参数
        detector_params.polygonalApproxAccuracyRate = 0.03  # 多边形逼近精度（值越小越精确） 0.03
        detector_params.minDistanceToBorder = 3           # 到图像边界的最小距离 3
        
        # 角点细化参数 - 提高角点精度
        detector_params.cornerRefinementMethod = cv2.aruco.CORNER_REFINE_SUBPIX  # 使用亚像素细化 
        detector_params.cornerRefinementWinSize = 5       # 细化窗口大小 5
        detector_params.cornerRefinementMaxIterations = 30 # 最大迭代次数 30
        detector_params.cornerRefinementMinAccuracy = 0.1 # 最小精度要求 0.1
        
        # 标记分析参数
        detector_params.markerBorderBits = 1              # 标记边框位数 1
        detector_params.perspectiveRemovePixelPerCell = 40 # 内部采样分辨率 4
        detector_params.perspectiveRemoveIgnoredMarginPerCell = 0.15 # 忽略的边界比例 0.13
        
        # 错误校正参数
        detector_params.errorCorrectionRate = 0.8         # 纠错率（0-1） 0.6
        
        # 检测模式参数
        detector_params.useAruco3Detection = True        # 使用 ArUco3 检测模式 False
        detector_params.minSideLengthCanonicalImg = 32   # 规范图像的最小边长 32
        detector_params.minMarkerLengthRatioOriginalImg = 0.01  # 标记长度与原图比例 0
        
        # ==================== 3. 自定义校正参数 ====================
        refine_params = cv2.aruco.RefineParameters()
        refine_params.minRepDistance = 10.0               # 重复标记的最小距离
        refine_params.errorCorrectionRate = 3.0          # 校正阶段的纠错率
        refine_params.checkAllOrders = True              # 尝试所有角点顺序
        
        # ==================== 4. 创建检测器 ====================
        aruco_detector = cv2.aruco.ArucoDetector(
            dictionary, 
            detector_params, 
            refine_params
        )
        
        return aruco_detector

    @staticmethod
    def detect_folder(folder = '../data/tag/frame_SwinIR'):
        # folder = '../data/tag/frame_origin_1sec'

        image_name_list = os.listdir(folder)
        total_image = len(image_name_list)
        count = 0
        
        aruco_detector = ArucoDetector.create_custom_aruco_detector()
        
        try:
            for image_name in image_name_list:
                full_name = os.path.join(folder, image_name)
                frame = cv2.imread(full_name)

                height, width = frame.shape[0], frame.shape[1]
                if height < 40 or width < 40:
                    ratio = max(40 / height, 40 / width)
                    frame = cv2.resize(frame, ((int)(width * ratio), (int)(height * ratio)))

                ids, corners, rejected = ArucoDetector.detect_frame(frame, aruco_detector)

                if ids is not None and len(ids) > 0:
                    count += 1
        except Exception as e:
            print(e)
        
        # ArucoDetector.outputArucoParam()
        print(f"Detect {count} / {total_image}, rate = {count / total_image}")

    @staticmethod
    def detect_frame(frame, aruco_detector = None):
        if aruco_detector is None:
            aruco_detector = ArucoDetector.create_custom_aruco_detector()
        
        frame_display = frame.copy()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 检测标记
        corners, ids, rejected = aruco_detector.detectMarkers(gray)

        # 绘制检测到的标记
        if ids is not None:
            cv2.aruco.drawDetectedMarkers(frame_display, corners, ids)
            # print(f"Detected {len(ids)} markers: {ids.flatten()}")
            
            # 为每个标记绘制详细信息和坐标系
            for i, corner in enumerate(corners):
                # 计算中心点
                center = np.mean(corner[0], axis=0).astype(int)
                
                # 绘制中心点和ID
                cv2.circle(frame_display, tuple(center), 5, (0, 255, 0), -1)
                cv2.putText(frame_display, f"ID:{ids[i][0]}", 
                           (center[0] + 10, center[1]), 
                           cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
                
                # 绘制角点编号
                for j, point in enumerate(corner[0]):
                    cv2.circle(frame_display, tuple(point.astype(int)), 3, (255, 0, 0), -1)
                    cv2.putText(frame_display, str(j), 
                               (int(point[0]) + 5, int(point[1])), 
                               cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 0, 0), 1)
        
        # 绘制被拒绝的候选区域（用于调试）
        if rejected is not None:
            for reject in rejected:
                cv2.polylines(frame_display, [reject.astype(int)], True, (0, 0, 255), 2)
        # # 显示信息
        # info_text = [
        #     f"Detected: {len(ids) if ids is not None else 0}",
        #     f"Rejected: {len(rejected) if rejected is not None else 0}",
        #     "Press 'q' to quit"
        # ]
        
        # for i, text in enumerate(info_text):
        #     cv2.putText(frame_display, text, (10, 30 + i * 25), 
        #                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        
        ids_list, corners_list = ArucoDetector.postprocess(ids, corners)
        return ids_list, corners_list, frame_display
    
    @staticmethod
    def postprocess(ids, corners):
        ids_list = ids.flatten()
        corners_list = []
        for corner in corners:
            corner = corner[0]
            rd = (tuple(corner[0].astype(int))[0], tuple(corner[0].astype(int))[1]) 
            ld = (tuple(corner[1].astype(int))[0], tuple(corner[1].astype(int))[1]) 
            lu = (tuple(corner[2].astype(int))[0], tuple(corner[2].astype(int))[1]) 
            ru = (tuple(corner[3].astype(int))[0], tuple(corner[3].astype(int))[1]) 
            corners_list.append({"ld": ld, "rd": rd, "lu": lu, "ru": ru})
        return ids_list, corners_list

if __name__ == "__main__":
    detector = ArucoDetector()
    detector.detect_folder()
