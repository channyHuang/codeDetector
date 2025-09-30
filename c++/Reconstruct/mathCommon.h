#pragma once 

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <tuple>

#include <Eigen/Geometry>

float* flattenVector(const std::vector<std::vector<float>>& vec, size_t& total_size);
float** convertVectorToFloatPtr(const std::vector<std::vector<float>>& vec);
Eigen::Quaternionf rvec_to_quaternion(const Eigen::Vector3f& rvec);
Eigen::Matrix3f quaternion_to_rotation_matrix(const Eigen::Quaternionf& quat);
Eigen::Matrix3f quaternion_to_rotation_matrix(const Eigen::Vector4f& quat_vec);
Eigen::Matrix3f quaternion_to_rotation_matrix(float w, float x, float y, float z);

Eigen::Matrix4f get_transform_matrix(const Eigen::Matrix3f& rotation, 
                                   const Eigen::Vector3f& translation);

std::tuple<float, float> calcPoseError(const Eigen::Matrix4f& T_meas, 
                                          const Eigen::Matrix4f& T_gt);
                                          