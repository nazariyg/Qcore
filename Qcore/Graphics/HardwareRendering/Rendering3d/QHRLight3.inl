//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetAmbient (const ColorRgba& rqColor)
{
    Ambient = rqColor;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetDiffuse (const ColorRgba& rqColor)
{
    Diffuse = rqColor;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetSpecular (const ColorRgba& rqColor)
{
    Specular = rqColor;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetPosition (const Vector3f& rqPosition)
{
    SetTranslate(rqPosition);
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetDirection (const Vector3f& rqDirection)
{
    Direction = rqDirection;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetAttenConstant (float fAttenConstant)
{
    AttenConstant = fAttenConstant;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetAttenLinear (float fAttenLinear)
{
    AttenLinear = fAttenLinear;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetAttenQuadratic (float fAttenQuadratic)
{
    AttenQuadratic = fAttenQuadratic;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetSpotExponent (float fSpotExponent)
{
    SpotExponent = fSpotExponent;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetSpotCutoff (float fSpotCutoffDeg)
{
    SpotCutoffDeg = fSpotCutoffDeg;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRLight3::SetOn (bool bOn)
{
    On = bOn;
}
//------------------------------------------------------------------------------------------------------------------



