#include "QHRMaterial.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
HRMaterial::HRMaterial (FaceType eFace, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
    const ColorRgba& rqSpecular, float fShininess, const ColorRgba& rqEmission)
    :
    Face(eFace),
    Ambient(rqAmbient),
    Diffuse(rqDiffuse),
    Specular(rqSpecular),
    Shininess(fShininess),
    Emission(rqEmission)
{
}
//------------------------------------------------------------------------------------------------------------------



