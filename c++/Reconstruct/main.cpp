#include <vector>
#include <string.h>

#include "camera.h"
#include "rigid3.h"
#include "absolute_pose.h"
#include "ransac.h"
#include "gtest.h"

using namespace colmap;

#include "libExportPnP.h"

#include "common.hpp"

int test() {
  const std::vector<Eigen::Vector3d> points3D = {
      Eigen::Vector3d(1, 1, 1),
      Eigen::Vector3d(0, 1, 1),
      Eigen::Vector3d(3, 1.0, 4),
      Eigen::Vector3d(3, 1.1, 4),
      Eigen::Vector3d(3, 1.2, 4),
      Eigen::Vector3d(3, 1.3, 4),
      Eigen::Vector3d(3, 1.4, 4),
      Eigen::Vector3d(2, 1, 7),
  };

  auto points3D_faulty = points3D;
  for (size_t i = 0; i < points3D.size(); ++i) {
    points3D_faulty[i](0) = 20;
  }

  const Camera camera = Camera::CreateFromModelId(
      kInvalidCameraId, CameraModelId::kPinhole, 12, 34, 56);
  auto img_from_cam_func =
      std::bind(&Camera::ImgFromCam, &camera, std::placeholders::_1);
  
  // NOLINTNEXTLINE(clang-analyzer-security.FloatLoopCounter)
  for (double qx = 0; qx < 1; qx += 0.2) {
    // NOLINTNEXTLINE(clang-analyzer-security.FloatLoopCounter)
    for (double tx = 0; tx < 1; tx += 0.1) {
      const Rigid3d expected_cam_from_world(
          Eigen::Quaterniond(1, qx, 0, 0).normalized(),
          Eigen::Vector3d(tx, 0, 0));

      // Project points to camera coordinate system.
      std::vector<EPNPEstimator::X_t> points2D;
      for (size_t i = 0; i < points3D.size(); ++i) {
        auto& point2D = points2D.emplace_back();
        point2D.camera_ray =
            (expected_cam_from_world * points3D[i]).normalized();
        point2D.image_point = img_from_cam_func(point2D.camera_ray).value();
      }

      RANSACOptions options;
      options.max_error = 1e-5;
      RANSAC<EPNPEstimator> ransac(options, EPNPEstimator(img_from_cam_func));
      const auto report = ransac.Estimate(points2D, points3D);

      EXPECT_TRUE(report.success);
      EXPECT_LT((expected_cam_from_world.ToMatrix() - report.model).norm(), 1e-3);

      // Test residuals of exact points.
      std::vector<double> residuals;
      ransac.estimator.Residuals(points2D, points3D, report.model, &residuals);
      for (size_t i = 0; i < residuals.size(); ++i) {
        EXPECT_LT(residuals[i], 1e-3);
      }

      // Test residuals of faulty points.
      ransac.estimator.Residuals(
          points2D, points3D_faulty, report.model, &residuals);
      for (size_t i = 0; i < residuals.size(); ++i) {
        EXPECT_GT(residuals[i], 0.1);
      }
    }
  }

    return 0;
}


int main() {
    std::vector<std::vector<float>> points2d = {
 {402., 172.},
 {452, 171.},
 {456, 130.},
 {405, 130.},
 {473, 174.},
 {522, 180.},
 {530, 139.},
 {481, 131.},
 {333, 113.}, 
 {384, 110.}, 
 {390,  71.}, 
 {341,  72.}, 
 {484, 113.}, 
 {535, 116.}, 
 {539,  79.}, 
 {489,  73.}, 
 {266, 187.}, 
 {310, 178.}, 
 {310, 137.}, 
 {266, 146.}, 
    };

    std::vector<std::vector<float>> points3d = {
      { 0.528,  -0.1848,  0.    },
 {  0.616, 0.1848,  0.    }, 
 {  0.616, 0.2728,  0.    }, 
 {  0.528, 0.2728,  0.    }, 
 {  0.6424, -0.1848,  0.    }, 
 {  0.7304, -0.1848,  0.    }, 
 {  0.7304, -0.2728,  0.    }, 
 {  0.6424, -0.2728,  0.    }, 
 {  0.4136, -0.2992,  0.    }, 
 {  0.5016, -0.2992,  0.    }, 
 {  0.5016, -0.3872,  0.    }, 
 {  0.4136, -0.3872,  0.    }, 
 {  0.6424, -0.2992,  0.    }, 
 {  0.7304, -0.2992,  0.    }, 
 {  0.7304, -0.3872,  0.    }, 
 {  0.6424, -0.3872,  0.    }, 
 {  0.2992, -0.1848,  0.    }, 
 {  0.3872, -0.1848,  0.    }, 
 {  0.3872, -0.2728,  0.    }, 
 {  0.2992, -0.2728,  0.    }, 
 };
    
    std::vector<std::vector<float>> pM = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    auto pPoints2D = convertVectorToFloatPtr(points2d);
    auto pPoints3D = convertVectorToFloatPtr(points3d);
    auto pMat = convertVectorToFloatPtr(pM);

    init();
    int res = estimate(pPoints2D, 20, pPoints3D, 20, pMat);
    printf("res = %d\n", res);
    for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        printf("%f ", pM[i][j]);
      }
      printf("\n");
    }

    return 0;
}