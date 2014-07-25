// This page contains rewritten NeuQuant algorithm of Anthony Dekker.  The copyright notice follows below.

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* NeuQuant Neural-Net Quantization Algorithm
 * ------------------------------------------
 *
 * Copyright (c) 1994 Anthony Dekker
 *
 * NEUQUANT Neural-Net quantization algorithm by Anthony Dekker, 1994.
 * See "Kohonen neural networks for optimal colour quantization"
 * in "Network: Computation in Neural Systems" Vol. 5 (1994) pp 351-367.
 * for a discussion of the algorithm.
 * See also  http://members.ozemail.com.au/~dekker/NEUQUANT.HTML
 *
 * Any party obtaining a copy of these files from the author, directly or
 * indirectly, is granted, free of charge, a full and unrestricted irrevocable,
 * world-wide, paid up, royalty-free, nonexclusive right and license to deal
 * in this software and documentation files (the "Software"), including without
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so, with the only requirement being
 * that this copyright notice remain intact.
 */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "QRasterImageQuantization.h"
#include "QProgressProbe.h"
#include "QSet.h"
#include "QSortUtils.h"
using namespace Q;

// support for exact palette retrieving
class UcColor
{
public:
    bool operator== (const UcColor& rqColor) const
    {
        return ( MemCmp(Components,rqColor.Components,3) == 0 );
    };
    bool operator< (const UcColor& rqColor) const
    {
        return ( MemCmp(Components,rqColor.Components,3) < 0 );
    };

    unsigned char Components[3];
};

// support for the Median-Cut quantization algorithm
inline int PackToIndex (unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
    return ((ucBlue & ~7) << 7) | ((ucGreen & ~7) << 2) | (ucRed >> 3);
}
inline unsigned char ExtractRed (int iIndex)
{
    return (iIndex & 31) << 3;
}
inline unsigned char ExtractGreen (int iIndex)
{
    return ((iIndex >> 5) & 255) << 3;
}
inline unsigned char ExtractBlue (int iIndex)
{
    return ((iIndex >> 10) & 255) << 3;
}
class MCCube
{
public:
    void Update (unsigned short* ausHistBuff)
    {
        MinR = 255;
        MaxR = 0;
        MinG = 255;
        MaxG = 0;
        MinB = 255;
        MaxB = 0;

        unsigned char ucR, ucG, ucB;
        int iIndex;
        for (int i = IndexLB; i <= IndexUB; i++)
        {
            iIndex = ausHistBuff[i];
            ucR = ExtractRed(iIndex);
            if ( ucR > MaxR )
                MaxR = ucR;
            if ( ucR < MinR )
                MinR = ucR;
            ucG = ExtractGreen(iIndex);
            if ( ucG > MaxG )
                MaxG = ucG;
            if ( ucG < MinG )
                MinG = ucG;
            ucB = ExtractBlue(iIndex);
            if ( ucB > MaxB )
                MaxB = ucB;
            if ( ucB < MinB )
                MinB = ucB;
        }
    };

    unsigned char MinR, MaxR, MinG, MaxG, MinB, MaxB;
    int IndexLB, IndexUB;
    unsigned int Population;
};
class CompareOnRed
{
public:
    bool operator() (int iIndex0, int iIndex1) const
    {
        return ( ExtractRed(iIndex0) < ExtractRed(iIndex1) ? true : false );
    };
};
class CompareOnGreen
{
public:
    bool operator() (int iIndex0, int iIndex1) const
    {
        return ( ExtractGreen(iIndex0) < ExtractGreen(iIndex1) ? true : false );
    };
};
class CompareOnBlue
{
public:
    bool operator() (int iIndex0, int iIndex1) const
    {
        return ( ExtractBlue(iIndex0) < ExtractBlue(iIndex1) ? true : false );
    };
};

// support for best matching palette color finding
class AnotherCellViewRecord
{
public:
    int Index, SqDist;
};
class ACVRComparator
{
public:
    bool operator() (const AnotherCellViewRecord& rqACVR0, const AnotherCellViewRecord& rqACVR1) const
    {
        return ( rqACVR0.SqDist < rqACVR1.SqDist );
    };
};

// support for black-and-white quantization
static const int gs_iBwPaletteSize = 2;
static const unsigned char gs_aaucBwPalette[gs_iBwPaletteSize][3] = {
    {  0,  0,  0}, {255,255,255}};

// support for primary-colors quantization
static const int gs_iPriPaletteSize = 8;
static const unsigned char gs_aaucPriPalette[gs_iPriPaletteSize][3] = {
    {255,255,255}, {255,255,  0}, {255,  0,255}, {255,  0,  0},
    {  0,255,255}, {  0,255,  0}, {  0,  0,255}, {  0,  0,  0}};

// support for Web-palette quantization
static const int gs_iWebPaletteSize = 216;
static const unsigned char gs_aaucWebPalette[gs_iWebPaletteSize][3] = {
    {255,255,255}, {255,255,204}, {255,255,153}, {255,255,102}, {255,255, 51}, {255,255,  0},
    {255,204,255}, {255,204,204}, {255,204,153}, {255,204,102}, {255,204, 51}, {255,204,  0},
    {255,153,255}, {255,153,204}, {255,153,153}, {255,153,102}, {255,153, 51}, {255,153,  0},
    {255,102,255}, {255,102,204}, {255,102,153}, {255,102,102}, {255,102, 51}, {255,102,  0},
    {255, 51,255}, {255, 51,204}, {255, 51,153}, {255, 51,102}, {255, 51, 51}, {255, 51,  0},
    {255,  0,255}, {255,  0,204}, {255,  0,153}, {255,  0,102}, {255,  0, 51}, {255,  0,  0},
    {204,255,255}, {204,255,204}, {204,255,153}, {204,255,102}, {204,255, 51}, {204,255,  0},
    {204,204,255}, {204,204,204}, {204,204,153}, {204,204,102}, {204,204, 51}, {204,204,  0},
    {204,153,255}, {204,153,204}, {204,153,153}, {204,153,102}, {204,153, 51}, {204,153,  0},
    {204,102,255}, {204,102,204}, {204,102,153}, {204,102,102}, {204,102, 51}, {204,102,  0},
    {204, 51,255}, {204, 51,204}, {204, 51,153}, {204, 51,102}, {204, 51, 51}, {204, 51,  0},
    {204,  0,255}, {204,  0,204}, {204,  0,153}, {204,  0,102}, {204,  0, 51}, {204,  0,  0},
    {153,255,255}, {153,255,204}, {153,255,153}, {153,255,102}, {153,255, 51}, {153,255,  0},
    {153,204,255}, {153,204,204}, {153,204,153}, {153,204,102}, {153,204, 51}, {153,204,  0},
    {153,153,255}, {153,153,204}, {153,153,153}, {153,153,102}, {153,153, 51}, {153,153,  0},
    {153,102,255}, {153,102,204}, {153,102,153}, {153,102,102}, {153,102, 51}, {153,102,  0},
    {153, 51,255}, {153, 51,204}, {153, 51,153}, {153, 51,102}, {153, 51, 51}, {153, 51,  0},
    {153,  0,255}, {153,  0,204}, {153,  0,153}, {153,  0,102}, {153,  0, 51}, {153,  0,  0},
    {102,255,255}, {102,255,204}, {102,255,153}, {102,255,102}, {102,255, 51}, {102,255,  0},
    {102,204,255}, {102,204,204}, {102,204,153}, {102,204,102}, {102,204, 51}, {102,204,  0},
    {102,153,255}, {102,153,204}, {102,153,153}, {102,153,102}, {102,153, 51}, {102,153,  0},
    {102,102,255}, {102,102,204}, {102,102,153}, {102,102,102}, {102,102, 51}, {102,102,  0},
    {102, 51,255}, {102, 51,204}, {102, 51,153}, {102, 51,102}, {102, 51, 51}, {102, 51,  0},
    {102,  0,255}, {102,  0,204}, {102,  0,153}, {102,  0,102}, {102,  0, 51}, {102,  0,  0},
    { 51,255,255}, { 51,255,204}, { 51,255,153}, { 51,255,102}, { 51,255, 51}, { 51,255,  0},
    { 51,204,255}, { 51,204,204}, { 51,204,153}, { 51,204,102}, { 51,204, 51}, { 51,204,  0},
    { 51,153,255}, { 51,153,204}, { 51,153,153}, { 51,153,102}, { 51,153, 51}, { 51,153,  0},
    { 51,102,255}, { 51,102,204}, { 51,102,153}, { 51,102,102}, { 51,102, 51}, { 51,102,  0},
    { 51, 51,255}, { 51, 51,204}, { 51, 51,153}, { 51, 51,102}, { 51, 51, 51}, { 51, 51,  0},
    { 51,  0,255}, { 51,  0,204}, { 51,  0,153}, { 51,  0,102}, { 51,  0, 51}, { 51,  0,  0},
    {  0,255,255}, {  0,255,204}, {  0,255,153}, {  0,255,102}, {  0,255, 51}, {  0,255,  0},
    {  0,204,255}, {  0,204,204}, {  0,204,153}, {  0,204,102}, {  0,204, 51}, {  0,204,  0},
    {  0,153,255}, {  0,153,204}, {  0,153,153}, {  0,153,102}, {  0,153, 51}, {  0,153,  0},
    {  0,102,255}, {  0,102,204}, {  0,102,153}, {  0,102,102}, {  0,102, 51}, {  0,102,  0},
    {  0, 51,255}, {  0, 51,204}, {  0, 51,153}, {  0, 51,102}, {  0, 51, 51}, {  0, 51,  0},
    {  0,  0,255}, {  0,  0,204}, {  0,  0,153}, {  0,  0,102}, {  0,  0, 51}, {  0,  0,  0}};

