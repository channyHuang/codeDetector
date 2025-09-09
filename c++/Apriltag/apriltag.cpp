#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#include "apriltag/apriltag.h"
#include "apriltag/tag36h11.h"

int detectApriltag(cv::Mat &cvImage) {
    int width = cvImage.cols, height = cvImage.rows;
    cv::Mat gray;
    cv::cvtColor(cvImage, gray, cv::COLOR_BGR2GRAY);
    image_u8_t im = {gray.cols, gray.rows, gray.cols, gray.data};

    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);

    if (errno == ENOMEM) {
        printf("Unable to add family to detector due to insufficient memory to allocate the tag-family decoder with the default maximum hamming value of 2. Try choosing an alternative tag family.\n");
        exit(-1);
    }

    td->quad_decimate = 1; // up/downsampling, > 1
    td->quad_sigma = 0; // gaussian blur, > 0
    td->nthreads = 1;
    td->debug = false;
    td->refine_edges = true;

    zarray_t *detections = apriltag_detector_detect(td, &im);

    if (errno == EAGAIN) {
        printf("Unable to create the %d threads requested.\n",td->nthreads);
        exit(-1);
    }
    printf("size = %d\n", zarray_size(detections));
    // Draw detection outlines
    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        printf("id = %d, point = (%f)\n", det->id, det->p[0][0]);
    }
    return 0;
}

int main() {
    std::string sImagePath = "tag36_11_00168.png";
    cv::Mat image = cv::imread(sImagePath);
    cv::resize(image, image, cv::Size(40, 40), 0, 0, cv::INTER_LINEAR);
    int count = detectApriltag(image);
    return 0;
}
