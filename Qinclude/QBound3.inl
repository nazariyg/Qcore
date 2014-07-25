//------------------------------------------------------------------------------------------------------------------
template <class Real>
Bound3<Real>::Bound3 ()
{
    // uninitialized
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Bound3<Real>::Bound3 (Real fMinX, Real fMaxX, Real fMinY, Real fMaxY, Real fMinZ, Real fMaxZ)
    :
    MinX(fMinX),
    MaxX(fMaxX),
    MinY(fMinY),
    MaxY(fMaxY),
    MinZ(fMinZ),
    MaxZ(fMaxZ)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Bound3<Real>::Merge (const Bound3& rqBound)
{
    if ( rqBound.MinX < MinX )
        MinX = rqBound.MinX;
    if ( rqBound.MaxX > MaxX )
        MaxX = rqBound.MaxX;
    if ( rqBound.MinY < MinY )
        MinY = rqBound.MinY;
    if ( rqBound.MaxY > MaxY )
        MaxY = rqBound.MaxY;
    if ( rqBound.MinZ < MinZ )
        MinZ = rqBound.MinZ;
    if ( rqBound.MaxZ > MaxZ )
        MaxZ = rqBound.MaxZ;
}
//------------------------------------------------------------------------------------------------------------------



