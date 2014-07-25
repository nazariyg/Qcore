#include "QClippingPlanesState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,ClippingPlanesState);

//------------------------------------------------------------------------------------------------------------------
ClippingPlanesState::ClippingPlanesState ()
{
}
//------------------------------------------------------------------------------------------------------------------
int ClippingPlanesState::AddPlane (double dA, double dB, double dC, double dD)
{
    return m_qPlanes.Push(Vector4d(dA,dB,dC,dD));
}
//------------------------------------------------------------------------------------------------------------------
int ClippingPlanesState::AddPlane (const Vector4d& rqPlaneEquation)
{
    return m_qPlanes.Push(rqPlaneEquation);
}
//------------------------------------------------------------------------------------------------------------------
int ClippingPlanesState::AddPlane (const Plane3d& rqPlane)
{
    return m_qPlanes.Push(Vector4d(rqPlane.Normal.X(),rqPlane.Normal.Y(),rqPlane.Normal.Z(),-rqPlane.Constant));
}
//------------------------------------------------------------------------------------------------------------------
void ClippingPlanesState::RemovePlane (int i)
{
    m_qPlanes.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------



