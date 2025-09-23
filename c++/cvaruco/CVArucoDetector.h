#pragma once

#include <deque>
#include <unordered_map>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/aruco.hpp>

#include "libExportCVAruco.h"

class ResultPool {
public:
    ResultPool(const int nMaxLen = 1000) {
        m_nMaxLen = nMaxLen;
    }
    virtual ~ResultPool() {
        while (!m_quResults.empty()) {
            m_quResults.pop_front();
        }
    }

    stIdsPoints* get() {
        if (m_quResults.size() < m_nMaxLen) {
            stIdsPoints *pStResult = new stIdsPoints();
            m_quResults.push_back(pStResult);
        } else {
            stIdsPoints *pStResult = m_quResults.front();
            m_quResults.pop_front();
            
            pStResult->count = 0;
            pStResult->reject_count = 0;
            delete []pStResult->ids;
            delete []pStResult->corners;
            delete []pStResult->rejects;

            pStResult->ids = nullptr;
            pStResult->corners = nullptr;
            pStResult->rejects = nullptr;
            m_quResults.push_back(pStResult);
        }
                
        return m_quResults.back();
    }

private:
    std::deque<stIdsPoints*> m_quResults;
    size_t m_nMaxLen = 0;
};

class CVArucoDetector {
public:
    static std::shared_ptr<CVArucoDetector> getInstance();
    virtual ~CVArucoDetector();

    void* init();
    stIdsPoints* detect(void* pHandle, stFrame* pFrame = nullptr);
    stFrame* drawResult(void* pHandle, stIdsPoints* pResult, stFrame* pFrame);
    int deinit(void* pHandle);

    CVArucoDetector();
private:
    cv::aruco::DetectorParameters createParamsDetect();

private:
    static std::shared_ptr<CVArucoDetector> m_pInstance;
    ResultPool m_pool;
    cv::aruco::Dictionary m_stDictionary;
    cv::aruco::DetectorParameters m_stParamsDetect;
};
