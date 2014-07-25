#ifndef QRASTERIMAGE_H
#define QRASTERIMAGE_H

// TERMS AND DEFINITIONS.
// paletted image    : an indexed-color image i.e. a pixel of which consists of one component being the index of
//                     a color (or color-alpha association) in the palette.
// gradational image : an image whose pixel component values can vary to indicate lower and higher levels of
//                     intensity/transparency; here, any non-paletted image.
// color model       : an image property that determines in which color space the image is represented and
//                     whether the image has transparency information; is characterized by the number and
//                     meaning of pixel components.
// image type        : an image property that, according to the color model, determines the structure of a
//                     pixel (for a gradational image); is characterized by the numeric type and bit widths of
//                     pixel samples.
// pixel component   : a constituent of a pixel; either an integer or floating-point number.
// pixel sample      : a group of bits associated with a pixel component.
// alpha             : numerical representation of transparency; the minimum value means full transparency, the
//                     maximum value means full opacity.
// row               : a horizontal line of pixels in an image.
// row stride        : an image property that indicates the size of a row in bytes, counting padding bytes if
//                     any.
//
// An object of the RasterImage class represents a rectangular raster image, strictly speaking, a
// two-dimensional array of pixels accompanied by attribute information.  The maximum number width and height
// of the image can be is 30,000 pixels.  The origin of coordinates is the top-left pixel of the image.  X and
// Y pixel coordinates are zero-based, that is, the top-left pixel is (0, 0), the pixel to the right is (1, 0),
// the pixel to the bottom is (0, 1).  The supported color models are: paletted, grayscale, RGB, CMYK, CIE
// L*a*b*, plus their with-alpha variants, which makes ten in total.  Any gradational image with floating-point
// pixel components can be in the High Dynamic Range mode, which allows a pixel component to have values beyond
// the usual range.  For a paletted image, the maximum number of entries in the palette is 256.  An image
// without transparency information can have a visibility mask that tells which pixel is visible and which is
// not.  A black-and-white image, also known as bi-level, is represented as an IT_PALETTED image for the
// reasons of generality and performance; the palette of such an image has two colors: black and white, in that
// order.
//
// The in-memory layout of the image data of a RasterImage object is the one that maximizes the number of image
// types that are directly (via the memory address of the data) usable by standard graphics systems such as
// DirectX and OpenGL.  A RasterImage object stores the image data in one block of heap memory; the palette, if
// present, is stored separately.  Inside the block, first go pixels of the topmost row (left-to-right), then
// go padding bytes if any (see below), then pixels of the row to the bottom, and so on.  A pixel is always
// byte-aligned in memory and occupies an integer number of bytes (1, 2, 3, or more).  To facilitate
// performance, the block always has a memory address multiple of 16 and the byte size of a row (row stride)
// multiple of the number defined by ms_iRowAlignment static member constant (which is a multiple of 4).  If
// the multiplicity of the row stride is not innate for an image, the pixel data of a row of the image is
// padded at the end by the same appropriate number of unused bytes.  A pixel sample of 8, 16, or 32 bit size
// is dealt with as an unsigned-integer or floating-point C++ data type of that size (endianness does matter):
// for 8 bits it is 'unsigned char' type, for 16 bits it is 'unsigned short', and for 32 bits it is 'unsigned
// int' or 'float' depending on the image type; a pixel sample of any other size is treated as a bit field
// (endianness does not matter).  Pixel samples of a L*a*b* image are encoded as specified in the public paper
// "Adobe Photoshop TIFF Technical Notes" (of March 22, 2002, p. 12).  A visibility mask, if present, is an
// array of 'char' data in the quantity equal to the number of pixels in the image, and is arranged in the same
// order as the pixels are in memory (left-to-right, top-to-bottom); they can take only two values: 1 means
// that the associated pixel should be considered as visible, 0 means the opposite.
//
// NOTE. There are a lot of dependencies between implementations of this class and the RasterImageConversion,
// RasterImageQuantization, RasterImageTransformation classes, or any other class dedicated to the raster image
// that much.

#include "QSmartPointer.h"
#include "QHeapArray.h"
#include "QPaletteColor.h"
#include "QColorRgb.h"
#include "QColorRgba.h"
#include "QColorCmyk.h"
#include "QColorCmyka.h"
#include "QColorLab.h"
#include "QColorLaba.h"
#include "QRecti.h"

