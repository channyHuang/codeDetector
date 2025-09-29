#include "libExportCVAruco.h"

#include <memory>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "json.hpp"

int detect(void* pHandle, cv::Mat& image) {
    std::shared_ptr<stFrame> pFrame = std::make_shared<stFrame>();
    pFrame->col = image.cols;
    pFrame->row = image.rows;
    pFrame->size = image.cols * image.rows * 3;
    pFrame->pChar = new unsigned char[pFrame->size];
    // printf("Input image width = %d, height = %d, size = %d\n", image.cols, image.rows, pFrame->size);
    memcpy(pFrame->pChar, image.data, pFrame->size);
    
    stIdsPoints *pResult = detect(pHandle, pFrame.get());
    if (pResult == nullptr) {
        printf("result is nullptr\n");
        return 0;
    } else if (pResult->count > 0) {
        printf("Detect count = [%d]\n", pResult->count);
    }

    if (pResult->count <= 0) {
        return 0;
    }

    stFrame* pNewFrame = drawResult(pHandle, pResult, pFrame.get());
    if (pNewFrame != nullptr) {
        printf("pNewFrame %d %d %d\n ", pNewFrame->col, pNewFrame->row, pNewFrame->size);
    }

    memcpy(image.data, pNewFrame->pChar, pNewFrame->size);
    cv::imwrite("result.jpg", image);

    return pResult->count;   
}

void setParamsOneByOne(void* pHandle, cv::Mat &image) {
    for (int i = 1; i < 40; ++i) {
        nlohmann::json stJsonData, stJsonDetectData;
        stJsonDetectData["perspectiveRemovePixelPerCell"] = i;
        stJsonDetectData["errorCorrectionRate"] = 0.6;
        stJsonData["detect"] = stJsonDetectData;
        std::string sJson = stJsonData.dump();
        // printf("set param json [%s]\n", sJson.c_str());
        auto res = setParams(pHandle, sJson.c_str(), sJson.length());

        int count = detect(pHandle, image);
    }
}

int main(int argc, char** argv) {
    std::string sImageName = "../../../data/tag/frame_SwinIR/frame0001.jpg_SwinIR.png";
    if (argc >= 2) {
        sImageName = std::string(argv[1]);
    }
    printf("sImageName = [%s]\n", sImageName.c_str());
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    cv::Mat image = cv::imread(sImageName);

    void* pHandle = initCVAruco();

    // setParamsOneByOne(pHandle, image);
    detect(pHandle, image);
    
    return 0;
}