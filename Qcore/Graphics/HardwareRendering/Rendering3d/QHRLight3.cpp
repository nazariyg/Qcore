#include "QHRLight3.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
HRLight3::HRLight3 (const Vector3f& rqDirection, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
    const ColorRgba& rqSpecular)
    :
    Source(ST_DIRECTIONAL),
    Ambient(rqAmbient),
    Diffuse(rqDiffuse),
    Specular(rqSpecular),
    Direction(rqDirection),
    On(true)
{
}
//------------------------------------------------------------------------------------------------------------------
HRLight3::HRLight3 (const Vector3f& rqPosition, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
    const ColorRgba& rqSpecular, float fAttenConstant, float fAttenLinear, float fAttenQuadratic)
    :
    Source(ST_POINT),
    Ambient(rqAmbient),
    Diffuse(rqDiffuse),
    Specular(rqSpecular),
    AttenConstant(fAttenConstant),
    AttenLinear(fAttenLinear),
    AttenQuadratic(fAttenQuadratic),
    On(true)
{
    SetTranslate(rqPosition);
}
//------------------------------------------------------------------------------------------------------------------
HRLight3::HRLight3 (const Vector3f& rqPosition, const Vector3f& rqDirection, const ColorRgba& rqAmbient,
    const ColorRgba& rqDiffuse, const ColorRgba& rqSpecular, float fAttenConstant, float fAttenLinear,
    float fAttenQuadratic, float fSpotExponent, float fSpotCutoffDeg)
    :
    Source(ST_SPOT),
    Ambient(rqAmbient),
    Diffuse(rqDiffuse),
    Specular(rqSpecular),
    Direction(rqDirection),
    AttenConstant(fAttenConstant),
    AttenLinear(fAttenLinear),
    AttenQuadratic(fAttenQuadratic),
    SpotExponent(fSpotExponent),
    SpotCutoffDeg(fSpotCutoffDeg),
    On(true)
{
    SetTranslate(rqPosition);
}
//------------------------------------------------------------------------------------------------------------------



