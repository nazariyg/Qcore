#ifndef QBASICMESHCONSTRUCTION_H
#define QBASICMESHCONSTRUCTION_H

#include "QVector3.h"

namespace Q
{

template <class Real>
class Q_ITEM BasicMeshConstruction
{
public:
    enum AxisType
    {
        AT_AXIS_X,
        AT_AXIS_Y,
        AT_AXIS_Z
    };

    // on output, a memory owned by this data structure is to be taken to some other ownership
    class BasicMeshRecord
    {
    public:
        BasicMeshRecord (int iVertexQuantity, Vector3<Real>* aqVertex, int iIndexQuantity, int* aiIndex,
            Vector3<Real>* aqNormal)
            :
            VertexQuantity(iVertexQuantity),
            Vertices(aqVertex),
            IndexQuantity(iIndexQuantity),
            Index(aiIndex),
            Normals(aqNormal)
        {
        };

        int VertexQuantity;
        Vector3<Real>* Vertices;
        int IndexQuantity;
        int* Index;
        Vector3<Real>* Normals;
    };

    // limitations: iSegmentsX >= 1, iSegmentsY >= 1, iSegmentsZ >= 1, fRadius <= fExtent_minimal/2,
    // iRadiusSegments >= 1
    static BasicMeshRecord ConstructBox (bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis,
        Real fExtentX, Real fExtentY, Real fExtentZ, int iSegmentsX, int iSegmentsY, int iSegmentsZ,
        Real fRadius = (Real)0.0, int iRadiusSegments = 0);

    // limitations: iSides >= 3, iSegments >= 2
    static BasicMeshRecord ConstructBall (bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis,
        Real fExtentX, Real fExtentY, Real fExtentZ, int iSides, int iSegments);

    // limitations: iSides >= 3, iSegments >= 1
    static BasicMeshRecord ConstructCylinder (bool bConstructNormals, const Vector3<Real>& rqCenter,
        AxisType eAxis, Real fExtentX, Real fExtentY, Real fExtentZ, int iSides, int iSegments);

    // limitations: iSides >= 3, iSegments >= 1
    static BasicMeshRecord ConstructCone (bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis,
        Real fExtentX, Real fExtentY, Real fExtentZ, int iSides, int iSegments);

    // limitations: fSectionExtent0 <= fRadius, iSections >= 3, iSides >= 3
    static BasicMeshRecord ConstructTorus (bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis,
        Real fRadius, Real fSectionExtent0, Real fSectionExtent1, int iSections, int iSides,
        Real fStartAngle = (Real)0.0, Real fEndAngle = Math<Real>::TWO_PI);

    // limitations: fInnerRadius <= fOuterRadius, iSides >= 3, iSegments >= 1
    static BasicMeshRecord ConstructTube (bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis,
        Real fLength, Real fInnerRadius, Real fOuterRadius, int iSides, int iSegments,
        Real fStartAngle = (Real)0.0, Real fEndAngle = Math<Real>::TWO_PI);

    // limitations: fRadius <= fLength/2, iSides >= 3, iSegments >= 2, iLengthSegments >= 1
    static BasicMeshRecord ConstructCapsule (bool bConstructNormals, const Vector3<Real>& rqCenter,
        AxisType eAxis, Real fRadius, Real fLength, int iSides, int iSegments, int iLengthSegments);

    // limitations: iDegree >= 1
    static BasicMeshRecord ConstructGeosphere (bool bConstructNormals, const Vector3<Real>& rqCenter,
        Real fRadius, int iDegree);

    // limitations: iDegree >= 1.  iDegree = 1 makes 2 triangles from a single Bezier patch, iDegree = 2 makes
    // 8 triangles, and so on following the formula 2*(degree^2).
    static BasicMeshRecord ConstructTeapot (bool bConstructNormals, const Vector3<Real>& rqCenter, Real fSize,
        int iDegree);

private:
    static void AddTriangle (int& riIQuantity, int* aiIndex, int iV0, int iV1, int iV2);
    static void AddConvexQuadrangle (int& riIQuantity, int* aiIndex, int iV0, int iV1, int iV2, int iV3);
    static void AddConvexPolygon (int& riIQuantity, int* aiIndex, int iVQuantity, const int* aiVIndex,
        bool bFromFirstToLast);

    // teapot Bezier patches
    static const Vector3<Real> ms_aqTeapotVertices[306];
    static const int ms_aaaiTeapotIndices[32][4][4];
};

typedef BasicMeshConstruction<float> BasicMeshConstructionf;
typedef BasicMeshConstruction<double> BasicMeshConstructiond;

}

#endif



