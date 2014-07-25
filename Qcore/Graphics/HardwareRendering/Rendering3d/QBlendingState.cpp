#include "QBlendingState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,BlendingState);

//------------------------------------------------------------------------------------------------------------------
BlendingState::BlendingState (EquationType eEquation, FactorType eSrcFactor,
    FactorType eDstFactor)
    :
    EquationRgb(eEquation),
    EquationAlpha(eEquation),
    SrcFactorRgb(eSrcFactor),
    DstFactorRgb(eDstFactor),
    SrcFactorAlpha(eSrcFactor),
    DstFactorAlpha(eDstFactor),
    BlendingColorSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
BlendingState::BlendingState (EquationType eEquationRgb, EquationType eEquationAlpha, FactorType eSrcFactor,
    FactorType eDstFactor)
    :
    EquationRgb(eEquationRgb),
    EquationAlpha(eEquationAlpha),
    SrcFactorRgb(eSrcFactor),
    DstFactorRgb(eDstFactor),
    SrcFactorAlpha(eSrcFactor),
    DstFactorAlpha(eDstFactor),
    BlendingColorSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
BlendingState::BlendingState (EquationType eEquation, FactorType eSrcFactorRgb, FactorType eDstFactorRgb,
    FactorType eSrcFactorAlpha, FactorType eDstFactorAlpha)
    :
    EquationRgb(eEquation),
    EquationAlpha(eEquation),
    SrcFactorRgb(eSrcFactorRgb),
    DstFactorRgb(eDstFactorRgb),
    SrcFactorAlpha(eSrcFactorAlpha),
    DstFactorAlpha(eDstFactorAlpha),
    BlendingColorSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
BlendingState::BlendingState (EquationType eEquationRgb, EquationType eEquationAlpha, FactorType eSrcFactorRgb,
    FactorType eDstFactorRgb, FactorType eSrcFactorAlpha, FactorType eDstFactorAlpha)
    :
    EquationRgb(eEquationRgb),
    EquationAlpha(eEquationAlpha),
    SrcFactorRgb(eSrcFactorRgb),
    DstFactorRgb(eDstFactorRgb),
    SrcFactorAlpha(eSrcFactorAlpha),
    DstFactorAlpha(eDstFactorAlpha),
    BlendingColorSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------



