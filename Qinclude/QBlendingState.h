#ifndef QBLENDINGSTATE_H
#define QBLENDINGSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM BlendingState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum EquationType
    {
        ET_ADD,
        ET_SUBTRACT,
        ET_REVERSE_SUBTRACT,
        ET_MIN,
        ET_MAX,
        ET_LOGIC_OP
    };

    enum FactorType
    {
        FT_ZERO,
        FT_ONE,
        FT_SRC_COLOR,
        FT_ONE_MINUS_SRC_COLOR,
        FT_DST_COLOR,
        FT_ONE_MINUS_DST_COLOR,
        FT_SRC_ALPHA,
        FT_ONE_MINUS_SRC_ALPHA,
        FT_DST_ALPHA,
        FT_ONE_MINUS_DST_ALPHA,
        FT_CONST_COLOR,
        FT_ONE_MINUS_CONST_COLOR,
        FT_CONST_ALPHA,
        FT_ONE_MINUS_CONST_ALPHA,
        FT_SRC_ALPHA_SATURATE
    };

    // Construction.

    // without rgb-alpha separation
    BlendingState (EquationType eEquation, FactorType eSrcFactor, FactorType eDstFactor);

    // equation separate
    BlendingState (EquationType eEquationRgb, EquationType eEquationAlpha, FactorType eSrcFactor,
        FactorType eDstFactor);

    // function separate
    BlendingState (EquationType eEquation, FactorType eSrcFactorRgb, FactorType eDstFactorRgb,
        FactorType eSrcFactorAlpha, FactorType eDstFactorAlpha);

    // all separate
    BlendingState (EquationType eEquationRgb, EquationType eEquationAlpha, FactorType eSrcFactorRgb,
        FactorType eDstFactorRgb, FactorType eSrcFactorAlpha, FactorType eDstFactorAlpha);

    // member access
    void SetEquationRgb (EquationType eEquation);
    void SetEquationAlpha (EquationType eEquation);
    void SetSrcFactorRgb (FactorType eFactor);
    void SetDstFactorRgb (FactorType eFactor);
    void SetSrcFactorAlpha (FactorType eFactor);
    void SetDstFactorAlpha (FactorType eFactor);
    void SetBlendingColor (const ColorRgba& rqColor);
    void UnsetBlendingColor ();

    EquationType EquationRgb;
    EquationType EquationAlpha;
    FactorType SrcFactorRgb;
    FactorType DstFactorRgb;
    FactorType SrcFactorAlpha;
    FactorType DstFactorAlpha;
    bool BlendingColorSet;  // default: false
    ColorRgba BlendingColor;
};

#include "QBlendingState.inl"

typedef SmartPointer<BlendingState> BlendingStatePtr;

}

#endif



