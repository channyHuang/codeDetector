#include "libExportPnP.h"

#include <memory>
#include <fstream>

#include "Estimator.h"

extern "C"
// ======================================
int init(float nFocalLength, int nWidth, int nHeight) {
    return Estimator::getInstance()->init(nFocalLength, nWidth, nHeight);
}

int estimate(float** pPoint2D, size_t nPoint2D, float** pPoint3D, size_t nPoint3D, float** pM) {
    if (pPoint2D == nullptr || pPoint3D == nullptr) {
        return -1;
    }
    return Estimator::getInstance()->estimate(pPoint2D, nPoint2D, pPoint3D, nPoint3D, pM);
}

int deinit() {
    return Estimator::getInstance()->deinit();
}
