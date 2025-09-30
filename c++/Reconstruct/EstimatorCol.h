#pragma once

#include "camera.h"

class EstimatorCol {
public:
    EstimatorCol();
    virtual ~EstimatorCol();
    static EstimatorCol* getInstance();

    int init(float nFocalLength, int nWidth, int nHeight);
    int deinit();

    int estimate(float** pPoint2D, size_t nPoint2D, 
                float** pPoint3D, size_t nPoint3D,
                float** pM);

private:
    int estimateEPnP(float** pPoint2D, size_t nPoint2D, 
                        float** pPoint3D, size_t nPoint3D,
                        float** pM);

private:
    static EstimatorCol* m_pInstance;
    colmap::Camera stCamera;
};