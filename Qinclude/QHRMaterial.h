#ifndef QHRMATERIAL_H
#define QHRMATERIAL_H

#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM HRMaterial : public Heap
{
public:
    enum FaceType
    {
        FT_FRONT,
        FT_BACK,
        FT_BOTH
    };

    // Construction.  fShininess must be in [0.0, 128.0].
    HRMaterial (FaceType eFace, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
        const ColorRgba& rqSpecular, float fShininess, const ColorRgba& rqEmission);

    FaceType Face;
    ColorRgba Ambient;
    ColorRgba Diffuse;
    ColorRgba Specular;
    float Shininess;
    ColorRgba Emission;
};

typedef SmartPointer<HRMaterial> HRMaterialPtr;

}

#endif