namespace Q
{

class Q_ITEM RasterImage : public Heap
{
public:
    // The list of supported image types is a combination of pixel layouts native to Adobe Photoshop and
    // DirectX/OpenGL.  For a gradational image, the enumerand of an image type, in addition to the color
    // model, indicates the in-memory layout of a pixel; a pixel of a paletted image is an 8-bit integer.  If a
    // gradational-image enumerand is not concluded with 'FLOAT', all the pixel components are integers of the
    // specified bit widths (separated by '_'); otherwise, all the pixel components are standard IEEE 32-bit
    // floating-points.  The in-memory order of pixel samples is the same as in the designation of the color
    // model (alpha always goes last).
    enum ImageType
    {
        IT_PALETTED,
        IT_PALETTED_WITH_ALPHA,
        IT_GRAYSCALE_8,
        IT_GRAYSCALE_16,
        IT_GRAYSCALE_24,
        IT_GRAYSCALE_32,
        IT_GRAYSCALE_FLOAT,
        IT_GRAYSCALE_WITH_ALPHA_8_8,
        IT_GRAYSCALE_WITH_ALPHA_16_16,
        IT_GRAYSCALE_WITH_ALPHA_FLOAT,
        IT_RGB_3_3_2,
        IT_RGB_5_6_5,
        IT_RGB_8_8_8,
        IT_RGB_16_16_16,
        IT_RGB_FLOAT,
        IT_RGBA_4_4_4_4,
        IT_RGBA_8_8_8_8,
        IT_RGBA_16_16_16_16,
        IT_RGBA_5_5_5_1,
        IT_RGBA_10_10_10_2,
        IT_RGBA_FLOAT,
        IT_CMYK_8_8_8_8,
        IT_CMYK_16_16_16_16,
        IT_CMYK_FLOAT,
        IT_CMYK_WITH_ALPHA_8_8_8_8_8,
        IT_CMYK_WITH_ALPHA_16_16_16_16_16,
        IT_CMYK_WITH_ALPHA_FLOAT,
        IT_LAB_8_8_8,
        IT_LAB_16_16_16,
        IT_LAB_FLOAT,
        IT_LAB_WITH_ALPHA_8_8_8_8,
        IT_LAB_WITH_ALPHA_16_16_16_16,
        IT_LAB_WITH_ALPHA_FLOAT
    };

    // color models
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_PALETTED_WITH_ALPHA,
        CMT_GRAYSCALE,
        CMT_GRAYSCALE_WITH_ALPHA,
        CMT_RGB,
        CMT_RGBA,
        CMT_CMYK,
        CMT_CMYK_WITH_ALPHA,
        CMT_LAB,
        CMT_LAB_WITH_ALPHA
    };

    enum PixelComponentType
    {
        PCT_UNSIGNED_INTEGER,
        PCT_FLOATING_POINT
    };

    // typedefs
    typedef StillArray<PaletteColor> Palette;
    typedef StillArray<char> VisibilityMask;

    // construction
    RasterImage (ImageType eType, int iWidth, int iHeight, bool bIsHighDynamicRange = false);
    RasterImage (ImageType eType, int iWidth, int iHeight, const Palette& rqPalette);

    // destruction
    ~RasterImage ();

    // member access
    ImageType GetType () const;
    int GetWidth () const;
    int GetHeight () const;
    bool IsHighDynamicRange () const;
    ColorModelType GetColorModel () const;
    int GetBitsPerPixel () const;  // "bpp"
    char* GetImage ();
    const char* GetImage () const;
    operator char* ();
    operator const char* () const;
    char* GetRow (int iRow);
    const char* GetRow (int iRow) const;
    int GetRowStride () const;
    int GetBytesPerPixel () const;
    int GetComponentsPerPixel () const;
    bool HasAlpha () const;
    PixelComponentType GetPixelComponentType () const;
    bool HasGradIntSampleUniformity () const;  // 'true' if all samples are vary-integers of the same bit width
    int GetBitsPerIntegerSample () const;      // can be called only if HasGradIntSampleUniformity returns 'true'
    int GetArea () const;