// support for Windows-palette quantization
static const int gs_iWinPaletteSize = 256;
static const unsigned char gs_aaucWinPalette[gs_iWinPaletteSize][3] = {
    {  0,  0,  0}, {128,  0,  0}, {  0,128,  0}, {128,128,  0}, {  0,  0,128}, {128,  0,128},
    {  0,128,128}, {128,128,128}, {192,220,192}, {166,202,240}, { 42, 63,170}, { 42, 63,255},
    { 42, 95,  0}, { 42, 95, 85}, { 42, 95,170}, { 42, 95,255}, { 42,127,  0}, { 42,127, 85},
    { 42,127,170}, { 42,127,255}, { 42,159,  0}, { 42,159, 85}, { 42,159,170}, { 42,159,255},
    { 42,191,  0}, { 42,191, 85}, { 42,191,170}, { 42,191,255}, { 42,223,  0}, { 42,223, 85},
    { 42,223,170}, { 42,223,255}, { 42,255,  0}, { 42,255, 85}, { 42,255,170}, { 42,255,255},
    { 85,  0,  0}, { 85,  0, 85}, { 85,  0,170}, { 85,  0,255}, { 85, 31,  0}, { 85, 31, 85},
    { 85, 31,170}, { 85, 31,255}, { 85, 63,  0}, { 85, 63, 85}, { 85, 63,170}, { 85, 63,255},
    { 85, 95,  0}, { 85, 95, 85}, { 85, 95,170}, { 85, 95,255}, { 85,127,  0}, { 85,127, 85},
    { 85,127,170}, { 85,127,255}, { 85,159,  0}, { 85,159, 85}, { 85,159,170}, { 85,159,255},
    { 85,191,  0}, { 85,191, 85}, { 85,191,170}, { 85,191,255}, { 85,223,  0}, { 85,223, 85},
    { 85,223,170}, { 85,223,255}, { 85,255,  0}, { 85,255, 85}, { 85,255,170}, { 85,255,255},
    {127,  0,  0}, {127,  0, 85}, {127,  0,170}, {127,  0,255}, {127, 31,  0}, {127, 31, 85},
    {127, 31,170}, {127, 31,255}, {127, 63,  0}, {127, 63, 85}, {127, 63,170}, {127, 63,255},
    {127, 95,  0}, {127, 95, 85}, {127, 95,170}, {127, 95,255}, {127,127,  0}, {127,127, 85},
    {127,127,170}, {127,127,255}, {127,159,  0}, {127,159, 85}, {127,159,170}, {127,159,255},
    {127,191,  0}, {127,191, 85}, {127,191,170}, {127,191,255}, {127,223,  0}, {127,223, 85},
    {127,223,170}, {127,223,255}, {127,255,  0}, {127,255, 85}, {127,255,170}, {127,255,255},
    {170,  0,  0}, {170,  0, 85}, {170,  0,170}, {170,  0,255}, {170, 31,  0}, {170, 31, 85},
    {170, 31,170}, {170, 31,255}, {170, 63,  0}, {170, 63, 85}, {170, 63,170}, {170, 63,255},
    {170, 95,  0}, {170, 95, 85}, {170, 95,170}, {170, 95,255}, {170,127,  0}, {170,127, 85},
    {170,127,170}, {170,127,255}, {170,159,  0}, {170,159, 85}, {170,159,170}, {170,159,255},
    {170,191,  0}, {170,191, 85}, {170,191,170}, {170,191,255}, {170,223,  0}, {170,223, 85},
    {170,223,170}, {170,223,255}, {170,255,  0}, {170,255, 85}, {170,255,170}, {170,255,255},
    {212,  0,  0}, {212,  0, 85}, {212,  0,170}, {212,  0,255}, {212, 31,  0}, {212, 31, 85},
    {212, 31,170}, {212, 31,255}, {212, 63,  0}, {212, 63, 85}, {212, 63,170}, {212, 63,255},
    {212, 95,  0}, {212, 95, 85}, {212, 95,170}, {212, 95,255}, {212,127,  0}, {212,127, 85},
    {212,127,170}, {212,127,255}, {212,159,  0}, {212,159, 85}, {212,159,170}, {212,159,255},
    {212,191,  0}, {212,191, 85}, {212,191,170}, {212,191,255}, {212,223,  0}, {212,223, 85},
    {212,223,170}, {212,223,255}, {212,255,  0}, {212,255, 85}, {212,255,170}, {212,255,255},
    {255,  0, 85}, {255,  0,170}, {255, 31,  0}, {255, 31, 85}, {255, 31,170}, {255, 31,255},
    {255, 63,  0}, {255, 63, 85}, {255, 63,170}, {255, 63,255}, {255, 95,  0}, {255, 95, 85},
    {255, 95,170}, {255, 95,255}, {255,127,  0}, {255,127, 85}, {255,127,170}, {255,127,255},
    {255,159,  0}, {255,159, 85}, {255,159,170}, {255,159,255}, {255,191,  0}, {255,191, 85},
    {255,191,170}, {255,191,255}, {255,223,  0}, {255,223, 85}, {255,223,170}, {255,223,255},
    {255,255, 85}, {255,255,170}, {204,204,255}, {255,204,255}, { 51,255,255}, {102,255,255},
    {153,255,255}, {204,255,255}, {  0,127,  0}, {  0,127, 85}, {  0,127,170}, {  0,127,255},
    {  0,159,  0}, {  0,159, 85}, {  0,159,170}, {  0,159,255}, {  0,191,  0}, {  0,191, 85},
    {  0,191,170}, {  0,191,255}, {  0,223,  0}, {  0,223, 85}, {  0,223,170}, {  0,223,255},
    {  0,255, 85}, {  0,255,170}, { 42,  0,  0}, { 42,  0, 85}, { 42,  0,170}, { 42,  0,255},
    { 42, 31,  0}, { 42, 31, 85}, { 42, 31,170}, { 42, 31,255}, { 42, 63,  0}, { 42, 63, 85},
    {255,251,240}, {160,160,164}, {128,128,128}, {255,  0,  0}, {  0,255,  0}, {255,255,  0},
    {  0,  0,255}, {255,  0,255}, {  0,255,255}, {255,255,255}};

