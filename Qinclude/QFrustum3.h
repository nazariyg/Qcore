#ifndef QFRUSTUM3_H
#define QFRUSTUM3_H

// Orthogonal frustum.  Let E be the origin, D be the direction vector, U be the up vector, and R be the right
// vector.  Let u > 0 and r > 0 be the extents in the U and R directions, respectively.  Let n and f be the
// extents in the D direction with 0 < n < f.  The four corners of the frustum in the near plane are
// E + n*D + s0*u*U + s1*r*R where |s0| = |s1| = 1 (four choices).  The four corners of the frustum in the far
// plane are E + f*D + (f/n)*(s0*u*U + s1*r*R) where |s0| = |s1| = 1 (four choices).

#include "QVector3.h"

namespace Q
{

template <class Real>
class Frustum3
{
public:
    // Construction.  The default frustum is E = (0,0,0), D = (0,0,-1), U = (0,1,0), and R = (1,0,0), DMin = 1,
    // DMax = 2, UBound = 1, RBound = 1.
    Frustum3 ();
    Frustum3 (const Vector3<Real>& rqOrigin, const Vector3<Real>& rqDVector, const Vector3<Real>& rqUVector,
        const Vector3<Real>& rqRVector, Real fDMin, Real fDMax, Real fUBound, Real fRBound);

    // The Update() function must be called whenever changes are made to DMin, DMax, UBound, or RBound.  The
    // values DRatio, MTwoUF, and MTwoRF are dependent on the changes, so call the Get accessors only after the
    // Update() call.
    void Update ();
    Real GetDRatio () const;
    Real GetMTwoUF () const;
    Real GetMTwoRF () const;

    void ComputeVertices (Vector3<Real> aqVertex[8]) const;

    Vector3<Real> Origin, DVector, UVector, RVector;
    Real DMin, DMax, UBound, RBound;

protected:
    // derived quantities
    Real m_fDRatio, m_fMTwoUF, m_fMTwoRF;
};

#include "QFrustum3.inl"

typedef Frustum3<float> Frustum3f;
typedef Frustum3<double> Frustum3d;

}

#endif



