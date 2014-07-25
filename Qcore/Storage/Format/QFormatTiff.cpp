#include "QFormatTiff.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatTiff.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QCompressionFax.h"
#include "QCompressionLzw.h"
#include "QCompressionZLib.h"
#include "QFormatJpeg.h"
#include "QCompressionPackBits.h"
#include "QRasterImageTransformation.h"
using namespace Q;

// static member constants
const int FormatTiff::ms_iKBytesPerSegment(32);
const float FormatTiff::ms_fDefaultXResolutionInchForSave(72.0f);
const float FormatTiff::ms_fDefaultYResolutionInchForSave(72.0f);
const int FormatTiff::ms_iRationalNumbersScaler(10000);
const bool FormatTiff::ms_bChooseBestFromManifoldZLibStrategies(true);
const bool FormatTiff::ms_bTrimTexts(true);

//------------------------------------------------------------------------------------------------------------------
FormatTiff::ImageFileDirectory::ImageFileDirectory (const RasterImagePtr& rspqImage,
    const StillArray<Short>& rqBitsPerSample, bool bTreatImageAsBilevel)
    :
    m_spqImageSaveTo(rspqImage),
    m_qBitsPerSample(rqBitsPerSample),
    m_bBitsPerSampleFieldIsPresent(true),
    m_bImageIsBilevel(bTreatImageAsBilevel),
    DEFAULT_INIT_LIST
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( !(rspqImage && (rspqImage->GetColorModel() != RasterImage::CMT_PALETTED
        ||  rspqImage->GetPaletteSize() != 2
        ||  rspqImage->GetPaletteColor(0) != PaletteColor(0,0,0)
        ||  rspqImage->GetPaletteColor(1) != PaletteColor(255,255,255))
        &&  bTreatImageAsBilevel) );
}
//------------------------------------------------------------------------------------------------------------------
FormatTiff::ImageFileDirectory::ImageFileDirectory ()
    :
    m_bBitsPerSampleFieldIsPresent(false),
    m_bImageIsBilevel(false),
    DEFAULT_INIT_LIST
{
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::ImageFileDirectory::AssignBitsPerSampleAndSampleFormatDefaults ()
{
    // this function is separated from AssignDerivedDefaultsAndAttributes because it is used by SMinSampleValue
    // and SMaxSampleValue field reading routines in order to know the values being assigned next

    if ( !m_bBitsPerSampleFieldIsPresent )
    {
        m_qBitsPerSample.SetQuantity(m_SamplesPerPixel);
        for (int i = 0; i < m_SamplesPerPixel; i++)
            m_qBitsPerSample[i] = 1;
    }

    if ( !m_bSampleFormatFieldIsPresent )
    {
        m_qSampleFormat.SetQuantity(m_SamplesPerPixel);
        for (int i = 0; i < m_SamplesPerPixel; i++)
            m_qSampleFormat[i] = SFT_UNSIGNED_INTEGER;
    }

    m_bBitsPerSamplesAreTheSame = true;
    for (int i = 1; i < m_SamplesPerPixel; i++)
    {
        if ( m_qBitsPerSample[i] != m_qBitsPerSample[0] )
        {
            m_bBitsPerSamplesAreTheSame = false;
            break;
        }
    }

    if ( m_bBitsPerSamplesAreTheSame )
        m_iBitsPerEachSample = m_qBitsPerSample[0];

    m_bPixelSamplesAllAreUI = true;
    for (int i = 0; i < m_SamplesPerPixel; i++)
    {
        if ( m_qSampleFormat[i] != SFT_UNSIGNED_INTEGER && m_qSampleFormat[i] != SFT_UNDEFINED )
        {
            m_bPixelSamplesAllAreUI = false;
            break;
        }
    }

    m_iBitsPerPixel = 0;
    for (int i = 0; i < m_SamplesPerPixel; i++)
        m_iBitsPerPixel += m_qBitsPerSample[i];
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::AssignDerivedDefaultsAndAttributes ()
{
    // This function is to assign defaults before TIFF file requirement checks are done (main part of them).
    // But it anyway must know if input data is correct, so, to minify redundancy, shared functions (like
    // Is*Allowed) are used.

    // some critical checks
    if ( m_qBitsPerSample.GetQuantity() != m_SamplesPerPixel
    ||   m_qSampleFormat.GetQuantity() != m_SamplesPerPixel
    ||   !m_bImageWidthFieldIsPresent || !m_bImageLengthFieldIsPresent
    ||   !m_bPhotometricInterpretationFieldIsPresent )
    {
        assert( false );
        return false;
    }

    if ( m_bPixelSamplesAllAreUI )
    {
        // assign sample range related attributes
        m_qSampleUIValueRanges.SetQuantity(m_SamplesPerPixel);
        m_qSampleUITopValues.SetQuantity(m_SamplesPerPixel);
        int iRange;
        for (int i = 0; i < m_SamplesPerPixel; i++)
        {
            iRange = 1 << m_qBitsPerSample[i];
            m_qSampleUIValueRanges[i] = iRange;
            m_qSampleUITopValues[i] = iRange - 1;
        }

        if ( m_bBitsPerSamplesAreTheSame )
        {
            m_iUIValueRangePerEachSample = m_qSampleUIValueRanges[0];
            m_iUITopValuePerEachSample = m_qSampleUITopValues[0];
        }
    }

    if ( !m_bMinSampleValueFieldIsPresent && IsMinSampleAllowed() )
    {
        m_qMinSampleValue.SetQuantity(m_SamplesPerPixel);
        for (int i = 0; i < m_SamplesPerPixel; i++)
            m_qMinSampleValue[i] = 0;
    }

    if ( !m_bMaxSampleValueFieldIsPresent && IsMaxSampleAllowed() )
    {
        m_qMaxSampleValue.SetQuantity(m_SamplesPerPixel);
        for (int i = 0; i < m_SamplesPerPixel; i++)
            m_qMaxSampleValue[i] = m_qSampleUITopValues[i];
    }

    // an image has a colorimetric interpretation if and only if both WhitePoint and PrimaryChromaticities
    // fields are present
    m_bImageHasColorimetricInterpretation =
        ( m_bWhitePointFieldIsPresent && m_bPrimaryChromaticitiesFieldIsPresent );

    if ( !m_bTransferFunctionFieldIsPresent && IsTransferFunctionAllowed() )
    {
        int iTableQuantity;
        if ( m_bBitsPerSamplesAreTheSame )
        {
            iTableQuantity = 1;
            m_qTransferFunction.SetQuantity(m_iUIValueRangePerEachSample);
        }
        else
        {
            iTableQuantity = 3;
            int iRangeSum = 0;
            for (int i = 0; i < m_SamplesPerPixel; i++)
                iRangeSum += m_qSampleUIValueRanges[i];
            m_qTransferFunction.SetQuantity(iRangeSum);
        }

        int iIndex = 0;
        for (int i0 = 0; i0 < iTableQuantity; i0++)
        {
            m_qTransferFunction[iIndex++] = 0;
            for (int i1 = 1; i1 < m_qSampleUIValueRanges[i0]; i1++)
            {
                m_qTransferFunction[iIndex++] =
                    (Short)Mathf::Floor(Mathf::Pow(((float)i1)/m_qSampleUITopValues[i0],2.2f)*65535+0.5f);
            }
        }
    }

    if ( !m_bDotRangeFieldIsPresent && IsDotRangeAllowed() )
    {
        if ( m_bBitsPerSamplesAreTheSame )
        {
            m_qDotRange.SetQuantity(1);
            m_qDotRange[0] = Link<Short,Short>(0,m_iUITopValuePerEachSample);
        }
        else
        {
            m_qDotRange.SetQuantity(m_SamplesPerPixel);
            for (int i = 0; i < m_SamplesPerPixel; i++)
                m_qDotRange[i] = Link<Short,Short>(0,m_qSampleUITopValues[i]);
        }
    }

    if ( !m_bSMinSampleValueFieldIsPresent )
    {
        m_qSMinSampleValueUI.SetQuantity(m_SamplesPerPixel);
        m_qSMinSampleValueSI.SetQuantity(m_SamplesPerPixel);
        m_qSMinSampleValueFP.SetQuantity(m_SamplesPerPixel);
        SampleFormatType eSFT;
        Short SampleBits;
        for (int i = 0; i < m_SamplesPerPixel; i++)
        {
            eSFT = m_qSampleFormat[i];
            SampleBits = m_qBitsPerSample[i];

            if ( eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_UNDEFINED )
                m_qSMinSampleValueUI[i] = 0;
            else if ( eSFT == SFT_SIGNED_INTEGER )
                m_qSMinSampleValueSI[i] = -(1 << (SampleBits - 1));
            else  // eSFT = SFT_FLOATING_POINT
                m_qSMinSampleValueFP[i] = 0.0f;
        }
    }

    if ( !m_bSMaxSampleValueFieldIsPresent )
    {
        m_qSMaxSampleValueUI.SetQuantity(m_SamplesPerPixel);
        m_qSMaxSampleValueSI.SetQuantity(m_SamplesPerPixel);
        m_qSMaxSampleValueFP.SetQuantity(m_SamplesPerPixel);
        SampleFormatType eSFT;
        Short SampleBits;
        for (int i = 0; i < m_SamplesPerPixel; i++)
        {
            eSFT = m_qSampleFormat[i];
            SampleBits = m_qBitsPerSample[i];

            if ( eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_UNDEFINED )
                m_qSMaxSampleValueUI[i] = m_qSampleUITopValues[i];
            else if ( eSFT == SFT_SIGNED_INTEGER )
                m_qSMaxSampleValueSI[i] = (1 << (SampleBits - 1)) - 1;
            else  // eSFT = SFT_FLOATING_POINT
                m_qSMaxSampleValueFP[i] = 1.0f;
        }
    }

    m_qSampleSTopValuesUI.SetQuantity(m_SamplesPerPixel);
    m_qSampleSTopValuesSI.SetQuantity(m_SamplesPerPixel);
    m_qSampleSTopValuesFP.SetQuantity(m_SamplesPerPixel);
    for (int i = 0; i < m_SamplesPerPixel; i++)
    {
        SampleFormatType eSFT = m_qSampleFormat[i];

        if ( eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_UNDEFINED )
            m_qSampleSTopValuesUI[i] = m_qSMaxSampleValueUI[i] - m_qSMinSampleValueUI[i];
        else if ( eSFT == SFT_SIGNED_INTEGER )
            m_qSampleSTopValuesSI[i] = m_qSMaxSampleValueSI[i] - m_qSMinSampleValueSI[i];
        else  // eSFT = SFT_FLOATING_POINT
            m_qSampleSTopValuesFP[i] = m_qSMaxSampleValueFP[i] - m_qSMinSampleValueFP[i];
    }

    if ( !m_bTransferRangeFieldIsPresent && IsTransferRangeAllowed() )
    {
        m_TransferBlack0 = 0;
        m_TransferWhite0 = m_qSampleUITopValues[0];
        m_TransferBlack1 = 0;
        m_TransferWhite1 = m_qSampleUITopValues[1];
        m_TransferBlack2 = 0;
        m_TransferWhite2 = m_qSampleUITopValues[2];
    }

    if ( !m_bReferenceBlackWhiteFieldIsPresent && IsReferenceBlackWhiteAllowed() )
    {
        m_fReferenceBlack0 = 0.0f;
        m_fReferenceWhite0 = (float)m_qSampleUITopValues[0];
        m_fReferenceBlack1 = 0.0f;
        m_fReferenceWhite1 = (float)m_qSampleUITopValues[1];
        m_fReferenceBlack2 = 0.0f;
        m_fReferenceWhite2 = (float)m_qSampleUITopValues[2];
    }

    // generalize subfile profile, NewSubfileType preferred
    if ( m_bNewSubfileTypeFieldIsPresent
    ||   (!m_bNewSubfileTypeFieldIsPresent && !m_bSubfileTypeFieldIsPresent) )
    {
        // copy or get the defaults
        m_bIsReducedResolutionVersion = m_bSubfileIsReducedResolutionVersion;
        m_bIsSinglePageOfMultipageImage = m_bSubfileIsSinglePageOfMultipageImage;
        m_bIsTransparencyMask = m_bSubfileIsTransparencyMask;
    }
    else  // m_bSubfileTypeFieldIsPresent = true
    {
        m_bIsReducedResolutionVersion = ( m_eSubfileType == ST_REDUCED_RESOLUTION_IMAGE );
        m_bIsSinglePageOfMultipageImage = ( m_eSubfileType == ST_SINGLE_PAGE_OF_MULTIPAGE_IMAGE );
        m_bIsTransparencyMask = false;
    }
    if ( m_bIsTransparencyMask && m_ePhotometricInterpretation != PIT_TRANSPARENCY_MASK )
    {
        assert( false );
        m_bIsTransparencyMask = false;
    }

    m_bImageIsYCbCrNonJpeg = ( m_ePhotometricInterpretation == PIT_YCC && m_eCompression != CT_JPEG_TNT );

    // derived color model
    m_bAlmostPalettedWithAlpha = false;
    if ( (m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO
    ||   m_ePhotometricInterpretation == PIT_BLACK_IS_ZERO) && m_SamplesPerPixel == 1 && m_iBitsPerEachSample == 1
    &&   !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_BILEVEL;
    }
    else if ( (m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO
         ||   m_ePhotometricInterpretation == PIT_BLACK_IS_ZERO) && m_SamplesPerPixel == 1
         &&   !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_GRAYSCALE;
    }
    else if ( (m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO
         ||   m_ePhotometricInterpretation == PIT_BLACK_IS_ZERO) && m_SamplesPerPixel == 2
         &&   m_bExtraSamplesFieldIsPresent && m_qExtraSamples.GetQuantity() == 1 )
    {
        m_eColorModel = CMT_GRAYSCALE_WITH_ALPHA;
    }
    else if ( m_ePhotometricInterpretation == PIT_RGB && m_SamplesPerPixel == 3 && !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_RGB;
    }
    else if ( m_ePhotometricInterpretation == PIT_RGB && m_SamplesPerPixel == 4 && m_bExtraSamplesFieldIsPresent
         &&   m_qExtraSamples.GetQuantity() == 1 )
    {
        m_eColorModel = CMT_RGBA;
    }
    else if ( m_ePhotometricInterpretation == PIT_PALETTED && m_SamplesPerPixel == 1
         &&   !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_PALETTED;
    }
    else if ( m_ePhotometricInterpretation == PIT_PALETTED && m_SamplesPerPixel == 2
         &&   m_bExtraSamplesFieldIsPresent && m_qExtraSamples.GetQuantity() == 1
         &&   m_qExtraSamples[0] == EST_UNASSOCIATED_ALPHA )
    {
        m_eColorModel = CMT_RGBA;
        m_bAlmostPalettedWithAlpha = true;
    }
    else if ( m_ePhotometricInterpretation == PIT_CMYK && m_SamplesPerPixel == 4 && !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_CMYK;
    }
    else if ( m_ePhotometricInterpretation == PIT_YCC && m_SamplesPerPixel == 3 && !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_YCC;
    }
    else if ( m_ePhotometricInterpretation == PIT_CIE_LAB && m_SamplesPerPixel == 3
         &&   !m_bExtraSamplesFieldIsPresent )
    {
        if ( !(m_bBitsPerSamplesAreTheSame && ((m_bPixelSamplesAllAreUI
        &&   (m_iBitsPerEachSample == 8 || m_iBitsPerEachSample == 16)) || m_iBitsPerEachSample == 32)) )
        {
            assert( false );
            return false;
        }
        m_eColorModel = CMT_LAB;
    }
    else if ( m_ePhotometricInterpretation == PIT_CIE_LAB && m_SamplesPerPixel == 4 && m_bExtraSamplesFieldIsPresent
         &&   m_qExtraSamples.GetQuantity() == 1 )
    {
        m_eColorModel = CMT_LAB_WITH_ALPHA;
    }
    else if ( m_bIsTransparencyMask && m_SamplesPerPixel == 1 && m_iBitsPerEachSample == 1
         &&   !m_bExtraSamplesFieldIsPresent )
    {
        m_eColorModel = CMT_GRAYSCALE;
    }
    else
    {
        assert( false );
        return false;
    }

    // strip/tile attributes
    if ( !(m_bStripOffsetsFieldIsPresent && m_bStripByteCountsFieldIsPresent
    &&   !m_bTileOffsetsFieldIsPresent && !m_bTileByteCountsFieldIsPresent
    &&   m_bTileWidthFieldIsPresent && m_bTileLengthFieldIsPresent) )
    {
        // usual case
        m_bStripsAreSpecified = ( m_bStripOffsetsFieldIsPresent && m_bStripByteCountsFieldIsPresent );
        m_bTilesAreSpecified = ( m_bTileOffsetsFieldIsPresent && m_bTileByteCountsFieldIsPresent
                            &&   m_bTileWidthFieldIsPresent && m_bTileLengthFieldIsPresent );
    }
    else
    {
        // special case for a tiled image
        m_bStripsAreSpecified = false;
        m_bTilesAreSpecified = true;
        m_qTileOffsets = m_qStripOffsets;
        m_qTileByteCounts = m_qStripByteCounts;
        m_qStripOffsets.RemoveAll();
        m_qStripByteCounts.RemoveAll();
    }
    if ( m_bStripsAreSpecified )
    {
        m_iRowsPerStrip = ( m_RowsPerStrip != 4294967295 ? m_RowsPerStrip : m_ImageLength );
        m_iStripsPerImage = (m_ImageLength + m_iRowsPerStrip - 1)/m_iRowsPerStrip;
        m_iSegmentsPerPlane = m_iStripsPerImage;
        if ( m_ePlanarConfiguration == PCT_PLANAR_FORMAT )
            m_iStripsPerImage *= m_SamplesPerPixel;
    }
    if ( m_bTilesAreSpecified )
    {
        m_iTilesAcross = (m_ImageWidth + m_TileWidth - 1)/m_TileWidth;
        m_iTilesDown = (m_ImageLength + m_TileLength - 1)/m_TileLength;
        m_iTilesPerImage = m_iTilesAcross*m_iTilesDown;
        m_iSegmentsPerPlane = m_iTilesPerImage;
        if ( m_ePlanarConfiguration == PCT_PLANAR_FORMAT )
            m_iTilesPerImage *= m_SamplesPerPixel;
    }

    // critical check: either strips or tiles
    int iS = 0;
    int iT = 0;
    if ( m_bStripsAreSpecified
    &&   m_qStripOffsets.GetQuantity() == m_iStripsPerImage
    &&   m_qStripByteCounts.GetQuantity() == m_iStripsPerImage )
    {
        iS = 1;
    }
    if ( m_bTilesAreSpecified
    &&   m_qTileOffsets.GetQuantity() == m_iTilesPerImage
    &&   m_qTileByteCounts.GetQuantity() == m_iTilesPerImage )
    {
        iT = 1;
    }
    if ( iS + iT != 1 )
    {
        assert( false );
        return false;
    }

    // generalize strip/tile organization by "segment"
    int iChunkySegmentRowSizeInBits, iChunkySegmentRowStride, iPlanarSegmentRowSizeInBits, iPlanarSegmentRowStride,
        iLumaOnlyRowStride;
    if ( m_bImageIsYCbCrNonJpeg
    &&   (m_qBitsPerSample.GetQuantity() != 3 || m_ePlanarConfiguration == PCT_PLANAR_FORMAT
    ||   !m_bBitsPerSamplesAreTheSame || m_iBitsPerEachSample != 8) )
    {
        assert( false );
        return false;
    }
    if ( m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
    {
        int iSW = ( m_bStripsAreSpecified ? m_ImageWidth : m_TileWidth );
        if ( !m_bImageIsYCbCrNonJpeg )
        {
            iChunkySegmentRowSizeInBits = iSW*m_iBitsPerPixel;
            iChunkySegmentRowStride = ( (iChunkySegmentRowSizeInBits & 7) == 0 ? iChunkySegmentRowSizeInBits/8 :
                iChunkySegmentRowSizeInBits/8 + 1 );
        }
        else
        {
            iChunkySegmentRowSizeInBits = iSW*m_qBitsPerSample[0];
            iLumaOnlyRowStride = iChunkySegmentRowSizeInBits/8;
            switch ( m_eYCbCrSubSamplingHoriz )
            {
            case YCCSST_ONE_TO_ONE:
                iChunkySegmentRowSizeInBits += iSW/1*(m_qBitsPerSample[1] + m_qBitsPerSample[2]);
                break;
            case YCCSST_ONE_TO_TWO:
                iChunkySegmentRowSizeInBits += iSW/2*(m_qBitsPerSample[1] + m_qBitsPerSample[2]);
                break;
            case YCCSST_ONE_TO_FOUR:
                iChunkySegmentRowSizeInBits += iSW/4*(m_qBitsPerSample[1] + m_qBitsPerSample[2]);
                break;
            }

            // all bits per samples must be multiple of 8 in this case
            iChunkySegmentRowStride = iChunkySegmentRowSizeInBits/8;
        }
    }
    if ( m_bStripsAreSpecified )
    {
        // strips are used
        m_qSegments.SetQuantity(m_iStripsPerImage);
        int iRowSum = 0;
        for (int i = 0; i < m_iStripsPerImage; i++)
        {
            SegmentRecord& rqSegment = m_qSegments[i];

            rqSegment.Offset = m_qStripOffsets[i];
            rqSegment.ByteCount = m_qStripByteCounts[i];
            rqSegment.Width = m_ImageWidth;
            rqSegment.Length =
                ( iRowSum + m_iRowsPerStrip <= (int)m_ImageLength ? m_iRowsPerStrip : m_ImageLength - iRowSum );
            if ( m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                if ( !m_bImageIsYCbCrNonJpeg )
                {
                    rqSegment.UncompressedByteCount = rqSegment.Length*iChunkySegmentRowStride;
                }
                else
                {
                    switch ( m_eYCbCrSubSamplingVert )
                    {
                    case YCCSST_ONE_TO_ONE:
                        rqSegment.UncompressedByteCount =
                            rqSegment.Length/1*iChunkySegmentRowStride;
                        break;
                    case YCCSST_ONE_TO_TWO:
                        rqSegment.UncompressedByteCount =
                            rqSegment.Length/2*(iChunkySegmentRowStride + iLumaOnlyRowStride);
                        break;
                    case YCCSST_ONE_TO_FOUR:
                        rqSegment.UncompressedByteCount =
                            rqSegment.Length/4*iChunkySegmentRowStride + rqSegment.Length*3/4*iLumaOnlyRowStride;
                        break;
                    }
                }
            }
            else  // m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                iPlanarSegmentRowSizeInBits = rqSegment.Width*m_qBitsPerSample[i/m_iSegmentsPerPlane];
                iPlanarSegmentRowStride =
                    ( (iPlanarSegmentRowSizeInBits & 7) == 0 ? iPlanarSegmentRowSizeInBits/8 :
                    iPlanarSegmentRowSizeInBits/8 + 1 );
                rqSegment.UncompressedByteCount = rqSegment.Length*iPlanarSegmentRowStride;
            }
            rqSegment.SubimageRectOuter.X = 0;
            rqSegment.SubimageRectOuter.Y = iRowSum;
            rqSegment.SubimageRectOuter.Width = rqSegment.Width;
            rqSegment.SubimageRectOuter.Height = rqSegment.Length;
            rqSegment.SubimageRectInnerIsMeaning = false;

            iRowSum += m_iRowsPerStrip;
            if ( m_ePlanarConfiguration == PCT_PLANAR_FORMAT && iRowSum >= (int)m_ImageLength )
                iRowSum = 0;
        }
    }
    else
    {
        // tiles are used
        m_qSegments.SetQuantity(m_iTilesPerImage);
        int iPlaneQuantity = ( m_ePlanarConfiguration != PCT_PLANAR_FORMAT ? 1 : m_SamplesPerPixel );
        int iColSum, iRowSum, iTile = 0;
        for (int i0 = 0; i0 < iPlaneQuantity; i0++)
        {
            iRowSum = 0;
            for (int i1 = 0; i1 < m_iTilesDown; i1++)
            {
                iColSum = 0;
                for (int i2 = 0; i2 < m_iTilesAcross; i2++)
                {
                    SegmentRecord& rqSegment = m_qSegments[iTile];

                    rqSegment.Offset = m_qTileOffsets[iTile];
                    rqSegment.ByteCount = m_qTileByteCounts[iTile];
                    rqSegment.Width = m_TileWidth;
                    rqSegment.Length = m_TileLength;
                    if ( m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
                    {
                        if ( !m_bImageIsYCbCrNonJpeg )
                        {
                            rqSegment.UncompressedByteCount = rqSegment.Length*iChunkySegmentRowStride;
                        }
                        else
                        {
                            switch ( m_eYCbCrSubSamplingVert )
                            {
                            case YCCSST_ONE_TO_ONE:
                                rqSegment.UncompressedByteCount =
                                    rqSegment.Length/1*iChunkySegmentRowStride;
                                break;
                            case YCCSST_ONE_TO_TWO:
                                rqSegment.UncompressedByteCount =
                                    rqSegment.Length/2*(iChunkySegmentRowStride + iLumaOnlyRowStride);
                                break;
                            case YCCSST_ONE_TO_FOUR:
                                rqSegment.UncompressedByteCount =
                                    rqSegment.Length/4*iChunkySegmentRowStride +
                                    rqSegment.Length*3/4*iLumaOnlyRowStride;
                                break;
                            }
                        }
                    }
                    else  // m_ePlanarConfiguration = PCT_PLANAR_FORMAT
                    {
                        iPlanarSegmentRowSizeInBits = rqSegment.Width*m_qBitsPerSample[iTile/m_iSegmentsPerPlane];
                        iPlanarSegmentRowStride =
                            ( (iPlanarSegmentRowSizeInBits & 7) == 0 ? iPlanarSegmentRowSizeInBits/8 :
                            iPlanarSegmentRowSizeInBits/8 + 1 );
                        rqSegment.UncompressedByteCount = rqSegment.Length*iPlanarSegmentRowStride;
                    }
                    rqSegment.SubimageRectInner.X = 0;
                    rqSegment.SubimageRectInner.Y = 0;
                    rqSegment.SubimageRectInner.Width = ( iColSum + m_TileWidth <= m_ImageWidth ?
                        m_TileWidth : m_ImageWidth - iColSum );
                    rqSegment.SubimageRectInner.Height = ( iRowSum + m_TileLength <= m_ImageLength ?
                        m_TileLength : m_ImageLength - iRowSum );
                    rqSegment.SubimageRectOuter.X = iColSum;
                    rqSegment.SubimageRectOuter.Y = iRowSum;
                    rqSegment.SubimageRectOuter.Width = rqSegment.SubimageRectInner.Width;
                    rqSegment.SubimageRectOuter.Height = rqSegment.SubimageRectInner.Height;
                    rqSegment.SubimageRectInnerIsMeaning =
                        ( rqSegment.SubimageRectInner.Width != rqSegment.Width
                     ||   rqSegment.SubimageRectInner.Height != rqSegment.Length );

                    iColSum += m_TileWidth;
                    iTile++;
                }

                iRowSum += m_TileLength;
            }
        }
    }

    m_bTypicalCaseForPixelSamples = ( m_bBitsPerSamplesAreTheSame && m_iBitsPerEachSample == 8
                                 &&   m_bPixelSamplesAllAreUI && !m_bSMinSampleValueFieldIsPresent
                                 &&   !m_bSMaxSampleValueFieldIsPresent );

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::AreBitsPerSampleInProperRanges () const
{
    SampleFormatType eSFT;
    Short SampleBits;
    for (int i = 0; i < m_SamplesPerPixel; i++)
    {
        eSFT = m_qSampleFormat[i];
        SampleBits = m_qBitsPerSample[i];

        if ( ((eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_SIGNED_INTEGER || eSFT == SFT_UNDEFINED)
        &&   (SampleBits == 0 || SampleBits > 24))
        ||   (eSFT == SFT_FLOATING_POINT && SampleBits != 32) )
        {
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsMinSampleAllowed () const
{
    return m_bPixelSamplesAllAreUI;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsMaxSampleAllowed () const
{
    return m_bPixelSamplesAllAreUI;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsTransferFunctionAllowed () const
{
    if ( m_bImageHasColorimetricInterpretation && m_bBitsPerSamplesAreTheSame && m_bPixelSamplesAllAreUI )
    {
        return ( m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO
            ||   m_ePhotometricInterpretation == PIT_BLACK_IS_ZERO
            ||   m_ePhotometricInterpretation == PIT_RGB
            ||   m_ePhotometricInterpretation == PIT_PALETTED
            ||   m_ePhotometricInterpretation == PIT_YCC );
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsDotRangeAllowed () const
{
    return m_bPixelSamplesAllAreUI;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsTransferRangeAllowed () const
{
    if ( m_bImageHasColorimetricInterpretation && m_bPixelSamplesAllAreUI )
    {
        return ( m_ePhotometricInterpretation == PIT_RGB
            ||   m_ePhotometricInterpretation == PIT_YCC );
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ImageFileDirectory::IsReferenceBlackWhiteAllowed () const
{
    if ( m_bImageHasColorimetricInterpretation && m_bPixelSamplesAllAreUI )
    {
        return ( m_ePhotometricInterpretation == PIT_RGB
            ||   m_ePhotometricInterpretation == PIT_YCC );
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
FormatTiff::FormatTiff (SegmentOrganizationType eSegmentOrganization)
    :
    m_eByteOrder(BOT_LITTLE_ENDIAN),
    m_eSegmentOrganization(eSegmentOrganization),
    m_bHasExifMetadata(false)
{
}
//------------------------------------------------------------------------------------------------------------------
int FormatTiff::InsertImageFileDirectory (const ImageFileDirectoryPtr& rspqIFD)
{
    // rely on the visibility mask if it is present
    if ( rspqIFD->m_spqImageSaveTo->HasVisibilityMask() )
    {
        RasterImage::Palette qBWPalette(2);
        qBWPalette[0] = PaletteColor(0,0,0);
        qBWPalette[1] = PaletteColor(255,255,255);
        RasterImagePtr spqVisibilityMaskImage = new RasterImage(RasterImage::IT_PALETTED,
            rspqIFD->m_spqImageSaveTo->GetWidth(),rspqIFD->m_spqImageSaveTo->GetHeight(),qBWPalette);
        int iVMValue;
        for (int iX = 0; iX < spqVisibilityMaskImage->GetWidth(); iX++)
        {
            for (int iY = 0; iY < spqVisibilityMaskImage->GetHeight(); iY++)
            {
                iVMValue = rspqIFD->m_spqImageSaveTo->GetVisibilityMask()[
                    iY*spqVisibilityMaskImage->GetWidth()+iX];
                spqVisibilityMaskImage->SetPixelIndex(( iVMValue == 0 ? 1 : 0 ),iX,iY);
            }
        }

        StillArray<Short> qBPS(1);
        qBPS[0] = 1;
        m_spqTransparencyMaskIFD = new ImageFileDirectory(spqVisibilityMaskImage,qBPS,true);
        m_spqTransparencyMaskIFD->SetCompression(CT_RLE);  // as recommended by the specification
        m_spqTransparencyMaskIFD->UnsetResolutionInfo();
        PrepareIFDForSaving(m_spqTransparencyMaskIFD,true);
    }

    PrepareIFDForSaving(rspqIFD,false);
    return m_qImageFileDirectories.Push(rspqIFD);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::SaveFile (const char* acFilename)
{
#ifdef _DEBUG
    // there must be no multiple images with a visibility mask in a single TIFF file
    int iImagesWithVM = 0;
    for (int i = 0; i < m_qImageFileDirectories.GetQuantity(); i++)
    {
        if ( m_qImageFileDirectories[i]->m_spqImageSaveTo->HasVisibilityMask() )
            iImagesWithVM++;
    }
    assert( !(iImagesWithVM != 0 && m_qImageFileDirectories.GetQuantity() != 1) );
#endif

    m_bLittleEndian = ( m_eByteOrder == BOT_LITTLE_ENDIAN );
    Stream qSaveStream(acFilename,m_bLittleEndian);

    // header
    unsigned char ucBO = ( m_bLittleEndian ? 73 : 77 );
    qSaveStream.Write(ucBO);
    qSaveStream.Write(ucBO);
    unsigned short us42 = 42;
    qSaveStream.Write(us42);
    int iIFDOffset = 8;
    qSaveStream.Write(iIFDOffset);

    StillArray<StillArray<int> > qOffsetsBytePosArrayArray(m_qImageFileDirectories.GetQuantity());
    StillArray<StillArray<int> > qByteCountsBytePosArrayArray(m_qImageFileDirectories.GetQuantity());
    StillArray<int> qTrncMaskOffsetsBytePosArray, qTrncMaskByteCountsBytePosArray;
    if ( m_spqTransparencyMaskIFD )
    {
        qTrncMaskOffsetsBytePosArray.SetQuantity(1);
        qTrncMaskByteCountsBytePosArray.SetQuantity(1);
    }

    bool bTrncMaskMode = false;

    // write image file directory fields and their values (either joined or separated)
    int iSeparatedValuesBytePos;
    unsigned short usFieldCount;
    ImageFileDirectory* pqIFD;
    for (int i = 0; i < m_qImageFileDirectories.GetQuantity(); i++)
    {
        pqIFD = m_qImageFileDirectories[i];
WritingIFDFields:
        const ImageFileDirectory& rqIFD = *pqIFD;

        // count IFD fields which must be saved
        usFieldCount = 0;
        if ( rqIFD.m_bNewSubfileTypeFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bImageWidthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bImageLengthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bBitsPerSampleFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bCompressionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPhotometricInterpretationFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bThreshholdingFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bCellWidthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bCellLengthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bDocumentNameFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bImageDescriptionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bMakeFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bModelFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bStripOffsetsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bOrientationFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bSamplesPerPixelFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bRowsPerStripFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bStripByteCountsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bMinSampleValueFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bMaxSampleValueFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bXResolutionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bYResolutionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPlanarConfigurationFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPageNameFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bXPositionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bYPositionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bGrayResponseUnitFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bGrayResponseCurveFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bT4OptionsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bResolutionUnitFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPageNumberFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTransferFunctionFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bSoftwareFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bDateTimeFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bArtistFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bHostComputerFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPredictorFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bWhitePointFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bPrimaryChromaticitiesFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bColorMapFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bHalftoneHintsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTileWidthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTileLengthFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTileOffsetsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTileByteCountsFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bDotRangeFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTargetPrinterFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bExtraSamplesFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bSampleFormatFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bSMinSampleValueFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bSMaxSampleValueFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bTransferRangeFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bJPEGTablesFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bReferenceBlackWhiteFieldIsPresent )
            usFieldCount++;
        if ( rqIFD.m_bCopyrightFieldIsPresent )
            usFieldCount++;
        if ( m_bHasExifMetadata && i == 0 )
            usFieldCount++;

        iSeparatedValuesBytePos = qSaveStream.GetBytePosition() + 2 + usFieldCount*12 + 4;
        qSaveStream.SetByteQuantity(iSeparatedValuesBytePos);

        // flow out 12-byte-sized IFD fields
        qSaveStream.Write(usFieldCount);
        if ( !bTrncMaskMode )
        {
            SaveIFDFields(rqIFD,iSeparatedValuesBytePos,qSaveStream,qOffsetsBytePosArrayArray[i],
                qByteCountsBytePosArrayArray[i]);
        }
        else
        {
            SaveIFDFields(rqIFD,iSeparatedValuesBytePos,qSaveStream,qTrncMaskOffsetsBytePosArray,
                qTrncMaskByteCountsBytePosArray);
        }

        if ( m_bHasExifMetadata && i == 0 )
        {
            // write Exif data in the first IFD
#ifdef _DEBUG
            switch ( m_eByteOrder )
            {
            case BOT_LITTLE_ENDIAN:
                assert( m_spqExifMetadata->GetByteOrder() == FormatExif::BOT_LITTLE_ENDIAN );
                break;
            case BOT_BIG_ENDIAN:
                assert( m_spqExifMetadata->GetByteOrder() == FormatExif::BOT_BIG_ENDIAN );
                break;
            }
#endif

            // generate data
            StillArray<char> qData;
            m_spqExifMetadata->SaveData(qData,FormatExif::EDDT_TIFF,iSeparatedValuesBytePos);

            // write Exif IFD pointer field
            unsigned short usTag = 34665;
            unsigned short usType = 4;
            int iCount = 1;
            qSaveStream.Write(usTag);
            qSaveStream.Write(usType);
            qSaveStream.Write(iCount);
            qSaveStream.Write(iSeparatedValuesBytePos);

            // write Exif data
            qSaveStream.PushPositionState();
            qSaveStream.SetBytePosition(iSeparatedValuesBytePos);
            qSaveStream.Write(qData.GetQuantity(),qData);
            iSeparatedValuesBytePos += qData.GetQuantity();
            qSaveStream.PopPositionState();
        }

        if ( !bTrncMaskMode )
        {
            if ( i != m_qImageFileDirectories.GetQuantity() - 1 )
                iIFDOffset = iSeparatedValuesBytePos;
            else
                iIFDOffset = ( !m_spqTransparencyMaskIFD ? 0 : iSeparatedValuesBytePos );
        }
        else
        {
            iIFDOffset = 0;
        }
        qSaveStream.Write(iIFDOffset);
        qSaveStream.SetBytePosition(iSeparatedValuesBytePos);

        if ( bTrncMaskMode )
            break;
    }
    if ( m_spqTransparencyMaskIFD && !bTrncMaskMode )
    {
        pqIFD = m_spqTransparencyMaskIFD;
        bTrncMaskMode = true;
        goto WritingIFDFields;
    }
    bTrncMaskMode = false;

    // for a progress bar
    ProgressProbe qProgress(this);

    // pixel data
    int iBytePos, iBitRatio, iSegmentRowSizeInBits, iSegmentRowSize, iValue, iValue0, iValue1, iValue2, iValue3,
        iIndex;
    unsigned short usValue, usValue0, usValue1, usValue2, usValue3;
    char* acRow;
    char* pcComp;
    unsigned char* pucRow;
    unsigned char* aucRow0;
    unsigned char* aucRow1;
    unsigned char* pucComp;
    unsigned char* pucComp0;
    unsigned char* pucComp1;
    Long LongValue;
    float fIntensity;
    Link<float,float> qIntensityWithAlpha;
    ColorRgb qColorRgb;
    ColorRgba qColorRgba;
    ColorCmyk qColorCmyk;
    ColorLab qColorLab;
    ColorLaba qColorLaba;
    float afLab[3];
    float afLaba[4];
    float fDonePerIFD = ( !m_spqTransparencyMaskIFD ? 1.0f/m_qImageFileDirectories.GetQuantity() :
        1.0f/(m_qImageFileDirectories.GetQuantity() + 1) );
    float fDonePerSegment, fHalfDonePerSegment;
    for (int i0 = 0; i0 < m_qImageFileDirectories.GetQuantity(); i0++)
    {
        pqIFD = m_qImageFileDirectories[i0];
WritingPixelData:
        ImageFileDirectory& rqIFD = *pqIFD;

        fDonePerSegment = fDonePerIFD/rqIFD.m_qSegments.GetQuantity();
        fHalfDonePerSegment = fDonePerSegment/2;

        CompressionFaxPtr spqFax;
        if ( rqIFD.m_eCompression == CT_MODIFIED_HUFFMAN )
        {
            spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_1D_TIFF,true);
        }
        else if ( rqIFD.m_eCompression == CT_GROUP_3 )
        {
            if ( !rqIFD.m_bT42d )
            {
                spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_1D,true);
            }
            else
            {
                // for "K factor" the default CompressionFax's value will be taken
                spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_2D,true);
            }
        }
        else if ( rqIFD.m_eCompression == CT_GROUP_4 )
        {
            spqFax = new CompressionFax(CompressionFax::ET_GROUP_4,true);
        }

        for (int i1 = 0; i1 < rqIFD.m_qSegments.GetQuantity(); i1++)
        {
            const SegmentRecord& rqSegment = rqIFD.m_qSegments[i1];

            StillArray<char> qDecompressedData;
            StillArray<char> qCompressedData;

            if ( rqIFD.m_eCompression == CT_JPEG_TNT )
            {
                RasterImagePtr spqSegmentImage = rqIFD.m_spqImageSaveTo->GetSubimage(
                    rqSegment.SubimageRectOuter.X,rqSegment.SubimageRectOuter.Y,rqSegment.Width,rqSegment.Length);

                if ( rqIFD.m_eColorModel == CMT_CMYK )
                {
                    // invert CMYK components
                    pucRow = (unsigned char*)spqSegmentImage->GetImage();
                    pucComp = pucRow;
                    iSegmentRowSize = spqSegmentImage->GetWidth()*4;
                    for (int iY = 0; iY < spqSegmentImage->GetHeight(); iY++)
                    {
                        for (int iX = 0; iX < iSegmentRowSize; iX++)
                        {
                            *pucComp = 255 - *pucComp;
                            pucComp++;
                        }

                        pucRow += spqSegmentImage->GetRowStride();
                        pucComp = pucRow;
                    }
                }
                else if ( rqIFD.m_eColorModel == CMT_LAB )
                {
                    // tune L*a*b* components
                    RasterImagePtr spqAltSegmentImage = new RasterImage(RasterImage::IT_RGB_8_8_8,
                        spqSegmentImage->GetWidth(),spqSegmentImage->GetHeight());
                    aucRow0 = (unsigned char*)spqSegmentImage->GetImage();
                    pucComp0 = aucRow0;
                    aucRow1 = (unsigned char*)spqAltSegmentImage->GetImage();
                    pucComp1 = aucRow1;
                    for (int iY = 0; iY < spqSegmentImage->GetHeight(); iY++)
                    {
                        for (int iX = 0; iX < spqSegmentImage->GetWidth(); iX++)
                        {
                            *pucComp1++ = pucComp0[0];
                            *pucComp1++ = pucComp0[1] - 128;
                            *pucComp1++ = pucComp0[2] - 128;

                            pucComp0 += 3;
                        }

                        aucRow0 += spqSegmentImage->GetRowStride();
                        pucComp0 = aucRow0;
                        aucRow1 += spqAltSegmentImage->GetRowStride();
                        pucComp1 = aucRow1;
                    }
                    spqSegmentImage = spqAltSegmentImage;
                }

                FormatJpeg qJpeg(spqSegmentImage);
                if ( rqIFD.m_eColorModel == CMT_CMYK )
                    qJpeg.SetColorModel(FormatJpeg::CMT_CMYK);
                else if ( rqIFD.m_eColorModel == CMT_LAB )
                    qJpeg.SetColorModel(FormatJpeg::CMT_RGB);
                qJpeg.SetQuality(rqIFD.m_fJpegQuality);
                qJpeg.UnsetAdobeMarker();
                qJpeg.UnsetJfifMarker();
                qJpeg.RedirectProgressTo(this,qProgress,fDonePerSegment);
                qJpeg.SaveImage(qCompressedData);

                // that's all for this segment
                goto EndOfCompression;
            }

            qProgress.SetAdvance(fHalfDonePerSegment/rqSegment.Length);

            qDecompressedData.SetQuantity(rqSegment.UncompressedByteCount);
            iBytePos = 0;
            iBitRatio = 0;
            if ( rqIFD.m_eColorModel == CMT_BILEVEL || rqIFD.m_bIsTransparencyMask )
            {
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        pcComp = rqIFD.m_spqImageSaveTo->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X + iX;
                        iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,1,
                            ( *pcComp == 0 ? 1 : 0 ));
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE )
            {
                if ( !(rqIFD.m_spqImageSaveTo->GetType() == RasterImage::IT_GRAYSCALE_8
                &&   rqIFD.m_bTypicalCaseForPixelSamples) )
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            fIntensity = rqIFD.m_spqImageSaveTo->GetPixelIntensity(
                                rqSegment.SubimageRectOuter.X+iX,
                                rqSegment.SubimageRectOuter.Y+iY);
                            if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() ==
                                 RasterImage::PCT_UNSIGNED_INTEGER )
                            {
                                iValue = Mathf::RoundToInt(fIntensity*rqIFD.m_qSampleUITopValues[0]);
                                if ( rqIFD.m_qBitsPerSample[0] != 16 )
                                {
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[0],iValue);
                                }
                                else
                                {
                                    usValue = iValue;
                                    if ( m_bLittleEndian )
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue);
                                    else
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue);
                                }
                            }
                            else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = PCT_FLOATING_POINT
                            {
                                if ( m_bLittleEndian )
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,1,&fIntensity);
                                else
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,1,&fIntensity);
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        acRow = rqIFD.m_spqImageSaveTo->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X;
                        MemCpy(qDecompressedData+iBytePos,acRow,rqSegment.Width);
                        iBytePos += rqSegment.Width;

                        qProgress.MakeAdvance();
                    }
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( !(rqIFD.m_spqImageSaveTo->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8
                &&   rqIFD.m_bTypicalCaseForPixelSamples) )
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qIntensityWithAlpha = rqIFD.m_spqImageSaveTo->GetPixelIntensityWithAlpha(
                                rqSegment.SubimageRectOuter.X+iX,
                                rqSegment.SubimageRectOuter.Y+iY);

                            // assosiate alpha
                            qIntensityWithAlpha.First *= qIntensityWithAlpha.Second;

                            if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() ==
                                 RasterImage::PCT_UNSIGNED_INTEGER )
                            {
                                iValue0 = Mathf::RoundToInt(
                                    qIntensityWithAlpha.First*rqIFD.m_qSampleUITopValues[0]);
                                iValue1 = Mathf::RoundToInt(
                                    qIntensityWithAlpha.Second*rqIFD.m_qSampleUITopValues[1]);
                                if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                                {
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[0],iValue0);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[1],iValue1);
                                }
                                else
                                {
                                    usValue0 = iValue0;
                                    usValue1 = iValue1;
                                    if ( m_bLittleEndian )
                                    {
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                    }
                                    else
                                    {
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                    }
                                }
                            }
                            else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = PCT_FLOATING_POINT
                            {
                                if ( m_bLittleEndian )
                                {
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,1,
                                        &qIntensityWithAlpha.First);
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,1,
                                        &qIntensityWithAlpha.Second);
                                }
                                else
                                {
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,1,
                                        &qIntensityWithAlpha.First);
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,1,
                                        &qIntensityWithAlpha.Second);
                                }
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        pucComp = (unsigned char*)(rqIFD.m_spqImageSaveTo->GetRow(
                            rqSegment.SubimageRectOuter.Y+iY) + rqSegment.SubimageRectOuter.X*2);
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qDecompressedData[iBytePos++] =
                                Mathf::RoundToInt(((float)(pucComp[0]*pucComp[1]))/255);
                            qDecompressedData[iBytePos++] = pucComp[1];

                            pucComp += 2;
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_RGB )
            {
                if ( !(rqIFD.m_spqImageSaveTo->GetType() == RasterImage::IT_RGB_8_8_8
                &&   rqIFD.m_bTypicalCaseForPixelSamples) )
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qColorRgb = rqIFD.m_spqImageSaveTo->GetPixelRgb(
                                rqSegment.SubimageRectOuter.X+iX,
                                rqSegment.SubimageRectOuter.Y+iY);
                            if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() ==
                                 RasterImage::PCT_UNSIGNED_INTEGER )
                            {
                                iValue0 = Mathf::RoundToInt(qColorRgb.R()*rqIFD.m_qSampleUITopValues[0]);
                                iValue1 = Mathf::RoundToInt(qColorRgb.G()*rqIFD.m_qSampleUITopValues[1]);
                                iValue2 = Mathf::RoundToInt(qColorRgb.B()*rqIFD.m_qSampleUITopValues[2]);
                                if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                                {
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[0],iValue0);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[1],iValue1);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[2],iValue2);
                                }
                                else
                                {
                                    usValue0 = iValue0;
                                    usValue1 = iValue1;
                                    usValue2 = iValue2;
                                    if ( m_bLittleEndian )
                                    {
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue2);
                                    }
                                    else
                                    {
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue2);
                                    }
                                }
                            }
                            else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = PCT_FLOATING_POINT
                            {
                                if ( m_bLittleEndian )
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,3,qColorRgb);
                                else
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,3,qColorRgb);
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else
                {
                    iSegmentRowSize = rqSegment.Width*3;
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        acRow = rqIFD.m_spqImageSaveTo->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X*3;
                        MemCpy(qDecompressedData+iBytePos,acRow,iSegmentRowSize);
                        iBytePos += iSegmentRowSize;

                        qProgress.MakeAdvance();
                    }
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_RGBA )
            {
                if ( !(rqIFD.m_spqImageSaveTo->GetType() == RasterImage::IT_RGBA_8_8_8_8
                &&   rqIFD.m_bTypicalCaseForPixelSamples) )
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qColorRgba = rqIFD.m_spqImageSaveTo->GetPixelRgba(
                                rqSegment.SubimageRectOuter.X+iX,
                                rqSegment.SubimageRectOuter.Y+iY);

                            // assosiate alpha
                            qColorRgba.R() *= qColorRgba.A();
                            qColorRgba.G() *= qColorRgba.A();
                            qColorRgba.B() *= qColorRgba.A();

                            if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() ==
                                 RasterImage::PCT_UNSIGNED_INTEGER )
                            {
                                iValue0 = Mathf::RoundToInt(qColorRgba.R()*rqIFD.m_qSampleUITopValues[0]);
                                iValue1 = Mathf::RoundToInt(qColorRgba.G()*rqIFD.m_qSampleUITopValues[1]);
                                iValue2 = Mathf::RoundToInt(qColorRgba.B()*rqIFD.m_qSampleUITopValues[2]);
                                iValue3 = Mathf::RoundToInt(qColorRgba.A()*rqIFD.m_qSampleUITopValues[3]);
                                if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                                {
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[0],iValue0);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[1],iValue1);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[2],iValue2);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[3],iValue3);
                                }
                                else
                                {
                                    usValue0 = iValue0;
                                    usValue1 = iValue1;
                                    usValue2 = iValue2;
                                    usValue3 = iValue3;
                                    if ( m_bLittleEndian )
                                    {
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue2);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue3);
                                    }
                                    else
                                    {
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue2);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue3);
                                    }
                                }
                            }
                            else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = PCT_FLOATING_POINT
                            {
                                if ( m_bLittleEndian )
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,4,qColorRgba);
                                else
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,4,qColorRgba);
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        pucComp = (unsigned char*)(rqIFD.m_spqImageSaveTo->GetRow(
                            rqSegment.SubimageRectOuter.Y+iY) + rqSegment.SubimageRectOuter.X*4);
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qDecompressedData[iBytePos++] =
                                Mathf::RoundToInt(((float)(pucComp[0]*pucComp[3]))/255);
                            qDecompressedData[iBytePos++] =
                                Mathf::RoundToInt(((float)(pucComp[1]*pucComp[3]))/255);
                            qDecompressedData[iBytePos++] =
                                Mathf::RoundToInt(((float)(pucComp[2]*pucComp[3]))/255);
                            qDecompressedData[iBytePos++] = pucComp[3];

                            pucComp += 4;
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_PALETTED )
            {
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        iIndex = rqIFD.m_spqImageSaveTo->GetPixelIndex(
                            rqSegment.SubimageRectOuter.X+iX,
                            rqSegment.SubimageRectOuter.Y+iY);
                        iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                            rqIFD.m_qBitsPerSample[0],iIndex);
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_CMYK )
            {
                if ( !(rqIFD.m_spqImageSaveTo->GetType() == RasterImage::IT_CMYK_8_8_8_8
                &&   rqIFD.m_bTypicalCaseForPixelSamples) )
                {
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            qColorCmyk = rqIFD.m_spqImageSaveTo->GetPixelCmyk(
                                rqSegment.SubimageRectOuter.X+iX,
                                rqSegment.SubimageRectOuter.Y+iY);
                            if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() ==
                                 RasterImage::PCT_UNSIGNED_INTEGER )
                            {
                                iValue0 = Mathf::RoundToInt(qColorCmyk.C()*rqIFD.m_qSampleUITopValues[0]);
                                iValue1 = Mathf::RoundToInt(qColorCmyk.M()*rqIFD.m_qSampleUITopValues[1]);
                                iValue2 = Mathf::RoundToInt(qColorCmyk.Y()*rqIFD.m_qSampleUITopValues[2]);
                                iValue3 = Mathf::RoundToInt(qColorCmyk.K()*rqIFD.m_qSampleUITopValues[3]);
                                if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                                {
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[0],iValue0);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[1],iValue1);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[2],iValue2);
                                    iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                        rqIFD.m_qBitsPerSample[3],iValue3);
                                }
                                else
                                {
                                    usValue0 = iValue0;
                                    usValue1 = iValue1;
                                    usValue2 = iValue2;
                                    usValue3 = iValue3;
                                    if ( m_bLittleEndian )
                                    {
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue2);
                                        iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue3);
                                    }
                                    else
                                    {
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue2);
                                        iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue3);
                                    }
                                }
                            }
                            else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = PCT_FLOATING_POINT
                            {
                                if ( m_bLittleEndian )
                                    iBytePos += Storage::Write4le(qDecompressedData+iBytePos,4,qColorCmyk);
                                else
                                    iBytePos += Storage::Write4be(qDecompressedData+iBytePos,4,qColorCmyk);
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else
                {
                    iSegmentRowSize = rqSegment.Width*4;
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        acRow = rqIFD.m_spqImageSaveTo->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X*4;
                        MemCpy(qDecompressedData+iBytePos,acRow,iSegmentRowSize);
                        iBytePos += iSegmentRowSize;

                        qProgress.MakeAdvance();
                    }
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_LAB )
            {
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        qColorLab = rqIFD.m_spqImageSaveTo->GetPixelLab(
                            rqSegment.SubimageRectOuter.X+iX,
                            rqSegment.SubimageRectOuter.Y+iY);
                        afLab[0] = qColorLab.L()/100;
                        if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_UNSIGNED_INTEGER )
                        {
                            afLab[1] = qColorLab.A()/255;
                            afLab[2] = qColorLab.B()/255;
                            for (int i2 = 1; i2 < 3; i2++)
                            {
                                if ( afLab[i2] < -0.5f )
                                    afLab[i2] = -0.5f;
                                else if ( afLab[i2] > 0.5f )
                                    afLab[i2] = 0.5f;
                            }

                            iValue0 = Mathf::RoundToInt(afLab[0]*rqIFD.m_qSampleUITopValues[0]);
                            iValue1 =  Mathf::RoundToInt(afLab[1]*rqIFD.m_qSampleUITopValues[1]);
                            iValue2 =  Mathf::RoundToInt(afLab[2]*rqIFD.m_qSampleUITopValues[2]);
                            if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                            {
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[0],iValue0);
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[1],iValue1);
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[2],iValue2);
                            }
                            else
                            {
                                usValue0 = iValue0;
                                usValue1 = iValue1;
                                usValue2 = iValue2;
                                if ( m_bLittleEndian )
                                {
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue2);
                                }
                                else
                                {
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue2);
                                }
                            }
                        }
                        else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = RasterImage::PCT_FLOATING_POINT
                        {
                            afLab[1] = (qColorLab.A() + 128)/255;
                            afLab[2] = (qColorLab.B() + 128)/255;
                            if ( m_bLittleEndian )
                                iBytePos += Storage::Write4le(qDecompressedData+iBytePos,3,afLab);
                            else
                                iBytePos += Storage::Write4be(qDecompressedData+iBytePos,3,afLab);
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_eColorModel = CMT_LAB_WITH_ALPHA
            {
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        qColorLaba = rqIFD.m_spqImageSaveTo->GetPixelLaba(
                            rqSegment.SubimageRectOuter.X+iX,
                            rqSegment.SubimageRectOuter.Y+iY);
                        afLaba[0] = qColorLaba.L()/100;
                        afLaba[1] = (qColorLaba.A() + 128)/255;
                        afLaba[2] = (qColorLaba.B() + 128)/255;
                        afLaba[3] = qColorLaba.Alpha();

                        // assosiate alpha
                        afLaba[0] *= afLaba[3];
                        afLaba[1] *= afLaba[3];
                        afLaba[2] *= afLaba[3];

                        if ( rqIFD.m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_UNSIGNED_INTEGER )
                        {
                            for (int i2 = 1; i2 < 3; i2++)
                            {
                                afLaba[i2] -= ((float)128)/255;

                                if ( afLaba[i2] < -0.5f )
                                    afLaba[i2] = -0.5f;
                                else if ( afLaba[i2] > 0.5f )
                                    afLaba[i2] = 0.5f;
                            }

                            iValue0 = Mathf::RoundToInt(afLaba[0]*rqIFD.m_qSampleUITopValues[0]);
                            iValue1 =  Mathf::RoundToInt(afLaba[1]*rqIFD.m_qSampleUITopValues[1]);
                            iValue2 =  Mathf::RoundToInt(afLaba[2]*rqIFD.m_qSampleUITopValues[2]);
                            iValue3 = Mathf::RoundToInt(afLaba[3]*rqIFD.m_qSampleUITopValues[3]);
                            if ( !(rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 16) )
                            {
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[0],iValue0);
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[1],iValue1);
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[2],iValue2);
                                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,
                                    rqIFD.m_qBitsPerSample[3],iValue3);
                            }
                            else
                            {
                                usValue0 = iValue0;
                                usValue1 = iValue1;
                                usValue2 = iValue2;
                                usValue3 = iValue3;
                                if ( m_bLittleEndian )
                                {
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue0);
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue1);
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue2);
                                    iBytePos += Storage::Write2le(qDecompressedData+iBytePos,1,&usValue3);
                                }
                                else
                                {
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue0);
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue1);
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue2);
                                    iBytePos += Storage::Write2be(qDecompressedData+iBytePos,1,&usValue3);
                                }
                            }
                        }
                        else  // rqIFD.m_spqImageSaveTo->GetPixelComponentType() = RasterImage::PCT_FLOATING_POINT
                        {
                            if ( m_bLittleEndian )
                                iBytePos += Storage::Write4le(qDecompressedData+iBytePos,4,afLaba);
                            else
                                iBytePos += Storage::Write4be(qDecompressedData+iBytePos,4,afLaba);
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }

            if ( rqIFD.m_ePredictor == PT_HORIZONTAL_DIFFERENCING )
            {
                // filter
                FilterSegmentImage(rqSegment,qDecompressedData,rqIFD);
            }

            // pack
            if ( rqIFD.m_eCompression == CT_NONE )
            {
                qCompressedData = qDecompressedData;
                qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
            }
            else if ( rqIFD.m_eCompression == CT_MODIFIED_HUFFMAN
                 ||   rqIFD.m_eCompression == CT_GROUP_3
                 ||   rqIFD.m_eCompression == CT_GROUP_4 )
            {
                spqFax->SetImageDimensions(rqSegment.Width,rqSegment.Length);
                spqFax->RedirectProgressTo(this,qProgress,fHalfDonePerSegment);
                spqFax->Compress(qDecompressedData,qCompressedData);
            }
            else if ( rqIFD.m_eCompression == CT_LZW )
            {
                CompressionLzw qLzw(CompressionLzw::ET_TIFF);
                qLzw.RedirectProgressTo(this,qProgress,fHalfDonePerSegment);
                qLzw.Compress(qDecompressedData,qDecompressedData.GetQuantity(),qCompressedData);
            }
            else if ( rqIFD.m_eCompression == CT_ZLIB )
            {
                CompressionZLib qZLib;
                if ( ms_bChooseBestFromManifoldZLibStrategies )
                    qZLib.SetCompressionStrategy(CompressionZLib::CST_BEST_AMONG_4);
                qZLib.Compress(qDecompressedData,qDecompressedData.GetQuantity(),qCompressedData);
                qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
            }
            else  // rqIFD.m_eCompression = CT_RLE
            {
                // to allow some particular decoders to properly read images, encode each row separately
                iSegmentRowSizeInBits = rqSegment.Width*rqIFD.m_iBitsPerPixel;
                iSegmentRowSize =
                    ( (iSegmentRowSizeInBits & 7) == 0 ? iSegmentRowSizeInBits/8 : iSegmentRowSizeInBits/8 + 1 );
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    StillArray<char> qSubCompressedData;
                    CompressionPackBits::Compress(qDecompressedData+iY*iSegmentRowSize,iSegmentRowSize,
                        qSubCompressedData);
                    qCompressedData.Append(qSubCompressedData);
                }
                qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
            }
