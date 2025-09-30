#include <vector>
#include <string.h>

#include "camera.h"
#include "rigid3.h"
#include "absolute_pose.h"
#include "ransac.h"
#include "gtest.h"

using namespace colmap;

#include "libExportPnP.h"

#include "mathCommon.h"
#include "Detector.hpp"

#include "EstimatorCV.h"
#include "Compare.h"
#include "jsonReader.hpp"

int testPnPCv() {
    std::string sImageName = "/home/channy/Downloads/ros_workspace/extractBagInfo/images_left/20180308_161737_373021.jpg";
    std::string sJsonName = "/home/channy/Downloads/ros_workspace/extractBagInfo/images_transfer/20180308_161737_373021_20180308_161738_243060.json";
    cv::Mat image = cv::imread(sImageName);
    cv::resize(image, image, cv::Size(1024, 1024), 0, 0, cv::INTER_LINEAR);

    std::vector<std::vector<float>> vPoints3d, vPoints2d;
    size_t count = detectPoints(image.data, image.rows * image.cols * 3, image.rows, image.cols, vPoints2d, vPoints3d);
    printf("detect count = [%d]\n", count);

    // groundtruth
    Compare cCompare;
    cCompare.init();

    std::vector<float> vR, vt;
    bool res = readPoseJson(sJsonName, vR, vt);
    if (!res) {
        return -1;
    }
    Eigen::Vector3f R_WI, t_WI;
    R_WI << vR[0], vR[1], vR[2];
    t_WI << vt[0], vt[1], vt[2];

    // estimate
    EstimatorCV cEstimatorCV;
    std::tuple<cv::Mat, cv::Mat> tResult = cEstimatorCV.estimate(vPoints2d, vPoints3d);   

    cv::Mat cvR = std::get<0>(tResult);
    cv::Mat cvT = std::get<1>(tResult);
    Eigen::Vector3f eR, et;
    eR << cvR.at<float>(0, 0), cvR.at<float>(1, 0), cvR.at<float>(2, 0);
    et << cvT.at<float>(0, 0), cvT.at<float>(1, 0), cvT.at<float>(2, 0);

    std::tuple<float, float> tError = cCompare.calcError(eR, eR, R_WI, t_WI);
    printf("tError = [%f %f]\n", std::get<0>(tError), std::get<1>(tError));
    
    return 0;
}

int testPnPCol() {
    std::string sImageName = "/home/channy/Downloads/ros_workspace/extractBagInfo/images_left/20180308_161737_373021.jpg";
    std::string sJsonName = "/home/channy/Downloads/ros_workspace/extractBagInfo/images_transfer/20180308_161737_373021_20180308_161738_243060.json";
    cv::Mat image = cv::imread(sImageName);
    cv::resize(image, image, cv::Size(1024, 1024), 0, 0, cv::INTER_LINEAR);

    std::vector<std::vector<float>> vPoints3d, vPoints2d;
    size_t count = detectPoints(image.data, image.rows * image.cols * 3, image.rows, image.cols, vPoints2d, vPoints3d);
    printf("detect count = [%d]\n", count);

    // groundtruth
    Compare cCompare;
    cCompare.init();

    std::vector<float> vR, vt;
    bool res = readPoseJson(sJsonName, vR, vt);
    if (!res) {
        return -1;
    }
    Eigen::Vector3f R_WI, t_WI;
    R_WI << vR[0], vR[1], vR[2];
    t_WI << vt[0], vt[1], vt[2];

    // estimate
    std::vector<std::vector<float>> pM = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    auto pPoints2D = convertVectorToFloatPtr(vPoints2d);
    auto pPoints3D = convertVectorToFloatPtr(vPoints3d);
    auto pMat = convertVectorToFloatPtr(pM);

    initPnP();
    int result = estimatePnP(pPoints2D, 20, pPoints3D, 20, pMat);
    printf("res = %d\n", res);
    for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        printf("%f ", pM[i][j]);
      }
      printf("\n");
    }

    return 0;
}

int main() {
    // testPnPCv();
    // testPnPCol();
    return 0;
}
