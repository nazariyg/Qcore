#include "QFormatIco.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatIco::FormatIco (FileResourceType eFileResource)
    :
    m_eFileResource(eFileResource)
{
}
//------------------------------------------------------------------------------------------------------------------
int FormatIco::InsertImage (const RasterImagePtr& rqImage, int iBitsPerPixel)
{
    assert( rqImage->GetWidth() != 0 && rqImage->GetHeight() != 0 );
    assert( rqImage->GetWidth() <= 255 && rqImage->GetHeight() <= 255 );
    assert( (rqImage->GetColorModel() == RasterImage::CMT_PALETTED
        &&  (iBitsPerPixel == -1 || iBitsPerPixel == 1 || iBitsPerPixel == 4 || iBitsPerPixel == 8))
        ||  (rqImage->GetColorModel() == RasterImage::CMT_RGB && iBitsPerPixel == 24)
        ||  (rqImage->GetColorModel() == RasterImage::CMT_RGBA && iBitsPerPixel == 32) );
    assert( !((rqImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||  rqImage->GetColorModel() == RasterImage::CMT_RGB) && !rqImage->HasVisibilityMask()) );
    assert( !(rqImage->GetColorModel() == RasterImage::CMT_PALETTED && iBitsPerPixel != -1
        &&  1 << iBitsPerPixel < rqImage->GetPaletteSize()) );

    // internal color model
    ColorModelType eColorModel;
    switch ( rqImage->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        eColorModel = CMT_PALETTED;
        break;
    case RasterImage::CMT_RGB:
        eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_RGBA:
        eColorModel = CMT_RGBA;
        break;
    }

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        iBitsPerPixel = Storage::GetUBitQuantity(rqImage->GetPaletteSize()-1);
        if ( 1 < iBitsPerPixel && iBitsPerPixel < 4 )
            iBitsPerPixel = 4;
        else if ( 4 < iBitsPerPixel && iBitsPerPixel < 8 )
            iBitsPerPixel = 8;
    }

    int iPos = m_qImageToSaveRecords.GetQuantity();
    ImageToSaveRecord& rqRec = m_qImageToSaveRecords.GetNewLast();
    rqRec.ImagePtr = rqImage;
    rqRec.ColorModel = eColorModel;
    rqRec.Bpp = iBitsPerPixel;
    rqRec.PaletteSize = ( eColorModel == CMT_PALETTED ? 1 << iBitsPerPixel : 0 );
    return iPos;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatIco::SaveFile (const char* acFilename)
{
    m_iImageQuantity = m_qImageToSaveRecords.GetQuantity();

    // for a progress bar
    int iSumHeight = 0;
    for (int i = 0; i < m_iImageQuantity; i++)
        iSumHeight += m_qImageToSaveRecords[i].ImagePtr->GetHeight();
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iSumHeight);

    Stream qSaveStream(acFilename,true);

    unsigned short usReserved = 0;
    qSaveStream.Write(usReserved);

    unsigned short usType = ( m_eFileResource == FRT_ICON ? 1 : 2 );
    qSaveStream.Write(usType);

    unsigned short usCount = m_iImageQuantity;
    qSaveStream.Write(usCount);

    // entries
    unsigned char ucWidth, ucHeight, ucColorCount, ucReserved = 0;
    unsigned short usPlanes = 1;  // color model regardless (common practice)
    unsigned short usBitsPerPixel;
    char cZero = 0;
    for (int i0 = 0; i0 < m_iImageQuantity; i0++)
    {
        const ImageToSaveRecord& rqRec = m_qImageToSaveRecords[i0];

        // icon image dimensions; overflow will happen if a dimension is > 255, but most readers rely only on
        // near-image information
        ucWidth = rqRec.ImagePtr->GetWidth();
        ucHeight = rqRec.ImagePtr->GetHeight();
        qSaveStream.Write(ucWidth);
        qSaveStream.Write(ucHeight);

        ucColorCount = ( rqRec.ColorModel == CMT_PALETTED && rqRec.Bpp != 8 ? 1 << rqRec.Bpp : 0 );
        qSaveStream.Write(ucColorCount);

        qSaveStream.Write(ucReserved);

        qSaveStream.Write(usPlanes);

        usBitsPerPixel = rqRec.Bpp;
        qSaveStream.Write(usBitsPerPixel);

        // will overwrite SizeInBytes and FileOffset later
        for (int i1 = 0; i1 < 8; i1++)
            qSaveStream.Write(cZero);
    }

    // icon images
    int iWidth, iHeight, iXORBitmapByteSize, iANDBitmapByteSize, iDegree, iPosAtRowStart, iValue;
    unsigned int uiSize = 40;
    unsigned int uiCompression = 0;
    unsigned int uiImageSize = 0;  // common practice
    unsigned int uiSizeInBytes, uiFileOffset, uiWidth, uiDHeight;
    unsigned char* pucRow;
    unsigned char* pucComp;
    char* acRow;
    char* pcCell;
    for (int i0 = 0; i0 < m_iImageQuantity; i0++)
    {
        const ImageToSaveRecord& rqRec = m_qImageToSaveRecords[i0];

        iWidth = rqRec.ImagePtr->GetWidth();
        iHeight = rqRec.ImagePtr->GetHeight();

        // put into ICO header information about image data offset and its size
        iXORBitmapByteSize = ((iWidth*rqRec.Bpp + 31)/32)*4*iHeight;
        iANDBitmapByteSize = ((iWidth + 31)/32)*4*iHeight;
        uiSizeInBytes = 40 + rqRec.PaletteSize*4 + iXORBitmapByteSize + iANDBitmapByteSize;
        uiFileOffset = qSaveStream.GetBytePosition();
        qSaveStream.PushPositionState();
        qSaveStream.SetBytePosition(6+i0*16+8);
        qSaveStream.Write(uiSizeInBytes);
        qSaveStream.Write(uiFileOffset);
        qSaveStream.PopPositionState();

        // InfoHeader.
        qSaveStream.Write(uiSize);

        uiWidth = iWidth;
        uiDHeight = 2*iHeight;
        qSaveStream.Write(uiWidth);
        qSaveStream.Write(uiDHeight);

        qSaveStream.Write(usPlanes);

        usBitsPerPixel = rqRec.Bpp;
        qSaveStream.Write(usBitsPerPixel);

        qSaveStream.Write(uiCompression);

        qSaveStream.Write(uiImageSize);

        // unused
        for (int i1 = 0; i1 < 16; i1++)
            qSaveStream.Write(cZero);

        // actual image data
        if ( rqRec.ColorModel == CMT_PALETTED )
        {
            // write the palette
            for (int i1 = 0; i1 < rqRec.ImagePtr->GetPaletteSize(); i1++)
            {
                const PaletteColor& rqPaletteColor = rqRec.ImagePtr->GetPaletteColor(i1);

                qSaveStream.Write(3,rqPaletteColor.Components);
                qSaveStream.Write(cZero);
            }

            // fill remaining space in the color map section
            iDegree = rqRec.PaletteSize - rqRec.ImagePtr->GetPaletteSize();
            for (int i1 = 0; i1 < iDegree; i1++)
            {
                for (int i2 = 0; i2 < 4; i2++)
                    qSaveStream.Write(cZero);
            }

            // save image pixels (write XOR bitmap)
            pucRow = (unsigned char*)(
                rqRec.ImagePtr->GetImage() + (iHeight - 1)*rqRec.ImagePtr->GetRowStride());
            pucComp = pucRow;
            for (int iY = 0; iY < iHeight; iY++)
            {
                iPosAtRowStart = qSaveStream.GetBytePosition();
                for (int iX = 0; iX < iWidth; iX++)
                {
                    iValue = *pucComp++;
                    qSaveStream.WriteBits(iValue,rqRec.Bpp);
                }

                // alignment
                if ( qSaveStream.GetBitRatio() != 0 )
                    qSaveStream.WriteBits(0,8-qSaveStream.GetBitRatio());
                while ( ((qSaveStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                    qSaveStream.Write(cZero);

                pucRow -= rqRec.ImagePtr->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
        else if ( rqRec.ColorModel == CMT_RGB )
        {
            // save image pixels (write XOR bitmap)
            ColorRgb qColor;
            unsigned char ucR, ucG, ucB;
            for (int iY = 0; iY < iHeight; iY++)
            {
                iPosAtRowStart = qSaveStream.GetBytePosition();
                for (int iX = 0; iX < iWidth; iX++)
                {
                    qColor = rqRec.ImagePtr->GetPixelRgb(iX,iHeight-iY-1);

                    ucR = Mathf::RoundToInt(qColor.R()*255);
                    ucG = Mathf::RoundToInt(qColor.G()*255);
                    ucB = Mathf::RoundToInt(qColor.B()*255);
                    qSaveStream.Write(ucB);
                    qSaveStream.Write(ucG);
                    qSaveStream.Write(ucR);
                }

                // alignment
                while ( ((qSaveStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                    qSaveStream.Write(cZero);

                qProgress.MakeAdvance();
            }
        }
        else  // rqRec.ColorModel = CMT_RGBA
        {
            // save image pixels (write XOR bitmap)
            ColorRgba qColor;
            unsigned char ucR, ucG, ucB, ucA;
            for (int iY = 0; iY < iHeight; iY++)
            {
                iPosAtRowStart = qSaveStream.GetBytePosition();
                for (int iX = 0; iX < iWidth; iX++)
                {
                    qColor = rqRec.ImagePtr->GetPixelRgba(iX,iHeight-iY-1);

                    ucR = Mathf::RoundToInt(qColor.R()*255);
                    ucG = Mathf::RoundToInt(qColor.G()*255);
                    ucB = Mathf::RoundToInt(qColor.B()*255);
                    ucA = Mathf::RoundToInt(qColor.A()*255);
                    qSaveStream.Write(ucB);
                    qSaveStream.Write(ucG);
                    qSaveStream.Write(ucR);
                    qSaveStream.Write(ucA);
                }

                // alignment
                while ( ((qSaveStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                    qSaveStream.Write(cZero);

                qProgress.MakeAdvance();
            }
        }

        // save visibility mask (write AND bitmap)
        RasterImage::VisibilityMask* pqVisibilityMask;
        RasterImage::VisibilityMask qRGBAVM;
        if ( rqRec.ColorModel == CMT_PALETTED
        ||   rqRec.ColorModel == CMT_RGB )
        {
            pqVisibilityMask = &rqRec.ImagePtr->GetVisibilityMask();
        }
        else  // rqRec.ColorModel = CMT_RGBA
        {
            qRGBAVM.SetQuantity(iWidth*iHeight);
            pcCell = qRGBAVM;
            for (int iY = 0; iY < iHeight; iY++)
            {
                for (int iX = 0; iX < iWidth; iX++)
                    *pcCell++ = ( rqRec.ImagePtr->GetPixelRgba(iX,iY).A() != 0.0f ? 1 : 0 );
            }
            pqVisibilityMask = &qRGBAVM;
        }
        acRow = pqVisibilityMask->GetElements() + (iHeight - 1)*iWidth;
        pcCell = acRow;
        for (int iY = 0; iY < iHeight; iY++)
        {
            iPosAtRowStart = qSaveStream.GetBytePosition();
            for (int iX = 0; iX < iWidth; iX++)
            {
                iValue = ( *pcCell++ == 0 ? 1 : 0 );
                qSaveStream.WriteBits(iValue,1);
            }

            // alignment
            if ( qSaveStream.GetBitRatio() != 0 )
                qSaveStream.WriteBits(0,8-qSaveStream.GetBitRatio());
            while ( ((qSaveStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                qSaveStream.Write(cZero);

            acRow -= iWidth;
            pcCell = acRow;
        }
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatIco::FormatIco (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // "signature", resource type, and number of icons
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize > 6 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    unsigned short usReserved, usType, usCount;
    Stream qHeaderStream(m_eStreaming,m_qFilename,0,6,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    qHeaderStream.Read(usReserved);
    qHeaderStream.Read(usType);
    qHeaderStream.Read(usCount);
    qHeaderStream.Finalize();
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( usReserved == 0 && (usType == 1 || usType == 2) ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    m_eFileResource = ( usType == 1 ? FRT_ICON : FRT_CURSOR );
    m_iImageQuantity = usCount;

    // read heading information on each icon image
    Stream qHIStream(m_eStreaming,m_qFilename,6,m_iImageQuantity*16,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    m_qImageRecords.SetQuantity(m_iImageQuantity);
    m_qImageStoreRecords.SetQuantity(m_iImageQuantity);
    unsigned int uiSizeInBytes, uiFileOffset, uiSize, uiWidth, uiDHeight, uiCompression;
    unsigned short usBitsPerPixel;
    int iXORBitmapByteSize, iANDBitmapByteSize;
    for (int i = 0; i < m_iImageQuantity; i++)
    {
        ImageRecord& rqIR = m_qImageRecords[i];
        ImageStoreRecord& rqISR = m_qImageStoreRecords[i];

        // skip Width, Height, ColorCount, Reserved, Planes, and BitCount (required information will be read
        // from near-image header)
        qHIStream.OffsetBytePosition(8);

        // image data location in the file
        qHIStream.Read(uiSizeInBytes);
        qHIStream.Read(uiFileOffset);
        rqISR.ImgByteOffset = uiFileOffset;
        rqISR.ImgByteSize = uiSizeInBytes;

        // Examine the near-image header (InfoHeader).
        Stream qImgHdrStream(m_eStreaming,m_qFilename,rqISR.ImgByteOffset,40,true,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        qImgHdrStream.Read(uiSize);
        C_CONDITIONAL_THROW_EXCEPTION(( uiSize == 40 ),ET_FORMAT_MISMATCH);

        // icon image dimensions
        qImgHdrStream.Read(uiWidth);
        qImgHdrStream.Read(uiDHeight);
        C_CONDITIONAL_THROW_EXCEPTION(( (uiDHeight & 1) == 0 ),ET_FORMAT_MISMATCH);
        rqIR.Width = uiWidth;
        rqIR.Height = uiDHeight/2;
        C_CONDITIONAL_THROW_EXCEPTION(( rqIR.Width != 0 && rqIR.Height != 0 ),ET_FORMAT_MISMATCH);
        rqIR.IsSquare = ( rqIR.Width == rqIR.Height );

        // skip Planes
        qImgHdrStream.OffsetBytePosition(2);

        // bpp
        qImgHdrStream.Read(usBitsPerPixel);
        C_CONDITIONAL_THROW_EXCEPTION(( usBitsPerPixel == 1 || usBitsPerPixel == 4 || usBitsPerPixel == 8
                                   ||   usBitsPerPixel == 24 || usBitsPerPixel == 32 ),ET_FORMAT_MISMATCH);
        switch ( usBitsPerPixel )
        {
        case 1:
        case 4:
        case 8:
            rqIR.ColorModel = CMT_PALETTED;
            break;
        case 24:
            rqIR.ColorModel = CMT_RGB;
            break;
        case 32:
            rqIR.ColorModel = CMT_RGBA;
            break;
        }
        rqIR.PaletteSize = ( rqIR.ColorModel == CMT_PALETTED ? 1 << usBitsPerPixel : 0 );
        rqISR.BitsPerPixel = usBitsPerPixel;

        // with known bpp, calculate expected byte sizes for XOR and AND bitmaps
        iXORBitmapByteSize = ((rqIR.Width*usBitsPerPixel + 31)/32)*4*rqIR.Height;
        iANDBitmapByteSize = ((rqIR.Width + 31)/32)*4*rqIR.Height;

        // perform the check; '>=' is for slight concession
        C_CONDITIONAL_THROW_EXCEPTION(
            ( rqISR.ImgByteSize >= 40 + rqIR.PaletteSize*4 + iXORBitmapByteSize + iANDBitmapByteSize ),
            ET_FORMAT_MISMATCH);

        // compression
        qImgHdrStream.Read(uiCompression);
        C_CONDITIONAL_THROW_EXCEPTION(( uiCompression == 0 ),ET_FORMAT_MISMATCH);

        qImgHdrStream.Finalize();
    }
    qHIStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatIco::LoadImage (RasterImagePtr& rspqImage, int i)
{
    bool bSuccess;

    const ImageRecord& rqIR = m_qImageRecords[i];
    const ImageStoreRecord& rqISR = m_qImageStoreRecords[i];

    Stream qImageStream(m_eStreaming,m_qFilename,rqISR.ImgByteOffset+40,rqISR.ImgByteSize-40,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/rqIR.Height);

    // load actual image
    unsigned char* pucRow;
    unsigned char* pucComp;
    int iPosAtRowStart, iValue;
    if ( rqIR.ColorModel == CMT_PALETTED )
    {
        // read palette
        RasterImage::Palette qPalette(rqIR.PaletteSize);
        for (int i = 0; i < rqIR.PaletteSize; i++)
        {
            PaletteColor& rqPaletteColor = qPalette[i];

            qImageStream.Read(3,rqPaletteColor.Components);
            qImageStream.OffsetBytePosition(1);
        }

        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,rqIR.Width,rqIR.Height,qPalette);

        // load pixel data (read XOR bitmap)
        pucRow = (unsigned char*)(rspqImage->GetImage() + (rqIR.Height - 1)*rspqImage->GetRowStride());
        pucComp = pucRow;
        for (int iY = 0; iY < rqIR.Height; iY++)
        {
            iPosAtRowStart = qImageStream.GetBytePosition();
            for (int iX = 0; iX < rqIR.Width; iX++)
            {
                qImageStream.ReadUBits(iValue,rqISR.BitsPerPixel);
                *pucComp++ = iValue;
            }

            // alignment
            qImageStream.AlignBytes();
            while ( ((qImageStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                qImageStream.OffsetBytePosition(1);

            pucRow -= rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }
    else if ( rqIR.ColorModel == CMT_RGB )
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,rqIR.Width,rqIR.Height);

        // load pixel data (read XOR bitmap)
        pucRow = (unsigned char*)(rspqImage->GetImage() + (rqIR.Height - 1)*rspqImage->GetRowStride());
        pucComp = pucRow;
        for (int iY = 0; iY < rqIR.Height; iY++)
        {
            iPosAtRowStart = qImageStream.GetBytePosition();
            for (int iX = 0; iX < rqIR.Width; iX++)
            {
                qImageStream.Read(pucComp[2]);
                qImageStream.Read(pucComp[1]);
                qImageStream.Read(pucComp[0]);
                pucComp += 3;
            }

            // alignment
            while ( ((qImageStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                qImageStream.OffsetBytePosition(1);

            pucRow -= rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }
    else  // rqIR.ColorModel = CMT_RGBA
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,rqIR.Width,rqIR.Height);

        // load pixel data (read XOR bitmap)
        pucRow = (unsigned char*)(rspqImage->GetImage() + (rqIR.Height - 1)*rspqImage->GetRowStride());
        pucComp = pucRow;
        for (int iY = 0; iY < rqIR.Height; iY++)
        {
            iPosAtRowStart = qImageStream.GetBytePosition();
            for (int iX = 0; iX < rqIR.Width; iX++)
            {
                qImageStream.Read(pucComp[2]);
                qImageStream.Read(pucComp[1]);
                qImageStream.Read(pucComp[0]);
                qImageStream.Read(pucComp[3]);
                pucComp += 4;
            }

            // alignment
            while ( ((qImageStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                qImageStream.OffsetBytePosition(1);

            pucRow -= rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }

    if ( rqIR.ColorModel == CMT_PALETTED
    ||   rqIR.ColorModel == CMT_RGB )
    {
        // load the visibility mask (read AND bitmap)
        RasterImage::VisibilityMask qVisibilityMask;
        qVisibilityMask.SetQuantity(rqIR.Width*rqIR.Height);
        char* acRow = qVisibilityMask + (rqIR.Height - 1)*rqIR.Width;
        char* pcCell = acRow;
        for (int iY = 0; iY < rqIR.Height; iY++)
        {
            iPosAtRowStart = qImageStream.GetBytePosition();
            for (int iX = 0; iX < rqIR.Width; iX++)
            {
                qImageStream.ReadUBits(iValue,1);
                *pcCell++ = ( iValue == 0 ? 1 : 0 );
            }

            // alignment
            qImageStream.AlignBytes();
            while ( ((qImageStream.GetBytePosition() - iPosAtRowStart) & 3) != 0 )
                qImageStream.OffsetBytePosition(1);

            acRow -= rqIR.Width;
            pcCell = acRow;
        }
        rspqImage->SetVisibilityMask(qVisibilityMask);
    }

    qImageStream.Finalize();

    return true;
}
//------------------------------------------------------------------------------------------------------------------



