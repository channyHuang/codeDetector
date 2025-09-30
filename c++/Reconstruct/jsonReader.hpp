#include "json.hpp"

#include <cstring>
#include <fstream>

bool readPoseJson(const std::string &sFileName, std::vector<float> &vR, std::vector<float> &vt) {
    std::ifstream ifs(sFileName, std::ios::in);
    nlohmann::json stJsonData = nlohmann::json::parse(ifs);
    ifs.close();

    if (stJsonData.empty()) {
        printf("Error: parse file error\n");
        return false;
    }
    if (!stJsonData.contains("transform")) {
        printf("Error: parse file has no transform\n");
        return false;
    }
    nlohmann::json stJsonPose = stJsonData["transform"];
    if (!stJsonPose.contains("translation") || !stJsonPose.contains("rotation")) {
        printf("Error: parse file has no translation or rotation\n");
        return false;
    }
    nlohmann::json stJsonTr = stJsonPose["translation"];
    if (!stJsonTr.contains("x") || !stJsonTr.contains("y") || !stJsonTr.contains("z")) {
        printf("Error: parse file has no x/y/z\n");
        return false;
    }
    nlohmann::json stJsonRot = stJsonPose["rotation"];
    if (!stJsonRot.contains("quaternion")) {
        return false;
    }
    nlohmann::json stJsonRQuat = stJsonRot["quaternion"];
    if (!stJsonRQuat.contains("x") || !stJsonRQuat.contains("y") || !stJsonRQuat.contains("z") || !stJsonRQuat.contains("w")) {
        printf("Error: parse file has no x/y/z/w\n");
        return false;
    }

    vR.push_back(stJsonRQuat["w"]);
    vR.push_back(stJsonRQuat["x"]);
    vR.push_back(stJsonRQuat["y"]);
    vR.push_back(stJsonRQuat["z"]);

    vt.push_back(stJsonTr["x"]);
    vt.push_back(stJsonTr["y"]);
    vt.push_back(stJsonTr["z"]);

    return true;
}