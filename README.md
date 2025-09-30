[toc]

# QRCode Recognize
QRCode二维码识别库效果简单对比
| Language | Library | Result | Speed |
|:---:|:---:|:---:|:---:|
| c++ | [ZXing](https://github.com/zxing-cpp/zxing-cpp) | Good, 好，都能正常识别 | Good |
| c | [ZBar](https://zbar.sourceforge.net/) | Build Bad | Normal |
| c++/python | opencv+wechat | Normal, 一般，多个码只识别到其中一个 | Normal |
| c++/python | cv2.QRCodeDetector | Normal, 一般，多个码识别失败 | Normal |
| python | [qreader](https://github.com/Eric-Canas/qreader) | Good, 好，多个码能识别，但内容可能识别失败 | Low |
| python | pyzbar | Bad, 差，略有倾斜的码都识别不了 | Normal |

其中`cv2.wechat_qrcode_WeChatQRCode`需要加载官方预训练模型[opencv_3rdparty](https://github.com/WeChatCV/opencv_3rdparty.git)且需要安装`opencv-contrib-python`，不同于`cv2.QRCodeDetector`环境安装`opencv-python`

## Result
### 测试图像1——倾斜：pyzbar和cv2.QRCodeDetector()识别失败，out!
![pyzbar_failed](data/qrcode/1_distort.jpg)
### 测试图像2——一图多码：剩下的都能正常识别
![multicode_normal](data/qrcode/2_multicode_normal.jpg)
![multicode](data/qrcode/2_multicode.png)
### 测试图像3——自定义一图多码：qreader和cv2.wechat_qrcode_WeChatQRCode识别失败，out!
最终只有`ZXing`能都正常识别出来。

![selfdefine](data/qrcode/3_selfdefine.jpg)

# fiducial marker 
[apriltag-imgs](https://github.com/AprilRobotics/apriltag-imgs)
以`Family = tag36h11`为测试图像。

| Language | Name | Result | Speed | Others |
|:---:|:---:|:---:|:---:|:---:|
| c++/python | [AprilTag](https://github.com/AprilRobotics/apriltag.git) | > 30x30 | Good | |
| c++/python | ArUco | > 35x35, in opencv | Good | |
| c++/python | WhyCode | | | pip install failed, 圆形标记 |
| c++ | [TopoTag](https://herohuyongtao.github.io/research/publications/topo-tag/) | - | - | 只有windows库，不开源 |
| c++ | [STag](https://github.com/manfredstoiber/stag) | - | SLOW | 圆形标记 |

其中`ArUco`需要安装`opencv-contrib-python`，不同于其它环境安装`opencv-python`

# Super-Resolution
以无人机定位地面上的apriltag点为需求，图像模糊检测tag失败，通过增加Super-Resolution操作，可以提升tag检测成功率。

使用[visual-inertial-dataset](https://cvg.cit.tum.de/data/datasets/visual-inertial-dataset)中`dataset-calib-cam2_512_16.bag`包中的图像作为测试数据集。使用[Real-ESRGAN](https://github.com/xinntao/Real-ESRGAN.git)或[SwinIR](https://github.com/JingyunLiang/SwinIR.git)或[DeepDeblur-PyTorch](https://github.com/SeungjunNah/DeepDeblur-PyTorch)做Super-Resolution。其中原图像使用apriltag和cv2.aruco均检测失败，而Super-Resolution新图像使用cv2.aruco可检测出一个id，apriltag依旧检测失败。

![原图像](./data/tag/frame0001.jpg)
![Super-Resolution新图像](./data/tag/frame0001_out.jpg)

通过调整`cv2.aruco`的参数可以提高/降低经过`Super-Resolution`处理后的图像帧的识别率。默认参数如下。
```py
    import cv2; [print(f"{attr}: {getattr(cv2.aruco.DetectorParameters(), attr)}") for attr in dir(cv2.aruco.DetectorParameters()) if not attr.startswith('_')]
```
```sh
adaptiveThreshConstant: 7.0
adaptiveThreshWinSizeMax: 23
adaptiveThreshWinSizeMin: 3
adaptiveThreshWinSizeStep: 10
aprilTagCriticalRad: 0.1745329201221466
aprilTagDeglitch: 0
aprilTagMaxLineFitMse: 10.0
aprilTagMaxNmaxima: 10
aprilTagMinClusterPixels: 5
aprilTagMinWhiteBlackDiff: 5
aprilTagQuadDecimate: 0.0
aprilTagQuadSigma: 0.0
cornerRefinementMaxIterations: 30
cornerRefinementMethod: 0
cornerRefinementMinAccuracy: 0.1
cornerRefinementWinSize: 5
detectInvertedMarker: False
errorCorrectionRate: 0.6
markerBorderBits: 1
maxErroneousBitsInBorderRate: 0.35
maxMarkerPerimeterRate: 4.0
minCornerDistanceRate: 0.05
minDistanceToBorder: 3
minGroupDistance: 0.20999999344348907
minMarkerDistanceRate: 0.125
minMarkerLengthRatioOriginalImg: 0.0
minMarkerPerimeterRate: 0.03
minOtsuStdDev: 5.0
minSideLengthCanonicalImg: 32
perspectiveRemoveIgnoredMarginPerCell: 0.13
perspectiveRemovePixelPerCell: 4
polygonalApproxAccuracyRate: 0.03
readDetectorParameters: <built-in method readDetectorParameters of cv2.aruco.DetectorParameters object at 0x7f6df55c6e80>
relativeCornerRefinmentWinSize: 0.30000001192092896
useAruco3Detection: False
writeDetectorParameters: <built-in method writeDetectorParameters of cv2.aruco.DetectorParameters object at 0x7f6df55c6e80>
```

查看Apriltag的源码发现检测过程为：边缘检测->轮廓四边形逼近->解码。默认解码匹配hamming距离最大为3。内部采用数组模拟hash，使用后移延伸处理冲突。TUM VI数据集中边界采用了双层黑边（外黑边界比内部白格子宽度大一倍）导致使用Apriltag检测失败。

而opencv的arucoDetector在c++中默认安装的不包含，需要勾选opencv_contrib源码编译安装。

# PnP
> b = (int)a c++/py 并不是四舍五入，而是取向0靠近的整数
## apriltag
返回结果按图像坐标系，图像左上角为原点
```c++
/*
o--------------> x
|
|
|
|
v
y
*/
```
## EPnP
colmap中只实现了EPnP，当检测到的点共面时失效。

EPnP:
1. 世界坐标系下的三维点${P_i}$，选择4个控制点 $c^w_{j}, j = 1,...,4$ 进行表达。其中控制点1为三维点的质心，其它控制点为三维点PCA主方向上前三个特征向量方向上的点（$A^TA, A=[P_1-c_1|P_2-c_2|...]$的特征值和特征向量）
2. 每个三维点表示成控制点的线性表达，得权重 $P^w_i = \sum c^w_{j} \alpha_{ij}$ ，权重对所有坐标系不变（在齐次坐标下），在相机坐标系下同样有 $P^c_i = \sum c^c_{j} \alpha_{ij}$
3. 三维点到相机坐标系二维点的投影 $P^c_i = [x_i, y_i, z_i] $，$ u_i = [ x_i / z_i, y_i / z_i]$ 消除 $z_i$ 得到 $ u_i, i=1,...,n$ 和 $ c^c_j,j=1,...,4 $ 的关系式，奇异值分解 $M_{2nx12} x=0$ $ M=U \Sigma V $，解是 4 个右奇异向量的线性组合，对应最小的 m 个奇异值（m 根据情况可能是 1, 2, 3, 4）。最终求得控制点在相机坐标系下的表达 $c^c_{j}$。设 $V$ 的最后 m 列（对应奇异值接近 0）为 $ {v_1,...,v_m} $, 则 $x = \sum_{k=1}^m \beta_k v_k$，其中标量$\beta_k$待求解
4. 距离约束求解$\beta_k$
5. 3D-3D匹配$P^w_j$和$c^c_j$，用 Umeyama 算法（SVD 方法），最终求得位姿$[R|t]$
6. 高斯－牛顿法非线性优化误差

误差来源：
* 退化，三维点共面/共线/共点，导致$A$不満秩，控制点不能正常表达所有三维点
* 病态，三维点接近共面，SVD计算出的“零”奇异值可能不是精确的零，而是一堆非常小但数值上不可靠的值。阈值主观化。

## OpenCV
IPPE:

