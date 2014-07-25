//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRImage<Real>::HRImage (const RasterImagePtr& rspqImage)
    :
    Image(rspqImage),
    TargetBufferSet(false),
    Position(Vector4<Real>::UNIT_W),
    FormatSet(false),
    DrawInWindowCoordinates(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::SetTargetBuffer (TargetBufferType eTargetBuffer)
{
    TargetBuffer = eTargetBuffer;
    TargetBufferSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::UnsetTargetBuffer ()
{
    TargetBufferSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::SetPosition (const Vector3<Real>& rqPosition)
{
    Position.X() = rqPosition.X();
    Position.Y() = rqPosition.Y();
    Position.Z() = rqPosition.Z();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::SetPosition (const Vector4<Real>& rqPosition)
{
    Position = rqPosition;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::SetFormat (FormatType eFormat)
{
    Format = eFormat;
    FormatSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::UnsetFormat ()
{
    FormatSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRImage<Real>::SetDrawInWindowCoordinates (bool bSet)
{
    DrawInWindowCoordinates = bSet;
}
//------------------------------------------------------------------------------------------------------------------