// support for MacOS-palette quantization
static const int gs_iMacPaletteSize = 256;
static const unsigned char gs_aaucMacPalette[gs_iMacPaletteSize][3] = {
    {255,255,255}, {255,255,204}, {255,255,153}, {255,255,102}, {255,255, 51}, {255,255,  0},
    {255,204,255}, {255,204,204}, {255,204,153}, {255,204,102}, {255,204, 51}, {255,204,  0},
    {255,153,255}, {255,153,204}, {255,153,153}, {255,153,102}, {255,153, 51}, {255,153,  0},
    {255,102,255}, {255,102,204}, {255,102,153}, {255,102,102}, {255,102, 51}, {255,102,  0},
    {255, 51,255}, {255, 51,204}, {255, 51,153}, {255, 51,102}, {255, 51, 51}, {255, 51,  0},
    {255,  0,255}, {255,  0,204}, {255,  0,153}, {255,  0,102}, {255,  0, 51}, {255,  0,  0},
    {204,255,255}, {204,255,204}, {204,255,153}, {204,255,102}, {204,255, 51}, {204,255,  0},
    {204,204,255}, {204,204,204}, {204,204,153}, {204,204,102}, {204,204, 51}, {204,204,  0},
    {204,153,255}, {204,153,204}, {204,153,153}, {204,153,102}, {204,153, 51}, {204,153,  0},
    {204,102,255}, {204,102,204}, {204,102,153}, {204,102,102}, {204,102, 51}, {204,102,  0},
    {204, 51,255}, {204, 51,204}, {204, 51,153}, {204, 51,102}, {204, 51, 51}, {204, 51,  0},
    {204,  0,255}, {204,  0,204}, {204,  0,153}, {204,  0,102}, {204,  0, 51}, {204,  0,  0},
    {153,255,255}, {153,255,204}, {153,255,153}, {153,255,102}, {153,255, 51}, {153,255,  0},
    {153,204,255}, {153,204,204}, {153,204,153}, {153,204,102}, {153,204, 51}, {153,204,  0},
    {153,153,255}, {153,153,204}, {153,153,153}, {153,153,102}, {153,153, 51}, {153,153,  0},
    {153,102,255}, {153,102,204}, {153,102,153}, {153,102,102}, {153,102, 51}, {153,102,  0},
    {153, 51,255}, {153, 51,204}, {153, 51,153}, {153, 51,102}, {153, 51, 51}, {153, 51,  0},
    {153,  0,255}, {153,  0,204}, {153,  0,153}, {153,  0,102}, {153,  0, 51}, {153,  0,  0},
    {102,255,255}, {102,255,204}, {102,255,153}, {102,255,102}, {102,255, 51}, {102,255,  0},
    {102,204,255}, {102,204,204}, {102,204,153}, {102,204,102}, {102,204, 51}, {102,204,  0},
    {102,153,255}, {102,153,204}, {102,153,153}, {102,153,102}, {102,153, 51}, {102,153,  0},
    {102,102,255}, {102,102,204}, {102,102,153}, {102,102,102}, {102,102, 51}, {102,102,  0},
    {102, 51,255}, {102, 51,204}, {102, 51,153}, {102, 51,102}, {102, 51, 51}, {102, 51,  0},
    {102,  0,255}, {102,  0,204}, {102,  0,153}, {102,  0,102}, {102,  0, 51}, {102,  0,  0},
    { 51,255,255}, { 51,255,204}, { 51,255,153}, { 51,255,102}, { 51,255, 51}, { 51,255,  0},
    { 51,204,255}, { 51,204,204}, { 51,204,153}, { 51,204,102}, { 51,204, 51}, { 51,204,  0},
    { 51,153,255}, { 51,153,204}, { 51,153,153}, { 51,153,102}, { 51,153, 51}, { 51,153,  0},
    { 51,102,255}, { 51,102,204}, { 51,102,153}, { 51,102,102}, { 51,102, 51}, { 51,102,  0},
    { 51, 51,255}, { 51, 51,204}, { 51, 51,153}, { 51, 51,102}, { 51, 51, 51}, { 51, 51,  0},
    { 51,  0,255}, { 51,  0,204}, { 51,  0,153}, { 51,  0,102}, { 51,  0, 51}, { 51,  0,  0},
    {  0,255,255}, {  0,255,204}, {  0,255,153}, {  0,255,102}, {  0,255, 51}, {  0,255,  0},
    {  0,204,255}, {  0,204,204}, {  0,204,153}, {  0,204,102}, {  0,204, 51}, {  0,204,  0},
    {  0,153,255}, {  0,153,204}, {  0,153,153}, {  0,153,102}, {  0,153, 51}, {  0,153,  0},
    {  0,102,255}, {  0,102,204}, {  0,102,153}, {  0,102,102}, {  0,102, 51}, {  0,102,  0},
    {  0, 51,255}, {  0, 51,204}, {  0, 51,153}, {  0, 51,102}, {  0, 51, 51}, {  0, 51,  0},
    {  0,  0,255}, {  0,  0,204}, {  0,  0,153}, {  0,  0,102}, {  0,  0, 51}, {238,  0,  0},
    {221,  0,  0}, {187,  0,  0}, {170,  0,  0}, {136,  0,  0}, {119,  0,  0}, { 85,  0,  0},
    { 68,  0,  0}, { 34,  0,  0}, { 17,  0,  0}, {  0,238,  0}, {  0,221,  0}, {  0,187,  0},
    {  0,170,  0}, {  0,136,  0}, {  0,119,  0}, {  0, 85,  0}, {  0, 68,  0}, {  0, 34,  0},
    {  0, 17,  0}, {  0,  0,238}, {  0,  0,221}, {  0,  0,187}, {  0,  0,170}, {  0,  0,136},
    {  0,  0,119}, {  0,  0, 85}, {  0,  0, 68}, {  0,  0, 34}, {  0,  0, 17}, {238,238,238},
    {221,221,221}, {187,187,187}, {170,170,170}, {136,136,136}, {119,119,119}, { 85, 85, 85},
    { 68, 68, 68}, { 34, 34, 34}, { 17, 17, 17}, {  0,  0,  0}};

