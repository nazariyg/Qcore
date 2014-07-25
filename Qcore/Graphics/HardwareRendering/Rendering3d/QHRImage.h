#ifndef QHRIMAGE_H
#define QHRIMAGE_H

#include "QStateSensitive.h"

namespace Q
{

template <class Real>
class HRImage : public Heap, public StateSensitive, public HRBufferable
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    enum TargetBufferType
    {
        TBT_FRONT,
        TBT_BACK,
        TBT_LEFT,
        TBT_RIGHT,
        TBT_FRONT_LEFT,
        TBT_FRONT_RIGHT,
        TBT_BACK_LEFT,
        TBT_BACK_RIGHT,
        TBT_FRONT_AND_BACK
    };

    enum FormatType
    {
        FT_STENCIL_INDEX,
        FT_DEPTH_COMPONENT,
        FT_RED,
        FT_GREEN,
        FT_BLUE,
        FT_ALPHA,
        FT_LUMINANCE
    };

    // construction
    HRImage (const RasterImagePtr& rspqImage);

    // member access
    void SetTargetBuffer (TargetBufferType eTargetBuffer);
    void UnsetTargetBuffer ();
    void SetPosition (const Vector3<Real>& rqPosition);
    void SetPosition (const Vector4<Real>& rqPosition);
    void SetFormat (FormatType eFormat);
    void UnsetFormat ();  // to do autodetect
    void SetDrawInWindowCoordinates (bool bSet);

    RasterImagePtr Image;
    bool TargetBufferSet;           // default: false
    TargetBufferType TargetBuffer;
    Vector4<Real> Position;         // default: (0, 0, 0, 1)
    bool FormatSet;                 // default: false (autodetect)
    FormatType Format;
    bool DrawInWindowCoordinates;   // default: false
};

#include "QHRImage.inl"

typedef HRImage<float> HRImagef;
typedef HRImage<double> HRImaged;

typedef SmartPointer<HRImagef> HRImagefPtr;
typedef SmartPointer<HRImaged> HRImagedPtr;

}

#endif



