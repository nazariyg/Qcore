#ifndef QDRAWINGCANVAS_H
#define QDRAWINGCANVAS_H

// The class is for drawing of vector and raster graphics into memory.  Two modes of drawing are able to be
// used: 32-bit mode with 'unsigned int' as the type of a color unit, and 16-bit mode with 'unsigned short'.
// Only these two types are recognized as a template parameter which sets the fashion.  The 'unsigned int' type
// also serves as generalization when needed.  Also, there are two ways of functioning: with specialization
// for still images and for dynamic.  For the reason to minimize overheads related with transferring of pixels
// in the direction of display, the second type implies image's subdivision on tiles, so enabling for the user
// to transfer only those tiles which were touched by drawing of the recent frame.

#include "QRasterImage.h"
#include "QSystUtils.h"

namespace Q
{

template <class PixelT>
class Q_ITEM DrawingCanvas : public Heap
{
public:
    enum DrawingType
    {
        DT_ONE_SHOT,       // optimized for one-time drawing
        DT_MOVING_PICTURE  // optimized for animation
    };

    class LineState
    {
    public:
        LineState () {};  // uninitialized
        LineState (unsigned int uiColor)
            :
            Color(uiColor)
        {
        };

        unsigned int Color;
    };

    // construction and destruction
    DrawingCanvas (DrawingType eDrawing, int iWidth, int iHeight);
    ~DrawingCanvas ();

    // joining of color components and disjoining of a color unit
    static unsigned int JointColor (unsigned char ucR, unsigned char ucG, unsigned char ucB);
    static void DisjointColor (unsigned int uiColor, unsigned char& rucR, unsigned char& rucG, unsigned char& rucB);

    // dimensions
    int GetWidth () const;
    int GetHeight () const;
    int GetTileQuantityH () const;  // only for DT_MOVING_PICTURE
    int GetTileQuantityV () const;  // only for DT_MOVING_PICTURE

    // image access for DT_ONE_SHOT
    int GetCanvasByteSize () const;
    const PixelT* GetCanvas () const;

    // image access for DT_MOVING_PICTURE
    bool IsTileUpdated (int iPosH, int iPosV) const;
    int GetByteSizePerTile () const;
    const PixelT* GetTile (int iPosH, int iPosV) const;

    // Background access.  For checkboard pattern iSide must be a power of two.
    void SetBackgroundColor (unsigned int uiBackgroundColor);
    unsigned int GetBackgroundColor () const;
    void SetBackgroundCheckboardPattern (unsigned int uiColor0 = 0xcccccc, unsigned int uiColor1 = 0xffffff,
        int iSide = 8);
    void GetBackgroundCheckboardPattern (unsigned int& ruiColor0, unsigned int& ruiColor1, int& riSide) const;

    // To be called before actual drawing begins.  A background should be already set at the moment if it is to
    // differ from the default and parameter atBackgroundPixels is not used.  The atBackgroundPixels parameter,
    // if is set, must point to an array of pixel values with array dimensions equal to width and height of the
    // drawing canvas.  If atBackgroundPixels is not set, background settings (above) will be used to clear the
    // canvas.
    void ClearCanvas (const PixelT* atBackgroundPixels = 0);

    // line drawing states
    void SetLineState (const LineState& rqLineState);   // influences the top record of the stack
    const LineState& GetLineState () const;             // returns the top record
    void PushLineState (const LineState& rqLineState);
    void PopLineState ();

    // pen position attributes
    void MoveTo (float fX, float fY);
    float GetPenX () const;
    float GetPenY () const;

    // plots single point at current pen position using active line attributes
    void Pixel ();

    // drawing line
    void LineTo (float fX, float fY);

    // drawing quadratic Bezier curve
    void CurveTo (float fX1, float fY1, float fX2, float fY2);

    // drawing cubic Bezier curve
    void CubicCurveTo (float fX1, float fY1, float fX2, float fY2, float fX3, float fY3);

    // Image drawing.  Only images with IT_RGB_8_8_8 or IT_RGBA_8_8_8_8 types are accepted.  The ucAlpha
    // parameter has meaning only for IT_RGB_8_8_8.
    void DrawRasterImage (const RasterImagePtr& rspqImage, float fPosX, float fPosY, unsigned char ucAlpha = 255);

    // support for DT_MOVING_PICTURE; square tile side must be a power of two
    static const int TileSide;  // 64

private:
    enum BackgroundType
    {
        BT_COLOR,
        BT_CHECKBOARD_PATTERN
    };

    void OnAffectWholeCanvas ();
    static PixelT GetPixelValueFromColor (unsigned int uiColor);
    static unsigned int GetColorFromPixelValue (PixelT tValue);
    PixelT* GetPixelPtr (int iX, int iY) const;
    void Plot (int iX, int iY);

    DrawingType m_eDrawing;
    int m_iWidth, m_iHeight, m_iArea;
    int m_iTileQuantityH, m_iTileQuantityV, m_iByteSizePerTile, m_iTSAnd, m_iTSExp;
    PixelT* m_atCanvas;                     // image holder for DT_ONE_SHOT
    PixelT*** m_aaatTile;                   // image holder for DT_MOVING_PICTURE
    char** m_aacTileToUpdateNumber;
    BackgroundType m_eBackground;           // default: BT_COLOR
    unsigned int m_uiBackgroundColor;       // default: white
    unsigned int m_uiBCPColor0, m_uiBCPColor1;
    int m_iBCPSide;
    Array<LineState> m_qLineStateStack;     // default: initially one record in, color: black
    float m_fPenX, m_fPenY;                 // default: (0.0, 0.0)
    PixelT m_tPlottingPV;
};

typedef DrawingCanvas<unsigned short> DrawingCanvas16;
typedef DrawingCanvas<unsigned int> DrawingCanvas32;

typedef SmartPointer<DrawingCanvas16> DrawingCanvas16Ptr;
typedef SmartPointer<DrawingCanvas32> DrawingCanvas32Ptr;

}

#endif



