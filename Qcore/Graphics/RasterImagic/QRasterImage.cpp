#include "QRasterImage.h"
#include "QMap.h"
#include "QSet.h"
#include "QSortUtils.h"
using namespace Q;

// indicates divisibility in bytes of image row size
const int RasterImage::ms_iRowAlignment(4);

// support for palette sorting
class EntryRecord
{
public:
    int OldIndex;
    PaletteColor Entry;
};
class EntryRecordComparator3
{
public:
    bool operator() (const EntryRecord& rqER0, const EntryRecord& rqER1) const
    {
        int iSum0 =
            rqER0.Entry.Components[0] +
            rqER0.Entry.Components[1] +
            rqER0.Entry.Components[2];
        int iSum1 =
            rqER1.Entry.Components[0] +
            rqER1.Entry.Components[1] +
            rqER1.Entry.Components[2];
        return ( iSum0 < iSum1 );
    };
};
class EntryRecordComparator4
{
public:
    bool operator() (const EntryRecord& rqER0, const EntryRecord& rqER1) const
    {
        int iSum0 =
            rqER0.Entry.Components[0] +
            rqER0.Entry.Components[1] +
            rqER0.Entry.Components[2] +
            rqER0.Entry.Components[3];
        int iSum1 =
            rqER1.Entry.Components[0] +
            rqER1.Entry.Components[1] +
            rqER1.Entry.Components[2] +
            rqER1.Entry.Components[3];
        return ( iSum0 < iSum1 );
    };
};

// support for individual pixel access
static const float gs_fInv255(1.0f/255);
static const float gs_fInv65535(1.0f/65535);
static const float gs_fInv7(1.0f/7);
static const float gs_fInv3(1.0f/3);
static const float gs_fInv31(1.0f/31);
static const float gs_fInv63(1.0f/63);
static const float gs_fInv15(1.0f/15);
static const float gs_fInv1023(1.0f/1023);
static const float gs_fInv100m255(((float)255)/100);
static const float gs_fInv100m65535(((float)65535)/100);
static const float gs_fInv255m100(((float)100)/255);
static const float gs_fInv65535m100(((float)100)/65535);
static const float gs_fInv256(1.0f/256);
static const double gs_dInv16777215(1.0/16777215);
static const double gs_dInv4294967295(1.0/4294967295);

