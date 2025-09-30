
#ifndef LIBEXPORTPNP_H
#define LIBEXPORTPNP_H

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

// ======================================
// init
D_EXTERN_C D_SHARE_EXPORT int initPnP(float nFocalLength = 190.97, int nWidth = 1024, int nHeight = 1024);
// estimate 
D_EXTERN_C D_SHARE_EXPORT int estimatePnP(float** pPoint2D, size_t nPoint2D, float** pPoint3D, size_t nPoint3D, float** pM);
// deinit
D_EXTERN_C D_SHARE_EXPORT int deinitPnP();

#endif // LIBEXPORTPNP_H