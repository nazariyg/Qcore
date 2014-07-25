#include "QGrphUtils3.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> GrphUtils3<Real>::GetPerspProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop, Real fNear,
    Real fFar)
{
    Real fRLDiff = fRight - fLeft;
    Real fTBDiff = fTop - fBottom;
    Real fFNDiff = fFar - fNear;
    return Matrix4<Real>(
        ((Real)2.0)*fNear/fRLDiff,(Real)0.0,(fRight+fLeft)/fRLDiff,(Real)0.0,
        (Real)0.0,((Real)2.0)*fNear/fTBDiff,(fTop+fBottom)/fTBDiff,(Real)0.0,
        (Real)0.0,(Real)0.0,-(fFar+fNear)/fFNDiff,((Real)-2.0)*fFar*fNear/fFNDiff,
        (Real)0.0,(Real)0.0,(Real)-1.0,(Real)0.0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> GrphUtils3<Real>::GetPerspProjMatrix (Real fUpFovDeg, Real fWOnHAspectRatio, Real fNear, Real fFar)
{
    Real fTop = fNear*Math<Real>::Tan(((Real)0.5)*fUpFovDeg*Math<Real>::DEG_TO_RAD);
    Real fRight = fWOnHAspectRatio*fTop;
    Real fBottom = -fTop;
    Real fLeft = -fRight;
    return GetPerspProjMatrix(fLeft,fRight,fBottom,fTop,fNear,fFar);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> GrphUtils3<Real>::GetOrthoProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop, Real fNear,
    Real fFar)
{
    Real fRLDiff = fRight - fLeft;
    Real fTBDiff = fTop - fBottom;
    Real fFNDiff = fFar - fNear;
    return Matrix4<Real>(
        ((Real)2.0)/fRLDiff,(Real)0.0,(Real)0.0,(fRight+fLeft)/fRLDiff,
        (Real)0.0,((Real)2.0)/fTBDiff,(Real)0.0,(fTop+fBottom)/fTBDiff,
        (Real)0.0,(Real)0.0,((Real)-2.0)/fFNDiff,(fFar+fNear)/fFNDiff,
        (Real)0.0,(Real)0.0,(Real)0.0,(Real)1.0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> GrphUtils3<Real>::GetOrthoProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop)
{
    return GetOrthoProjMatrix(fLeft,fRight,fBottom,fTop,(Real)-1.0,(Real)1.0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GrphUtils3<Real>::ConstructNormals (int iVertexQuantity, const Vector3<Real>* aqVertex, int iIndexQuantity,
    const int* aiIndex, Vector3<Real>*& raqNormal)
{
    assert( iVertexQuantity >= 0 && iIndexQuantity % 3 == 0 );

    raqNormal = new Vector3<Real>[iVertexQuantity];
    MemSet(raqNormal,0,iVertexQuantity*sizeof(Vector3<Real>));
    Vector3<Real> qNormal;
    int iTriQuantity = iIndexQuantity/3;
    int iV0, iV1, iV2, iCQ = 0;
    for (int i = 0; i < iTriQuantity; i++)
    {
        iV0 = aiIndex[iCQ++];
        iV1 = aiIndex[iCQ++];
        iV2 = aiIndex[iCQ++];

        const Vector3<Real>& rqV0 = aqVertex[iV0];
        const Vector3<Real>& rqV1 = aqVertex[iV1];
        const Vector3<Real>& rqV2 = aqVertex[iV2];
        qNormal = (rqV1-rqV0).Cross(rqV2-rqV0);
        raqNormal[iV0] += qNormal;
        raqNormal[iV1] += qNormal;
        raqNormal[iV2] += qNormal;
    }
    for (int i = 0; i < iVertexQuantity; i++)
        raqNormal[i].Normalize();
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class GrphUtils3<float>;

// double
template
class GrphUtils3<double>;
//------------------------------------------------------------------------------------------------------------------



