#ifndef QCLIPPINGPLANESSTATE_H
#define QCLIPPINGPLANESSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"
#include "QPlane3.h"
#include "QModestArray.h"

namespace Q
{

class Q_ITEM ClippingPlanesState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    ClippingPlanesState ();  // empty

    // member access
    int GetPlaneQuantity () const;
    int AddPlane (double dA, double dB, double dC, double dD);
    int AddPlane (const Vector4d& rqPlaneEquation);
    int AddPlane (const Plane3d& rqPlane);
    const Vector4d& GetPlane (int i) const;
    void RemovePlane (int i);

private:
    ModestArray<Vector4d> m_qPlanes;
};

#include "QClippingPlanesState.inl"

typedef SmartPointer<ClippingPlanesState> ClippingPlanesStatePtr;

}

#endif