EndOfCompression:

            // got the segment's attributes, so go back and write them
            LongValue = qSaveStream.GetBytePosition();
            qSaveStream.PushPositionState();
            if ( !bTrncMaskMode )
                qSaveStream.SetBytePosition(qOffsetsBytePosArrayArray[i0][i1]);
            else
                qSaveStream.SetBytePosition(qTrncMaskOffsetsBytePosArray[i1]);
            qSaveStream.Write(LongValue);
            LongValue = qCompressedData.GetQuantity();
            if ( !bTrncMaskMode )
                qSaveStream.SetBytePosition(qByteCountsBytePosArrayArray[i0][i1]);
            else
                qSaveStream.SetBytePosition(qTrncMaskByteCountsBytePosArray[i1]);
            qSaveStream.Write(LongValue);
            qSaveStream.PopPositionState();

            qSaveStream.Write(qCompressedData.GetQuantity(),qCompressedData);
        }

        if ( bTrncMaskMode )
            break;
    }
    if ( m_spqTransparencyMaskIFD && !bTrncMaskMode )
    {
        pqIFD = m_spqTransparencyMaskIFD;
        bTrncMaskMode = true;
        goto WritingPixelData;
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatTiff::FormatTiff (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_bHasExifMetadata(false)
{
    bool bSuccess;

    int iDataSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,acFilename),ET_CANNOT_ACCESS_FILE);

    // 26: header + at least one IFD with at least one entry
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iDataSize >= 26 ),ET_FORMAT_IDENTIFIER_MISMATCH);

    // byte order
    Stream qByteOrderStream(m_eStreaming,m_acStreamSrcParam,0,2,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned short usByteOrder;
    qByteOrderStream.Read(usByteOrder);
    switch ( usByteOrder )
    {
    case 18761:
        m_bLittleEndian = true;
        break;
    case 19789:
        m_bLittleEndian = false;
        break;
    default:
        C_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_IDENTIFIER_MISMATCH);
    }
    qByteOrderStream.Finalize();
    m_eByteOrder = ( m_bLittleEndian ? BOT_LITTLE_ENDIAN : BOT_BIG_ENDIAN );

    // signature and offset to the first IFD
    int iIFDOffset;
    Stream qSignatureAndOffsetStream(m_eStreaming,m_acStreamSrcParam,2,6,m_bLittleEndian,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned short usIdentifier;
    qSignatureAndOffsetStream.Read(usIdentifier);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( usIdentifier == 42 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureAndOffsetStream.Read(iIFDOffset);
    qSignatureAndOffsetStream.Finalize();

    unsigned short usFieldCount;
    int iFieldSummaryLength;
    bool bIFDSuccess;
    for (/**/; /**/; /**/)
    {
        // number of entries
        Stream qFieldCountStream(m_eStreaming,m_acStreamSrcParam,iIFDOffset,2,m_bLittleEndian,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qFieldCountStream.Read(usFieldCount);
        qFieldCountStream.Finalize();

        iFieldSummaryLength = usFieldCount*12;

        // read IFD fields
        bIFDSuccess = false;
        ImageFileDirectoryPtr spqIFD;
        Stream qFieldStream(m_eStreaming,m_acStreamSrcParam,iIFDOffset+2,iFieldSummaryLength,m_bLittleEndian,
            bSuccess);
        if ( bSuccess )
        {
            spqIFD = new ImageFileDirectory();
            if ( LoadIFDFields(usFieldCount,*spqIFD,qFieldStream) )
            {
                if ( spqIFD->m_bPhotometricInterpretationFieldIsPresent
                &&   !spqIFD->m_bExtraSamplesFieldIsPresent
                &&   ((spqIFD->m_SamplesPerPixel == 2
                &&   (spqIFD->m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO
                ||   spqIFD->m_ePhotometricInterpretation == PIT_BLACK_IS_ZERO))
                ||   (spqIFD->m_SamplesPerPixel == 4 && spqIFD->m_ePhotometricInterpretation == PIT_RGB)) )
                {
                    // make the concession for this careless TIFF part
                    spqIFD->m_qExtraSamples.SetQuantity(1);
                    spqIFD->m_qExtraSamples[0] = EST_UNASSOCIATED_ALPHA;
                    spqIFD->m_bExtraSamplesFieldIsPresent = true;
                }

                spqIFD->AssignBitsPerSampleAndSampleFormatDefaults();
                if ( spqIFD->AreBitsPerSampleInProperRanges() )
                {
                    if ( spqIFD->AssignDerivedDefaultsAndAttributes() )
                    {
                        if ( !spqIFD->m_bIsTransparencyMask )
                        {
                            m_qImageFileDirectories.Push(spqIFD);
                        }
                        else
                        {
                            // there must be single transparency mask in the file
                            C_CONDITIONAL_THROW_EXCEPTION(( !m_spqTransparencyMaskIFD ),ET_FORMAT_MISMATCH);
                            m_spqTransparencyMaskIFD = spqIFD;
                        }
                        bIFDSuccess = true;
                    }
                }
                else
                {
                    THROW_WARNING(WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD);
                }
            }
        }
        else
        {
            THROW_WARNING(WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD);
        }
        qFieldStream.Finalize();

        if ( bIFDSuccess )
        {
            // now, can do the main part of TIFF file conformity tests
            if ( !ExamineIFDFields(*spqIFD) )
            {
                if ( !spqIFD->m_bIsTransparencyMask )
                    m_qImageFileDirectories.Pop();
                else
                    m_spqTransparencyMaskIFD = 0;
            }
        }

        Stream qNextOffsetStream(m_eStreaming,m_acStreamSrcParam,iIFDOffset+2+iFieldSummaryLength,4,m_bLittleEndian,
            bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qNextOffsetStream.Read(iIFDOffset);
        qNextOffsetStream.Finalize();
        if ( iIFDOffset == 0 )
            break;
    }
    C_CONDITIONAL_THROW_EXCEPTION(( !m_qImageFileDirectories.IsEmpty() ),ET_FORMAT_MISMATCH);

    // a transparency mask, if is present, is to be for only one covered image
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_spqTransparencyMaskIFD && m_qImageFileDirectories.GetQuantity() != 1) ),
        ET_FORMAT_MISMATCH);

    // Exif
    Stream qExifStream(m_eStreaming,m_acStreamSrcParam,0,iDataSize,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    m_spqExifMetadata = new FormatExif(true,qExifStream,iDataSize);
    if ( m_spqExifMetadata->LoadData() )
    {
        m_bHasExifMetadata = true;
    }
    else
    {
        // not distinguishing Exif exception type
        THROW_WARNING(WT_EXIF_METADATA_ARE_INCORRECT);

        m_spqExifMetadata = 0;
    }
    qExifStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::LoadIFDImage (RasterImagePtr& rspqImage, int i)
{
    return LoadIFD(rspqImage,*m_qImageFileDirectories[i],false);
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::PrepareIFDForSaving (const ImageFileDirectoryPtr& rspqIFD, bool bPreparingTransparencyMask)
{
    // the control procedure determining which field with what content is to be included in the file

    ImageFileDirectory& rqIFD = *rspqIFD;
    RasterImage& rqImage = *rqIFD.m_spqImageSaveTo;

    assert( rqImage.GetColorModel() == RasterImage::CMT_PALETTED
        ||  rqImage.GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  rqImage.GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        ||  rqImage.GetColorModel() == RasterImage::CMT_RGB
        ||  rqImage.GetColorModel() == RasterImage::CMT_RGBA
        ||  rqImage.GetColorModel() == RasterImage::CMT_CMYK
        ||  rqImage.GetColorModel() == RasterImage::CMT_LAB
        ||  rqImage.GetColorModel() == RasterImage::CMT_LAB_WITH_ALPHA );
    assert( !(rqImage.GetColorModel() == RasterImage::CMT_PALETTED
        &&  (1 << rqIFD.m_qBitsPerSample[0]) < rqImage.GetPaletteSize()) );

    // Assign fields (with certain variables or defaults) if they were not already assigned.

    // SamplesPerPixel
    rqIFD.m_SamplesPerPixel = rqImage.GetComponentsPerPixel();
    rqIFD.m_bSamplesPerPixelFieldIsPresent = true;

    if ( rqImage.GetPixelComponentType() == RasterImage::PCT_FLOATING_POINT )
    {
        // SampleFormat
        rqIFD.m_qSampleFormat.SetQuantity(rqIFD.m_SamplesPerPixel);
        for (int i = 0; i < rqIFD.m_SamplesPerPixel; i++)
            rqIFD.m_qSampleFormat[i] = SFT_FLOATING_POINT;
        rqIFD.m_bSampleFormatFieldIsPresent = true;
    }

    // the first assign/check
    rqIFD.AssignBitsPerSampleAndSampleFormatDefaults();
    assert( rqIFD.AreBitsPerSampleInProperRanges() );

    // NewSubfileType
    if ( bPreparingTransparencyMask )
        rqIFD.m_bSubfileIsTransparencyMask = true;
    rqIFD.m_bNewSubfileTypeFieldIsPresent = true;

    // ImageWidth and ImageLength
    rqIFD.m_ImageWidth = rqIFD.m_spqImageSaveTo->GetWidth();
    rqIFD.m_bImageWidthFieldIsPresent = true;
    rqIFD.m_ImageLength = rqIFD.m_spqImageSaveTo->GetHeight();
    rqIFD.m_bImageLengthFieldIsPresent = true;

    // Compression
    rqIFD.m_bCompressionFieldIsPresent = true;

    // PhotometricInterpretation
    switch ( rqImage.GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        if ( !bPreparingTransparencyMask )
            rqIFD.m_ePhotometricInterpretation = ( rqIFD.m_bImageIsBilevel ? PIT_WHITE_IS_ZERO : PIT_PALETTED );
        else
            rqIFD.m_ePhotometricInterpretation = PIT_TRANSPARENCY_MASK;
        break;
    case RasterImage::CMT_GRAYSCALE:
    case RasterImage::CMT_GRAYSCALE_WITH_ALPHA:
        rqIFD.m_ePhotometricInterpretation = PIT_BLACK_IS_ZERO;
        break;
    case RasterImage::CMT_RGB:
        rqIFD.m_ePhotometricInterpretation = ( rqIFD.m_eCompression != CT_JPEG_TNT ? PIT_RGB : PIT_YCC );
        break;
    case RasterImage::CMT_RGBA:
        rqIFD.m_ePhotometricInterpretation = PIT_RGB;
        break;
    case RasterImage::CMT_CMYK:
        rqIFD.m_ePhotometricInterpretation = PIT_CMYK;
        break;
    case RasterImage::CMT_LAB:
    case RasterImage::CMT_LAB_WITH_ALPHA:
        rqIFD.m_ePhotometricInterpretation = PIT_CIE_LAB;
        break;
    }
    rqIFD.m_bPhotometricInterpretationFieldIsPresent = true;

    if ( rqIFD.m_ePhotometricInterpretation == PIT_PALETTED )
    {
        // ColorMap
        rqIFD.m_qColorMap.SetQuantity(3*rqImage.GetPaletteSize());
        for (int i = 0; i < rqImage.GetPaletteSize(); i++)
        {
            const PaletteColor& rqPaletteColor = rqImage.GetPaletteColor(i);

            rqIFD.m_qColorMap[0*rqImage.GetPaletteSize()+i] =
                Mathf::RoundToInt(((float)rqPaletteColor.R())/255*65535);
            rqIFD.m_qColorMap[1*rqImage.GetPaletteSize()+i] =
                Mathf::RoundToInt(((float)rqPaletteColor.G())/255*65535);
            rqIFD.m_qColorMap[2*rqImage.GetPaletteSize()+i] =
                Mathf::RoundToInt(((float)rqPaletteColor.B())/255*65535);
        }
        rqIFD.m_bColorMapFieldIsPresent = true;
    }

    if ( rqImage.GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
    ||   rqImage.GetColorModel() == RasterImage::CMT_RGBA
    ||   rqImage.GetColorModel() == RasterImage::CMT_LAB_WITH_ALPHA )
    {
        // ExtraSamples
        rqIFD.m_qExtraSamples.SetQuantity(1);
        rqIFD.m_qExtraSamples[0] = EST_ASSOCIATED_ALPHA;
        rqIFD.m_bExtraSamplesFieldIsPresent = true;
    }

    // segment organization
    int iBytesPerSegment = ms_iKBytesPerSegment*1024;
    if ( m_eSegmentOrganization == SOT_STRIP_ORIENTED )
    {
        int iRowSizeInBits = rqIFD.m_ImageWidth*rqIFD.m_iBitsPerPixel;
        int iRowSize = ( (iRowSizeInBits & 7) == 0 ? iRowSizeInBits/8 : iRowSizeInBits/8 + 1 );

        // RowsPerStrip
        if ( rqIFD.m_eCompression != CT_JPEG_TNT )
        {
            rqIFD.m_RowsPerStrip = iBytesPerSegment/iRowSize;
            if ( rqIFD.m_RowsPerStrip == 0 )
                rqIFD.m_RowsPerStrip = 1;
        }
        else
        {
            rqIFD.m_RowsPerStrip = rqIFD.m_ImageLength;
        }
        rqIFD.m_bRowsPerStripFieldIsPresent = true;

        int iStripsPerImage = (rqIFD.m_ImageLength + rqIFD.m_RowsPerStrip - 1)/rqIFD.m_RowsPerStrip;

        // StripOffsets (just allocate meanwhile)
        rqIFD.m_qStripOffsets.SetQuantity(iStripsPerImage);
        rqIFD.m_bStripOffsetsFieldIsPresent = true;

        // StripByteCounts (just allocate meanwhile)
        rqIFD.m_qStripByteCounts.SetQuantity(iStripsPerImage);
        rqIFD.m_bStripByteCountsFieldIsPresent = true;
    }
    else  // m_eSegmentOrganization = SOT_TILE_ORIENTED
    {
        // try to maintain image aspect ratio for tile dimensions, and use the equation
        // TileArea = (TileWidth^2)/ImageAspectRatio
        float fAspectRatio = ((float)rqIFD.m_ImageWidth)/rqIFD.m_ImageLength;
        int iMappedTileWidth =
            Mathf::RoundToInt(Mathf::Sqrt((((float)iBytesPerSegment)/rqIFD.m_iBitsPerPixel*8)*fAspectRatio));

        // TileWidth
        if ( rqIFD.m_eCompression != CT_JPEG_TNT )
        {
            rqIFD.m_TileWidth = iMappedTileWidth;
            if ( rqIFD.m_TileWidth == 0 )
                rqIFD.m_TileWidth = 1;
        }
        else
        {
            rqIFD.m_TileWidth = rqIFD.m_ImageWidth;
        }
        if ( (rqIFD.m_TileWidth & 15) != 0 )
            rqIFD.m_TileWidth = (rqIFD.m_TileWidth/16 + 1)*16;
        rqIFD.m_bTileWidthFieldIsPresent = true;

        // TileLength
        if ( rqIFD.m_eCompression != CT_JPEG_TNT )
        {
            rqIFD.m_TileLength = Mathf::RoundToInt(((float)iMappedTileWidth)/fAspectRatio);
            if ( rqIFD.m_TileLength == 0 )
                rqIFD.m_TileLength = 1;
        }
        else
        {
            rqIFD.m_TileLength = rqIFD.m_ImageLength;
        }
        if ( (rqIFD.m_TileLength & 15) != 0 )
            rqIFD.m_TileLength = (rqIFD.m_TileLength/16 + 1)*16;
        rqIFD.m_bTileLengthFieldIsPresent = true;

        int iTilesAcross = (rqIFD.m_ImageWidth + rqIFD.m_TileWidth - 1)/rqIFD.m_TileWidth;
        int iTilesDown = (rqIFD.m_ImageLength + rqIFD.m_TileLength - 1)/rqIFD.m_TileLength;
        int iTilesPerImage = iTilesAcross*iTilesDown;

        // TileOffsets (just allocate meanwhile)
        rqIFD.m_qTileOffsets.SetQuantity(iTilesPerImage);
        rqIFD.m_bTileOffsetsFieldIsPresent = true;

        // TileByteCounts (just allocate meanwhile)
        rqIFD.m_qTileByteCounts.SetQuantity(iTilesPerImage);
        rqIFD.m_bTileByteCountsFieldIsPresent = true;

        int iW = iTilesAcross*rqIFD.m_TileWidth;
        int iH = iTilesDown*rqIFD.m_TileLength;
        if ( iW != rqIFD.m_spqImageSaveTo->GetWidth()
        ||   iH != rqIFD.m_spqImageSaveTo->GetHeight() )
        {
            // expand image canvas and replicate utmost pixels
            RasterImagePtr spqExImage;
            if ( rqIFD.m_spqImageSaveTo->GetColorModel() != RasterImage::CMT_PALETTED )
            {
                spqExImage = new RasterImage(rqIFD.m_spqImageSaveTo->GetType(),iW,iH);
            }
            else
            {
                spqExImage = new RasterImage(rqIFD.m_spqImageSaveTo->GetType(),iW,iH,
                    rqIFD.m_spqImageSaveTo->GetPalette());
            }
            spqExImage->SetSubimage(rqIFD.m_spqImageSaveTo,0,0);
            if ( iW != rqIFD.m_spqImageSaveTo->GetWidth() )
            {
                int iWVal = (rqIFD.m_spqImageSaveTo->GetWidth() - 1)*spqExImage->GetBytesPerPixel();
                char* acRow;
                char* pcExPixel;
                for (int i0 = 0; i0 < rqIFD.m_spqImageSaveTo->GetHeight(); i0++)
                {
                    acRow = spqExImage->GetRow(i0);
                    pcExPixel = acRow + iWVal;
                    for (int i1 = rqIFD.m_spqImageSaveTo->GetWidth(); i1 < spqExImage->GetWidth(); i1++)
                        MemCpy(acRow+i1*spqExImage->GetBytesPerPixel(),pcExPixel,spqExImage->GetBytesPerPixel());
                }
            }
            if ( iH != rqIFD.m_spqImageSaveTo->GetHeight() )
            {
                char* acExRow = spqExImage->GetRow(rqIFD.m_spqImageSaveTo->GetHeight()-1);
                int iRowSize = rqIFD.m_spqImageSaveTo->GetWidth()*spqExImage->GetBytesPerPixel();
                for (int i = rqIFD.m_spqImageSaveTo->GetHeight(); i < spqExImage->GetHeight(); i++)
                    MemCpy(spqExImage->GetRow(i),acExRow,iRowSize);
            }
            rqIFD.m_spqImageSaveTo = spqExImage;
        }
    }

    // XResolution, YResolution, and ResolutionUnit
    if ( !rqIFD.m_bDoNotWriteResolutionInfo )
    {
        if ( !rqIFD.m_bXResolutionFieldIsPresent
        &&   !rqIFD.m_bYResolutionFieldIsPresent
        &&   !rqIFD.m_bResolutionUnitFieldIsPresent )
        {
            rqIFD.m_fXResolution = ms_fDefaultXResolutionInchForSave;
            rqIFD.m_bXResolutionFieldIsPresent = true;
            rqIFD.m_fYResolution = ms_fDefaultYResolutionInchForSave;
            rqIFD.m_bYResolutionFieldIsPresent = true;
            rqIFD.m_bResolutionUnitFieldIsPresent = true;
        }
    }

    if ( rqIFD.m_SamplesPerPixel > 1 )
    {
        // PlanarConfiguration
        rqIFD.m_bPlanarConfigurationFieldIsPresent = true;
    }

    if ( (rqIFD.m_eCompression == CT_LZW || rqIFD.m_eCompression == CT_ZLIB)
    &&   rqIFD.m_bBitsPerSamplesAreTheSame && rqIFD.m_iBitsPerEachSample == 8
    &&   rqIFD.m_ePhotometricInterpretation != PIT_PALETTED )
    {
        // Predictor
        rqIFD.m_ePredictor = PT_HORIZONTAL_DIFFERENCING;
        rqIFD.m_bPredictorFieldIsPresent = true;
    }

    // the last assignation
    rqIFD.AssignDerivedDefaultsAndAttributes();
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::SaveIFDFields (const ImageFileDirectory& rqIFD, int& riSeparatedValuesBytePos,
    Stream& rqSaveStream, StillArray<int>& rqOffsetsBytePosArray, StillArray<int>& rqByteCountsBytePosArray)
{
    unsigned short usTag, usType;
    FieldType eType;
    int iCount, iValueLength;
    bool bValueFitsFourBytes;
    Short ShortValue;
    Long LongValue;
    char cNull = 0;

    // NewSubfileType
    if ( rqIFD.m_bNewSubfileTypeFieldIsPresent )
    {
        usTag = 254;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        LongValue = 0;
        if ( rqIFD.m_bSubfileIsReducedResolutionVersion )
            LongValue |= 1;
        if ( rqIFD.m_bSubfileIsSinglePageOfMultipageImage )
            LongValue |= 2;
        if ( rqIFD.m_bSubfileIsTransparencyMask )
            LongValue |= 4;
        rqSaveStream.Write(LongValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ImageWidth
    if ( rqIFD.m_bImageWidthFieldIsPresent )
    {
        usTag = 256;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_ImageWidth);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ImageLength
    if ( rqIFD.m_bImageLengthFieldIsPresent )
    {
        usTag = 257;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_ImageLength);
        AFTER_FIELD_VALUE_WRITE;
    }

    // BitsPerSample
    if ( rqIFD.m_bBitsPerSampleFieldIsPresent )
    {
        usTag = 258;
        eType = FT_SHORT;
        iCount = rqIFD.m_qBitsPerSample.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qBitsPerSample.GetQuantity(),rqIFD.m_qBitsPerSample);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Compression
    if ( rqIFD.m_bCompressionFieldIsPresent )
    {
        usTag = 259;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_eCompression )
        {
        case CT_NONE:
            ShortValue = 1;
            break;
        case CT_MODIFIED_HUFFMAN:
            ShortValue = 2;
            break;
        case CT_GROUP_3:
            ShortValue = 3;
            break;
        case CT_GROUP_4:
            ShortValue = 4;
            break;
        case CT_LZW:
            ShortValue = 5;
            break;
        case CT_JPEG_TNT:
            ShortValue = 7;
            break;
        case CT_ZLIB:
            ShortValue = 8;
            break;
        case CT_RLE:
            ShortValue = 32773;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // PhotometricInterpretation
    if ( rqIFD.m_bPhotometricInterpretationFieldIsPresent )
    {
        usTag = 262;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_ePhotometricInterpretation )
        {
        case PIT_WHITE_IS_ZERO:
            ShortValue = 0;
            break;
        case PIT_BLACK_IS_ZERO:
            ShortValue = 1;
            break;
        case PIT_RGB:
            ShortValue = 2;
            break;
        case PIT_PALETTED:
            ShortValue = 3;
            break;
        case PIT_TRANSPARENCY_MASK:
            ShortValue = 4;
            break;
        case PIT_CMYK:
            ShortValue = 5;
            break;
        case PIT_YCC:
            ShortValue = 6;
            break;
        case PIT_CIE_LAB:
            ShortValue = 8;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Threshholding
    if ( rqIFD.m_bThreshholdingFieldIsPresent )
    {
        usTag = 263;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_eThreshholding )
        {
        case TT_NO_DITHERING_OR_HALFTONING_APPLIED:
            ShortValue = 1;
            break;
        case TT_ORDERED_DITHERING_OR_HALFTONING_APPLIED:
            ShortValue = 2;
            break;
        case TT_RANDOMIZED_DITHERING_OR_HALFTONING_APPLIED:
            ShortValue = 3;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // CellWidth
    if ( rqIFD.m_bCellWidthFieldIsPresent )
    {
        usTag = 264;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_CellWidth);
        AFTER_FIELD_VALUE_WRITE;
    }

    // CellLength
    if ( rqIFD.m_bCellLengthFieldIsPresent )
    {
        usTag = 265;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_CellLength);
        AFTER_FIELD_VALUE_WRITE;
    }

    // DocumentName
    if ( rqIFD.m_bDocumentNameFieldIsPresent )
    {
        usTag = 269;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qDocumentName);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qDocumentName,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ImageDescription
    if ( rqIFD.m_bImageDescriptionFieldIsPresent )
    {
        usTag = 270;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qImageDescription);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qImageDescription,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Make
    if ( rqIFD.m_bMakeFieldIsPresent )
    {
        usTag = 271;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qMake);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qMake,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Model
    if ( rqIFD.m_bModelFieldIsPresent )
    {
        usTag = 272;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qModel);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qModel,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // StripOffsets
    if ( rqIFD.m_bStripOffsetsFieldIsPresent )
    {
        rqOffsetsBytePosArray.SetQuantity(rqIFD.m_qStripOffsets.GetQuantity());

        usTag = 273;
        eType = FT_LONG;
        iCount = rqIFD.m_qStripOffsets.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qStripOffsets.GetQuantity(); i++)
        {
            rqOffsetsBytePosArray[i] = rqSaveStream.GetBytePosition();
            rqSaveStream.Write(rqIFD.m_qStripOffsets[i]);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // Orientation
    if ( rqIFD.m_bOrientationFieldIsPresent )
    {
        usTag = 274;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_eOrientation )
        {
        case OT_TOP_LEFT:
            ShortValue = 1;
            break;
        case OT_TOP_RIGHT:
            ShortValue = 2;
            break;
        case OT_BOTTOM_RIGHT:
            ShortValue = 3;
            break;
        case OT_BOTTOM_LEFT:
            ShortValue = 4;
            break;
        case OT_LEFT_TOP:
            ShortValue = 5;
            break;
        case OT_RIGHT_TOP:
            ShortValue = 6;
            break;
        case OT_RIGHT_BOTTOM:
            ShortValue = 7;
            break;
        case OT_LEFT_BOTTOM:
            ShortValue = 8;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // SamplesPerPixel
    if ( rqIFD.m_bSamplesPerPixelFieldIsPresent )
    {
        usTag = 277;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_SamplesPerPixel);
        AFTER_FIELD_VALUE_WRITE;
    }

    // RowsPerStrip
    if ( rqIFD.m_bRowsPerStripFieldIsPresent )
    {
        usTag = 278;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_RowsPerStrip);
        AFTER_FIELD_VALUE_WRITE;
    }

    // StripByteCounts
    if ( rqIFD.m_bStripByteCountsFieldIsPresent )
    {
        rqByteCountsBytePosArray.SetQuantity(rqIFD.m_qStripByteCounts.GetQuantity());

        usTag = 279;
        eType = FT_LONG;
        iCount = rqIFD.m_qStripByteCounts.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qStripByteCounts.GetQuantity(); i++)
        {
            rqByteCountsBytePosArray[i] = rqSaveStream.GetBytePosition();
            rqSaveStream.Write(rqIFD.m_qStripByteCounts[i]);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // MinSampleValue
    if ( rqIFD.m_bMinSampleValueFieldIsPresent )
    {
        usTag = 280;
        eType = FT_SHORT;
        iCount = rqIFD.m_qMinSampleValue.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qMinSampleValue.GetQuantity(),rqIFD.m_qMinSampleValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // MaxSampleValue
    if ( rqIFD.m_bMaxSampleValueFieldIsPresent )
    {
        usTag = 281;
        eType = FT_SHORT;
        iCount = rqIFD.m_qMaxSampleValue.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qMaxSampleValue.GetQuantity(),rqIFD.m_qMaxSampleValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // XResolution
    if ( rqIFD.m_bXResolutionFieldIsPresent )
    {
        usTag = 282;
        eType = FT_RATIONAL;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fXResolution,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // YResolution
    if ( rqIFD.m_bYResolutionFieldIsPresent )
    {
        usTag = 283;
        eType = FT_RATIONAL;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fYResolution,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // PlanarConfiguration
    if ( rqIFD.m_bPlanarConfigurationFieldIsPresent )
    {
        usTag = 284;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_ePlanarConfiguration )
        {
        case PCT_CHUNKY_FORMAT:
            ShortValue = 1;
            break;
        case PCT_PLANAR_FORMAT:
            ShortValue = 2;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // PageName
    if ( rqIFD.m_bPageNameFieldIsPresent )
    {
        usTag = 285;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qPageName);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qPageName,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // XPosition
    if ( rqIFD.m_bXPositionFieldIsPresent )
    {
        usTag = 286;
        eType = FT_RATIONAL;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fXPosition,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // YPosition
    if ( rqIFD.m_bYPositionFieldIsPresent )
    {
        usTag = 287;
        eType = FT_RATIONAL;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fYPosition,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // GrayResponseUnit
    if ( rqIFD.m_bGrayResponseUnitFieldIsPresent )
    {
        usTag = 290;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_eGrayResponseUnit )
        {
        case GRUT_TENTHS_OF_UNIT:
            ShortValue = 1;
            break;
        case GRUT_HUNDREDTHS_OF_UNIT:
            ShortValue = 2;
            break;
        case GRUT_THOUSANDTHS_OF_UNIT:
            ShortValue = 3;
            break;
        case GRUT_TEN_THOUSANDTHS_OF_UNIT:
            ShortValue = 4;
            break;
        case GRUT_HUNDRED_THOUSANDTHS_OF_UNIT:
            ShortValue = 5;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // GrayResponseCurve
    if ( rqIFD.m_bGrayResponseCurveFieldIsPresent )
    {
        usTag = 291;
        eType = FT_SHORT;
        iCount = rqIFD.m_qGrayResponseCurve.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qGrayResponseCurve.GetQuantity(),rqIFD.m_qGrayResponseCurve);
        AFTER_FIELD_VALUE_WRITE;
    }

    // T4Options
    if ( rqIFD.m_bT4OptionsFieldIsPresent )
    {
        usTag = 292;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        LongValue = 0;
        if ( rqIFD.m_bT42d )
            LongValue |= 1;
        if ( rqIFD.m_bT4UncompressedModeUsed )
            LongValue |= 2;
        if ( rqIFD.m_bT4FillBitsBeforeEolUsed )
            LongValue |= 4;
        rqSaveStream.Write(LongValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ResolutionUnit
    if ( rqIFD.m_bResolutionUnitFieldIsPresent )
    {
        usTag = 296;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_eResolutionUnit )
        {
        case RUT_NO_ABSOLUTE_UNIT_OF_MEASUREMENT:
            ShortValue = 1;
            break;
        case RUT_INCH:
            ShortValue = 2;
            break;
        case RUT_CENTIMETER:
            ShortValue = 3;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // PageNumber
    if ( rqIFD.m_bPageNumberFieldIsPresent )
    {
        usTag = 297;
        eType = FT_SHORT;
        iCount = 2;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_PageNumber);
        rqSaveStream.Write(rqIFD.m_TotalPageNumber);
        AFTER_FIELD_VALUE_WRITE;
    }

    // TransferFunction
    if ( rqIFD.m_bTransferFunctionFieldIsPresent )
    {
        usTag = 301;
        eType = FT_SHORT;
        iCount = rqIFD.m_qTransferFunction.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qTransferFunction.GetQuantity(),rqIFD.m_qTransferFunction);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Software
    if ( rqIFD.m_bSoftwareFieldIsPresent )
    {
        usTag = 305;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qSoftware);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qSoftware,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // DateTime
    if ( rqIFD.m_bDateTimeFieldIsPresent )
    {
        usTag = 306;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qDateTime);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qDateTime,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Artist
    if ( rqIFD.m_bArtistFieldIsPresent )
    {
        usTag = 315;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qArtist);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qArtist,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // HostComputer
    if ( rqIFD.m_bHostComputerFieldIsPresent )
    {
        usTag = 316;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qHostComputer);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qHostComputer,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Predictor
    if ( rqIFD.m_bPredictorFieldIsPresent )
    {
        usTag = 317;
        eType = FT_SHORT;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        switch ( rqIFD.m_ePredictor )
        {
        case PT_NO_PREDICTION:
            ShortValue = 1;
            break;
        case PT_HORIZONTAL_DIFFERENCING:
            ShortValue = 2;
            break;
        }
        rqSaveStream.Write(ShortValue);
        AFTER_FIELD_VALUE_WRITE;
    }

    // WhitePoint
    if ( rqIFD.m_bWhitePointFieldIsPresent )
    {
        usTag = 318;
        eType = FT_RATIONAL;
        iCount = 2;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fWhitePointX,rqSaveStream);
        WriteRational(rqIFD.m_fWhitePointY,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // PrimaryChromaticities
    if ( rqIFD.m_bPrimaryChromaticitiesFieldIsPresent )
    {
        usTag = 319;
        eType = FT_RATIONAL;
        iCount = 6;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fChromaticityRedX,rqSaveStream);
        WriteRational(rqIFD.m_fChromaticityRedY,rqSaveStream);
        WriteRational(rqIFD.m_fChromaticityGreenX,rqSaveStream);
        WriteRational(rqIFD.m_fChromaticityGreenY,rqSaveStream);
        WriteRational(rqIFD.m_fChromaticityBlueX,rqSaveStream);
        WriteRational(rqIFD.m_fChromaticityBlueY,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ColorMap
    if ( rqIFD.m_bColorMapFieldIsPresent )
    {
        usTag = 320;
        eType = FT_SHORT;
        iCount = rqIFD.m_qColorMap.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qColorMap.GetQuantity(),rqIFD.m_qColorMap);
        AFTER_FIELD_VALUE_WRITE;
    }

    // HalftoneHints
    if ( rqIFD.m_bHalftoneHintsFieldIsPresent )
    {
        usTag = 321;
        eType = FT_SHORT;
        iCount = 2;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_HighlightGrayLevel);
        rqSaveStream.Write(rqIFD.m_ShadowGrayLevel);
        AFTER_FIELD_VALUE_WRITE;
    }

    // TileWidth
    if ( rqIFD.m_bTileWidthFieldIsPresent )
    {
        usTag = 322;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_TileWidth);
        AFTER_FIELD_VALUE_WRITE;
    }

    // TileLength
    if ( rqIFD.m_bTileLengthFieldIsPresent )
    {
        usTag = 323;
        eType = FT_LONG;
        iCount = 1;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_TileLength);
        AFTER_FIELD_VALUE_WRITE;
    }

    // TileOffsets
    if ( rqIFD.m_bTileOffsetsFieldIsPresent )
    {
        rqOffsetsBytePosArray.SetQuantity(rqIFD.m_qTileOffsets.GetQuantity());

        usTag = 324;
        eType = FT_LONG;
        iCount = rqIFD.m_qTileOffsets.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qTileOffsets.GetQuantity(); i++)
        {
            rqOffsetsBytePosArray[i] = rqSaveStream.GetBytePosition();
            rqSaveStream.Write(rqIFD.m_qTileOffsets[i]);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // TileByteCounts
    if ( rqIFD.m_bTileByteCountsFieldIsPresent )
    {
        rqByteCountsBytePosArray.SetQuantity(rqIFD.m_qTileByteCounts.GetQuantity());

        usTag = 325;
        eType = FT_LONG;
        iCount = rqIFD.m_qTileByteCounts.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qTileByteCounts.GetQuantity(); i++)
        {
            rqByteCountsBytePosArray[i] = rqSaveStream.GetBytePosition();
            rqSaveStream.Write(rqIFD.m_qTileByteCounts[i]);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // DotRange
    if ( rqIFD.m_bDotRangeFieldIsPresent )
    {
        usTag = 336;
        eType = FT_SHORT;
        iCount = rqIFD.m_qDotRange.GetQuantity()*2;
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qDotRange.GetQuantity(); i++)
        {
            const Link<Short,Short>& rqRange = rqIFD.m_qDotRange[i];

            rqSaveStream.Write(rqRange.First);
            rqSaveStream.Write(rqRange.Second);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // TargetPrinter
    if ( rqIFD.m_bTargetPrinterFieldIsPresent )
    {
        usTag = 337;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qTargetPrinter);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qTargetPrinter,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ExtraSamples
    if ( rqIFD.m_bExtraSamplesFieldIsPresent )
    {
        usTag = 338;
        eType = FT_SHORT;
        iCount = rqIFD.m_qExtraSamples.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qExtraSamples.GetQuantity(); i++)
        {
            switch ( rqIFD.m_qExtraSamples[i] )
            {
            case EST_UNSPECIFIED:
                ShortValue = 0;
                break;
            case EST_ASSOCIATED_ALPHA:
                ShortValue = 1;
                break;
            case EST_UNASSOCIATED_ALPHA:
                ShortValue = 2;
                break;
            }
            rqSaveStream.Write(ShortValue);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // SampleFormat
    if ( rqIFD.m_bSampleFormatFieldIsPresent )
    {
        usTag = 339;
        eType = FT_SHORT;
        iCount = rqIFD.m_qSampleFormat.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        for (int i = 0; i < rqIFD.m_qSampleFormat.GetQuantity(); i++)
        {
            switch ( rqIFD.m_qSampleFormat[i] )
            {
            case SFT_UNSIGNED_INTEGER:
                ShortValue = 1;
                break;
            case SFT_SIGNED_INTEGER:
                ShortValue = 2;
                break;
            case SFT_FLOATING_POINT:
                ShortValue = 3;
                break;
            case SFT_UNDEFINED:
                ShortValue = 4;
                break;
            }
            rqSaveStream.Write(ShortValue);
        }
        AFTER_FIELD_VALUE_WRITE;
    }

    // SMinSampleValue
    if ( rqIFD.m_bSMinSampleValueFieldIsPresent )
    {
        usTag = 340;
        if ( !rqIFD.m_qSMinSampleValueUI.IsEmpty() )
        {
            eType = FT_LONG;
            iCount = rqIFD.m_qSMinSampleValueUI.GetQuantity();
        }
        else
        {
            eType = FT_FLOAT;
            iCount = rqIFD.m_qSMinSampleValueFP.GetQuantity();
        }
        BEFORE_FIELD_VALUE_WRITE;
        if ( !rqIFD.m_qSMinSampleValueUI.IsEmpty() )
            rqSaveStream.Write(rqIFD.m_qSMinSampleValueUI.GetQuantity(),rqIFD.m_qSMinSampleValueUI);
        else
            rqSaveStream.Write(rqIFD.m_qSMinSampleValueFP.GetQuantity(),rqIFD.m_qSMinSampleValueFP);
        AFTER_FIELD_VALUE_WRITE;
    }

    // SMaxSampleValue
    if ( rqIFD.m_bSMaxSampleValueFieldIsPresent )
    {
        usTag = 341;
        if ( !rqIFD.m_qSMaxSampleValueUI.IsEmpty() )
        {
            eType = FT_LONG;
            iCount = rqIFD.m_qSMaxSampleValueUI.GetQuantity();
        }
        else
        {
            eType = FT_FLOAT;
            iCount = rqIFD.m_qSMaxSampleValueFP.GetQuantity();
        }
        BEFORE_FIELD_VALUE_WRITE;
        if ( !rqIFD.m_qSMaxSampleValueUI.IsEmpty() )
            rqSaveStream.Write(rqIFD.m_qSMaxSampleValueUI.GetQuantity(),rqIFD.m_qSMaxSampleValueUI);
        else
            rqSaveStream.Write(rqIFD.m_qSMaxSampleValueFP.GetQuantity(),rqIFD.m_qSMaxSampleValueFP);
        AFTER_FIELD_VALUE_WRITE;
    }

    // TransferRange
    if ( rqIFD.m_bTransferRangeFieldIsPresent )
    {
        usTag = 342;
        eType = FT_SHORT;
        iCount = 6;
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_TransferBlack0);
        rqSaveStream.Write(rqIFD.m_TransferWhite0);
        rqSaveStream.Write(rqIFD.m_TransferBlack1);
        rqSaveStream.Write(rqIFD.m_TransferWhite1);
        rqSaveStream.Write(rqIFD.m_TransferBlack2);
        rqSaveStream.Write(rqIFD.m_TransferWhite2);
        AFTER_FIELD_VALUE_WRITE;
    }

    // JPEGTables
    if ( rqIFD.m_bJPEGTablesFieldIsPresent )
    {
        usTag = 347;
        eType = FT_UNDEFINED;
        iCount = rqIFD.m_qJPEGTables.GetQuantity();
        BEFORE_FIELD_VALUE_WRITE;
        rqSaveStream.Write(rqIFD.m_qJPEGTables.GetQuantity(),rqIFD.m_qJPEGTables);
        AFTER_FIELD_VALUE_WRITE;
    }

    // ReferenceBlackWhite
    if ( rqIFD.m_bReferenceBlackWhiteFieldIsPresent )
    {
        usTag = 532;
        eType = FT_RATIONAL;
        iCount = 6;
        BEFORE_FIELD_VALUE_WRITE;
        WriteRational(rqIFD.m_fReferenceBlack0,rqSaveStream);
        WriteRational(rqIFD.m_fReferenceWhite0,rqSaveStream);
        WriteRational(rqIFD.m_fReferenceBlack1,rqSaveStream);
        WriteRational(rqIFD.m_fReferenceWhite1,rqSaveStream);
        WriteRational(rqIFD.m_fReferenceBlack2,rqSaveStream);
        WriteRational(rqIFD.m_fReferenceWhite2,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }

    // Copyright
    if ( rqIFD.m_bCopyrightFieldIsPresent )
    {
        usTag = 33432;
        eType = FT_ASCII;
        iCount = GetCountForString(rqIFD.m_qCopyright);
        BEFORE_FIELD_VALUE_WRITE;
        WriteString(rqIFD.m_qCopyright,rqSaveStream);
        AFTER_FIELD_VALUE_WRITE;
    }
}
//------------------------------------------------------------------------------------------------------------------
int FormatTiff::GetCountForString (const StringArray& rqStringArray)
{
    int iCount = 0;
    for (int i = 0; i < rqStringArray.GetQuantity(); i++)
        iCount += rqStringArray[i].GetLength() + 1;
    return iCount;
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::WriteString (const StringArray& rqStringArray, Stream& rqSaveStream)
{
    char cNull = 0;
    for (int i = 0; i < rqStringArray.GetQuantity(); i++)
    {
        const String& rqString = rqStringArray[i];

        rqSaveStream.Write(rqString.GetLength(),rqString);
        rqSaveStream.Write(cNull);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::WriteRational (float fValue, Stream& rqSaveStream)
{
    Long Num, Denom;
    if ( fValue < 1.0f )
    {
        Num = ms_iRationalNumbersScaler;
        Denom = Mathf::RoundToInt(((float)Num)/fValue);
    }
    else
    {
        Denom = ms_iRationalNumbersScaler;
        Num = Mathf::RoundToInt(fValue*Denom);
    }
    rqSaveStream.Write(Num);
    rqSaveStream.Write(Denom);
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::FilterSegmentImage (const SegmentRecord& rqSegment, StillArray<char>& rqDecompressedData,
    const ImageFileDirectory& rqIFD)
{
    // 8 bits per each sample is assumed
    int iRowSize = rqSegment.Width*rqIFD.m_SamplesPerPixel;
    char* acRow = rqDecompressedData;
    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
    {
        for (int iX = iRowSize-1; iX >= rqIFD.m_SamplesPerPixel; iX--)
            acRow[iX] -= acRow[iX-rqIFD.m_SamplesPerPixel];

        acRow += iRowSize;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::LoadIFDFields (int iFieldCount, ImageFileDirectory& rqIFD, Stream& rqFieldStream)
{
    bool bSuccess;
    Stream* pqSeparatedValueStream = 0;

    unsigned short usTag, usType;
    FieldType eType;
    int iCount, iBytePosAfterCount, iValueOffset, iValueLength;
    bool bFieldIsOk, bValueFitsFourBytes;
    for (int i0 = 0; i0 < iFieldCount; i0++)
    {
        rqFieldStream.Read(usTag);
        rqFieldStream.Read(usType);
        rqFieldStream.Read(iCount);

        iBytePosAfterCount = rqFieldStream.GetBytePosition();

        bFieldIsOk = true;

        switch ( usType )
        {
        case 1:
            eType = FT_BYTE;
            iValueLength = iCount;
            break;
        case 2:
            eType = FT_ASCII;
            iValueLength = iCount;
            break;
        case 3:
            eType = FT_SHORT;
            iValueLength = 2*iCount;
            break;
        case 4:
            eType = FT_LONG;
            iValueLength = 4*iCount;
            break;
        case 5:
            eType = FT_RATIONAL;
            iValueLength = 8*iCount;
            break;
        case 6:
            eType = FT_SBYTE;
            iValueLength = iCount;
            break;
        case 7:
            eType = FT_UNDEFINED;
            iValueLength = iCount;
            break;
        case 8:
            eType = FT_SSHORT;
            iValueLength = 2*iCount;
            break;
        case 9:
            eType = FT_SLONG;
            iValueLength = 4*iCount;
            break;
        case 10:
            eType = FT_SRATIONAL;
            iValueLength = 8*iCount;
            break;
        case 11:
            eType = FT_FLOAT;
            iValueLength = 4*iCount;
            break;
        case 12:
            eType = FT_DOUBLE;
            iValueLength = 8*iCount;
            break;
        default:
            bFieldIsOk = false;
            bValueFitsFourBytes = true;
            goto FieldFingering;
        }
        bValueFitsFourBytes = ( iValueLength <= 4 );

        // find out how to retrieve values
        if ( !bValueFitsFourBytes )
        {
            rqFieldStream.Read(iValueOffset);
            pqSeparatedValueStream = new Stream(m_eStreaming,m_acStreamSrcParam,iValueOffset,iValueLength,
                m_bLittleEndian,bSuccess);
            if ( !bSuccess )
            {
                bFieldIsOk = false;
                goto FieldFingering;
            }
        }
        Stream& rqValueStream = ( bValueFitsFourBytes ? rqFieldStream : *pqSeparatedValueStream );

FieldFingering:
        if ( usTag == 254 )
        {
            // NewSubfileType
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_LONG && iCount == 1 ));

            Long Value;
            rqValueStream.Read(Value);
            if ( ((Value >> 0) & 1) == 1 )
                rqIFD.m_bSubfileIsReducedResolutionVersion = true;
            if ( ((Value >> 1) & 1) == 1 )
                rqIFD.m_bSubfileIsSinglePageOfMultipageImage = true;
            if ( ((Value >> 2) & 1) == 1 )
                rqIFD.m_bSubfileIsTransparencyMask = true;

            rqIFD.m_bNewSubfileTypeFieldIsPresent = true;
        }
        else if ( usTag == 255 )
        {
            // SubfileType
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eSubfileType = ST_FULL_RESOLUTION_IMAGE;
                break;
            case 2:
                rqIFD.m_eSubfileType = ST_REDUCED_RESOLUTION_IMAGE;
                break;
            case 3:
                rqIFD.m_eSubfileType = ST_SINGLE_PAGE_OF_MULTIPAGE_IMAGE;
                break;
            default:
                SKIP_FIELD_WITH_THE_WARNING;
            }

            rqIFD.m_bSubfileTypeFieldIsPresent = true;
        }
        else if ( usTag == 256 )
        {
            // ImageWidth (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( (eType == FT_SHORT || eType == FT_LONG) && iCount == 1 ));

            if ( eType == FT_SHORT )
            {
                Short Value;
                rqValueStream.Read(Value);
                rqIFD.m_ImageWidth = Value;
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(rqIFD.m_ImageWidth);
            }
            ESCAPE_IFD_ON_FAIL(( rqIFD.m_ImageWidth != 0 ));

            rqIFD.m_bImageWidthFieldIsPresent = true;
        }
        else if ( usTag == 257 )
        {
            // ImageLength (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( (eType == FT_SHORT || eType == FT_LONG) && iCount == 1 ));

            if ( eType == FT_SHORT )
            {
                Short Value;
                rqValueStream.Read(Value);
                rqIFD.m_ImageLength = Value;
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(rqIFD.m_ImageLength);
            }
            ESCAPE_IFD_ON_FAIL(( rqIFD.m_ImageLength != 0 ));

            rqIFD.m_bImageLengthFieldIsPresent = true;
        }
        else if ( usTag == 258 )
        {
            // BitsPerSample (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qBitsPerSample.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qBitsPerSample);

            rqIFD.m_bBitsPerSampleFieldIsPresent = true;
        }
        else if ( usTag == 259 )
        {
            // Compression (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eCompression = CT_NONE;
                break;
            case 2:
                rqIFD.m_eCompression = CT_MODIFIED_HUFFMAN;
                break;
            case 3:
                rqIFD.m_eCompression = CT_GROUP_3;
                break;
            case 4:
                rqIFD.m_eCompression = CT_GROUP_4;
                break;
            case 5:
                rqIFD.m_eCompression = CT_LZW;
                break;
            case 6:
                ESCAPE_IFD_WITH_WARNING(
                    WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_OBSOLETE_JPEG_COMPRESSION);
                break;
            case 7:
                rqIFD.m_eCompression = CT_JPEG_TNT;
                break;
            case 8:
                rqIFD.m_eCompression = CT_ZLIB;
                break;
            case 32773:
                rqIFD.m_eCompression = CT_RLE;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bCompressionFieldIsPresent = true;
        }
        else if ( usTag == 262 )
        {
            // PhotometricInterpretation (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 0:
                rqIFD.m_ePhotometricInterpretation = PIT_WHITE_IS_ZERO;
                break;
            case 1:
                rqIFD.m_ePhotometricInterpretation = PIT_BLACK_IS_ZERO;
                break;
            case 2:
                rqIFD.m_ePhotometricInterpretation = PIT_RGB;
                break;
            case 3:
                rqIFD.m_ePhotometricInterpretation = PIT_PALETTED;
                break;
            case 4:
                rqIFD.m_ePhotometricInterpretation = PIT_TRANSPARENCY_MASK;
                break;
            case 5:
                rqIFD.m_ePhotometricInterpretation = PIT_CMYK;
                break;
            case 6:
                rqIFD.m_ePhotometricInterpretation = PIT_YCC;
                break;
            case 8:
                rqIFD.m_ePhotometricInterpretation = PIT_CIE_LAB;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bPhotometricInterpretationFieldIsPresent = true;
        }
        else if ( usTag == 263 )
        {
            // Threshholding
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eThreshholding = TT_NO_DITHERING_OR_HALFTONING_APPLIED;
                break;
            case 2:
                rqIFD.m_eThreshholding = TT_ORDERED_DITHERING_OR_HALFTONING_APPLIED;
                break;
            case 3:
                rqIFD.m_eThreshholding = TT_RANDOMIZED_DITHERING_OR_HALFTONING_APPLIED;
                break;
            default:
                SKIP_FIELD_WITH_THE_WARNING;
            }

            rqIFD.m_bThreshholdingFieldIsPresent = true;
        }
        else if ( usTag == 264 )
        {
            // CellWidth
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            rqValueStream.Read(rqIFD.m_CellWidth);

            rqIFD.m_bCellWidthFieldIsPresent = true;
        }
        else if ( usTag == 265 )
        {
            // CellLength
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            rqValueStream.Read(rqIFD.m_CellLength);

            rqIFD.m_bCellLengthFieldIsPresent = true;
        }
        else if ( usTag == 266 )
        {
            // FillOrder (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eFillOrder = FOT_HIGH_ORDER_BIT_FIRST;
                break;
            case 2:
                rqIFD.m_eFillOrder = FOT_LOW_ORDER_BIT_FIRST;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bFillOrderFieldIsPresent = true;
        }
        else if ( usTag == 269 )
        {
            // DocumentName
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qDocumentName,iCount,rqValueStream));

            rqIFD.m_bDocumentNameFieldIsPresent = true;
        }
        else if ( usTag == 270 )
        {
            // ImageDescription
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qImageDescription,iCount,rqValueStream));

            rqIFD.m_bImageDescriptionFieldIsPresent = true;
        }
        else if ( usTag == 271 )
        {
            // Make
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qMake,iCount,rqValueStream));

            rqIFD.m_bMakeFieldIsPresent = true;
        }
        else if ( usTag == 272 )
        {
            // Model
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qModel,iCount,rqValueStream));

            rqIFD.m_bModelFieldIsPresent = true;
        }
        else if ( usTag == 273 )
        {
            // StripOffsets (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT || eType == FT_LONG ));

            rqIFD.m_qStripOffsets.SetQuantity(iCount);
            if ( eType == FT_SHORT )
            {
                Short Value;
                for (int i1 = 0; i1 < iCount; i1++)
                {
                    rqValueStream.Read(Value);
                    rqIFD.m_qStripOffsets[i1] = Value;
                }
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(iCount,rqIFD.m_qStripOffsets);
            }

            rqIFD.m_bStripOffsetsFieldIsPresent = true;
        }
        else if ( usTag == 274 )
        {
            // Orientation
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eOrientation = OT_TOP_LEFT;
                break;
            case 2:
                rqIFD.m_eOrientation = OT_TOP_RIGHT;
                break;
            case 3:
                rqIFD.m_eOrientation = OT_BOTTOM_RIGHT;
                break;
            case 4:
                rqIFD.m_eOrientation = OT_BOTTOM_LEFT;
                break;
            case 5:
                rqIFD.m_eOrientation = OT_LEFT_TOP;
                break;
            case 6:
                rqIFD.m_eOrientation = OT_RIGHT_TOP;
                break;
            case 7:
                rqIFD.m_eOrientation = OT_RIGHT_BOTTOM;
                break;
            case 8:
                rqIFD.m_eOrientation = OT_LEFT_BOTTOM;
                break;
            default:
                SKIP_FIELD_WITH_THE_WARNING;
            }

            rqIFD.m_bOrientationFieldIsPresent = true;
        }
        else if ( usTag == 277 )
        {
            // SamplesPerPixel (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            rqValueStream.Read(rqIFD.m_SamplesPerPixel);
            CONDITIONAL_ESCAPE_IFD_WITH_WARNING(( 1 <= rqIFD.m_SamplesPerPixel && rqIFD.m_SamplesPerPixel <= 4 ),
                WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_UNSUPPORTED_NUMBER_OF_SAMPLES_PER_PIXEL);

            rqIFD.m_bSamplesPerPixelFieldIsPresent = true;
        }
        else if ( usTag == 278 )
        {
            // RowsPerStrip (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( (eType == FT_SHORT || eType == FT_LONG) && iCount == 1 ));

            if ( eType == FT_SHORT )
            {
                Short Value;
                rqValueStream.Read(Value);
                rqIFD.m_RowsPerStrip = Value;
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(rqIFD.m_RowsPerStrip);
            }
            ESCAPE_IFD_ON_FAIL(( rqIFD.m_RowsPerStrip != 0 ));

            rqIFD.m_bRowsPerStripFieldIsPresent = true;
        }
        else if ( usTag == 279 )
        {
            // StripByteCounts (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT || eType == FT_LONG ));

            rqIFD.m_qStripByteCounts.SetQuantity(iCount);
            if ( eType == FT_SHORT )
            {
                Short Value;
                for (int i1 = 0; i1 < iCount; i1++)
                {
                    rqValueStream.Read(Value);
                    rqIFD.m_qStripByteCounts[i1] = Value;
                }
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(iCount,rqIFD.m_qStripByteCounts);
            }

            rqIFD.m_bStripByteCountsFieldIsPresent = true;
        }
        else if ( usTag == 280 )
        {
            // MinSampleValue
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qMinSampleValue.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qMinSampleValue);

            rqIFD.m_bMinSampleValueFieldIsPresent = true;
        }
        else if ( usTag == 281 )
        {
            // MaxSampleValue
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qMaxSampleValue.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qMaxSampleValue);

            rqIFD.m_bMaxSampleValueFieldIsPresent = true;
        }
        else if ( usTag == 282 )
        {
            // XResolution
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 1 ));

            rqIFD.m_fXResolution = ReadRational(rqValueStream);

            rqIFD.m_bXResolutionFieldIsPresent = true;
        }
        else if ( usTag == 283 )
        {
            // YResolution
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 1 ));

            rqIFD.m_fYResolution = ReadRational(rqValueStream);

            rqIFD.m_bYResolutionFieldIsPresent = true;
        }
        else if ( usTag == 284 )
        {
            // PlanarConfiguration (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_ePlanarConfiguration = PCT_CHUNKY_FORMAT;
                break;
            case 2:
                rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bPlanarConfigurationFieldIsPresent = true;
        }
        else if ( usTag == 285 )
        {
            // PageName
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qPageName,iCount,rqValueStream));

            rqIFD.m_bPageNameFieldIsPresent = true;
        }
        else if ( usTag == 286 )
        {
            // XPosition
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 1 ));

            rqIFD.m_fXPosition = ReadRational(rqValueStream);

            rqIFD.m_bXPositionFieldIsPresent = true;
        }
        else if ( usTag == 287 )
        {
            // YPosition
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 1 ));

            rqIFD.m_fYPosition = ReadRational(rqValueStream);

            rqIFD.m_bYPositionFieldIsPresent = true;
        }
        else if ( usTag == 288 )
        {
            // FreeOffsets
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_LONG ));

            rqIFD.m_qFreeOffsets.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qFreeOffsets);

            rqIFD.m_bFreeOffsetsFieldIsPresent = true;
        }
        else if ( usTag == 289 )
        {
            // FreeByteCounts
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_LONG ));

            rqIFD.m_qFreeByteCounts.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qFreeByteCounts);

            rqIFD.m_bFreeByteCountsFieldIsPresent = true;
        }
        else if ( usTag == 290 )
        {
            // GrayResponseUnit
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eGrayResponseUnit = GRUT_TENTHS_OF_UNIT;
                break;
            case 2:
                rqIFD.m_eGrayResponseUnit = GRUT_HUNDREDTHS_OF_UNIT;
                break;
            case 3:
                rqIFD.m_eGrayResponseUnit = GRUT_THOUSANDTHS_OF_UNIT;
                break;
            case 4:
                rqIFD.m_eGrayResponseUnit = GRUT_TEN_THOUSANDTHS_OF_UNIT;
                break;
            case 5:
                rqIFD.m_eGrayResponseUnit = GRUT_HUNDRED_THOUSANDTHS_OF_UNIT;
                break;
            default:
                SKIP_FIELD_WITH_THE_WARNING;
            }

            rqIFD.m_bGrayResponseUnitFieldIsPresent = true;
        }
        else if ( usTag == 291 )
        {
            // GrayResponseCurve
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qGrayResponseCurve.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qGrayResponseCurve);

            rqIFD.m_bGrayResponseCurveFieldIsPresent = true;
        }
        else if ( usTag == 292 )
        {
            // T4Options (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_LONG && iCount == 1 ));

            Long Value;
            rqValueStream.Read(Value);
            if ( ((Value >> 0) & 1) == 1 )
                rqIFD.m_bT42d = true;
            if ( ((Value >> 1) & 1) == 1 )
                rqIFD.m_bT4UncompressedModeUsed = true;
            if ( ((Value >> 2) & 1) == 1 )
                rqIFD.m_bT4FillBitsBeforeEolUsed = true;

            rqIFD.m_bT4OptionsFieldIsPresent = true;
        }
        else if ( usTag == 293 )
        {
            // T6Options (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_LONG && iCount == 1 ));

            Long Value;
            rqValueStream.Read(Value);
            ESCAPE_IFD_ON_FAIL(( (Value & 1) == 0 ));
            if ( ((Value >> 1) & 1) == 1 )
                rqIFD.m_bT6UncompressedModeAllowed = true;

            rqIFD.m_bT6OptionsFieldIsPresent = true;
        }
        else if ( usTag == 296 )
        {
            // ResolutionUnit
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eResolutionUnit = RUT_NO_ABSOLUTE_UNIT_OF_MEASUREMENT;
                break;
            case 2:
                rqIFD.m_eResolutionUnit = RUT_INCH;
                break;
            case 3:
                rqIFD.m_eResolutionUnit = RUT_CENTIMETER;
                break;
            default:
                SKIP_FIELD_WITH_THE_WARNING;
            }

            rqIFD.m_bResolutionUnitFieldIsPresent = true;
        }
        else if ( usTag == 297 )
        {
            // PageNumber
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 2 ));

            rqValueStream.Read(rqIFD.m_PageNumber);
            rqValueStream.Read(rqIFD.m_TotalPageNumber);

            rqIFD.m_bPageNumberFieldIsPresent = true;
        }
        else if ( usTag == 301 )
        {
            // TransferFunction
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qTransferFunction.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qTransferFunction);

            rqIFD.m_bTransferFunctionFieldIsPresent = true;
        }
        else if ( usTag == 305 )
        {
            // Software
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qSoftware,iCount,rqValueStream));

            rqIFD.m_bSoftwareFieldIsPresent = true;
        }
        else if ( usTag == 306 )
        {
            // DateTime
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII && iCount == 20 ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qDateTime,iCount,rqValueStream));

            rqIFD.m_bDateTimeFieldIsPresent = true;
        }
        else if ( usTag == 315 )
        {
            // Artist
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qArtist,iCount,rqValueStream));

            rqIFD.m_bArtistFieldIsPresent = true;
        }
        else if ( usTag == 316 )
        {
            // HostComputer
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qHostComputer,iCount,rqValueStream));

            rqIFD.m_bHostComputerFieldIsPresent = true;
        }
        else if ( usTag == 317 )
        {
            // Predictor (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_ePredictor = PT_NO_PREDICTION;
                break;
            case 2:
                rqIFD.m_ePredictor = PT_HORIZONTAL_DIFFERENCING;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bPredictorFieldIsPresent = true;
        }
        else if ( usTag == 318 )
        {
            // WhitePoint
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 2 ));

            rqIFD.m_fWhitePointX = ReadRational(rqValueStream);
            rqIFD.m_fWhitePointY = ReadRational(rqValueStream);

            rqIFD.m_bWhitePointFieldIsPresent = true;
        }
        else if ( usTag == 319 )
        {
            // PrimaryChromaticities
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 6 ));

            rqIFD.m_fChromaticityRedX = ReadRational(rqValueStream);
            rqIFD.m_fChromaticityRedY = ReadRational(rqValueStream);
            rqIFD.m_fChromaticityGreenX = ReadRational(rqValueStream);
            rqIFD.m_fChromaticityGreenY = ReadRational(rqValueStream);
            rqIFD.m_fChromaticityBlueX = ReadRational(rqValueStream);
            rqIFD.m_fChromaticityBlueY = ReadRational(rqValueStream);

            rqIFD.m_bPrimaryChromaticitiesFieldIsPresent = true;
        }
        else if ( usTag == 320 )
        {
            // ColorMap (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qColorMap.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qColorMap);

            rqIFD.m_bColorMapFieldIsPresent = true;
        }
        else if ( usTag == 321 )
        {
            // HalftoneHints
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 2 ));

            rqValueStream.Read(rqIFD.m_HighlightGrayLevel);
            rqValueStream.Read(rqIFD.m_ShadowGrayLevel);

            rqIFD.m_bHalftoneHintsFieldIsPresent = true;
        }
        else if ( usTag == 322 )
        {
            // TileWidth (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( (eType == FT_SHORT || eType == FT_LONG) && iCount == 1 ));

            if ( eType == FT_SHORT )
            {
                Short Value;
                rqValueStream.Read(Value);
                rqIFD.m_TileWidth = Value;
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(rqIFD.m_TileWidth);
            }
            ESCAPE_IFD_ON_FAIL(( rqIFD.m_TileWidth != 0 && (rqIFD.m_TileWidth & 15) == 0 ));

            rqIFD.m_bTileWidthFieldIsPresent = true;
        }
        else if ( usTag == 323 )
        {
            // TileLength (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( (eType == FT_SHORT || eType == FT_LONG) && iCount == 1 ));

            if ( eType == FT_SHORT )
            {
                Short Value;
                rqValueStream.Read(Value);
                rqIFD.m_TileLength = Value;
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(rqIFD.m_TileLength);
            }
            ESCAPE_IFD_ON_FAIL(( rqIFD.m_TileLength != 0 && (rqIFD.m_TileLength & 15) == 0 ));

            rqIFD.m_bTileLengthFieldIsPresent = true;
        }
        else if ( usTag == 324 )
        {
            // TileOffsets (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_LONG ));

            rqIFD.m_qTileOffsets.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qTileOffsets);

            rqIFD.m_bTileOffsetsFieldIsPresent = true;
        }
        else if ( usTag == 325 )
        {
            // TileByteCounts (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT || eType == FT_LONG ));

            rqIFD.m_qTileByteCounts.SetQuantity(iCount);
            if ( eType == FT_SHORT )
            {
                Short Value;
                for (int i1 = 0; i1 < iCount; i1++)
                {
                    rqValueStream.Read(Value);
                    rqIFD.m_qTileByteCounts[i1] = Value;
                }
            }
            else  // eType = FT_LONG
            {
                rqValueStream.Read(iCount,rqIFD.m_qTileByteCounts);
            }

            rqIFD.m_bTileByteCountsFieldIsPresent = true;
        }
        else if ( usTag == 332 )
        {
            // InkSet (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eInkSet = IST_CMYK;
                break;
            case 2:
                rqIFD.m_eInkSet = IST_NOT_CMYK;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bInkSetFieldIsPresent = true;
        }
        else if ( usTag == 333 )
        {
            // InkNames (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_ASCII ));

            ESCAPE_IFD_ON_FAIL(ReadString(rqIFD.m_qInkNames,iCount,rqValueStream));

            rqIFD.m_bInkNamesFieldIsPresent = true;
        }
        else if ( usTag == 334 )
        {
            // NumberOfInks (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            rqValueStream.Read(rqIFD.m_NumberOfInks);

            rqIFD.m_bNumberOfInksFieldIsPresent = true;
        }
        else if ( usTag == 336 )
        {
            // DotRange
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_BYTE || eType == FT_SHORT ));

            SKIP_FIELD_ON_FAIL(( (iCount & 1) == 0 ));
            int iCD2 = iCount/2;
            rqIFD.m_qDotRange.SetQuantity(iCD2);
            if ( eType == FT_BYTE )
            {
                Byte Value;
                for (int i1 = 0; i1 < iCD2; i1++)
                {
                    Link<Short,Short>& rqRange = rqIFD.m_qDotRange[i1];

                    rqValueStream.Read(Value);
                    rqRange.First = Value;
                    rqValueStream.Read(Value);
                    rqRange.Second = Value;
                }
            }
            else  // eType = FT_SHORT
            {
                for (int i1 = 0; i1 < iCD2; i1++)
                {
                    Link<Short,Short>& rqRange = rqIFD.m_qDotRange[i1];

                    rqValueStream.Read(rqRange.First);
                    rqValueStream.Read(rqRange.Second);
                }
            }

            rqIFD.m_bDotRangeFieldIsPresent = true;
        }
        else if ( usTag == 337 )
        {
            // TargetPrinter
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qTargetPrinter,iCount,rqValueStream));

            rqIFD.m_bTargetPrinterFieldIsPresent = true;
        }
        else if ( usTag == 338 )
        {
            // ExtraSamples
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qExtraSamples.SetQuantity(iCount);
            Short Value;
            for (int i1 = 0; i1 < iCount; i1++)
            {
                rqValueStream.Read(Value);
                switch ( Value )
                {
                case 0:
                    rqIFD.m_qExtraSamples[i1] = EST_UNSPECIFIED;
                    break;
                case 1:
                    rqIFD.m_qExtraSamples[i1] = EST_ASSOCIATED_ALPHA;
                    break;
                case 2:
                    rqIFD.m_qExtraSamples[i1] = EST_UNASSOCIATED_ALPHA;
                    break;
                default:
                    rqIFD.m_qExtraSamples[i1] = EST_UNASSOCIATED_ALPHA;  // make a concession
                }
            }

            rqIFD.m_bExtraSamplesFieldIsPresent = true;
        }
        else if ( usTag == 339 )
        {
            // SampleFormat (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT ));

            rqIFD.m_qSampleFormat.SetQuantity(iCount);
            Short Value;
            for (int i1 = 0; i1 < iCount; i1++)
            {
                rqValueStream.Read(Value);
                switch ( Value )
                {
                case 1:
                    rqIFD.m_qSampleFormat[i1] = SFT_UNSIGNED_INTEGER;
                    break;
                case 2:
                    rqIFD.m_qSampleFormat[i1] = SFT_SIGNED_INTEGER;
                    break;
                case 3:
                    rqIFD.m_qSampleFormat[i1] = SFT_FLOATING_POINT;
                    break;
                case 4:
                    rqIFD.m_qSampleFormat[i1] = SFT_UNDEFINED;
                    break;
                default:
                    ESCAPE_IFD_WITH_THE_WARNING;
                }
            }
            for (int i1 = 1; i1 < iCount; i1++)
            {
                CONDITIONAL_ESCAPE_IFD_WITH_WARNING(( rqIFD.m_qSampleFormat[i1] == rqIFD.m_qSampleFormat[0] ),
                    WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_SAMPLE_FORMAT_NONUNIFORMITY);
            }

            rqIFD.m_bSampleFormatFieldIsPresent = true;
        }
        else if ( usTag == 340 )
        {
            // SMinSampleValue
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( true ));

            // will need some extra values, so do this
            rqIFD.AssignBitsPerSampleAndSampleFormatDefaults();
            CONDITIONAL_ESCAPE_IFD_WITH_WARNING(rqIFD.AreBitsPerSampleInProperRanges(),
                WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD);

            rqIFD.m_qSMinSampleValueUI.SetQuantity(iCount);
            rqIFD.m_qSMinSampleValueSI.SetQuantity(iCount);
            rqIFD.m_qSMinSampleValueFP.SetQuantity(iCount);
            SampleFormatType eSFT;
            Short SampleBits;
            for (int i1 = 0; i1 < iCount; i1++)
            {
                eSFT = rqIFD.m_qSampleFormat[i1];
                SampleBits = rqIFD.m_qBitsPerSample[i1];

                if ( eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_UNDEFINED )
                {
                    if ( SampleBits <= 8 )
                    {
                        Byte Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMinSampleValueUI[i1] = Value;
                    }
                    else if ( SampleBits <= 16 )
                    {
                        Short Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMinSampleValueUI[i1] = Value;
                    }
                    else  // SampleBits <= 24
                    {
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(rqIFD.m_qSMinSampleValueUI[i1]));
                    }
                }
                else if ( eSFT == SFT_SIGNED_INTEGER )
                {
                    if ( SampleBits <= 8 )
                    {
                        SByte Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMinSampleValueSI[i1] = Value;
                    }
                    else if ( SampleBits <= 16 )
                    {
                        SShort Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMinSampleValueSI[i1] = Value;
                    }
                    else  // SampleBits <= 24
                    {
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(rqIFD.m_qSMinSampleValueSI[i1]));
                    }
                }
                else  // eSFT = SFT_FLOATING_POINT
                {
                    Float Value;
                    SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                    rqIFD.m_qSMinSampleValueFP[i1] = Value;
                }
            }

            rqIFD.m_bSMinSampleValueFieldIsPresent = true;
        }
        else if ( usTag == 341 )
        {
            // SMaxSampleValue
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( true ));

            // will need some extra values, so do this
            rqIFD.AssignBitsPerSampleAndSampleFormatDefaults();
            CONDITIONAL_ESCAPE_IFD_WITH_WARNING(rqIFD.AreBitsPerSampleInProperRanges(),
                WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD);

            rqIFD.m_qSMaxSampleValueUI.SetQuantity(iCount);
            rqIFD.m_qSMaxSampleValueSI.SetQuantity(iCount);
            rqIFD.m_qSMaxSampleValueFP.SetQuantity(iCount);
            SampleFormatType eSFT;
            Short SampleBits;
            for (int i1 = 0; i1 < iCount; i1++)
            {
                eSFT = rqIFD.m_qSampleFormat[i1];
                SampleBits = rqIFD.m_qBitsPerSample[i1];

                if ( eSFT == SFT_UNSIGNED_INTEGER || eSFT == SFT_UNDEFINED )
                {
                    if ( SampleBits <= 8 )
                    {
                        Byte Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMaxSampleValueUI[i1] = Value;
                    }
                    else if ( SampleBits <= 16 )
                    {
                        Short Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMaxSampleValueUI[i1] = Value;
                    }
                    else  // SampleBits <= 24
                    {
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(rqIFD.m_qSMaxSampleValueUI[i1]));
                    }
                }
                else if ( eSFT == SFT_SIGNED_INTEGER )
                {
                    if ( SampleBits <= 8 )
                    {
                        SByte Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMaxSampleValueSI[i1] = Value;
                    }
                    else if ( SampleBits <= 16 )
                    {
                        SShort Value;
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                        rqIFD.m_qSMaxSampleValueSI[i1] = Value;
                    }
                    else  // SampleBits <= 24
                    {
                        SKIP_FIELD_ON_FAIL(rqValueStream.Read(rqIFD.m_qSMaxSampleValueSI[i1]));
                    }
                }
                else  // eSFT = SFT_FLOATING_POINT
                {
                    Float Value;
                    SKIP_FIELD_ON_FAIL(rqValueStream.Read(Value));
                    rqIFD.m_qSMaxSampleValueFP[i1] = Value;
                }
            }

            rqIFD.m_bSMaxSampleValueFieldIsPresent = true;
        }
        else if ( usTag == 342 )
        {
            // TransferRange
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 6 ));

            rqValueStream.Read(rqIFD.m_TransferBlack0);
            rqValueStream.Read(rqIFD.m_TransferWhite0);
            rqValueStream.Read(rqIFD.m_TransferBlack1);
            rqValueStream.Read(rqIFD.m_TransferWhite1);
            rqValueStream.Read(rqIFD.m_TransferBlack2);
            rqValueStream.Read(rqIFD.m_TransferWhite2);

            rqIFD.m_bTransferRangeFieldIsPresent = true;
        }
        else if ( usTag == 347 )
        {
            // JPEGTables (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_UNDEFINED ));

            rqIFD.m_qJPEGTables.SetQuantity(iCount);
            rqValueStream.Read(iCount,rqIFD.m_qJPEGTables);

            rqIFD.m_bJPEGTablesFieldIsPresent = true;
        }
        else if ( usTag == 529 )
        {
            // YCbCrCoefficients (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 3 ));

            rqIFD.m_fYCbCrLumaRed = ReadRational(rqValueStream);
            rqIFD.m_fYCbCrLumaGreen = ReadRational(rqValueStream);
            rqIFD.m_fYCbCrLumaBlue = ReadRational(rqValueStream);

            rqIFD.m_bYCbCrCoefficientsFieldIsPresent = true;
        }
        else if ( usTag == 530 )
        {
            // YCbCrSubSampling (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 2 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eYCbCrSubSamplingHoriz = YCCSST_ONE_TO_ONE;
                break;
            case 2:
                rqIFD.m_eYCbCrSubSamplingHoriz = YCCSST_ONE_TO_TWO;
                break;
            case 4:
                rqIFD.m_eYCbCrSubSamplingHoriz = YCCSST_ONE_TO_FOUR;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eYCbCrSubSamplingVert = YCCSST_ONE_TO_ONE;
                break;
            case 2:
                rqIFD.m_eYCbCrSubSamplingVert = YCCSST_ONE_TO_TWO;
                break;
            case 4:
                rqIFD.m_eYCbCrSubSamplingVert = YCCSST_ONE_TO_FOUR;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bYCbCrSubSamplingFieldIsPresent = true;
        }
        else if ( usTag == 531 )
        {
            // YCbCrPositioning (critical)
            CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(( eType == FT_SHORT && iCount == 1 ));

            Short Value;
            rqValueStream.Read(Value);
            switch ( Value )
            {
            case 1:
                rqIFD.m_eYCbCrPositioning = YCCPT_CENTERED;
                break;
            case 2:
                rqIFD.m_eYCbCrPositioning = YCCPT_COSITED;
                break;
            default:
                ESCAPE_IFD_WITH_THE_WARNING;
            }

            rqIFD.m_bYCbCrPositioningFieldIsPresent = true;
        }
        else if ( usTag == 532 )
        {
            // ReferenceBlackWhite
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_RATIONAL && iCount == 6 ));

            rqIFD.m_fReferenceBlack0 = ReadRational(rqValueStream);
            rqIFD.m_fReferenceWhite0 = ReadRational(rqValueStream);
            rqIFD.m_fReferenceBlack1 = ReadRational(rqValueStream);
            rqIFD.m_fReferenceWhite1 = ReadRational(rqValueStream);
            rqIFD.m_fReferenceBlack2 = ReadRational(rqValueStream);
            rqIFD.m_fReferenceWhite2 = ReadRational(rqValueStream);

            rqIFD.m_bReferenceBlackWhiteFieldIsPresent = true;
        }
        else if ( usTag == 33432 )
        {
            // Copyright
            CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(( eType == FT_ASCII ));

            SKIP_FIELD_ON_FAIL(ReadString(rqIFD.m_qCopyright,iCount,rqValueStream));

            rqIFD.m_bCopyrightFieldIsPresent = true;
        }

