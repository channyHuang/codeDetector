#include "libExportPnP.h"

#include <memory>
#include <fstream>

#include "EstimatorCol.h"

extern "C"
// ======================================
int initPnP(float nFocalLength, int nWidth, int nHeight) {
    return EstimatorCol::getInstance()->init(nFocalLength, nWidth, nHeight);
}

int estimatePnP(float** pPoint2D, size_t nPoint2D, float** pPoint3D, size_t nPoint3D, float** pM) {
    if (pPoint2D == nullptr || pPoint3D == nullptr) {
        return -1;
    }
    return EstimatorCol::getInstance()->estimate(pPoint2D, nPoint2D, pPoint3D, nPoint3D, pM);
}

int deinitPnP() {
    return EstimatorCol::getInstance()->deinit();
}
