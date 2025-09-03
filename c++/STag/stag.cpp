#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>


int detectStag(cv::Mat &cvImage) {
    int width = cvImage.cols, height = cvImage.rows;
    
    return barcodes.size();
}

int main() {
    std::string sImagePath = "../../data/3_selfdefine.jpg";
    cv::Mat image = cv::imread(sImagePath);
    int count = detectStag(image);
    return 0;
}
