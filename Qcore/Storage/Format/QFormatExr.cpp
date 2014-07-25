#include "QFormatExr.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QStorage.h"
#include "QProgressProbe.h"
using namespace Q;

// OpenEXR includes.  Some compiler warnings, caused by OpenEXR, are disabled:
//   "C++ exception specification ignored except to indicate a function is not __declspec(nothrow)";
//   "'identifier' used on polymorphic type 'type' with /GR-; unpredictable behavior may result".
#include "ImfRgbaFile.h"
#include "ImfTiledInputFile.h"
#include "ImfTiledRgbaFile.h"
#include "ImfChannelList.h"
#include "ImfPreviewImage.h"
#include "ImfArray.h"
#if defined(_WIN32)
#pragma warning( push )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4541 )
#endif
#include "ImfStandardAttributes.h"
#if defined(_WIN32)
#pragma warning( pop )
#endif

//------------------------------------------------------------------------------------------------------------------
FormatExr::FormatExr (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iImageWidth(rspqImage->GetWidth()),
    m_iImageHeight(rspqImage->GetHeight()),
    m_qImageDisplayWindow(0,rspqImage->GetWidth()-1,0,rspqImage->GetHeight()-1),
    m_qImageDataWindow(0,rspqImage->GetWidth()-1,0,rspqImage->GetHeight()-1),
    m_fPixelAspectRatio(1.0f),
    m_fScreenWindowCenterX(0.0f),
    m_fScreenWindowCenterY(0.0f),
    m_fScreenWindowWidth(1.0f),
    m_bColorModelPrecised(false),
    m_eLineOrder(LOT_INCREASING_Y),
    m_eCompression(CT_PIZ),
    m_bIsTiled(false),
    m_bHasPreviewImage(false),
    m_bHasChromaticities(false),
    m_bHasWhiteLuminance(false),
    m_bHasHorizontalDensity(false),
    m_bHasOwner(false),
    m_bHasComments(false),
    m_bHasCaptureDate(false),
    m_bHasUtcOffset(false),
    m_bHasLongitude(false),
    m_bHasLatitude(false),
    m_bHasAltitude(false),
    m_bHasFocus(false),
    m_bHasExposureTime(false),
    m_bHasAperture(false),
    m_bHasIsoSpeed(false),
    m_bIsEnvironmentMap(false),
    m_bHasKeyCode(false),
    m_bHasTimeCode(false),
    m_bHasWrapmodes(false),
    m_bMultiplyAlpha(true),
    m_bOffScreenMode(false)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGB
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGBA );
    assert( !rspqImage->HasVisibilityMask() );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatExr::SaveImage (const char* acFilename)
{
    assert( !(m_bColorModelPrecised && m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  m_eColorModel != CMT_Y) );
    assert( !(m_bColorModelPrecised
        &&  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        &&  m_eColorModel != CMT_YA) );
    assert( !(m_bColorModelPrecised && m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        &&  !(m_eColorModel == CMT_RGB || m_eColorModel == CMT_YC)) );
    assert( !(m_bColorModelPrecised && m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGBA
        &&  !(m_eColorModel == CMT_RGBA || m_eColorModel == CMT_YCA)) );

    if ( !m_bColorModelPrecised )
    {
        // do autodetect
        switch ( m_spqImageSaveTo->GetColorModel() )
        {
        case RasterImage::CMT_GRAYSCALE:
            m_eColorModel = CMT_Y;
            break;
        case RasterImage::CMT_GRAYSCALE_WITH_ALPHA:
            m_eColorModel = CMT_YA;
            break;
        case RasterImage::CMT_RGB:
            m_eColorModel = CMT_RGB;
            break;
        case RasterImage::CMT_RGBA:
            m_eColorModel = CMT_RGBA;
            break;
        }
    }

    try
    {
        // construct EXR header
        Imath::Box2i qDispWnd;
        Imath::Box2i qDataWnd;
        Imath::V2f qScreenWindowCenter;
        Imf::LineOrder eLineOrder;
        Imf::Compression eCompression;
        qDispWnd.min.x = m_qImageDisplayWindow.MinX;
        qDispWnd.max.x = m_qImageDisplayWindow.MaxX;
        qDispWnd.min.y = m_qImageDisplayWindow.MinY;
        qDispWnd.max.y = m_qImageDisplayWindow.MaxY;
        qDataWnd.min.x = m_qImageDataWindow.MinX;
        qDataWnd.max.x = m_qImageDataWindow.MaxX;
        qDataWnd.min.y = m_qImageDataWindow.MinY;
        qDataWnd.max.y = m_qImageDataWindow.MaxY;
        qScreenWindowCenter.x = m_fScreenWindowCenterX;
        qScreenWindowCenter.y = m_fScreenWindowCenterY;
        switch ( m_eLineOrder )
        {
        case LOT_INCREASING_Y:
            eLineOrder = Imf::INCREASING_Y;
            break;
        case LOT_DECREASING_Y:
            eLineOrder = Imf::DECREASING_Y;
            break;
        case LOT_RANDOM_Y:
            eLineOrder = Imf::RANDOM_Y;
            break;
        }
        switch ( m_eCompression )
        {
        case CT_NONE:
            eCompression = Imf::NO_COMPRESSION;
            break;
        case CT_RLE:
            eCompression = Imf::RLE_COMPRESSION;
            break;
        case CT_ZIPS:
            eCompression = Imf::ZIPS_COMPRESSION;
            break;
        case CT_ZIP:
            eCompression = Imf::ZIP_COMPRESSION;
            break;
        case CT_PIZ:
            eCompression = Imf::PIZ_COMPRESSION;
            break;
        case CT_PXR24:
            eCompression = Imf::PXR24_COMPRESSION;
            break;
        case CT_B44:
            eCompression = Imf::B44_COMPRESSION;
            break;
        case CT_B44A:
            eCompression = Imf::B44A_COMPRESSION;
            break;
        }
        Imf::Header qHeader(qDispWnd,qDataWnd,m_fPixelAspectRatio,qScreenWindowCenter,m_fScreenWindowWidth,
            eLineOrder,eCompression);

        if ( m_bIsTiled )
        {
            // tile description
            Imf::TileDescription qTileDesc;
            qTileDesc.xSize = m_iTileWidth;
            qTileDesc.ySize = m_iTileHeight;
            switch ( m_eLevelMode )
            {
            case LMT_ONE_LEVEL:
                qTileDesc.mode = Imf::ONE_LEVEL;
                break;
            case LMT_MIPMAP_LEVELS:
                qTileDesc.mode = Imf::MIPMAP_LEVELS;
                break;
            case LMT_RIPMAP_LEVELS:
                qTileDesc.mode = Imf::RIPMAP_LEVELS;
                break;
            }
            switch ( m_eLevelRoundingMode )
            {
            case LRMT_ROUND_DOWN:
                qTileDesc.roundingMode = Imf::ROUND_DOWN;
                break;
            case LRMT_ROUND_UP:
                qTileDesc.roundingMode = Imf::ROUND_UP;
                break;
            }

            qHeader.setTileDescription(qTileDesc);
        }

        if ( m_bHasPreviewImage )
        {
            // preview image
            Imf::PreviewImage qPImg(m_iPreviewImageWidth,m_iPreviewImageHeight);
            for (int i = 0; i < m_qPreviewImagePixels.GetQuantity(); i++)
            {
                const UcRgba& rqPPix0 = m_qPreviewImagePixels[i];
                Imf::PreviewRgba& rqPPix1 = qPImg.pixels()[i];

                rqPPix1.r = rqPPix0.R;
                rqPPix1.g = rqPPix0.G;
                rqPPix1.b = rqPPix0.B;
                rqPPix1.a = rqPPix0.A;
            }

            qHeader.setPreviewImage(qPImg);
        }

        // Standard attributes.

        if ( m_bHasChromaticities )
        {
            // chromaticities of the primaries and white point
            Imf::Chromaticities qChroma;
            qChroma.red.x = m_fChromaticityRedX;
            qChroma.red.y = m_fChromaticityRedY;
            qChroma.green.x = m_fChromaticityGreenX;
            qChroma.green.y = m_fChromaticityGreenY;
            qChroma.blue.x = m_fChromaticityBlueX;
            qChroma.blue.y = m_fChromaticityBlueY;
            qChroma.white.x = m_fWhitePointX;
            qChroma.white.y = m_fWhitePointY;

            Imf::addChromaticities(qHeader,qChroma);
        }

        if ( m_bHasWhiteLuminance )
        {
            // luminance of white
            Imf::addWhiteLuminance(qHeader,m_fWhiteLuminance);
        }

        if ( m_bHasHorizontalDensity )
        {
            // horizontal output density
            Imf::addXDensity(qHeader,m_fHorizontalDensity);
        }

        if ( m_bHasOwner )
        {
            // name of the owner
            Imf::addOwner(qHeader,std::string(m_qOwner));
        }

        if ( m_bHasComments )
        {
            // image information
            Imf::addComments(qHeader,std::string(m_qComments));
        }

        if ( m_bHasCaptureDate )
        {
            // the date when the image was captured or created
            String qCDS;
            m_qCaptureDate.ToString(qCDS);
            Imf::addCapDate(qHeader,std::string(qCDS));
        }

        if ( m_bHasUtcOffset )
        {
            // offset of local time from UTC, in seconds
            Imf::addUtcOffset(qHeader,m_fUtcOffset);
        }

        // the location where the image was recorded; longitude and latitude in degrees, altitude in meters
        if ( m_bHasLongitude )
            Imf::addLongitude(qHeader,m_fLongitude);
        if ( m_bHasLatitude )
            Imf::addLatitude(qHeader,m_fLatitude);
        if ( m_bHasAltitude )
            Imf::addAltitude(qHeader,m_fAltitude);

        if ( m_bHasFocus )
        {
            // the camera's focus distance, in meters
            Imf::addFocus(qHeader,m_fFocus);
        }

        if ( m_bHasExposureTime )
        {
            // exposure time
            Imf::addExpTime(qHeader,m_fExposureTime);
        }

        if ( m_bHasAperture )
        {
            // the camera's lens aperture
            Imf::addAperture(qHeader,m_fAperture);
        }

        if ( m_bHasIsoSpeed )
        {
            // ISO speed of the film or image sensor that was used to record the image
            Imf::addIsoSpeed(qHeader,m_fIsoSpeed);
        }

        if ( m_bIsEnvironmentMap )
        {
            // type of environment map
            Imf::Envmap eEM;
            switch ( m_eEnvironmentMapType )
            {
            case EMT_LATITUDE_LONGITUDE:
                eEM = Imf::ENVMAP_LATLONG;
                break;
            case EMT_CUBE:
                eEM = Imf::ENVMAP_CUBE;
                break;
            }

            Imf::addEnvmap(qHeader,eEM);
        }

        if ( m_bHasKeyCode )
        {
            // key-code for motion picture film frames
            assert( 0 <= m_qKeyCode.FilmManufacturerCode && m_qKeyCode.FilmManufacturerCode <= 99 );
            assert( 0 <= m_qKeyCode.FilmTypeCode && m_qKeyCode.FilmTypeCode <= 99 );
            assert( 0 <= m_qKeyCode.FilmRollPrefix && m_qKeyCode.FilmRollPrefix <= 999999 );
            assert( 0 <= m_qKeyCode.Count && m_qKeyCode.Count <= 9999 );
            assert( 0 <= m_qKeyCode.FrameOffset && m_qKeyCode.FrameOffset <= 119 );
            assert( 1 <= m_qKeyCode.PerforationsPerFrame && m_qKeyCode.PerforationsPerFrame <= 15 );
            assert( 20 <= m_qKeyCode.PerforationsPerCount && m_qKeyCode.PerforationsPerCount <= 120 );

            Imf::addKeyCode(qHeader,Imf::KeyCode(
                m_qKeyCode.FilmManufacturerCode,
                m_qKeyCode.FilmTypeCode,
                m_qKeyCode.FilmRollPrefix,
                m_qKeyCode.Count,
                m_qKeyCode.FrameOffset,
                m_qKeyCode.PerforationsPerFrame,
                m_qKeyCode.PerforationsPerCount));
        }

        if ( m_bHasTimeCode )
        {
            // time-code
            assert( 0 <= m_qTimeCode.Hours && m_qTimeCode.Hours <= 23 );
            assert( 0 <= m_qTimeCode.Minutes && m_qTimeCode.Minutes <= 59 );
            assert( 0 <= m_qTimeCode.Seconds && m_qTimeCode.Seconds <= 59 );
            assert( 0 <= m_qTimeCode.Frame && m_qTimeCode.Frame <= 29 );
#ifdef _DEBUG
            for (int i = 0; i < 8; i++)
                assert( 0 <= m_qTimeCode.BinaryGroup[i] && m_qTimeCode.BinaryGroup[i] <= 15 );
#endif

            Imf::addTimeCode(qHeader,Imf::TimeCode(
                m_qTimeCode.Hours,
                m_qTimeCode.Minutes,
                m_qTimeCode.Seconds,
                m_qTimeCode.Frame,
                m_qTimeCode.DropFrame,
                m_qTimeCode.ColorFrame,
                m_qTimeCode.FieldPhase,
                m_qTimeCode.Bgf0,
                m_qTimeCode.Bgf1,
                m_qTimeCode.Bgf2,
                m_qTimeCode.BinaryGroup[0],
                m_qTimeCode.BinaryGroup[1],
                m_qTimeCode.BinaryGroup[2],
                m_qTimeCode.BinaryGroup[3],
                m_qTimeCode.BinaryGroup[4],
                m_qTimeCode.BinaryGroup[5],
                m_qTimeCode.BinaryGroup[6],
                m_qTimeCode.BinaryGroup[7]));
        }

        if ( m_bHasWrapmodes )
        {
            // how texture map images are extrapolated
            Imf::addWrapmodes(qHeader,std::string(m_qWrapmodes));
        }

        // examine the header
        qHeader.sanityCheck();

        // construct RgbaOutputFile object
        Imf::RgbaChannels eRgbaChannels;
        switch ( m_eColorModel )
        {
        case CMT_RGB:
            eRgbaChannels = Imf::WRITE_RGB;
            break;
        case CMT_RGBA:
            eRgbaChannels = Imf::WRITE_RGBA;
            break;
        case CMT_YC:
            eRgbaChannels = Imf::WRITE_YC;
            break;
        case CMT_YCA:
            eRgbaChannels = Imf::WRITE_YCA;
            break;
        case CMT_Y:
            eRgbaChannels = Imf::WRITE_Y;
            break;
        case CMT_YA:
            eRgbaChannels = Imf::WRITE_YA;
            break;
        }
        Imf::RgbaOutputFile qOut(acFilename,qHeader,eRgbaChannels);

        // Write title image.

        assert( !((m_eColorModel == CMT_YC || m_eColorModel == CMT_YCA)
            &&  ((m_iImageWidth & 1) != 0 || (m_iImageHeight & 1) != 0)) );

        ProgressProbe qProgress(this);

        // number of scanlines in a transmission block (OpenEXR library currently doesn't support chunked
        // output)
        int iScanlinesPerChunk = m_iImageHeight;

        Imf::Array2D<Imf::Rgba> qChunk(iScanlinesPerChunk,m_iImageWidth);
        int iScanlinesToWrite;
        qOut.setFrameBuffer(&qChunk[0][0],1,m_iImageWidth);
        float fIntensity;
        Link<float,float> qIntensityWithAlpha;
        ColorRgb qColorRgb;
        ColorRgba qColorRgba;
        for (int iY = 0; iY != m_iImageHeight; /**/)
        {
            iScanlinesToWrite = Mathi::Min(m_iImageHeight-iY,iScanlinesPerChunk);

            if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE )
            {
                for (int i = 0; i < iScanlinesToWrite; i++, iY++)
                {
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        fIntensity = m_spqImageSaveTo->GetPixelIntensity(iX,iY);
                        qColorRgba.R() = fIntensity;
                        qColorRgba.G() = fIntensity;
                        qColorRgba.B() = fIntensity;
                        qColorRgba.A() = 1.0f;
                        Imf::Rgba& rqRgba = qChunk[i][iX];

                        OnWritePixel(qColorRgba,false);

                        rqRgba.r = qColorRgba.R();
                        rqRgba.g = qColorRgba.G();
                        rqRgba.b = qColorRgba.B();
                        rqRgba.a = qColorRgba.A();
                    }
                }
            }
            else if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                for (int i = 0; i < iScanlinesToWrite; i++, iY++)
                {
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        qIntensityWithAlpha = m_spqImageSaveTo->GetPixelIntensityWithAlpha(iX,iY);
                        qColorRgba.R() = qIntensityWithAlpha.First;
                        qColorRgba.G() = qIntensityWithAlpha.First;
                        qColorRgba.B() = qIntensityWithAlpha.First;
                        qColorRgba.A() = qIntensityWithAlpha.Second;
                        Imf::Rgba& rqRgba = qChunk[i][iX];

                        OnWritePixel(qColorRgba,true);

                        rqRgba.r = qColorRgba.R();
                        rqRgba.g = qColorRgba.G();
                        rqRgba.b = qColorRgba.B();
                        rqRgba.a = qColorRgba.A();
                    }
                }
            }
            else if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB )
            {
                for (int i = 0; i < iScanlinesToWrite; i++, iY++)
                {
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        qColorRgba.R() = qColorRgb.R();
                        qColorRgba.G() = qColorRgb.G();
                        qColorRgba.B() = qColorRgb.B();
                        qColorRgba.A() = 1.0f;
                        Imf::Rgba& rqRgba = qChunk[i][iX];

                        OnWritePixel(qColorRgba,false);

                        rqRgba.r = qColorRgba.R();
                        rqRgba.g = qColorRgba.G();
                        rqRgba.b = qColorRgba.B();
                        rqRgba.a = qColorRgba.A();
                    }
                }
            }
            else  // m_spqImageSaveTo->GetColorModel() = RasterImage::CMT_RGBA
            {
                for (int i = 0; i < iScanlinesToWrite; i++, iY++)
                {
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        qColorRgba = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                        Imf::Rgba& rqRgba = qChunk[i][iX];

                        OnWritePixel(qColorRgba,true);

                        rqRgba.r = qColorRgba.R();
                        rqRgba.g = qColorRgba.G();
                        rqRgba.b = qColorRgba.B();
                        rqRgba.a = qColorRgba.A();
                    }
                }
            }

            // output EXR pixels
            qOut.writePixels(iScanlinesToWrite);
        }

        qProgress.SetDone(1.0f);
    }
    catch (...)
    {
        THROW_EXCEPTION(ET_OPEN_EXR_ERROR);
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatExr::FormatExr (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_bUnmultiplyAlpha(true),
    m_bOnScreenMode(false)
{
    // examine the signature
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 4 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    bool bSuccess;
    Stream qSignatureStream(m_eStreaming,m_qFilename,0,4,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acB[4];
    qSignatureStream.Read(4,acB);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(
        ( acB[0] == 0x76 && acB[1] == 0x2f && acB[2] == 0x31 && acB[3] == 0x01 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureStream.Finalize();

    // try to construct RgbaInputFile object and do OpenEXR library heading data related calls
    try
    {
        Imf::RgbaInputFile qInFile(acFilename);

        // check file's completeness
        C_CONDITIONAL_THROW_EXCEPTION(qInFile.isComplete(),ET_FILE_IS_INCOMPLETE);

        // check header's sanity
        try
        {
            qInFile.header().sanityCheck(qInFile.header().hasTileDescription());
        }
        catch (...)
        {
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        // version
        m_iVersion = qInFile.version();

        const Imf::Header& rqHeader = qInFile.header();

        // dimensions
        const Imath::Box2i& rqDispWnd = rqHeader.displayWindow();
        const Imath::Box2i& rqDataWnd = rqHeader.dataWindow();
        m_iImageWidth = rqDataWnd.max.x - rqDataWnd.min.x + 1;
        m_iImageHeight = rqDataWnd.max.y - rqDataWnd.min.y + 1;
        m_qImageDisplayWindow.MinX = rqDispWnd.min.x;
        m_qImageDisplayWindow.MaxX = rqDispWnd.max.x;
        m_qImageDisplayWindow.MinY = rqDispWnd.min.y;
        m_qImageDisplayWindow.MaxY = rqDispWnd.max.y;
        m_qImageDataWindow.MinX = rqDataWnd.min.x;
        m_qImageDataWindow.MaxX = rqDataWnd.max.x;
        m_qImageDataWindow.MinY = rqDataWnd.min.y;
        m_qImageDataWindow.MaxY = rqDataWnd.max.y;

        // aspect ratio
        m_fPixelAspectRatio = rqHeader.pixelAspectRatio();

        // screen window
        m_fScreenWindowCenterX = rqHeader.screenWindowCenter().x;
        m_fScreenWindowCenterY = rqHeader.screenWindowCenter().y;
        m_fScreenWindowWidth = rqHeader.screenWindowWidth();

        // channels
        ChannelRecord qChRec;
        const Imf::ChannelList& rqChannels = qInFile.header().channels();
        for (Imf::ChannelList::ConstIterator qIt = rqChannels.begin(); qIt != rqChannels.end(); ++qIt)
        {
            const Imf::Name& rqName = qIt.name();
            const Imf::Channel& rqChannel = qIt.channel();

            qChRec.Name = rqName.text();
            switch ( rqChannel.type )
            {
            case Imf::UINT:
                qChRec.DataType = CDT_UINT;
                break;
            case Imf::HALF:
                qChRec.DataType = CDT_HALF;
                break;
            case Imf::FLOAT:
                qChRec.DataType = CDT_FLOAT;
                break;
            default:
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }
            qChRec.SamplingX = rqChannel.xSampling;
            qChRec.SamplingY = rqChannel.ySampling;

            m_qChannelRecords.Push(qChRec);
        }
        m_iChannelQuantity = m_qChannelRecords.GetQuantity();

        // identify the color model
        switch ( qInFile.channels() )
        {
        case Imf::WRITE_RGB:
            m_eColorModel = CMT_RGB;
            break;
        case Imf::WRITE_RGBA:
            m_eColorModel = CMT_RGBA;
            break;
        case Imf::WRITE_YC:
            m_eColorModel = CMT_YC;
            break;
        case Imf::WRITE_YCA:
            m_eColorModel = CMT_YCA;
            break;
        case Imf::WRITE_Y:
            m_eColorModel = CMT_Y;
            break;
        case Imf::WRITE_YA:
            m_eColorModel = CMT_YA;
            break;
        default:
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        // line order
        switch ( rqHeader.lineOrder() )
        {
        case Imf::INCREASING_Y:
            m_eLineOrder = LOT_INCREASING_Y;
            break;
        case Imf::DECREASING_Y:
            m_eLineOrder = LOT_DECREASING_Y;
            break;
        case Imf::RANDOM_Y:
            m_eLineOrder = LOT_RANDOM_Y;
            break;
        default:
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        // compression
        switch ( rqHeader.compression() )
        {
        case Imf::NO_COMPRESSION:
            m_eCompression = CT_NONE;
            break;
        case Imf::RLE_COMPRESSION:
            m_eCompression = CT_RLE;
            break;
        case Imf::ZIPS_COMPRESSION:
            m_eCompression = CT_ZIPS;
            break;
        case Imf::ZIP_COMPRESSION:
            m_eCompression = CT_ZIP;
            break;
        case Imf::PIZ_COMPRESSION:
            m_eCompression = CT_PIZ;
            break;
        case Imf::PXR24_COMPRESSION:
            m_eCompression = CT_PXR24;
            break;
        case Imf::B44_COMPRESSION:
            m_eCompression = CT_B44;
            break;
        case Imf::B44A_COMPRESSION:
            m_eCompression = CT_B44A;
            break;
        default:
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        // tile description
        m_bIsTiled = false;
        if ( rqHeader.hasTileDescription() )
        {
            const Imf::TileDescription& rqTileDesc = rqHeader.tileDescription();

            m_bIsTiled = true;
            m_iTileWidth = rqTileDesc.xSize;
            m_iTileHeight = rqTileDesc.ySize;
            switch ( rqTileDesc.mode )
            {
            case Imf::ONE_LEVEL:
                m_eLevelMode = LMT_ONE_LEVEL;
                break;
            case Imf::MIPMAP_LEVELS:
                m_eLevelMode = LMT_MIPMAP_LEVELS;
                break;
            case Imf::RIPMAP_LEVELS:
                m_eLevelMode = LMT_RIPMAP_LEVELS;
                break;
            default:
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }
            switch ( rqTileDesc.roundingMode )
            {
            case Imf::ROUND_DOWN:
                m_eLevelRoundingMode = LRMT_ROUND_DOWN;
                break;
            case Imf::ROUND_UP:
                m_eLevelRoundingMode = LRMT_ROUND_UP;
                break;
            default:
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }
        }

        // preview image
        m_bHasPreviewImage = false;
        if ( rqHeader.hasPreviewImage() )
        {
            const Imf::PreviewImage& rqPImg = rqHeader.previewImage();

            m_bHasPreviewImage = true;
            m_iPreviewImageWidth = rqPImg.width();
            m_iPreviewImageHeight = rqPImg.height();
            m_qPreviewImagePixels.SetQuantity(m_iPreviewImageWidth*m_iPreviewImageHeight);
            for (int i = 0; i < m_qPreviewImagePixels.GetQuantity(); i++)
            {
                const Imf::PreviewRgba& rqPPix0 = rqPImg.pixels()[i];
                UcRgba& rqPPix1 = m_qPreviewImagePixels[i];

                rqPPix1.R = rqPPix0.r;
                rqPPix1.G = rqPPix0.g;
                rqPPix1.B = rqPPix0.b;
                rqPPix1.A = rqPPix0.a;
            }
        }

        // primary chromaticities and reference-white
        m_bHasChromaticities = false;
        if ( Imf::hasChromaticities(rqHeader) )
        {
            const Imf::Chromaticities& rqChroma = Imf::chromaticities(rqHeader);

            m_bHasChromaticities = true;
            m_fChromaticityRedX = rqChroma.red.x;
            m_fChromaticityRedY = rqChroma.red.y;
            m_fChromaticityGreenX = rqChroma.green.x;
            m_fChromaticityGreenY = rqChroma.green.y;
            m_fChromaticityBlueX = rqChroma.blue.x;
            m_fChromaticityBlueY = rqChroma.blue.y;
            m_fWhitePointX = rqChroma.white.x;
            m_fWhitePointY = rqChroma.white.y;
        }

        // luminance of white
        m_bHasWhiteLuminance = false;
        if ( Imf::hasWhiteLuminance(rqHeader) )
        {
            m_bHasWhiteLuminance = true;
            m_fWhiteLuminance = Imf::whiteLuminance(rqHeader);
        }

        // horizontal output density
        m_bHasHorizontalDensity = false;
        if ( Imf::hasXDensity(rqHeader) )
        {
            m_bHasHorizontalDensity = true;
            m_fHorizontalDensity = Imf::xDensity(rqHeader);
        }

        // name of the owner
        m_bHasOwner = false;
        if ( Imf::hasOwner(rqHeader) )
        {
            const std::string& rqOwr = Imf::owner(rqHeader);

            m_bHasOwner = true;
            m_qOwner.SetLength((int)rqOwr.size());
            for (int i = 0; i < m_qOwner.GetLength(); i++)
                m_qOwner[i] = rqOwr[i];
        }

        // image information
        m_bHasComments = false;
        if ( Imf::hasComments(rqHeader) )
        {
            const std::string& rqComm = Imf::comments(rqHeader);

            m_bHasComments = true;
            m_qComments.SetLength((int)rqComm.size());
            for (int i = 0; i < m_qComments.GetLength(); i++)
                m_qComments[i] = rqComm[i];
        }

        // the date when the image was captured or created
        m_bHasCaptureDate = false;
        if ( Imf::hasCapDate(rqHeader) )
        {
            const std::string& rqCDate = Imf::capDate(rqHeader);

            m_bHasCaptureDate = true;
            m_qCaptureDate = Date(rqCDate.data());
        }

        // offset of local time from UTC
        m_bHasUtcOffset = false;
        if ( Imf::hasUtcOffset(rqHeader) )
        {
            m_bHasUtcOffset = true;
            m_fUtcOffset = Imf::utcOffset(rqHeader);
        }

        // the location where the image was recorded
        m_bHasLongitude = false;
        if ( Imf::hasLongitude(rqHeader) )
        {
            m_bHasLongitude = true;
            m_fLongitude = Imf::longitude(rqHeader);
        }
        m_bHasLatitude = false;
        if ( Imf::hasLatitude(rqHeader) )
        {
            m_bHasLatitude = true;
            m_fLatitude = Imf::latitude(rqHeader);
        }
        m_bHasAltitude = false;
        if ( Imf::hasAltitude(rqHeader) )
        {
            m_bHasAltitude = true;
            m_fAltitude = Imf::altitude(rqHeader);
        }

        // the camera's focus distance
        m_bHasFocus = false;
        if ( Imf::hasFocus(rqHeader) )
        {
            m_bHasFocus = true;
            m_fFocus = Imf::focus(rqHeader);
        }

        // exposure time
        m_bHasExposureTime = false;
        if ( Imf::hasExpTime(rqHeader) )
        {
            m_bHasExposureTime = true;
            m_fExposureTime = Imf::expTime(rqHeader);
        }

        // the camera's lens aperture
        m_bHasAperture = false;
        if ( Imf::hasAperture(rqHeader) )
        {
            m_bHasAperture = true;
            m_fAperture = Imf::aperture(rqHeader);
        }

        // ISO speed
        m_bHasIsoSpeed = false;
        if ( Imf::hasIsoSpeed(rqHeader) )
        {
            m_bHasIsoSpeed = true;
            m_fIsoSpeed = Imf::isoSpeed(rqHeader);
        }

        // type of environment map
        m_bIsEnvironmentMap = false;
        if ( Imf::hasEnvmap(rqHeader) )
        {
            m_bIsEnvironmentMap = true;
            switch ( Imf::envmap(rqHeader) )
            {
            case Imf::ENVMAP_LATLONG:
                m_eEnvironmentMapType = EMT_LATITUDE_LONGITUDE;
                break;
            case Imf::ENVMAP_CUBE:
                m_eEnvironmentMapType = EMT_CUBE;
                break;
            default:
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }
        }

        // key-code for motion picture film frames
        m_bHasKeyCode = false;
        if ( Imf::hasKeyCode(rqHeader) )
        {
            const Imf::KeyCode& rqKC = Imf::keyCode(rqHeader);

            m_bHasKeyCode = true;
            m_qKeyCode.FilmManufacturerCode = rqKC.filmMfcCode();
            m_qKeyCode.FilmTypeCode = rqKC.filmType();
            m_qKeyCode.FilmRollPrefix = rqKC.prefix();
            m_qKeyCode.Count = rqKC.count();
            m_qKeyCode.FrameOffset = rqKC.perfOffset();
            m_qKeyCode.PerforationsPerFrame = rqKC.perfsPerFrame();
            m_qKeyCode.PerforationsPerCount = rqKC.perfsPerCount();
        }

        // time-code
        m_bHasTimeCode = false;
        if ( Imf::hasTimeCode(rqHeader) )
        {
            const Imf::TimeCode& rqTC = Imf::timeCode(rqHeader);

            m_bHasTimeCode = true;
            m_qTimeCode.Hours = rqTC.hours();
            m_qTimeCode.Minutes = rqTC.minutes();
            m_qTimeCode.Seconds = rqTC.seconds();
            m_qTimeCode.Frame = rqTC.frame();
            m_qTimeCode.DropFrame = rqTC.dropFrame();
            m_qTimeCode.ColorFrame = rqTC.colorFrame();
            m_qTimeCode.FieldPhase = rqTC.fieldPhase();
            m_qTimeCode.Bgf0 = rqTC.bgf0();
            m_qTimeCode.Bgf1 = rqTC.bgf1();
            m_qTimeCode.Bgf2 = rqTC.bgf2();
            for (int i = 0; i < 8; i++)
                m_qTimeCode.BinaryGroup[i] = rqTC.binaryGroup(i+1);
        }

        // how texture map images are extrapolated
        m_bHasWrapmodes = false;
        if ( Imf::hasWrapmodes(rqHeader) )
        {
            const std::string& rqWM = Imf::wrapmodes(rqHeader);

            m_bHasWrapmodes = true;
            m_qWrapmodes.SetLength((int)rqWM.size());
            for (int i = 0; i < m_qWrapmodes.GetLength(); i++)
                m_qWrapmodes[i] = rqWM[i];
        }
    }
    catch (...)
    {
        C_THROW_EXCEPTION(ET_OPEN_EXR_ERROR);
    }

    if ( m_bIsTiled
    &&   (m_eLevelMode == LMT_MIPMAP_LEVELS || m_eLevelMode == LMT_RIPMAP_LEVELS) )
    {
        // this is taken out because otherwise there would be another OpenEXR object that opens the same file
        // again
        Imf::TiledInputFile qTInFile(acFilename);
        if ( m_eLevelMode == LMT_MIPMAP_LEVELS )
        {
            m_iMipmapLevelQuantity = qTInFile.numLevels();
        }
        else  // m_eLevelMode = LMT_RIPMAP_LEVELS
        {
            m_iRipmapLevelQuantityX = qTInFile.numXLevels();
            m_iRipmapLevelQuantityY = qTInFile.numYLevels();
        }
    }

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatExr::LoadImage (RasterImagePtr& rspqImage)
{
    // number of scanlines in a transmission block
    static const int s_iScanlinesPerChunk = 1 << 4;

    // allocate the output image
    AllocOutImage(rspqImage,m_iImageWidth,m_iImageHeight);

    // for a progress bar
    ProgressProbe qProgress(this);

    try
    {
        // read the image as RGBA and convert it into image format actually being outputted
        float* afPix;
        int iY = 0;
        Imf::RgbaInputFile qInFile(m_qFilename);
        Imath::Box2i qDW = qInFile.dataWindow();
        Imf::Array2D<Imf::Rgba> qChunk(s_iScanlinesPerChunk,m_iImageWidth);
        while ( qDW.min.y <= qDW.max.y )
        {
            qInFile.setFrameBuffer(&qChunk[0][0]-qDW.min.x-qDW.min.y*m_iImageWidth,1,m_iImageWidth);
            qInFile.readPixels(qDW.min.y,Mathi::Min(qDW.min.y+s_iScanlinesPerChunk-1,qDW.max.y));

            if ( rspqImage->GetType() == RasterImage::IT_RGB_FLOAT )
            {
                for (int i = 0; i < s_iScanlinesPerChunk && iY < m_iImageHeight; i++, iY++)
                {
                    afPix = (float*)rspqImage->GetRow(iY);
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        const Imf::Rgba& rqRgba = qChunk[i][iX];

                        afPix[0] = rqRgba.r;
                        afPix[1] = rqRgba.g;
                        afPix[2] = rqRgba.b;

                        OnReadPixel(afPix,3,false);

                        afPix += 3;
                    }
                }
            }
            else if ( rspqImage->GetType() == RasterImage::IT_RGBA_FLOAT )
            {
                for (int i = 0; i < s_iScanlinesPerChunk && iY < m_iImageHeight; i++, iY++)
                {
                    afPix = (float*)rspqImage->GetRow(iY);
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        const Imf::Rgba& rqRgba = qChunk[i][iX];

                        afPix[0] = rqRgba.r;
                        afPix[1] = rqRgba.g;
                        afPix[2] = rqRgba.b;
                        afPix[3] = rqRgba.a;

                        OnReadPixel(afPix,3,true);

                        afPix += 4;
                    }
                }
            }
            else if ( rspqImage->GetType() == RasterImage::IT_GRAYSCALE_FLOAT )
            {
                for (int i = 0; i < s_iScanlinesPerChunk && iY < m_iImageHeight; i++, iY++)
                {
                    afPix = (float*)rspqImage->GetRow(iY);
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        const Imf::Rgba& rqRgba = qChunk[i][iX];

                        // assume that rqRgba.r = rqRgba.g = rqRgba.b
                        afPix[0] = rqRgba.r;

                        OnReadPixel(afPix,1,false);

                        afPix += 1;
                    }
                }
            }
            else  // rspqImage->GetType() = RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT
            {
                for (int i = 0; i < s_iScanlinesPerChunk && iY < m_iImageHeight; i++, iY++)
                {
                    afPix = (float*)rspqImage->GetRow(iY);
                    for (int iX = 0; iX < m_iImageWidth; iX++)
                    {
                        const Imf::Rgba& rqRgba = qChunk[i][iX];

                        // assume that rqRgba.r = rqRgba.g = rqRgba.b
                        afPix[0] = rqRgba.r;
                        afPix[1] = rqRgba.a;

                        OnReadPixel(afPix,1,true);

                        afPix += 2;
                    }
                }
            }

            qDW.min.y += s_iScanlinesPerChunk;

            qProgress.SetDone(((float)iY)/m_iImageHeight);
        }
    }
    catch (...)
    {
        THROW_EXCEPTION(ET_OPEN_EXR_ERROR);
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatExr::OnWritePixel (ColorRgba& rqPixel, bool bHasAlpha) const
{
    if ( m_bOffScreenMode )
    {
        // apply off-screen mode
        if ( m_fOffScreenGamma != 0.0f )
        {
            // apply inverse gamma
            for (int i = 0; i < 3; i++)
                rqPixel[i] = Mathf::Pow(rqPixel[i],m_fOffScreenGamma);
        }

        // apply inverse exposure
        for (int i = 0; i < 3; i++)
            rqPixel[i] *= m_fOffScreenExposureCoeff;
    }

    if ( bHasAlpha && m_bMultiplyAlpha )
    {
        // associate alpha
        for (int i = 0; i < 3; i++)
            rqPixel[i] *= rqPixel[3];
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatExr::AllocOutImage (RasterImagePtr& rspqImage, int iWidth, int iHeight) const
{
    if ( m_eColorModel == CMT_RGB
    ||   m_eColorModel == CMT_YC )
    {
        rspqImage = new RasterImage(RasterImage::IT_RGB_FLOAT,iWidth,iHeight,true);
    }
    else if ( m_eColorModel == CMT_RGBA
         ||   m_eColorModel == CMT_YCA )
    {
        rspqImage = new RasterImage(RasterImage::IT_RGBA_FLOAT,iWidth,iHeight,true);
    }
    else if ( m_eColorModel == CMT_Y )
    {
        rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_FLOAT,iWidth,iHeight,true);
    }
    else  // m_eColorModel = CMT_YA
    {
        rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_FLOAT,iWidth,iHeight,true);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatExr::OnReadPixel (float* afPix, int iSQ, bool bHasAlpha) const
{
    // for a pixel provided, the function does alpha clamping, and may do on-screen-mode alpha unassociation
    // and application
    if ( bHasAlpha )
    {
        // clamp alpha value
        if ( afPix[iSQ] < 0.0f )
            afPix[iSQ] = 0.0f;
        else if ( afPix[iSQ] > 1.0f )
            afPix[iSQ] = 1.0f;

        if ( m_bUnmultiplyAlpha )
        {
            // unassociate alpha
            if ( afPix[iSQ] != 0.0f )
            {
                float fInvA = 1.0f/afPix[iSQ];
                for (int i = 0; i < iSQ; i++)
                    afPix[i] *= fInvA;
            }
            else
            {
                for (int i = 0; i < iSQ; i++)
                    afPix[i] = ( afPix[i] < 0.0f ? -Mathf::MAX_REAL : Mathf::MAX_REAL );
            }
        }
    }

    if ( m_bOnScreenMode )
    {
        // apply on-screen mode
        for (int i = 0; i < iSQ; i++)
        {
            // apply exposure
            afPix[i] *= m_fOnScreenExposureCoeff;

            if ( m_fOnScreenGamma != 0.0f )
            {
                // apply gamma
                afPix[i] = Mathf::Pow(afPix[i],m_fOnScreenInvGamma);
            }

            if ( afPix[i] != afPix[i]  // it can be NaN
            ||   afPix[i] < 0.0f )
            {
                afPix[i] = 0.0f;
            }
            else if ( afPix[i] > 1.0f )
            {
                afPix[i] = 1.0f;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



