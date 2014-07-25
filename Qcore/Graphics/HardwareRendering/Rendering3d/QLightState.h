#ifndef QLIGHTSTATE_H
#define QLIGHTSTATE_H

#include "QStateBase.h"
#include "QHRLight3.h"
#include "QModestArray.h"

namespace Q
{

class Q_ITEM LightState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum ShadeModelType
    {
        SMT_FLAT,
        SMT_SMOOTH
    };

    // construction
    LightState ();  // empty

    // member access
    void SetShadeModel (ShadeModelType eShadeModel);
    void UnsetShadeModel ();
    void SetGlobalAmbient (const ColorRgba& rqColor);
    void UnsetGlobalAmbient ();
    void SetTwoSided (bool bTwoSided);
    void UnsetTwoSided ();
    void SetActualViewpointForSpecular (bool bSet);
    void UnsetActualViewpointForSpecular ();
    void SetSeparateSpecular (bool bSeparate);
    void UnsetSeparateSpecular ();

    int GetLightQuantity () const;
    int AddLight (const HRLight3Ptr& rspqLight);
    const HRLight3Ptr& GetLight (int i) const;
    void RemoveLight (int i);

    bool ShadeModelSet;                     // default: false
    ShadeModelType ShadeModel;
    bool GlobalAmbientSet;                  // default: false
    ColorRgba GlobalAmbient;
    bool TwoSidedSet;                       // default: false
    bool TwoSided;
    bool ActualViewpointForSpecularSet;     // default: false
    bool ActualViewpointForSpecular;
    bool SeparateSpecularSet;               // default: false
    bool SeparateSpecular;

private:
    ModestArray<HRLight3Ptr> m_qLights;
};

#include "QLightState.inl"

typedef SmartPointer<LightState> LightStatePtr;

}

#endif