    // generalized pixel access
    void SetPixelIndex (int iIndex, int iX, int iY) const;
    int GetPixelIndex (int iX, int iY) const;
    void SetPixelIntensity (float fIntensity, int iX, int iY) const;
    float GetPixelIntensity (int iX, int iY) const;
    void SetPixelIntensityWithAlpha (const Link<float,float>& rqIntensityWithAlpha, int iX, int iY) const;
    Link<float,float> GetPixelIntensityWithAlpha (int iX, int iY) const;
    void SetPixelRgb (const ColorRgb& rqRgb, int iX, int iY) const;
    ColorRgb GetPixelRgb (int iX, int iY) const;
    void SetPixelRgba (const ColorRgba& rqRgba, int iX, int iY) const;
    ColorRgba GetPixelRgba (int iX, int iY) const;
    void SetPixelCmyk (const ColorCmyk& rqCmyk, int iX, int iY) const;
    ColorCmyk GetPixelCmyk (int iX, int iY) const;
    void SetPixelCmyka (const ColorCmyka& rqCmyka, int iX, int iY) const;
    ColorCmyka GetPixelCmyka (int iX, int iY) const;
    void SetPixelLab (const ColorLab& rqLab, int iX, int iY) const;
    ColorLab GetPixelLab (int iX, int iY) const;
    void SetPixelLaba (const ColorLaba& rqLaba, int iX, int iY) const;
    ColorLaba GetPixelLaba (int iX, int iY) const;

    // palette related
    void SetPalette (const Palette& rqPalette);
    Palette& GetPalette ();
    const Palette& GetPalette () const;
    int GetPaletteSize () const;
    PaletteColor& GetPaletteColor (int i);
    const PaletteColor& GetPaletteColor (int i) const;
    bool OptimizePalette ();  // removes unreferenced entries and duplicates; returns 'true' if were any
    void SortPalette ();      // places darker colors before brighter ones (and updates image indices)

    // visibility mask related
    bool HasVisibilityMask () const;
    bool IsPixelVisible (int iX, int iY) const;  // can be called only if there is a visibility mask
    void SetVisibilityMask (const VisibilityMask& rqVisibilityMask);
    VisibilityMask& GetVisibilityMask ();
    const VisibilityMask& GetVisibilityMask () const;
    void RemoveVisibilityMask ();

    // image copying
    SmartPointer<RasterImage> GetDuplicate (bool bCopyPixels = true, bool bCopyVisibilityMask = true) const;

    // Subimage access.  An image can be pasted into another image if it has the same characteristics (only
    // dimensions can differ).
    void SetSubimage (const SmartPointer<RasterImage>& rspqSubimage, int iX, int iY);
    SmartPointer<RasterImage> GetSubimage (int iX, int iY, int iWidth, int iHeight) const;
    SmartPointer<RasterImage> GetSubimage (const Recti& rqRect) const;

    // only for HDR images (and except L*a*b* images)
    void ClampComponentsToUnitInterval ();

private:
    friend class RasterImageConversion;
    friend class RasterImageAdjustment;
    friend class RasterImageTransformation;

    static ColorModelType GetColorModelFromType (ImageType eType);
    void Initialize (int iWidth, int iHeight);

    ImageType m_eType;
    Palette m_qPalette;
    ColorModelType m_eColorModel;
    int m_iWidth, m_iHeight, m_iArea;
    bool m_bIsHighDynamicRange;
    int m_iRowStride;
    char* m_acImageContainer;
    char* m_acImage;
    int m_iBitsPerPixel, m_iBytesPerPixel, m_iComponentsPerPixel;
    PixelComponentType m_ePixelComponentType;
    bool m_bHasGradIntSampleUniformity;
    int m_iBitsPerIntegerSample;
    bool m_bHasAlpha;
    VisibilityMask m_qVisibilityMask;

    // the number the row stride is to be a multiple of
    static const int ms_iRowAlignment;  // 4 (this number is default to DirectX/OpenGL)
};

#include "QRasterImage.inl"

typedef SmartPointer<RasterImage> RasterImagePtr;
typedef SmartPointer<HeapArray<RasterImagePtr> > RasterImagePtrArrayPtr;

}

#endif



