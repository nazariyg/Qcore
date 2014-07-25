//------------------------------------------------------------------------------------------------------------------
template <class Real>
Box3<Real>::Box3 ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Box3<Real>::Box3 (const Vector3<Real>& rqCenter, const Vector3<Real>* aqAxis, const Real* afExtent)
    :
    Center(rqCenter)
{
    for (int i = 0; i < 3; i++)
    {
        Axis[i] = aqAxis[i];
        Extent[i] = afExtent[i];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Box3<Real>::Box3 (const Vector3<Real>& rqCenter, const Vector3<Real>& rqAxis0, const Vector3<Real>& rqAxis1,
    const Vector3<Real>& rqAxis2, Real fExtent0, Real fExtent1, Real fExtent2)
    :
    Center(rqCenter)
{
    Axis[0] = rqAxis0;
    Axis[1] = rqAxis1;
    Axis[2] = rqAxis2;
    Extent[0] = fExtent0;
    Extent[1] = fExtent1;
    Extent[2] = fExtent2;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Box3<Real>::ComputeVertices (Vector3<Real> aqVertex[8]) const
{
    Vector3<Real> aqEAxis[3] =
    {
        Extent[0]*Axis[0],
        Extent[1]*Axis[1],
        Extent[2]*Axis[2]
    };

    aqVertex[0] = Center - aqEAxis[0] - aqEAxis[1] - aqEAxis[2];
    aqVertex[1] = Center + aqEAxis[0] - aqEAxis[1] - aqEAxis[2];
    aqVertex[2] = Center + aqEAxis[0] + aqEAxis[1] - aqEAxis[2];
    aqVertex[3] = Center - aqEAxis[0] + aqEAxis[1] - aqEAxis[2];
    aqVertex[4] = Center - aqEAxis[0] - aqEAxis[1] + aqEAxis[2];
    aqVertex[5] = Center + aqEAxis[0] - aqEAxis[1] + aqEAxis[2];
    aqVertex[6] = Center + aqEAxis[0] + aqEAxis[1] + aqEAxis[2];
    aqVertex[7] = Center - aqEAxis[0] + aqEAxis[1] + aqEAxis[2];
}
//------------------------------------------------------------------------------------------------------------------



