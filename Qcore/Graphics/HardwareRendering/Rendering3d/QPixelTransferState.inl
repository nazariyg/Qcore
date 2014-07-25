//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetScale (const ColorRgba& rqScale)
{
    Scale = rqScale;
    ScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetScale ()
{
    ScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetBias (const ColorRgba& rqBias)
{
    Bias = rqBias;
    BiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetBias ()
{
    BiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetDepthScale (float fDepthScale)
{
    DepthScale = fDepthScale;
    DepthScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetDepthScale ()
{
    DepthScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetDepthBias (float fDepthBias)
{
    DepthBias = fDepthBias;
    DepthBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetDepthBias ()
{
    DepthBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetConvolutionFilterStoreCompressed (bool bSet)
{
    ConvolutionFilterStoreCompressedSet = bSet;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetConvolutionFilterInternalFormat (ConvolutionFilterInternalFormatType eFormat)
{
    ConvolutionFilterInternalFormat = eFormat;
    ConvolutionFilterInternalFormatSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetConvolutionFilterInternalFormat ()
{
    ConvolutionFilterInternalFormatSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetConvolutionFilterScale (const ColorRgba& rqScale)
{
    ConvolutionFilterScale = rqScale;
    ConvolutionFilterScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetConvolutionFilterScale ()
{
    ConvolutionFilterScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetConvolutionFilterBias (const ColorRgba& rqBias)
{
    ConvolutionFilterBias = rqBias;
    ConvolutionFilterBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetConvolutionFilterBias ()
{
    ConvolutionFilterBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetConvolutionFilterBorder (ConvolutionFilterBorderType eConvolutionFilterBorder)
{
    ConvolutionFilterBorder = eConvolutionFilterBorder;
    ConvolutionFilterBorderSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetConvolutionFilterBorder ()
{
    ConvolutionFilterBorderSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionRedScale (float fScale)
{
    PostConvolutionRedScale = fScale;
    PostConvolutionRedScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionRedScale ()
{
    PostConvolutionRedScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionGreenScale (float fScale)
{
    PostConvolutionGreenScale = fScale;
    PostConvolutionGreenScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionGreenScale ()
{
    PostConvolutionGreenScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionBlueScale (float fScale)
{
    PostConvolutionBlueScale = fScale;
    PostConvolutionBlueScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionBlueScale ()
{
    PostConvolutionBlueScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionAlphaScale (float fScale)
{
    PostConvolutionAlphaScale = fScale;
    PostConvolutionAlphaScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionAlphaScale ()
{
    PostConvolutionAlphaScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionRedBias (float fBias)
{
    PostConvolutionRedBias = fBias;
    PostConvolutionRedBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionRedBias ()
{
    PostConvolutionRedBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionGreenBias (float fBias)
{
    PostConvolutionGreenBias = fBias;
    PostConvolutionGreenBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionGreenBias ()
{
    PostConvolutionGreenBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionBlueBias (float fBias)
{
    PostConvolutionBlueBias = fBias;
    PostConvolutionBlueBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionBlueBias ()
{
    PostConvolutionBlueBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvolutionAlphaBias (float fBias)
{
    PostConvolutionAlphaBias = fBias;
    PostConvolutionAlphaBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvolutionAlphaBias ()
{
    PostConvolutionAlphaBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorMatrix (const Matrix4f& rqColorMatrix)
{
    ColorMatrix = rqColorMatrix;
    ColorMatrixSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorMatrix ()
{
    ColorMatrixSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixRedScale (float fScale)
{
    PostColorMatrixRedScale = fScale;
    PostColorMatrixRedScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixRedScale ()
{
    PostColorMatrixRedScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixGreenScale (float fScale)
{
    PostColorMatrixGreenScale = fScale;
    PostColorMatrixGreenScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixGreenScale ()
{
    PostColorMatrixGreenScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixBlueScale (float fScale)
{
    PostColorMatrixBlueScale = fScale;
    PostColorMatrixBlueScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixBlueScale ()
{
    PostColorMatrixBlueScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixAlphaScale (float fScale)
{
    PostColorMatrixAlphaScale = fScale;
    PostColorMatrixAlphaScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixAlphaScale ()
{
    PostColorMatrixAlphaScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixRedBias (float fBias)
{
    PostColorMatrixRedBias = fBias;
    PostColorMatrixRedBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixRedBias ()
{
    PostColorMatrixRedBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixGreenBias (float fBias)
{
    PostColorMatrixGreenBias = fBias;
    PostColorMatrixGreenBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixGreenBias ()
{
    PostColorMatrixGreenBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixBlueBias (float fBias)
{
    PostColorMatrixBlueBias = fBias;
    PostColorMatrixBlueBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixBlueBias ()
{
    PostColorMatrixBlueBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostColorMatrixAlphaBias (float fBias)
{
    PostColorMatrixAlphaBias = fBias;
    PostColorMatrixAlphaBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostColorMatrixAlphaBias ()
{
    PostColorMatrixAlphaBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorTable (ColorTablesInternalFormatType eColorTableInternalFormat,
    const RasterImagePtr& rspqColorTableImage)
{
    ColorTableInternalFormat = eColorTableInternalFormat;
    ColorTableImage = rspqColorTableImage;
    ColorTableSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorTable ()
{
    ColorTableSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorTableScale (const ColorRgba& rqColorTableScale)
{
    ColorTableScale = rqColorTableScale;
    ColorTableScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorTableScale ()
{
    ColorTableScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorTableBias (const ColorRgba& rqColorTableBias)
{
    ColorTableBias = rqColorTableBias;
    ColorTableBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorTableBias ()
{
    ColorTableBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvColorTable (
    ColorTablesInternalFormatType ePostConvColorTableInternalFormat,
    const RasterImagePtr& rspqPostConvColorTableImage)
{
    PostConvColorTableInternalFormat = ePostConvColorTableInternalFormat;
    PostConvColorTableImage = rspqPostConvColorTableImage;
    PostConvColorTableSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvColorTable ()
{
    PostConvColorTableSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvColorTableScale (const ColorRgba& rqPostConvColorTableScale)
{
    PostConvColorTableScale = rqPostConvColorTableScale;
    PostConvColorTableScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvColorTableScale ()
{
    PostConvColorTableScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetPostConvColorTableBias (const ColorRgba& rqPostConvColorTableBias)
{
    PostConvColorTableBias = rqPostConvColorTableBias;
    PostConvColorTableBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetPostConvColorTableBias ()
{
    PostConvColorTableBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorMatrixColorTable (
    ColorTablesInternalFormatType eColorMatrixColorTableInternalFormat,
    const RasterImagePtr& rspqColorMatrixColorTableImage)
{
    ColorMatrixColorTableInternalFormat = eColorMatrixColorTableInternalFormat;
    ColorMatrixColorTableImage = rspqColorMatrixColorTableImage;
    ColorMatrixColorTableSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorMatrixColorTable ()
{
    ColorMatrixColorTableSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorMatrixColorTableScale (const ColorRgba& rqColorMatrixColorTableScale)
{
    ColorMatrixColorTableScale = rqColorMatrixColorTableScale;
    ColorMatrixColorTableScaleSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorMatrixColorTableScale ()
{
    ColorMatrixColorTableScaleSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::SetColorMatrixColorTableBias (const ColorRgba& rqColorMatrixColorTableBias)
{
    ColorMatrixColorTableBias = rqColorMatrixColorTableBias;
    ColorMatrixColorTableBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void PixelTransferState::UnsetColorMatrixColorTableBias ()
{
    ColorMatrixColorTableBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------



