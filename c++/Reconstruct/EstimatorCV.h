#pragma once

#include <opencv2/opencv.hpp>

class EstimatorCV {
public:
    EstimatorCV();
    virtual ~EstimatorCV();

public:
    std::tuple<cv::Mat, cv::Mat> estimate(std::vector<std::vector<float>> &vPoints2d, 
                                        std::vector<std::vector<float>> &vPoints3d, 
                                        int flag = cv::SOLVEPNP_ITERATIVE);

private:

};
