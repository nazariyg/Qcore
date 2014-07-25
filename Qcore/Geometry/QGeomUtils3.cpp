#include "QGeomUtils3.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real> GeomUtils3<Real>::GetBoundingBoxOf (int iQuantity, const Vector3<Real>* aqPoint)
{
    assert( iQuantity > 0 );

    const Vector3<Real>& rqPoint = aqPoint[0];
    AxisAlignedBox3<Real> qBound(
        rqPoint.X(),rqPoint.X(),
        rqPoint.Y(),rqPoint.Y(),
        rqPoint.Z(),rqPoint.Z());
    for (int i0 = 1; i0 < iQuantity; i0++)
    {
        const Vector3<Real>& rqPoint = aqPoint[i0];

        for (int i1 = 0; i1 < 3; i1++)
        {
            if ( rqPoint[i1] < qBound.Min[i1] )
                qBound.Min[i1] = rqPoint[i1];
            else if ( rqPoint[i1] > qBound.Max[i1] )
                qBound.Max[i1] = rqPoint[i1];
        }
    }
    return qBound;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real> GeomUtils3<Real>::GetBoundingBoxOf (const Box3<Real>& rqBox)
{
    Vector3<Real> aqVertex[8];
    rqBox.ComputeVertices(aqVertex);
    return GetBoundingBoxOf(8,aqVertex);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real> GeomUtils3<Real>::GetBoundingBoxOf (const Frustum3<Real>& rqFrustum)
{
    Vector3<Real> aqVertex[8];
    rqFrustum.ComputeVertices(aqVertex);
    return GetBoundingBoxOf(8,aqVertex);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
AxisAlignedBox3<Real> GeomUtils3<Real>::GetBoundingBoxOf (const Sphere3<Real>& rqSphere)
{
    return AxisAlignedBox3<Real>(
        rqSphere.Center.X()-rqSphere.Radius,rqSphere.Center.X()+rqSphere.Radius,
        rqSphere.Center.Y()-rqSphere.Radius,rqSphere.Center.Y()+rqSphere.Radius,
        rqSphere.Center.Z()-rqSphere.Radius,rqSphere.Center.Z()+rqSphere.Radius);
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class GeomUtils3<float>;

// double
template
class GeomUtils3<double>;
//------------------------------------------------------------------------------------------------------------------



