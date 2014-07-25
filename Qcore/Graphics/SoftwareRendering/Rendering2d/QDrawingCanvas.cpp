#include "QDrawingCanvas.h"
#include "QDiscretePlotting.h"
#include "QStorage.h"
using namespace Q;

// support for DT_MOVING_PICTURE; square tile side must be a power of two
template <class PixelT>
const int DrawingCanvas<PixelT>::TileSide(64);

//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
DrawingCanvas<PixelT>::DrawingCanvas (DrawingType eDrawing, int iWidth, int iHeight)
    :
    m_eDrawing(eDrawing),
    m_iWidth(iWidth),
    m_iHeight(iHeight),
    m_iArea(iWidth*iHeight),
    m_eBackground(BT_COLOR),
    m_uiBackgroundColor(0xffffff),
    m_fPenX(0.0f),
    m_fPenY(0.0f)
{
    assert( iWidth >= 0 && iHeight >= 0 );

    if ( eDrawing == DT_ONE_SHOT )
    {
        m_atCanvas = new PixelT[m_iArea];
    }
    else  // eDrawing = DT_MOVING_PICTURE
    {
        // Initialize the tiling system.
        assert( IsPowerOfTwo(TileSide) );
        m_iTSAnd = TileSide - 1;
        m_iTSExp = Storage::GetUBitQuantity(TileSide) - 1;

        m_iTileQuantityH = ( (iWidth & m_iTSAnd) != 0 ? (iWidth >> m_iTSExp) + 1 : iWidth >> m_iTSExp );
        m_iTileQuantityV = ( (iHeight & m_iTSAnd) != 0 ? (iHeight >> m_iTSExp) + 1 : iHeight >> m_iTSExp );

        int iTileArea = TileSide*TileSide;
        m_iByteSizePerTile = iTileArea*sizeof(PixelT);

        m_aaatTile = new PixelT**[m_iTileQuantityV];
        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
        {
            m_aaatTile[i0] = new PixelT*[m_iTileQuantityH];
            for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                m_aaatTile[i0][i1] = new PixelT[iTileArea];
        }

        m_aacTileToUpdateNumber = new char*[m_iTileQuantityV];
        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
            m_aacTileToUpdateNumber[i0] = new char[m_iTileQuantityH];
    }
    OnAffectWholeCanvas();

    // initial line attributes
    m_qLineStateStack.Push(LineState(0x000000));
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
DrawingCanvas<PixelT>::~DrawingCanvas ()
{
    if ( m_eDrawing == DT_ONE_SHOT )
    {
        delete[] m_atCanvas;
    }
    else  // m_eDrawing = DT_MOVING_PICTURE
    {
        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
        {
            for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                delete[] m_aaatTile[i0][i1];
            delete[] m_aaatTile[i0];
        }
        delete[] m_aaatTile;

        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
            delete[] m_aacTileToUpdateNumber[i0];
        delete[] m_aacTileToUpdateNumber;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
typename unsigned int DrawingCanvas<PixelT>::JointColor (unsigned char ucR, unsigned char ucG, unsigned char ucB)
{
    return (ucR << 16) | (ucG << 8) | ucB;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::DisjointColor (unsigned int uiColor, unsigned char& rucR, unsigned char& rucG,
    unsigned char& rucB)
{
    rucR = (uiColor >> 16) & 255;
    rucG = (uiColor >> 8) & 255;
    rucB = uiColor & 255;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetTileQuantityH () const
{
    assert( m_eDrawing == DT_MOVING_PICTURE );
    return m_iTileQuantityH;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetTileQuantityV () const
{
    assert( m_eDrawing == DT_MOVING_PICTURE );
    return m_iTileQuantityV;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetCanvasByteSize () const
{
    assert( m_eDrawing == DT_ONE_SHOT );
    return m_iArea*sizeof(PixelT);
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
const PixelT* DrawingCanvas<PixelT>::GetCanvas () const
{
    assert( m_eDrawing == DT_ONE_SHOT );
    return m_atCanvas;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
bool DrawingCanvas<PixelT>::IsTileUpdated (int iPosH, int iPosV) const
{
    assert( m_eDrawing == DT_MOVING_PICTURE );
    assert( 0 <= iPosH && iPosH < m_iTileQuantityH
        &&  0 <= iPosV && iPosV < m_iTileQuantityV );
    return ( m_aacTileToUpdateNumber[iPosV][iPosH] != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
int DrawingCanvas<PixelT>::GetByteSizePerTile () const
{
    assert( m_eDrawing == DT_MOVING_PICTURE );
    return m_iByteSizePerTile;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
const PixelT* DrawingCanvas<PixelT>::GetTile (int iPosH, int iPosV) const
{
    assert( m_eDrawing == DT_MOVING_PICTURE );
    assert( 0 <= iPosH && iPosH < m_iTileQuantityH
        &&  0 <= iPosV && iPosV < m_iTileQuantityV );
    return m_aaatTile[iPosV][iPosH];
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::SetBackgroundColor (unsigned int uiBackgroundColor)
{
    m_eBackground = BT_COLOR;
    m_uiBackgroundColor = uiBackgroundColor;

    OnAffectWholeCanvas();
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
typename unsigned int DrawingCanvas<PixelT>::GetBackgroundColor () const
{
    assert( m_eBackground == BT_COLOR );
    return m_uiBackgroundColor;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::SetBackgroundCheckboardPattern (unsigned int uiColor0, unsigned int uiColor1, int iSide)
{
    assert( iSide > 0 && IsPowerOfTwo(iSide) );

    m_eBackground = BT_CHECKBOARD_PATTERN;
    m_uiBCPColor0 = uiColor0;
    m_uiBCPColor1 = uiColor1;
    m_iBCPSide = iSide;

    OnAffectWholeCanvas();
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::GetBackgroundCheckboardPattern (unsigned int& ruiColor0, unsigned int& ruiColor1,
    int& riSide) const
{
    assert( m_eBackground == BT_CHECKBOARD_PATTERN );

    ruiColor0 = m_uiBCPColor0;
    ruiColor1 = m_uiBCPColor1;
    riSide = m_iBCPSide;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::ClearCanvas (const PixelT* atBackgroundPixels)
{
    // clear from the previous
    if ( m_eDrawing == DT_ONE_SHOT )
    {
        if ( atBackgroundPixels == 0 )
        {
            // using the background settings
            PixelT* ptPix = m_atCanvas;
            if ( m_eBackground == BT_COLOR )
            {
                PixelT tValue = GetPixelValueFromColor(m_uiBackgroundColor);
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                        *ptPix++ = tValue;
                }
            }
            else  // m_eBackground = BT_CHECKBOARD_PATTERN
            {
                PixelT tValue0 = GetPixelValueFromColor(m_uiBCPColor0);
                PixelT tValue1 = GetPixelValueFromColor(m_uiBCPColor1);
                int iAnd = 1 << (Storage::GetUBitQuantity(m_iBCPSide) - 1);
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                        *ptPix++ = ( ((iX & iAnd) ^ (iY & iAnd)) != 0 ? tValue0 : tValue1 );
                }
            }
        }
        else
        {
            // using the provided background image
            MemCpy(m_atCanvas,atBackgroundPixels,m_iArea*sizeof(PixelT));
        }
    }
    else  // m_eDrawing = DT_MOVING_PICTURE
    {
        PixelT* ptPix;
        if ( atBackgroundPixels == 0 )
        {
            // using the background settings
            if ( m_eBackground == BT_COLOR )
            {
                PixelT tValue = GetPixelValueFromColor(m_uiBackgroundColor);
                for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
                {
                    for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                    {
                        if ( m_aacTileToUpdateNumber[i0][i1] == 1 )
                        {
                            ptPix = m_aaatTile[i0][i1];
                            for (int iY = 0; iY < TileSide; iY++)
                            {
                                for (int iX = 0; iX < TileSide; iX++)
                                    *ptPix++ = tValue;
                            }
                        }
                    }
                }
            }
            else  // m_eBackground = BT_CHECKBOARD_PATTERN
            {
                PixelT tValue0 = GetPixelValueFromColor(m_uiBCPColor0);
                PixelT tValue1 = GetPixelValueFromColor(m_uiBCPColor1);
                int iAnd = 1 << (Storage::GetUBitQuantity(m_iBCPSide) - 1);
                int iCanX, iCanY;
                int iTileX, iTileY = 0;
                for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
                {
                    iTileX = 0;
                    for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                    {
                        if ( m_aacTileToUpdateNumber[i0][i1] == 1 )
                        {
                            ptPix = m_aaatTile[i0][i1];
                            for (int iY = 0; iY < TileSide; iY++)
                            {
                                iCanY = iTileY + iY;
                                for (int iX = 0; iX < TileSide; iX++)
                                {
                                    iCanX = iTileX + iX;
                                    *ptPix++ = ( ((iCanX & iAnd) ^ (iCanY & iAnd)) != 0 ? tValue0 : tValue1 );
                                }
                            }
                        }

                        iTileX += TileSide;
                    }

                    iTileY += TileSide;
                }
            }
        }
        else
        {
            // using the provided background image
            int iVar0, iVar1, iVar2;
            for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
            {
                iVar0 = i0*TileSide;
                for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                {
                    if ( m_aacTileToUpdateNumber[i0][i1] == 1 )
                    {
                        iVar2 = i1*TileSide;
                        ptPix = m_aaatTile[i0][i1];
                        for (int iY = 0; iY < TileSide; iY++)
                        {
                            iVar1 = (iVar0 + iY)*m_iWidth + iVar2;
                            for (int iX = 0; iX < TileSide; iX++)
                                *ptPix++ = atBackgroundPixels[iVar1++];
                        }
                    }
                }
            }
        }

        // deal with update numbers
        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
        {
            for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
            {
                if ( m_aacTileToUpdateNumber[i0][i1] == 1 )
                    m_aacTileToUpdateNumber[i0][i1] = 2;
                else if ( m_aacTileToUpdateNumber[i0][i1] == 2 )
                    m_aacTileToUpdateNumber[i0][i1] = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::SetLineState (const LineState& rqLineState)
{
    m_qLineStateStack.GetLast() = rqLineState;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
const typename DrawingCanvas<PixelT>::LineState& DrawingCanvas<PixelT>::GetLineState () const
{
    return m_qLineStateStack.GetLast();
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::PushLineState (const LineState& rqLineState)
{
    m_qLineStateStack.Push(rqLineState);
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::PopLineState ()
{
    m_qLineStateStack.Pop();
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::MoveTo (float fX, float fY)
{
    m_fPenX = fX;
    m_fPenY = fY;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
float DrawingCanvas<PixelT>::GetPenX () const
{
    return m_fPenX;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
float DrawingCanvas<PixelT>::GetPenY () const
{
    return m_fPenY;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::Pixel ()
{
    const LineState& rqLineState = GetLineState();
    m_tPlottingPV = GetPixelValueFromColor(rqLineState.Color);

    Plot(Mathf::RoundToInt(m_fPenX),Mathf::RoundToInt(m_fPenY));
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::LineTo (float fX, float fY)
{
    const LineState& rqLineState = GetLineState();
    m_tPlottingPV = GetPixelValueFromColor(rqLineState.Color);

    DiscretePlotting<DrawingCanvas>::PlotLine(m_fPenX,m_fPenY,fX,fY,this,&DrawingCanvas::Plot);

    m_fPenX = fX;
    m_fPenY = fY;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::CurveTo (float fX1, float fY1, float fX2, float fY2)
{
    const LineState& rqLineState = GetLineState();
    m_tPlottingPV = GetPixelValueFromColor(rqLineState.Color);

    DiscretePlotting<DrawingCanvas>::PlotQuadraticBezierCurve(m_fPenX,m_fPenY,fX1,fY1,fX2,fY2,this,
        &DrawingCanvas::Plot);

    m_fPenX = fX2;
    m_fPenY = fY2;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::CubicCurveTo (float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
{
    const LineState& rqLineState = GetLineState();
    m_tPlottingPV = GetPixelValueFromColor(rqLineState.Color);

    DiscretePlotting<DrawingCanvas>::PlotCubicBezierCurve(m_fPenX,m_fPenY,fX1,fY1,fX2,fY2,fX3,fY3,this,
        &DrawingCanvas::Plot);

    m_fPenX = fX3;
    m_fPenY = fY3;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::DrawRasterImage (const RasterImagePtr& rspqImage, float fPosX, float fPosY,
    unsigned char ucAlpha)
{
    assert( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||  rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );

    if ( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8 && ucAlpha == 0 )
        return;

    int iPosX = Mathf::RoundToInt(fPosX);
    int iPosY = Mathf::RoundToInt(fPosY);
    unsigned char* aucImage = (unsigned char*)rspqImage->GetImage();
    unsigned char* aucPix;
    int iYVar;
    PixelT* ptDstPix;
    unsigned char ucSrcR, ucSrcG, ucSrcB;
    unsigned char ucDstR, ucDstG, ucDstB;
    int iRevA;
    if ( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8 )
    {
        iRevA = 255 - ucAlpha;
        for (int iY = iPosY, iImgY = 0; iImgY < rspqImage->GetHeight(); iY++, iImgY++)
        {
            iYVar = iImgY*rspqImage->GetRowStride();
            for (int iX = iPosX, iImgX = 0, iSX = 0; iImgX < rspqImage->GetWidth(); iX++, iImgX++, iSX += 3)
            {
                if ( 0 <= iX && iX < m_iWidth
                &&   0 <= iY && iY < m_iHeight
                &&   !(rspqImage->HasVisibilityMask() && !rspqImage->IsPixelVisible(iImgX,iImgY)) )
                {
                    aucPix = aucImage + iYVar + iSX;
                    if ( ucAlpha == 255 )
                    {
                        *GetPixelPtr(iX,iY) = GetPixelValueFromColor(JointColor(aucPix[0],aucPix[1],aucPix[2]));
                    }
                    else
                    {
                        ptDstPix = GetPixelPtr(iX,iY);

                        // source components
                        ucSrcR = aucPix[0];
                        ucSrcG = aucPix[1];
                        ucSrcB = aucPix[2];

                        // destination components
                        DisjointColor(GetColorFromPixelValue(*ptDstPix),ucDstR,ucDstG,ucDstB);

                        // blend without floating-point arithmetics and roundings
                        ucDstR = (ucDstR*iRevA + ucSrcR*ucAlpha)/255;
                        ucDstG = (ucDstG*iRevA + ucSrcG*ucAlpha)/255;
                        ucDstB = (ucDstB*iRevA + ucSrcB*ucAlpha)/255;

                        *ptDstPix = GetPixelValueFromColor(JointColor(ucDstR,ucDstG,ucDstB));
                    }

                    if ( m_eDrawing == DT_MOVING_PICTURE )
                    {
                        // mark the tile as updated
                        m_aacTileToUpdateNumber[iY >> m_iTSExp][iX >> m_iTSExp] = 1;
                    }
                }
            }
        }
    }
    else  // rspqImage->GetType() = IT_RGBA_8_8_8_8
    {
        unsigned char ucSrcA;
        for (int iY = iPosY, iImgY = 0; iImgY < rspqImage->GetHeight(); iY++, iImgY++)
        {
            iYVar = iImgY*rspqImage->GetRowStride();
            for (int iX = iPosX, iImgX = 0, iSX = 0; iImgX < rspqImage->GetWidth(); iX++, iImgX++, iSX += 4)
            {
                if ( 0 <= iX && iX < m_iWidth
                &&   0 <= iY && iY < m_iHeight )
                {
                    aucPix = aucImage + iYVar + iSX;
                    if ( aucPix[3] != 0 )
                    {
                        if ( aucPix[3] == 255 )
                        {
                            *GetPixelPtr(iX,iY) = GetPixelValueFromColor(JointColor(aucPix[0],aucPix[1],aucPix[2]));
                        }
                        else  // process the alpha value
                        {
                            // source components
                            ucSrcR = aucPix[0];
                            ucSrcG = aucPix[1];
                            ucSrcB = aucPix[2];
                            ucSrcA = aucPix[3];

                            // destination components
                            ptDstPix = GetPixelPtr(iX,iY);
                            DisjointColor(GetColorFromPixelValue(*ptDstPix),ucDstR,ucDstG,ucDstB);

                            // blend without floating-point arithmetics and roundings
                            iRevA = 255 - ucSrcA;
                            ucDstR = (ucDstR*iRevA + ucSrcR*ucSrcA)/255;
                            ucDstG = (ucDstG*iRevA + ucSrcG*ucSrcA)/255;
                            ucDstB = (ucDstB*iRevA + ucSrcB*ucSrcA)/255;

                            *ptDstPix = GetPixelValueFromColor(JointColor(ucDstR,ucDstG,ucDstB));
                        }

                        if ( m_eDrawing == DT_MOVING_PICTURE )
                        {
                            // mark the tile as updated
                            m_aacTileToUpdateNumber[iY >> m_iTSExp][iX >> m_iTSExp] = 1;
                        }
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::OnAffectWholeCanvas ()
{
    if ( m_eDrawing == DT_MOVING_PICTURE )
    {
        // mark tiles as updated
        for (int i0 = 0; i0 < m_iTileQuantityV; i0++)
        {
            for (int i1 = 0; i1 < m_iTileQuantityH; i1++)
                m_aacTileToUpdateNumber[i0][i1] = 1;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <>
unsigned short DrawingCanvas<unsigned short>::GetPixelValueFromColor (unsigned int uiColor)
{
    unsigned char ucR, ucG, ucB;
    DisjointColor(uiColor,ucR,ucG,ucB);
    return ((ucR >> 3) << 11) | ((ucG >> 2) << 5) | (ucB >> 3);
}
//------------------------------------------------------------------------------------------------------------------
template <>
unsigned int DrawingCanvas<unsigned int>::GetPixelValueFromColor (unsigned int uiColor)
{
    return uiColor;
}
//------------------------------------------------------------------------------------------------------------------
template <>
unsigned int DrawingCanvas<unsigned short>::GetColorFromPixelValue (unsigned short usValue)
{
    unsigned char ucR = (usValue >> 11) << 3;
    unsigned char ucG = (usValue >> 5) << 2;
    unsigned char ucB = usValue << 3;
    return JointColor(ucR,ucG,ucB);
}
//------------------------------------------------------------------------------------------------------------------
template <>
unsigned int DrawingCanvas<unsigned int>::GetColorFromPixelValue (unsigned int uiValue)
{
    return uiValue;
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
PixelT* DrawingCanvas<PixelT>::GetPixelPtr (int iX, int iY) const
{
    return ( m_eDrawing == DT_ONE_SHOT ?
        m_atCanvas + iY*m_iWidth + iX :
        m_aaatTile[iY >> m_iTSExp][iX >> m_iTSExp] + ((iY & m_iTSAnd) << m_iTSExp) + (iX & m_iTSAnd) );
}
//------------------------------------------------------------------------------------------------------------------
template <class PixelT>
void DrawingCanvas<PixelT>::Plot (int iX, int iY)
{
    if ( 0 <= iX && iX < m_iWidth
    &&   0 <= iY && iY < m_iHeight )
    {
        *GetPixelPtr(iX,iY) = m_tPlottingPV;

        if ( m_eDrawing == DT_MOVING_PICTURE )
        {
            // mark the tile as updated
            m_aacTileToUpdateNumber[iY >> m_iTSExp][iX >> m_iTSExp] = 1;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// 16-bit mode
template
class DrawingCanvas<unsigned short>;

// 32-bit mode
template
class DrawingCanvas<unsigned int>;
//------------------------------------------------------------------------------------------------------------------



