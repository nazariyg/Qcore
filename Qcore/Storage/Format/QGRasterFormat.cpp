#include "QGRasterFormat.h"
#include "QExptWarnBase.mcr"
#include "QGRasterFormat.mcr"
#include "QProgressProbe.h"
#include "QFormatBmp.h"
#include "QFormatCin.h"
#include "QFormatDds.h"
#include "QFormatExif.h"
#include "QFormatExr.h"
#include "QFormatGif.h"
#include "QFormatHdr.h"
#include "QFormatIco.h"
#include "QFormatIff.h"
#include "QFormatJpeg.h"
#include "QFormatPcx.h"
#include "QFormatPng.h"
#include "QFormatPsd.h"
#include "QFormatRas.h"
#include "QFormatRla.h"
#include "QFormatSct.h"
#include "QFormatSgi.h"
#include "QFormatTga.h"
#include "QFormatTiff.h"
#include "QFormatWbmp.h"
#include "QRasterImageConversion.h"
using namespace Q;

// raster image file extensions
static const char* gs_acExtSetBmp = "bmp rle dib";
static const char* gs_acExtSetCin = "cin";
static const char* gs_acExtSetDds = "dds";
static const char* gs_acExtSetExr = "exr";
static const char* gs_acExtSetGif = "gif";
static const char* gs_acExtSetHdr = "hdr pic";
static const char* gs_acExtSetIco = "ico cur";
static const char* gs_acExtSetIff = "iff ilbm lbm ham ham8 ehb";
static const char* gs_acExtSetJpeg = "jpg jpeg jpe";
static const char* gs_acExtSetPcx = "pcx";
static const char* gs_acExtSetPng = "png";
static const char* gs_acExtSetPsd = "psd";
static const char* gs_acExtSetRas = "ras";
static const char* gs_acExtSetRla = "rla";
static const char* gs_acExtSetSct = "sct";
static const char* gs_acExtSetSgi = "sgi rgb bw rgba int inta";
static const char* gs_acExtSetTga = "tga vda icb vst";
static const char* gs_acExtSetTiff = "tif tiff";
static const char* gs_acExtSetWbmp = "wbmp wbm";

