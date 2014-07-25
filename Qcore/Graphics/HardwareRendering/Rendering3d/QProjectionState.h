#ifndef QPROJECTIONSTATE_H
#define QPROJECTIONSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"
#include "QGrphUtils3.h"

namespace Q
{

template <class Real>
class ProjectionState : public StateBase
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    enum ProjectionType
    {
        PT_PERSPECTIVE,
        PT_ORTHOGRAPHIC
    };

    // Construction.  If four floating-point parameters provided, they are interpreted as fUpFovDeg,
    // fWOnHAspectRatio, fNear, fFar in case of perspective projection, for orthographic projection they are
    // fLeft, fRight, fBottom, fTop with near and far clipping planes set to -1.0 and 1.0 respectively.
    ProjectionState (const Matrix4<Real>& rqProjMatrix);
    ProjectionState (ProjectionType eProjection, Real fLeft, Real fRight, Real fBottom, Real fTop, Real fNear,
        Real fFar);
    ProjectionState (ProjectionType eProjection, Real fParam0, Real fParam1, Real fParam2, Real fParam3);

    Matrix4<Real> ProjectionMatrix;
};

#include "QProjectionState.inl"

typedef ProjectionState<float> ProjectionStatef;
typedef ProjectionState<double> ProjectionStated;

typedef SmartPointer<ProjectionStatef> ProjectionStatefPtr;
typedef SmartPointer<ProjectionStated> ProjectionStatedPtr;

}

#endif



