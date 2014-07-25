#include "QFormatGif.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatGif.mcr"
#include "QProgressProbe.h"
#include "QSystUtils.h"
#include "QStorage.h"
#include "QBound2i.h"
#include "QCompressionLzw.h"
#include "QRasterImageConversion.h"
using namespace Q;

const PaletteColor FormatGif::ms_qTransparencyColor(0,0,0);
const PaletteColor FormatGif::ms_qDraftColor(255,255,255);

// support for interlaced loading
const int FormatGif::ms_aiIrlcStrt[4] = {0, 4, 2, 1};
const int FormatGif::ms_aiIrlcStep[4] = {8, 8, 4, 2};

// support for GenerateInputAnimation
const float FormatGif::ms_fPixelSimilarityBaseMapper(0.95f);

//------------------------------------------------------------------------------------------------------------------
FormatGif::~FormatGif ()
{
    if ( m_pvLzw )
        delete (CompressionLzw*)m_pvLzw;
}
//------------------------------------------------------------------------------------------------------------------
FormatGif::FormatGif (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iLogicalScreenWidth(rspqImage->GetWidth()),
    m_iLogicalScreenHeight(rspqImage->GetHeight()),
    m_bTitleImageIsInterlaced(false),
    m_pvLzw(0)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED );
    assert( !(rspqImage->HasVisibilityMask() && rspqImage->GetPaletteSize() == 256) );

    // form Global Color Table
    m_qGlobalColorTable = rspqImage->GetPalette();
    if ( rspqImage->HasVisibilityMask() )
    {
        m_qGlobalColorTable.AdvanceQuantity(1);
        m_qGlobalColorTable.GetLast() = ms_qTransparencyColor;
    }
    m_bHasGlobalColorTable = true;

    m_iBitsPerPixel = Storage::GetUBitQuantity(m_qGlobalColorTable.GetQuantity()-1);
}
//------------------------------------------------------------------------------------------------------------------
FormatGif::FormatGif (const Animation& rqAnimation, int iLogicalScreenWidth, int iLogicalScreenHeight,
    bool bAllPalettesAreEqual)
    :
    m_qAnimationSaveTo(rqAnimation),
    m_iLogicalScreenWidth(iLogicalScreenWidth),
    m_iLogicalScreenHeight(iLogicalScreenHeight),
    m_iLoopQuantity(0),
    m_pvLzw(0)
{
#ifdef _DEBUG
    assert( !rqAnimation.IsEmpty() );
    assert( iLogicalScreenWidth > 0 && iLogicalScreenHeight > 0 );
    assert( rqAnimation.GetFirst().DisposalMethod != DT_RESTORE_TO_PREVIOUS );
    for (int i = 0; i < rqAnimation.GetQuantity(); i++)
    {
        const AnimationFrame& rqAnimationFrame = rqAnimation[i];

        assert( rqAnimationFrame.ImagePtr->GetWidth() != 0 && rqAnimationFrame.ImagePtr->GetHeight() != 0 );
        assert( rqAnimationFrame.ImagePtr->GetWidth() <= 65535 && rqAnimationFrame.ImagePtr->GetHeight() <= 65535 );
        assert( rqAnimationFrame.ImagePtr->GetColorModel() == RasterImage::CMT_PALETTED );
        assert( !(rqAnimationFrame.ImagePtr->HasVisibilityMask()
            &&  rqAnimationFrame.ImagePtr->GetPaletteSize() == 256) );
        assert( rqAnimationFrame.PositionX + rqAnimationFrame.ImagePtr->GetWidth() <= iLogicalScreenWidth );
        assert( rqAnimationFrame.PositionY + rqAnimationFrame.ImagePtr->GetHeight() <= iLogicalScreenHeight );
    }
#endif

    if ( bAllPalettesAreEqual )
    {
        // form Global Color Table
        m_qGlobalColorTable = rqAnimation.GetFirst().ImagePtr->GetPalette();
        bool bAtLeastOneVisMask = false;
        for (int i = 0; i < rqAnimation.GetQuantity(); i++)
        {
            if ( rqAnimation[i].ImagePtr->HasVisibilityMask() )
            {
                bAtLeastOneVisMask = true;
                break;
            }
        }
        if ( bAtLeastOneVisMask )
        {
            m_qGlobalColorTable.AdvanceQuantity(1);
            m_qGlobalColorTable.GetLast() = ms_qTransparencyColor;
        }
        m_bHasGlobalColorTable = true;

        m_iBitsPerPixel = Storage::GetUBitQuantity(m_qGlobalColorTable.GetQuantity()-1);
    }
    else
    {
        // form Local Color Tables
        m_qLocalColorTablesSaveTo.SetQuantity(rqAnimation.GetQuantity());
        for (int i = 0; i < rqAnimation.GetQuantity(); i++)
        {
            const AnimationFrame& rqAnimationFrame = rqAnimation[i];
            RasterImage::Palette& rqLocalColorTable = m_qLocalColorTablesSaveTo[i];

            rqLocalColorTable = rqAnimationFrame.ImagePtr->GetPalette();
            if ( rqAnimationFrame.ImagePtr->HasVisibilityMask() )
            {
                rqLocalColorTable.AdvanceQuantity(1);
                rqLocalColorTable.GetLast() = ms_qTransparencyColor;
            }
        }
        m_bHasGlobalColorTable = false;

        // Color Resolution will take the maximal value of local Color Resolutions
        m_iBitsPerPixel = 1;
        m_qFrameBitsPerPixelSaveTo.SetQuantity(rqAnimation.GetQuantity());
        int iBpp;
        for (int i = 0; i < rqAnimation.GetQuantity(); i++)
        {
            iBpp = Storage::GetUBitQuantity(m_qLocalColorTablesSaveTo[i].GetQuantity()-1);
            if ( iBpp > m_iBitsPerPixel )
                m_iBitsPerPixel = iBpp;
            m_qFrameBitsPerPixelSaveTo[i] = iBpp;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::SaveImage (const char* acFilename)
{
    Stream qSaveStream(acFilename,true);
    WriteImage(qSaveStream);

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::SaveImage (StillArray<char>& rqStaticBytestream)
{
    rqStaticBytestream.RemoveAll();

    Stream qSaveStream(true);
    qSaveStream.SetDoNotFreeBufferAfterComplete();
    WriteImage(qSaveStream);
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::SaveAnimation (const char* acFilename)
{
    Stream qSaveStream(acFilename,true);
    WriteAnimation(qSaveStream);

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::SaveAnimation (StillArray<char>& rqStaticBytestream)
{
    rqStaticBytestream.RemoveAll();

    Stream qSaveStream(true);
    qSaveStream.SetDoNotFreeBufferAfterComplete();
    WriteAnimation(qSaveStream);
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());
}
//------------------------------------------------------------------------------------------------------------------
FormatGif::FormatGif (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_pvLzw(0)
{
    rbSuccess = LoadHeadingData();
}
//------------------------------------------------------------------------------------------------------------------
FormatGif::FormatGif (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess)
    :
    FormatBase(bBytestreamIsStatic,acBytestream,( bBytestreamIsStatic ? iBytestreamSize : -1 )),
    m_pvLzw(0)
{
    if ( !bBytestreamIsStatic )
    {
        m_iDevelopedRowQuantity = 0;
        m_iCurrentPassIndex = 0;
    }

    rbSuccess = LoadHeadingData(iBytestreamSize);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize)
{
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && !m_bDraftImageAllocated )
    {
        // draft image
        RasterImage::Palette qDIPalette(1);
        qDIPalette[0] = ms_qDraftColor;
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iLogicalScreenWidth,m_iLogicalScreenHeight,
            qDIPalette);
        MemSet(rspqImage->GetImage(),0,m_iLogicalScreenHeight*rspqImage->GetRowStride());

        m_bDraftImageAllocated = true;
    }

    bool bSuccess;
    int iNextReadPos = m_iFirstIDOrGCEPosition;

    RasterImage::Palette qLocalColorTable;

    AnimationFrame qAnimationFrame;
    if ( m_bIsAnimated )
    {
        // Graphic Control Extension.
        iNextReadPos += 2;
        Stream qGCEStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,6,true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        unsigned char ucBlockSize;
        qGCEStream.Read(ucBlockSize);
        CONDITIONAL_THROW_EXCEPTION(( ucBlockSize == 4 ),ET_FORMAT_MISMATCH);

        qGCEStream.OffsetBitPosition(7);

        int iTransparentColorFlag;
        qGCEStream.ReadUBits(iTransparentColorFlag,1);
        qAnimationFrame.m_bHasTransparency = ( iTransparentColorFlag == 1 );

        qGCEStream.OffsetBytePosition(2);

        unsigned char ucTransparentColorIndex;
        qGCEStream.Read(ucTransparentColorIndex);
        qAnimationFrame.m_iTransparentColorIndex = ucTransparentColorIndex;

        unsigned char ucBlockTerminator;
        qGCEStream.Read(ucBlockTerminator);
        CONDITIONAL_THROW_EXCEPTION(( ucBlockTerminator == 0 ),ET_FORMAT_MISMATCH);

        qGCEStream.Finalize();

        iNextReadPos += 6;
    }

    unsigned char ucByte;
    unsigned short usImageLeftPosition, usImageTopPosition, usImageWidth, usImageHeight;
    bool bHasLocalColorTable, bInterlaced, bFitsLSD;
    REACH_IMAGE_DESCRIPTOR_AND_READ_IT_WITH_LOCAL_COLOR_TABLE(false);
    const RasterImage::Palette& rqActiveColorTable =
        ( !bHasLocalColorTable ? m_qGlobalColorTable : qLocalColorTable );

    // the next block have to be Table Based Image Data
    int iSuccess = LoadImageData(iNextReadPos,usImageWidth,usImageHeight,bInterlaced,rqActiveColorTable,
        ( !m_bIsAnimated ? 0 : &qAnimationFrame ),iBytestreamSize,false,rspqImage);
    PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

Exit:  // dummy label

    m_bDraftImageAllocated = false;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::LoadAnimation (Animation& rqAnimation, int iDynamicBytestreamSize)
{
    assert( m_bIsAnimated );
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

    bool bSuccess, bHasLocalColorTable, bInterlaced, bFitsLSD, bBrokenBeforeImageRead, bLastFrameWithNoGCE;
    unsigned char ucBlockSize, ucTransparentColorIndex, ucBlockTerminator, ucByte;
    unsigned short usDelayTime, usImageLeftPosition, usImageTopPosition, usImageWidth, usImageHeight;
    int iNextReadPos, iDisposalMethod, iUserInputFlag, iTransparentColorFlag, iSuccess;
    if ( !m_bHasAnimationRestorePoint )
    {
        bLastFrameWithNoGCE = false;
        iNextReadPos = m_iFirstIDOrGCEPosition;
    }
    else
    {
        bLastFrameWithNoGCE = m_bRestoreLastFrameWithNoGCE;
        iNextReadPos = m_iNextReadPosAfterLastAnimationFrame;

        if ( m_iAnimationRestorePointId == 0 )
            goto Restoring0;
        else  // m_iAnimationRestorePointId = 1
            goto Restoring1;
    }
    for (/**/; /**/; /**/)
    {
        rqAnimation.AdvanceQuantity(1);

        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            // draft image
            RasterImage::Palette qDIPalette(1);
            qDIPalette[0] = ms_qDraftColor;
            rqAnimation.GetLast().ImagePtr = new RasterImage(RasterImage::IT_PALETTED,
                m_iLogicalScreenWidth,m_iLogicalScreenHeight,qDIPalette);
            MemSet(rqAnimation.GetLast().ImagePtr->GetImage(),0,
                m_iLogicalScreenHeight*rqAnimation.GetLast().ImagePtr->GetRowStride());

            // save the restore point
            m_bRestoreLastFrameWithNoGCE = bLastFrameWithNoGCE;
            m_iNextReadPosAfterLastAnimationFrame = iNextReadPos;
            m_iAnimationRestorePointId = 0;
            m_bHasAnimationRestorePoint = true;
        }
Restoring0:

        bBrokenBeforeImageRead = true;

        AnimationFrame& rqAnimationFrame = rqAnimation.GetLast();

        {
            if ( !bLastFrameWithNoGCE )
            {
                // Graphic Control Extension
                iNextReadPos += 2;
                Stream qGCEStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,6,true,bSuccess,iBytestreamSize);
                ANIM_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

                qGCEStream.Read(ucBlockSize);
                if ( ucBlockSize != 4 )
                {
                    // a concession
                    goto Exit;
                }

                qGCEStream.OffsetBitPosition(3);

                qGCEStream.ReadUBits(iDisposalMethod,3);
                switch ( iDisposalMethod )
                {
                case 0:
                    rqAnimationFrame.DisposalMethod = DT_UNSPECIFIED;
                    break;
                case 1:
                    rqAnimationFrame.DisposalMethod = DT_LEAVE;
                    break;
                case 2:
                    rqAnimationFrame.DisposalMethod = DT_RESTORE_TO_BACKGROUND;
                    break;
                case 3:
                    rqAnimationFrame.DisposalMethod = ( rqAnimation.GetQuantity() != 1 ? DT_RESTORE_TO_PREVIOUS :
                        DT_RESTORE_TO_BACKGROUND );
                    break;
                default:
                    rqAnimationFrame.DisposalMethod = DT_LEAVE;  // a concession
                }

                qGCEStream.ReadUBits(iUserInputFlag,1);
                rqAnimationFrame.UserInputExpected = ( iUserInputFlag == 1 );

                qGCEStream.ReadUBits(iTransparentColorFlag,1);
                rqAnimationFrame.m_bHasTransparency = ( iTransparentColorFlag == 1 );

                qGCEStream.Read(usDelayTime);
                rqAnimationFrame.DelayTime = usDelayTime;

                qGCEStream.Read(ucTransparentColorIndex);
                rqAnimationFrame.m_iTransparentColorIndex = ucTransparentColorIndex;

                qGCEStream.Read(ucBlockTerminator);
                if ( ucBlockTerminator != 0 )
                {
                    // a concession
                    goto Exit;
                }

                qGCEStream.Finalize();

                iNextReadPos += 6;
            }
            else
            {
                // copy from the previous frame
                const AnimationFrame& rqPrevFrame = rqAnimation[rqAnimation.GetQuantity()-2];

                rqAnimationFrame.DisposalMethod = rqPrevFrame.DisposalMethod;
                rqAnimationFrame.UserInputExpected = rqPrevFrame.UserInputExpected;
                rqAnimationFrame.DelayTime = rqPrevFrame.DelayTime;
                rqAnimationFrame.m_bHasTransparency = rqPrevFrame.m_bHasTransparency;
                rqAnimationFrame.m_iTransparentColorIndex = rqPrevFrame.m_iTransparentColorIndex;
            }

            // reach and read Image Descriptor
            RasterImage::Palette qLocalColorTable;
            REACH_IMAGE_DESCRIPTOR_AND_READ_IT_WITH_LOCAL_COLOR_TABLE(true);
            const RasterImage::Palette& rqActiveColorTable =
                ( !bHasLocalColorTable ? m_qGlobalColorTable : qLocalColorTable );

            // the next block have to be Table Based Image Data
            iSuccess = LoadImageData(iNextReadPos,usImageWidth,usImageHeight,bInterlaced,rqActiveColorTable,
                &rqAnimationFrame,iBytestreamSize,true,rqAnimationFrame.ImagePtr);
            if ( iSuccess == -1 )
                return false;
            if ( iSuccess == 1 )
            {
                // a concession
                goto Exit;
            }

            if ( !bFitsLSD )
            {
                // a concession
                int iSIWidth = ( usImageLeftPosition + usImageWidth > m_iLogicalScreenWidth ?
                    m_iLogicalScreenWidth - usImageLeftPosition : usImageWidth );
                int iSIHeight = ( usImageTopPosition + usImageHeight > m_iLogicalScreenHeight ?
                    m_iLogicalScreenHeight - usImageTopPosition : usImageHeight );
                rqAnimationFrame.ImagePtr = rqAnimationFrame.ImagePtr->GetSubimage(0,0,iSIWidth,iSIHeight);
            }

            rqAnimationFrame.PositionX = usImageLeftPosition;
            rqAnimationFrame.PositionY = usImageTopPosition;
        }

        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            // save the restore point
            m_bRestoreLastFrameWithNoGCE = bLastFrameWithNoGCE;
            m_iNextReadPosAfterLastAnimationFrame = iNextReadPos;
            m_iAnimationRestorePointId = 1;
            m_bHasAnimationRestorePoint = true;
        }
Restoring1:

        bBrokenBeforeImageRead = false;

        // reach the next frame's Graphic Control Extension or Trailer
        for (/**/; /**/; /**/)
        {
            Stream qByteStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize);
            ANIM_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            qByteStream.Read(ucByte);
            qByteStream.Finalize();
            if ( ucByte == 0 )
            {
                // a concession
            }
            else if ( ucByte == 33 )
            {
                // Extension Introducer
                Stream qLabelStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize);
                ANIM_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                qLabelStream.Read(ucByte);
                qLabelStream.Finalize();
                if ( ucByte == 1 )
                {
                    // Plain Text Extension
                    // a concession
                    goto Exit;
                }
                else if ( ucByte == 249 )
                {
                    // Graphic Control Extension
                    iNextReadPos -= 2;
                    break;
                }
                ELSE_IF_COMMENT_OR_APPLICATION_EXTENSION(true)
                else
                {
                    // unknown extension
                    // a concession
                    goto Exit;
                }
            }
            else if ( ucByte == 59 )
            {
                // Trailer
                goto Exit;
            }
            else if ( ucByte == 44 )
            {
                // Image Descriptor; it seems that the next frame will be the last one
                bLastFrameWithNoGCE = true;
                iNextReadPos--;
                break;
            }
            else
            {
                // unknown/unexpected block label
                // a concession
                goto Exit;
            }
        }
    }
Exit:

    m_bHasAnimationRestorePoint = false;

    if ( bBrokenBeforeImageRead )
        rqAnimation.AdvanceQuantity(-1);
    CONDITIONAL_THROW_EXCEPTION(( !rqAnimation.IsEmpty() ),ET_FORMAT_MISMATCH);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::GenerateInputAnimation (const RasterImageSequence& rqImageSequence, Animation& rqAnimation,
    bool bCropFrameImages, bool bWriteUnchangingPixelsAsTransparency, float fRequiredPixelSimilarity)
{
    assert( rqImageSequence.GetFrameQuantity() != 0 );
    assert( rqImageSequence.GetFrameQuantity() == rqAnimation.GetQuantity() );
    assert( rqImageSequence.GetFrameImageType() == RasterImage::CMT_PALETTED );
    assert( 0.0f <= fRequiredPixelSimilarity && fRequiredPixelSimilarity <= 1.0f );

    int iFrameQuantity = rqImageSequence.GetFrameQuantity();
    int iScreenWidth = rqImageSequence.GetScreenWidth();
    int iScreenHeight = rqImageSequence.GetScreenHeight();
    float fUsedRequiredPixelSimilarity =
        ms_fPixelSimilarityBaseMapper + (1.0f - ms_fPixelSimilarityBaseMapper)*fRequiredPixelSimilarity;

    // precise transparency information
    StillArray<bool> qTransparencyInInputFrames(iFrameQuantity);
    for (int i0 = 0; i0 < iFrameQuantity; i0++)
    {
        const RasterImagePtr& rspqInputFrameImage = rqImageSequence.GetFrameImage(i0);

        qTransparencyInInputFrames[i0] = false;
        if ( rspqInputFrameImage->HasVisibilityMask() )
        {
            for (int i1 = 0; i1 < rspqInputFrameImage->GetVisibilityMask().GetQuantity(); i1++)
            {
                if ( rspqInputFrameImage->GetVisibilityMask()[i1] == 0 )
                {
                    qTransparencyInInputFrames[i0] = true;
                    break;
                }
            }
        }
    }

    // generate frames
    bool bThisIsLastOne, bPrevHasTransparency, bThisHasTransparency, bAtLeastOne,
        bUnchangingPixelsAsTransparencyWritten;
    Bound2i qBound;
    int iIndex;
    for (int i0 = -1, i1 = 0, i2 = 1; i1 < iFrameQuantity; i0++, i1++, i2++)
    {
        const RasterImagePtr& rspqThisInputFrameImage = rqImageSequence.GetFrameImage(i1);
        AnimationFrame& rqThisAnimationFrame = rqAnimation[i1];
        RasterImagePtr& rspqThisOutputFrameImage = rqThisAnimationFrame.ImagePtr;
        bThisHasTransparency = qTransparencyInInputFrames[i1];
        bThisIsLastOne = ( i2 == iFrameQuantity );

        bUnchangingPixelsAsTransparencyWritten = false;
        if ( i1 == 0 )
        {
            if ( !bThisHasTransparency || !bCropFrameImages )
            {
Label0:
                rspqThisOutputFrameImage = rspqThisInputFrameImage->GetDuplicate();
                rqThisAnimationFrame.PositionX = 0;
                rqThisAnimationFrame.PositionY = 0;
            }
            else
            {
                qBound.MinX = qBound.MinY = Mathi::MAX_INT;
                qBound.MaxX = qBound.MaxY = Mathi::MIN_INT;
                bAtLeastOne = false;
                for (int iX = 0; iX < iScreenWidth; iX++)
                {
                    for (int iY = 0; iY < iScreenHeight; iY++)
                    {
                        if ( rspqThisInputFrameImage->GetVisibilityMask()[iY*iScreenWidth+iX] == 1 )
                        {
                            if ( iX < qBound.MinX )
                                qBound.MinX = iX;
                            if ( iX > qBound.MaxX )
                                qBound.MaxX = iX;
                            if ( iY < qBound.MinY )
                                qBound.MinY = iY;
                            if ( iY > qBound.MaxY )
                                qBound.MaxY = iY;

                            bAtLeastOne = true;
                        }
                    }
                }
                if ( !bAtLeastOne )
                    goto Label0;

                rspqThisOutputFrameImage = rspqThisInputFrameImage->GetSubimage(qBound.MinX,qBound.MinY,
                    qBound.MaxX-qBound.MinX+1,qBound.MaxY-qBound.MinY+1);
                rqThisAnimationFrame.PositionX = qBound.MinX;
                rqThisAnimationFrame.PositionY = qBound.MinY;
            }
        }
        else
        {
            const RasterImagePtr& rspqPrevInputFrameImage = rqImageSequence.GetFrameImage(i0);
            AnimationFrame& rqPrevAnimationFrame = rqAnimation[i0];
            bPrevHasTransparency = qTransparencyInInputFrames[i0];

            if ( !bThisHasTransparency )
            {
                if ( !bPrevHasTransparency )
                {
                    if ( !bCropFrameImages )
                    {
Label1:
                        rspqThisOutputFrameImage = rspqThisInputFrameImage->GetDuplicate();
                        rqThisAnimationFrame.PositionX = 0;
                        rqThisAnimationFrame.PositionY = 0;
                    }
                    else
                    {
                        qBound.MinX = qBound.MinY = Mathi::MAX_INT;
                        qBound.MaxX = qBound.MaxY = Mathi::MIN_INT;
                        bAtLeastOne = false;
                        for (int iX = 0; iX < iScreenWidth; iX++)
                        {
                            for (int iY = 0; iY < iScreenHeight; iY++)
                            {
                                if ( !ArePixelsSimilar(iX,iY,iX,iY,rspqThisInputFrameImage,rspqPrevInputFrameImage,
                                     fUsedRequiredPixelSimilarity) )
                                {
                                    if ( iX < qBound.MinX )
                                        qBound.MinX = iX;
                                    if ( iX > qBound.MaxX )
                                        qBound.MaxX = iX;
                                    if ( iY < qBound.MinY )
                                        qBound.MinY = iY;
                                    if ( iY > qBound.MaxY )
                                        qBound.MaxY = iY;

                                    bAtLeastOne = true;
                                }
                            }
                        }
                        if ( !bAtLeastOne )
                            goto Label1;

                        rspqThisOutputFrameImage = rspqThisInputFrameImage->GetSubimage(qBound.MinX,qBound.MinY,
                            qBound.MaxX-qBound.MinX+1,qBound.MaxY-qBound.MinY+1);
                        rqThisAnimationFrame.PositionX = qBound.MinX;
                        rqThisAnimationFrame.PositionY = qBound.MinY;
                    }

                    if ( bWriteUnchangingPixelsAsTransparency && rspqThisOutputFrameImage->GetPaletteSize() != 256 )
                    {
                        rspqThisOutputFrameImage->GetVisibilityMask().SetQuantity(
                            rspqThisOutputFrameImage->GetWidth()*rspqThisOutputFrameImage->GetHeight());

                        bAtLeastOne = false;
                        for (int iX = 0; iX < rspqThisOutputFrameImage->GetWidth(); iX++)
                        {
                            for (int iY = 0; iY < rspqThisOutputFrameImage->GetHeight(); iY++)
                            {
                                iIndex = iY*rspqThisOutputFrameImage->GetWidth() + iX;
                                if ( !ArePixelsSimilar(iX,iY,
                                     rqThisAnimationFrame.PositionX+iX,rqThisAnimationFrame.PositionY+iY,
                                     rspqThisOutputFrameImage,rspqPrevInputFrameImage,
                                     fUsedRequiredPixelSimilarity) )
                                {
                                    rspqThisOutputFrameImage->GetVisibilityMask()[iIndex] = 1;
                                }
                                else
                                {
                                    rspqThisOutputFrameImage->GetVisibilityMask()[iIndex] = 0;
                                    bAtLeastOne = true;
                                }
                            }
                        }

                        if ( bAtLeastOne )
                            bUnchangingPixelsAsTransparencyWritten = true;
                        else
                            rspqThisOutputFrameImage->GetVisibilityMask().RemoveAll();
                    }
                }
                else
                {
                    rspqThisOutputFrameImage = rspqThisInputFrameImage->GetDuplicate();
                    rqThisAnimationFrame.PositionX = 0;
                    rqThisAnimationFrame.PositionY = 0;
                }

                rqPrevAnimationFrame.DisposalMethod = DT_LEAVE;
            }
            else
            {
                if ( !bCropFrameImages )
                {
Label2:
                    rspqThisOutputFrameImage = rspqThisInputFrameImage->GetDuplicate();
                    rqThisAnimationFrame.PositionX = 0;
                    rqThisAnimationFrame.PositionY = 0;
                }
                else
                {
                    qBound.MinX = qBound.MinY = Mathi::MAX_INT;
                    qBound.MaxX = qBound.MaxY = Mathi::MIN_INT;
                    bAtLeastOne = false;
                    for (int iX = 0; iX < iScreenWidth; iX++)
                    {
                        for (int iY = 0; iY < iScreenHeight; iY++)
                        {
                            if ( rspqThisInputFrameImage->GetVisibilityMask()[iY*iScreenWidth+iX] == 1 )
                            {
                                if ( iX < qBound.MinX )
                                    qBound.MinX = iX;
                                if ( iX > qBound.MaxX )
                                    qBound.MaxX = iX;
                                if ( iY < qBound.MinY )
                                    qBound.MinY = iY;
                                if ( iY > qBound.MaxY )
                                    qBound.MaxY = iY;

                                bAtLeastOne = true;
                            }
                        }
                    }
                    if ( !bAtLeastOne )
                        goto Label2;

                    rspqThisOutputFrameImage = rspqThisInputFrameImage->GetSubimage(qBound.MinX,qBound.MinY,
                        qBound.MaxX-qBound.MinX+1,qBound.MaxY-qBound.MinY+1);
                    rqThisAnimationFrame.PositionX = qBound.MinX;
                    rqThisAnimationFrame.PositionY = qBound.MinY;
                }

                if ( !bPrevHasTransparency || !bWriteUnchangingPixelsAsTransparency
                ||   rspqThisInputFrameImage->GetVisibilityMask() != rspqPrevInputFrameImage->GetVisibilityMask() )
                {
                    rqPrevAnimationFrame.DisposalMethod = DT_RESTORE_TO_BACKGROUND;
                }
                else
                {
                    bAtLeastOne = false;
                    for (int iX = 0; iX < rspqThisOutputFrameImage->GetWidth(); iX++)
                    {
                        for (int iY = 0; iY < rspqThisOutputFrameImage->GetHeight(); iY++)
                        {
                            iIndex = iY*rspqThisOutputFrameImage->GetWidth() + iX;
                            if ( ArePixelsSimilar(iX,iY,
                                 rqThisAnimationFrame.PositionX+iX,rqThisAnimationFrame.PositionY+iY,
                                 rspqThisOutputFrameImage,rspqPrevInputFrameImage,
                                 fUsedRequiredPixelSimilarity) )
                            {
                                rspqThisOutputFrameImage->GetVisibilityMask()[iIndex] = 0;
                                bAtLeastOne = true;
                            }
                        }
                    }

                    if ( bAtLeastOne )
                        bUnchangingPixelsAsTransparencyWritten = true;

                    rqPrevAnimationFrame.DisposalMethod = DT_LEAVE;
                }
            }
        }
        if ( !bThisHasTransparency && rspqThisOutputFrameImage->HasVisibilityMask()
        &&   !bUnchangingPixelsAsTransparencyWritten )
        {
            rspqThisOutputFrameImage->GetVisibilityMask().RemoveAll();
        }
    }
    rqAnimation.GetLast().DisposalMethod = DT_RESTORE_TO_BACKGROUND;
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::GenerateOutputImageSequence (const Animation& rqAnimation, RasterImageSequence& rqImageSequence,
    StillArray<RasterImagePtr>& rqForRestToPrevImages, bool bLastFrameOnly)
{
    rqImageSequence.SetFrameQuantity(rqAnimation.GetQuantity());
    rqForRestToPrevImages.SetQuantity(rqAnimation.GetQuantity());

    int iLogicalScreenWidth = rqImageSequence.GetScreenWidth();
    int iLogicalScreenHeight = rqImageSequence.GetScreenHeight();

    unsigned char* aucRow0;
    unsigned char* pucComp0;
    unsigned char* aucRow1;
    unsigned char* pucComp1;
    int iStart = ( !bLastFrameOnly ? 0 : rqAnimation.GetQuantity() - 1 );
    for (int i0 = iStart-1, i1 = iStart; i1 < rqAnimation.GetQuantity(); i0 = i1++)
    {
        const AnimationFrame& rqThisFrame = rqAnimation[i1];
        RasterImagePtr spqImage;

        if ( i1 == 0 )
        {
            spqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,iLogicalScreenWidth,iLogicalScreenHeight);
            spqImage->GetVisibilityMask().SetQuantity(iLogicalScreenWidth*iLogicalScreenHeight);
            MemSet(spqImage->GetVisibilityMask(),0,spqImage->GetVisibilityMask().GetQuantity());

            RasterImageConversion qRIC(rqThisFrame.ImagePtr,RasterImage::IT_RGB_8_8_8);
            RasterImagePtr spqSubimage = qRIC.GetConverted();
            if ( !spqSubimage->HasVisibilityMask() )
            {
                spqSubimage->GetVisibilityMask().SetQuantity(spqSubimage->GetWidth()*spqSubimage->GetHeight());
                MemSet(spqSubimage->GetVisibilityMask(),1,spqSubimage->GetVisibilityMask().GetQuantity());
            }

            spqImage->SetSubimage(spqSubimage,rqThisFrame.PositionX,rqThisFrame.PositionY);
        }
        else
        {
            const AnimationFrame& rqPrevFrame = rqAnimation[i0];

            spqImage = rqImageSequence.GetFrameImage(i0)->GetDuplicate();

            // handle Disposal Method
            if ( rqPrevFrame.DisposalMethod == DT_RESTORE_TO_BACKGROUND )
            {
                for (int iY = 0; iY < rqPrevFrame.ImagePtr->GetHeight(); iY++)
                {
                    MemSet(spqImage->GetVisibilityMask()+(rqPrevFrame.PositionY+iY)*iLogicalScreenWidth+
                        rqPrevFrame.PositionX,0,rqPrevFrame.ImagePtr->GetWidth());
                }
            }
            else if ( rqPrevFrame.DisposalMethod == DT_RESTORE_TO_PREVIOUS )
            {
                spqImage->SetSubimage(rqForRestToPrevImages[i0]->GetSubimage(
                    rqPrevFrame.PositionX,rqPrevFrame.PositionY,
                    rqPrevFrame.ImagePtr->GetWidth(),rqPrevFrame.ImagePtr->GetHeight()),
                    rqPrevFrame.PositionX,rqPrevFrame.PositionY);
            }

            // remember the current image for "restore to previous" ability
            rqForRestToPrevImages[i1] = spqImage->GetDuplicate();

            // draw this frame image
            aucRow0 = (unsigned char*)rqThisFrame.ImagePtr->GetImage();
            pucComp0 = aucRow0;
            aucRow1 = (unsigned char*)spqImage->GetRow(rqThisFrame.PositionY);
            pucComp1 = aucRow1 + rqThisFrame.PositionX*3;
            for (int iY = 0; iY < rqThisFrame.ImagePtr->GetHeight(); iY++)
            {
                for (int iX = 0; iX < rqThisFrame.ImagePtr->GetWidth(); iX++)
                {
                    if ( !(rqThisFrame.ImagePtr->HasVisibilityMask()
                    &&   rqThisFrame.ImagePtr->GetVisibilityMask()[iY*rqThisFrame.ImagePtr->GetWidth()+iX] == 0) )
                    {
                        const PaletteColor& rqPaletteColor =
                            rqThisFrame.ImagePtr->GetPaletteColor((int)*pucComp0);

                        pucComp1[0] = rqPaletteColor.R();
                        pucComp1[1] = rqPaletteColor.G();
                        pucComp1[2] = rqPaletteColor.B();

                        spqImage->GetVisibilityMask()[(rqThisFrame.PositionY+iY)*iLogicalScreenWidth+
                            rqThisFrame.PositionX+iX] = 1;
                    }
                    pucComp0++;
                    pucComp1 += 3;
                }

                aucRow0 += rqThisFrame.ImagePtr->GetRowStride();
                pucComp0 = aucRow0;
                aucRow1 += spqImage->GetRowStride();
                pucComp1 = aucRow1 + rqThisFrame.PositionX*3;
            }
        }

        rqImageSequence.SetFrameImage(spqImage,i1);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::WriteImage (Stream& rqStream)
{
    // Header
    const char* acSignatureWithVersion = "GIF89a";
    rqStream.Write(6,acSignatureWithVersion);

    // Logical Screen Descriptor.
    unsigned short usLogicalScreenWidth = m_iLogicalScreenWidth;
    unsigned short usLogicalScreenHeight = m_iLogicalScreenHeight;
    rqStream.Write(usLogicalScreenWidth);
    rqStream.Write(usLogicalScreenHeight);

    rqStream.WriteBits(1,1);

    int iColorResolution = m_iBitsPerPixel - 1;
    rqStream.WriteBits(iColorResolution,3);

    rqStream.WriteBits(0,1);

    int iSizeOfGlobalColorTable = Storage::GetUBitQuantity(m_qGlobalColorTable.GetQuantity()-1) - 1;
    rqStream.WriteBits(iSizeOfGlobalColorTable,3);

    unsigned char ucBackgroundColorIndex = 0;
    rqStream.Write(ucBackgroundColorIndex);

    unsigned char ucPixelAspectRatio = 0;
    rqStream.Write(ucPixelAspectRatio);

    // Global Color Table
    for (int i = 0; i < m_qGlobalColorTable.GetQuantity(); i++)
    {
        const PaletteColor& rqPaletteColor = m_qGlobalColorTable[i];

        rqStream.Write(rqPaletteColor.R());
        rqStream.Write(rqPaletteColor.G());
        rqStream.Write(rqPaletteColor.B());
    }
    char acZeroes[3] = {0, 0, 0};
    int iDummyColorQuantity = (1 << (iSizeOfGlobalColorTable + 1)) - m_qGlobalColorTable.GetQuantity();
    for (int i = 0; i < iDummyColorQuantity; i++)
        rqStream.Write(3,acZeroes);

    if ( m_spqImageSaveTo->HasVisibilityMask() )
    {
        // Graphic Control Extension.
        unsigned char ucExtensionIntroducer = 33;
        rqStream.Write(ucExtensionIntroducer);

        unsigned char ucGraphicControlLabel = 249;
        rqStream.Write(ucGraphicControlLabel);

        unsigned char ucBlockSize = 4;
        rqStream.Write(ucBlockSize);

        rqStream.WriteBits(0,7);

        rqStream.WriteBits(1,1);

        unsigned short usDelayTime = 0;
        rqStream.Write(usDelayTime);

        unsigned char ucTransparentColorIndex = m_spqImageSaveTo->GetPaletteSize();
        rqStream.Write(ucTransparentColorIndex);

        unsigned char ucBlockTerminator = 0;
        rqStream.Write(ucBlockTerminator);
    }

    // Image Descriptor.
    unsigned char ucImageSeparator = 44;
    rqStream.Write(ucImageSeparator);

    unsigned short usImageLeftPosition = 0;
    unsigned short usImageTopPosition = 0;
    rqStream.Write(usImageLeftPosition);
    rqStream.Write(usImageTopPosition);

    unsigned short usImageWidth = m_iLogicalScreenWidth;
    unsigned short usImageHeight = m_iLogicalScreenHeight;
    rqStream.Write(usImageWidth);
    rqStream.Write(usImageHeight);

    rqStream.WriteBits(0,1);

    int iInterlaceFlag = ( m_bTitleImageIsInterlaced ? 1 : 0 );
    rqStream.WriteBits(iInterlaceFlag,1);

    rqStream.WriteBits(0,6);

    // for a progress bar
    ProgressProbe qProgress(this);

    WriteImageData(m_spqImageSaveTo,m_iBitsPerPixel,m_bTitleImageIsInterlaced,qProgress,1.0f,rqStream);

    // Trailer
    unsigned char ucTrailer = 59;
    rqStream.Write(ucTrailer);
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::WriteAnimation (Stream& rqStream)
{
    // Header
    const char* acSignatureWithVersion = "GIF89a";
    rqStream.Write(6,acSignatureWithVersion);

    // Logical Screen Descriptor.
    unsigned short usLogicalScreenWidth = m_iLogicalScreenWidth;
    unsigned short usLogicalScreenHeight = m_iLogicalScreenHeight;
    rqStream.Write(usLogicalScreenWidth);
    rqStream.Write(usLogicalScreenHeight);

    rqStream.WriteBits(1,1);  // always write some table as Global Color Table

    int iColorResolution = m_iBitsPerPixel - 1;
    rqStream.WriteBits(iColorResolution,3);

    rqStream.WriteBits(0,1);

    // Global Color Table or first Local Color Table
    const RasterImage::Palette& rqFirstColorTable =
        ( m_bHasGlobalColorTable ? m_qGlobalColorTable : m_qLocalColorTablesSaveTo.GetFirst() );

    int iSizeOfGlobalColorTable = Storage::GetUBitQuantity(rqFirstColorTable.GetQuantity()-1) - 1;
    rqStream.WriteBits(iSizeOfGlobalColorTable,3);

    unsigned char ucBackgroundColorIndex = 0;
    rqStream.Write(ucBackgroundColorIndex);

    unsigned char ucPixelAspectRatio = 0;
    rqStream.Write(ucPixelAspectRatio);

    for (int i = 0; i < rqFirstColorTable.GetQuantity(); i++)
    {
        const PaletteColor& rqPaletteColor = rqFirstColorTable[i];

        rqStream.Write(rqPaletteColor.R());
        rqStream.Write(rqPaletteColor.G());
        rqStream.Write(rqPaletteColor.B());
    }
    char acZeroes[3] = {0, 0, 0};
    int iDummyColorQuantity = (1 << (iSizeOfGlobalColorTable + 1)) - rqFirstColorTable.GetQuantity();
    for (int i = 0; i < iDummyColorQuantity; i++)
        rqStream.Write(3,acZeroes);

    if ( m_iLoopQuantity != 1 )
    {
        // Netscape Navigator's "N-Loops" Application Extension.
        unsigned char aucBeginning[3] = {33, 255, 11};
        rqStream.Write(3,aucBeginning);

        const char* acExSign = "NETSCAPE2.0";
        rqStream.Write(11,acExSign);

        unsigned char aucBeforeLQ[2] = {3, 1};
        rqStream.Write(2,aucBeforeLQ);

        unsigned short usLoopQuantity = ( m_iLoopQuantity == 0 ? 0 : m_iLoopQuantity - 1 );
        rqStream.Write(usLoopQuantity);

        unsigned char ucDSBT = 0;
        rqStream.Write(ucDSBT);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    float fFrameScopeCoefficient = 1.0f/m_qAnimationSaveTo.GetQuantity();

    bool bLocalColorTable;
    unsigned char ucTransparentColorIndex;
    unsigned short usDelayTime, usImageLeftPosition, usImageTopPosition, usImageWidth, usImageHeight;
    int iUsedBitsPerPixel, iDisposalMethod, iUserInputFlag, iTransparentColorFlag, iLocalColorTableFlag,
        iSizeOfLocalColorTable;
    unsigned char ucExtensionIntroducer = 33;
    unsigned char ucGraphicControlLabel = 249;
    unsigned char ucBlockSize = 4;
    unsigned char ucBlockTerminator = 0;
    unsigned char ucImageSeparator = 44;
    for (int i0 = 0; i0 < m_qAnimationSaveTo.GetQuantity(); i0++)
    {
        const AnimationFrame& rqAnimationFrame = m_qAnimationSaveTo[i0];
        RasterImage::Palette& rqUsedColorTable = ( m_bHasGlobalColorTable ? m_qGlobalColorTable :
            m_qLocalColorTablesSaveTo[i0] );
        iUsedBitsPerPixel = ( m_bHasGlobalColorTable ? m_iBitsPerPixel : m_qFrameBitsPerPixelSaveTo[i0] );

        // Graphic Control Extension.
        rqStream.Write(ucExtensionIntroducer);
        rqStream.Write(ucGraphicControlLabel);
        rqStream.Write(ucBlockSize);

        rqStream.WriteBits(0,3);

        switch ( rqAnimationFrame.DisposalMethod )
        {
        case DT_UNSPECIFIED:
            iDisposalMethod = 0;
            break;
        case DT_LEAVE:
            iDisposalMethod = 1;
            break;
        case DT_RESTORE_TO_BACKGROUND:
            iDisposalMethod = 2;
            break;
        case DT_RESTORE_TO_PREVIOUS:
            iDisposalMethod = 3;
            break;
        }
        rqStream.WriteBits(iDisposalMethod,3);

        iUserInputFlag = ( rqAnimationFrame.UserInputExpected ? 1 : 0 );
        rqStream.WriteBits(iUserInputFlag,1);

        iTransparentColorFlag = ( rqAnimationFrame.ImagePtr->HasVisibilityMask() ? 1 : 0 );
        rqStream.WriteBits(iTransparentColorFlag,1);

        usDelayTime = rqAnimationFrame.DelayTime;
        rqStream.Write(usDelayTime);

        ucTransparentColorIndex = rqAnimationFrame.ImagePtr->GetPaletteSize();
        rqStream.Write(ucTransparentColorIndex);

        rqStream.Write(ucBlockTerminator);

        // Image Descriptor.
        rqStream.Write(ucImageSeparator);

        usImageLeftPosition = rqAnimationFrame.PositionX;
        usImageTopPosition = rqAnimationFrame.PositionY;
        rqStream.Write(usImageLeftPosition);
        rqStream.Write(usImageTopPosition);

        usImageWidth = rqAnimationFrame.ImagePtr->GetWidth();
        usImageHeight = rqAnimationFrame.ImagePtr->GetHeight();
        rqStream.Write(usImageWidth);
        rqStream.Write(usImageHeight);

        bLocalColorTable = ( !m_bHasGlobalColorTable && i0 != 0 );
        iLocalColorTableFlag = ( bLocalColorTable ? 1 : 0 );
        rqStream.WriteBits(iLocalColorTableFlag,1);

        rqStream.WriteBits(0,1);  // always non-interlaced

        rqStream.WriteBits(0,3);

        iSizeOfLocalColorTable =
            ( bLocalColorTable ? Storage::GetUBitQuantity(rqUsedColorTable.GetQuantity()-1) - 1 : 0 );
        rqStream.WriteBits(iSizeOfLocalColorTable,3);

        if ( bLocalColorTable )
        {
            // Local Color Table.
            for (int i1 = 0; i1 < rqUsedColorTable.GetQuantity(); i1++)
            {
                const PaletteColor& rqPaletteColor = rqUsedColorTable[i1];

                rqStream.Write(rqPaletteColor.R());
                rqStream.Write(rqPaletteColor.G());
                rqStream.Write(rqPaletteColor.B());
            }
            iDummyColorQuantity = (1 << (iSizeOfLocalColorTable + 1)) - rqUsedColorTable.GetQuantity();
            for (int i1 = 0; i1 < iDummyColorQuantity; i1++)
                rqStream.Write(3,acZeroes);
        }

        // actual frame image
        WriteImageData(rqAnimationFrame.ImagePtr,iUsedBitsPerPixel,false,qProgress,fFrameScopeCoefficient,rqStream);
    }

    // Trailer
    unsigned char ucTrailer = 59;
    rqStream.Write(ucTrailer);
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::WriteImageData (const RasterImagePtr& rspqImage, int iBitsPerPixel, bool bInterlaced,
    ProgressProbe& rqProgress, float fScopeCoefficient, Stream& rqStream)
{
    int iLzwMinimumCodeSize = ( iBitsPerPixel == 1 ? 2 : iBitsPerPixel );

    rqProgress.SetAdvance(fScopeCoefficient*0.5f/rspqImage->GetHeight());

    int iImageSizeInBits = rspqImage->GetWidth()*rspqImage->GetHeight()*iLzwMinimumCodeSize;
    int iImageSize = ( (iImageSizeInBits & 7) == 0 ? iImageSizeInBits/8 : iImageSizeInBits/8 + 1 );
    StillArray<char> qDecompressedData(iImageSize);
    int iIndex;
    unsigned char* pucRow;
    unsigned char* pucPixel;
    int iBytePos = 0;
    int iBitRatio = 0;
    if ( !bInterlaced )
    {
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucPixel = pucRow;
        for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
        {
            for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
            {
                if ( !(rspqImage->HasVisibilityMask()
                &&   rspqImage->GetVisibilityMask()[iY*rspqImage->GetWidth()+iX] == 0) )
                {
                    iIndex = *pucPixel;
                }
                else
                {
                    iIndex = rspqImage->GetPaletteSize();
                }
                pucPixel++;
                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,iLzwMinimumCodeSize,iIndex);
            }

            pucRow += rspqImage->GetRowStride();
            pucPixel = pucRow;

            rqProgress.MakeAdvance();
        }
    }
    else
    {
        int iRow;
        int iPass = 0;
        int iRowWithinPass = 0;
        for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
        {
            iRow = GetIrlcRow(iPass,iRowWithinPass);
            if ( iRow >= rspqImage->GetHeight() )
            {
                iRowWithinPass = 0;

                // skip empty passes if any; GIF interlacing scheme should keep iPass in the proper range
                do
                {
                    iPass++;
                } while ( !(ms_aiIrlcStrt[iPass] < rspqImage->GetHeight()) );

                iRow = GetIrlcRow(iPass,iRowWithinPass);
            }
            pucPixel = (unsigned char*)rspqImage->GetRow(iRow);

            for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
            {
                if ( !(rspqImage->HasVisibilityMask()
                &&   rspqImage->GetVisibilityMask()[iRow*rspqImage->GetWidth()+iX] == 0) )
                {
                    iIndex = *pucPixel;
                }
                else
                {
                    iIndex = rspqImage->GetPaletteSize();
                }
                pucPixel++;
                iBytePos += Storage::WriteBits(qDecompressedData+iBytePos,iBitRatio,iLzwMinimumCodeSize,iIndex);
            }

            iRowWithinPass++;

            rqProgress.MakeAdvance();
        }
    }

    // pack
    StillArray<char> qCompressedData;
    CompressionLzw qLzw(CompressionLzw::ET_GIF);
    qLzw.SetSymbolLength(iLzwMinimumCodeSize);
    qLzw.RedirectProgressTo(this,rqProgress,fScopeCoefficient*0.5f);
    qLzw.Compress(qDecompressedData,qDecompressedData.GetQuantity(),qCompressedData);
    qDecompressedData.RemoveAll();

    // reverse bit orders within bytes
    unsigned char* aucCompressedData = (unsigned char*)qCompressedData.GetElements();
    for (int i = 0; i < qCompressedData.GetQuantity(); i++)
        aucCompressedData[i] = Storage::ReverseBitOrder[aucCompressedData[i]];

    // LZW Minimum Code Size
    unsigned char ucLzwMinimumCodeSize = iLzwMinimumCodeSize;
    rqStream.Write(ucLzwMinimumCodeSize);

    // flow out compressed data by sub-blocks
    unsigned char ucBlockSize;
    int iBytesWritten = 0;
    int iBytesLeft = qCompressedData.GetQuantity();
    for (/**/; /**/; /**/)
    {
        ucBlockSize = ( iBytesLeft >= 255 ? 255 : iBytesLeft );
        rqStream.Write(ucBlockSize);
        rqStream.Write(ucBlockSize,qCompressedData+iBytesWritten);
        iBytesWritten += ucBlockSize;
        iBytesLeft -= ucBlockSize;
        if ( iBytesLeft == 0 )
            break;
    }

    // Block Terminator
    unsigned char ucBlockTerminator = 0;
    rqStream.Write(ucBlockTerminator);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::ArePixelsSimilar (int iX0, int iY0, int iX1, int iY1, const RasterImagePtr& rspqImage0,
    const RasterImagePtr& rspqImage1, float fRequiredPixelSimilarity)
{
    const PaletteColor& rqPE0 = rspqImage0->GetPaletteColor(rspqImage0->GetPixelIndex(iX0,iY0));
    const PaletteColor& rqPE1 = rspqImage1->GetPaletteColor(rspqImage1->GetPixelIndex(iX1,iY1));

    int iDR = rqPE0.R() - rqPE1.R();
    int iDG = rqPE0.G() - rqPE1.G();
    int iDB = rqPE0.B() - rqPE1.B();
    return ( 1.0f - ((float)(iDR*iDR + iDG*iDG + iDB*iDB))/195075 >= fRequiredPixelSimilarity );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatGif::LoadHeadingData (int iBytestreamSize)
{
    // defaults:
    m_bIsAnimated = false;
    m_bTitleImageIsInterlaced = false;
    m_iLoopQuantity = 1;
    m_bDraftImageAllocated = false;
    m_bDynaInitialized = false;
    m_bHasRestorePoint = false;
    m_bHasNRPALSB = false;
    m_bHasAnimationRestorePoint = false;

    assert( iBytestreamSize == -1 || iBytestreamSize >= 0 );

    bool bSuccess;

    // header/signature
    unsigned char aucHeader87a[6] = {71, 73, 70, 56, 55, 97};
    unsigned char aucHeader89a[6] = {71, 73, 70, 56, 57, 97};
    int iDataSize;
    if ( m_eStreaming == Stream::ST_READING_FROM_FILE )
        CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,m_qFilename),ET_CANNOT_ACCESS_FILE)
    else
        iDataSize = iBytestreamSize;
    if ( iDataSize < 6 )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_IDENTIFIER_MISMATCH)
        else
            return false;
    }
    Stream qHeaderStream(m_eStreaming,m_acStreamSrcParam,0,6,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acHeader[6];
    qHeaderStream.Read(6,acHeader);
    qHeaderStream.Finalize();
    bool bVersionIs87a = ( MemCmp(acHeader,aucHeader87a,6) == 0 );
    bool bVersionIs89a = ( MemCmp(acHeader,aucHeader89a,6) == 0 );
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( bVersionIs87a || bVersionIs89a ),
        ET_FORMAT_IDENTIFIER_MISMATCH);

    // Logical Screen Descriptor.
    Stream qLSDStream(m_eStreaming,m_acStreamSrcParam,6,7,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned short usLogicalScreenWidth, usLogicalScreenHeight;
    qLSDStream.Read(usLogicalScreenWidth);
    qLSDStream.Read(usLogicalScreenHeight);
    CONDITIONAL_THROW_EXCEPTION(( usLogicalScreenWidth != 0 && usLogicalScreenHeight != 0 ),
        ET_FORMAT_MISMATCH);
    m_iLogicalScreenWidth = usLogicalScreenWidth;
    m_iLogicalScreenHeight = usLogicalScreenHeight;

    int iGlobalColorTableFlag;
    qLSDStream.ReadUBits(iGlobalColorTableFlag,1);
    m_bHasGlobalColorTable = ( iGlobalColorTableFlag == 1 );

    int iColorResolution;
    qLSDStream.ReadUBits(iColorResolution,3);
    m_iBitsPerPixel = iColorResolution + 1;

    qLSDStream.OffsetBitPosition(1);  // ignore Sort Flag

    int iSizeOfGlobalColorTable;
    qLSDStream.ReadUBits(iSizeOfGlobalColorTable,3);
    int iGlobalColorTableSize;
    if ( m_bHasGlobalColorTable )
        iGlobalColorTableSize = 1 << (iSizeOfGlobalColorTable + 1);

    unsigned char ucBackgroundColorIndex;
    qLSDStream.Read(ucBackgroundColorIndex);
    if ( m_bHasGlobalColorTable )
        m_iBackgroundColorIndex = ucBackgroundColorIndex;

    qLSDStream.Finalize();

    int iNextReadPos = 13;

    if ( m_bHasGlobalColorTable )
    {
        // Global Color Table
        m_qGlobalColorTable.SetQuantity(iGlobalColorTableSize);
        int iSizeInBytes = iGlobalColorTableSize*3;
        Stream qGCTStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,iSizeInBytes,true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        iNextReadPos += iSizeInBytes;
        for (int i = 0; i < iGlobalColorTableSize; i++)
        {
            PaletteColor& rqPaletteColor = m_qGlobalColorTable[i];

            qGCTStream.Read(rqPaletteColor.R());
            qGCTStream.Read(rqPaletteColor.G());
            qGCTStream.Read(rqPaletteColor.B());
        }
        qGCTStream.Finalize();
    }

    // reach Image Descriptor or Graphic Control Extension
    unsigned char ucByte;
    int iBytePosAtEI;
    for (/**/; /**/; /**/)
    {
        Stream qByteStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qByteStream.Read(ucByte);
        qByteStream.Finalize();
        if ( ucByte == 0 )
        {
            // a concession
        }
        else if ( ucByte == 33 )
        {
            // Extension Introducer
            Stream qLabelStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            qLabelStream.Read(ucByte);
            qLabelStream.Finalize();

            iBytePosAtEI = iNextReadPos;

            // can be Netscape Navigator's "N-Loops" Application Extension
            if ( ucByte == 255 )
            {
                Stream qBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,
                    iBytestreamSize);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                qBlockSizeStream.Read(ucByte);
                qBlockSizeStream.Finalize();
                if ( ucByte == 11 )
                {
                    const char* acExSign = "NETSCAPE2.0";
                    Stream qExSignStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,11,true,bSuccess,
                        iBytestreamSize);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    iNextReadPos += 11;
                    char acExSignData[11];
                    qExSignStream.Read(11,acExSignData);
                    qExSignStream.Finalize();
                    if ( MemCmp(acExSignData,acExSign,11) == 0 )
                    {
                        Stream qSubBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,
                            iBytestreamSize);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        qSubBlockSizeStream.Read(ucByte);
                        qSubBlockSizeStream.Finalize();
                        if ( ucByte == 3 )
                        {
                            iNextReadPos++;
                            Stream qSubBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,2,true,
                                bSuccess,iBytestreamSize);
                            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                            iNextReadPos += 2;
                            unsigned short usLoopQuantity;
                            qSubBlockSizeStream.Read(usLoopQuantity);
                            qSubBlockSizeStream.Finalize();

                            m_iLoopQuantity = ( usLoopQuantity == 0 ? 0 : usLoopQuantity + 1 );

                            iNextReadPos++;
                            continue;
                        }
                    }
                }

                iNextReadPos = iBytePosAtEI;
                ucByte = 255;
            }

            if ( ucByte == 1 )
            {
                // Plain Text Extension
                THROW_EXCEPTION(ET_PLAIN_TEXT_EXTENSION_NOT_SUPPORTED);
            }
            else if ( ucByte == 249 )
            {
                // Graphic Control Extension
                m_bIsAnimated = true;
                m_iFirstIDOrGCEPosition = iNextReadPos - 2;
                break;
            }
            ELSE_IF_COMMENT_OR_APPLICATION_EXTENSION(false)
            else
            {
                // unknown extension
                THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }
        }
        else if ( ucByte == 44 )
        {
            // Image Descriptor
            m_iFirstIDOrGCEPosition = iNextReadPos - 1;
            break;
        }
        else if ( ucByte == 59 )
        {
            // Trailer
            THROW_EXCEPTION(ET_FILE_HAS_NO_IMAGES);
        }
        else
        {
            // unknown block label
            THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }
    }
Exit:  // dummy label

    // get know if the title image is interlaced
    iNextReadPos = m_iFirstIDOrGCEPosition;
    if ( m_bIsAnimated )
        iNextReadPos += 8;
    unsigned short usImageLeftPosition, usImageTopPosition, usImageWidth, usImageHeight;
    bool bHasLocalColorTable, bInterlaced, bFitsLSD;
    RasterImage::Palette qLocalColorTable;
    REACH_IMAGE_DESCRIPTOR_AND_READ_IT_WITH_LOCAL_COLOR_TABLE(false);
    if ( bInterlaced )
        m_bTitleImageIsInterlaced = true;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
int FormatGif::LoadImageData (int& riNextReadPos, int iWidth, int iHeight, bool bInterlaced,
    const RasterImage::Palette& rqActiveColorTable, AnimationFrame* pqAnimationFrame, int iBytestreamSize,
    bool bAnimationFrame, RasterImagePtr& rspqImage)
{
    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && !m_bDynaInitialized )
    {
        m_iCurrentPassIndex = 0;
        m_iDevelopedRowQuantity = 0;
    }

    // for a progress bar
    ProgressProbe qProgress(this,( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ? 0.0f :
        ( !bInterlaced ? ((float)m_iDevelopedRowQuantity)/iHeight :
        ((float)m_iCurrentPassIndex + ((float)m_iDevelopedRowQuantity)/iHeight)/4 ) ));

    bool bSuccess;

    Stream qLZWMCSStream(m_eStreaming,m_acStreamSrcParam,riNextReadPos++,1,true,bSuccess,iBytestreamSize);
    LEVEL_LOW_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned char ucLzwMinimumCodeSize;
    qLZWMCSStream.Read(ucLzwMinimumCodeSize);
    qLZWMCSStream.Finalize();
    if ( !(2 <= ucLzwMinimumCodeSize && ucLzwMinimumCodeSize <= 8) )
    {
        if ( !bAnimationFrame )
            assert( false );
        return 1;
    }

    if ( m_bHasNRPALSB )
        riNextReadPos = m_iNextReadPosAfterLastSubBlock;
    unsigned char ucBlockSize;
    unsigned char* aucSubBlockData;
    int iPrevQ;
    for (/**/; /**/; /**/)
    {
        Stream qBlockSizeStream(m_eStreaming,m_acStreamSrcParam,riNextReadPos++,1,true,bSuccess,iBytestreamSize);
        if ( !bSuccess )
            break;
        qBlockSizeStream.Read(ucBlockSize);
        qBlockSizeStream.Finalize();
        if ( ucBlockSize == 0 )
        {
            // Block Terminator
            break;
        }

        Stream qSubBlockStream(m_eStreaming,m_acStreamSrcParam,riNextReadPos,ucBlockSize,true,bSuccess,
            iBytestreamSize);
        riNextReadPos += ucBlockSize;
        if ( !bSuccess )
            break;
        iPrevQ = m_qCompressedImageData.GetQuantity();
        m_qCompressedImageData.SetQuantity(iPrevQ+ucBlockSize);
        MemCpy(m_qCompressedImageData+iPrevQ,qSubBlockStream,ucBlockSize);
        qSubBlockStream.Finalize();

        // reverse bit orders within bytes
        aucSubBlockData = (unsigned char*)(m_qCompressedImageData.GetElements() + iPrevQ);
        for (int i = 0; i < ucBlockSize; i++)
            aucSubBlockData[i] = Storage::ReverseBitOrder[aucSubBlockData[i]];

        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            m_iNextReadPosAfterLastSubBlock = riNextReadPos;
            m_bHasNRPALSB = true;
        }
    }

    // decompress
    if ( !m_pvLzw )
    {
        m_pvLzw = new CompressionLzw(CompressionLzw::ET_GIF);
        ((CompressionLzw*)m_pvLzw)->SetSymbolLength(ucLzwMinimumCodeSize);
        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            ((CompressionLzw*)m_pvLzw)->SetDynamicalDecompressionMode();
    }
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        ((CompressionLzw*)m_pvLzw)->RedirectProgressTo(this,qProgress,0.5f);
    bool bDecompressionSuccess =
        ((CompressionLzw*)m_pvLzw)->Decompress(m_qCompressedImageData,m_qCompressedImageData.GetQuantity(),
         m_qDecompressedImageData);

    if ( m_qDecompressedImageData.IsEmpty() )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            if ( !bAnimationFrame )
                assert( false );
            return 1;
        }
        else
        {
            return -1;
        }
    }

    // Assign pixels.
    int iReqImageSizeInBits = iWidth*iHeight*ucLzwMinimumCodeSize;
    int iReqImageSize = ( (iReqImageSizeInBits & 7) == 0 ? iReqImageSizeInBits/8 : iReqImageSizeInBits/8 + 1 );
    if ( m_qDecompressedImageData.GetQuantity() < iReqImageSize
    &&   (m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM || !bDecompressionSuccess) )
    {
        // a concession, append lacking bytes
        iPrevQ = m_qDecompressedImageData.GetQuantity();
        m_qDecompressedImageData.SetQuantity(iReqImageSize);
        MemSet(m_qDecompressedImageData+iPrevQ,0,iReqImageSize-iPrevQ);
    }

    bool bVisMask = ( pqAnimationFrame && pqAnimationFrame->m_bHasTransparency );
    if ( !(m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_bDynaInitialized) )
    {
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,iWidth,iHeight,rqActiveColorTable);
        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            MemSet(rspqImage->GetImage(),0,iHeight*rspqImage->GetRowStride());
            if ( bInterlaced )
            {
                m_qRowsToFilledState.SetQuantity(iHeight);
                for (int i = 0; i < iHeight; i++)
                    m_qRowsToFilledState[i] = false;
                m_qSavedImageRow.SetQuantity(rspqImage->GetRowStride());
            }
        }
        if ( bVisMask )
        {
            rspqImage->GetVisibilityMask().SetQuantity(iWidth*iHeight);
            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && bInterlaced )
                m_qSavedVMRow.SetQuantity(iWidth);
        }

        m_bDynaInitialized = true;
    }

    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        qProgress.SetAdvance(0.5f/iHeight);

    int iValue, iRow, iX, iY, iBytePos, iBitRatio, iPass, iRowWithinPass, iHeightMO;
    char* acRow;
    char* pcPixel;
    if ( bInterlaced )
        iHeightMO = iHeight - 1;
    if ( !m_bHasRestorePoint )
    {
        iBytePos = 0;
        iBitRatio = 0;
        if ( bInterlaced )
        {
            iPass = 0;
            iRowWithinPass = 0;
        }

        acRow = rspqImage->GetImage();
        pcPixel = acRow;
    }
    else
    {
        // restore the state
        iX = m_iRestoreX;
        iY = m_iRestoreY;
        iBytePos = m_iRestoreBytePos;
        iBitRatio = m_iRestoreBitRatio;
        if ( !bInterlaced )
        {
            acRow = rspqImage->GetRow(iY);
        }
        else
        {
            iPass = m_iRestorePass;
            iRowWithinPass = m_iRestoreRowWithinPass;

            iRow = GetIrlcRow(iPass,iRowWithinPass);

            acRow = rspqImage->GetRow(iRow);
            MemCpy(acRow,m_qSavedImageRow,rspqImage->GetRowStride());
            if ( bVisMask )
                MemCpy(rspqImage->GetVisibilityMask()+iRow*iWidth,m_qSavedVMRow,iWidth);
        }
        pcPixel = acRow + iX;

        goto Restoring;
    }
    for (iY = 0; iY < iHeight; iY++)
    {
        for (iX = 0; iX < iWidth; iX++)
        {
Restoring:
            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && bDecompressionSuccess
            &&   ((CompressionLzw*)m_pvLzw)->HasDecompressionStateRecord()
            &&   iY*iWidth + iX + 1 > ((CompressionLzw*)m_pvLzw)->GetDecodedSymbolQuantity() )
            {
                m_iRestoreX = iX;
                m_iRestoreY = iY;
                m_iRestoreBytePos = iBytePos;
                m_iRestoreBitRatio = iBitRatio;
                if ( bInterlaced )
                {
                    m_iRestorePass = iPass;
                    m_iRestoreRowWithinPass = iRowWithinPass;
                    MemCpy(m_qSavedImageRow,acRow,rspqImage->GetRowStride());
                    if ( bVisMask )
                    {
                        MemCpy(m_qSavedVMRow,rspqImage->GetVisibilityMask()+
                            GetIrlcRow(iPass,iRowWithinPass)*iWidth,iWidth);
                    }
                }
                m_bHasRestorePoint = true;

                if ( !bAnimationFrame && bInterlaced )
                    UpdateInterlacedView(rspqImage);

                return -1;
            }

            iBytePos += Storage::ReadUBits(m_qDecompressedImageData+iBytePos,iBitRatio,ucLzwMinimumCodeSize,iValue);
            if ( iValue >= rqActiveColorTable.GetQuantity() )
            {
                // a concession
                iValue = 0;
            }
            *pcPixel++ = iValue;

            if ( bVisMask )
            {
                if ( !bInterlaced )
                {
                    rspqImage->GetVisibilityMask()[iY*iWidth+iX] =
                        ( iValue == pqAnimationFrame->m_iTransparentColorIndex ? 0 : 1 );
                }
                else
                {
                    rspqImage->GetVisibilityMask()[GetIrlcRow(iPass,iRowWithinPass)*iWidth+iX] =
                        ( iValue == pqAnimationFrame->m_iTransparentColorIndex ? 0 : 1 );
                }
            }
        }

        if ( !bInterlaced )
        {
            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                m_iDevelopedRowQuantity++;

            acRow += rspqImage->GetRowStride();
            pcPixel = acRow;
        }
        else
        {
            if ( iY == iHeightMO )
            {
                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                    m_iDevelopedRowQuantity = iHeight;
                UPDATE_DONE;
                break;
            }

            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                m_qRowsToFilledState[GetIrlcRow(iPass,iRowWithinPass)] = true;

            iRowWithinPass++;
            iRow = GetIrlcRow(iPass,iRowWithinPass);

            if ( iRow < iHeight )
            {
                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                    m_iDevelopedRowQuantity = iRow;

                acRow = rspqImage->GetRow(iRow);
                pcPixel = acRow;
            }
            else
            {
                iRowWithinPass = 0;

                // skip empty passes if any; GIF interlacing scheme should keep iPass in the proper range
                do
                {
                    iPass++;
                } while ( !(ms_aiIrlcStrt[iPass] < iHeight) );

                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                {
                    if ( !bAnimationFrame )
                    {
                        m_iDevelopedRowQuantity = iHeight;
                        UpdateInterlacedView(rspqImage);
                    }

                    m_iCurrentPassIndex = iPass;
                    m_iDevelopedRowQuantity = 0;
                }

                acRow = rspqImage->GetRow(GetIrlcRow(iPass,iRowWithinPass));
                pcPixel = acRow;
            }
        }

        UPDATE_DONE;
    }

    if ( m_pvLzw )
    {
        delete (CompressionLzw*)m_pvLzw;
        m_pvLzw = 0;
    }
    m_qCompressedImageData.RemoveAll();
    m_qDecompressedImageData.RemoveAll();
    m_qRowsToFilledState.RemoveAll();
    m_qSavedImageRow.RemoveAll();
    m_qSavedVMRow.RemoveAll();
    m_bDynaInitialized = false;
    m_bHasRestorePoint = false;
    m_bHasNRPALSB = false;

    qProgress.SetDone(1.0f);

    return 0;
}
//------------------------------------------------------------------------------------------------------------------
void FormatGif::UpdateInterlacedView (RasterImagePtr& rspqImage) const
{
    char* acReferenceRow = rspqImage->GetImage();
    char* acRow = rspqImage->GetImage() + rspqImage->GetRowStride();
    char* acVMReferenceRow;
    char* acVMRow;
    if ( rspqImage->HasVisibilityMask() )
    {
        acVMReferenceRow = rspqImage->GetVisibilityMask();
        acVMRow = acVMReferenceRow + rspqImage->GetWidth();
    }
    int iLineUpdateTo = m_iDevelopedRowQuantity + 8;
    if ( iLineUpdateTo > rspqImage->GetHeight() )
        iLineUpdateTo = rspqImage->GetHeight();
    for (int i = 1; i < iLineUpdateTo; i++)
    {
        if ( !m_qRowsToFilledState[i] )
        {
            MemCpy(acRow,acReferenceRow,rspqImage->GetWidth());
            if ( rspqImage->HasVisibilityMask() )
                MemCpy(acVMRow,acVMReferenceRow,rspqImage->GetWidth());
        }
        else
        {
            acReferenceRow = acRow;
            if ( rspqImage->HasVisibilityMask() )
                acVMReferenceRow = acVMRow;
        }

        acRow += rspqImage->GetRowStride();
        if ( rspqImage->HasVisibilityMask() )
            acVMRow += rspqImage->GetWidth();
    }
}
//------------------------------------------------------------------------------------------------------------------



