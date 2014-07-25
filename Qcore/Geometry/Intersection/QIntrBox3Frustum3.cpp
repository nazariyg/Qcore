#include "QIntrBox3Frustum3.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
IntrBox3Frustum3<Real>::IntrBox3Frustum3 (const Box3<Real>& rqBox, const Frustum3<Real>& rqFrustum)
    :
    IntrBase3<Real,Box3<Real>,Frustum3<Real> >(rqBox,rqFrustum)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Box3<Real>& IntrBox3Frustum3<Real>::GetBox () const
{
    return m_rqOperand0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Frustum3<Real>& IntrBox3Frustum3<Real>::GetFrustum () const
{
    return m_rqOperand1;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool IntrBox3Frustum3<Real>::Test ()
{
    // first, test the bounding boxes
    if ( !TestBoundingBoxes() )
        return false;

    // for convenience
    const Vector3<Real>* aqA = m_rqOperand0.Axis;
    const Real* afE = m_rqOperand0.Extent;

    Vector3<Real> qDiff = m_rqOperand0.Center - m_rqOperand1.Origin;  // C-E

    Real afA[3];      // Dot(R,A[i])
    Real afB[3];      // Dot(U,A[i])
    Real afC[3];      // Dot(D,A[i])
    Real afD[3];      // (Dot(R,C-E),Dot(U,C-E),Dot(D,C-E))
    Real afNA[3];     // dmin*Dot(R,A[i])
    Real afNB[3];     // dmin*Dot(U,A[i])
    Real afNC[3];     // dmin*Dot(D,A[i])
    Real afND[3];     // dmin*(Dot(R,C-E),Dot(U,C-E),?)
    Real afRC[3];     // rmax*Dot(D,A[i])
    Real afRD[3];     // rmax*(?,?,Dot(D,C-E))
    Real afUC[3];     // umax*Dot(D,A[i])
    Real afUD[3];     // umax*(?,?,Dot(D,C-E))
    Real afNApRC[3];  // dmin*Dot(R,A[i]) + rmax*Dot(D,A[i])
    Real afNAmRC[3];  // dmin*Dot(R,A[i]) - rmax*Dot(D,A[i])
    Real afNBpUC[3];  // dmin*Dot(U,A[i]) + umax*Dot(D,A[i])
    Real afNBmUC[3];  // dmin*Dot(U,A[i]) - umax*Dot(D,A[i])
    Real afRBpUA[3];  // rmax*Dot(U,A[i]) + umax*Dot(R,A[i])
    Real afRBmUA[3];  // rmax*Dot(U,A[i]) - umax*Dot(R,A[i])
    Real fDdD, fRad, fP, fMin, fMax, fMTwoUF, fMTwoRF, fTmp;
    int i, j;

    // M = D
    afD[2] = qDiff.Dot(m_rqOperand1.DVector);
    for (i = 0; i < 3; i++)
        afC[i] = aqA[i].Dot(m_rqOperand1.DVector);
    fRad =
        afE[0]*Math<Real>::FAbs(afC[0]) +
        afE[1]*Math<Real>::FAbs(afC[1]) +
        afE[2]*Math<Real>::FAbs(afC[2]);
    if ( afD[2] + fRad < m_rqOperand1.DMin
    ||   afD[2] - fRad > m_rqOperand1.DMax )
    {
        return false;
    }

    // M = n*R - r*D
    for (i = 0; i < 3; i++)
    {
        afA[i] = aqA[i].Dot(m_rqOperand1.RVector);
        afRC[i] = m_rqOperand1.RBound*afC[i];
        afNA[i] = m_rqOperand1.DMin*afA[i];
        afNAmRC[i] = afNA[i] - afRC[i];
    }
    afD[0] = qDiff.Dot(m_rqOperand1.RVector);
    fRad =
        afE[0]*Math<Real>::FAbs(afNAmRC[0]) +
        afE[1]*Math<Real>::FAbs(afNAmRC[1]) +
        afE[2]*Math<Real>::FAbs(afNAmRC[2]);
    afND[0] = m_rqOperand1.DMin*afD[0];
    afRD[2] = m_rqOperand1.RBound*afD[2];
    fDdD = afND[0] - afRD[2];
    fMTwoRF = m_rqOperand1.GetMTwoRF();
    if ( fDdD + fRad < fMTwoRF || fDdD > fRad )
        return false;

    // M = -n*R - r*D
    for (i = 0; i < 3; i++)
        afNApRC[i] = afNA[i] + afRC[i];
    fRad =
        afE[0]*Math<Real>::FAbs(afNApRC[0]) +
        afE[1]*Math<Real>::FAbs(afNApRC[1]) +
        afE[2]*Math<Real>::FAbs(afNApRC[2]);
    fDdD = -(afND[0] + afRD[2]);
    if ( fDdD + fRad < fMTwoRF || fDdD > fRad )
        return false;

    // M = n*U - u*D
    for (i = 0; i < 3; i++)
    {
        afB[i] = aqA[i].Dot(m_rqOperand1.UVector);
        afUC[i] = m_rqOperand1.UBound*afC[i];
        afNB[i] = m_rqOperand1.DMin*afB[i];
        afNBmUC[i] = afNB[i] - afUC[i];
    }
    afD[1] = qDiff.Dot(m_rqOperand1.UVector);
    fRad =
        afE[0]*Math<Real>::FAbs(afNBmUC[0]) +
        afE[1]*Math<Real>::FAbs(afNBmUC[1]) +
        afE[2]*Math<Real>::FAbs(afNBmUC[2]);
    afND[1] = m_rqOperand1.DMin*afD[1];
    afUD[2] = m_rqOperand1.UBound*afD[2];
    fDdD = afND[1] - afUD[2];
    fMTwoUF = m_rqOperand1.GetMTwoUF();
    if ( fDdD + fRad < fMTwoUF || fDdD > fRad )
        return false;

    // M = -n*U - u*D
    for (i = 0; i < 3; i++)
        afNBpUC[i] = afNB[i] + afUC[i];
    fRad =
        afE[0]*Math<Real>::FAbs(afNBpUC[0]) +
        afE[1]*Math<Real>::FAbs(afNBpUC[1]) +
        afE[2]*Math<Real>::FAbs(afNBpUC[2]);
    fDdD = -(afND[1] + afUD[2]);
    if ( fDdD + fRad < fMTwoUF || fDdD > fRad )
        return false;

    // M = A[i]
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afA[i]) +
             m_rqOperand1.UBound*Math<Real>::FAbs(afB[i]);
        afNC[i] = m_rqOperand1.DMin*afC[i];
        fMin = afNC[i] - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = afNC[i] + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afA[i]*afD[0] + afB[i]*afD[1] + afC[i]*afD[2];
        if ( fDdD + afE[i] < fMin || fDdD - afE[i] > fMax )
            return false;
    }

    // M = Cross(R,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.UBound*Math<Real>::FAbs(afC[i]);
        fMin = -afNB[i] - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = -afNB[i] + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afC[i]*afD[1] - afB[i]*afD[2];
        fRad =
            afE[0]*Math<Real>::FAbs(afB[i]*afC[0]-afB[0]*afC[i]) +
            afE[1]*Math<Real>::FAbs(afB[i]*afC[1]-afB[1]*afC[i]) +
            afE[2]*Math<Real>::FAbs(afB[i]*afC[2]-afB[2]*afC[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    // M = Cross(U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afC[i]);
        fMin = afNA[i] - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = afNA[i] + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = -afC[i]*afD[0] + afA[i]*afD[2];
        fRad =
            afE[0]*Math<Real>::FAbs(afA[i]*afC[0]-afA[0]*afC[i]) +
            afE[1]*Math<Real>::FAbs(afA[i]*afC[1]-afA[1]*afC[i]) +
            afE[2]*Math<Real>::FAbs(afA[i]*afC[2]-afA[2]*afC[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    // M = Cross(n*D+r*R+u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        Real fRB = m_rqOperand1.RBound*afB[i];
        Real fUA = m_rqOperand1.UBound*afA[i];
        afRBpUA[i] = fRB + fUA;
        afRBmUA[i] = fRB - fUA;
    }
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afNBmUC[i]) +
             m_rqOperand1.UBound*Math<Real>::FAbs(afNAmRC[i]);
        fTmp = -m_rqOperand1.DMin*afRBmUA[i];
        fMin = fTmp - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = fTmp + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afD[0]*afNBmUC[i] - afD[1]*afNAmRC[i] - afD[2]*afRBmUA[i];
        fRad = (Real)0.0;
        for (j = 0; j < 3; j++)
            fRad += afE[j]*Math<Real>::FAbs(afA[j]*afNBmUC[i]-afB[j]*afNAmRC[i]-afC[j]*afRBmUA[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    // M = Cross(n*D+r*R-u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afNBpUC[i]) +
             m_rqOperand1.UBound*Math<Real>::FAbs(afNAmRC[i]);
        fTmp = -m_rqOperand1.DMin*afRBpUA[i];
        fMin = fTmp - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = fTmp + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afD[0]*afNBpUC[i] - afD[1]*afNAmRC[i] - afD[2]*afRBpUA[i];
        fRad = (Real)0.0;
        for (j = 0; j < 3; j++)
            fRad += afE[j]*Math<Real>::FAbs(afA[j]*afNBpUC[i]-afB[j]*afNAmRC[i]-afC[j]*afRBpUA[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    // M = Cross(n*D-r*R+u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afNBmUC[i]) +
             m_rqOperand1.UBound*Math<Real>::FAbs(afNApRC[i]);
        fTmp = m_rqOperand1.DMin*afRBpUA[i];
        fMin = fTmp - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = fTmp + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afD[0]*afNBmUC[i] - afD[1]*afNApRC[i] + afD[2]*afRBpUA[i];
        fRad = (Real)0.0;
        for (j = 0; j < 3; j++)
            fRad += afE[j]*Math<Real>::FAbs(afA[j]*afNBmUC[i]-afB[j]*afNApRC[i]+afC[j]*afRBpUA[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    // M = Cross(n*D-r*R-u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = m_rqOperand1.RBound*Math<Real>::FAbs(afNBpUC[i]) +
             m_rqOperand1.UBound*Math<Real>::FAbs(afNApRC[i]);
        fTmp = m_rqOperand1.DMin*afRBmUA[i];
        fMin = fTmp - fP;
        if ( fMin < (Real)0.0 )
            fMin *= m_rqOperand1.GetDRatio();
        fMax = fTmp + fP;
        if ( fMax > (Real)0.0 )
            fMax *= m_rqOperand1.GetDRatio();
        fDdD = afD[0]*afNBpUC[i] - afD[1]*afNApRC[i] + afD[2]*afRBmUA[i];
        fRad = (Real)0.0;
        for (j = 0; j < 3; j++)
            fRad += afE[j]*Math<Real>::FAbs(afA[j]*afNBpUC[i]-afB[j]*afNApRC[i]+afC[j]*afRBmUA[i]);
        if ( fDdD + fRad < fMin || fDdD - fRad > fMax )
            return false;
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class IntrBox3Frustum3<float>;

// double
template
class IntrBox3Frustum3<double>;
//------------------------------------------------------------------------------------------------------------------



