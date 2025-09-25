#include "libExportCVAruco.h"

#include <memory>
#include <fstream>

#include "CVArucoDetector.h"

extern "C"
// ======================================
void* init() {
    return CVArucoDetector::getInstance()->init();
}

stIdsPoints* detect(void* pHandle, stFrame* pFrame) {
    if (pHandle == nullptr || pFrame == nullptr) {
        printf("Error: input param is null\n");
        return nullptr;
    }
    return CVArucoDetector::getInstance()->detect(pHandle, pFrame);
}

Frame* drawResult(void* pHandle, stIdsPoints* pResult, stFrame* pFrame) {
    if (pHandle == nullptr || pResult == nullptr || pFrame == nullptr) {
        printf("Error: input param is null\n");
        return nullptr;
    }
    return CVArucoDetector::getInstance()->drawResult(pHandle, pResult, pFrame);
}

int deinit(void* pHandle) {
    if (pHandle == nullptr) return -1;
    return CVArucoDetector::getInstance()->deinit(pHandle);
}
