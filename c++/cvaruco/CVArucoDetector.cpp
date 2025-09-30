#include <iostream>
#include <vector>
#include <memory>

#include "json.hpp"

#include "CVArucoDetector.h"

#define PIN printf("%d[%s]\n",__LINE__,__FILE__)

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
    stParamsDetect.markerBorderBits = 1;
    stParamsDetect.perspectiveRemovePixelPerCell = 40;
    stParamsDetect.perspectiveRemoveIgnoredMarginPerCell = 0.15;
    // error refine
    stParamsDetect.errorCorrectionRate = 0.8; 
    // detect mode
    stParamsDetect.useAruco3Detection = true;
    stParamsDetect.minSideLengthCanonicalImg = 32;
    stParamsDetect.minMarkerLengthRatioOriginalImg = 0.01;
    
    stParamsDetect.relativeCornerRefinmentWinSize = 0.3f;
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

cv::aruco::RefineParameters CVArucoDetector::createParamsRefine() {
    cv::aruco::RefineParameters stParamsRefine;
    stParamsRefine.minRepDistance = 10.f;
    stParamsRefine.errorCorrectionRate = 3.f;
    stParamsRefine.checkAllOrders = true;
    return stParamsRefine;
}

void* CVArucoDetector::init() {
    m_stDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_36h11);
    m_stParamsDetect = createParamsDetect();
    m_stParamsRefine = createParamsRefine();

    cv::aruco::ArucoDetector* pDetector = new cv::aruco::ArucoDetector(m_stDictionary, m_stParamsDetect, m_stParamsRefine);
    return pDetector;
}

template<typename TV, typename TS>
int setEachParams(const std::string& key, TV value, TS &st) {
    if constexpr (std::is_same_v<TS, cv::aruco::DetectorParameters>) {
        if (strcmp(key.c_str(), "perspectiveRemovePixelPerCell") == 0) {
            st.perspectiveRemovePixelPerCell = value;
        } else if (strcmp(key.c_str(), "adaptiveThreshWinSizeMin") == 0) {
            st.adaptiveThreshWinSizeMin = value;
        } else if (strcmp(key.c_str(), "errorCorrectionRate") == 0) {
            st.errorCorrectionRate = value;
        } else if (strcmp(key.c_str(), "minDistanceToBorder") == 0) {
            st.minDistanceToBorder = value;
        } else {
            return -1;
        }
    } else if constexpr (std::is_same_v<TS, cv::aruco::RefineParameters>) {
        if (strcmp(key.c_str(), "minRepDistance") == 0) {
            st.minRepDistance = value;
        } else if (strcmp(key.c_str(), "checkAllOrders") == 0) {
            st.checkAllOrders = value;
        }  else {
            return -1;
        }
    } else {
        return -1;
    }
    
    return 0;
}

int CVArucoDetector::updateParams(void* pHandle, const char* pDetectParamFile, const char* pRefineParamFile) {
    cv::aruco::ArucoDetector* pDetector = static_cast<cv::aruco::ArucoDetector*>(pHandle);
    if (pDetector == nullptr) {
        printf("Error in detect input handle...\n");
        return -1;
    }
    
    cv::FileStorage fs("params_detect.json", cv::FileStorage::READ);
    if (!fs.isOpened()) {
        return -1;
    }
    cv::FileNode fn = fs.root();
    bool success = m_stParamsDetect.readDetectorParameters(fn);
    
    // cv::FileNode paramsNode = fs["detect"];
    // if (!paramsNode.empty()) {
    //     bool success = detectorParams->readDetectorParameters(paramsNode);
    // }
    
    cv::FileStorage fs_refine("params_refine.json", cv::FileStorage::READ);
    if (!fs_refine.isOpened()) {
        return -1;
    } 
    fn = fs_refine.root();
    success = m_stParamsRefine.readRefineParameters(fn);
    return success ? 0 : -1;
}

int CVArucoDetector::outputParams(void* pHandle, const char* pDetectParamName, const char* pRefineParamName) {
    cv::aruco::ArucoDetector* pDetector = static_cast<cv::aruco::ArucoDetector*>(pHandle);
    if (pDetector == nullptr) {
        printf("Error in detect input handle...\n");
        return -1;
    }
    int res = 0;
    if (pDetectParamName != nullptr) {
        cv::FileStorage fs(std::string(pDetectParamName), cv::FileStorage::WRITE);
        if (fs.isOpened()) {
            cv::aruco::DetectorParameters stParamsDetect = pDetector->getDetectorParameters();
            stParamsDetect.writeDetectorParameters(fs);
        } else {
            res = -1;
        }
    }
    if (pRefineParamName != nullptr) {
        cv::FileStorage fs_refine(pRefineParamName, cv::FileStorage::WRITE);
        if (fs_refine.isOpened()) {
            cv::aruco::RefineParameters stParamsRefine = pDetector->getRefineParameters();
            stParamsRefine.writeRefineParameters(fs_refine);
        } else {
            res = -1;
        }
    }
    return res;
}

