#ifndef QINTRBOX3FRUSTUM3_H
#define QINTRBOX3FRUSTUM3_H

#include "QIntrBase3.h"

namespace Q
{

template <class Real>
class Q_ITEM IntrBox3Frustum3 : public IntrBase3<Real,Box3<Real>,Frustum3<Real> >
{
public:
    // construction
    IntrBox3Frustum3 (const Box3<Real>& rqBox, const Frustum3<Real>& rqFrustum);

    // object access
    const Box3<Real>& GetBox () const;
    const Frustum3<Real>& GetFrustum () const;

    // test-intersection query
    bool Test ();
};

typedef IntrBox3Frustum3<float> IntrBox3Frustum3f;
typedef IntrBox3Frustum3<double> IntrBox3Frustum3d;

}

#endif



