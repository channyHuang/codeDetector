#include "Compare.h"

#include <tuple>

#include "mathCommon.h"

Compare::Compare() {}
Compare::~Compare() {}

void Compare::init() {
    T_CI << -0.9995250378696743, 0.029615343885863205, -0.008522328211654736, 0.04727988224914392,
        0.0075019185074052044, -0.03439736061393144, -0.9993800792498829, -0.047443232143367084,
        -0.02989013031643309, -0.998969345370175, 0.03415885127385616, -0.0681999605066297,
        0.0, 0.0, 0.0, 1.0;

    T_IC << -0.99952504,  0.00750192, -0.02989013,  0.04557484,
        0.02961534, -0.03439736, -0.99896935, -0.0711618,
        -0.00852233, -0.99938008,  0.03415885, -0.04468125,
        0.0, 0.0, 0.0, 1.0;

    T_WG << -0.999641,   0.0266075, -0.00313757,    0.146549,
        -0.00698843,   -0.145899,    0.989275,   -0.192952,
        0.0258644,    0.988942,    0.146032,    0.766471,
        0.0, 0.0, 0.0, 1.0;
}

std::tuple<float, float> Compare::calcError(const Eigen::Quaternionf& R_GC_quat, 
                        const Eigen::Vector3f& t_GC, 
                        const Eigen::Quaternionf& R_WI_quat, 
                        const Eigen::Vector3f& t_WI) {
    Eigen::Matrix3f R_GC = quaternion_to_rotation_matrix(R_GC_quat);
    Eigen::Matrix3f R_WI = quaternion_to_rotation_matrix(R_WI_quat);

    Eigen::Matrix4f T_GC = get_transform_matrix(R_GC, t_GC);
    Eigen::Matrix4f T_WI = get_transform_matrix(R_WI, t_WI);

    Eigen::Matrix4f T_WC_means = T_WG * T_GC;
    Eigen::Matrix4f T_WC_gt = T_WI * T_IC;

    std::tuple<float, float> tErrors = calcPoseError(T_WC_means, T_WC_gt);

    return tErrors;
}

std::tuple<float, float> Compare::calcError(const Eigen::Vector3f& R_GC_vec,
                        const Eigen::Vector3f& t_GC,
                        const Eigen::Vector3f& R_WI_vec,
                        const Eigen::Vector3f& t_WI) {
    Eigen::Quaternionf R_GC_quat = rvec_to_quaternion(R_GC_vec);
    Eigen::Quaternionf R_WI_quat = rvec_to_quaternion(R_WI_vec);
    return calcError(R_GC_quat, t_GC, R_WI_quat, t_WI);
}