//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::DetectFormat (const char* acFilename, FormatType& reFormat)
{
    bool bSuccess;
    GRasterFormat qFmt(acFilename,bSuccess);
    if ( !bSuccess )
        return false;
    reFormat = qFmt.m_eFormat;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
GRasterFormat::GRasterFormat (const char* acFilename, bool& rbSuccess)
    :
    m_qFilename(acFilename),
    m_bFormatUnrecognized(false)
{
    // lock the file
    m_pqFile = fopen(acFilename,"rb");
    C_CONDITIONAL_THROW_EXCEPTION(m_pqFile,ET_CANNOT_ACCESS_FILE);

    // extract the extension (if any)
    String qExt;
    int iDotPos = -1;
    for (int i = m_qFilename.GetLength()-2; i >= 0; i--)
    {
        if ( m_qFilename[i] == '.' )
        {
            iDotPos = i;
            break;
        }
    }
    if ( iDotPos == -1 )
        goto Guessing;
    qExt = m_qFilename.GetSubstringByRange(iDotPos+1,m_qFilename.GetLength());
    qExt.Trim();
    qExt.ToLowerCase();

    // examine the extension; finish the construction if the corresponding format initializes Ok
    TEST_EXTENSION_AS(Bmp);
    TEST_EXTENSION_AS(Cin);
    TEST_EXTENSION_AS(Dds);
    TEST_EXTENSION_AS(Exr);
    TEST_EXTENSION_AS(Gif);
    TEST_EXTENSION_AS(Hdr);
    TEST_EXTENSION_AS(Ico);
    TEST_EXTENSION_AS(Iff);
    TEST_EXTENSION_AS(Jpeg);
    TEST_EXTENSION_AS(Pcx);
    TEST_EXTENSION_AS(Png);
    TEST_EXTENSION_AS(Psd);
    TEST_EXTENSION_AS(Ras);
    TEST_EXTENSION_AS(Rla);
    TEST_EXTENSION_AS(Sct);
    TEST_EXTENSION_AS(Sgi);
    TEST_EXTENSION_AS(Tga);
    TEST_EXTENSION_AS(Tiff);
    TEST_EXTENSION_AS(Wbmp);

Guessing:
    // file extension did not help, so try to guess the format from the file's data; formats that assume a
    // signature are to be tested in the first place
    TRY_TO_INIT_AS(Bmp);
    TRY_TO_INIT_AS(Cin);
    TRY_TO_INIT_AS(Dds);
    TRY_TO_INIT_AS(Exr);
    TRY_TO_INIT_AS(Gif);
    TRY_TO_INIT_AS(Ico);
    TRY_TO_INIT_AS(Iff);
    TRY_TO_INIT_AS(Jpeg);
    TRY_TO_INIT_AS(Pcx);
    TRY_TO_INIT_AS(Png);
    TRY_TO_INIT_AS(Psd);
    TRY_TO_INIT_AS(Ras);
    TRY_TO_INIT_AS(Sct);
    TRY_TO_INIT_AS(Sgi);
    TRY_TO_INIT_AS(Tiff);
    TRY_TO_INIT_AS(Hdr);   // a signature-free format
    TRY_TO_INIT_AS(Rla);   // a signature-free format
    TRY_TO_INIT_AS(Tga);   // a signature-free format
    TRY_TO_INIT_AS(Wbmp);  // a signature-free format

    // could not recognize the format
    m_bFormatUnrecognized = true;
    C_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_UNRECOGNIZED);
}
//------------------------------------------------------------------------------------------------------------------
GRasterFormat::~GRasterFormat ()
{
    if ( m_pqFile )
    {
        // unlock the file
        fclose(m_pqFile);
    }
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::LoadImage (RasterImagePtr& rspqImage)
{
    assert( !m_bFormatUnrecognized );

    // for a progress bar
    ProgressProbe qProgress(this);

    // load the image
    bool bSuccess;
    if ( m_eFormat == FT_BMP )
        LOAD_IMAGE(Bmp,LoadImage(rspqImage))
    else if ( m_eFormat == FT_CIN )
        LOAD_IMAGE(Cin,LoadImage(rspqImage))
    else if ( m_eFormat == FT_DDS )
        LOAD_IMAGE(Dds,LoadMainImage(rspqImage))
    else if ( m_eFormat == FT_EXR )
        LOAD_IMAGE(Exr,LoadImage(rspqImage))
    else if ( m_eFormat == FT_GIF )
        LOAD_IMAGE(Gif,LoadImage(rspqImage))
    else if ( m_eFormat == FT_HDR )
        LOAD_IMAGE(Hdr,LoadImage(rspqImage))
    else if ( m_eFormat == FT_ICO )
        LOAD_IMAGE(Ico,LoadImage(rspqImage,0))
    else if ( m_eFormat == FT_IFF )
        LOAD_IMAGE(Iff,LoadImage(rspqImage))
    else if ( m_eFormat == FT_JPEG )
        LOAD_IMAGE(Jpeg,LoadImage(rspqImage))
    else if ( m_eFormat == FT_PCX )
        LOAD_IMAGE(Pcx,LoadImage(rspqImage))
    else if ( m_eFormat == FT_PNG )
        LOAD_IMAGE(Png,LoadImage(rspqImage))
    else if ( m_eFormat == FT_PSD )
        LOAD_IMAGE(Psd,LoadCompositeImage(rspqImage))
    else if ( m_eFormat == FT_RAS )
        LOAD_IMAGE(Ras,LoadImage(rspqImage))
    else if ( m_eFormat == FT_RLA )
        LOAD_IMAGE(Rla,LoadImage(rspqImage))
    else if ( m_eFormat == FT_SCT )
        LOAD_IMAGE(Sct,LoadImage(rspqImage))
    else if ( m_eFormat == FT_SGI )
        LOAD_IMAGE(Sgi,LoadImage(rspqImage))
    else if ( m_eFormat == FT_TGA )
        LOAD_IMAGE(Tga,LoadImage(rspqImage))
    else if ( m_eFormat == FT_TIFF )
        LOAD_IMAGE(Tiff,LoadIFDImage(rspqImage,0))
    else  // m_eFormat = FT_WBMP
        LOAD_IMAGE(Wbmp,LoadImage(rspqImage))

    if ( !rspqImage->HasAlpha() )
    {
        if ( rspqImage->GetType() != RasterImage::IT_RGB_8_8_8 )
        {
            // convert to IT_RGB_8_8_8
            RasterImageConversion qConv(rspqImage,RasterImage::IT_RGB_8_8_8);
            qConv.RedirectProgressTo(this,qProgress,0.5f);
            rspqImage = qConv.GetConverted();
        }
    }
    else
    {
        if ( rspqImage->GetType() != RasterImage::IT_RGBA_8_8_8_8 )
        {
            // convert to IT_RGBA_8_8_8_8
            RasterImageConversion qConv(rspqImage,RasterImage::IT_RGBA_8_8_8_8);
            qConv.RedirectProgressTo(this,qProgress,0.5f);
            rspqImage = qConv.GetConverted();
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsBmp ()
{
    bool bSuccess;
    FormatBmp qBmp(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_BMP;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsCin ()
{
    bool bSuccess;
    FormatCin qCin(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_CIN;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsDds ()
{
    bool bSuccess;
    FormatDds qDds(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_DDS;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsExr ()
{
    bool bSuccess;
    FormatExr qExr(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_EXR;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsGif ()
{
    bool bSuccess;
    FormatGif qGif(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_GIF;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsHdr ()
{
    bool bSuccess;
    FormatHdr qHdr(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_HDR;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsIco ()
{
    bool bSuccess;
    FormatIco qIco(m_qFilename,bSuccess);
    if ( !bSuccess || qIco.GetImageQuantity() == 0 )
        return false;
    m_eFormat = FT_ICO;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsIff ()
{
    bool bSuccess;
    FormatIff qIff(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_IFF;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsJpeg ()
{
    bool bSuccess;
    FormatJpeg qJpeg(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_JPEG;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsPcx ()
{
    bool bSuccess;
    FormatPcx qPcx(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_PCX;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsPng ()
{
    bool bSuccess;
    FormatPng qPng(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_PNG;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsPsd ()
{
    bool bSuccess;
    FormatPsd qPsd(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_PSD;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsRas ()
{
    bool bSuccess;
    FormatRas qRas(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_RAS;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsRla ()
{
    bool bSuccess;
    FormatRla qRla(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_RLA;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsSct ()
{
    bool bSuccess;
    FormatSct qSct(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_SCT;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsSgi ()
{
    bool bSuccess;
    FormatSgi qSgi(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_SGI;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsTga ()
{
    bool bSuccess;
    FormatTga qTga(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_TGA;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsTiff ()
{
    bool bSuccess;
    FormatTiff qTiff(m_qFilename,bSuccess);
    if ( !bSuccess || qTiff.GetImageFileDirectoryQuantity() == 0 )
        return false;
    m_eFormat = FT_TIFF;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool GRasterFormat::InitAsWbmp ()
{
    bool bSuccess;
    FormatWbmp qWbmp(m_qFilename,bSuccess);
    if ( !bSuccess )
        return false;
    m_eFormat = FT_WBMP;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
void GRasterFormat::OnException (int iExceptionType)
{
    ExptWarnBase::OnException(iExceptionType);

    if ( m_pqFile )
    {
        // unlock the file
        if ( fclose(m_pqFile) != 0 )
            assert( false );
    }
}
//------------------------------------------------------------------------------------------------------------------



