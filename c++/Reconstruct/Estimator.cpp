#include "Estimator.h"

#include <vector>
#include <string.h>

#include "rigid3.h"
#include "absolute_pose.h"
#include "ransac.h"
#include "gtest.h"

using namespace colmap;

Estimator* Estimator::m_pInstance = nullptr;

Estimator* Estimator::getInstance() {
    if (m_pInstance == nullptr) {
        m_pInstance = new Estimator();
    }
    return m_pInstance;
}

Estimator::Estimator() {}

Estimator::~Estimator() {}

int Estimator::init(float nFocalLength, int nWidth, int nHeight) {
    stCamera = colmap::Camera::CreateFromModelId(
      kInvalidCameraId, CameraModelId::kPinhole, nFocalLength, nWidth, nHeight);
    return 0;
}

int Estimator::deinit() {}

int Estimator::estimate(float** pPoint2D, size_t nPoint2D, 
                        float** pPoint3D, size_t nPoint3D,
                        float** pM) {
    int res = estimateEPnP(pPoint2D, nPoint2D, pPoint3D, nPoint3D, pM);
    if (res) {
        return 0;
    }
    // 
    return 0;           
}

int Estimator::estimateEPnP(float** pPoint2D, size_t nPoint2D, 
                        float** pPoint3D, size_t nPoint3D,
                        float** pM) {
    std::vector<Eigen::Vector3d> points3D;
    for (size_t i = 0; i < nPoint3D; ++i) {
        points3D.emplace_back(Eigen::Vector3d(**pPoint3D, *(*(pPoint3D) + 1), *(*(pPoint3D) + 2)));
        pPoint3D++;
    }

    std::vector<EPNPEstimator::X_t> points2D;
    for (size_t i = 0; i < nPoint2D; ++i) {
        auto& point2D = points2D.emplace_back();
        point2D.image_point = Eigen::Vector2d(**pPoint2D, *(*(pPoint2D) + 1));
        pPoint2D++;
    }

    auto points3D_faulty = points3D;
    for (size_t i = 0; i < points3D.size(); ++i) {
        points3D_faulty[i](0) = 20;
    }

    // const colmap::Camera camera = colmap::Camera::CreateFromModelId(
    //   kInvalidCameraId, CameraModelId::kPinhole, 12, 34, 56);
    auto img_from_cam_func =
        std::bind(&Camera::ImgFromCam, &stCamera, std::placeholders::_1);

    RANSACOptions options;
    options.max_error = 1e-5;
    RANSAC<EPNPEstimator> ransac(options, EPNPEstimator(img_from_cam_func));
    const auto report = ransac.Estimate(points2D, points3D);
    if (!report.success) {
        return -1;
    }

    // Test residuals of exact points.
    std::vector<double> residuals;
    ransac.estimator.Residuals(points2D, points3D, report.model, &residuals);

    // Test residuals of faulty points.
    ransac.estimator.Residuals(
        points2D, points3D_faulty, report.model, &residuals);

    
    return 0;    
}
