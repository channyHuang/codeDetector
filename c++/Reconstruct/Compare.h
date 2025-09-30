#pragma once

#include <Eigen/Eigen>

class Compare {
public:
    Compare();
    virtual ~Compare();

    void init();
    std::tuple<float, float> calcError(const Eigen::Quaternionf& R_GC_quat, 
                        const Eigen::Vector3f& t_GC, 
                        const Eigen::Quaternionf& R_WI_quat, 
                        const Eigen::Vector3f& t_WI); 
    std::tuple<float, float> calcError(const Eigen::Vector3f& R_GC_vec,
                        const Eigen::Vector3f& t_GC,
                        const Eigen::Vector3f& R_WI_vec,
                        const Eigen::Vector3f& t_WI); 

private:
    Eigen::Matrix4f T_CI;
    Eigen::Matrix<float, 4, 4> T_IC;
    Eigen::Matrix<float, 4, 4> T_WG;
};
