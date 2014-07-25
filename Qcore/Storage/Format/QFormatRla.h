#ifndef QFORMATRLA_H
#define QFORMATRLA_H

// Wavefront Image Format.  File extension: .rla.  For the format, bits-per-sample value equal to 32 means that
// samples are stored as 4-byte floating point data units in uncompressed form.  It is assumed, for RGBA
// images, that RGB values in each pixel are stored premultiplied by the alpha value.  For both saving and
// loading, the class works with files having "storage type" equal to 0, which is the common practice among
// graphical applications.
//
// For saving, acceptable RasterImage's color models are: CMT_RGB and CMT_RGBA.  The possible bits-per-pixel
// values in conjunction with the color models are noted below:
//   RasterImage::CMT_RGB  : the bits-per-pixel value must be in the range [3, 96] and be divisible by 3;
//   RasterImage::CMT_RGBA : the bits-per-pixel value must be in the range [4, 128] and be divisible by 4.
// An image must be without a visibility mask.  An image also must have dimensions which do not exceed 32768.
// NOTE. Most applications do not support RLA images with bits-per-sample value other than 8, 16, and 32. Saved
// files have "revision number" 0xfffe, gamma value 1.0, the format's default chromaticities, and aspect ratio
// 4/3.
//
// For loading, an image can arrive with type IT_RGB_8_8_8, IT_RGB_16_16_16, IT_RGB_FLOAT, IT_RGBA_8_8_8_8,
// IT_RGBA_16_16_16_16, or IT_RGBA_FLOAT.  An image appears without a visibility mask.  NOTE. This format has
// no signature in its design.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatRla : public FormatBase, public ProgressBase
{
public:
    // relates only to loading
    enum ColorModelType
    {
        CMT_RGB,
        CMT_RGBA
    };

    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,   // saving
        ET_FORMAT_MISMATCH,  // loading
        ET_UNSUPPORTED_FILE      // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatRla (const RasterImagePtr& rspqImage, int iBitsPerPixel);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatRla (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    static void EncodeByteSequence (const unsigned char* aucByte, int iByteQuantity,
        StillArray<char>& rqEncodedBytes);
    void EncodeChannelUChar (const unsigned char* aucSample, StillArray<char>& rqEncodedBytes) const;
    void EncodeChannelUShort (const unsigned short* ausSample, StillArray<char>& rqEncodedBytes) const;
    void EncodeChannelUInt (const unsigned int* auiSample, StillArray<char>& rqEncodedBytes) const;
    void EncodeChannelFloat (const float* afSample, StillArray<char>& rqEncodedBytes) const;
    static bool DecodeByteSequence (const char* acByte, int iByteQuantity, unsigned char* aucDecodedByte,
        int iDecodedByteQuantity);
    bool DecodeChannelUChar (const char* acByte, int iByteQuantity, unsigned char* aucDecodedSample) const;
    bool DecodeChannelUShort (const char* acByte, int iByteQuantity, unsigned short* ausDecodedSample) const;
    bool DecodeChannelUInt (const char* acByte, int iByteQuantity, unsigned int* auiDecodedSample) const;
    bool DecodeChannelFloat (const char* acByte, int iByteQuantity, float* afDecodedSample) const;

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    ColorModelType m_eColorModel;
    int m_iBitsPerColorSample, m_iBitsPerAlphaSample, m_iBitsPerPixel;
    bool m_bColorSamplesAreFP, m_bAlphaSamplesAreFP;
};

#include "QFormatRla.inl"

}

#endif



