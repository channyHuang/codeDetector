#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#include "ZXing/ReadBarcode.h"

int detectZXing(cv::Mat &cvImage) {
    int width = cvImage.cols, height = cvImage.rows;
    printf("width = %d, height = %d\n", width, height);
    auto image = ZXing::ImageView(cvImage.data, width, height, ZXing::ImageFormat::BGR);
    auto options = ZXing::ReaderOptions().setFormats(ZXing::BarcodeFormat::Any);
    auto barcodes = ZXing::ReadBarcodes(image, options);

    std::cout << "Detect count: " << barcodes.size() << std::endl;
    for (const auto& b : barcodes) {
        std::cout << ZXing::ToString(b.format()) << ": " << b.text() << "\n";
        std::cout << ZXing::ToString(b.position()) << std::endl;
    }
    return barcodes.size();
}

int main() {
    std::string sImagePath = "../../data/3_selfdefine.jpg";
    cv::Mat image = cv::imread(sImagePath);
    int count = detectZXing(image);
    return 0;
}
