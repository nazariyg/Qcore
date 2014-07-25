//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetEquationRgb (EquationType eEquation)
{
    EquationRgb = eEquation;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetEquationAlpha (EquationType eEquation)
{
    EquationAlpha = eEquation;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetSrcFactorRgb (FactorType eFactor)
{
    SrcFactorRgb = eFactor;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetDstFactorRgb (FactorType eFactor)
{
    DstFactorRgb = eFactor;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetSrcFactorAlpha (FactorType eFactor)
{
    SrcFactorAlpha = eFactor;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetDstFactorAlpha (FactorType eFactor)
{
    DstFactorAlpha = eFactor;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::SetBlendingColor (const ColorRgba& rqColor)
{
    BlendingColor = rqColor;
    BlendingColorSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void BlendingState::UnsetBlendingColor ()
{
    BlendingColorSet = false;
}
//------------------------------------------------------------------------------------------------------------------



