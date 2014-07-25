//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real>::AxisAlignedBox3 ()
{
    // uninitialized
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real>::AxisAlignedBox3 (
    Real fMinX, Real fMaxX,
    Real fMinY, Real fMaxY,
    Real fMinZ, Real fMaxZ)
{
    Min[0] = fMinX;
    Max[0] = fMaxX;
    Min[1] = fMinY;
    Max[1] = fMaxY;
    Min[2] = fMinZ;
    Max[2] = fMaxZ;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool AxisAlignedBox3<Real>::Contains (const Vector3<Real>& rqPoint) const
{
    return ( rqPoint.X() >= Min[0] && rqPoint.X() <= Max[0]
        &&   rqPoint.Y() >= Min[1] && rqPoint.Y() <= Max[1]
        &&   rqPoint.Z() >= Min[2] && rqPoint.Z() <= Max[2] );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool AxisAlignedBox3<Real>::TestIntersection (const AxisAlignedBox3<Real>& rqBox) const
{
    return ( Min[0] <= rqBox.Max[0] && Max[0] >= rqBox.Min[0]
        &&   Min[1] <= rqBox.Max[1] && Max[1] >= rqBox.Min[1]
        &&   Min[2] <= rqBox.Max[2] && Max[2] >= rqBox.Min[2] );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool AxisAlignedBox3<Real>::Contains (const Vector3<Real>& rqPoint, Real fTolerance) const
{
    return ( Min[0] - rqPoint.X() <= fTolerance && rqPoint.X() - Max[0] <= fTolerance
        &&   Min[1] - rqPoint.Y() <= fTolerance && rqPoint.Y() - Max[1] <= fTolerance
        &&   Min[2] - rqPoint.Z() <= fTolerance && rqPoint.Z() - Max[2] <= fTolerance );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool AxisAlignedBox3<Real>::TestIntersection (const AxisAlignedBox3<Real>& rqBox, Real fTolerance) const
{
    return ( Min[0] - rqBox.Max[0] <= fTolerance && Max[0] - rqBox.Min[0] >= -fTolerance
        &&   Min[1] - rqBox.Max[1] <= fTolerance && Max[1] - rqBox.Min[1] >= -fTolerance
        &&   Min[2] - rqBox.Max[2] <= fTolerance && Max[2] - rqBox.Min[2] >= -fTolerance );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool AxisAlignedBox3<Real>::FindIntersection (const AxisAlignedBox3& rqBox, AxisAlignedBox3& rqIntr) const
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if ( Max[i] < rqBox.Min[i] || Min[i] > rqBox.Max[i] )
            return false;
    }

    for (i = 0; i < 3; i++)
    {
        if ( Max[i] <= rqBox.Max[i] )
            rqIntr.Max[i] = Max[i];
        else
            rqIntr.Max[i] = rqBox.Max[i];

        if ( Min[i] <= rqBox.Min[i] )
            rqIntr.Min[i] = rqBox.Min[i];
        else
            rqIntr.Min[i] = Min[i];
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void AxisAlignedBox3<Real>::MergeBounds (const AxisAlignedBox3<Real>& rqBox0, const AxisAlignedBox3<Real>& rqBox1,
    AxisAlignedBox3& rqMrg)
{
    for (int i = 0; i < 3; i++)
    {
        rqMrg.Min[i] = Math<Real>::Min(rqBox0.Min[i],rqBox1.Min[i]);
        rqMrg.Max[i] = Math<Real>::Max(rqBox0.Max[i],rqBox1.Max[i]);
    }
}
//------------------------------------------------------------------------------------------------------------------



