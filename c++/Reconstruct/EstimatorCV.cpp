#include "EstimatorCV.h"

EstimatorCV::EstimatorCV() {}

EstimatorCV::~EstimatorCV() {}

std::tuple<cv::Mat, cv::Mat> EstimatorCV::estimate(std::vector<std::vector<float>> &vPoints2d, std::vector<std::vector<float>> &vPoints3d, int flag) {
    std::vector<cv::Point2f> vPoints2dCV;
    std::vector<cv::Point3f> vPoints3dCV;
    for (size_t i = 0; i < vPoints2d.size(); ++i) {
      vPoints2dCV.emplace_back(cv::Point2f(vPoints2d[i][0], vPoints2d[i][1]));
    }
      for (size_t i = 0; i < vPoints3d.size(); ++i) {
      vPoints3dCV.emplace_back(cv::Point3f(vPoints3d[i][0], vPoints3d[i][1], vPoints3d[i][2]));
    }
    printf("point size [%d] - [%d]\n", vPoints2dCV.size(), vPoints3dCV.size());
    cv::Mat intrinsics = cv::Mat::eye(3, 3, CV_32FC1);
    intrinsics.at<float> (0, 0) = 190.0;
    intrinsics.at<float> (1, 1) = 190.0;
    intrinsics.at<float> (0, 2) = 512 / 2;
    intrinsics.at<float> (1, 2) = 512 / 2;
    cv::Mat distortion = cv::Mat::zeros(5, 1, CV_32FC1);
    distortion.at<float> (0, 0) = 0.0034823894022493434;
    distortion.at<float> (1, 0) = 0.0007150348452162257;
    distortion.at<float> (2, 0) = -0.0020532361418706202;
    distortion.at<float> (3, 0) = 0.00020293673591811182;
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32FC1);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32FC1);

    cv::solvePnP(vPoints3dCV, vPoints2dCV, intrinsics, distortion, rvec, tvec, false, flag);
    printf("rvec [%f, %f, %f]\n", rvec.at<float>(0, 0), rvec.at<float>(1, 0), rvec.at<float>(2, 0));
    printf("tvec [%f, %f, %f]\n", tvec.at<float>(0, 0), tvec.at<float>(1, 0), tvec.at<float>(2, 0));
    
    return std::tuple<cv::Mat, cv::Mat>(rvec, tvec);
}
