#ifndef QGRPHUTILS3_H
#define QGRPHUTILS3_H

#include "QMatrix4.h"

namespace Q
{

template <class Real>
class Q_ITEM GrphUtils3
{
public:
    // perspective projection matrix generation
    static Matrix4<Real> GetPerspProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop, Real fNear,
        Real fFar);

    // the same, in more convenient way
    static Matrix4<Real> GetPerspProjMatrix (Real fUpFovDeg, Real fWOnHAspectRatio, Real fNear, Real fFar);

    // orthogonal projection matrix generation
    static Matrix4<Real> GetOrthoProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop, Real fNear,
        Real fFar);

    // the same, with near = -1.0 and far = 1.0
    static Matrix4<Real> GetOrthoProjMatrix (Real fLeft, Real fRight, Real fBottom, Real fTop);

    // getting of "smooth" normals basing on geometry
    static void ConstructNormals (int iVertexQuantity, const Vector3<Real>* aqVertex, int iIndexQuantity,
        const int* aiIndex, Vector3<Real>*& raqNormal);
};

typedef GrphUtils3<float> GrphUtils3f;
typedef GrphUtils3<double> GrphUtils3d;

}

#endif



