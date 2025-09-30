#include <unordered_map>
#include <memory>

#include <opencv2/opencv.hpp>

#include "libExportCVAruco.h"

std::unordered_map<size_t, std::vector<float>> initApPoints(float size = 0.088, float spacing = 0.3, size_t cols = 6, size_t rows = 6) {
    std::unordered_map<size_t, std::vector<float>> mapApPoints;
    float pad = size * spacing;
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            size_t id = r * cols + c;
            float x = c * (size + pad) + (size + pad);
            float y = r * (size + pad) + (size + pad);
            std::vector<float> pts = {x, y, size};
            mapApPoints[id] = pts;
        }
    }
    return mapApPoints;
}

size_t detectPoints(unsigned char *pData, 
                    size_t nDataLen, 
                    size_t nWidth, 
                    size_t nHeight,
                    std::vector<std::vector<float> > &vPoints2d, 
                    std::vector<std::vector<float> > &vPoints3d) {

    std::shared_ptr<stFrame> pFrame = std::make_shared<stFrame>();
    pFrame->col = nWidth;
    pFrame->row = nHeight;
    pFrame->size = nWidth * nHeight * 3;
    pFrame->pChar = new unsigned char[pFrame->size];
    memcpy(pFrame->pChar, pData, pFrame->size);

    void* pHandle = initCVAruco();
    auto pResult = detect(pHandle, pFrame.get());

    for (size_t i = 0; i < pResult->count; i++) {
        cv::Point2i rightDown = cv::Point2f(pResult->corners[i * 8], pResult->corners[i * 8 + 1]); 
        cv::Point2i leftDown = cv::Point2f(pResult->corners[i * 8 + 2], pResult->corners[i * 8 + 3]);
        cv::Point2i leftUp = cv::Point2f(pResult->corners[i * 8 + 4], pResult->corners[i * 8 + 5]);
        cv::Point2i rightUp = cv::Point2f(pResult->corners[i * 8 + 6], pResult->corners[i * 8 + 7]);
    
        vPoints2d.emplace_back(std::vector<float>({rightDown.x, rightDown.y}));
        vPoints2d.emplace_back(std::vector<float>({leftDown.x, leftDown.y}));
        vPoints2d.emplace_back(std::vector<float>({leftUp.x, leftUp.y}));
        vPoints2d.emplace_back(std::vector<float>({rightUp.x, rightUp.y}));
    }  

    std::unordered_map<size_t, std::vector<float>> mapApPoints = initApPoints();
    float offset = -1.0f;
    for (size_t i = 0; i < pResult->count; ++i) {
        auto itr = mapApPoints.find(pResult->ids[i]);
        if (itr == mapApPoints.end()) {
            continue;
        }
        if (offset < 0) {
            offset = itr->second[2] / 2;
        }
        vPoints3d.emplace_back(std::vector<float>({itr->second[0] - offset, itr->second[1] + offset, 0}));
        vPoints3d.emplace_back(std::vector<float>({itr->second[0] + offset, itr->second[1] + offset, 0}));
        vPoints3d.emplace_back(std::vector<float>({itr->second[0] + offset, itr->second[1] - offset, 0}));
        vPoints3d.emplace_back(std::vector<float>({itr->second[0] - offset, itr->second[1] - offset, 0}));
    }

    return pResult->count;
}