//------------------------------------------------------------------------------------------------------------------
bool RasterImageQuantization::TryToGetExactPalette (const RasterImagePtr& rspqImage,
    RasterImage::Palette& rqPalette)
{
    assert( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||  rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );

    // scan the image remembering colors used
    Set<UcColor> qUsedColors;
    UcColor qColor;
    unsigned char* pucRow = (unsigned char*)rspqImage->GetImage();
    unsigned char* aucPix = pucRow;
    for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
    {
        for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
        {
            MemCpy(qColor.Components,aucPix,3);
            qUsedColors.Insert(qColor);
            if ( qUsedColors.GetQuantity() > 256 )
            {
                // may not proceed
                goto Exit;
            }

            aucPix += rspqImage->GetBytesPerPixel();
        }

        pucRow += rspqImage->GetRowStride();
        aucPix = pucRow;
    }
Exit:

    if ( qUsedColors.GetQuantity() <= 256 )
    {
        // the exact palette can be generated
        rqPalette.SetQuantity(qUsedColors.GetQuantity());
        int iI = 0;
        for (Set<UcColor>::IteratorT qIt = qUsedColors.GetBegin(); qIt != qUsedColors.GetEnd(); ++qIt)
        {
            const UcColor& rqColor = *qIt;
            PaletteColor& rqCell = rqPalette[iI++];

            rqCell.R() = rqColor.Components[0];
            rqCell.G() = rqColor.Components[1];
            rqCell.B() = rqColor.Components[2];
        }

        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
RasterImageQuantization::RasterImageQuantization (const RasterImagePtr& rspqImage,
    AdaptiveQuantizationType eAdaptiveQuantization, int iPaletteSize, ForcingType eForcing,
    RasterImage::Palette* pqCustomForcingPalette)
    :
    m_spqImage(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_bAdaptive(true),
    m_eAdaptiveQuantization(eAdaptiveQuantization),
    m_iPaletteSize(iPaletteSize),
    m_eForcing(eForcing),
    m_eDithering(DT_NONE),
    m_bPreservePaletteColorForTransparency(false)
{
    assert( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||  rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );
    assert( 1 <= iPaletteSize && iPaletteSize <= 256 );
    assert( !(eForcing == FT_CUSTOM_PALETTE && !pqCustomForcingPalette) );
    assert( !(pqCustomForcingPalette
        &&  !(1 <= pqCustomForcingPalette->GetQuantity() && pqCustomForcingPalette->GetQuantity() <= 256)) );
    assert( !(eForcing == FT_BLACK_AND_WHITE && iPaletteSize < gs_iBwPaletteSize) );
    assert( !(eForcing == FT_PRIMARY_COLORS && iPaletteSize < gs_iPriPaletteSize) );
    assert( !(eForcing == FT_WEB_COLORS && iPaletteSize < gs_iWebPaletteSize) );
    assert( !(eForcing == FT_CUSTOM_PALETTE && iPaletteSize < pqCustomForcingPalette->GetQuantity()) );

    if ( pqCustomForcingPalette )
        m_qCustomPalette = *pqCustomForcingPalette;
}
//------------------------------------------------------------------------------------------------------------------
RasterImageQuantization::RasterImageQuantization (const RasterImagePtr& rspqImage,
    ProvidentialQuantizationType eProvidentialQuantization, RasterImage::Palette* pqCustomPalette)
    :
    m_spqImage(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_bAdaptive(false),
    m_eProvidentialQuantization(eProvidentialQuantization),
    m_eDithering(DT_NONE),
    m_bPreservePaletteColorForTransparency(false)
{
    assert( rspqImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||  rspqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );
    assert( !(eProvidentialQuantization == PQT_CUSTOM_PALETTE && !pqCustomPalette) );
    assert( !(pqCustomPalette && !(1 <= pqCustomPalette->GetQuantity() && pqCustomPalette->GetQuantity() <= 256)) );

    if ( pqCustomPalette )
        m_qCustomPalette = *pqCustomPalette;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageQuantization::GetQuantized ()
{
    // for a progress bar
    ProgressProbe qProgress(this);
    float fScopeCoefficient;

    RasterImage::Palette qQuantPalette;
    if ( m_bAdaptive )
    {
        // since some number of palette entries can be reserved accordingly to m_eForcing, get this number and
        // derive actually required palette size
        int iForcingAdvance;
        switch ( m_eForcing )
        {
        case FT_NONE:
            iForcingAdvance = 0;
            break;
        case FT_BLACK_AND_WHITE:
            iForcingAdvance = gs_iBwPaletteSize;
            break;
        case FT_PRIMARY_COLORS:
            iForcingAdvance = gs_iPriPaletteSize;
            break;
        case FT_WEB_COLORS:
            iForcingAdvance = gs_iWebPaletteSize;
            break;
        case FT_CUSTOM_PALETTE:
            iForcingAdvance = m_qCustomPalette.GetQuantity();
            break;
        }
        int iAdaptivePaletteSize = m_iPaletteSize - iForcingAdvance;
        if ( m_bPreservePaletteColorForTransparency )
            iAdaptivePaletteSize--;

        // if the next is wrong, then forcing-related or transparency-related settings (or both) are
        // incompatible with the target palette size
        assert( iAdaptivePaletteSize >= 0 );

        // perform adaptive image quantization
        if ( iAdaptivePaletteSize != 0 )
        {
            if ( m_eAdaptiveQuantization == AQT_MEDIAN_CUT )
                FindPaletteForImageUsingMedianCut(m_spqImage,iAdaptivePaletteSize,qQuantPalette,qProgress);
            else  // m_eAdaptiveQuantization = AQT_NEURAL_NET
                FindPaletteForImageUsingNeuralNet(m_spqImage,iAdaptivePaletteSize,qQuantPalette,qProgress);
        }

        if ( m_eForcing != FT_NONE )
        {
            // allocate appropriate space in the palette and fill it with the forced colors
            qQuantPalette.AdvanceQuantity(iForcingAdvance);
            if ( m_eForcing == FT_BLACK_AND_WHITE )
            {
                for (int i0 = 0, i1 = iAdaptivePaletteSize; i1 < qQuantPalette.GetQuantity(); i0++, i1++)
                {
                    PaletteColor& rqCell = qQuantPalette[i1];

                    rqCell.R() = gs_aaucBwPalette[i0][0];
                    rqCell.G() = gs_aaucBwPalette[i0][1];
                    rqCell.B() = gs_aaucBwPalette[i0][2];
                }
            }
            else if ( m_eForcing == FT_PRIMARY_COLORS )
            {
                for (int i0 = 0, i1 = iAdaptivePaletteSize; i1 < qQuantPalette.GetQuantity(); i0++, i1++)
                {
                    PaletteColor& rqCell = qQuantPalette[i1];

                    rqCell.R() = gs_aaucPriPalette[i0][0];
                    rqCell.G() = gs_aaucPriPalette[i0][1];
                    rqCell.B() = gs_aaucPriPalette[i0][2];
                }
            }
            else if ( m_eForcing == FT_WEB_COLORS )
            {
                for (int i0 = 0, i1 = iAdaptivePaletteSize; i1 < qQuantPalette.GetQuantity(); i0++, i1++)
                {
                    PaletteColor& rqCell = qQuantPalette[i1];

                    rqCell.R() = gs_aaucWebPalette[i0][0];
                    rqCell.G() = gs_aaucWebPalette[i0][1];
                    rqCell.B() = gs_aaucWebPalette[i0][2];
                }
            }
            else  // m_eForcing = FT_CUSTOM_PALETTE
            {
                for (int i0 = 0, i1 = iAdaptivePaletteSize; i1 < qQuantPalette.GetQuantity(); i0++, i1++)
                {
                    const PaletteColor& rqCell0 = m_qCustomPalette[i0];
                    PaletteColor& rqCell1 = qQuantPalette[i1];

                    rqCell1.R() = rqCell0.R();
                    rqCell1.G() = rqCell0.G();
                    rqCell1.B() = rqCell0.B();
                }
            }
        }

        fScopeCoefficient = 0.5f;
    }
    else  // providential quantization
    {
        if ( m_eProvidentialQuantization == PQT_BLACK_AND_WHITE )
        {
            qQuantPalette.SetQuantity(gs_iBwPaletteSize);
            for (int i = 0; i < gs_iBwPaletteSize; i++)
            {
                PaletteColor& rqCell = qQuantPalette[i];

                rqCell.R() = gs_aaucBwPalette[i][0];
                rqCell.G() = gs_aaucBwPalette[i][1];
                rqCell.B() = gs_aaucBwPalette[i][2];
            }
        }
        else if ( m_eProvidentialQuantization == PQT_WEB_PALETTE )
        {
            qQuantPalette.SetQuantity(gs_iWebPaletteSize);
            for (int i = 0; i < gs_iWebPaletteSize; i++)
            {
                PaletteColor& rqCell = qQuantPalette[i];

                rqCell.R() = gs_aaucWebPalette[i][0];
                rqCell.G() = gs_aaucWebPalette[i][1];
                rqCell.B() = gs_aaucWebPalette[i][2];
            }
        }
        else if ( m_eProvidentialQuantization == PQT_WIN_PALETTE )
        {
            qQuantPalette.SetQuantity(gs_iWinPaletteSize);
            for (int i = 0; i < gs_iWinPaletteSize; i++)
            {
                PaletteColor& rqCell = qQuantPalette[i];

                rqCell.R() = gs_aaucWinPalette[i][0];
                rqCell.G() = gs_aaucWinPalette[i][1];
                rqCell.B() = gs_aaucWinPalette[i][2];
            }
        }
        else if ( m_eProvidentialQuantization == PQT_MAC_PALETTE )
        {
            qQuantPalette.SetQuantity(gs_iMacPaletteSize);
            for (int i = 0; i < gs_iMacPaletteSize; i++)
            {
                PaletteColor& rqCell = qQuantPalette[i];

                rqCell.R() = gs_aaucMacPalette[i][0];
                rqCell.G() = gs_aaucMacPalette[i][1];
                rqCell.B() = gs_aaucMacPalette[i][2];
            }
        }
        else  // m_eProvidentialQuantization = PQT_CUSTOM_PALETTE
        {
            qQuantPalette = m_qCustomPalette;
        }

        fScopeCoefficient = 1.0f;
    }

    // find the best matching palette entry for each pixel in the image and form the paletted one
    RasterImagePtr spqQuantImage = GetQuantizedOnPalette(qQuantPalette,qProgress,fScopeCoefficient);

    if ( m_bPreservePaletteColorForTransparency )
    {
        // if the next is wrong, then forcing-related or transparency-related settings (or both) are
        // incompatible with the target palette size
        assert( spqQuantImage->GetPaletteSize() < 256 );

        // place into the palette the entry designating full transparency by using threshold 128
        for (int i = 0; i < spqQuantImage->GetPaletteSize(); i++)
            spqQuantImage->GetPaletteColor(i).A() = 255;
        int iTranspIndex = spqQuantImage->GetPaletteSize();
        spqQuantImage->GetPalette().AdvanceQuantity(1);
        spqQuantImage->GetPalette().GetLast() = PaletteColor(0,0,0,0);
        unsigned char* aucRow0 = (unsigned char*)m_spqImage->GetImage();
        unsigned char* aucPix0 = aucRow0;
        unsigned char* aucRow1 = (unsigned char*)spqQuantImage->GetImage();
        unsigned char* pucPix1 = aucRow1;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                if ( aucPix0[3] < 128 )
                    *pucPix1 = iTranspIndex;

                aucPix0 += 4;
                pucPix1++;
            }

            aucRow0 += m_spqImage->GetRowStride();
            aucPix0 = aucRow0;
            aucRow1 += spqQuantImage->GetRowStride();
            pucPix1 = aucRow1;
        }
    }

    if ( m_spqImage->HasVisibilityMask()
    &&   spqQuantImage->GetType() == RasterImage::IT_PALETTED )
    {
        // transmit the visibility mask
        spqQuantImage->SetVisibilityMask(m_spqImage->GetVisibilityMask());
    }

    return spqQuantImage;
}
//------------------------------------------------------------------------------------------------------------------
void RasterImageQuantization::FindPaletteForImageUsingMedianCut (const RasterImagePtr& rspqImage, int iPaletteSize,
    RasterImage::Palette& rqPalette, ProgressProbe& rqProgress)
{
    // The Median-Cut algorithm inherits from the classical Heckbert's approach: in three-dimensional color
    // space with degraded resolution all pixels leave their marks, the initial bounding box is formed, and the
    // recursive splitting starts division of a box at the median along the longest extension while number of
    // sub-boxes is less than number of entries in the target palette.  This implementation, however, uses some
    // improvements in order to increase resulting quantization quality.

    rqPalette.SetQuantity(iPaletteSize);

    StillArray<MCCube> qCubeArray(iPaletteSize);
    StillArray<unsigned int> qHistogram(32768);
    MemSet(qHistogram,0,131072);
    StillArray<unsigned short> qHistBuff(32768);

    // generate the color appearance histogram
    unsigned char* pucRow = (unsigned char*)rspqImage->GetImage();
    unsigned char* pucR = pucRow + 0;
    unsigned char* pucG = pucRow + 1;
    unsigned char* pucB = pucRow + 2;
    for (int iY = 0; iY < rspqImage->GetHeight(); iY++)
    {
        for (int iX = 0; iX < rspqImage->GetWidth(); iX++)
        {
            qHistogram[PackToIndex(*pucR,*pucG,*pucB)]++;

            pucR += rspqImage->GetBytesPerPixel();
            pucG += rspqImage->GetBytesPerPixel();
            pucB += rspqImage->GetBytesPerPixel();
        }

        pucRow += rspqImage->GetRowStride();
        pucR = pucRow + 0;
        pucG = pucRow + 1;
        pucB = pucRow + 2;
    }

    int iCubeQuantity = 0;
    MCCube qCube, qCube0, qCube1;

    // create the initial bounding box
    qCube.Population = 0;
    int iIndex = 0;
    for (int i = 0; i < 32768; i++)
    {
        if ( qHistogram[i] != 0 )
        {
            qHistBuff[iIndex++] = i;
            qCube.Population += qHistogram[i];
        }
    }
    qCube.IndexLB = 0;
    qCube.IndexUB = ( iIndex != 0 ? iIndex - 1 : 0 );
    qCube.Update(qHistBuff);
    qCubeArray[iCubeQuantity++] = qCube;

    // actual color reduction
    unsigned char ucExtR, ucExtG, ucExtB, ucMaxExt;
    int iMaxVolume, iVolume, iCutPosition, iM, iMedian;
    unsigned int uiPopulation, uiHalfPop;
    while ( iCubeQuantity < iPaletteSize )
    {
        // find next candidate-for-splitting cube from the list of existing cubes; it is the one with the
        // maximal volume among all
        iMaxVolume = 0;
        iCutPosition = -1;
        for (int i = 0; i < iCubeQuantity; i++)
        {
            const MCCube& rqCube = qCubeArray[i];

            iVolume =
                (rqCube.MaxR - rqCube.MinR)*
                (rqCube.MaxG - rqCube.MinG)*
                (rqCube.MaxB - rqCube.MinB);
            if ( iVolume > iMaxVolume )
            {
                iMaxVolume = iVolume;
                iCutPosition = i;
            }
        }
        if ( iCutPosition == -1 )
        {
            // no more cubes to split
            break;
        }

        // sort histogram records along cube's longest axis
        qCube = qCubeArray[iCutPosition];
        ucExtR = qCube.MaxR - qCube.MinR;
        ucExtG = qCube.MaxG - qCube.MinG;
        ucExtB = qCube.MaxB - qCube.MinB;
        ucMaxExt = Mathi::Max3(ucExtR,ucExtG,ucExtB);
        if ( ucExtR == ucMaxExt )
            SortUtils<unsigned short,CompareOnRed>::Sort(qCube.IndexUB-qCube.IndexLB+1,qHistBuff+qCube.IndexLB);
        else if ( ucExtG == ucMaxExt )
            SortUtils<unsigned short,CompareOnGreen>::Sort(qCube.IndexUB-qCube.IndexLB+1,qHistBuff+qCube.IndexLB);
        else  // ucExtB = ucMaxExt
            SortUtils<unsigned short,CompareOnBlue>::Sort(qCube.IndexUB-qCube.IndexLB+1,qHistBuff+qCube.IndexLB);

        // find position of "median"
        uiPopulation = 0;
        uiHalfPop = qCube.Population/2;
        for (iM = qCube.IndexLB; iM < qCube.IndexUB; iM++)
        {
            if ( uiPopulation >= uiHalfPop )
                break;

            iIndex = qHistBuff[iM];
            uiPopulation += qHistogram[iIndex];
        }
        iMedian = iM;

        // split the cube at the median
        qCube0 = qCube;
        qCube0.IndexUB = ( iMedian != 0 ? iMedian - 1 : 0 );
        qCube0.Population = uiPopulation;
        qCube0.Update(qHistBuff);
        qCubeArray[iCutPosition] = qCube0;
        qCube1 = qCube;
        qCube1.IndexLB = iMedian;
        qCube1.Population = qCube.Population - uiPopulation;
        qCube1.Update(qHistBuff);
        qCubeArray[iCubeQuantity++] = qCube1;

        rqProgress.SetDone(((float)iCubeQuantity)/iPaletteSize*0.5f);
    }

    // now it's possible to create the quantization palette
    unsigned int uiSumR, uiSumG, uiSumB;
    for (int i0 = 0; i0 < iCubeQuantity; i0++)
    {
        const MCCube& rqCube = qCubeArray[i0];
        PaletteColor& rqPaletteColor = rqPalette[i0];

        uiSumR = uiSumG = uiSumB = 0;
        for (int i1 = rqCube.IndexLB; i1 <= rqCube.IndexUB; i1++)
        {
            iIndex = qHistBuff[i1];
            uiPopulation = qHistogram[iIndex];

            uiSumR += ExtractRed(iIndex)*uiPopulation;
            uiSumG += ExtractGreen(iIndex)*uiPopulation;
            uiSumB += ExtractBlue(iIndex)*uiPopulation;
        }

        rqPaletteColor.R() = Mathf::RoundToInt(((float)uiSumR)/rqCube.Population);
        rqPaletteColor.G() = Mathf::RoundToInt(((float)uiSumG)/rqCube.Population);
        rqPaletteColor.B() = Mathf::RoundToInt(((float)uiSumB)/rqCube.Population);
    }
}
//------------------------------------------------------------------------------------------------------------------
void RasterImageQuantization::FindPaletteForImageUsingNeuralNet (const RasterImagePtr& rspqImage, int iPaletteSize,
    RasterImage::Palette& rqPalette, ProgressProbe& rqProgress)
{
    static const int s_iSamples = 4;  // the quality-speed slider, must be in [1, 30] (1: best quality)
    static const int s_iCycleQuantity = 1 << 8;  // number of learning cycles

    rqPalette.SetQuantity(iPaletteSize);

    // reduce colors using Neural-Net quantization algorithm
    int i, j, k, l, iB, iG, iR, iInitRad, iRadius, iRad, iAlpha, iStep, iDelta, iSamplePixels, iAlphaDec,
        iImageDataSize, iDist, iA, iBiasDist, iBetaFreq, iBestPos, iBestD, iBestBiasD, iLo, iHi, iRadPow2,
        iTmp, iMaxNetPos, iSumOfSteps, iSizeOfRow, iRow, iCol;
    int* piPixel;
    int* piBiasEl;
    int* piFreqEl;
    int* piRP;
    unsigned char* aucComp;
    static const int s_iPrime1 = 499;
    static const int s_iPrime2 = 491;
    static const int s_iPrime3 = 487;
    static const int s_iPrime4 = 503;
    iInitRad = ( iPaletteSize >= 8 ? iPaletteSize >> 3 : 1 );
    iMaxNetPos = iPaletteSize - 1;
    typedef int NNPixel[4];
    NNPixel* aqNetwork = new NNPixel[iPaletteSize];
    int* aiFreq = new int[iPaletteSize];
    int* aiBias = new int[iPaletteSize];
    int* aiRadPower = new int[iInitRad];
    for (i = 0; i < iPaletteSize; i++)
    {
        piPixel = aqNetwork[i];
        piPixel[0] = piPixel[1] = piPixel[2] = (i << 12)/iPaletteSize;
        aiFreq[i] = 65536/iPaletteSize;
        aiBias[i] = 0;
    }
    iSizeOfRow = rspqImage->GetWidth()*rspqImage->GetBytesPerPixel();
    iImageDataSize = rspqImage->GetHeight()*iSizeOfRow;
    iAlphaDec = 30 + (s_iSamples - 1)/3;
    aucComp = (unsigned char*)rspqImage->GetImage();
    iSamplePixels = iImageDataSize/(rspqImage->GetBytesPerPixel()*s_iSamples);
    iDelta = iSamplePixels/s_iCycleQuantity;
    iAlpha = 1024;
    iRadius = iInitRad << 6;
    iRad = iRadius >> 6;
    if ( iRad <= 1 )
        iRad = 0;
    for (i = 0; i < iRad; i++)
    {
        iRadPow2 = iRad*iRad;
        aiRadPower[i] = 256*(iRadPow2 - i*i)*iAlpha/iRadPow2;
    }
    if ( iImageDataSize % s_iPrime1 != 0 )
    {
        iStep = rspqImage->GetBytesPerPixel()*s_iPrime1;
    }
    else
    {
        if ( iImageDataSize % s_iPrime2 != 0 )
        {
            iStep = rspqImage->GetBytesPerPixel()*s_iPrime2;
        }
        else
        {
            if ( iImageDataSize % s_iPrime3 != 0 )
                iStep = rspqImage->GetBytesPerPixel()*s_iPrime3;
            else
                iStep = rspqImage->GetBytesPerPixel()*s_iPrime4;
        }
    }
    i = 0;
    iSumOfSteps = 0;
    while ( i < iSamplePixels )
    {
        if ( iSumOfSteps >= iImageDataSize )
            break;
        iR = aucComp[0] << 4;
        iG = aucComp[1] << 4;
        iB = aucComp[2] << 4;
        iBestD = 2147483647;
        iBestBiasD = iBestD;
        iBestPos = -1;
        j = iBestPos;
        piBiasEl = aiBias;
        piFreqEl = aiFreq;
        for (k = 0; k < iPaletteSize; k++)
        {
            piPixel = aqNetwork[k];
            iDist = piPixel[2] - iB;
            if ( iDist < 0 )
                iDist = -iDist;
            iA = piPixel[1] - iG;
            if ( iA < 0 )
                iA = -iA;
            iDist += iA;
            iA = piPixel[0] - iR;
            if ( iA < 0 )
                iA = -iA;
            iDist += iA;
            if ( iDist < iBestD )
            {
                iBestD = iDist;
                iBestPos = k;
            }
            iBiasDist = iDist - (*piBiasEl >> 12);
            if ( iBiasDist < iBestBiasD )
            {
                iBestBiasD = iBiasDist;
                j = k;
            }
            iBetaFreq = *piFreqEl >> 10;
            *piFreqEl++ -= iBetaFreq;
            *piBiasEl++ += iBetaFreq << 10;
        }
        aiFreq[iBestPos] += 64;
        aiBias[iBestPos] -= 65536;
        piPixel = aqNetwork[j];
        *piPixel -= (*piPixel - iR)*iAlpha/1024;
        piPixel++;
        *piPixel -= (*piPixel - iG)*iAlpha/1024;
        piPixel++;
        *piPixel -= (*piPixel - iB)*iAlpha/1024;
        if ( iRad != 0 )
        {
            iLo = j - iRad;
            if ( iLo < -1 )
                iLo = -1;
            iHi = j + iRad;
            if ( iHi > iPaletteSize )
                iHi = iPaletteSize;
            k = j + 1;
            l = j - 1;
            piRP = aiRadPower;
            while ( k < iHi || l > iLo )
            {
                iA = *++piRP;
                if ( k < iHi )
                {
                    piPixel = aqNetwork[k];
                    *piPixel -= (*piPixel - iR)*iA/262144;
                    piPixel++;
                    *piPixel -= (*piPixel - iG)*iA/262144;
                    piPixel++;
                    *piPixel -= (*piPixel - iB)*iA/262144;
                    k++;
                }
                if ( l > iLo )
                {
                    piPixel = aqNetwork[l];
                    *piPixel -= (*piPixel - iR)*iA/262144;
                    piPixel++;
                    *piPixel -= (*piPixel - iG)*iA/262144;
                    piPixel++;
                    *piPixel -= (*piPixel - iB)*iA/262144;
                    l--;
                }
            }
        }
        iSumOfSteps += iStep;
        if ( iSumOfSteps >= iImageDataSize )
            iSumOfSteps %= iImageDataSize;
        iRow = iSumOfSteps / iSizeOfRow;
        iCol = iSumOfSteps % iSizeOfRow;
        aucComp = (unsigned char*)rspqImage->GetImage() + iRow*rspqImage->GetRowStride() + iCol;
        i++;
        if ( iDelta != 0 && i % iDelta == 0 )
        {
            iAlpha -= iAlpha/iAlphaDec;
            iRadius -= iRadius/30;
            iRad = iRadius >> 6;
            if ( iRad <= 1 )
                iRad = 0;
            for (j = 0; j < iRad; j++)
            {
                iRadPow2 = iRad*iRad;
                aiRadPower[j] = 256*(iRadPow2 - j*j)*iAlpha/iRadPow2;
            }
        }

        rqProgress.SetDone(((float)i)/iSamplePixels*0.5f);
    }
    for (i = 0; i < iPaletteSize; i++)
    {
        for (j = 0; j < 3; j++)
        {
            iTmp = (aqNetwork[i][j] + 8) >> 4;
            if ( iTmp > 255 )
                iTmp = 255;
            aqNetwork[i][j] = iTmp;
        }
        aqNetwork[i][3] = i;
    }
    for (i = 0; i < iPaletteSize; i++)
    {
        rqPalette[i].R() = aqNetwork[i][0];
        rqPalette[i].G() = aqNetwork[i][1];
        rqPalette[i].B() = aqNetwork[i][2];
    }
    delete[] aqNetwork;
    delete[] aiFreq;
    delete[] aiBias;
    delete[] aiRadPower;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageQuantization::GetQuantizedOnPalette (const RasterImage::Palette& rqPalette,
    ProgressProbe& rqProgress, float fScopeCoefficient)
{
    // Finding of the best matching palette entry for each pixel.  Usage of the brute-force approach makes
    // unacceptable disproportion between quantization time and palette index selection time (about 1:60) and
    // rises overall image procession time too much.  Thus, the LUT-based technique is invented.
    static const int s_iMaxLookupsInNearests = 24;  // 24 is the optimal

    // allocate the output image
    RasterImagePtr spqQuantImage;
    if ( !m_bPreservePaletteColorForTransparency )
        spqQuantImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,rqPalette);
    else
        spqQuantImage = new RasterImage(RasterImage::IT_PALETTED_WITH_ALPHA,m_iWidth,m_iHeight,rqPalette);

    int iPalSize = rqPalette.GetQuantity();

    // this LUT will map 4-bit values to 8-bit ones in the "soft" way
    int ai4To8[16];
    float f255Div15 = ((float)255)/15;
    for (int i = 0; i < 16; i++)
        ai4To8[i] = Mathf::RoundToInt(f255Div15*i);

    // form the LUT based on 12-bit color values
    unsigned char auc444ColorToIndex[4096];
    int iR, iG, iB, iMinSqDist, iSqDist;
    for (int i0 = 0; i0 < 4096; i0++)
    {
        iR = ai4To8[(i0 >> 8) & 15];
        iG = ai4To8[(i0 >> 4) & 15];
        iB = ai4To8[(i0 >> 0) & 15];

        // for this shrunken-and-grown color find the palette entry index examining squared distances
        iMinSqDist = Mathi::MAX_INT;
        for (int i1 = 0; i1 < iPalSize; i1++)
        {
            const PaletteColor& rqCell = rqPalette[i1];

            iSqDist =
                (iR - rqCell.R())*(iR - rqCell.R()) +
                (iG - rqCell.G())*(iG - rqCell.G()) +
                (iB - rqCell.B())*(iB - rqCell.B());
            if ( iSqDist < iMinSqDist )
            {
                iMinSqDist = iSqDist;
                auc444ColorToIndex[i0] = i1;
            }
        }
    }

    // process each unique pair of palette entries making them remember each other and the squared distance
    // between
    StillArray<int> qAlreadyIn(iPalSize);
    for (int i = 0; i < iPalSize; i++)
        qAlreadyIn[i] = 0;
    int iPalSizeMO = iPalSize - 1;
    StillArray<StillArray<AnotherCellViewRecord> > qAnotherCellViewRecords(iPalSize);
    for (int i = 0; i < iPalSize; i++)
        qAnotherCellViewRecords[i].SetQuantity(iPalSizeMO);
    for (int i0 = 0; i0 < iPalSize; i0++)
    {
        const PaletteColor& rqCell0 = rqPalette[i0];

        for (int i1 = i0+1; i1 < iPalSize; i1++)
        {
            const PaletteColor& rqCell1 = rqPalette[i1];

            AnotherCellViewRecord& rqACVR0 = qAnotherCellViewRecords[i0][qAlreadyIn[i0]++];
            AnotherCellViewRecord& rqACVR1 = qAnotherCellViewRecords[i1][qAlreadyIn[i1]++];

            iSqDist =
                (rqCell0.R() - rqCell1.R())*(rqCell0.R() - rqCell1.R()) +
                (rqCell0.G() - rqCell1.G())*(rqCell0.G() - rqCell1.G()) +
                (rqCell0.B() - rqCell1.B())*(rqCell0.B() - rqCell1.B());

            rqACVR0.Index = i1;
            rqACVR0.SqDist = iSqDist;

            rqACVR1.Index = i0;
            rqACVR1.SqDist = iSqDist;
        }
    }
    for (int i = 0; i < iPalSize; i++)
    {
        // sort the list of records to make more close entries appear earlier
        SortUtils<AnotherCellViewRecord,ACVRComparator>::Sort(iPalSizeMO,qAnotherCellViewRecords[i]);
    }

    // some LUTs to optimize the final stage
    int aiLut0[256], aiLut1[256], aiLut2[256], aiLut3[511];
    for (int i = 0; i < 256; i++)
        aiLut0[i] = (i >> 4) << 8;
    for (int i = 0; i < 256; i++)
        aiLut1[i] = (i >> 4) << 4;
    for (int i = 0; i < 256; i++)
        aiLut2[i] = i >> 4;
    for (int i = 0; i < 511; i++)
        aiLut3[i] = Mathi::Sqr(i-255);

    // support for dithering
    float fAlpha, fBeta, fGamma, fSigma;
    int iWidthMO, iHeightMO, iBegin, iEnd, iDir, iXPD, iXMD;
    bool bXPDOk;
    ColorRgb* aqErr;
    ColorRgb* aqNextErr;
    ColorRgb* pqSave;
    ColorRgb qColor, qAltColor, qErrColor;
    float aafBayerMat[4][4], fBayer;
    float fNoise, fNoiseFactor;
    if ( m_eDithering == DT_DIFFUSION )
    {
        fAlpha = ((float)7)/16*m_fDitheringAmount;
        fBeta = ((float)3)/16*m_fDitheringAmount;
        fGamma = ((float)5)/16*m_fDitheringAmount;
        fSigma = ((float)1)/16*m_fDitheringAmount;
        iWidthMO = m_iWidth - 1;
        iHeightMO = m_iHeight - 1;
        aqErr = new ColorRgb[m_iWidth];
        aqNextErr = new ColorRgb[m_iWidth];
        MemSet(aqErr,0,m_iWidth*sizeof(ColorRgb));
        MemSet(aqNextErr,0,m_iWidth*sizeof(ColorRgb));
    }
    else if ( m_eDithering == DT_PATTERN )
    {
        float fInv15 = 1.0f/15;
        float fCoeff = m_fDitheringAmount/0.75f*0.1f*(1.0f + ((float)(256 - iPalSize))/256*8.0f);
        aafBayerMat[0][0] = (((float)15)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[0][1] = (((float) 7)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[0][2] = (((float)13)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[0][3] = (((float) 5)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[1][0] = (((float) 3)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[1][1] = (((float)11)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[1][2] = (((float) 1)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[1][3] = (((float) 9)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[2][0] = (((float)12)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[2][1] = (((float) 4)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[2][2] = (((float)14)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[2][3] = (((float) 6)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[3][0] = (((float) 0)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[3][1] = (((float) 8)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[3][2] = (((float) 2)*fInv15 - 0.5f)*fCoeff;
        aafBayerMat[3][3] = (((float)10)*fInv15 - 0.5f)*fCoeff;
    }
    else if ( m_eDithering == DT_NOISE )
    {
        // reset seed
        Mathf::SymmetricRandom(1);
        fNoiseFactor = m_fDitheringAmount*0.05f;
    }

    rqProgress.SetAdvance(1.0f/m_iHeight*fScopeCoefficient);

    // find the palette index for each pixel
    unsigned char* aucRow0 = (unsigned char*)m_spqImage->GetImage();
    unsigned char* aucPix0;
    unsigned char* aucRow1 = (unsigned char*)spqQuantImage->GetImage();
    unsigned char* pucPix1 = aucRow1;
    int iStartIndex, iIndex, iAdR, iAdG, iAdB;
    iBegin = 0;
    iEnd = m_iWidth;
    iDir = 1;
    float fInv255 = 1.0f/255;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        for (int iX = iBegin; iX != iEnd; iX += iDir)
        {
            aucPix0 = aucRow0 + iX*m_spqImage->GetBytesPerPixel();

            if ( m_eDithering == DT_NONE )
            {
                iR = aucPix0[0];
                iG = aucPix0[1];
                iB = aucPix0[2];
            }
            else  // dithering is set
            {
                for (int i = 0; i < 3; i++)
                    qColor[i] = fInv255*aucPix0[i];

                if ( m_eDithering == DT_DIFFUSION )
                {
                    qColor += aqErr[iX];
                    qAltColor = qColor;
                }
                else if ( m_eDithering == DT_PATTERN )
                {
                    fBayer = aafBayerMat[iY & 3][iX & 3];
                    for (int i = 0; i < 3; i++)
                        qAltColor[i] = qColor[i] + fBayer;
                }
                else  // m_eDithering = DT_NOISE
                {
                    fNoise = Mathf::SymmetricRandom()*fNoiseFactor;
                    for (int i = 0; i < 3; i++)
                        qAltColor[i] = qColor[i] + fNoise;
                }

                qAltColor.Clamp();
                iR = Mathf::RoundToInt(qAltColor.R()*255);
                iG = Mathf::RoundToInt(qAltColor.G()*255);
                iB = Mathf::RoundToInt(qAltColor.B()*255);
            }

            iAdR = 255 + iR;
            iAdG = 255 + iG;
            iAdB = 255 + iB;

            // find the fairly good matching palette entry to be the starting point
            iStartIndex = auc444ColorToIndex[aiLut0[iR] | aiLut1[iG] | aiLut2[iB]];
            *pucPix1 = iStartIndex;

            // perform contest
            iMinSqDist =
                aiLut3[iAdR-rqPalette[iStartIndex].R()] +
                aiLut3[iAdG-rqPalette[iStartIndex].G()] +
                aiLut3[iAdB-rqPalette[iStartIndex].B()];
            for (int i = 0; i < iPalSizeMO && i != s_iMaxLookupsInNearests; i++)
            {
                iIndex = qAnotherCellViewRecords[iStartIndex][i].Index;
                iSqDist =
                    aiLut3[iAdR-rqPalette[iIndex].R()] +
                    aiLut3[iAdG-rqPalette[iIndex].G()] +
                    aiLut3[iAdB-rqPalette[iIndex].B()];
                if ( iSqDist < iMinSqDist )
                {
                    iMinSqDist = iSqDist;
                    *pucPix1 = iIndex;
                }
            }

            if ( m_eDithering == DT_DIFFUSION )
            {
                // error distribution (diffusion) for Floyd-Steinberg dithering
                qErrColor = qColor - rqPalette[(int)*pucPix1].GetColorRgb();
                iXPD = iX + iDir;
                iXMD = iX - iDir;
                bXPDOk = ( 0 <= iXPD && iXPD < m_iWidth );
                if ( bXPDOk )
                    aqErr[iXPD] += qErrColor*fAlpha;
                if ( iY != iHeightMO )
                {
                    if ( 0 <= iXMD && iXMD < m_iWidth )
                        aqNextErr[iXMD] += qErrColor*fBeta;
                    aqNextErr[iX] += qErrColor*fGamma;
                    if ( bXPDOk )
                        aqNextErr[iXPD] += qErrColor*fSigma;
                }
            }

            pucPix1 += iDir;
        }

        aucRow0 += m_spqImage->GetRowStride();
        aucRow1 += spqQuantImage->GetRowStride();
        pucPix1 = aucRow1;

        if ( m_eDithering == DT_DIFFUSION )
        {
            // swap error buffers and invert scan direction
            pqSave = aqErr;
            aqErr = aqNextErr;
            aqNextErr = pqSave;
            MemSet(aqNextErr,0,m_iWidth*sizeof(ColorRgb));
            iDir = -iDir;
            if ( iDir == 1 )
            {
                iBegin = 0;
                iEnd = m_iWidth;
            }
            else  // iDir = -1
            {
                iBegin = iWidthMO;
                iEnd = -1;
                pucPix1 += iWidthMO;
            }
        }

        rqProgress.MakeAdvance();
    }

    if ( m_eDithering == DT_DIFFUSION )
    {
        delete[] aqErr;
        delete[] aqNextErr;
    }

    return spqQuantImage;
}
//------------------------------------------------------------------------------------------------------------------