int CVArucoDetector::setParams(void* pHandle, const char* pJsonData, size_t nJsonLen) {
    cv::aruco::ArucoDetector* pDetector = static_cast<cv::aruco::ArucoDetector*>(pHandle);
    if (pDetector == nullptr) {
        printf("Error in detect input handle...\n");
        return -1;
    }

    nlohmann::json stJsonParams = nlohmann::json::parse(pJsonData, pJsonData + nJsonLen);
    if (stJsonParams.empty()) {
        printf("Error parse input json params");
		return -2;
    }
    if (stJsonParams.contains("detect")) {
        nlohmann::json stJsonParamsDetect = stJsonParams["detect"];
        for (auto item : stJsonParamsDetect.items()) {
            std::string sKey = item.key();
            setEachParams(sKey, item.value(), m_stParamsDetect);
        }
    }
    if (stJsonParams.contains("refine")) {
        nlohmann::json stJsonParamsRefine = stJsonParams["refine"];
        for (auto item : stJsonParamsRefine.items()) {
            std::string sKey = item.key();
            setEachParams(sKey, item.value(), m_stParamsRefine);
        }
    }
    pDetector->setDetectorParameters(m_stParamsDetect);
    pDetector->setRefineParameters(m_stParamsRefine);
    // printf("CVArucoDetector::setParams success\n");
    return 0;
}

stIdsPoints* CVArucoDetector::detect(void* pHandle, stFrame* pFrame) {
    // printf("CVArucoDetector::detect ...\n");
    cv::Mat image(pFrame->row, pFrame->col, CV_8UC3, pFrame->pChar);

    cv::aruco::ArucoDetector* pDetector = static_cast<cv::aruco::ArucoDetector*>(pHandle);
    if (pDetector == nullptr) {
        printf("Error in detect input handle...\n");
        return nullptr;
    }

    std::vector<std::vector<cv::Point2f> > corners, rejects;
    std::vector<int> ids;

    try {
        pDetector->detectMarkers(image, corners, ids, rejects);
    } catch(const std::exception &e) {
        printf("Exception: %s", e.what());
        return nullptr;
    }

    stIdsPoints* pResult = m_pool.get();
    if (ids.size() <= 0 || ids.size() != corners.size()) {
        // printf("Detect Success but None\n");
        return pResult;
    }

    pResult->count = ids.size();
    if (pResult->count > 0) {
        pResult->ids = new int[pResult->count];
        pResult->corners = new int[pResult->count * 8];
    }
    if (rejects.size() > 0) {
        pResult->reject_count = rejects.size();
        pResult->rejects = new int[pResult->reject_count * 8];
    }

    for (size_t i = 0; i < ids.size(); ++i) {
        pResult->ids[i] = ids[i];
        if (corners[i].size() != 4) {
            printf("Error in detect result: corner %d size != 4\n", i);
            continue;
        }
        // rd, ld, lu, ru
        pResult->corners[i * 8 + 0] = corners[i][0].x;
        pResult->corners[i * 8 + 1] = corners[i][0].y;
        pResult->corners[i * 8 + 2] = corners[i][1].x;
        pResult->corners[i * 8 + 3] = corners[i][1].y;
        pResult->corners[i * 8 + 4] = corners[i][2].x;
        pResult->corners[i * 8 + 5] = corners[i][2].y;
        pResult->corners[i * 8 + 6] = corners[i][3].x;
        pResult->corners[i * 8 + 7] = corners[i][3].y;
    }

    for (size_t i = 0; i < rejects.size(); ++i) {
        for (size_t j = 0; j < rejects[i].size(); ++j) {
            pResult->rejects[i * 8 + j * 2] = rejects[i][j].x;
            pResult->rejects[i * 8 + j * 2 + 1] = rejects[i][j].y;
        }
    }
    return pResult;
}

stFrame* CVArucoDetector::drawResult(void* pHandle, stIdsPoints* pResult, stFrame* pFrame) {
    // printf("CVArucoDetector::drawResult pFrame.size = (%d, %d), result.count = %d\n", pFrame->row, pFrame->col, pResult->count);
    if ((pResult->count == 0) && (pResult->reject_count == 0)) return pFrame;
    cv::Mat image(pFrame->row, pFrame->col, CV_8UC3, pFrame->pChar);
    try {
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
    } catch(const std::exception &e) {
        printf("Exception: %s\n", e.what());
    }
    memcpy(pFrame->pChar, image.data, pFrame->size);

    return pFrame;
}

int CVArucoDetector::deinit(void* pHandle) {
    return 0;
}