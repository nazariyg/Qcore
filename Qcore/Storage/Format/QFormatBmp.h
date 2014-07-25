#ifndef QFORMATBMP_H
#define QFORMATBMP_H

// Microsoft Windows & OS/2 Bitmap Format.  File extensions: .bmp .rle .dib.
//
// For saving, accepted RasterImage's color models are: CMT_PALETTED and CMT_RGB.  Possible bits-per-pixel
// values, in conjunction with the color models, are noted below (but see the BMP peculiarities after):
//   RasterImage::CMT_PALETTED : the bits-per-pixel value can be 1, 4, or 8;
//   RasterImage::CMT_RGB      : the bits-per-pixel value can be 16, 24, or 32.
// For paletted images, the bpp value must allow image's pixels to address any entry in the palette, making
// true the next: 2^bpp >= palette_size.  When saving RGB images with CT_NONE compression, 16 bpp assumes
// insetting of each sample into 5 bits leaving one bit as unspecified, 24 bpp into 8 bits, and 32 bpp into 8
// bits too but leaving one byte as unspecified.  The RLE compression can be involved only with paletted
// images; for CT_RLE_4 bpp value must be 4 and for CT_RLE_8 it must be accordingly 8.  Compression type
// CT_BITFIELDS is only for RGB images and implies that red, green, and blue samples lay within a pixel data
// unit accordingly to the bit-masks specified; that pixel data unit can be of 2 or 4 bytes in size, so
// CT_BITFIELDS and bpp 24 are incompatible since 24 bits = 3 bytes; red, green, and blue bit-masks must not
// overlap.  Also, if OS type is OST_OS2, an image can be saved only as paletted or as RGB with 24 bpp; the
// OS/2 sub-format admits CT_NONE compression only.  Visibility masks are not supported by the format.  Image
// dimensions must not exceed 65535.  The implementation of RLE encoding soundly doesn't use "delta jumps" in
// order to ensure correct reading of produced images by all applications which support BMP format.
// DEFAULTS. OS type is OST_WINDOWS; compression type is CT_NONE; horizontal and vertical resolutions are both
// 2835 pixels per meter (72 dpi).
//
// For loading, a constructed RasterImage object has type IT_PALETTED, IT_RGB_8_8_8, or IT_RGB_16_16_16.
// Without a visibility mask associated.  Images with Adobe-style channel masks are read properly.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatBmp : public FormatBase, public ProgressBase
{
public:
    enum OsType
    {
        OST_WINDOWS,
        OST_OS2
    };

    enum CompressionType
    {
        CT_NONE,
        CT_RLE_4,
        CT_RLE_8,
        CT_BITFIELDS  // method of component layout
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_RGB
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  Parameter iBitsPerPixel can be omitted only for paletted images, and in this
    // case the minimal possible bpp is automatically derived from the palette size.  NOTE. If CT_RLE_4 or
    // CT_RLE_8 compression is intended, it's recommended to set the bpp value explicitly.
    FormatBmp (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // file settings
    void SetOs (OsType eOs);
    void SetCompression (CompressionType eCompression);
    void SetBitMasks (unsigned int uiMaskR, unsigned int uiMaskG, unsigned int uiMaskB);  // only when CT_BITFIELDS
    void SetPixelsPerMeterHorizontally (int iResolution);
    void SetPixelsPerMeterVertically (int iResolution);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);
    void SaveImage (StillArray<char>& rqStaticBytestream);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read data from the header.
    FormatBmp (const char* acFilename, bool& rbSuccess);
    FormatBmp (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    OsType GetOs () const;
    ColorModelType GetColorModel () const;
    CompressionType GetCompression () const;
    unsigned int GetRedMask () const;    // only when CT_BITFIELDS
    unsigned int GetGreenMask () const;  // only when CT_BITFIELDS
    unsigned int GetBlueMask () const;   // only when CT_BITFIELDS
    int GetPixelsPerMeterHorizontally () const;
    int GetPixelsPerMeterVertically () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize = -1);

    // To be used only when loading from a dynamic bytestream.  With this function an application can hide
    // those rows which are not yet transmitted.
    int GetDevelopedRowQuantity () const;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    void WriteImageData (Stream& rqStream);
    bool LoadHeadingData (int iBytestreamSize = -1);

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    OsType m_eOs;                       // (default: OST_WINDOWS)
    CompressionType m_eCompression;     // (default: CT_NONE)
    int m_iPixelsPerMeterHorizontally;  // (default: 2835)
    int m_iPixelsPerMeterVertically;    // (default: 2835)
    int m_iNumberOfPlanes;              // (default: 1)
    bool m_bPaletted;
    int m_iBitsPerPixel, m_iPaletteSize, m_iImportantColorQuantity, m_iDevelopedRowQuantity;
    unsigned int m_uiRedMask, m_uiGreenMask, m_uiBlueMask;
    int m_iSizeOfImageData, m_iOffsetToPalette;
};

#include "QFormatBmp.inl"

typedef SmartPointer<FormatBmp> FormatBmpPtr;

}

#endif



