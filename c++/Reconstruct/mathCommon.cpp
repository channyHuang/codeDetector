#include "mathCommon.h"

float* flattenVector(const std::vector<std::vector<float>>& vec, size_t& total_size) {
    total_size = std::accumulate(vec.begin(), vec.end(), 0ull,
        [](size_t sum, const auto& row) { return sum + row.size(); });
    
    if (total_size == 0) return nullptr;
    
    float* result = new float[total_size];
    float* current = result;
    
    for (const auto& row : vec) {
        current = std::copy(row.begin(), row.end(), current);
    }
    
    return result;
}

float** convertVectorToFloatPtr(const std::vector<std::vector<float>>& vec) {
    if (vec.empty()) return nullptr;
    
    // Allocate memory for row pointers
    float** result = new float*[vec.size()];
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i].empty()) {
            result[i] = nullptr;
            continue;
        }
        
        // Allocate memory for each row and copy data
        result[i] = new float[vec[i].size()];
        std::copy(vec[i].begin(), vec[i].end(), result[i]);
    }
    
    return result;
}



Eigen::Quaternionf rvec_to_quaternion(const Eigen::Vector3f& rvec) {
    /**
     * Convert a rotation vector (axis-angle) to a unit quaternion.
     * 
     * @param rvec: A 3-element rotation vector [r_x, r_y, r_z].
     *              Its direction is the axis and its magnitude is the angle in radians.
     * 
     * @return: A 4-element unit quaternion [w, i, j, k].
     */
    
    float angle = rvec.norm();
    
    // If the angle is 0, return the identity quaternion to avoid division by zero
    if (std::abs(angle) < 1e-8f) {
        return Eigen::Quaternionf(1.0f, 0.0f, 0.0f, 0.0f);
    }
    
    // Normalize the rotation vector to get the unit axis
    Eigen::Vector3f axis = rvec / angle;
    
    float half_angle = angle * 0.5f;
    float w = std::cos(half_angle);
    float sin_half = std::sin(half_angle);
    
    float i = axis[0] * sin_half;
    float j = axis[1] * sin_half;
    float k = axis[2] * sin_half;
    
    return Eigen::Quaternionf(w, i, j, k);
}

Eigen::Matrix3f quaternion_to_rotation_matrix(const Eigen::Quaternionf& quat) {
    /**
     * Convert quaternion to rotation matrix
     * 
     * @param quat: Eigen quaternion [w, x, y, z]
     * @return: 3x3 rotation matrix
     */
    return quat.normalized().toRotationMatrix();
}

// 重载版本：接受 Vector4f 输入
Eigen::Matrix3f quaternion_to_rotation_matrix(const Eigen::Vector4f& quat_vec) {
    Eigen::Quaternionf quat(quat_vec[0], quat_vec[1], quat_vec[2], quat_vec[3]);
    return quat.normalized().toRotationMatrix();
}

// 重载版本：接受分开的四个分量
Eigen::Matrix3f quaternion_to_rotation_matrix(float w, float x, float y, float z) {
    Eigen::Quaternionf quat(w, x, y, z);
    return quat.normalized().toRotationMatrix();
}

Eigen::Matrix4f get_transform_matrix(const Eigen::Matrix3f& rotation, 
                                   const Eigen::Vector3f& translation) {
    /**
     * 从旋转矩阵和平移向量构建4x4变换矩阵
     * 
     * @param rotation: 3x3旋转矩阵
     * @param translation: 3x1平移向量
     * @return: 4x4齐次变换矩阵
     */
    Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
    T.block<3, 3>(0, 0) = rotation;
    T.block<3, 1>(0, 3) = translation;
    return T;
}

std::tuple<float, float> calcPoseError(const Eigen::Matrix4f& T_meas, 
                                          const Eigen::Matrix4f& T_gt) {
    /**
     * 计算两个位姿之间的误差
     * 
     * @param T_meas: 测量得到的变换矩阵
     * @param T_gt: 真实值（ground truth）变换矩阵
     * @return: 旋转误差（弧度）, 平移误差
     */
    
    // 提取旋转矩阵
    Eigen::Matrix3f R_meas = T_meas.block<3, 3>(0, 0);
    Eigen::Matrix3f R_gt = T_gt.block<3, 3>(0, 0);
    
    // 计算相对旋转误差：R_err = R_gt^T * R_meas
    Eigen::Matrix3f R_err = R_gt.transpose() * R_meas;
    
    // 将旋转矩阵转换为角轴表示，获取角度（弧度）
    Eigen::AngleAxisf angle_axis(R_err);
    float angle_err = angle_axis.angle();
    
    // 提取平移向量并计算欧氏距离
    Eigen::Vector3f t_meas = T_meas.block<3, 1>(0, 3);
    Eigen::Vector3f t_gt = T_gt.block<3, 1>(0, 3);
    float trans_err = (t_gt - t_meas).norm();
    
    return std::make_tuple(angle_err, trans_err);
}
