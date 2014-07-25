#ifndef QRASTERIMAGESEQUENCE_H
#define QRASTERIMAGESEQUENCE_H

// The raster-image animation container class.

#include "QRasterImage.h"

namespace Q
{

class Q_ITEM RasterImageSequence
{
public:
    // construction
    RasterImageSequence (RasterImage::ImageType eFrameImageType, int iScreenWidth, int iScreenHeight,
        int iFrameQuantity = 0);

    // member access
    RasterImage::ImageType GetFrameImageType () const;
    int GetScreenWidth () const;
    int GetScreenHeight () const;
    int GetFrameQuantity () const;
    void SetFrameImage (const RasterImagePtr& rspqImage, int i);  // type, width, and height must be as declared
    const RasterImagePtr& GetFrameImage (int i) const;

    // frame array resizing
    void SetFrameQuantity (int iFrameQuantity);

private:
    RasterImage::ImageType m_eFrameImageType;
    int m_iScreenWidth, m_iScreenHeight;
    StillArray<RasterImagePtr> m_qFrames;
};

#include "QRasterImageSequence.inl"

}

#endif



