#include "QFormatDds.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatDds.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QCompressionDxt.h"
#include "QRasterImageTransformation.h"
#include "QRasterImageConversion.h"
using namespace Q;

// DDS constants
enum
{
    FDDS_CAPS = 0x00000001,
    FDDS_HEIGHT = 0x00000002,
    FDDS_WIDTH = 0x00000004,
    FDDS_PITCH = 0x00000008,
    FDDS_PIXELFORMAT = 0x00001000,
    FDDS_MIPMAPCOUNT = 0x00020000,
    FDDS_LINEARSIZE = 0x00080000,
    FDDS_DEPTH = 0x00800000,
    FDDS_ALPHAPIXELS = 0x00000001,
    FDDS_FOURCC = 0x00000004,
    FDDS_RGB = 0x00000040,
    FDDS_COMPLEX = 0x00000008,
    FDDS_TEXTURE = 0x00001000,
    FDDS_MIPMAP = 0x00400000,
    FDDS_CUBEMAP = 0x00000200,
    FDDS_POSITIVEX = 0x00000400,
    FDDS_NEGATIVEX = 0x00000800,
    FDDS_POSITIVEY = 0x00001000,
    FDDS_NEGATIVEY = 0x00002000,
    FDDS_POSITIVEZ = 0x00004000,
    FDDS_NEGATIVEZ = 0x00008000,
    FDDS_VOLUME = 0x00200000
};
static const char* gs_acDds = "DDS ";
static const char* gs_acDxt1 = "DXT1";
static const char* gs_acDxt2 = "DXT2";
static const char* gs_acDxt3 = "DXT3";
static const char* gs_acDxt4 = "DXT4";
static const char* gs_acDxt5 = "DXT5";

