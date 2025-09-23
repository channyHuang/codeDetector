#include <iostream>
#include <vector>
#include <memory>

#include "CVArucoDetector.h"

#define HIGH_VERSION 0  

std::shared_ptr<CVArucoDetector> CVArucoDetector::m_pInstance = nullptr;

std::shared_ptr<CVArucoDetector> CVArucoDetector::getInstance() {
    if (m_pInstance == nullptr) {
        m_pInstance = std::make_shared<CVArucoDetector>();
    }
    return m_pInstance;
}

CVArucoDetector::CVArucoDetector() {}

CVArucoDetector::~CVArucoDetector() {}

cv::aruco::DetectorParameters CVArucoDetector::createParamsDetect() {
    cv::aruco::DetectorParameters stParamsDetect;

    // light selffix
    stParamsDetect.adaptiveThreshWinSizeMin = 5;
    stParamsDetect.adaptiveThreshWinSizeMax = 23;
    stParamsDetect.adaptiveThreshWinSizeStep = 10;
    stParamsDetect.adaptiveThreshConstant = 7;
    // masks size filter
    stParamsDetect.minMarkerPerimeterRate = 0.03;
    stParamsDetect.maxMarkerPerimeterRate = 4.0;
    stParamsDetect.minMarkerDistanceRate = 0.125;
    stParamsDetect.minCornerDistanceRate = 0.05;
    // contours
    stParamsDetect.polygonalApproxAccuracyRate = 0.03;
    stParamsDetect.minDistanceToBorder = 3;
    // corner
    stParamsDetect.cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
    stParamsDetect.cornerRefinementWinSize = 5;
    stParamsDetect.cornerRefinementMaxIterations = 30;
    stParamsDetect.cornerRefinementMinAccuracy = 0.1;
    // mask analyze 
    stParamsDetect.markerBorderBits = 30;
    stParamsDetect.perspectiveRemovePixelPerCell = 40;
    stParamsDetect.perspectiveRemoveIgnoredMarginPerCell = 0.15;
    // error refine
    stParamsDetect.errorCorrectionRate = 0.8;
    // detect mode
    // stParamsDetect.useAruco3Detection = true;
    // stParamsDetect.minSideLengthCanonicalImg = 32;
    // stParamsDetect.minMarkerLengthRatioOriginalImg = 0;
    // 
    // stParamsDetect.relativeCornerRefinmentWinSize = 0.3f;
    stParamsDetect.maxErroneousBitsInBorderRate = 0.35;
    stParamsDetect.minOtsuStdDev = 5.0;

    stParamsDetect.aprilTagQuadDecimate = 0.0;
    stParamsDetect.aprilTagQuadSigma = 0.0;
    stParamsDetect.aprilTagMinClusterPixels = 5;
    stParamsDetect.aprilTagMaxNmaxima = 10;
    stParamsDetect.aprilTagCriticalRad = (float)(10* CV_PI /180);
    stParamsDetect.aprilTagMaxLineFitMse = 10.0;
    stParamsDetect.aprilTagMinWhiteBlackDiff = 5;
    stParamsDetect.aprilTagDeglitch = 0;
    stParamsDetect.detectInvertedMarker = false;

    return stParamsDetect;
}

void* CVArucoDetector::init() {
    m_stDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_36h11);
    m_stParamsDetect = createParamsDetect();

    // std::shared_ptr<cv::aruco::ArucoDetector> pDetector = std::make_shared<cv::aruco::ArucoDetector>(m_stDictionary, m_stParamsDetect);
    // return pDetector.get();
    return nullptr;
}