MarkerSkipping:
        if ( bValueFitsFourBytes )
        {
            rqFieldStream.SetBytePosition(iBytePosAfterCount+4);
        }
        else if ( pqSeparatedValueStream )
        {
            pqSeparatedValueStream->Finalize();
            delete pqSeparatedValueStream;
            pqSeparatedValueStream = 0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ReadString (StringArray& rqStringArray, int iCount, Stream& rqValueStream)
{
    if ( iCount != 0 )
    {
        String qChars;
        qChars.SetLength(iCount-1);
        rqValueStream.Read(iCount,qChars);
        qChars.Trim();  // necessarily remove possible garbage at the end

        // treating two 0s in row between substrings as incorrectness
        int iSubStringQuantity = 0;
        bool bPrevWasNul = false;
        for (int i = 0; i <= qChars.GetLength(); i++)
        {
            if ( qChars.GetChars()[i] != 0 )
            {
                bPrevWasNul = false;
            }
            else
            {
                if ( !bPrevWasNul )
                {
                    iSubStringQuantity++;
                    bPrevWasNul = true;
                }
                else
                {
                    return false;
                }
            }
        }

        rqStringArray.SetQuantity(iSubStringQuantity);
        int iPrevSL = 0;
        for (int i = 0; i < iSubStringQuantity; i++)
        {
            String& rqString = rqStringArray[i];

            rqString = qChars.GetChars() + iPrevSL;
            iPrevSL += rqString.GetLength() + 1;
            if ( ms_bTrimTexts )
                rqString.Trim();
        }
    }
    return true;
}
//------------------------------------------------------------------------------------------------------------------
float FormatTiff::ReadRational (Stream& rqValueStream)
{
    Long Value0, Value1;
    rqValueStream.Read(Value0);
    rqValueStream.Read(Value1);
    return ((float)Value0)/Value1;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::ExamineIFDFields (ImageFileDirectory& rqIFD)
{
    CRITICAL_TEST(( !(rqIFD.m_eColorModel != CMT_BILEVEL
               &&   (rqIFD.m_eCompression == CT_MODIFIED_HUFFMAN
               ||   rqIFD.m_eCompression == CT_GROUP_3
               ||   rqIFD.m_eCompression == CT_GROUP_4)) ));
    CRITICAL_TEST(( !(rqIFD.m_eCompression == CT_JPEG_TNT
               &&   rqIFD.m_eColorModel != CMT_GRAYSCALE
               &&   rqIFD.m_eColorModel != CMT_CMYK
               &&   rqIFD.m_eColorModel != CMT_YCC
               &&   rqIFD.m_eColorModel != CMT_LAB) ));
    CRITICAL_TEST(( !(rqIFD.m_eCompression == CT_JPEG_TNT
               &&   (!rqIFD.m_bBitsPerSamplesAreTheSame || !rqIFD.m_bPixelSamplesAllAreUI
               ||   rqIFD.m_iBitsPerEachSample != 8)) ));
    CRITICAL_TEST(( !(rqIFD.m_eCompression == CT_JPEG_TNT
               &&   rqIFD.m_ePlanarConfiguration == PCT_PLANAR_FORMAT) ));
    CRITICAL_TEST(( !(rqIFD.m_ePhotometricInterpretation == PIT_PALETTED && (!rqIFD.m_bColorMapFieldIsPresent
               ||   !rqIFD.m_bBitsPerSamplesAreTheSame || !rqIFD.m_bPixelSamplesAllAreUI
               ||   rqIFD.m_qColorMap.GetQuantity() != 3*rqIFD.m_iUIValueRangePerEachSample
               ||   rqIFD.m_iUIValueRangePerEachSample > 256)) ));
    CRITICAL_TEST(( !(rqIFD.m_bAlmostPalettedWithAlpha && rqIFD.m_ePlanarConfiguration == PCT_PLANAR_FORMAT) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   ((rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_ONE)
               ||   (rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_FOUR
               &&   (rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_ONE
               ||   rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_TWO)))) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   ((rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_ImageWidth % 2 != 0)
               ||   (rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_FOUR
               &&   rqIFD.m_ImageWidth % 4 != 0))) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   ((rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_ImageLength % 2 != 0)
               ||   (rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_FOUR
               &&   rqIFD.m_ImageLength % 4 != 0))) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   rqIFD.m_bStripsAreSpecified
               &&   ((rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_iRowsPerStrip % 2 != 0)
               ||   (rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_FOUR
               &&   rqIFD.m_iRowsPerStrip % 4 != 0))) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   rqIFD.m_bTilesAreSpecified
               &&   ((rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_TileWidth % 2 != 0)
               ||   (rqIFD.m_eYCbCrSubSamplingHoriz == YCCSST_ONE_TO_FOUR
               &&   rqIFD.m_TileWidth % 4 != 0))) ));
    CRITICAL_TEST(( !(rqIFD.m_bImageIsYCbCrNonJpeg
               &&   rqIFD.m_bTilesAreSpecified
               &&   ((rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_TWO
               &&   rqIFD.m_TileLength % 2 != 0)
               ||   (rqIFD.m_eYCbCrSubSamplingVert == YCCSST_ONE_TO_FOUR
               &&   rqIFD.m_TileLength % 4 != 0))) ));
    NON_CRITICAL_TEST(( !(rqIFD.m_bMinSampleValueFieldIsPresent
                   &&   (!rqIFD.IsMinSampleAllowed()
                   ||   !(rqIFD.m_qMinSampleValue.GetQuantity() == 1
                   ||   rqIFD.m_qMinSampleValue.GetQuantity() == rqIFD.m_SamplesPerPixel))) ),
                   rqIFD.m_bMinSampleValueFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bMaxSampleValueFieldIsPresent
                   &&   (!rqIFD.IsMaxSampleAllowed()
                   ||   !(rqIFD.m_qMaxSampleValue.GetQuantity() == 1
                   ||   rqIFD.m_qMaxSampleValue.GetQuantity() == rqIFD.m_SamplesPerPixel))) ),
                   rqIFD.m_bMaxSampleValueFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bGrayResponseCurveFieldIsPresent && (!rqIFD.m_bBitsPerSamplesAreTheSame
                   ||   !rqIFD.m_bPixelSamplesAllAreUI
                   ||   rqIFD.m_qGrayResponseCurve.GetQuantity() != rqIFD.m_iUIValueRangePerEachSample)) ),
                   rqIFD.m_bGrayResponseCurveFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bTransferFunctionFieldIsPresent && (!rqIFD.IsTransferFunctionAllowed()
                   ||   !(rqIFD.m_qTransferFunction.GetQuantity() == rqIFD.m_iUIValueRangePerEachSample
                   ||   rqIFD.m_qTransferFunction.GetQuantity() ==
                        3*rqIFD.m_iUIValueRangePerEachSample))) ),rqIFD.m_bTransferFunctionFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bDotRangeFieldIsPresent && (!rqIFD.IsDotRangeAllowed()
                   ||   !(rqIFD.m_qDotRange.GetQuantity() == 1
                   ||   rqIFD.m_qDotRange.GetQuantity() == rqIFD.m_SamplesPerPixel))) ),
                   rqIFD.m_bDotRangeFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bSMinSampleValueFieldIsPresent
                   &&   rqIFD.m_qSMinSampleValueUI.GetQuantity() != rqIFD.m_SamplesPerPixel) ),
                   rqIFD.m_bSMinSampleValueFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bSMaxSampleValueFieldIsPresent
                   &&   rqIFD.m_qSMaxSampleValueUI.GetQuantity() != rqIFD.m_SamplesPerPixel) ),
                   rqIFD.m_bSMaxSampleValueFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bTransferRangeFieldIsPresent && !rqIFD.IsTransferRangeAllowed()) ),
        rqIFD.m_bTransferRangeFieldIsPresent);
    NON_CRITICAL_TEST(( !(rqIFD.m_bReferenceBlackWhiteFieldIsPresent
                   &&   rqIFD.m_ePhotometricInterpretation != PIT_YCC
                   &&   !rqIFD.IsReferenceBlackWhiteAllowed()) ),
                   rqIFD.m_bReferenceBlackWhiteFieldIsPresent);
    CRITICAL_TEST(( !(rqIFD.m_ePredictor == PT_HORIZONTAL_DIFFERENCING
               &&   (!rqIFD.m_bBitsPerSamplesAreTheSame || !rqIFD.m_bPixelSamplesAllAreUI
               ||   !(rqIFD.m_iBitsPerEachSample == 8 || rqIFD.m_iBitsPerEachSample == 16))) ));
    CRITICAL_TEST(( !(rqIFD.m_ePredictor == PT_HORIZONTAL_DIFFERENCING
               &&   rqIFD.m_eCompression != CT_LZW && rqIFD.m_eCompression != CT_ZLIB) ));

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTiff::LoadIFD (RasterImagePtr& rspqImage, const ImageFileDirectory& rqIFD, bool bLoadingTransparencyMask)
{
    // sample precision contest
    int iLargestBPS;
    for (int i = 0; i < rqIFD.m_SamplesPerPixel; i++)
    {
        if ( i == 0 )
            iLargestBPS = rqIFD.m_qBitsPerSample[0];
        else if ( rqIFD.m_qBitsPerSample[i] > iLargestBPS )
            iLargestBPS = rqIFD.m_qBitsPerSample[i];
    }

    // allocate the output image
    RasterImage::Palette qPalette;
    if ( rqIFD.m_eColorModel == CMT_PALETTED || rqIFD.m_bAlmostPalettedWithAlpha )
    {
        qPalette.SetQuantity(rqIFD.m_iUIValueRangePerEachSample);
        for (int i = 0; i < rqIFD.m_iUIValueRangePerEachSample; i++)
        {
            qPalette[i] = PaletteColor(
                Mathf::RoundToInt(((float)rqIFD.m_qColorMap[0*rqIFD.m_iUIValueRangePerEachSample+i])/65535*255),
                Mathf::RoundToInt(((float)rqIFD.m_qColorMap[1*rqIFD.m_iUIValueRangePerEachSample+i])/65535*255),
                Mathf::RoundToInt(((float)rqIFD.m_qColorMap[2*rqIFD.m_iUIValueRangePerEachSample+i])/65535*255));
        }
    }
    if ( rqIFD.m_eColorModel == CMT_BILEVEL )
    {
        RasterImage::Palette qPalette(2);
        qPalette[0] = PaletteColor(0,0,0);
        qPalette[1] = PaletteColor(255,255,255);
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,rqIFD.m_ImageWidth,rqIFD.m_ImageLength,qPalette);
    }
    else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE )
    {
        if ( iLargestBPS <= 8 )
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
        else if ( iLargestBPS <= 16 )
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_16,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
        else  // 16 < iLargestBPS and iLargestBPS <= 24
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_FLOAT,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
    }
    else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
    {
        if ( iLargestBPS <= 8 )
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else if ( iLargestBPS <= 16 )
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else  // 16 < iLargestBPS and iLargestBPS <= 24
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
    }
    else if ( rqIFD.m_eColorModel == CMT_RGB || rqIFD.m_eColorModel == CMT_YCC )
    {
        if ( iLargestBPS <= 8 )
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        else if ( iLargestBPS <= 16 )
            rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        else  // 16 < iLargestBPS and iLargestBPS <= 24
            rspqImage = new RasterImage(RasterImage::IT_RGB_FLOAT,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
    }
    else if ( rqIFD.m_eColorModel == CMT_RGBA )
    {
        if ( iLargestBPS <= 8 || rqIFD.m_bAlmostPalettedWithAlpha )
        {
            rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
        else if ( iLargestBPS <= 16 )
        {
            rspqImage = new RasterImage(RasterImage::IT_RGBA_16_16_16_16,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else  // 16 < iLargestBPS and iLargestBPS <= 24
        {
            rspqImage = new RasterImage(RasterImage::IT_RGBA_FLOAT,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
    }
    else if ( rqIFD.m_eColorModel == CMT_PALETTED )
    {
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,rqIFD.m_ImageWidth,rqIFD.m_ImageLength,qPalette);
    }
    else if ( rqIFD.m_eColorModel == CMT_CMYK )
    {
        if ( iLargestBPS <= 8 )
        {
            rspqImage = new RasterImage(RasterImage::IT_CMYK_8_8_8_8,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
        else if ( iLargestBPS <= 16 )
        {
            rspqImage = new RasterImage(RasterImage::IT_CMYK_16_16_16_16,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else  // 16 < iLargestBPS and iLargestBPS <= 24
        {
            rspqImage = new RasterImage(RasterImage::IT_CMYK_FLOAT,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        }
    }
    else if ( rqIFD.m_eColorModel == CMT_LAB )
    {
        if ( iLargestBPS <= 8 )
            rspqImage = new RasterImage(RasterImage::IT_LAB_8_8_8,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        else if ( iLargestBPS <= 16 )
            rspqImage = new RasterImage(RasterImage::IT_LAB_16_16_16,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
        else  // 16 < iLargestBPS and iLargestBPS <= 24
            rspqImage = new RasterImage(RasterImage::IT_LAB_FLOAT,rqIFD.m_ImageWidth,rqIFD.m_ImageLength);
    }
    else  // rqIFD.m_eColorModel = CMT_LAB_WITH_ALPHA
    {
        if ( iLargestBPS <= 8 )
        {
            rspqImage = new RasterImage(RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else if ( iLargestBPS <= 16 )
        {
            rspqImage = new RasterImage(RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
        else  // 16 < iLargestBPS and iLargestBPS <= 24
        {
            rspqImage = new RasterImage(RasterImage::IT_LAB_WITH_ALPHA_FLOAT,rqIFD.m_ImageWidth,
                rqIFD.m_ImageLength);
        }
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // Process strips/tiles.

    // Allocate the fax compression object: Huffman's tree is generated once per TIFF file.  For fax encoding,
    // ignore m_bT4FillBitsBeforeEolUsed (fill bits are 0s) because CompressionFax processes EOLs in the manner
    // "skip all consecutive 0-bits and one 1-bit after".
    CompressionFaxPtr spqFax;
    if ( rqIFD.m_eCompression == CT_MODIFIED_HUFFMAN )
    {
        spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_1D_TIFF,false);
    }
    else if ( rqIFD.m_eCompression == CT_GROUP_3 )
    {
        if ( !rqIFD.m_bT42d )
        {
            // 1-dimensional
            spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_1D,false);
        }
        else
        {
            // 2-dimensional.  Ignore m_bT4UncompressedModeUsed since TIFF specification isn't quite clear
            // about usage of uncompressed mode in Group 3 2D encoding, assume CCITT Rec. T.4 approach which is
            // by default.
            spqFax = new CompressionFax(CompressionFax::ET_GROUP_3_2D,false);
        }
    }
    else if ( rqIFD.m_eCompression == CT_GROUP_4 )
    {
        spqFax = new CompressionFax(CompressionFax::ET_GROUP_4,false);
        if ( rqIFD.m_bT6UncompressedModeAllowed )
            spqFax->SetUncompressedModeEncoding(CompressionFax::UMET_TIFF);
    }

    bool bSuccess;
    int iBytePos, iBitRatio, iValue, iCurrCompPlane, iPaletteSize, iSubSFHoriz, iSubSFVert, iSSFVMO, iChromaW,
        iChromaH, iR, iG, iB, iValue0, iValue1;
    unsigned short usValue, usValue0, usValue1, usValue2, usValue3;
    float fValue, fValue0, fValue1, fValue2, fValue3;
    double dValue, dValue0, dValue1, dValue2, dValue3;
    unsigned char* pucRow;
    unsigned char* aucRow0;
    unsigned char* aucRow1;
    unsigned char* aucRow2;
    unsigned char* pucComp;
    unsigned char* pucComp0;
    unsigned char* pucComp1;
    unsigned char* pucComp2;
    unsigned char* aucData;
    char* pcComp;
    StillArray<int> qCrInR, qCbInG, qCrInG, qCbInB;

    // YCbCr related
    if ( rqIFD.m_bImageIsYCbCrNonJpeg )
    {
        // Prepare YCbCr (non-JPEG).
        switch ( rqIFD.m_eYCbCrSubSamplingHoriz )
        {
        case YCCSST_ONE_TO_ONE:
            iSubSFHoriz = 1;
            break;
        case YCCSST_ONE_TO_TWO:
            iSubSFHoriz = 2;
            break;
        case YCCSST_ONE_TO_FOUR:
            iSubSFHoriz = 4;
            break;
        }
        switch ( rqIFD.m_eYCbCrSubSamplingVert )
        {
        case YCCSST_ONE_TO_ONE:
            iSubSFVert = 1;
            break;
        case YCCSST_ONE_TO_TWO:
            iSubSFVert = 2;
            break;
        case YCCSST_ONE_TO_FOUR:
            iSubSFVert = 4;
            break;
        }
        iSSFVMO = iSubSFVert - 1;

        // rely on the default or supplied RGB space parameters (Kr, Kg, Kb)
        float fCrInR = 2.0f*(1.0f - rqIFD.m_fYCbCrLumaRed);
        float fCbInG = 2.0f*(rqIFD.m_fYCbCrLumaBlue*rqIFD.m_fYCbCrLumaBlue - rqIFD.m_fYCbCrLumaBlue)/
            rqIFD.m_fYCbCrLumaGreen;
        float fCrInG = 2.0f*(rqIFD.m_fYCbCrLumaRed*rqIFD.m_fYCbCrLumaRed - rqIFD.m_fYCbCrLumaRed)/
            rqIFD.m_fYCbCrLumaGreen;
        float fCbInB = 2.0f*(1.0f - rqIFD.m_fYCbCrLumaBlue);
        qCrInR.SetQuantity(256);
        qCbInG.SetQuantity(256);
        qCrInG.SetQuantity(256);
        qCbInB.SetQuantity(256);
        for (int i0 = 0, i1 = -128; i0 < 256; i0++, i1++)
        {
            qCrInR[i0] = Mathf::RoundToInt(fCrInR*i1);
            qCbInG[i0] = Mathf::RoundToInt(fCbInG*i1);
            qCrInG[i0] = Mathf::RoundToInt(fCrInG*i1);
            qCbInB[i0] = Mathf::RoundToInt(fCbInB*i1);
        }
    }

    // process segments
    float fDonePerSegment = 1.0f/rqIFD.m_qSegments.GetQuantity();
    float fHalfDonePerSegment = fDonePerSegment/2;
    for (int i0 = 0; i0 < rqIFD.m_qSegments.GetQuantity(); i0++)
    {
        const SegmentRecord& rqSegment = rqIFD.m_qSegments[i0];

        StillArray<char> qCompressedData;
        Stream qSegmentStream(m_eStreaming,m_acStreamSrcParam,rqSegment.Offset,rqSegment.ByteCount,m_bLittleEndian,
            bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qCompressedData.SetQuantity(rqSegment.ByteCount);
        MemCpy(qCompressedData,qSegmentStream,rqSegment.ByteCount);
        qSegmentStream.Finalize();

        if ( rqIFD.m_eFillOrder == FOT_LOW_ORDER_BIT_FIRST )
        {
            // reverse bit orders within bytes
            aucData = (unsigned char*)qCompressedData.GetElements();
            for (int i1 = 0; i1 < (int)rqSegment.ByteCount; i1++)
                aucData[i1] = Storage::ReverseBitOrder[aucData[i1]];
        }

        // unpack
        StillArray<char> qDecompressedData;
        if ( rqIFD.m_eCompression == CT_NONE )
        {
            ESCAPE_ON_FAIL(( rqSegment.ByteCount == rqSegment.UncompressedByteCount ));
            qDecompressedData.SetQuantity(rqSegment.ByteCount);
            MemCpy(qDecompressedData,qCompressedData,rqSegment.ByteCount);
            qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
        }
        else if ( rqIFD.m_eCompression == CT_MODIFIED_HUFFMAN
             ||   rqIFD.m_eCompression == CT_GROUP_3
             ||   rqIFD.m_eCompression == CT_GROUP_4 )
        {
            spqFax->SetImageDimensions(rqSegment.Width,rqSegment.Length);
            spqFax->RedirectProgressTo(this,qProgress,fHalfDonePerSegment);
            ESCAPE_ON_FAIL(spqFax->Decompress(qCompressedData,rqSegment.ByteCount,qDecompressedData));
        }
        else if ( rqIFD.m_eCompression == CT_LZW )
        {
            CompressionLzw qLzw(CompressionLzw::ET_TIFF);
            qLzw.RedirectProgressTo(this,qProgress,fHalfDonePerSegment);
            ESCAPE_ON_FAIL(qLzw.Decompress(qCompressedData,rqSegment.ByteCount,qDecompressedData));
        }
        else if ( rqIFD.m_eCompression == CT_JPEG_TNT )
        {
            // must begin with SOI
            ESCAPE_ON_FAIL(( qCompressedData[0] == (char)255 && qCompressedData[1] == (char)216 ));

            FormatJpegPtr spqJpeg;
            RasterImagePtr spqJpegImage;
            StillArray<char> qJpegDataStream;
            if ( rqIFD.m_bJPEGTablesFieldIsPresent )
            {
                // Imitate JPEG data stream.  The next lines satisfy the TIFF Technical Note #2 requirement
                // that "local" tables can redefine "global" tables for a single segment only.
                int iActTablesSize = rqIFD.m_qJPEGTables.GetQuantity() - 4;
                qJpegDataStream.SetQuantity(iActTablesSize+rqSegment.ByteCount);

                // write SOI
                qJpegDataStream[0] = (char)255;
                qJpegDataStream[1] = (char)216;

                // write table(s)
                MemCpy(qJpegDataStream+2,rqIFD.m_qJPEGTables+2,iActTablesSize);

                // and JPEG image itself
                MemCpy(qJpegDataStream+2+iActTablesSize,qCompressedData+2,rqSegment.ByteCount-2);

                spqJpeg = new FormatJpeg(true,qJpegDataStream,qJpegDataStream.GetQuantity(),bSuccess);
            }
            else
            {
                // the segment must have its own tables
                spqJpeg = new FormatJpeg(true,qCompressedData,rqSegment.ByteCount,bSuccess);
            }
            ESCAPE_ON_FAIL(bSuccess);
            ESCAPE_ON_FAIL(( !(rqIFD.m_eColorModel == CMT_GRAYSCALE
                        &&   spqJpeg->GetColorModel() != FormatJpeg::CMT_GRAYSCALE) ));
            ESCAPE_ON_FAIL(( !(rqIFD.m_eColorModel == CMT_CMYK
                        &&   spqJpeg->GetColorModel() != FormatJpeg::CMT_CMYK) ));
            ESCAPE_ON_FAIL(( !(rqIFD.m_eColorModel == CMT_YCC
                        &&   spqJpeg->GetColorModel() != FormatJpeg::CMT_YCC) ));
            ESCAPE_ON_FAIL(( !(rqIFD.m_eColorModel == CMT_LAB
                        &&   spqJpeg->GetColorModel() != FormatJpeg::CMT_RGB) ));
            ESCAPE_ON_FAIL(( rqSegment.Width == spqJpeg->GetWidth()
                        &&   rqSegment.Length == spqJpeg->GetHeight() ));
            spqJpeg->RedirectProgressTo(this,qProgress,fDonePerSegment);
            ESCAPE_ON_FAIL(spqJpeg->LoadImage(spqJpegImage));

            if ( rqIFD.m_eColorModel == CMT_CMYK )
            {
                // invert CMYK components
                pucRow = (unsigned char*)spqJpegImage->GetImage();
                pucComp = pucRow;
                for (int iY = 0; iY < spqJpegImage->GetHeight(); iY++)
                {
                    for (int iX = 0; iX < spqJpegImage->GetWidth(); iX++)
                    {
                        for (int i1 = 0; i1 < 4; i1++)
                            *pucComp++ = 255 - *pucComp;
                    }

                    pucRow += spqJpegImage->GetRowStride();
                    pucComp = pucRow;
                }
            }
            else if ( rqIFD.m_eColorModel == CMT_LAB )
            {
                // tune CIE L*a*b* components
                RasterImagePtr spqTunedImage = new RasterImage(RasterImage::IT_LAB_8_8_8,rqSegment.Width,
                    rqSegment.Length);
                aucRow0 = (unsigned char*)spqJpegImage->GetImage();
                pucComp0 = aucRow0;
                aucRow1 = (unsigned char*)spqTunedImage->GetImage();
                pucComp1 = aucRow1;
                for (int iY = 0; iY < spqJpegImage->GetHeight(); iY++)
                {
                    for (int iX = 0; iX < spqJpegImage->GetWidth(); iX++)
                    {
                        *pucComp1++ = *pucComp0++;
                        *pucComp1++ = ((char)*pucComp0++) + 128;
                        *pucComp1++ = ((char)*pucComp0++) + 128;
                    }

                    aucRow0 += spqJpegImage->GetRowStride();
                    pucComp0 = aucRow0;
                    aucRow1 += spqTunedImage->GetRowStride();
                    pucComp1 = aucRow1;
                }
                spqJpegImage = spqTunedImage;
            }

            // segment image fitting in
            if ( !rqSegment.SubimageRectInnerIsMeaning )
            {
                rspqImage->SetSubimage(spqJpegImage,
                    rqSegment.SubimageRectOuter.X,
                    rqSegment.SubimageRectOuter.Y);
            }
            else
            {
                rspqImage->SetSubimage(spqJpegImage->GetSubimage(rqSegment.SubimageRectInner),
                    rqSegment.SubimageRectOuter.X,
                    rqSegment.SubimageRectOuter.Y);
            }

            // that's all for this segment
            continue;
        }
        else if ( rqIFD.m_eCompression == CT_ZLIB )
        {
            CompressionZLib qZLib;
            CONDITIONAL_THROW_EXCEPTION(qZLib.Decompress(qCompressedData,rqSegment.ByteCount,qDecompressedData),
                ET_ZLIB_EXCEPTION);
            qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
        }
        else  // rqIFD.m_eCompression = CT_RLE
        {
            ESCAPE_ON_FAIL(CompressionPackBits::Decompress(qCompressedData,rqSegment.ByteCount,qDecompressedData));
            qProgress.MakeOneTimeAdvance(fHalfDonePerSegment);
        }
        ESCAPE_ON_FAIL(( qDecompressedData.GetQuantity() == rqSegment.UncompressedByteCount ));
        qCompressedData.RemoveAll();

        if ( rqIFD.m_ePredictor == PT_HORIZONTAL_DIFFERENCING )
        {
            // unfilter
            UnfilterSegmentImage(rqSegment,qDecompressedData,rqIFD);
        }

        qProgress.SetAdvance(fHalfDonePerSegment/rqSegment.Length);

        // assign pixels
        iBytePos = 0;
        iBitRatio = 0;
        if ( rqIFD.m_eColorModel == CMT_BILEVEL )
        {
            RasterImage::Palette qPalette(2);
            qPalette[0] = PaletteColor(0,0,0);
            qPalette[1] = PaletteColor(255,255,255);

            for (int iY = 0; iY < (int)rqSegment.Length; iY++)
            {
                for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                {
                    iBytePos += Storage::ReadUBits(qDecompressedData+iBytePos,iBitRatio,1,iValue);

                    if ( !(rqSegment.SubimageRectInnerIsMeaning
                    &&   (iX >= rqSegment.SubimageRectInner.Width
                    ||   iY >= rqSegment.SubimageRectInner.Height)) )
                    {
                        pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X + iX;
                        if ( rqIFD.m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO )
                            *pcComp = ( iValue == 0 ? 1 : 0 );
                        else  // rqIFD.m_ePhotometricInterpretation = PIT_BLACK_IS_ZERO
                            *pcComp = ( iValue == 0 ? 0 : 1 );
                    }
                }

                if ( iBitRatio != 0 )
                {
                    iBytePos++;
                    iBitRatio = 0;
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE )
        {
            for (int iY = 0; iY < (int)rqSegment.Length; iY++)
            {
                for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                {
                    dValue = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                    if ( !(rqSegment.SubimageRectInnerIsMeaning
                    &&   (iX >= rqSegment.SubimageRectInner.Width
                    ||   iY >= rqSegment.SubimageRectInner.Height)) )
                    {
                        if ( rqIFD.m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO )
                            dValue = 1.0 - dValue;

                        if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_8 )
                        {
                            pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                rqSegment.SubimageRectOuter.X + iX;
                            *pcComp = Mathd::RoundToInt(dValue*255);
                        }
                        else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_16 )
                        {
                            pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                (rqSegment.SubimageRectOuter.X + iX)*2;
                            usValue = Mathd::RoundToInt(dValue*65535);
                            MemCpy(pcComp,&usValue,2);
                        }
                        else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_FLOAT
                        {
                            pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                (rqSegment.SubimageRectOuter.X + iX)*4;
                            fValue = (float)dValue;
                            MemCpy(pcComp,&fValue,4);
                        }
                    }
                }

                if ( iBitRatio != 0 )
                {
                    iBytePos++;
                    iBitRatio = 0;
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
        {
            if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                // interleaved component distribution
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rqIFD.m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO )
                                dValue0 = 1.0 - dValue0;

                            if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*2;
                                pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                pcComp[1] = Mathd::RoundToInt(dValue1*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4;
                                usValue0 = Mathd::RoundToInt(dValue0*65535);
                                usValue1 = Mathd::RoundToInt(dValue1*65535);
                                MemCpy(pcComp+0,&usValue0,2);
                                MemCpy(pcComp+2,&usValue1,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8;
                                fValue0 = (float)dValue0;
                                fValue1 = (float)dValue1;
                                MemCpy(pcComp+0,&fValue0,4);
                                MemCpy(pcComp+4,&fValue1,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                // planar component distribution
                iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*2 + iCurrCompPlane;
                                *pcComp = Mathd::RoundToInt(dValue*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4 + iCurrCompPlane*2;
                                usValue = Mathd::RoundToInt(dValue*65535);
                                MemCpy(pcComp,&usValue,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8 + iCurrCompPlane*4;
                                fValue = (float)dValue;
                                MemCpy(pcComp,&fValue,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_RGB )
        {
            if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                // interleaved component distribution
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue2 = ReadSample(2,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*3;
                                pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                pcComp[1] = Mathd::RoundToInt(dValue1*255);
                                pcComp[2] = Mathd::RoundToInt(dValue2*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_RGB_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*6;
                                usValue0 = Mathd::RoundToInt(dValue0*65535);
                                usValue1 = Mathd::RoundToInt(dValue1*65535);
                                usValue2 = Mathd::RoundToInt(dValue2*65535);
                                MemCpy(pcComp+0,&usValue0,2);
                                MemCpy(pcComp+2,&usValue1,2);
                                MemCpy(pcComp+4,&usValue2,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_RGB_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*12;
                                fValue0 = (float)dValue0;
                                fValue1 = (float)dValue1;
                                fValue2 = (float)dValue2;
                                MemCpy(pcComp+0,&fValue0,4);
                                MemCpy(pcComp+4,&fValue1,4);
                                MemCpy(pcComp+8,&fValue2,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                // planar component distribution
                iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*3 + iCurrCompPlane;
                                *pcComp = Mathd::RoundToInt(dValue*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_RGB_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*6 + iCurrCompPlane*2;
                                usValue = Mathd::RoundToInt(dValue*65535);
                                MemCpy(pcComp,&usValue,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_RGB_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*12 + iCurrCompPlane*4;
                                fValue = (float)dValue;
                                MemCpy(pcComp,&fValue,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_RGBA )
        {
            if ( !rqIFD.m_bAlmostPalettedWithAlpha )
            {
                if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
                {
                    // interleaved component distribution
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                            dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                            dValue2 = ReadSample(2,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                            dValue3 = ReadSample(3,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                            if ( !(rqSegment.SubimageRectInnerIsMeaning
                            &&   (iX >= rqSegment.SubimageRectInner.Width
                            ||   iY >= rqSegment.SubimageRectInner.Height)) )
                            {
                                if ( rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 )
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*4;
                                    pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                    pcComp[1] = Mathd::RoundToInt(dValue1*255);
                                    pcComp[2] = Mathd::RoundToInt(dValue2*255);
                                    pcComp[3] = Mathd::RoundToInt(dValue3*255);
                                }
                                else if ( rspqImage->GetType() == RasterImage::IT_RGBA_16_16_16_16 )
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*8;
                                    usValue0 = Mathd::RoundToInt(dValue0*65535);
                                    usValue1 = Mathd::RoundToInt(dValue1*65535);
                                    usValue2 = Mathd::RoundToInt(dValue2*65535);
                                    usValue3 = Mathd::RoundToInt(dValue3*65535);
                                    MemCpy(pcComp+0,&usValue0,2);
                                    MemCpy(pcComp+2,&usValue1,2);
                                    MemCpy(pcComp+4,&usValue2,2);
                                    MemCpy(pcComp+6,&usValue3,2);
                                }
                                else  // rspqImage->GetType() = RasterImage::IT_RGBA_FLOAT
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*16;
                                    fValue0 = (float)dValue0;
                                    fValue1 = (float)dValue1;
                                    fValue2 = (float)dValue2;
                                    fValue3 = (float)dValue3;
                                    MemCpy(pcComp+ 0,&fValue0,4);
                                    MemCpy(pcComp+ 4,&fValue1,4);
                                    MemCpy(pcComp+ 8,&fValue2,4);
                                    MemCpy(pcComp+12,&fValue3,4);
                                }
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
                {
                    // planar component distribution
                    iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                    for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                    {
                        for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                        {
                            dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                            if ( !(rqSegment.SubimageRectInnerIsMeaning
                            &&   (iX >= rqSegment.SubimageRectInner.Width
                            ||   iY >= rqSegment.SubimageRectInner.Height)) )
                            {
                                if ( rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 )
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*4 + iCurrCompPlane;
                                    *pcComp = Mathd::RoundToInt(dValue*255);
                                }
                                else if ( rspqImage->GetType() == RasterImage::IT_RGBA_16_16_16_16 )
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*8 + iCurrCompPlane*2;
                                    usValue = Mathd::RoundToInt(dValue*65535);
                                    MemCpy(pcComp,&usValue,2);
                                }
                                else  // rspqImage->GetType() = RasterImage::IT_RGBA_FLOAT
                                {
                                    pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                        (rqSegment.SubimageRectOuter.X + iX)*16 + iCurrCompPlane*4;
                                    fValue = (float)dValue;
                                    MemCpy(pcComp,&fValue,4);
                                }
                            }
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else
            {
                // for interleaved component distribution only
                iPaletteSize = rqIFD.m_qColorMap.GetQuantity()/3;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        iBytePos += Storage::ReadUBits(qDecompressedData+iBytePos,iBitRatio,
                            rqIFD.m_qBitsPerSample[0],iValue0);
                        iBytePos += Storage::ReadUBits(qDecompressedData+iBytePos,iBitRatio,
                            rqIFD.m_qBitsPerSample[1],iValue1);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            ESCAPE_ON_FAIL(( iValue0 < iPaletteSize ));
                            pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                (rqSegment.SubimageRectOuter.X + iX)*4;
                            const PaletteColor& rqPaletteColor = qPalette[iValue0];
                            pcComp[0] = rqPaletteColor.R();
                            pcComp[1] = rqPaletteColor.G();
                            pcComp[2] = rqPaletteColor.B();
                            pcComp[3] = iValue1;
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_PALETTED )
        {
            iPaletteSize = rqIFD.m_qColorMap.GetQuantity()/3;
            for (int iY = 0; iY < (int)rqSegment.Length; iY++)
            {
                for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                {
                    iBytePos += Storage::ReadUBits(qDecompressedData+iBytePos,iBitRatio,rqIFD.m_iBitsPerEachSample,
                        iValue);

                    if ( !(rqSegment.SubimageRectInnerIsMeaning
                    &&   (iX >= rqSegment.SubimageRectInner.Width
                    ||   iY >= rqSegment.SubimageRectInner.Height)) )
                    {
                        ESCAPE_ON_FAIL(( iValue < iPaletteSize ));
                        pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            rqSegment.SubimageRectOuter.X + iX;
                        *pcComp = iValue;
                    }
                }

                if ( iBitRatio != 0 )
                {
                    iBytePos++;
                    iBitRatio = 0;
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_CMYK )
        {
            if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                // interleaved component distribution
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue2 = ReadSample(2,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue3 = ReadSample(3,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_CMYK_8_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4;
                                pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                pcComp[1] = Mathd::RoundToInt(dValue1*255);
                                pcComp[2] = Mathd::RoundToInt(dValue2*255);
                                pcComp[3] = Mathd::RoundToInt(dValue3*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_CMYK_16_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8;
                                usValue0 = Mathd::RoundToInt(dValue0*65535);
                                usValue1 = Mathd::RoundToInt(dValue1*65535);
                                usValue2 = Mathd::RoundToInt(dValue2*65535);
                                usValue3 = Mathd::RoundToInt(dValue3*65535);
                                MemCpy(pcComp+0,&usValue0,2);
                                MemCpy(pcComp+2,&usValue1,2);
                                MemCpy(pcComp+4,&usValue2,2);
                                MemCpy(pcComp+6,&usValue3,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_CMYK_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*16;
                                fValue0 = (float)dValue0;
                                fValue1 = (float)dValue1;
                                fValue2 = (float)dValue2;
                                fValue3 = (float)dValue3;
                                MemCpy(pcComp+ 0,&fValue0,4);
                                MemCpy(pcComp+ 4,&fValue1,4);
                                MemCpy(pcComp+ 8,&fValue2,4);
                                MemCpy(pcComp+12,&fValue3,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                // planar component distribution
                iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_CMYK_8_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4 + iCurrCompPlane;
                                *pcComp = Mathd::RoundToInt(dValue*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_CMYK_16_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8 + iCurrCompPlane*2;
                                usValue = Mathd::RoundToInt(dValue*65535);
                                MemCpy(pcComp,&usValue,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_CMYK_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*16 + iCurrCompPlane*4;
                                fValue = (float)dValue;
                                MemCpy(pcComp,&fValue,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_YCC )
        {
            // for interleaved component distribution only
            iChromaW = rqSegment.Width/iSubSFHoriz;
            iChromaH = rqSegment.Length/iSubSFVert;
            RasterImagePtr aspqComponentImages[3] = {
                new RasterImage(RasterImage::IT_GRAYSCALE_8,rqSegment.Width,rqSegment.Length),
                new RasterImage(RasterImage::IT_GRAYSCALE_8,iChromaW,iChromaH),
                new RasterImage(RasterImage::IT_GRAYSCALE_8,iChromaW,iChromaH)};
            Link<int,int> aqCoords[3] = {
                MakeLink(0,0),
                MakeLink(0,0),
                MakeLink(0,0)};
            for (/**/; /**/; /**/)
            {
                // Y
                for (int i1 = 0; i1 < iSubSFVert; i1++)
                {
                    for (int i2 = 0; i2 < iSubSFHoriz; i2++)
                    {
                        dValue = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        pcComp = aspqComponentImages[0]->GetRow(aqCoords[0].Second) + aqCoords[0].First;
                        *pcComp = Mathd::RoundToInt(dValue*255);

                        aqCoords[0].First++;
                    }

                    if ( i1 != iSSFVMO )
                        aqCoords[0].First -= iSubSFHoriz;
                    aqCoords[0].Second++;
                }

                // Cb and Cr
                for (int i1 = 1; i1 < 3; i1++)
                {
                    dValue = ReadSample(i1,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                    pcComp = aspqComponentImages[i1]->GetRow(aqCoords[i1].Second) + aqCoords[i1].First;
                    *pcComp = Mathd::RoundToInt(dValue*255);
                    if ( ++aqCoords[i1].First == iChromaW )
                    {
                        aqCoords[i1].First = 0;
                        aqCoords[i1].Second++;
                    }
                }

                if ( aqCoords[0].First != rqSegment.Width || aqCoords[0].Second != rqSegment.Length )
                {
                    // to the next block
                    if ( aqCoords[0].First != rqSegment.Width )
                        aqCoords[0].Second -= iSubSFVert;
                    else
                        aqCoords[0].First = 0;
                }
                else
                {
                    break;
                }
            }

            // resample
            for (int i1 = 1; i1 < 3; i1++)
            {
                RasterImageTransformation qRIT(aspqComponentImages[i1]);
                aspqComponentImages[i1] = qRIT.GetResampled(rqSegment.Width,rqSegment.Length,
                    RasterImageTransformation::IT_BILINEAR);
            }

            aucRow0 = (unsigned char*)aspqComponentImages[0]->GetImage();
            aucRow1 = (unsigned char*)aspqComponentImages[1]->GetImage();
            aucRow2 = (unsigned char*)aspqComponentImages[2]->GetImage();
            pucComp0 = aucRow0;
            pucComp1 = aucRow1;
            pucComp2 = aucRow2;
            for (int iY = 0; iY < (int)rqSegment.Length; iY++)
            {
                for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                {
                    if ( !(rqSegment.SubimageRectInnerIsMeaning
                    &&   (iX >= rqSegment.SubimageRectInner.Width
                    ||   iY >= rqSegment.SubimageRectInner.Height)) )
                    {
                        iR = *pucComp0 + qCrInR[(int)*pucComp2];
                        iG = *pucComp0 + qCbInG[(int)*pucComp1] + qCrInG[(int)*pucComp2];
                        iB = *pucComp0 + qCbInB[(int)*pucComp1];
                        if ( iR < 0 )
                            iR = 0;
                        else if ( iR > 255 )
                            iR = 255;
                        if ( iG < 0 )
                            iG = 0;
                        else if ( iG > 255 )
                            iG = 255;
                        if ( iB < 0 )
                            iB = 0;
                        else if ( iB > 255 )
                            iB = 255;

                        pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                            (rqSegment.SubimageRectOuter.X + iX)*3;
                        *(pcComp + 0) = iR;
                        *(pcComp + 1) = iG;
                        *(pcComp + 2) = iB;
                    }

                    pucComp0++;
                    pucComp1++;
                    pucComp2++;
                }

                aucRow0 += aspqComponentImages[0]->GetRowStride();
                aucRow1 += aspqComponentImages[1]->GetRowStride();
                aucRow2 += aspqComponentImages[2]->GetRowStride();
                pucComp0 = aucRow0;
                pucComp1 = aucRow1;
                pucComp2 = aucRow2;

                qProgress.MakeAdvance();
            }
        }
        else if ( rqIFD.m_eColorModel == CMT_LAB )
        {
            if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                // interleaved component distribution
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue2 = ReadSample(2,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_LAB_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*3;
                                pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                pcComp[1] = (char)Mathd::RoundToInt(dValue1*255) + 128;
                                pcComp[2] = (char)Mathd::RoundToInt(dValue2*255) + 128;
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_LAB_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*6;
                                usValue0 = Mathd::RoundToInt(dValue0*65535);
                                usValue1 = (short)Mathd::RoundToInt(dValue1*65535) + 32768;
                                usValue2 = (short)Mathd::RoundToInt(dValue2*65535) + 32768;
                                MemCpy(pcComp+0,&usValue0,2);
                                MemCpy(pcComp+2,&usValue1,2);
                                MemCpy(pcComp+4,&usValue2,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_LAB_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*12;
                                fValue0 = (float)(dValue0*100);
                                fValue1 = (float)(dValue1*255 - 128);
                                fValue2 = (float)(dValue2*255 - 128);
                                MemCpy(pcComp+0,&fValue0,4);
                                MemCpy(pcComp+4,&fValue1,4);
                                MemCpy(pcComp+8,&fValue2,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                // planar component distribution
                iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_LAB_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*3 + iCurrCompPlane;
                                if ( iCurrCompPlane == 0 )
                                    *pcComp = Mathd::RoundToInt(dValue*255);
                                else
                                    *pcComp = (char)Mathd::RoundToInt(dValue*255) + 128;
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_LAB_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*6 + iCurrCompPlane*2;
                                if ( iCurrCompPlane == 0 )
                                    usValue = Mathd::RoundToInt(dValue*65535);
                                else
                                    usValue = (short)Mathd::RoundToInt(dValue*65535) + 32768;
                                MemCpy(pcComp,&usValue,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_LAB_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*12 + iCurrCompPlane*4;
                                if ( iCurrCompPlane == 0 )
                                    fValue = (float)(dValue*100);
                                else
                                    fValue = (float)(dValue*255 - 128);
                                MemCpy(pcComp,&fValue,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // rqIFD.m_eColorModel = CMT_LAB_WITH_ALPHA
        {
            if ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT )
            {
                // interleaved component distribution
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue0 = ReadSample(0,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue1 = ReadSample(1,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue2 = ReadSample(2,qDecompressedData,rqIFD,iBytePos,iBitRatio);
                        dValue3 = ReadSample(3,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4;
                                pcComp[0] = Mathd::RoundToInt(dValue0*255);
                                pcComp[1] = (char)Mathd::RoundToInt(dValue1*255) + 128;
                                pcComp[2] = (char)Mathd::RoundToInt(dValue2*255) + 128;
                                pcComp[3] = Mathd::RoundToInt(dValue3*255);
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8;
                                usValue0 = Mathd::RoundToInt(dValue0*65535);
                                usValue1 = (short)Mathd::RoundToInt(dValue1*65535) + 32768;
                                usValue2 = (short)Mathd::RoundToInt(dValue2*65535) + 32768;
                                usValue3 = Mathd::RoundToInt(dValue3*65535);
                                MemCpy(pcComp+0,&usValue0,2);
                                MemCpy(pcComp+2,&usValue1,2);
                                MemCpy(pcComp+4,&usValue2,2);
                                MemCpy(pcComp+6,&usValue3,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_LAB_WITH_ALPHA_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*16;
                                fValue0 = (float)(dValue0*100);
                                fValue1 = (float)(dValue1*255 - 128);
                                fValue2 = (float)(dValue2*255 - 128);
                                fValue3 = (float)dValue3;
                                MemCpy(pcComp+ 0,&fValue0,4);
                                MemCpy(pcComp+ 4,&fValue1,4);
                                MemCpy(pcComp+ 8,&fValue2,4);
                                MemCpy(pcComp+12,&fValue3,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // rqIFD.m_ePlanarConfiguration = PCT_PLANAR_FORMAT
            {
                // planar component distribution
                iCurrCompPlane = i0/rqIFD.m_iSegmentsPerPlane;
                for (int iY = 0; iY < (int)rqSegment.Length; iY++)
                {
                    for (int iX = 0; iX < (int)rqSegment.Width; iX++)
                    {
                        dValue = ReadSample(iCurrCompPlane,qDecompressedData,rqIFD,iBytePos,iBitRatio);

                        if ( !(rqSegment.SubimageRectInnerIsMeaning
                        &&   (iX >= rqSegment.SubimageRectInner.Width
                        ||   iY >= rqSegment.SubimageRectInner.Height)) )
                        {
                            if ( rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*4 + iCurrCompPlane;
                                if ( iCurrCompPlane == 0 || iCurrCompPlane == 3 )
                                    *pcComp = Mathd::RoundToInt(dValue*255);
                                else
                                    *pcComp = (char)Mathd::RoundToInt(dValue*255) + 128;
                            }
                            else if ( rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16 )
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*8 + iCurrCompPlane*2;
                                if ( iCurrCompPlane == 0 || iCurrCompPlane == 3 )
                                    usValue = Mathd::RoundToInt(dValue*65535);
                                else
                                    usValue = (short)Mathd::RoundToInt(dValue*65535) + 32768;
                                MemCpy(pcComp,&usValue,2);
                            }
                            else  // rspqImage->GetType() = RasterImage::IT_LAB_WITH_ALPHA_FLOAT
                            {
                                pcComp = rspqImage->GetRow(rqSegment.SubimageRectOuter.Y+iY) +
                                    (rqSegment.SubimageRectOuter.X + iX)*16 + iCurrCompPlane*4;
                                if ( iCurrCompPlane == 0 )
                                    fValue = (float)(dValue*100);
                                else if ( iCurrCompPlane != 3 )
                                    fValue = (float)(dValue*255 - 128);
                                else
                                    fValue = (float)dValue;
                                MemCpy(pcComp,&fValue,4);
                            }
                        }
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

    // if the image has alpha channel, free pixel components out from alpha component/coefficient
    if ( rqIFD.m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA
    &&   rqIFD.m_qExtraSamples[0] == EST_ASSOCIATED_ALPHA )
    {
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    if ( pucComp[1] != 0 )
                    {
                        iValue = Mathf::RoundToInt(((float)pucComp[0])/pucComp[1]*255);
                        if ( iValue > 255 )
                            iValue = 255;
                        pucComp[0] = iValue;
                    }
                    else
                    {
                        pucComp[0] = 255;
                    }

                    pucComp += 2;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&usValue0,pucComp+0,2);
                    MemCpy(&usValue1,pucComp+2,2);
                    if ( usValue1 != 0 )
                    {
                        iValue = Mathf::RoundToInt(((float)usValue0)/usValue1*65535);
                        if ( iValue > 65535 )
                            iValue = 65535;
                        usValue0 = iValue;
                    }
                    else
                    {
                        usValue0 = 65535;
                    }
                    MemCpy(pucComp,&usValue0,2);

                    pucComp += 4;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&fValue0,pucComp+0,4);
                    MemCpy(&fValue1,pucComp+4,4);
                    if ( fValue1 > Mathf::ZERO_TOLERANCE )
                    {
                        fValue0 /= fValue1;
                        if ( fValue0 > 1.0f )
                            fValue0 = 1.0f;
                    }
                    else
                    {
                        fValue0 = 1.0f;
                    }
                    MemCpy(pucComp,&fValue0,4);

                    pucComp += 8;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
    }
    else if ( (rqIFD.m_eColorModel == CMT_RGBA || rqIFD.m_eColorModel == CMT_LAB_WITH_ALPHA)
         &&   rqIFD.m_qExtraSamples[0] == EST_ASSOCIATED_ALPHA )
    {
        int iValue0, iValue1, iValue2;
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        if ( rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8
        ||   rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    if ( pucComp[3] != 0 )
                    {
                        iValue0 = Mathf::RoundToInt(((float)pucComp[0])/pucComp[3]*255);
                        iValue1 = Mathf::RoundToInt(((float)pucComp[1])/pucComp[3]*255);
                        iValue2 = Mathf::RoundToInt(((float)pucComp[2])/pucComp[3]*255);
                        if ( iValue0 > 255 )
                            iValue0 = 255;
                        if ( iValue1 > 255 )
                            iValue1 = 255;
                        if ( iValue2 > 255 )
                            iValue2 = 255;
                        pucComp[0] = iValue0;
                        pucComp[1] = iValue1;
                        pucComp[2] = iValue2;
                    }
                    else
                    {
                        pucComp[0] = 255;
                        pucComp[1] = 255;
                        pucComp[2] = 255;
                    }

                    pucComp += 4;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else if ( rspqImage->GetType() == RasterImage::IT_RGBA_16_16_16_16
             ||   rspqImage->GetType() == RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&usValue0,pucComp+0,2);
                    MemCpy(&usValue1,pucComp+2,2);
                    MemCpy(&usValue2,pucComp+4,2);
                    MemCpy(&usValue3,pucComp+6,2);
                    if ( usValue3 != 0 )
                    {
                        iValue0 = Mathf::RoundToInt(((float)usValue0)/usValue3*65535);
                        iValue1 = Mathf::RoundToInt(((float)usValue1)/usValue3*65535);
                        iValue2 = Mathf::RoundToInt(((float)usValue2)/usValue3*65535);
                        if ( iValue0 > 65535 )
                            iValue0 = 65535;
                        if ( iValue1 > 65535 )
                            iValue1 = 65535;
                        if ( iValue2 > 65535 )
                            iValue2 = 65535;
                        usValue0 = iValue0;
                        usValue1 = iValue1;
                        usValue2 = iValue2;
                    }
                    else
                    {
                        usValue0 = 65535;
                        usValue1 = 65535;
                        usValue2 = 65535;
                    }
                    MemCpy(pucComp+0,&usValue0,2);
                    MemCpy(pucComp+2,&usValue1,2);
                    MemCpy(pucComp+4,&usValue2,2);

                    pucComp += 8;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else if ( rspqImage->GetType() == RasterImage::IT_RGBA_FLOAT )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&fValue0,pucComp+ 0,4);
                    MemCpy(&fValue1,pucComp+ 4,4);
                    MemCpy(&fValue2,pucComp+ 8,4);
                    MemCpy(&fValue3,pucComp+12,4);
                    if ( fValue3 > Mathf::ZERO_TOLERANCE )
                    {
                        fValue0 /= fValue3;
                        fValue1 /= fValue3;
                        fValue2 /= fValue3;
                        if ( fValue0 > 1.0f )
                            fValue0 = 1.0f;
                        if ( fValue1 > 1.0f )
                            fValue1 = 1.0f;
                        if ( fValue2 > 1.0f )
                            fValue2 = 1.0f;
                    }
                    else
                    {
                        fValue0 = 1.0f;
                        fValue1 = 1.0f;
                        fValue2 = 1.0f;
                    }
                    MemCpy(pucComp+0,&fValue0,4);
                    MemCpy(pucComp+4,&fValue1,4);
                    MemCpy(pucComp+8,&fValue2,4);

                    pucComp += 16;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else  // rspqImage->GetType() = IT_LAB_WITH_ALPHA_FLOAT
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&fValue0,pucComp+ 0,4);
                    MemCpy(&fValue1,pucComp+ 4,4);
                    MemCpy(&fValue2,pucComp+ 8,4);
                    MemCpy(&fValue3,pucComp+12,4);
                    if ( fValue3 > Mathf::ZERO_TOLERANCE )
                    {
                        fValue0 /= fValue3;
                        fValue1 /= fValue3;
                        fValue2 /= fValue3;
                        if ( fValue0 > 100.0f )
                            fValue0 = 100.0f;
                    }
                    else
                    {
                        fValue0 = 100.0f;
                        fValue1 = ( fValue1 < 0.0f ? -128.0f : 127.0f );
                        fValue2 = ( fValue2 < 0.0f ? -128.0f : 127.0f );
                    }
                    MemCpy(pucComp+0,&fValue0,4);
                    MemCpy(pucComp+4,&fValue1,4);
                    MemCpy(pucComp+8,&fValue2,4);

                    pucComp += 16;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
    }

    if ( rqIFD.m_ePlanarConfiguration == PCT_PLANAR_FORMAT && rqIFD.m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA
    &&   rqIFD.m_ePhotometricInterpretation == PIT_WHITE_IS_ZERO )
    {
        // invert the intensity component (couldn't do this before due to component separation with possibility
        // of associated alpha)
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    *pucComp = 255 - *pucComp;
                    pucComp += 2;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16 )
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&usValue,pucComp,2);
                    usValue = 65535 - usValue;
                    MemCpy(pucComp,&usValue,2);

                    pucComp += 4;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
        else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT
        {
            for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                {
                    MemCpy(&fValue,pucComp,4);
                    fValue = 1.0f - fValue;
                    MemCpy(pucComp,&fValue,4);

                    pucComp += 8;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;
            }
        }
    }

    if ( !bLoadingTransparencyMask && m_spqTransparencyMaskIFD )
    {
        // process the TIFF transparency mask
        RasterImagePtr spqTransparencyMaskImage;
        bSuccess = false;
        if ( LoadIFD(spqTransparencyMaskImage,*m_spqTransparencyMaskIFD,true) )
        {
            if ( spqTransparencyMaskImage->GetWidth() == rspqImage->GetWidth()
            &&   spqTransparencyMaskImage->GetHeight() == rspqImage->GetHeight() )
            {
                RasterImage::VisibilityMask qVisibilityMask(rspqImage->GetWidth()*rspqImage->GetHeight());
                pucRow = (unsigned char*)spqTransparencyMaskImage->GetImage();
                pucComp = pucRow;
                for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
                {
                    for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
                    {
                        qVisibilityMask[iY*rspqImage->GetWidth()+iX] = ( *pucComp == 0 ? 0 : 1 );
                        pucComp++;
                    }

                    pucRow += spqTransparencyMaskImage->GetRowStride();
                    pucComp = pucRow;
                }
                rspqImage->SetVisibilityMask(qVisibilityMask);
                bSuccess = true;
            }
        }
        if ( !bSuccess )
            THROW_WARNING(WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD);
    }

    // rely on orientation information
    if ( rqIFD.m_eOrientation == OT_TOP_RIGHT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedHorizontally();
    }
    else if ( rqIFD.m_eOrientation == OT_BOTTOM_RIGHT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated180();
    }
    else if ( rqIFD.m_eOrientation == OT_BOTTOM_LEFT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( rqIFD.m_eOrientation == OT_LEFT_TOP )
    {
        {
            RasterImageTransformation qRIT(rspqImage);
            rspqImage = qRIT.GetRotated90Ccw();
        }
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( rqIFD.m_eOrientation == OT_RIGHT_TOP )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated90Ccw();
    }
    else if ( rqIFD.m_eOrientation == OT_RIGHT_BOTTOM )
    {
        {
            RasterImageTransformation qRIT(rspqImage);
            rspqImage = qRIT.GetRotated90Cw();
        }
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( rqIFD.m_eOrientation == OT_LEFT_BOTTOM )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated90Cw();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
double FormatTiff::ReadSample (int iSamplePos, const StillArray<char>& rqDecompressedData,
    const ImageFileDirectory& rqIFD, int& riBytePos, int& riBitRatio) const
{
    if ( rqIFD.m_bTypicalCaseForPixelSamples )
        return ((double)((unsigned char)rqDecompressedData[riBytePos++]))/255;

    double dValue;
    if ( rqIFD.m_qSampleFormat[iSamplePos] != SFT_FLOATING_POINT )
    {
        bool bUnsigned = ( rqIFD.m_qSampleFormat[iSamplePos] == SFT_UNSIGNED_INTEGER
                      ||   rqIFD.m_qSampleFormat[iSamplePos] == SFT_UNDEFINED );
        int iValue;
        if ( rqIFD.m_qBitsPerSample[iSamplePos] != 16 )
        {
            if ( bUnsigned )
            {
                riBytePos += Storage::ReadUBits(rqDecompressedData+riBytePos,riBitRatio,
                    rqIFD.m_qBitsPerSample[iSamplePos],iValue);
            }
            else
            {
                riBytePos += Storage::ReadSBits(rqDecompressedData+riBytePos,riBitRatio,
                    rqIFD.m_qBitsPerSample[iSamplePos],iValue);
            }
        }
        else
        {
            unsigned short usValue;
            if ( m_bLittleEndian )
                riBytePos += Storage::Read2le(rqDecompressedData+riBytePos,1,&usValue);
            else
                riBytePos += Storage::Read2be(rqDecompressedData+riBytePos,1,&usValue);
            iValue = ( bUnsigned ? usValue : (short)usValue );
        }

        if ( bUnsigned )
        {
            if ( iValue < (int)rqIFD.m_qSMinSampleValueUI[iSamplePos] )
                iValue = rqIFD.m_qSMinSampleValueUI[iSamplePos];
            if ( iValue > (int)rqIFD.m_qSMaxSampleValueUI[iSamplePos] )
                iValue = rqIFD.m_qSMaxSampleValueUI[iSamplePos];

            dValue = ((double)(iValue - rqIFD.m_qSMinSampleValueUI[iSamplePos]))/
                rqIFD.m_qSampleSTopValuesUI[iSamplePos];
        }
        else
        {
            if ( iValue < (int)rqIFD.m_qSMinSampleValueSI[iSamplePos] )
                iValue = rqIFD.m_qSMinSampleValueSI[iSamplePos];
            if ( iValue > (int)rqIFD.m_qSMaxSampleValueSI[iSamplePos] )
                iValue = rqIFD.m_qSMaxSampleValueSI[iSamplePos];

            dValue = ((double)(iValue - rqIFD.m_qSMinSampleValueSI[iSamplePos]))/
                rqIFD.m_qSampleSTopValuesSI[iSamplePos];
        }
    }
    else
    {
        float fValue;
        if ( m_bLittleEndian )
            riBytePos += Storage::Read4le(rqDecompressedData+riBytePos,1,&fValue);
        else
            riBytePos += Storage::Read4be(rqDecompressedData+riBytePos,1,&fValue);

        if ( fValue < rqIFD.m_qSMinSampleValueFP[iSamplePos] )
            fValue = rqIFD.m_qSMinSampleValueFP[iSamplePos];
        if ( fValue > rqIFD.m_qSMaxSampleValueFP[iSamplePos] )
            fValue = rqIFD.m_qSMaxSampleValueFP[iSamplePos];

        dValue = (fValue - rqIFD.m_qSMinSampleValueFP[iSamplePos])/rqIFD.m_qSampleSTopValuesFP[iSamplePos];
    }

    if ( dValue < 0.0 )
        dValue = 0.0;
    else if ( dValue > 1.0 )
        dValue = 1.0;

    return dValue;
}
//------------------------------------------------------------------------------------------------------------------
void FormatTiff::UnfilterSegmentImage (const SegmentRecord& rqSegment, StillArray<char>& rqDecompressedData,
    const ImageFileDirectory& rqIFD) const
{
    // assuming that bits per each sample are the same and equal to 8 or 16
    int iUsedSPP = ( rqIFD.m_ePlanarConfiguration == PCT_CHUNKY_FORMAT ? rqIFD.m_SamplesPerPixel : 1 );
    int iPixelSize = ( rqIFD.m_iBitsPerEachSample == 8 ? iUsedSPP : 2*iUsedSPP );
    int iWidthInComps = rqSegment.Width*iUsedSPP;
    int iRowStride = rqSegment.Width*iPixelSize;
    char* acRow = rqDecompressedData;
    char* pcComp0 = acRow;
    char* pcComp1 = acRow + iPixelSize;
    if ( rqIFD.m_iBitsPerEachSample == 8 )
    {
        for (int iY = 0; iY < (int)rqSegment.Length; iY++)
        {
            for (int iX = iUsedSPP; iX < iWidthInComps; iX++)
                *pcComp1++ += *pcComp0++;

            acRow += iRowStride;
            pcComp0 = acRow;
            pcComp1 = acRow + iPixelSize;
        }
    }
    else  // rqIFD.m_iBitsPerEachSample = 16
    {
        short sValue0, sValue1;
        for (int iY = 0; iY < (int)rqSegment.Length; iY++)
        {
            for (int iX = iUsedSPP; iX < iWidthInComps; iX++)
            {
                if ( m_bLittleEndian )
                {
                    Storage::Read2le(pcComp0,1,&sValue0);
                    Storage::Read2le(pcComp1,1,&sValue1);
                }
                else
                {
                    Storage::Read2be(pcComp0,1,&sValue0);
                    Storage::Read2be(pcComp1,1,&sValue1);
                }
                sValue1 += sValue0;
                if ( m_bLittleEndian )
                    Storage::Write2le(pcComp1,1,&sValue1);
                else
                    Storage::Write2be(pcComp1,1,&sValue1);

                pcComp0 += 2;
                pcComp1 += 2;
            }

            acRow += iRowStride;
            pcComp0 = acRow;
            pcComp1 = acRow + iPixelSize;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



