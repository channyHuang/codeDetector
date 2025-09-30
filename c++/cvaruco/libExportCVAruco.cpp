#include "libExportCVAruco.h"

#include <memory>
#include <fstream>

#include "CVArucoDetector.h"

extern "C"
// ======================================
void* initCVAruco() {
    return CVArucoDetector::getInstance()->init();
}

int setParams(void* pHandle, const char* pData, size_t nDataLen) {
    if (pHandle == nullptr || pData == nullptr || nDataLen <= 0) {
        printf("Error: input param is null\n");
        return -1;
    }
    return CVArucoDetector::getInstance()->setParams(pHandle, pData, nDataLen);
}

int updateParams(void* pHandle, const char* pDetectParamFile, const char* pRefineParamFile) {
    if (pHandle == nullptr) {
        printf("Error: input param is null\n");
        return -1;
    }
    return CVArucoDetector::getInstance()->updateParams(pHandle, pDetectParamFile, pRefineParamFile);
}

int outputParams(void* pHandle, const char* pDetectParamName, const char* pRefineParamName) {
    if (pDetectParamName == nullptr && pRefineParamName == nullptr) {
        return -1;
    }
    return CVArucoDetector::getInstance()->outputParams(pHandle, pDetectParamName, pRefineParamName);
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

int deinitCVAruco(void* pHandle) {
    if (pHandle == nullptr) return -1;
    return CVArucoDetector::getInstance()->deinit(pHandle);
}