stIdsPoints* CVArucoDetector::detect(void* pHandle, stFrame* pFrame) {
    // printf("CVArucoDetector::detect \n");
    cv::Mat image(pFrame->row, pFrame->col, CV_8UC3, pFrame->pChar);

    std::vector<std::vector<cv::Point2f> > corners, rejects;
    std::vector<int> ids;
    printf("CVArucoDetector::detect a\n");
// #ifdef HIGH_VERSION
//         cv::aruco::ArucoDetector* pDetector = std::static_cast<cv::aruco::ArucoDetector*>(pHandle);
//         if (pDetector == nullptr) {
//             printf("Error in detect input handle...\n");
//             return stIdsPoints();
//         }
//         pDetector->detectMarkers(image, corners, ids, rejects);
// #else
//         cv::aruco::detectMarkers(image, &m_stDictionary, corners, ids, &m_stParamsDetect);
// #endif
    printf("CVArucoDetector::detect b\n");
    
    stIdsPoints* pResult = m_pool.get();
    // if (ids.size() <= 0 || ids.size() != corners.size()) return pResult;
    // printf("CVArucoDetector::detect c\n");

    // pResult->count = ids.size();
    // if (pResult->count > 0) {
    //     pResult->ids = new int[pResult->count];
    //     pResult->corners = new int[pResult->count * 8];
    // }
    // if (rejects.size() > 0) {
    //     pResult->reject_count = 0;
    //     pResult->rejects = new int[pResult->reject_count * 8];
    // }

    // for (size_t i = 0; i < ids.size(); ++i) {
    //     pResult->ids[i] = ids[i];
    //     if (corners[i].size() != 4) {
    //         printf("Error in detect result: corner %d size != 4\n", i);
    //         continue;
    //     }
    //     // rd, ld, lu, ru
    //     pResult->corners[i * 8 + 0] = corners[i][0].x;
    //     pResult->corners[i * 8 + 1] = corners[i][0].y;
    //     pResult->corners[i * 8 + 2] = corners[i][1].x;
    //     pResult->corners[i * 8 + 3] = corners[i][1].y;
    //     pResult->corners[i * 8 + 4] = corners[i][2].x;
    //     pResult->corners[i * 8 + 5] = corners[i][2].y;
    //     pResult->corners[i * 8 + 6] = corners[i][3].x;
    //     pResult->corners[i * 8 + 7] = corners[i][3].y;
    // }

    // for (size_t i = 0; i < rejects.size(); ++i) {
    //     for (size_t j = 0; j < rejects[i].size(); ++j) {
    //         pResult->rejects[i * 8 + j * 2] = rejects[i][j].x;
    //         pResult->rejects[i * 8 + j * 2 + 1] = rejects[i][j].y;
    //     }
    // }
    printf("aaa pResult %d\n", (pResult == nullptr) ? 0 : 1);
    return pResult;
}

stFrame* CVArucoDetector::drawResult(void* pHandle, stIdsPoints* pResult, stFrame* pFrame) {
    printf("pFrame %d\n", pFrame->row);
    cv::Mat image(pFrame->row, pFrame->col, CV_8UC3, pFrame->pChar);
    printf("pResult->count = %d\n", pResult->count);
    if ((pResult->count == 0) && (pResult->reject_count == 0)) return pFrame;
    
    for (size_t i = 0; i < pResult->count; ++i) {
        cv::Point2i rightDown = cv::Point2f(pResult->corners[i * 8], pResult->corners[i * 8 + 1]); 
        cv::Point2i leftDown = cv::Point2f(pResult->corners[i * 8 + 2], pResult->corners[i * 8 + 3]);
        cv::Point2i leftUp = cv::Point2f(pResult->corners[i * 8 + 4], pResult->corners[i * 8 + 5]);
        cv::Point2i rightUp = cv::Point2f(pResult->corners[i * 8 + 6], pResult->corners[i * 8 + 7]);
        cv::Point2i center = (rightDown + leftUp) / 2;
 
        cv::putText(image, std::to_string(pResult->ids[i]), center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
        
        cv::line(image, rightDown, rightUp, cv::Scalar(0, 0, 255), 1);
        cv::line(image, rightUp, leftUp, cv::Scalar(0, 0, 255), 1);
        cv::line(image, leftUp, leftDown, cv::Scalar(0, 0, 255), 1);
        cv::line(image, leftDown, rightDown, cv::Scalar(0, 0, 255), 1);
    }
    cv::imshow("mid", image);
    cv::waitKey(1000);
    memcpy(pFrame->pChar, image.data, pFrame->size);

    return pFrame;
}

int CVArucoDetector::deinit(void* pHandle) {
    return 0;
}