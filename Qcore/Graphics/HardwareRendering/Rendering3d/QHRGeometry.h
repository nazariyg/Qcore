#ifndef QHRGEOMETRY_H
#define QHRGEOMETRY_H

// Hardware Renderer geometry holding class.  Normals must be of unit length.

#include "QTransformableSpatial3.h"
#include "QStateSensitive.h"
#include "QHRMaterial.h"

namespace Q
{

template <class Real>
class HRGeometry : public Heap, public TransformableSpatial3<Real,true>, public StateSensitive, public HRBufferable
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    enum GeometryType
    {
        GT_POINTS,
        GT_LINES_SEPARATE,
        GT_LINES_STRIP,
        GT_LINES_LOOP,
        GT_TRIANGLES_SEPARATE,
        GT_TRIANGLES_STRIP,
        GT_TRIANGLES_FAN,
        GT_QUADS_SEPARATE,
        GT_QUADS_STRIP,
        GT_POLYGONS,
        GT_INDEXED_POINTS,
        GT_INDEXED_LINES,
        GT_INDEXED_LINES_SEPARATE,
        GT_INDEXED_LINES_STRIP,
        GT_INDEXED_LINES_LOOP,
        GT_INDEXED_TRIANGLES_SEPARATE,
        GT_INDEXED_TRIANGLES_STRIP,
        GT_INDEXED_TRIANGLES_FAN,
        GT_INDEXED_QUADS_SEPARATE,
        GT_INDEXED_QUADS_STRIP,
        GT_INDEXED_POLYGONS
    };

    typedef SmartPointer<HeapArray<Vector2<Real> > > Vertex2HeapArrayPtr;
    typedef SmartPointer<HeapArray<Vector3<Real> > > Vertex3HeapArrayPtr;
    typedef SmartPointer<HeapArray<Vector4<Real> > > Vertex4HeapArrayPtr;
    typedef SmartPointer<HeapArray<Vector3<Real> > > NormalHeapArrayPtr;
    typedef SmartPointer<HeapArray<HRMaterialPtr> > MaterialHeapArrayPtr;
    typedef SmartPointer<HeapArray<SmartPointer<HeapArray<Real> > > > TexCoord1HeapArrayPtrHeapArrayPtr;
    typedef SmartPointer<HeapArray<SmartPointer<HeapArray<Vector2<Real> > > > > TexCoord2HeapArrayPtrHeapArrayPtr;
    typedef SmartPointer<HeapArray<SmartPointer<HeapArray<Vector3<Real> > > > > TexCoord3HeapArrayPtrHeapArrayPtr;
    typedef SmartPointer<HeapArray<SmartPointer<HeapArray<Vector4<Real> > > > > TexCoord4HeapArrayPtrHeapArrayPtr;
    typedef SmartPointer<HeapArray<Real> > FogCoordHeapArrayPtr;

    // construction
    HRGeometry (GeometryType eGeometry);

    // if vertices were changed
    virtual void UpdateLocalBound ();

    GeometryType Geometry;
    Vertex2HeapArrayPtr Vertices2;                      // default: 0
    Vertex3HeapArrayPtr Vertices3;                      // default: 0
    Vertex4HeapArrayPtr Vertices4;                      // default: 0
    ColorRgbHeapArrayPtr RgbColors;                     // default: 0
    ColorRgbaHeapArrayPtr RgbaColors;                   // default: 0
    ColorRgbHeapArrayPtr SecondaryColors;               // default: 0
    NormalHeapArrayPtr Normals;                         // default: 0
    MaterialHeapArrayPtr Materials;                     // default: 0
    TexCoord1HeapArrayPtrHeapArrayPtr TexCoords1;       // default: 0
    TexCoord2HeapArrayPtrHeapArrayPtr TexCoords2;       // default: 0
    TexCoord3HeapArrayPtrHeapArrayPtr TexCoords3;       // default: 0
    TexCoord4HeapArrayPtrHeapArrayPtr TexCoords4;       // default: 0
    BoolHeapArrayPtr EdgeFlags;                         // default: 0
    FogCoordHeapArrayPtr FogCoords;                     // default: 0
    IntHeapArrayPtr Index;                              // default: 0
};

#include "QHRGeometry.inl"

typedef HRGeometry<float> HRGeometryf;
typedef HRGeometry<double> HRGeometryd;

typedef SmartPointer<HRGeometryf> HRGeometryfPtr;
typedef SmartPointer<HRGeometryd> HRGeometrydPtr;

}

#endif



