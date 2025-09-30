
#ifndef LIBEXPORTCVARUCO_H
#define LIBEXPORTCVARUCO_H

#include <string.h>
#include <string>

#ifdef __cplusplus
#define D_EXTERN_C extern "C"
#else
#define D_EXTERN_C
#endif

#define __SHARE_EXPORT

#ifdef __SHARE_EXPORT
#define D_SHARE_EXPORT D_DECL_EXPORT
#else
#define D_SHARE_EXPORT D_DECL_IMPORT
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#define D_CALLTYPE __stdcall
#define D_DECL_EXPORT __declspec(dllexport)
#define D_DECL_IMPORT
#else
#define __stdcall
#define D_CALLTYPE
#define D_DECL_EXPORT __attribute__((visibility("default")))
#define D_DECL_IMPORT __attribute__((visibility("default")))
#endif

// Image struct
typedef struct stFrame {
    // width
    int col = 0;
    // height
    int row = 0;
    // format
    int fmt = 0;
    unsigned int size = 0;
    // image data
    unsigned char* pChar = nullptr;
} Frame;

typedef struct stIdsPoints {
    int count = 0;
    int* ids = nullptr;
    int* corners = nullptr;
    int reject_count = 0;
    int* rejects = nullptr;
} IdsPoints;

// ======================================
// init
D_EXTERN_C D_SHARE_EXPORT void* initCVAruco();
// set params
D_EXTERN_C D_SHARE_EXPORT int setParams(void* pHandle, const char* pData, size_t nDataLen);
D_EXTERN_C D_SHARE_EXPORT int updateParams(void* pHandle, 
                                            const char* pDetectParamFile = nullptr, 
                                            const char* pRefineParamFile = nullptr);
D_EXTERN_C D_SHARE_EXPORT int outputParams(void* pHandle, const char* pDetectParamName = nullptr, const char* pRefineParamName = nullptr);
// detect
D_EXTERN_C D_SHARE_EXPORT stIdsPoints* detect(void* pHandle, stFrame* pFrame);
// draw
D_EXTERN_C D_SHARE_EXPORT Frame* drawResult(void* pHandle, stIdsPoints* pResult, stFrame* pFrame);
// deinit
D_EXTERN_C D_SHARE_EXPORT int deinitCVAruco(void* pHandle);

#endif // LIBEXPORTCVARUCO_H