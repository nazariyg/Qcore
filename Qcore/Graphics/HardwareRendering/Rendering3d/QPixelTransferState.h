#ifndef QPIXELTRANSFERSTATE_H
#define QPIXELTRANSFERSTATE_H

#include "QStateBase.h"
#include "QHeapArrayTypedefs.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM PixelTransferState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum PixelColorMappingType
    {
        PCMT_R_TO_R,
        PCMT_G_TO_G,
        PCMT_B_TO_B,
        PCMT_A_TO_A
    };

    enum ConvolutionFilterType
    {
        CFT_1D,
        CFT_2D
    };

    enum ConvolutionFilterInternalFormatType
    {
        CFIFT_ALPHA,
        CFIFT_LUMINANCE,
        CFIFT_LUMINANCE_ALPHA,
        CFIFT_INTENSITY,
        CFIFT_RGB,
        CFIFT_RGBA,
        CFIFT_ALPHA4,
        CFIFT_ALPHA8,
        CFIFT_ALPHA12,
        CFIFT_ALPHA16,
        CFIFT_LUMINANCE4,
        CFIFT_LUMINANCE8,
        CFIFT_LUMINANCE12,
        CFIFT_LUMINANCE16,
        CFIFT_LUMINANCE4_ALPHA4,
        CFIFT_LUMINANCE6_ALPHA2,
        CFIFT_LUMINANCE8_ALPHA8,
        CFIFT_LUMINANCE12_ALPHA4,
        CFIFT_LUMINANCE12_ALPHA12,
        CFIFT_LUMINANCE16_ALPHA16,
        CFIFT_INTENSITY4,
        CFIFT_INTENSITY8,
        CFIFT_INTENSITY12,
        CFIFT_INTENSITY16,
        CFIFT_R3_G3_B2,
        CFIFT_RGB4,
        CFIFT_RGB5,
        CFIFT_RGB8,
        CFIFT_RGB10,
        CFIFT_RGB12,
        CFIFT_RGB16,
        CFIFT_RGBA2,
        CFIFT_RGBA4,
        CFIFT_RGB5_A1,
        CFIFT_RGBA8,
        CFIFT_RGB10_A2,
        CFIFT_RGBA12,
        CFIFT_RGBA16,
        CFIFT_COMPRESSED_ALPHA,
        CFIFT_COMPRESSED_LUMINANCE,
        CFIFT_COMPRESSED_LUMINANCE_ALPHA,
        CFIFT_COMPRESSED_INTENSITY,
        CFIFT_COMPRESSED_RGB,
        CFIFT_COMPRESSED_RGBA
    };

    enum ConvolutionFilterBorderType
    {
        CFBT_REDUCE,
        CFBT_CONSTANT_BORDER,
        CFBT_REPLICATE_BORDER
    };

    enum ColorTablesInternalFormatType
    {
        CTIFT_ALPHA,
        CTIFT_LUMINANCE,
        CTIFT_LUMINANCE_ALPHA,
        CTIFT_INTENSITY,
        CTIFT_RGB,
        CTIFT_RGBA,
        CTIFT_ALPHA4,
        CTIFT_ALPHA8,
        CTIFT_ALPHA12,
        CTIFT_ALPHA16,
        CTIFT_LUMINANCE4,
        CTIFT_LUMINANCE8,
        CTIFT_LUMINANCE12,
        CTIFT_LUMINANCE16,
        CTIFT_LUMINANCE4_ALPHA4,
        CTIFT_LUMINANCE6_ALPHA2,
        CTIFT_LUMINANCE8_ALPHA8,
        CTIFT_LUMINANCE12_ALPHA4,
        CTIFT_LUMINANCE12_ALPHA12,
        CTIFT_LUMINANCE16_ALPHA16,
        CTIFT_INTENSITY4,
        CTIFT_INTENSITY8,
        CTIFT_INTENSITY12,
        CTIFT_INTENSITY16,
        CTIFT_R3_G3_B2,
        CTIFT_RGB4,
        CTIFT_RGB5,
        CTIFT_RGB8,
        CTIFT_RGB10,
        CTIFT_RGB12,
        CTIFT_RGB16,
        CTIFT_RGBA2,
        CTIFT_RGBA4,
        CTIFT_RGB5_A1,
        CTIFT_RGBA8,
        CTIFT_RGB10_A2,
        CTIFT_RGBA12,
        CTIFT_RGBA16,
        CTIFT_COMPRESSED_ALPHA,
        CTIFT_COMPRESSED_LUMINANCE,
        CTIFT_COMPRESSED_LUMINANCE_ALPHA,
        CTIFT_COMPRESSED_INTENSITY,
        CTIFT_COMPRESSED_RGB,
        CTIFT_COMPRESSED_RGBA
    };

    // Construction.

    // General constructor.  All the make-call sets are 'false'.
    PixelTransferState ();

    // color to color mapping
    PixelTransferState (PixelColorMappingType ePixelColorMapping, int iMapSize,
        const FloatHeapArrayPtr& rspqColorMapEntry);

    // stencil to stencil mapping
    PixelTransferState (int iMapSize, const UIntHeapArrayPtr& rspqStencilMapEntry);

    // one- and two-dimensional convolution filter
    PixelTransferState (ConvolutionFilterType eType, const RasterImagePtr& rspqFilter);

    // separable two-dimensional convolution filter
    PixelTransferState (const RasterImagePtr& rspqRowImage, const RasterImagePtr& rspqColumnImage);

    // Member setting functions.

    // regular
    void SetScale (const ColorRgba& rqScale);
    void UnsetScale ();
    void SetBias (const ColorRgba& rqBias);
    void UnsetBias ();
    void SetDepthScale (float fDepthScale);
    void UnsetDepthScale ();
    void SetDepthBias (float fDepthBias);
    void UnsetDepthBias ();

    // Convolution.  One-, two-dimensional, or separable two-dimensional.
    void SetConvolutionFilterStoreCompressed (bool bSet);  // actual for autodetect
    void SetConvolutionFilterInternalFormat (ConvolutionFilterInternalFormatType eFormat);
    void UnsetConvolutionFilterInternalFormat ();  // to do autodetect
    void SetConvolutionFilterScale (const ColorRgba& rqScale);
    void UnsetConvolutionFilterScale ();
    void SetConvolutionFilterBias (const ColorRgba& rqBias);
    void UnsetConvolutionFilterBias ();
    void SetConvolutionFilterBorder (ConvolutionFilterBorderType eConvolutionFilterBorder);
    void UnsetConvolutionFilterBorder ();
    void SetPostConvolutionRedScale (float fScale);
    void UnsetPostConvolutionRedScale ();
    void SetPostConvolutionGreenScale (float fScale);
    void UnsetPostConvolutionGreenScale ();
    void SetPostConvolutionBlueScale (float fScale);
    void UnsetPostConvolutionBlueScale ();
    void SetPostConvolutionAlphaScale (float fScale);
    void UnsetPostConvolutionAlphaScale ();
    void SetPostConvolutionRedBias (float fBias);
    void UnsetPostConvolutionRedBias ();
    void SetPostConvolutionGreenBias (float fBias);
    void UnsetPostConvolutionGreenBias ();
    void SetPostConvolutionBlueBias (float fBias);
    void UnsetPostConvolutionBlueBias ();
    void SetPostConvolutionAlphaBias (float fBias);
    void UnsetPostConvolutionAlphaBias ();

    // color matrix
    void SetColorMatrix (const Matrix4f& rqColorMatrix);
    void UnsetColorMatrix ();
    void SetPostColorMatrixRedScale (float fScale);
    void UnsetPostColorMatrixRedScale ();
    void SetPostColorMatrixGreenScale (float fScale);
    void UnsetPostColorMatrixGreenScale ();
    void SetPostColorMatrixBlueScale (float fScale);
    void UnsetPostColorMatrixBlueScale ();
    void SetPostColorMatrixAlphaScale (float fScale);
    void UnsetPostColorMatrixAlphaScale ();
    void SetPostColorMatrixRedBias (float fBias);
    void UnsetPostColorMatrixRedBias ();
    void SetPostColorMatrixGreenBias (float fBias);
    void UnsetPostColorMatrixGreenBias ();
    void SetPostColorMatrixBlueBias (float fBias);
    void UnsetPostColorMatrixBlueBias ();
    void SetPostColorMatrixAlphaBias (float fBias);
    void UnsetPostColorMatrixAlphaBias ();

    // color table
    void SetColorTable (ColorTablesInternalFormatType eColorTableInternalFormat,
        const RasterImagePtr& rspqColorTableImage);
    void UnsetColorTable ();
    void SetColorTableScale (const ColorRgba& rqColorTableScale);
    void UnsetColorTableScale ();
    void SetColorTableBias (const ColorRgba& rqColorTableBias);
    void UnsetColorTableBias ();
    void SetPostConvColorTable (ColorTablesInternalFormatType ePostConvColorTableInternalFormat,
        const RasterImagePtr& rspqPostConvColorTableImage);
    void UnsetPostConvColorTable ();
    void SetPostConvColorTableScale (const ColorRgba& rqPostConvColorTableScale);
    void UnsetPostConvColorTableScale ();
    void SetPostConvColorTableBias (const ColorRgba& rqPostConvColorTableBias);
    void UnsetPostConvColorTableBias ();
    void SetColorMatrixColorTable (ColorTablesInternalFormatType eColorMatrixColorTableInternalFormat,
        const RasterImagePtr& rspqColorMatrixColorTableImage);
    void UnsetColorMatrixColorTable ();
    void SetColorMatrixColorTableScale (const ColorRgba& rqColorMatrixColorTableScale);
    void UnsetColorMatrixColorTableScale ();
    void SetColorMatrixColorTableBias (const ColorRgba& rqColorMatrixColorTableBias);
    void UnsetColorMatrixColorTableBias ();

    // Member variables.

    // regular
    bool ScaleSet;                                      // default: false
    ColorRgba Scale;
    bool BiasSet;                                       // default: false
    ColorRgba Bias;
    bool DepthScaleSet;                                 // default: false
    float DepthScale;
    bool DepthBiasSet;                                  // default: false
    float DepthBias;

    // mapping
    bool ColorToColorMappingSet;                        // (default: false)
    PixelColorMappingType PixelColorMapping;
    int ColorToColorMapSize;
    FloatHeapArrayPtr ColorMapEntry;
    bool StencilToStencilMappingSet;                    // (default: false)
    int StencilToStencilMapSize;
    UIntHeapArrayPtr StencilMapEntry;

    // Convolution.  One-, two-dimensional, or separable two-dimensional.
    RasterImagePtr ConvolutionFilterImage;
    RasterImagePtr ConvolutionFilterSeparableRowImage;
    RasterImagePtr ConvolutionFilterSeparableColumnImage;
    ConvolutionFilterType ConvolutionFilter;
    bool AnyConvolutionFilterSet;                       // (default: false)
    bool ConvolutionFilterSeparableSet;                 // (default: false)
    bool ConvolutionFilterStoreCompressedSet;           // default: false
    bool ConvolutionFilterInternalFormatSet;            // default: false (autodetect)
    ConvolutionFilterInternalFormatType ConvolutionFilterInternalFormat;
    bool ConvolutionFilterScaleSet;                     // default: false
    ColorRgba ConvolutionFilterScale;
    bool ConvolutionFilterBiasSet;                      // default: false
    ColorRgba ConvolutionFilterBias;
    bool ConvolutionFilterBorderSet;                    // default: false
    ConvolutionFilterBorderType ConvolutionFilterBorder;
    bool PostConvolutionRedScaleSet;                    // default: false
    float PostConvolutionRedScale;
    bool PostConvolutionGreenScaleSet;                  // default: false
    float PostConvolutionGreenScale;
    bool PostConvolutionBlueScaleSet;                   // default: false
    float PostConvolutionBlueScale;
    bool PostConvolutionAlphaScaleSet;                  // default: false
    float PostConvolutionAlphaScale;
    bool PostConvolutionRedBiasSet;                     // default: false
    float PostConvolutionRedBias;
    bool PostConvolutionGreenBiasSet;                   // default: false
    float PostConvolutionGreenBias;
    bool PostConvolutionBlueBiasSet;                    // default: false
    float PostConvolutionBlueBias;
    bool PostConvolutionAlphaBiasSet;                   // default: false
    float PostConvolutionAlphaBias;

    // color matrix
    bool ColorMatrixSet;                                // default: false
    Matrix4f ColorMatrix;
    bool PostColorMatrixRedScaleSet;                    // default: false
    float PostColorMatrixRedScale;
    bool PostColorMatrixGreenScaleSet;                  // default: false
    float PostColorMatrixGreenScale;
    bool PostColorMatrixBlueScaleSet;                   // default: false
    float PostColorMatrixBlueScale;
    bool PostColorMatrixAlphaScaleSet;                  // default: false
    float PostColorMatrixAlphaScale;
    bool PostColorMatrixRedBiasSet;                     // default: false
    float PostColorMatrixRedBias;
    bool PostColorMatrixGreenBiasSet;                   // default: false
    float PostColorMatrixGreenBias;
    bool PostColorMatrixBlueBiasSet;                    // default: false
    float PostColorMatrixBlueBias;
    bool PostColorMatrixAlphaBiasSet;                   // default: false
    float PostColorMatrixAlphaBias;

    // color table
    bool ColorTableSet;                                 // default: false
    ColorTablesInternalFormatType ColorTableInternalFormat;
    RasterImagePtr ColorTableImage;
    bool ColorTableScaleSet;                            // default: false
    ColorRgba ColorTableScale;
    bool ColorTableBiasSet;                             // default: false
    ColorRgba ColorTableBias;
    bool PostConvColorTableSet;                         // default: false
    ColorTablesInternalFormatType PostConvColorTableInternalFormat;
    RasterImagePtr PostConvColorTableImage;
    bool PostConvColorTableScaleSet;                    // default: false
    ColorRgba PostConvColorTableScale;
    bool PostConvColorTableBiasSet;                     // default: false
    ColorRgba PostConvColorTableBias;
    bool ColorMatrixColorTableSet;                      // default: false
    ColorTablesInternalFormatType ColorMatrixColorTableInternalFormat;
    RasterImagePtr ColorMatrixColorTableImage;
    bool ColorMatrixColorTableScaleSet;                 // default: false
    ColorRgba ColorMatrixColorTableScale;
    bool ColorMatrixColorTableBiasSet;                  // default: false
    ColorRgba ColorMatrixColorTableBias;
};

#include "QPixelTransferState.inl"

typedef SmartPointer<PixelTransferState> PixelTransferStatePtr;

}

#endif



