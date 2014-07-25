#ifndef QHRLIGHT3_H
#define QHRLIGHT3_H

#include "QSmartPointer.h"
#include "QTransformableNoScale3.h"

namespace Q
{

class Q_ITEM HRLight3 : public TransformableNoScale3<float,true>, public Heap
{
public:
    enum SourceType
    {
        ST_DIRECTIONAL,
        ST_POINT,
        ST_SPOT
    };

    // Construction.

    // directional
    HRLight3 (const Vector3f& rqDirection, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
        const ColorRgba& rqSpecular);

    // point
    HRLight3 (const Vector3f& rqPosition, const ColorRgba& rqAmbient, const ColorRgba& rqDiffuse,
        const ColorRgba& rqSpecular, float fAttenConstant, float fAttenLinear, float fAttenQuadratic);

    // spot
    HRLight3 (const Vector3f& rqPosition, const Vector3f& rqDirection, const ColorRgba& rqAmbient,
        const ColorRgba& rqDiffuse, const ColorRgba& rqSpecular, float fAttenConstant, float fAttenLinear,
        float fAttenQuadratic, float fSpotExponent, float fSpotCutoffDeg);

    // member access
    void SetAmbient (const ColorRgba& rqColor);
    void SetDiffuse (const ColorRgba& rqColor);
    void SetSpecular (const ColorRgba& rqColor);
    void SetPosition (const Vector3f& rqPosition);
    void SetDirection (const Vector3f& rqDirection);
    void SetAttenConstant (float fAttenConstant);
    void SetAttenLinear (float fAttenLinear);
    void SetAttenQuadratic (float fAttenQuadratic);
    void SetSpotExponent (float fSpotExponent);
    void SetSpotCutoff (float fSpotCutoffDeg);
    void SetOn (bool bOn);

    SourceType Source;
    ColorRgba Ambient;
    ColorRgba Diffuse;
    ColorRgba Specular;
    Vector3f Direction;
    float AttenConstant;
    float AttenLinear;
    float AttenQuadratic;
    float SpotExponent;
    float SpotCutoffDeg;
    bool On;
};

#include "QHRLight3.inl"

typedef SmartPointer<HRLight3> HRLight3Ptr;

}

#endif