//------------------------------------------------------------------------------------------------------------------
RasterImage::RasterImage (ImageType eType, int iWidth, int iHeight, const Palette& rqPalette)
    :
    m_eType(eType),
    m_qPalette(rqPalette),
    m_bIsHighDynamicRange(false)
{
    assert( eType == IT_PALETTED || eType == IT_PALETTED_WITH_ALPHA );
    assert( 1 <= rqPalette.GetQuantity() && rqPalette.GetQuantity() <= 256 );

    Initialize(iWidth,iHeight);
}
//------------------------------------------------------------------------------------------------------------------
RasterImage::RasterImage (ImageType eType, int iWidth, int iHeight, bool bIsHighDynamicRange)
    :
    m_eType(eType),
    m_bIsHighDynamicRange(bIsHighDynamicRange)
{
    assert( eType != IT_PALETTED && eType != IT_PALETTED_WITH_ALPHA );

    Initialize(iWidth,iHeight);

    assert( !(m_ePixelComponentType != PCT_FLOATING_POINT && bIsHighDynamicRange) );
}
//------------------------------------------------------------------------------------------------------------------
RasterImage::~RasterImage ()
{
    delete[] m_acImageContainer;
}
//------------------------------------------------------------------------------------------------------------------
bool RasterImage::OptimizePalette ()
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    Array<PaletteColor> qNewPalette;
    Map<int,int> qIndexOldToNew;
    for (int i = 0; i < m_qPalette.GetQuantity(); i++)
    {
        qNewPalette.Push(m_qPalette[i]);
        qIndexOldToNew[i] = i;
    }

    int iOldQuantity = m_qPalette.GetQuantity();

    // try to remove unreferenced entries
    Set<int> qRefEnt;
    Map<int,int>::IteratorT qIt;
    for (int iX = 0; iX < m_iWidth; iX++)
    {
        for (int iY = 0; iY < m_iHeight; iY++)
            qRefEnt.Insert(GetPixelIndex(iX,iY));
    }
    for (int i0 = 0; i0 < iOldQuantity; i0++)
    {
        if ( !qRefEnt.IsPresent(i0) )
        {
            qIt = qIndexOldToNew.Find(i0);
            qNewPalette.Remove(qIt->Second);
            qIt->Second = -1;
            for (int i1 = i0+1; i1 < iOldQuantity; i1++)
                qIndexOldToNew.Find(i1)->Second--;
        }
    }

    // try to remove duplicates
    Map<int,int>::IteratorT qIt0, qIt1;
    bool bSame;
    int iComponentQuantity = ( m_eColorModel == CMT_PALETTED ? 3 : 4 );
    for (int i0 = 0; i0 < iOldQuantity; i0++)
    {
        qIt0 = qIndexOldToNew.Find(i0);
        if ( qIt0->Second >= 0 )
        {
            const PaletteColor& rqPE0 = qNewPalette[qIt0->Second];
            for (int i1 = 0; i1 < i0; i1++)
            {
                qIt1 = qIndexOldToNew.Find(i1);
                if ( qIt1->Second >= 0 )
                {
                    const PaletteColor& rqPE1 = qNewPalette[qIt1->Second];

                    bSame = true;
                    for (int i2 = 0; i2 < iComponentQuantity; i2++)
                    {
                        if ( rqPE0.Components[i2] != rqPE1.Components[i2] )
                        {
                            bSame = false;
                            break;
                        }
                    }

                    if ( bSame )
                    {
                        qNewPalette.Remove(qIt0->Second);
                        qIt0->Second = qIt1->Second;
                        for (int i2 = i0+1; i2 < iOldQuantity; i2++)
                            qIndexOldToNew.Find(i2)->Second--;

                        break;
                    }
                }
            }
        }
    }

    bool bRet = ( qNewPalette.GetQuantity() != m_qPalette.GetQuantity() );

    // assign new palette and remap indices
    m_qPalette = qNewPalette;
    for (int iX = 0; iX < m_iWidth; iX++)
    {
        for (int iY = 0; iY < m_iHeight; iY++)
            SetPixelIndex(qIndexOldToNew.Find(GetPixelIndex(iX,iY))->Second,iX,iY);
    }

    return bRet;
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SortPalette ()
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    int iPaletteSize = m_qPalette.GetQuantity();

    // sort
    StillArray<EntryRecord> qPEArray(iPaletteSize);
    for (int i = 0; i < iPaletteSize; i++)
    {
        EntryRecord& rqER = qPEArray[i];

        rqER.OldIndex = i;
        rqER.Entry = m_qPalette[i];
    }
    if ( m_eColorModel == CMT_PALETTED )
        SortUtils<EntryRecord,EntryRecordComparator3>::Sort(qPEArray);
    else  // m_eColorModel = CMT_PALETTED_WITH_ALPHA
        SortUtils<EntryRecord,EntryRecordComparator4>::Sort(qPEArray);
    Map<int,int> qIndexOldToNew;
    for (int i = 0; i < iPaletteSize; i++)
        qIndexOldToNew[qPEArray[i].OldIndex] = i;

    // assign new palette and remap indices
    for (int i = 0; i < iPaletteSize; i++)
        m_qPalette[i] = qPEArray[i].Entry;
    for (int iX = 0; iX < m_iWidth; iX++)
    {
        for (int iY = 0; iY < m_iHeight; iY++)
            SetPixelIndex(qIndexOldToNew.Find(GetPixelIndex(iX,iY))->Second,iX,iY);
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelIndex (int iIndex, int iX, int iY) const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );
    assert( 0 <= iIndex && iIndex < m_qPalette.GetQuantity() );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    *pucPix = iIndex;
}
//------------------------------------------------------------------------------------------------------------------
int RasterImage::GetPixelIndex (int iX, int iY) const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    return *pucPix;
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelIntensity (float fIntensity, int iX, int iY) const
{
    assert( m_eColorModel == CMT_GRAYSCALE );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= fIntensity && fIntensity <= 1.0f)) );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_GRAYSCALE_8 )
    {
        *pucPix = Mathf::RoundToInt(fIntensity*255);
    }
    else if ( m_eType == IT_GRAYSCALE_16 )
    {
        *((unsigned short*)pucPix) = Mathf::RoundToInt(fIntensity*65535);
    }
    else if ( m_eType == IT_GRAYSCALE_24 )
    {
        // use 'double' instead of 'float'
        unsigned int uiValue = Mathd::RoundToInt(((double)fIntensity)*16777215);
#ifndef Q_BIG_ENDIAN
        MemCpy(pucPix,&uiValue,3);
#else
        MemCpy(pucPix,&uiValue+1,3);
#endif
    }
    else if ( m_eType == IT_GRAYSCALE_32 )
    {
        // use 'double' instead of 'float'
        *((unsigned int*)pucPix) = Mathd::RoundToInt(((double)fIntensity)*4294967295);
    }
    else  // m_eType = IT_GRAYSCALE_FLOAT
    {
        MemCpy(pucPix,&fIntensity,4);
    }
}
//------------------------------------------------------------------------------------------------------------------
float RasterImage::GetPixelIntensity (int iX, int iY) const
{
    assert( m_eColorModel == CMT_GRAYSCALE );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_GRAYSCALE_8 )
    {
        return ((float)*pucPix)*gs_fInv255;
    }
    else if ( m_eType == IT_GRAYSCALE_16 )
    {
        return ((float)*((unsigned short*)pucPix))*gs_fInv65535;
    }
    else if ( m_eType == IT_GRAYSCALE_24 )
    {
        // use 'double' instead of 'float'
        unsigned int uiValue = 0;
#ifndef Q_BIG_ENDIAN
        MemCpy(&uiValue,pucPix,3);
#else
        MemCpy(&uiValue+1,pucPix,3);
#endif
        return (float)(((double)uiValue)*gs_dInv16777215);
    }
    else if ( m_eType == IT_GRAYSCALE_32 )
    {
        return (float)(((double)*((unsigned int*)pucPix))*gs_dInv4294967295);
    }
    else  // m_eType = IT_GRAYSCALE_FLOAT
    {
        float fValue;
        MemCpy(&fValue,pucPix,4);
        return fValue;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelIntensityWithAlpha (const Link<float,float>& rqIntensityWithAlpha, int iX, int iY) const
{
    assert( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqIntensityWithAlpha.First && rqIntensityWithAlpha.First <= 1.0f)) );
    assert( 0.0f <= rqIntensityWithAlpha.Second && rqIntensityWithAlpha.Second <= 1.0f );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_GRAYSCALE_WITH_ALPHA_8_8 )
    {
        *(pucPix + 0) = Mathf::RoundToInt(rqIntensityWithAlpha.First*255);
        *(pucPix + 1) = Mathf::RoundToInt(rqIntensityWithAlpha.Second*255);
    }
    else if ( m_eType == IT_GRAYSCALE_WITH_ALPHA_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqIntensityWithAlpha.First*65535);
        *(pusPix + 1) = Mathf::RoundToInt(rqIntensityWithAlpha.Second*65535);
    }
    else  // m_eType = IT_GRAYSCALE_WITH_ALPHA_FLOAT
    {
        MemCpy(pucPix+0,&rqIntensityWithAlpha.First,4);
        MemCpy(pucPix+4,&rqIntensityWithAlpha.Second,4);
    }
}
//------------------------------------------------------------------------------------------------------------------
Link<float,float> RasterImage::GetPixelIntensityWithAlpha (int iX, int iY) const
{
    assert( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_GRAYSCALE_WITH_ALPHA_8_8 )
    {
        return Link<float,float>(
            ((float)*(pucPix+0))*gs_fInv255,
            ((float)*(pucPix+1))*gs_fInv255);
    }
    else if ( m_eType == IT_GRAYSCALE_WITH_ALPHA_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return Link<float,float>(
            ((float)*(pusPix+0))*gs_fInv65535,
            ((float)*(pusPix+1))*gs_fInv65535);
    }
    else  // m_eType = IT_GRAYSCALE_WITH_ALPHA_FLOAT
    {
        Link<float,float> qIntensityWithAlpha;
        MemCpy(&qIntensityWithAlpha.First,pucPix+0,4);
        MemCpy(&qIntensityWithAlpha.Second,pucPix+4,4);
        return qIntensityWithAlpha;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelRgb (const ColorRgb& rqRgb, int iX, int iY) const
{
    assert( m_eColorModel == CMT_RGB );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqRgb.R() && rqRgb.R() <= 1.0f
        &&  0.0f <= rqRgb.G() && rqRgb.G() <= 1.0f
        &&  0.0f <= rqRgb.B() && rqRgb.B() <= 1.0f)) );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_RGB_3_3_2 )
    {
        unsigned char ucValue = 0;
        ucValue |= Mathf::RoundToInt(rqRgb.R()*7) << 5;
        ucValue |= Mathf::RoundToInt(rqRgb.G()*7) << 2;
        ucValue |= Mathf::RoundToInt(rqRgb.B()*3);
        *pucPix = ucValue;
    }
    else if ( m_eType == IT_RGB_5_6_5 )
    {
        unsigned short usValue = 0;
        usValue |= Mathf::RoundToInt(rqRgb.R()*31) << 11;
        usValue |= Mathf::RoundToInt(rqRgb.G()*63) << 5;
        usValue |= Mathf::RoundToInt(rqRgb.B()*31);
        *((unsigned short*)pucPix) = usValue;
    }
    else if ( m_eType == IT_RGB_8_8_8 )
    {
        *(pucPix + 0) = Mathf::RoundToInt(rqRgb.R()*255);
        *(pucPix + 1) = Mathf::RoundToInt(rqRgb.G()*255);
        *(pucPix + 2) = Mathf::RoundToInt(rqRgb.B()*255);
    }
    else if ( m_eType == IT_RGB_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqRgb.R()*65535);
        *(pusPix + 1) = Mathf::RoundToInt(rqRgb.G()*65535);
        *(pusPix + 2) = Mathf::RoundToInt(rqRgb.B()*65535);
    }
    else  // m_eType = IT_RGB_FLOAT
    {
        MemCpy(pucPix,rqRgb,12);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb RasterImage::GetPixelRgb (int iX, int iY) const
{
    assert( m_eColorModel == CMT_RGB );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_RGB_3_3_2 )
    {
        return ColorRgb(
            ((float)((*pucPix >> 5) & 7))*gs_fInv7,
            ((float)((*pucPix >> 2) & 7))*gs_fInv7,
            ((float)((*pucPix >> 0) & 3))*gs_fInv3);
    }
    else if ( m_eType == IT_RGB_5_6_5 )
    {
        unsigned short usValue = *((unsigned short*)pucPix);
        return ColorRgb(
            ((float)((usValue >> 11) & 31))*gs_fInv31,
            ((float)((usValue >>  5) & 63))*gs_fInv63,
            ((float)((usValue >>  0) & 31))*gs_fInv31);
    }
    else if ( m_eType == IT_RGB_8_8_8 )
    {
        return ColorRgb(
            ((float)*(pucPix+0))*gs_fInv255,
            ((float)*(pucPix+1))*gs_fInv255,
            ((float)*(pucPix+2))*gs_fInv255);
    }
    else if ( m_eType == IT_RGB_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorRgb(
            ((float)*(pusPix+0))*gs_fInv65535,
            ((float)*(pusPix+1))*gs_fInv65535,
            ((float)*(pusPix+2))*gs_fInv65535);
    }
    else  // m_eType = IT_RGB_FLOAT
    {
        ColorRgb qRgb;
        MemCpy(qRgb,pucPix,12);
        return qRgb;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelRgba (const ColorRgba& rqRgba, int iX, int iY) const
{
    assert( m_eColorModel == CMT_RGBA );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqRgba.R() && rqRgba.R() <= 1.0f
        &&  0.0f <= rqRgba.G() && rqRgba.G() <= 1.0f
        &&  0.0f <= rqRgba.B() && rqRgba.B() <= 1.0f)) );
    assert( 0.0f <= rqRgba.A() && rqRgba.A() <= 1.0f );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_RGBA_4_4_4_4 )
    {
        unsigned short usValue = 0;
        usValue |= Mathf::RoundToInt(rqRgba.R()*15) << 12;
        usValue |= Mathf::RoundToInt(rqRgba.G()*15) << 8;
        usValue |= Mathf::RoundToInt(rqRgba.B()*15) << 4;
        usValue |= Mathf::RoundToInt(rqRgba.A()*15);
        *((unsigned short*)pucPix) = usValue;
    }
    else if ( m_eType == IT_RGBA_8_8_8_8 )
    {
        *(pucPix + 0) = Mathf::RoundToInt(rqRgba.R()*255);
        *(pucPix + 1) = Mathf::RoundToInt(rqRgba.G()*255);
        *(pucPix + 2) = Mathf::RoundToInt(rqRgba.B()*255);
        *(pucPix + 3) = Mathf::RoundToInt(rqRgba.A()*255);
    }
    else if ( m_eType == IT_RGBA_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqRgba.R()*65535);
        *(pusPix + 1) = Mathf::RoundToInt(rqRgba.G()*65535);
        *(pusPix + 2) = Mathf::RoundToInt(rqRgba.B()*65535);
        *(pusPix + 3) = Mathf::RoundToInt(rqRgba.A()*65535);
    }
    else if ( m_eType == IT_RGBA_5_5_5_1 )
    {
        unsigned short usValue = 0;
        usValue |= Mathf::RoundToInt(rqRgba.R()*31) << 11;
        usValue |= Mathf::RoundToInt(rqRgba.G()*31) << 6;
        usValue |= Mathf::RoundToInt(rqRgba.B()*31) << 1;
        usValue |= Mathf::RoundToInt(rqRgba.A());
        *((unsigned short*)pucPix) = usValue;
    }
    else if ( m_eType == IT_RGBA_10_10_10_2 )
    {
        unsigned int uiValue = 0;
        uiValue |= Mathf::RoundToInt(rqRgba.R()*1023) << 22;
        uiValue |= Mathf::RoundToInt(rqRgba.G()*1023) << 12;
        uiValue |= Mathf::RoundToInt(rqRgba.B()*1023) << 2;
        uiValue |= Mathf::RoundToInt(rqRgba.A()*3);
        *((unsigned int*)pucPix) = uiValue;
    }
    else  // m_eType = IT_RGBA_FLOAT
    {
        MemCpy(pucPix,rqRgba,16);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba RasterImage::GetPixelRgba (int iX, int iY) const
{
    assert( m_eColorModel == CMT_RGBA );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_RGBA_4_4_4_4 )
    {
        unsigned short usValue = *((unsigned short*)pucPix);
        return ColorRgba(
            ((float)((usValue >> 12) & 15))*gs_fInv15,
            ((float)((usValue >>  8) & 15))*gs_fInv15,
            ((float)((usValue >>  4) & 15))*gs_fInv15,
            ((float)((usValue >>  0) & 15))*gs_fInv15);
    }
    else if ( m_eType == IT_RGBA_8_8_8_8 )
    {
        return ColorRgba(
            ((float)*(pucPix+0))*gs_fInv255,
            ((float)*(pucPix+1))*gs_fInv255,
            ((float)*(pucPix+2))*gs_fInv255,
            ((float)*(pucPix+3))*gs_fInv255);
    }
    else if ( m_eType == IT_RGBA_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorRgba(
            ((float)*(pusPix+0))*gs_fInv65535,
            ((float)*(pusPix+1))*gs_fInv65535,
            ((float)*(pusPix+2))*gs_fInv65535,
            ((float)*(pusPix+3))*gs_fInv65535);
    }
    else if ( m_eType == IT_RGBA_5_5_5_1 )
    {
        unsigned short usValue = *((unsigned short*)pucPix);
        return ColorRgba(
            ((float)((usValue >> 11) & 31))*gs_fInv31,
            ((float)((usValue >>  6) & 31))*gs_fInv31,
            ((float)((usValue >>  1) & 31))*gs_fInv31,
            ((float)((usValue >>  0) &  1)));
    }
    else if ( m_eType == IT_RGBA_10_10_10_2 )
    {
        unsigned int uiValue = *((unsigned int*)pucPix);
        return ColorRgba(
            ((float)((uiValue >> 22) & 1023))*gs_fInv1023,
            ((float)((uiValue >> 12) & 1023))*gs_fInv1023,
            ((float)((uiValue >>  2) & 1023))*gs_fInv1023,
            ((float)((uiValue >>  0) &    3))*gs_fInv3);
    }
    else  // m_eType = IT_RGBA_FLOAT
    {
        ColorRgba qRgba;
        MemCpy(qRgba,pucPix,16);
        return qRgba;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelCmyk (const ColorCmyk& rqCmyk, int iX, int iY) const
{
    assert( m_eColorModel == CMT_CMYK );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqCmyk.C() && rqCmyk.C() <= 1.0f
        &&  0.0f <= rqCmyk.M() && rqCmyk.M() <= 1.0f
        &&  0.0f <= rqCmyk.Y() && rqCmyk.Y() <= 1.0f
        &&  0.0f <= rqCmyk.K() && rqCmyk.K() <= 1.0f)) );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_CMYK_8_8_8_8 )
    {
        *(pucPix + 0) = Mathf::RoundToInt(rqCmyk.C()*255);
        *(pucPix + 1) = Mathf::RoundToInt(rqCmyk.M()*255);
        *(pucPix + 2) = Mathf::RoundToInt(rqCmyk.Y()*255);
        *(pucPix + 3) = Mathf::RoundToInt(rqCmyk.K()*255);
    }
    else if ( m_eType == IT_CMYK_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqCmyk.C()*65535);
        *(pusPix + 1) = Mathf::RoundToInt(rqCmyk.M()*65535);
        *(pusPix + 2) = Mathf::RoundToInt(rqCmyk.Y()*65535);
        *(pusPix + 3) = Mathf::RoundToInt(rqCmyk.K()*65535);
    }
    else  // m_eType = IT_CMYK_FLOAT
    {
        MemCpy(pucPix,rqCmyk,16);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk RasterImage::GetPixelCmyk (int iX, int iY) const
{
    assert( m_eColorModel == CMT_CMYK );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_CMYK_8_8_8_8 )
    {
        return ColorCmyk(
            ((float)*(pucPix+0))*gs_fInv255,
            ((float)*(pucPix+1))*gs_fInv255,
            ((float)*(pucPix+2))*gs_fInv255,
            ((float)*(pucPix+3))*gs_fInv255);
    }
    else if ( m_eType == IT_CMYK_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorCmyk(
            ((float)*(pusPix+0))*gs_fInv65535,
            ((float)*(pusPix+1))*gs_fInv65535,
            ((float)*(pusPix+2))*gs_fInv65535,
            ((float)*(pusPix+3))*gs_fInv65535);
    }
    else  // m_eType = IT_CMYK_FLOAT
    {
        ColorCmyk qCmyk;
        MemCpy(qCmyk,pucPix,16);
        return qCmyk;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelCmyka (const ColorCmyka& rqCmyka, int iX, int iY) const
{
    assert( m_eColorModel == CMT_CMYK_WITH_ALPHA );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqCmyka.C() && rqCmyka.C() <= 1.0f
        &&  0.0f <= rqCmyka.M() && rqCmyka.M() <= 1.0f
        &&  0.0f <= rqCmyka.Y() && rqCmyka.Y() <= 1.0f
        &&  0.0f <= rqCmyka.K() && rqCmyka.K() <= 1.0f)) );
    assert( 0.0f <= rqCmyka.Alpha() && rqCmyka.Alpha() <= 1.0f );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_CMYK_WITH_ALPHA_8_8_8_8_8 )
    {
        *(pucPix + 0) = Mathf::RoundToInt(rqCmyka.C()*255);
        *(pucPix + 1) = Mathf::RoundToInt(rqCmyka.M()*255);
        *(pucPix + 2) = Mathf::RoundToInt(rqCmyka.Y()*255);
        *(pucPix + 3) = Mathf::RoundToInt(rqCmyka.K()*255);
        *(pucPix + 4) = Mathf::RoundToInt(rqCmyka.Alpha()*255);
    }
    else if ( m_eType == IT_CMYK_WITH_ALPHA_16_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqCmyka.C()*65535);
        *(pusPix + 1) = Mathf::RoundToInt(rqCmyka.M()*65535);
        *(pusPix + 2) = Mathf::RoundToInt(rqCmyka.Y()*65535);
        *(pusPix + 3) = Mathf::RoundToInt(rqCmyka.K()*65535);
        *(pusPix + 4) = Mathf::RoundToInt(rqCmyka.Alpha()*65535);
    }
    else  // m_eType = IT_CMYK_WITH_ALPHA_FLOAT
    {
        MemCpy(pucPix,rqCmyka,20);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka RasterImage::GetPixelCmyka (int iX, int iY) const
{
    assert( m_eColorModel == CMT_CMYK_WITH_ALPHA );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_CMYK_WITH_ALPHA_8_8_8_8_8 )
    {
        return ColorCmyka(
            ((float)*(pucPix+0))*gs_fInv255,
            ((float)*(pucPix+1))*gs_fInv255,
            ((float)*(pucPix+2))*gs_fInv255,
            ((float)*(pucPix+3))*gs_fInv255,
            ((float)*(pucPix+4))*gs_fInv255);
    }
    else if ( m_eType == IT_CMYK_WITH_ALPHA_16_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorCmyka(
            ((float)*(pusPix+0))*gs_fInv65535,
            ((float)*(pusPix+1))*gs_fInv65535,
            ((float)*(pusPix+2))*gs_fInv65535,
            ((float)*(pusPix+3))*gs_fInv65535,
            ((float)*(pusPix+4))*gs_fInv65535);
    }
    else  // m_eType = IT_CMYK_WITH_ALPHA_FLOAT
    {
        ColorCmyka qCmyka;
        MemCpy(qCmyka,pucPix,20);
        return qCmyka;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelLab (const ColorLab& rqLab, int iX, int iY) const
{
    assert( m_eColorModel == CMT_LAB );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqLab.L() && rqLab.L() <= 100.0f)) );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_LAB_8_8_8 )
    {
        // clamp a* and b* to [-128, 127] and encode them as unsigned
        float fA = rqLab.A() + 128;
        float fB = rqLab.B() + 128;
        if ( fA < 0 )
            fA = 0;
        else if ( fA > 255 )
            fA = 255;
        if ( fB < 0 )
            fB = 0;
        else if ( fB > 255 )
            fB = 255;
        *(pucPix + 0) = Mathf::RoundToInt(rqLab.L()*gs_fInv100m255);
        *(pucPix + 1) = Mathf::RoundToInt(fA);
        *(pucPix + 2) = Mathf::RoundToInt(fB);
    }
    else if ( m_eType == IT_LAB_16_16_16 )
    {
        // clamp a* and b* to [-128, 127] and encode them as unsigned scaled by 256
        float fA = (rqLab.A() + 128)*256;
        float fB = (rqLab.B() + 128)*256;
        if ( fA < 0 )
            fA = 0;
        else if ( fA > 65535 )
            fA = 65535;
        if ( fB < 0 )
            fB = 0;
        else if ( fB > 65535 )
            fB = 65535;
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqLab.L()*gs_fInv100m65535);
        *(pusPix + 1) = Mathf::RoundToInt(fA);
        *(pusPix + 2) = Mathf::RoundToInt(fB);
    }
    else  // m_eType = IT_LAB_FLOAT
    {
        MemCpy(pucPix,rqLab,12);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorLab RasterImage::GetPixelLab (int iX, int iY) const
{
    assert( m_eColorModel == CMT_LAB );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_LAB_8_8_8 )
    {
        return ColorLab(
            ((float)*(pucPix+0))*gs_fInv255m100,
            ((float)*(pucPix+1))-128,
            ((float)*(pucPix+2))-128);
    }
    else if ( m_eType == IT_LAB_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorLab(
            ((float)*(pusPix+0))*gs_fInv65535m100,
            ((float)*(pusPix+1))*gs_fInv256-128,
            ((float)*(pusPix+2))*gs_fInv256-128);
    }
    else  // m_eType = IT_LAB_FLOAT
    {
        ColorLab qLab;
        MemCpy(qLab,pucPix,12);
        return qLab;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetPixelLaba (const ColorLaba& rqLaba, int iX, int iY) const
{
    assert( m_eColorModel == CMT_LAB_WITH_ALPHA );
    assert( !(!m_bIsHighDynamicRange && !(
            0.0f <= rqLaba.L() && rqLaba.L() <= 100.0f)) );
    assert( 0.0f <= rqLaba.Alpha() && rqLaba.Alpha() <= 1.0f );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_LAB_WITH_ALPHA_8_8_8_8 )
    {
        // clamp a* and b* to [-128, 127] and encode them as unsigned
        float fA = rqLaba.A() + 128;
        float fB = rqLaba.B() + 128;
        if ( fA < 0 )
            fA = 0;
        else if ( fA > 255 )
            fA = 255;
        if ( fB < 0 )
            fB = 0;
        else if ( fB > 255 )
            fB = 255;
        *(pucPix + 0) = Mathf::RoundToInt(rqLaba.L()*gs_fInv100m255);
        *(pucPix + 1) = Mathf::RoundToInt(fA);
        *(pucPix + 2) = Mathf::RoundToInt(fB);
        *(pucPix + 3) = Mathf::RoundToInt(rqLaba.Alpha()*255);
    }
    else if ( m_eType == IT_LAB_WITH_ALPHA_16_16_16_16 )
    {
        // clamp a* and b* to [-128, 127] and encode them as unsigned scaled by 256
        float fA = (rqLaba.A() + 128)*256;
        float fB = (rqLaba.B() + 128)*256;
        if ( fA < 0 )
            fA = 0;
        else if ( fA > 65535 )
            fA = 65535;
        if ( fB < 0 )
            fB = 0;
        else if ( fB > 65535 )
            fB = 65535;
        unsigned short* pusPix = (unsigned short*)pucPix;
        *(pusPix + 0) = Mathf::RoundToInt(rqLaba.L()*gs_fInv100m65535);
        *(pusPix + 1) = Mathf::RoundToInt(fA);
        *(pusPix + 2) = Mathf::RoundToInt(fB);
        *(pusPix + 3) = Mathf::RoundToInt(rqLaba.Alpha()*65535);
    }
    else  // m_eType = IT_LAB_WITH_ALPHA_FLOAT
    {
        MemCpy(pucPix,rqLaba,16);
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba RasterImage::GetPixelLaba (int iX, int iY) const
{
    assert( m_eColorModel == CMT_LAB_WITH_ALPHA );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    unsigned char* pucPix = (unsigned char*)(m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel);
    if ( m_eType == IT_LAB_WITH_ALPHA_8_8_8_8 )
    {
        return ColorLaba(
            ((float)*(pucPix+0))*gs_fInv255m100,
            ((float)*(pucPix+1))-128,
            ((float)*(pucPix+2))-128,
            ((float)*(pucPix+3))*gs_fInv255);
    }
    else if ( m_eType == IT_LAB_WITH_ALPHA_16_16_16_16 )
    {
        unsigned short* pusPix = (unsigned short*)pucPix;
        return ColorLaba(
            ((float)*(pusPix+0))*gs_fInv65535m100,
            ((float)*(pusPix+1))*gs_fInv256-128,
            ((float)*(pusPix+2))*gs_fInv256-128,
            ((float)*(pusPix+3))*gs_fInv65535);
    }
    else  // m_eType = IT_LAB_WITH_ALPHA_FLOAT
    {
        ColorLaba qLaba;
        MemCpy(qLaba,pucPix,16);
        return qLaba;
    }
}
//------------------------------------------------------------------------------------------------------------------
SmartPointer<RasterImage> RasterImage::GetDuplicate (bool bCopyPixels, bool bCopyVisibilityMask) const
{
    // allocate the copy image
    RasterImagePtr spqCopy;
    if ( m_eColorModel == CMT_PALETTED || m_eColorModel == CMT_PALETTED_WITH_ALPHA )
        spqCopy = new RasterImage(m_eType,m_iWidth,m_iHeight,m_qPalette);
    else
        spqCopy = new RasterImage(m_eType,m_iWidth,m_iHeight,m_bIsHighDynamicRange);

    if ( bCopyPixels )
    {
        // copy pixels
        MemCpy(spqCopy->m_acImage,m_acImage,m_iHeight*m_iRowStride);
    }

    if ( !m_qVisibilityMask.IsEmpty() )
    {
        spqCopy->m_qVisibilityMask.SetQuantity(m_qVisibilityMask.GetQuantity());
        if ( bCopyVisibilityMask )
            MemCpy(spqCopy->m_qVisibilityMask,m_qVisibilityMask,m_qVisibilityMask.GetQuantity());
    }

    return spqCopy;
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::SetSubimage (const RasterImagePtr& rspqSubimage, int iX, int iY)
{
    // only an image with the same characteristics can be pasted
    assert( iX >= 0 && iY >= 0 );
    assert( iX + rspqSubimage->m_iWidth <= m_iWidth && iY + rspqSubimage->m_iHeight <= m_iHeight );
    assert( rspqSubimage->m_eType == m_eType );
    assert( rspqSubimage->m_qPalette == m_qPalette );
    assert( rspqSubimage->m_bIsHighDynamicRange == m_bIsHighDynamicRange );
    assert( rspqSubimage->m_qVisibilityMask.IsEmpty() == m_qVisibilityMask.IsEmpty() );

    // paste the pixels provided
    const char* acRow0 = rspqSubimage->m_acImage;
    char* acRow1 = m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel;
    int iRowSize = rspqSubimage->m_iWidth*m_iBytesPerPixel;
    for (int i = 0; i < rspqSubimage->m_iHeight; i++)
    {
        MemCpy(acRow1,acRow0,iRowSize);

        acRow0 += rspqSubimage->m_iRowStride;
        acRow1 += m_iRowStride;
    }

    if ( !rspqSubimage->m_qVisibilityMask.IsEmpty() )
    {
        // do similar for the visibility mask
        acRow0 = rspqSubimage->m_qVisibilityMask;
        acRow1 = m_qVisibilityMask + iY*m_iWidth + iX;
        for (int i = 0; i < rspqSubimage->m_iHeight; i++)
        {
            MemCpy(acRow1,acRow0,rspqSubimage->m_iWidth);

            acRow0 += rspqSubimage->m_iWidth;
            acRow1 += m_iWidth;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
SmartPointer<RasterImage> RasterImage::GetSubimage (int iX, int iY, int iWidth, int iHeight) const
{
    assert( iX >= 0 && iY >= 0 && iWidth >= 0 && iHeight >= 0 );
    assert( iX + iWidth <= m_iWidth && iY + iHeight <= m_iHeight );

    // allocate the subimage
    RasterImagePtr spqSubimage;
    if ( m_eColorModel == CMT_PALETTED || m_eColorModel == CMT_PALETTED_WITH_ALPHA )
        spqSubimage = new RasterImage(m_eType,iWidth,iHeight,m_qPalette);
    else
        spqSubimage = new RasterImage(m_eType,iWidth,iHeight,m_bIsHighDynamicRange);

    // extract sub-pixels
    const char* acRow0 = m_acImage + iY*m_iRowStride + iX*m_iBytesPerPixel;
    char* acRow1 = spqSubimage->m_acImage;
    int iRowSize = iWidth*m_iBytesPerPixel;
    for (int i = 0; i < iHeight; i++)
    {
        MemCpy(acRow1,acRow0,iRowSize);

        acRow0 += m_iRowStride;
        acRow1 += spqSubimage->m_iRowStride;
    }

    if ( !m_qVisibilityMask.IsEmpty() )
    {
        // do similar for the visibility mask
        spqSubimage->m_qVisibilityMask.SetQuantity(iWidth*iHeight);

        acRow0 = m_qVisibilityMask + iY*m_iWidth + iX;
        acRow1 = spqSubimage->m_qVisibilityMask;
        for (int i = 0; i < iHeight; i++)
        {
            MemCpy(acRow1,acRow0,iWidth);

            acRow0 += m_iWidth;
            acRow1 += iWidth;
        }
    }

    return spqSubimage;
}
//------------------------------------------------------------------------------------------------------------------
SmartPointer<RasterImage> RasterImage::GetSubimage (const Recti& rqRect) const
{
    return GetSubimage(rqRect.X,rqRect.Y,rqRect.Width,rqRect.Height);
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::ClampComponentsToUnitInterval ()
{
    assert( m_bIsHighDynamicRange );

    int iSamplesInRow = m_iWidth*m_iComponentsPerPixel;
    char* acRow = m_acImage;
    float* pfSample;
    for (int i0 = 0; i0 < m_iHeight; i0++)
    {
        pfSample = (float*)acRow;
        for (int i1 = 0; i1 < iSamplesInRow; i1++)
        {
            if ( *pfSample < 0.0f )
                *pfSample = 0.0f;
            else if ( *pfSample > 1.0f )
                *pfSample = 1.0f;

            pfSample++;
        }

        acRow += m_iRowStride;
    }
}
//------------------------------------------------------------------------------------------------------------------
RasterImage::ColorModelType RasterImage::GetColorModelFromType (ImageType eType)
{
    // generalize image type to color model
    switch ( eType )
    {
    case IT_PALETTED:
        return CMT_PALETTED;
    case IT_PALETTED_WITH_ALPHA:
        return CMT_PALETTED_WITH_ALPHA;
    case IT_GRAYSCALE_8:
        return CMT_GRAYSCALE;
    case IT_GRAYSCALE_16:
        return CMT_GRAYSCALE;
    case IT_GRAYSCALE_24:
        return CMT_GRAYSCALE;
    case IT_GRAYSCALE_32:
        return CMT_GRAYSCALE;
    case IT_GRAYSCALE_FLOAT:
        return CMT_GRAYSCALE;
    case IT_GRAYSCALE_WITH_ALPHA_8_8:
        return CMT_GRAYSCALE_WITH_ALPHA;
    case IT_GRAYSCALE_WITH_ALPHA_16_16:
        return CMT_GRAYSCALE_WITH_ALPHA;
    case IT_GRAYSCALE_WITH_ALPHA_FLOAT:
        return CMT_GRAYSCALE_WITH_ALPHA;
    case IT_RGB_3_3_2:
        return CMT_RGB;
    case IT_RGB_5_6_5:
        return CMT_RGB;
    case IT_RGB_8_8_8:
        return CMT_RGB;
    case IT_RGB_16_16_16:
        return CMT_RGB;
    case IT_RGB_FLOAT:
        return CMT_RGB;
    case IT_RGBA_4_4_4_4:
        return CMT_RGBA;
    case IT_RGBA_8_8_8_8:
        return CMT_RGBA;
    case IT_RGBA_16_16_16_16:
        return CMT_RGBA;
    case IT_RGBA_5_5_5_1:
        return CMT_RGBA;
    case IT_RGBA_10_10_10_2:
        return CMT_RGBA;
    case IT_RGBA_FLOAT:
        return CMT_RGBA;
    case IT_CMYK_8_8_8_8:
        return CMT_CMYK;
    case IT_CMYK_16_16_16_16:
        return CMT_CMYK;
    case IT_CMYK_FLOAT:
        return CMT_CMYK;
    case IT_CMYK_WITH_ALPHA_8_8_8_8_8:
        return CMT_CMYK_WITH_ALPHA;
    case IT_CMYK_WITH_ALPHA_16_16_16_16_16:
        return CMT_CMYK_WITH_ALPHA;
    case IT_CMYK_WITH_ALPHA_FLOAT:
        return CMT_CMYK_WITH_ALPHA;
    case IT_LAB_8_8_8:
        return CMT_LAB;
    case IT_LAB_16_16_16:
        return CMT_LAB;
    case IT_LAB_FLOAT:
        return CMT_LAB;
    case IT_LAB_WITH_ALPHA_8_8_8_8:
        return CMT_LAB_WITH_ALPHA;
    case IT_LAB_WITH_ALPHA_16_16_16_16:
        return CMT_LAB_WITH_ALPHA;
    default:  // IT_LAB_WITH_ALPHA_FLOAT (to avoid the warning)
        return CMT_LAB_WITH_ALPHA;
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImage::Initialize (int iWidth, int iHeight)
{
    assert( iWidth >= 0 && iHeight >= 0 );

    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iArea = iWidth*iHeight;

    // derive image attributes from image type
    m_eColorModel = GetColorModelFromType(m_eType);
    switch ( m_eType )
    {
    case IT_PALETTED:
        m_iBitsPerPixel = 8;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_PALETTED_WITH_ALPHA:
        m_iBitsPerPixel = 8;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_8:
        m_iBitsPerPixel = 8;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_16:
        m_iBitsPerPixel = 16;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_24:
        m_iBitsPerPixel = 24;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_32:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_FLOAT:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 1;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = false;
        break;
    case IT_GRAYSCALE_WITH_ALPHA_8_8:
        m_iBitsPerPixel = 16;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 2;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_GRAYSCALE_WITH_ALPHA_16_16:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 2;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_GRAYSCALE_WITH_ALPHA_FLOAT:
        m_iBitsPerPixel = 64;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 2;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = true;
        break;
    case IT_RGB_3_3_2:
        m_iBitsPerPixel = 8;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_RGB_5_6_5:
        m_iBitsPerPixel = 16;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_RGB_8_8_8:
        m_iBitsPerPixel = 24;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_RGB_16_16_16:
        m_iBitsPerPixel = 48;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_RGB_FLOAT:
        m_iBitsPerPixel = 96;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = false;
        break;
    case IT_RGBA_4_4_4_4:
        m_iBitsPerPixel = 16;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_RGBA_8_8_8_8:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_RGBA_16_16_16_16:
        m_iBitsPerPixel = 64;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_RGBA_5_5_5_1:
        m_iBitsPerPixel = 16;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_RGBA_10_10_10_2:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_RGBA_FLOAT:
        m_iBitsPerPixel = 128;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = true;
        break;
    case IT_CMYK_8_8_8_8:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_CMYK_16_16_16_16:
        m_iBitsPerPixel = 64;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_CMYK_FLOAT:
        m_iBitsPerPixel = 128;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = false;
        break;
    case IT_CMYK_WITH_ALPHA_8_8_8_8_8:
        m_iBitsPerPixel = 40;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 5;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_CMYK_WITH_ALPHA_16_16_16_16_16:
        m_iBitsPerPixel = 80;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 5;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_CMYK_WITH_ALPHA_FLOAT:
        m_iBitsPerPixel = 160;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 5;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = true;
        break;
    case IT_LAB_8_8_8:
        m_iBitsPerPixel = 24;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_LAB_16_16_16:
        m_iBitsPerPixel = 48;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = false;
        break;
    case IT_LAB_FLOAT:
        m_iBitsPerPixel = 96;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 3;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = false;
        break;
    case IT_LAB_WITH_ALPHA_8_8_8_8:
        m_iBitsPerPixel = 32;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_LAB_WITH_ALPHA_16_16_16_16:
        m_iBitsPerPixel = 64;
        m_bHasGradIntSampleUniformity = true;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_UNSIGNED_INTEGER;
        m_bHasAlpha = true;
        break;
    case IT_LAB_WITH_ALPHA_FLOAT:
        m_iBitsPerPixel = 128;
        m_bHasGradIntSampleUniformity = false;
        m_iComponentsPerPixel = 4;
        m_ePixelComponentType = PCT_FLOATING_POINT;
        m_bHasAlpha = true;
        break;
    }
    m_iBytesPerPixel = m_iBitsPerPixel/8;
    if ( m_bHasGradIntSampleUniformity )
        m_iBitsPerIntegerSample = m_iBitsPerPixel/m_iComponentsPerPixel;

    // align the row-stride value
    m_iRowStride = iWidth*m_iBytesPerPixel;
    while ( m_iRowStride % ms_iRowAlignment != 0 )
        m_iRowStride++;

    // allocate pixels
    m_acImageContainer = new char[iHeight*m_iRowStride+16];
    unsigned long ulICAddress;
    MemCpy(&ulICAddress,&m_acImageContainer,sizeof(unsigned long));
    m_acImage = m_acImageContainer + (16 - ulICAddress % 16);
}
//------------------------------------------------------------------------------------------------------------------



