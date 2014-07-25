#ifndef QINTRBASE3_H
#define QINTRBASE3_H

// The base class for spatial intersection classes.  Additionally to intersection operand containment, the
// layer of IntrBase3 is also responsible for bounding box handling: in view of the fact that for many
// geometric objects axis-aligned bounding box computation is inexpensive relatively to actual intersection
// testing/finding with other objects, an IntrBase3 derivative may call or may not call the TestBoundingBoxes
// protected member function which computes that bounding box (or boxes, if any) which was not previously
// imparted by the user and then performs very cheap inter-bound intersection test.

#include "QGeomUtils3.h"

namespace Q
{

template <class Real, class Operand0, class Operand1>
class IntrBase3
{
protected:
    // construction
    IntrBase3 (const Operand0& rqOperand0, const Operand1& rqOperand1);

public:
    // letting know precomputed bounds
    void ImpartBoundingBox0 (const AxisAlignedBox3<Real>& rqBoundingBox);
    void ImpartBoundingBox1 (const AxisAlignedBox3<Real>& rqBoundingBox);

protected:
    bool TestBoundingBoxes () const;

    const Operand0& m_rqOperand0;
    const Operand1& m_rqOperand1;
    bool m_bKnownBB0;  // default: false
    bool m_bKnownBB1;  // default: false
    AxisAlignedBox3<Real> m_qBoundingBox0;
    AxisAlignedBox3<Real> m_qBoundingBox1;
};

#include "QIntrBase3.inl"

}

#endif



