#include "libExportCVAruco.h"

#include <memory>
#include <iostream>

#include <opencv2/opencv.hpp>


int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    cv::Mat image = cv::imread("frame0001.jpg_SwinIR.png");
    // cv::imshow("origin", image);
    // cv::waitKey(1000);

    void* pHandle = init();
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
    } else {
        printf("Detect count = %d\n", pResult->count);
        
    }
    stFrame* pNewFrame = drawResult(pHandle, pResult, pFrame.get());
    if (pNewFrame != nullptr) {
        printf("pNewFrame %d %d %d\n ", pNewFrame->col, pNewFrame->row, pNewFrame->size);
    }

    memcpy(image.data, pNewFrame->pChar, pNewFrame->size);
    cv::imshow("res", image);
    cv::waitKey(1000);

    return 0;
}