//------------------------------------------------------------------------------------------------------------------
FormatDds::FormatDds (const RasterImagePtr& rspq2DTexture, PackingType ePacking, int iResolutionLevelQuantity,
    StillArray<RasterImagePtr>* pqCustomMipmaps)
    :
    m_eResource(RT_2D_TEXTURE),
    m_spq2DTextureSaveTo(rspq2DTexture),
    m_ePacking(ePacking),
    m_iMainImageWidth(rspq2DTexture->GetWidth()),
    m_iMainImageHeight(rspq2DTexture->GetHeight()),
    m_bHasCustomMipmaps(( pqCustomMipmaps != 0 ))
{
    assert( iResolutionLevelQuantity == -1 || iResolutionLevelQuantity >= 1 );

    // perform shared among resource types asserts and assignations
    InitAndPrepareForSaving(rspq2DTexture,iResolutionLevelQuantity);

    if ( pqCustomMipmaps )
    {
#ifdef _DEBUG
        assert( pqCustomMipmaps->GetQuantity() == iResolutionLevelQuantity - 1 );
        int iW = rspq2DTexture->GetWidth();
        int iH = rspq2DTexture->GetHeight();
        int iI = 0;
        while ( !(iW == 1 && iH == 1) )
        {
            if ( iW != 1 )
                iW /= 2;
            if ( iH != 1 )
                iH /= 2;
            assert( (*pqCustomMipmaps)[iI]->GetWidth() == iW
                &&  (*pqCustomMipmaps)[iI]->GetHeight() == iH );
            if ( ++iI == pqCustomMipmaps->GetQuantity() )
                break;
        }
#endif

        m_qCustomMipmaps = *pqCustomMipmaps;
    }
}
//------------------------------------------------------------------------------------------------------------------
FormatDds::FormatDds (
    const RasterImagePtr& rspqFacePositiveX, const RasterImagePtr& rspqFaceNegativeX,
    const RasterImagePtr& rspqFacePositiveY, const RasterImagePtr& rspqFaceNegativeY,
    const RasterImagePtr& rspqFacePositiveZ, const RasterImagePtr& rspqFaceNegativeZ,
    PackingType ePacking, int iResolutionLevelQuantity, const StillArray<RasterImagePtr> aqCustomMipmaps[6])
    :
    m_eResource(RT_CUBIC_ENVIRONMENT_MAP),
    m_ePacking(ePacking),
    m_iMainImageWidth(rspqFacePositiveX->GetWidth()),
    m_iMainImageHeight(rspqFacePositiveX->GetHeight()),
    m_bHasCustomMipmaps(( aqCustomMipmaps != 0 ))
{
    assert( iResolutionLevelQuantity == -1 || iResolutionLevelQuantity >= 1 );

    m_aspqCubicEnvMapSaveTo[0] = rspqFacePositiveX;
    m_aspqCubicEnvMapSaveTo[1] = rspqFaceNegativeX;
    m_aspqCubicEnvMapSaveTo[2] = rspqFacePositiveY;
    m_aspqCubicEnvMapSaveTo[3] = rspqFaceNegativeY;
    m_aspqCubicEnvMapSaveTo[4] = rspqFacePositiveZ;
    m_aspqCubicEnvMapSaveTo[5] = rspqFaceNegativeZ;

#ifdef _DEBUG
    int iSide = rspqFacePositiveX->GetWidth();
    assert( rspqFacePositiveX->GetHeight() == iSide );
    for (int i = 1; i < 6; i++)
    {
        assert( m_aspqCubicEnvMapSaveTo[i]->GetWidth() == iSide );
        assert( m_aspqCubicEnvMapSaveTo[i]->GetHeight() == iSide );
    }
#endif

    // perform shared among resource types asserts and assignations
    InitAndPrepareForSaving(rspqFacePositiveX,iResolutionLevelQuantity);

    if ( aqCustomMipmaps )
    {
#ifdef _DEBUG
        int iW, iH, iI, iRLQMO = iResolutionLevelQuantity - 1;
        for (int i = 0; i < 6; i++)
        {
            assert( aqCustomMipmaps[i].GetQuantity() == iRLQMO );
            iW = rspqFacePositiveX->GetWidth();
            iH = rspqFacePositiveX->GetHeight();
            iI = 0;
            while ( !(iW == 1 && iH == 1) )
            {
                if ( iW != 1 )
                    iW /= 2;
                if ( iH != 1 )
                    iH /= 2;
                assert( aqCustomMipmaps[i][iI]->GetWidth() == iW
                    &&  aqCustomMipmaps[i][iI]->GetHeight() == iH );
                if ( ++iI == aqCustomMipmaps[i].GetQuantity() )
                    break;
            }
        }
#endif

        for (int i = 0; i < 6; i++)
            m_aqCEMCustomMipmaps[i] = aqCustomMipmaps[i];
    }
}
//------------------------------------------------------------------------------------------------------------------
FormatDds::FormatDds (const StillArray<RasterImagePtr>& rqSlices, PackingType ePacking,
    StillArray<RasterImagePtr>* pqCustomMipmaps)
    :
    m_eResource(RT_VOLUME_TEXTURE),
    m_qSlices(rqSlices),
    m_ePacking(ePacking),
    m_iMainImageWidth(rqSlices.GetFirst()->GetWidth()),
    m_iMainImageHeight(rqSlices.GetFirst()->GetHeight()),
    m_iVolumeTextureDepth(rqSlices.GetQuantity()),
    m_bHasCustomMipmaps(( pqCustomMipmaps != 0 ))
{
#ifdef _DEBUG
    assert( !rqSlices.IsEmpty() && IsPowerOfTwo(rqSlices.GetQuantity()) );
    int iWidth = rqSlices.GetFirst()->GetWidth();
    int iHeight = rqSlices.GetFirst()->GetHeight();
    assert( iWidth != 0 && iHeight != 0
        &&  IsPowerOfTwo(iWidth) && IsPowerOfTwo(iHeight) );
    for (int i = 1; i < rqSlices.GetQuantity(); i++)
    {
        assert( rqSlices[i]->GetWidth() == iWidth );
        assert( rqSlices[i]->GetHeight() == iHeight );
    }
#endif

    // volume-texture number of resolution levels
    int iResolutionLevelQuantity = ( !pqCustomMipmaps ? 0 : pqCustomMipmaps->GetQuantity() );

    // perform shared among resource types asserts and assignations
    InitAndPrepareForSaving(rqSlices.GetFirst(),iResolutionLevelQuantity);

    if ( pqCustomMipmaps )
    {
#ifdef _DEBUG
        int iW = rqSlices.GetFirst()->GetWidth();
        int iH = rqSlices.GetFirst()->GetHeight();
        int iD = m_iVolumeTextureDepth;
        int iMM = 0;
        while ( !(iW == 1 && iH == 1) )
        {
            if ( iW != 1 )
                iW /= 2;
            if ( iH != 1 )
                iH /= 2;
            if ( iD != 1 )
                iD /= 2;
            for (int i = 0; i < iD; i++)
            {
                assert( (*pqCustomMipmaps)[iMM]->GetWidth() == iW
                    &&  (*pqCustomMipmaps)[iMM]->GetHeight() == iH );
                if ( ++iMM == pqCustomMipmaps->GetQuantity() )
                    goto Exit;
            }
        }
Exit:
#endif

        m_qCustomMipmaps = *pqCustomMipmaps;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::SaveFile (const char* acFilename)
{
    char cZero = 0;

    Stream qSaveStream(acFilename,true);

    // signature
    qSaveStream.Write(4,gs_acDds);

    unsigned int uiSize;

    // Surface Format Header.
    uiSize = 124;
    qSaveStream.Write(uiSize);

    unsigned int uiHeaderFlags = 0;
    uiHeaderFlags |= FDDS_HEIGHT;
    uiHeaderFlags |= FDDS_WIDTH;
    uiHeaderFlags |= FDDS_PIXELFORMAT;
    uiHeaderFlags |= FDDS_CAPS;
    if ( m_bHasNonFullResImages )
        uiHeaderFlags |= FDDS_MIPMAPCOUNT;
    if ( m_eResource == RT_VOLUME_TEXTURE )
        uiHeaderFlags |= FDDS_DEPTH;
    if ( m_eCompression == CT_NONE )
        uiHeaderFlags |= FDDS_PITCH;
    else  // DXT compression
        uiHeaderFlags |= FDDS_LINEARSIZE;
    qSaveStream.Write(uiHeaderFlags);

    unsigned int uiHeight = m_iMainImageHeight;
    unsigned int uiWidth = m_iMainImageWidth;
    qSaveStream.Write(uiHeight);
    qSaveStream.Write(uiWidth);

    unsigned int uiPitchOrLinearSize;
    if ( m_eCompression == CT_NONE )
        uiPitchOrLinearSize = m_iMainImageWidth*m_iBitsPerPixel/8;
    else  // DXT compression
        uiPitchOrLinearSize = CalculateImageOccupancy(m_iMainImageWidth,m_iMainImageHeight);
    if ( m_eResource == RT_VOLUME_TEXTURE )
        uiPitchOrLinearSize *= m_iVolumeTextureDepth;
    qSaveStream.Write(uiPitchOrLinearSize);

    unsigned int uiDepth = ( m_eResource == RT_VOLUME_TEXTURE ? m_iVolumeTextureDepth : 0 );
    qSaveStream.Write(uiDepth);

    unsigned int uiMipmapCount = m_iResolutionLevelQuantity;
    qSaveStream.Write(uiMipmapCount);

    for (int i = 0; i < 44; i++)
        qSaveStream.Write(cZero);

    uiSize = 32;
    qSaveStream.Write(uiSize);

    unsigned int uiPixelFormatFlags = 0;
    if ( !m_bDxtCompression )
    {
        // no compression
        uiPixelFormatFlags |= FDDS_RGB;
        if ( m_eColorModel == CMT_RGBA )
            uiPixelFormatFlags |= FDDS_ALPHAPIXELS;
    }
    else
    {
        // DXT compression
        uiPixelFormatFlags |= FDDS_FOURCC;
    }
    qSaveStream.Write(uiPixelFormatFlags);

    switch ( m_eCompression )
    {
    case CT_NONE:
        for (int i = 0; i < 4; i++)
            qSaveStream.Write(cZero);
        break;
    case CT_DXT_1:
        qSaveStream.Write(4,gs_acDxt1);
        break;
    case CT_DXT_2:
        qSaveStream.Write(4,gs_acDxt2);
        break;
    case CT_DXT_3:
        qSaveStream.Write(4,gs_acDxt3);
        break;
    case CT_DXT_4:
        qSaveStream.Write(4,gs_acDxt4);
        break;
    case CT_DXT_5:
        qSaveStream.Write(4,gs_acDxt5);
        break;
    }

    unsigned int uiRgbBitCount = m_iBitsPerPixel;
    qSaveStream.Write(uiRgbBitCount);

    qSaveStream.Write(m_uiRedBitMask);
    qSaveStream.Write(m_uiGreenBitMask);
    qSaveStream.Write(m_uiBlueBitMask);

    qSaveStream.Write(m_uiAlphaBitMask);

    unsigned int uiCaps1 = 0;
    uiCaps1 |= FDDS_TEXTURE;
    if ( m_bHasNonFullResImages )
        uiCaps1 |= FDDS_MIPMAP;
    if ( m_bHasNonFullResImages
    ||   m_eResource == RT_CUBIC_ENVIRONMENT_MAP
    ||   m_eResource == RT_VOLUME_TEXTURE )
    {
        uiCaps1 |= FDDS_COMPLEX;
    }
    qSaveStream.Write(uiCaps1);

    unsigned int uiCaps2 = 0;
    if ( m_eResource == RT_CUBIC_ENVIRONMENT_MAP )
    {
        uiCaps2 |= FDDS_CUBEMAP;
        uiCaps2 |= FDDS_POSITIVEX;
        uiCaps2 |= FDDS_NEGATIVEX;
        uiCaps2 |= FDDS_POSITIVEY;
        uiCaps2 |= FDDS_NEGATIVEY;
        uiCaps2 |= FDDS_POSITIVEZ;
        uiCaps2 |= FDDS_NEGATIVEZ;
    }
    if ( m_eResource == RT_VOLUME_TEXTURE )
        uiCaps2 |= FDDS_VOLUME;
    qSaveStream.Write(uiCaps2);

    for (int i = 0; i < 12; i++)
        qSaveStream.Write(cZero);

    // for a progress bar
    ProgressProbe qProgress(this);

    // write main image(s) and mipmaps if they are implied
    float fScopeCoefficient;
    if ( m_eResource == RT_2D_TEXTURE )
    {
        fScopeCoefficient = 1.0f/m_iResolutionLevelQuantity;

        // main image
        WriteImage(m_spq2DTextureSaveTo,qProgress,fScopeCoefficient,qSaveStream);

        if ( m_bHasNonFullResImages )
        {
            // mipmap levels
            if ( !m_bHasCustomMipmaps )
            {
                // generate and output mipmaps
                WriteMipmapLevels(m_spq2DTextureSaveTo,qProgress,fScopeCoefficient,qSaveStream);
            }
            else
            {
                // output pre-created mipmaps
                WriteCustomMipmapLevels(m_qCustomMipmaps,qProgress,fScopeCoefficient,qSaveStream);
            }
        }
    }
    else if ( m_eResource == RT_CUBIC_ENVIRONMENT_MAP )
    {
        fScopeCoefficient = 1.0f/(6*m_iResolutionLevelQuantity);

        for (int i = 0; i < 6; i++)
        {
            // full-resolution
            WriteImage(m_aspqCubicEnvMapSaveTo[i],qProgress,fScopeCoefficient,qSaveStream);

            if ( m_bHasNonFullResImages )
            {
                // mipmap levels
                if ( !m_bHasCustomMipmaps )
                {
                    // generate and output mipmaps
                    WriteMipmapLevels(m_aspqCubicEnvMapSaveTo[i],qProgress,fScopeCoefficient,qSaveStream);
                }
                else
                {
                    // output pre-created mipmaps
                    WriteCustomMipmapLevels(m_aqCEMCustomMipmaps[i],qProgress,fScopeCoefficient,qSaveStream);
                }
            }
        }
    }
    else  // m_eResource = RT_VOLUME_TEXTURE
    {
        fScopeCoefficient = 1.0f/(m_iVolumeTextureDepth + m_iResolutionLevelQuantity);

        // all full-resolution slices go first
        for (int i = 0; i < m_qSlices.GetQuantity(); i++)
            WriteImage(m_qSlices[i],qProgress,fScopeCoefficient,qSaveStream);

        if ( m_bHasCustomMipmaps )
        {
            // custom mipmap levels
            int iW, iH, iStrgW, iStrgH;
            for (int i = 0; i < m_qCustomMipmaps.GetQuantity(); i++)
            {
                iW = m_qCustomMipmaps[i]->GetWidth();
                iH = m_qCustomMipmaps[i]->GetHeight();
                RasterImagePtr spqStrgMipmapLevel;
                if ( !m_bDxtCompression
                ||   ((iW & 3) == 0 && (iH & 3) == 0) )
                {
                    // can store a mipmap image with its original dimensions
                    spqStrgMipmapLevel = m_qCustomMipmaps[i];
                }
                else
                {
                    // find the closest greater frame sides compatible with DXT encoding
                    iStrgW = iW;
                    iStrgH = iH;
                    while ( (iStrgW & 3) != 0 )
                        iStrgW++;
                    while ( (iStrgH & 3) != 0 )
                        iStrgH++;

                    // create it and place the mipmap in
                    spqStrgMipmapLevel = new RasterImage(m_qCustomMipmaps[i]->GetType(),iStrgW,iStrgH);
                    MemSet(spqStrgMipmapLevel->GetImage(),0,iStrgH*spqStrgMipmapLevel->GetRowStride());
                    spqStrgMipmapLevel->SetSubimage(m_qCustomMipmaps[i],0,0);
                }
                WriteImage(spqStrgMipmapLevel,qProgress,fScopeCoefficient,qSaveStream);
            }
        }
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatDds::FormatDds (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // signature
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 4 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qSignatureStream(m_eStreaming,m_qFilename,0,4,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acSignature[4];
    qSignatureStream.Read(4,acSignature);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acSignature,gs_acDds,4) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureStream.Finalize();

    unsigned int uiSize;

    // Surface Format Header.
    Stream qHeaderStream(m_eStreaming,m_qFilename,4,124,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    qHeaderStream.Read(uiSize);
    C_CONDITIONAL_THROW_EXCEPTION(( uiSize == 124 ),ET_FORMAT_MISMATCH);

    unsigned int uiHeaderFlags;
    qHeaderStream.Read(uiHeaderFlags);
    C_CONDITIONAL_THROW_EXCEPTION((
        (uiHeaderFlags & FDDS_HEIGHT) != 0
    &&  (uiHeaderFlags & FDDS_WIDTH) != 0
    &&  (uiHeaderFlags & FDDS_PIXELFORMAT) != 0
    &&  (uiHeaderFlags & FDDS_CAPS) != 0 ),ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(
        ( !((uiHeaderFlags & FDDS_PITCH) != 0 && (uiHeaderFlags & FDDS_LINEARSIZE) != 0) ),
        ET_FORMAT_MISMATCH);

    unsigned int uiHeight, uiWidth;
    qHeaderStream.Read(uiHeight);
    qHeaderStream.Read(uiWidth);
    C_CONDITIONAL_THROW_EXCEPTION(( uiWidth != 0 && uiHeight != 0 ),ET_FORMAT_MISMATCH);
    m_iMainImageWidth = uiWidth;
    m_iMainImageHeight = uiHeight;

    unsigned int uiPitchOrLinearSize;
    qHeaderStream.Read(uiPitchOrLinearSize);

    unsigned int uiDepth;
    qHeaderStream.Read(uiDepth);
    m_iVolumeTextureDepth = uiDepth;

    unsigned int uiMipmapCount;
    qHeaderStream.Read(uiMipmapCount);
    m_iResolutionLevelQuantity = uiMipmapCount;

    qHeaderStream.OffsetBytePosition(44);

    qHeaderStream.Read(uiSize);
    C_CONDITIONAL_THROW_EXCEPTION(( uiSize == 32 ),ET_FORMAT_MISMATCH);

    unsigned int uiPixelFormatFlags;
    qHeaderStream.Read(uiPixelFormatFlags);
    C_CONDITIONAL_THROW_EXCEPTION(
        ( ( (uiPixelFormatFlags & FDDS_RGB) != 0 ) != ( (uiPixelFormatFlags & FDDS_FOURCC) != 0 ) ),
        ET_FORMAT_MISMATCH);

    char acFourCC[4];
    qHeaderStream.Read(4,acFourCC);

    m_bDxtCompression = false;
    if ( (uiPixelFormatFlags & FDDS_RGB) != 0 )
    {
        m_eCompression = CT_NONE;
    }
    else  // DXT compression
    {
        if ( MemCmp(acFourCC,gs_acDxt1,4) == 0 )
            m_eCompression = CT_DXT_1;
        else if ( MemCmp(acFourCC,gs_acDxt2,4) == 0 )
            m_eCompression = CT_DXT_2;
        else if ( MemCmp(acFourCC,gs_acDxt3,4) == 0 )
            m_eCompression = CT_DXT_3;
        else if ( MemCmp(acFourCC,gs_acDxt4,4) == 0 )
            m_eCompression = CT_DXT_4;
        else if ( MemCmp(acFourCC,gs_acDxt5,4) == 0 )
            m_eCompression = CT_DXT_5;
        else
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);

        // for DXT compression, image dimensions must be multiple of four
        C_CONDITIONAL_THROW_EXCEPTION(( (m_iMainImageWidth & 3) == 0 && (m_iMainImageHeight & 3) == 0 ),
            ET_FORMAT_MISMATCH);

        m_bDxtCompression = true;
    }

    unsigned int uiRgbBitCount;
    qHeaderStream.Read(uiRgbBitCount);

    qHeaderStream.Read(m_uiRedBitMask);
    qHeaderStream.Read(m_uiGreenBitMask);
    qHeaderStream.Read(m_uiBlueBitMask);
    CALC_BITMASK_PARAMS(m_uiRedBitMask,m_iRedRightShift,m_iRedLength);
    CALC_BITMASK_PARAMS(m_uiGreenBitMask,m_iGreenRightShift,m_iGreenLength);
    CALC_BITMASK_PARAMS(m_uiBlueBitMask,m_iBlueRightShift,m_iBlueLength);

    qHeaderStream.Read(m_uiAlphaBitMask);
    unsigned int uiTBitMask = m_uiAlphaBitMask;
    m_iAlphaRightShift = 0;
    while ( (uiTBitMask & 1) == 0 )
    {
        uiTBitMask >>= 1;
        if ( ++m_iAlphaRightShift == 32 )
            break;
    }
    int iAlphaLength = 0;
    while ( (uiTBitMask & 1) == 1 )
    {
        uiTBitMask >>= 1;
        if ( ++iAlphaLength == 32 )
            break;
    }
    m_iAlphaMaxValue = (1 << iAlphaLength) - 1;

    unsigned int uiCaps1;
    qHeaderStream.Read(uiCaps1);
    C_CONDITIONAL_THROW_EXCEPTION(( (uiCaps1 & FDDS_TEXTURE) != 0 ),ET_FORMAT_MISMATCH);
    m_bHasNonFullResImages = ( (uiCaps1 & FDDS_MIPMAP) != 0 );
    if ( m_bHasNonFullResImages )
    {
        C_CONDITIONAL_THROW_EXCEPTION(( (uiHeaderFlags & FDDS_MIPMAPCOUNT) != 0 ),ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( m_iResolutionLevelQuantity > 0 ),ET_FORMAT_MISMATCH);
    }

    m_eResource = RT_2D_TEXTURE;
    unsigned int uiCaps2;
    qHeaderStream.Read(uiCaps2);
    if ( (uiCaps2 & FDDS_CUBEMAP) != 0 )
    {
        m_eResource = RT_CUBIC_ENVIRONMENT_MAP;

        // faces must be square
        C_CONDITIONAL_THROW_EXCEPTION(( m_iMainImageWidth == m_iMainImageHeight ),ET_FORMAT_MISMATCH);

        m_bHasFacePositiveX = ( (uiCaps2 & FDDS_POSITIVEX) != 0 );
        m_bHasFaceNegativeX = ( (uiCaps2 & FDDS_NEGATIVEX) != 0 );
        m_bHasFacePositiveY = ( (uiCaps2 & FDDS_POSITIVEY) != 0 );
        m_bHasFaceNegativeY = ( (uiCaps2 & FDDS_NEGATIVEY) != 0 );
        m_bHasFacePositiveZ = ( (uiCaps2 & FDDS_POSITIVEZ) != 0 );
        m_bHasFaceNegativeZ = ( (uiCaps2 & FDDS_NEGATIVEZ) != 0 );

        // must be at least one face
        C_CONDITIONAL_THROW_EXCEPTION((
            m_bHasFacePositiveX || m_bHasFaceNegativeX
        ||  m_bHasFacePositiveY || m_bHasFaceNegativeY
        ||  m_bHasFacePositiveZ || m_bHasFaceNegativeZ ),ET_FORMAT_MISMATCH);
    }
    if ( (uiCaps2 & FDDS_VOLUME) != 0 )
    {
        C_CONDITIONAL_THROW_EXCEPTION(( m_eResource == RT_2D_TEXTURE ),ET_FORMAT_MISMATCH);
        m_eResource = RT_VOLUME_TEXTURE;

        // main image dimensions and depth must be power of two
        C_CONDITIONAL_THROW_EXCEPTION(( IsPowerOfTwo(m_iMainImageWidth) && IsPowerOfTwo(m_iMainImageHeight) ),
            ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( (uiHeaderFlags & FDDS_DEPTH) != 0 ),ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( m_iVolumeTextureDepth > 0 && IsPowerOfTwo(m_iVolumeTextureDepth) ),
            ET_FORMAT_MISMATCH);
    }

    int iExpectedMainImageDataSize;
    if ( m_eCompression == CT_NONE )
    {
        C_CONDITIONAL_THROW_EXCEPTION(
            ( uiRgbBitCount == 8 || uiRgbBitCount == 16 || uiRgbBitCount == 24 || uiRgbBitCount == 32 ),
            ET_FORMAT_MISMATCH);
        int iBitsPerRow = m_iMainImageWidth*uiRgbBitCount;
        int iPitch = ( (iBitsPerRow & 7) == 0 ? iBitsPerRow/8 : iBitsPerRow/8 + 1 );
        if ( m_eResource == RT_VOLUME_TEXTURE )
            iPitch *= m_iVolumeTextureDepth;
        if ( (uiHeaderFlags & FDDS_PITCH) != 0 )
            C_CONDITIONAL_THROW_EXCEPTION(( uiPitchOrLinearSize == iPitch ),ET_FORMAT_MISMATCH);
        iExpectedMainImageDataSize = m_iMainImageHeight*iPitch;
        if ( (uiHeaderFlags & FDDS_LINEARSIZE) != 0 )
        {
            C_CONDITIONAL_THROW_EXCEPTION(( uiPitchOrLinearSize == iExpectedMainImageDataSize ),
                ET_FORMAT_MISMATCH);
        }

        m_iBitsPerPixel = uiRgbBitCount;
    }
    else  // DXT compression
    {
        if ( m_eCompression == CT_DXT_1 )
        {
            // 8 bytes per block
            iExpectedMainImageDataSize = m_iMainImageWidth*m_iMainImageHeight/2;
            if ( m_eResource == RT_VOLUME_TEXTURE )
                iExpectedMainImageDataSize *= m_iVolumeTextureDepth;
            if ( (uiHeaderFlags & FDDS_LINEARSIZE) != 0 )
            {
                C_CONDITIONAL_THROW_EXCEPTION(( uiPitchOrLinearSize == iExpectedMainImageDataSize ),
                    ET_FORMAT_MISMATCH);
            }

            m_iBitsPerPixel = 4;
        }
        else  // DXT2, DXT3, DXT4, or DXT5 compression
        {
            // 16 bytes per block
            iExpectedMainImageDataSize = m_iMainImageWidth*m_iMainImageHeight;
            if ( m_eResource == RT_VOLUME_TEXTURE )
                iExpectedMainImageDataSize *= m_iVolumeTextureDepth;
            if ( (uiHeaderFlags & FDDS_LINEARSIZE) != 0 )
            {
                C_CONDITIONAL_THROW_EXCEPTION(( uiPitchOrLinearSize == iExpectedMainImageDataSize ),
                    ET_FORMAT_MISMATCH);
            }

            m_iBitsPerPixel = 8;
        }
    }

    if ( m_eCompression == CT_NONE )
        m_eColorModel = ( (uiPixelFormatFlags & FDDS_ALPHAPIXELS) == 0 ? CMT_RGB : CMT_RGBA );
    else  // DXT compression
        m_eColorModel = CMT_RGBA;

    qHeaderStream.Finalize();

    if ( m_eCompression == CT_NONE )
    {
        m_bEightBitsPerSample = ( m_iRedLength <= 8
                             &&   m_iGreenLength <= 8
                             &&   m_iBlueLength <= 8 );
        if ( m_bEightBitsPerSample )
        {
            m_iRedLeftShift = 8 - m_iRedLength;
            m_iGreenLeftShift = 8 - m_iGreenLength;
            m_iBlueLeftShift = 8 - m_iBlueLength;
        }
        else  // bps of output images will be 16
        {
            m_iRedLeftShift = 16 - m_iRedLength;
            m_iGreenLeftShift = 16 - m_iGreenLength;
            m_iBlueLeftShift = 16 - m_iBlueLength;
        }
    }

    if ( m_bHasNonFullResImages )
    {
        // check the declared mipmap level number
        C_CONDITIONAL_THROW_EXCEPTION(( m_iResolutionLevelQuantity <= GetMaxPossibleResolutionLevelQuantity() ),
            ET_FORMAT_MISMATCH);
    }

    if ( m_eResource == RT_CUBIC_ENVIRONMENT_MAP )
    {
        // calculate occupancy per cube face
        m_iOccupancyPerCubeFace = 0;
        int iRLQMO = ( !m_bHasNonFullResImages ? 0 : m_iResolutionLevelQuantity - 1 );
        int iW = m_iMainImageWidth;
        int iH = m_iMainImageHeight;
        int iStrgW, iStrgH, iI = -1;
        for (/**/; /**/; /**/)
        {
            iStrgW = iW;
            iStrgH = iH;
            if ( m_bDxtCompression )
            {
                while ( (iStrgW & 3) != 0 )
                    iStrgW++;
                while ( (iStrgH & 3) != 0 )
                    iStrgH++;
            }

            if ( iI++ == iRLQMO )
                break;

            m_iOccupancyPerCubeFace += CalculateImageOccupancy(iStrgW,iStrgH);
            if ( iW != 1 )
                iW /= 2;
            if ( iH != 1 )
                iH /= 2;
        }
    }

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadMainImage (RasterImagePtr& rspqMainImage)
{
    return ReadImage(128,m_iMainImageWidth,m_iMainImageHeight,rspqMainImage);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::Load2DTextureMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_2D_TEXTURE && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveX (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveX );

    int iImageDataOffset = 128;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeX (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeX );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveY (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveY );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeY (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeY );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveZ (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveZ );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeZ (RasterImagePtr& rspqFace)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeZ );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveZ )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqFace);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveXMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveX && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeXMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeX && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveYMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveY && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeYMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeY && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFacePositiveZMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFacePositiveZ && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadFaceNegativeZMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP && m_bHasFaceNegativeZ && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset = 128;
    if ( m_bHasFacePositiveX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeX )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFaceNegativeY )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    if ( m_bHasFacePositiveZ )
        iImageDataOffset += m_iOccupancyPerCubeFace;
    return ReadMipmapLevel(iLevel,iImageDataOffset,rspqMipmapLevel);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadVolumeTextureSlice (int iSlice, RasterImagePtr& rspqSlice)
{
    assert( m_eResource == RT_VOLUME_TEXTURE
        &&  0 <= iSlice && iSlice < m_iVolumeTextureDepth );

    int iImageDataOffset = 128 + iSlice*CalculateImageOccupancy(m_iMainImageWidth,m_iMainImageHeight);
    return ReadImage(iImageDataOffset,m_iMainImageWidth,m_iMainImageHeight,rspqSlice);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::LoadVolumeTextureMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel)
{
    assert( m_eResource == RT_VOLUME_TEXTURE && m_bHasNonFullResImages
        &&  0 <= iLevel && iLevel < m_iResolutionLevelQuantity );

    int iImageDataOffset =
        128 + m_iVolumeTextureDepth*CalculateImageOccupancy(m_iMainImageWidth,m_iMainImageHeight);
    int iW = m_iMainImageWidth;
    int iH = m_iMainImageHeight;
    int iD = m_iVolumeTextureDepth;
    int iStrgW, iStrgH, iOccupancy, iMM = 0;
    while ( !(iW == 1 && iH == 1) )
    {
        if ( iW != 1 )
            iW /= 2;
        if ( iH != 1 )
            iH /= 2;
        if ( iD != 1 )
            iD /= 2;
        iStrgW = iW;
        iStrgH = iH;
        if ( m_bDxtCompression )
        {
            // find the closest greater frame sides compatible with DXT encoding
            while ( (iStrgW & 3) != 0 )
                iStrgW++;
            while ( (iStrgH & 3) != 0 )
                iStrgH++;
        }
        iOccupancy = CalculateImageOccupancy(iStrgW,iStrgH);
        for (int i = 0; i < iD; i++)
        {
            if ( iMM++ == iLevel )
                goto Exit;

            iImageDataOffset += iOccupancy;
        }
    }
Exit:

    if ( !ReadImage(iImageDataOffset,iStrgW,iStrgH,rspqMipmapLevel) )
        return false;
    if ( iStrgW != iW
    ||   iStrgH != iH )
    {
        // extract the mipmap from the container frame
        rspqMipmapLevel = rspqMipmapLevel->GetSubimage(0,0,iW,iH);
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
int FormatDds::GetMaxPossibleResolutionLevelQuantity () const
{
    int iMaxPossibleResolutionLevelQuantity;
    int iW = m_iMainImageWidth;
    int iH = m_iMainImageHeight;
    if ( m_eResource == RT_2D_TEXTURE
    ||   m_eResource == RT_CUBIC_ENVIRONMENT_MAP )
    {
        iMaxPossibleResolutionLevelQuantity = 1;
        while ( !(iW == 1 && iH == 1) )
        {
            if ( iW != 1 )
                iW /= 2;
            if ( iH != 1 )
                iH /= 2;
            iMaxPossibleResolutionLevelQuantity++;
        }
    }
    else  // m_eResource = RT_VOLUME_TEXTURE
    {
        int iD = m_iVolumeTextureDepth;
        iMaxPossibleResolutionLevelQuantity = 0;
        while ( !(iW == 1 && iH == 1) )
        {
            if ( iW != 1 )
                iW /= 2;
            if ( iH != 1 )
                iH /= 2;
            if ( iD != 1 )
                iD /= 2;
            iMaxPossibleResolutionLevelQuantity += iD;
        }
    }
    return iMaxPossibleResolutionLevelQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void FormatDds::InitAndPrepareForSaving (const RasterImagePtr& rspqMainImage, int iResolutionLevelQuantity)
{
    // shared defaults
    m_eDxtQuality = DXTCQT_MEDIUM;
    m_fDxtWeigFcrR = 0.3086f;
    m_fDxtWeigFcrG = 0.6094f;
    m_fDxtWeigFcrB = 0.0820f;
    m_iDxt1aThreshold = 128;

    if ( iResolutionLevelQuantity == -1 )
        m_iResolutionLevelQuantity = GetMaxPossibleResolutionLevelQuantity();
    else
        m_iResolutionLevelQuantity = iResolutionLevelQuantity;

    assert( rspqMainImage->GetWidth() != 0 && rspqMainImage->GetHeight() != 0 );
    assert( rspqMainImage->GetColorModel() == RasterImage::CMT_RGB
        ||  rspqMainImage->GetColorModel() == RasterImage::CMT_RGBA );
    assert( !rspqMainImage->HasVisibilityMask() );
    assert( m_iResolutionLevelQuantity <= GetMaxPossibleResolutionLevelQuantity() );
    assert( !(rspqMainImage->GetColorModel() == RasterImage::CMT_RGB
        &&  (m_ePacking == PT_A8_R8_G8_B8
        ||  m_ePacking == PT_A1_R5_G5_B5
        ||  m_ePacking == PT_A4_R4_G4_B4
        ||  m_ePacking == PT_A2_R10_G10_B10
        ||  m_ePacking == PT_A8_R3_G3_B2
        ||  m_ePacking == PT_DXT_2
        ||  m_ePacking == PT_DXT_3
        ||  m_ePacking == PT_DXT_4
        ||  m_ePacking == PT_DXT_5)) );
    assert( !(rspqMainImage->GetColorModel() == RasterImage::CMT_RGBA
        &&  (m_ePacking == PT_R8_G8_B8
        ||  m_ePacking == PT_R5_G6_B5
        ||  m_ePacking == PT_X8_R8_G8_B8
        ||  m_ePacking == PT_X1_R5_G5_B5
        ||  m_ePacking == PT_R3_G3_B2
        ||  m_ePacking == PT_X4_R4_G4_B4)) );
    assert( !((m_ePacking == PT_DXT_1
        ||  m_ePacking == PT_DXT_2
        ||  m_ePacking == PT_DXT_3
        ||  m_ePacking == PT_DXT_4
        ||  m_ePacking == PT_DXT_5) && ((m_iMainImageWidth & 3) != 0 || (m_iMainImageHeight & 3) != 0)) );

    // specify type of DXT compression
    switch ( m_ePacking )
    {
    case PT_DXT_1:
        m_eCompression = CT_DXT_1;
        break;
    case PT_DXT_2:
        m_eCompression = CT_DXT_2;
        break;
    case PT_DXT_3:
        m_eCompression = CT_DXT_3;
        break;
    case PT_DXT_4:
        m_eCompression = CT_DXT_4;
        break;
    case PT_DXT_5:
        m_eCompression = CT_DXT_5;
        break;
    default:  // no compression
        m_eCompression = CT_NONE;
    }

    // get color bitmasks
    switch ( m_ePacking )
    {
    case PT_A8_R8_G8_B8:
        m_uiRedBitMask = 16711680;
        m_uiGreenBitMask = 65280;
        m_uiBlueBitMask = 255;
        m_uiAlphaBitMask = 4278190080;
        break;
    case PT_A1_R5_G5_B5:
        m_uiRedBitMask = 31744;
        m_uiGreenBitMask = 992;
        m_uiBlueBitMask = 31;
        m_uiAlphaBitMask = 32768;
        break;
    case PT_A4_R4_G4_B4:
        m_uiRedBitMask = 3840;
        m_uiGreenBitMask = 240;
        m_uiBlueBitMask = 15;
        m_uiAlphaBitMask = 61440;
        break;
    case PT_R8_G8_B8:
        m_uiRedBitMask = 16711680;
        m_uiGreenBitMask = 65280;
        m_uiBlueBitMask = 255;
        m_uiAlphaBitMask = 0;
        break;
    case PT_R5_G6_B5:
        m_uiRedBitMask = 63488;
        m_uiGreenBitMask = 2016;
        m_uiBlueBitMask = 31;
        m_uiAlphaBitMask = 0;
        break;
    case PT_A2_R10_G10_B10:
        m_uiRedBitMask = 1023;
        m_uiGreenBitMask = 1047552;
        m_uiBlueBitMask = 1072693248;
        m_uiAlphaBitMask = 3221225472;
        break;
    case PT_X8_R8_G8_B8:
        m_uiRedBitMask = 16711680;
        m_uiGreenBitMask = 65280;
        m_uiBlueBitMask = 255;
        m_uiAlphaBitMask = 0;
        break;
    case PT_X1_R5_G5_B5:
        m_uiRedBitMask = 31744;
        m_uiGreenBitMask = 992;
        m_uiBlueBitMask = 31;
        m_uiAlphaBitMask = 0;
        break;
    case PT_R3_G3_B2:
        m_uiRedBitMask = 224;
        m_uiGreenBitMask = 28;
        m_uiBlueBitMask = 3;
        m_uiAlphaBitMask = 0;
        break;
    case PT_A8_R3_G3_B2:
        m_uiRedBitMask = 224;
        m_uiGreenBitMask = 28;
        m_uiBlueBitMask = 3;
        m_uiAlphaBitMask = 65280;
        break;
    case PT_X4_R4_G4_B4:
        m_uiRedBitMask = 3840;
        m_uiGreenBitMask = 240;
        m_uiBlueBitMask = 15;
        m_uiAlphaBitMask = 0;
        break;
    default:  // DXT compression
        m_uiRedBitMask = 0;
        m_uiGreenBitMask = 0;
        m_uiBlueBitMask = 0;
        m_uiAlphaBitMask = 0;
    }

    // from bitmasks get values supporting actual storage process
    int iRedLength, iGreenLength, iBlueLength, iAlphaLength;
    CALC_BITMASK_PARAMS(m_uiRedBitMask,m_iRedLeftShift,iRedLength);
    CALC_BITMASK_PARAMS(m_uiGreenBitMask,m_iGreenLeftShift,iGreenLength);
    CALC_BITMASK_PARAMS(m_uiBlueBitMask,m_iBlueLeftShift,iBlueLength);
    CALC_BITMASK_PARAMS(m_uiAlphaBitMask,m_iAlphaLeftShift,iAlphaLength);
    m_iRedMaxValue = (1 << iRedLength) - 1;
    m_iGreenMaxValue = (1 << iGreenLength) - 1;
    m_iBlueMaxValue = (1 << iBlueLength) - 1;
    m_iAlphaMaxValue = (1 << iAlphaLength) - 1;
    m_bEightBitsPerSample = ( iRedLength <= 8
                         &&   iGreenLength <= 8
                         &&   iBlueLength <= 8 );

    if ( m_eResource == RT_2D_TEXTURE
    ||   m_eResource == RT_CUBIC_ENVIRONMENT_MAP )
    {
        m_bHasNonFullResImages = ( m_iResolutionLevelQuantity - 1 > 0 );
    }
    else  // m_eResource = RT_VOLUME_TEXTURE
    {
        m_bHasNonFullResImages = ( m_iResolutionLevelQuantity > 0 );
    }

    // get bits-per-pixel value
    switch ( m_ePacking )
    {
    case PT_A8_R8_G8_B8:
        m_iBitsPerPixel = 32;
        break;
    case PT_A1_R5_G5_B5:
        m_iBitsPerPixel = 16;
        break;
    case PT_A4_R4_G4_B4:
        m_iBitsPerPixel = 16;
        break;
    case PT_R8_G8_B8:
        m_iBitsPerPixel = 24;
        break;
    case PT_R5_G6_B5:
        m_iBitsPerPixel = 16;
        break;
    case PT_A2_R10_G10_B10:
        m_iBitsPerPixel = 32;
        break;
    case PT_X8_R8_G8_B8:
        m_iBitsPerPixel = 32;
        break;
    case PT_X1_R5_G5_B5:
        m_iBitsPerPixel = 16;
        break;
    case PT_R3_G3_B2:
        m_iBitsPerPixel = 8;
        break;
    case PT_A8_R3_G3_B2:
        m_iBitsPerPixel = 16;
        break;
    case PT_X4_R4_G4_B4:
        m_iBitsPerPixel = 16;
        break;
    default:  // DXT compression
        m_iBitsPerPixel = 0;
    }

    // internal color model
    switch ( rspqMainImage->GetColorModel() )
    {
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        break;
    }

    m_bDxtCompression = ( m_ePacking == PT_DXT_1
                     ||   m_ePacking == PT_DXT_2
                     ||   m_ePacking == PT_DXT_3
                     ||   m_ePacking == PT_DXT_4
                     ||   m_ePacking == PT_DXT_5 );
}
//------------------------------------------------------------------------------------------------------------------
void FormatDds::WriteImage (const RasterImagePtr& rspqImage, ProgressProbe& rqProgress, float fScopeCoefficient,
    Stream& rqStream)
{
    int iWidth = rspqImage->GetWidth();
    int iHeight = rspqImage->GetHeight();
    if ( m_eCompression == CT_NONE )
    {
        rqProgress.SetAdvance(fScopeCoefficient/iHeight);

        ColorRgb qColorRgb;
        ColorRgba qColorRgba;
        float fRed, fGreen, fBlue, fAlpha;
        unsigned int uiValue;
        unsigned char ucValue;
        unsigned short usValue;
        char cByte0, cByte1, cByte2;
        char* pcV;
        for (int iY = 0; iY < iHeight; iY++)
        {
            for (int iX = 0; iX < iWidth; iX++)
            {
                if ( m_eColorModel == CMT_RGB )
                {
                    qColorRgb = rspqImage->GetPixelRgb(iX,iY);
                    fRed = qColorRgb.R();
                    fGreen = qColorRgb.G();
                    fBlue = qColorRgb.B();
                }
                else  // m_eColorModel = CMT_RGBA
                {
                    qColorRgba = rspqImage->GetPixelRgba(iX,iY);
                    if ( m_ePacking != PT_A2_R10_G10_B10 )
                    {
                        fRed = qColorRgba.R();
                        fGreen = qColorRgba.G();
                        fBlue = qColorRgba.B();
                    }
                    else
                    {
                        // reversed component order is common for PT_A2_R10_G10_B10
                        fRed = qColorRgba.B();
                        fGreen = qColorRgba.G();
                        fBlue = qColorRgba.R();
                    }
                    fAlpha = qColorRgba.A();
                }

                uiValue = 0;
                uiValue |= Mathf::RoundToInt(fRed*m_iRedMaxValue) << m_iRedLeftShift;
                uiValue |= Mathf::RoundToInt(fGreen*m_iGreenMaxValue) << m_iGreenLeftShift;
                uiValue |= Mathf::RoundToInt(fBlue*m_iBlueMaxValue) << m_iBlueLeftShift;
                if ( m_eColorModel == CMT_RGBA )
                    uiValue |= Mathf::RoundToInt(fAlpha*m_iAlphaMaxValue) << m_iAlphaLeftShift;

                if ( m_iBitsPerPixel == 8 )
                {
                    ucValue = uiValue;
                    rqStream.Write(ucValue);
                }
                else if ( m_iBitsPerPixel == 16 )
                {
                    usValue = uiValue;
                    rqStream.Write(usValue);
                }
                else if ( m_iBitsPerPixel == 24 )
                {
                    // 3-byte based storage compels to deal with endianness directly
                    pcV = (char*)&uiValue;
#ifndef Q_BIG_ENDIAN
                    cByte0 = pcV[0];
                    cByte1 = pcV[1];
                    cByte2 = pcV[2];
#else
                    cByte2 = pcV[1];
                    cByte1 = pcV[2];
                    cByte0 = pcV[3];
#endif
                    rqStream.Write(cByte0);
                    rqStream.Write(cByte1);
                    rqStream.Write(cByte2);
                }
                else  // m_iBitsPerPixel = 32
                {
                    rqStream.Write(uiValue);
                }
            }

            rqProgress.MakeAdvance();
        }
    }
    else  // DXT compression
    {
        CompressionDxt::CompressionType eCDxtCompression;
        switch ( m_eCompression )
        {
        case CT_DXT_1:
            eCDxtCompression = CompressionDxt::CT_DXT_1;
            break;
        case CT_DXT_2:
            eCDxtCompression = CompressionDxt::CT_DXT_2;
            break;
        case CT_DXT_3:
            eCDxtCompression = CompressionDxt::CT_DXT_3;
            break;
        case CT_DXT_4:
            eCDxtCompression = CompressionDxt::CT_DXT_4;
            break;
        case CT_DXT_5:
            eCDxtCompression = CompressionDxt::CT_DXT_5;
            break;
        }
        CompressionDxt::CompressionQualityType eCDxtQuality;
        switch ( m_eDxtQuality )
        {
        case DXTCQT_LOW:
            eCDxtQuality = CompressionDxt::CQT_LOW;
            break;
        case DXTCQT_MEDIUM:
            eCDxtQuality = CompressionDxt::CQT_MEDIUM;
            break;
        case DXTCQT_HIGH:
            eCDxtQuality = CompressionDxt::CQT_HIGH;
            break;
        }

        CompressionDxt qDxt(eCDxtCompression,eCDxtQuality);
        qDxt.SetWeightingFactors(
            m_fDxtWeigFcrR,
            m_fDxtWeigFcrG,
            m_fDxtWeigFcrB);
        qDxt.SetDxt1aThreshold(m_iDxt1aThreshold);
        qDxt.RedirectProgressTo(this,rqProgress,fScopeCoefficient);
        RasterImagePtr spqImageToPack;
        if ( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||   rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 )
        {
            spqImageToPack = rspqImage;
        }
        else
        {
            if ( m_eColorModel == CMT_RGB )
            {
                RasterImageConversion qRIC(rspqImage,RasterImage::IT_RGB_8_8_8);
                spqImageToPack = qRIC.GetConverted();
            }
            else  // m_eColorModel = CMT_RGBA
            {
                RasterImageConversion qRIC(rspqImage,RasterImage::IT_RGBA_8_8_8_8);
                spqImageToPack = qRIC.GetConverted();
            }
        }
        StillArray<char> qCompressedImageData;
        qDxt.Compress(spqImageToPack,qCompressedImageData);
        rqStream.Write(qCompressedImageData.GetQuantity(),qCompressedImageData);
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::ReadImage (int iOffset, int iWidth, int iHeight, RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // for a progress bar
    ProgressProbe qProgress(this);

    // read image data
    int iExpectedImageDataSize;
    if ( m_eCompression == CT_NONE )
    {
        // allocate the output image
        if ( m_bEightBitsPerSample )
        {
            if ( m_eColorModel == CMT_RGB )
                rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,iWidth,iHeight);
            else  // m_eColorModel = CMT_RGBA
                rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,iWidth,iHeight);
        }
        else
        {
            if ( m_eColorModel == CMT_RGB )
                rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,iWidth,iHeight);
            else  // m_eColorModel = CMT_RGBA
                rspqImage = new RasterImage(RasterImage::IT_RGBA_16_16_16_16,iWidth,iHeight);
        }

        qProgress.SetAdvance(1.0f/iHeight);

        int iBitsPerRow = iWidth*m_iBitsPerPixel;
        int iPitch = ( (iBitsPerRow & 7) == 0 ? iBitsPerRow/8 : iBitsPerRow/8 + 1 );
        iExpectedImageDataSize = iHeight*iPitch;
        Stream qImageDataStream(m_eStreaming,m_qFilename,iOffset,iExpectedImageDataSize,true,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        unsigned char ucValue;
        unsigned short usValue;
        char cByte0, cByte1, cByte2;
        char* pcV;
        unsigned int uiValue;
        unsigned char* pucRow = (unsigned char*)rspqImage->GetImage();
        unsigned char* pucComp = pucRow;
        for (int iY = 0; iY < iHeight; iY++)
        {
            qImageDataStream.SetBytePosition(iY*iPitch,true);
            for (int iX = 0; iX < iWidth; iX++)
            {
                if ( m_iBitsPerPixel == 8 )
                {
                    qImageDataStream.Read(ucValue);
                    uiValue = ucValue;
                }
                else if ( m_iBitsPerPixel == 16 )
                {
                    qImageDataStream.Read(usValue);
                    uiValue = usValue;
                }
                else if ( m_iBitsPerPixel == 24 )
                {
                    // 3-byte based storage compels to deal with endianness directly
                    qImageDataStream.Read(cByte0);
                    qImageDataStream.Read(cByte1);
                    qImageDataStream.Read(cByte2);
                    uiValue = 0;
                    pcV = (char*)&uiValue;
#ifndef Q_BIG_ENDIAN
                    pcV[0] = cByte0;
                    pcV[1] = cByte1;
                    pcV[2] = cByte2;
#else
                    pcV[1] = cByte2;
                    pcV[2] = cByte1;
                    pcV[3] = cByte0;
#endif
                }
                else  // m_iBitsPerPixel = 32
                {
                    qImageDataStream.Read(uiValue);
                }

                if ( m_bEightBitsPerSample )
                {
                    pucComp[0] = ((uiValue & m_uiRedBitMask) >> m_iRedRightShift) << m_iRedLeftShift;
                    pucComp[0] |= pucComp[0] >> m_iRedLength;
                    pucComp[1] = ((uiValue & m_uiGreenBitMask) >> m_iGreenRightShift) << m_iGreenLeftShift;
                    pucComp[1] |= pucComp[1] >> m_iGreenLength;
                    pucComp[2] = ((uiValue & m_uiBlueBitMask) >> m_iBlueRightShift) << m_iBlueLeftShift;
                    pucComp[2] |= pucComp[2] >> m_iBlueLength;
                    pucComp += 3;
                    if ( m_eColorModel == CMT_RGBA )
                    {
                        *pucComp++ = Mathf::RoundToInt(
                            ((float)((uiValue & m_uiAlphaBitMask) >> m_iAlphaRightShift))/m_iAlphaMaxValue*255);
                    }
                }
                else
                {
                    // it's probably A2_R10_G10_B10 scheme with strange mask order
                    usValue = ((uiValue & m_uiBlueBitMask) >> m_iBlueRightShift) << m_iBlueLeftShift;
                    usValue |= usValue >> m_iBlueLength;
                    MemCpy(pucComp,&usValue,2);
                    pucComp += 2;
                    usValue = ((uiValue & m_uiGreenBitMask) >> m_iGreenRightShift) << m_iGreenLeftShift;
                    usValue |= usValue >> m_iGreenLength;
                    MemCpy(pucComp,&usValue,2);
                    pucComp += 2;
                    usValue = ((uiValue & m_uiRedBitMask) >> m_iRedRightShift) << m_iRedLeftShift;
                    usValue |= usValue >> m_iRedLength;
                    MemCpy(pucComp,&usValue,2);
                    pucComp += 2;
                    if ( m_eColorModel == CMT_RGBA )
                    {
                        usValue = Mathf::RoundToInt(
                            ((float)((uiValue & m_uiAlphaBitMask) >> m_iAlphaRightShift))/m_iAlphaMaxValue*65535);
                        MemCpy(pucComp,&usValue,2);
                        pucComp += 2;
                    }
                }
            }

            pucRow += rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
        qImageDataStream.Finalize();
    }
    else  // DXT compression
    {
        iExpectedImageDataSize = iWidth*iHeight;
        if ( m_eCompression == CT_DXT_1 )
            iExpectedImageDataSize /= 2;
        Stream qImageDataStream(m_eStreaming,m_qFilename,iOffset,iExpectedImageDataSize,true,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        CompressionDxt::CompressionType eDxtCompression;
        switch ( m_eCompression )
        {
        case CT_DXT_1:
            eDxtCompression = CompressionDxt::CT_DXT_1;
            break;
        case CT_DXT_2:
            eDxtCompression = CompressionDxt::CT_DXT_2;
            break;
        case CT_DXT_3:
            eDxtCompression = CompressionDxt::CT_DXT_3;
            break;
        case CT_DXT_4:
            eDxtCompression = CompressionDxt::CT_DXT_4;
            break;
        case CT_DXT_5:
            eDxtCompression = CompressionDxt::CT_DXT_5;
            break;
        }
        CompressionDxt qDxt(eDxtCompression,iWidth,iHeight);
        qDxt.RedirectProgressTo(this,qProgress,1.0f);
        qDxt.Decompress(qImageDataStream,rspqImage);

        qImageDataStream.Finalize();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
int FormatDds::CalculateImageOccupancy (int iWidth, int iHeight) const
{
    int iOccupancy;
    if ( m_eCompression == CT_NONE )
    {
        int iBitsPerRow = iWidth*m_iBitsPerPixel;
        int iPitch = ( (iBitsPerRow & 7) == 0 ? iBitsPerRow/8 : iBitsPerRow/8 + 1 );
        iOccupancy = iHeight*iPitch;
    }
    else  // DXT compression
    {
        iOccupancy = iWidth*iHeight;
        if ( m_eCompression == CT_DXT_1 )
            iOccupancy /= 2;
    }
    return iOccupancy;
}
//------------------------------------------------------------------------------------------------------------------
void FormatDds::WriteMipmapLevels (const RasterImagePtr& rspqFullResImage, ProgressProbe& rqProgress,
    float fScopeCoefficient, Stream& rqStream)
{
    // function is to be used only by 2D textures and cubic environment maps
    RasterImagePtr spqPrevMipmapLevel = rspqFullResImage;
    int iW = m_iMainImageWidth;
    int iH = m_iMainImageHeight;
    int iStrgW, iStrgH, iRLQMO = m_iResolutionLevelQuantity - 1;
    for (int i = 0; i < iRLQMO; i++)
    {
        if ( iW != 1 )
            iW /= 2;
        if ( iH != 1 )
            iH /= 2;

        RasterImageTransformation qRIT(spqPrevMipmapLevel);
        RasterImagePtr spqMipmapLevel = qRIT.GetResampled(iW,iH,RasterImageTransformation::IT_BILINEAR);
        RasterImagePtr spqStrgMipmapLevel;
        if ( !m_bDxtCompression
        ||   ((iW & 3) == 0 && (iH & 3) == 0) )
        {
            // can store a mipmap image with its original dimensions
            spqStrgMipmapLevel = spqMipmapLevel;
        }
        else
        {
            // find the closest greater frame sides compatible with DXT encoding
            iStrgW = iW;
            iStrgH = iH;
            while ( (iStrgW & 3) != 0 )
                iStrgW++;
            while ( (iStrgH & 3) != 0 )
                iStrgH++;

            // create it and place the mipmap in
            spqStrgMipmapLevel = new RasterImage(spqMipmapLevel->GetType(),iStrgW,iStrgH);
            MemSet(spqStrgMipmapLevel->GetImage(),0,iStrgH*spqStrgMipmapLevel->GetRowStride());
            spqStrgMipmapLevel->SetSubimage(spqMipmapLevel,0,0);
        }
        WriteImage(spqStrgMipmapLevel,rqProgress,fScopeCoefficient,rqStream);

        spqPrevMipmapLevel = spqMipmapLevel;
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatDds::WriteCustomMipmapLevels (const StillArray<RasterImagePtr>& rqCustomMipmaps,
    ProgressProbe& rqProgress, float fScopeCoefficient, Stream& rqStream)
{
    // the function is to be used only by 2D textures and cubic environment maps
    int iW = m_iMainImageWidth;
    int iH = m_iMainImageHeight;
    int iStrgW, iStrgH;
    for (int i = 0; i < rqCustomMipmaps.GetQuantity(); i++)
    {
        if ( iW != 1 )
            iW /= 2;
        if ( iH != 1 )
            iH /= 2;

        RasterImagePtr spqStrgMipmapLevel;
        if ( !m_bDxtCompression
        ||   ((iW & 3) == 0 && (iH & 3) == 0) )
        {
            // can store a mipmap image with its original dimensions
            spqStrgMipmapLevel = rqCustomMipmaps[i];
        }
        else
        {
            // find the closest greater frame sides compatible with DXT encoding
            iStrgW = iW;
            iStrgH = iH;
            while ( (iStrgW & 3) != 0 )
                iStrgW++;
            while ( (iStrgH & 3) != 0 )
                iStrgH++;

            // create it and place the mipmap in
            spqStrgMipmapLevel = new RasterImage(rqCustomMipmaps[i]->GetType(),iStrgW,iStrgH);
            MemSet(spqStrgMipmapLevel->GetImage(),0,iStrgH*spqStrgMipmapLevel->GetRowStride());
            spqStrgMipmapLevel->SetSubimage(rqCustomMipmaps[i],0,0);
        }
        WriteImage(spqStrgMipmapLevel,rqProgress,fScopeCoefficient,rqStream);
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatDds::ReadMipmapLevel (int iLevel, int iImageDataOffset, RasterImagePtr& rspqMipmapLevel)
{
    // the function is to be used only by 2D textures and cubic environment maps
    int iW = m_iMainImageWidth;
    int iH = m_iMainImageHeight;
    int iStrgW, iStrgH, i = 0;
    for (/**/; /**/; /**/)
    {
        iStrgW = iW;
        iStrgH = iH;
        if ( m_bDxtCompression )
        {
            // find the closest greater frame sides compatible with DXT encoding
            while ( (iStrgW & 3) != 0 )
                iStrgW++;
            while ( (iStrgH & 3) != 0 )
                iStrgH++;
        }

        if ( i++ == iLevel )
            break;

        iImageDataOffset += CalculateImageOccupancy(iStrgW,iStrgH);
        if ( iW != 1 )
            iW /= 2;
        if ( iH != 1 )
            iH /= 2;
    }

    if ( !ReadImage(iImageDataOffset,iStrgW,iStrgH,rspqMipmapLevel) )
        return false;
    if ( iStrgW != iW
    ||   iStrgH != iH )
    {
        // extract the mipmap from the container frame
        rspqMipmapLevel = rspqMipmapLevel->GetSubimage(0,0,iW,iH);
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



