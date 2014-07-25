//------------------------------------------------------------------------------------------------------------------
inline void LightState::SetShadeModel (ShadeModelType eShadeModel)
{
    ShadeModel = eShadeModel;
    ShadeModelSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::UnsetShadeModel ()
{
    ShadeModelSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::SetGlobalAmbient (const ColorRgba& rqColor)
{
    GlobalAmbient = rqColor;
    GlobalAmbientSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::UnsetGlobalAmbient ()
{
    GlobalAmbientSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::SetTwoSided (bool bTwoSided)
{
    TwoSided = bTwoSided;
    TwoSidedSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::UnsetTwoSided ()
{
    TwoSidedSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::SetActualViewpointForSpecular (bool bSet)
{
    ActualViewpointForSpecular = bSet;
    ActualViewpointForSpecularSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::UnsetActualViewpointForSpecular ()
{
    ActualViewpointForSpecularSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::SetSeparateSpecular (bool bSeparate)
{
    SeparateSpecular = bSeparate;
    SeparateSpecularSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void LightState::UnsetSeparateSpecular ()
{
    SeparateSpecularSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline int LightState::GetLightQuantity () const
{
    return m_qLights.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline const HRLight3Ptr& LightState::GetLight (int i) const
{
    return m_qLights[i];
}
//------------------------------------------------------------------------------------------------------------------



