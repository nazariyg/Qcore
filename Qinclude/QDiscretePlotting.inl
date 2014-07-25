//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotLine (int iX0, int iY0, int iX1, int iY1, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    int iX = iX0;
    int iY = iY0;
    int iDx = iX1 - iX0;
    int iDy = iY1 - iY0;
    int iSx = ( iDx > 0 ? 1 : ( iDx < 0 ? -1 : 0 ) );
    int iSy = ( iDy > 0 ? 1 : ( iDy < 0 ? -1 : 0 ) );
    if ( iDx < 0 )
        iDx = -iDx;
    if ( iDy < 0 )
        iDy = -iDy;
    int iAx = iDx << 1;
    int iAy = iDy << 1;
    int iDecX, iDecY;
    if ( iDy < iDx )
    {
        // single-step in x-direction
        iDecY = iAy - iDx;
        for (/**/; /**/; iX += iSx, iDecY += iAy)
        {
            (pqClass->*oPlot)(iX,iY);
            if ( iX == iX1 )
                break;
            if ( iDecY >= 0 )
            {
                iDecY -= iAx;
                iY += iSy;
            }
        }
    }
    else
    {
        // single-step in y-direction
        iDecX = iAx - iDy;
        for (/**/; /**/; iY += iSy, iDecX += iAx)
        {
            (pqClass->*oPlot)(iX,iY);
            if ( iY == iY1 )
                break;
            if ( iDecX >= 0 )
            {
                iDecX -= iAy;
                iX += iSx;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotCircle (int iCX, int iCY, int iRadius, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    assert( iRadius >= 0 );

    if ( iRadius < 9 && iRadius != 4 && iRadius != 6 )
    {
        // special cases for radius = 0, 1, 2, 3, 5, 7, and 8 grant much nicer results than with the general
        // algorithm
        if ( iRadius == 0 )
        {
            (pqClass->*oPlot)(iCX,iCY);
        }
        else if ( iRadius == 1 )
        {
            (pqClass->*oPlot)(iCX+1,iCY+0);
            (pqClass->*oPlot)(iCX+0,iCY-1);
            (pqClass->*oPlot)(iCX-1,iCY+0);
            (pqClass->*oPlot)(iCX+0,iCY+1);
        }
        else if ( iRadius == 2 )
        {
            (pqClass->*oPlot)(iCX+2,iCY+0);
            (pqClass->*oPlot)(iCX+2,iCY-1);
            (pqClass->*oPlot)(iCX+1,iCY-2);
            (pqClass->*oPlot)(iCX+0,iCY-2);
            (pqClass->*oPlot)(iCX-1,iCY-2);
            (pqClass->*oPlot)(iCX-2,iCY-1);
            (pqClass->*oPlot)(iCX-2,iCY+0);
            (pqClass->*oPlot)(iCX-2,iCY+1);
            (pqClass->*oPlot)(iCX-1,iCY+2);
            (pqClass->*oPlot)(iCX+0,iCY+2);
            (pqClass->*oPlot)(iCX+1,iCY+2);
            (pqClass->*oPlot)(iCX+2,iCY+1);
        }
        else if ( iRadius == 3 )
        {
            (pqClass->*oPlot)(iCX+3,iCY+0);
            (pqClass->*oPlot)(iCX+3,iCY-1);
            (pqClass->*oPlot)(iCX+2,iCY-2);
            (pqClass->*oPlot)(iCX+1,iCY-3);
            (pqClass->*oPlot)(iCX+0,iCY-3);
            (pqClass->*oPlot)(iCX-1,iCY-3);
            (pqClass->*oPlot)(iCX-2,iCY-2);
            (pqClass->*oPlot)(iCX-3,iCY-1);
            (pqClass->*oPlot)(iCX-3,iCY+0);
            (pqClass->*oPlot)(iCX-3,iCY+1);
            (pqClass->*oPlot)(iCX-2,iCY+2);
            (pqClass->*oPlot)(iCX-1,iCY+3);
            (pqClass->*oPlot)(iCX+0,iCY+3);
            (pqClass->*oPlot)(iCX+1,iCY+3);
            (pqClass->*oPlot)(iCX+2,iCY+2);
            (pqClass->*oPlot)(iCX+3,iCY+1);
        }
        else if ( iRadius == 5 )
        {
            (pqClass->*oPlot)(iCX+5,iCY+0);
            (pqClass->*oPlot)(iCX+5,iCY-1);
            (pqClass->*oPlot)(iCX+4,iCY-2);
            (pqClass->*oPlot)(iCX+4,iCY-3);
            (pqClass->*oPlot)(iCX+3,iCY-4);
            (pqClass->*oPlot)(iCX+2,iCY-4);
            (pqClass->*oPlot)(iCX+1,iCY-5);
            (pqClass->*oPlot)(iCX+0,iCY-5);
            (pqClass->*oPlot)(iCX-1,iCY-5);
            (pqClass->*oPlot)(iCX-2,iCY-4);
            (pqClass->*oPlot)(iCX-3,iCY-4);
            (pqClass->*oPlot)(iCX-4,iCY-3);
            (pqClass->*oPlot)(iCX-4,iCY-2);
            (pqClass->*oPlot)(iCX-5,iCY-1);
            (pqClass->*oPlot)(iCX-5,iCY+0);
            (pqClass->*oPlot)(iCX-5,iCY+1);
            (pqClass->*oPlot)(iCX-4,iCY+2);
            (pqClass->*oPlot)(iCX-4,iCY+3);
            (pqClass->*oPlot)(iCX-3,iCY+4);
            (pqClass->*oPlot)(iCX-2,iCY+4);
            (pqClass->*oPlot)(iCX-1,iCY+5);
            (pqClass->*oPlot)(iCX+0,iCY+5);
            (pqClass->*oPlot)(iCX+1,iCY+5);
            (pqClass->*oPlot)(iCX+2,iCY+4);
            (pqClass->*oPlot)(iCX+3,iCY+4);
            (pqClass->*oPlot)(iCX+4,iCY+3);
            (pqClass->*oPlot)(iCX+4,iCY+2);
            (pqClass->*oPlot)(iCX+5,iCY+1);
        }
        else if ( iRadius == 7 )
        {
            (pqClass->*oPlot)(iCX+7,iCY+0);
            (pqClass->*oPlot)(iCX+7,iCY-1);
            (pqClass->*oPlot)(iCX+7,iCY-2);
            (pqClass->*oPlot)(iCX+6,iCY-3);
            (pqClass->*oPlot)(iCX+6,iCY-4);
            (pqClass->*oPlot)(iCX+5,iCY-5);
            (pqClass->*oPlot)(iCX+4,iCY-6);
            (pqClass->*oPlot)(iCX+3,iCY-6);
            (pqClass->*oPlot)(iCX+2,iCY-7);
            (pqClass->*oPlot)(iCX+1,iCY-7);
            (pqClass->*oPlot)(iCX+0,iCY-7);
            (pqClass->*oPlot)(iCX-1,iCY-7);
            (pqClass->*oPlot)(iCX-2,iCY-7);
            (pqClass->*oPlot)(iCX-3,iCY-6);
            (pqClass->*oPlot)(iCX-4,iCY-6);
            (pqClass->*oPlot)(iCX-5,iCY-5);
            (pqClass->*oPlot)(iCX-6,iCY-4);
            (pqClass->*oPlot)(iCX-6,iCY-3);
            (pqClass->*oPlot)(iCX-7,iCY-2);
            (pqClass->*oPlot)(iCX-7,iCY-1);
            (pqClass->*oPlot)(iCX-7,iCY+0);
            (pqClass->*oPlot)(iCX-7,iCY+1);
            (pqClass->*oPlot)(iCX-7,iCY+2);
            (pqClass->*oPlot)(iCX-6,iCY+3);
            (pqClass->*oPlot)(iCX-6,iCY+4);
            (pqClass->*oPlot)(iCX-5,iCY+5);
            (pqClass->*oPlot)(iCX-4,iCY+6);
            (pqClass->*oPlot)(iCX-3,iCY+6);
            (pqClass->*oPlot)(iCX-2,iCY+7);
            (pqClass->*oPlot)(iCX-1,iCY+7);
            (pqClass->*oPlot)(iCX+0,iCY+7);
            (pqClass->*oPlot)(iCX+1,iCY+7);
            (pqClass->*oPlot)(iCX+2,iCY+7);
            (pqClass->*oPlot)(iCX+3,iCY+6);
            (pqClass->*oPlot)(iCX+4,iCY+6);
            (pqClass->*oPlot)(iCX+5,iCY+5);
            (pqClass->*oPlot)(iCX+6,iCY+4);
            (pqClass->*oPlot)(iCX+6,iCY+3);
            (pqClass->*oPlot)(iCX+7,iCY+2);
            (pqClass->*oPlot)(iCX+7,iCY+1);
        }
        else  // iRadius = 8
        {
            (pqClass->*oPlot)(iCX+8,iCY+0);
            (pqClass->*oPlot)(iCX+8,iCY-1);
            (pqClass->*oPlot)(iCX+8,iCY-2);
            (pqClass->*oPlot)(iCX+7,iCY-3);
            (pqClass->*oPlot)(iCX+7,iCY-4);
            (pqClass->*oPlot)(iCX+6,iCY-5);
            (pqClass->*oPlot)(iCX+5,iCY-6);
            (pqClass->*oPlot)(iCX+4,iCY-7);
            (pqClass->*oPlot)(iCX+3,iCY-7);
            (pqClass->*oPlot)(iCX+2,iCY-8);
            (pqClass->*oPlot)(iCX+1,iCY-8);
            (pqClass->*oPlot)(iCX+0,iCY-8);
            (pqClass->*oPlot)(iCX-1,iCY-8);
            (pqClass->*oPlot)(iCX-2,iCY-8);
            (pqClass->*oPlot)(iCX-3,iCY-7);
            (pqClass->*oPlot)(iCX-4,iCY-7);
            (pqClass->*oPlot)(iCX-6,iCY-5);
            (pqClass->*oPlot)(iCX-5,iCY-6);
            (pqClass->*oPlot)(iCX-7,iCY-4);
            (pqClass->*oPlot)(iCX-7,iCY-3);
            (pqClass->*oPlot)(iCX-8,iCY-2);
            (pqClass->*oPlot)(iCX-8,iCY-1);
            (pqClass->*oPlot)(iCX-8,iCY+0);
            (pqClass->*oPlot)(iCX-8,iCY+1);
            (pqClass->*oPlot)(iCX-8,iCY+2);
            (pqClass->*oPlot)(iCX-7,iCY+3);
            (pqClass->*oPlot)(iCX-7,iCY+4);
            (pqClass->*oPlot)(iCX-6,iCY+5);
            (pqClass->*oPlot)(iCX-5,iCY+6);
            (pqClass->*oPlot)(iCX-4,iCY+7);
            (pqClass->*oPlot)(iCX-3,iCY+7);
            (pqClass->*oPlot)(iCX-2,iCY+8);
            (pqClass->*oPlot)(iCX-1,iCY+8);
            (pqClass->*oPlot)(iCX+0,iCY+8);
            (pqClass->*oPlot)(iCX+1,iCY+8);
            (pqClass->*oPlot)(iCX+2,iCY+8);
            (pqClass->*oPlot)(iCX+3,iCY+7);
            (pqClass->*oPlot)(iCX+4,iCY+7);
            (pqClass->*oPlot)(iCX+6,iCY+5);
            (pqClass->*oPlot)(iCX+5,iCY+6);
            (pqClass->*oPlot)(iCX+7,iCY+4);
            (pqClass->*oPlot)(iCX+7,iCY+3);
            (pqClass->*oPlot)(iCX+8,iCY+2);
            (pqClass->*oPlot)(iCX+8,iCY+1);
        }
        return;
    }

    // four extreme points on the axes
    (pqClass->*oPlot)(iCX+iRadius,iCY);
    (pqClass->*oPlot)(iCX,iCY-iRadius);
    (pqClass->*oPlot)(iCX-iRadius,iCY);
    (pqClass->*oPlot)(iCX,iCY+iRadius);

    // eight octants
    int iX, iY, iDec, iVar0, iVar1, iVar2, iVar3, iVar4, iVar5, iVar6, iVar7;
    for (iX = 1, iY = iRadius, iDec = 3-(iRadius << 1); iX < iY; iX++)
    {
        iVar0 = iCX + iX;
        iVar1 = iCX - iX;
        iVar2 = iCX + iY;
        iVar3 = iCX - iY;
        iVar4 = iCY + iY;
        iVar5 = iCY - iY;
        iVar6 = iCY + iX;
        iVar7 = iCY - iX;
        (pqClass->*oPlot)(iVar0,iVar4);
        (pqClass->*oPlot)(iVar0,iVar5);
        (pqClass->*oPlot)(iVar1,iVar4);
        (pqClass->*oPlot)(iVar1,iVar5);
        (pqClass->*oPlot)(iVar2,iVar6);
        (pqClass->*oPlot)(iVar2,iVar7);
        (pqClass->*oPlot)(iVar3,iVar6);
        (pqClass->*oPlot)(iVar3,iVar7);

        if ( iDec >= 0 )
        {
            iDec += -(iY << 2) + 4;
            iY--;
        }
        iDec += (iX << 2) + 6;
    }

    if ( iX == iY )
    {
        // four points connecting same-quarter octants
        (pqClass->*oPlot)(iCX+iX,iCY+iX);
        (pqClass->*oPlot)(iCX+iX,iCY-iX);
        (pqClass->*oPlot)(iCX-iX,iCY+iX);
        (pqClass->*oPlot)(iCX-iX,iCY-iX);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotRectangle (int iLeft, int iTop, int iRight, int iBottom, int iRadius,
    ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    assert( iLeft <= iRight && iTop <= iBottom && iRadius >= 0 );

    int iEx = iRight - iLeft;
    int iEy = iBottom - iTop;
    int iHalfMinE = Mathi::Min(iEx,iEy) >> 1;
    if ( iRadius > iHalfMinE )
        iRadius = iHalfMinE;
    int iX, iY;
    int iDegX = iRight - iRadius;
    int iDegY = iBottom - iRadius;
    for (iX = iLeft+iRadius; iX <= iDegX; iX++)
    {
        (pqClass->*oPlot)(iX,iTop);
        (pqClass->*oPlot)(iX,iBottom);
    }
    for (iY = iTop+iRadius; iY <= iDegY; iY++)
    {
        (pqClass->*oPlot)(iLeft,iY);
        (pqClass->*oPlot)(iRight,iY);
    }

    if ( iRadius != 0 )
    {
        int iC0X = iRight - iRadius;
        int iC0Y = iBottom - iRadius;
        int iC1X = iC0X;
        int iC1Y = iTop + iRadius;
        int iC2X = iLeft + iRadius;
        int iC2Y = iC1Y;
        int iC3X = iC2X;
        int iC3Y = iC0Y;
        int iDec;
        for (iX = 1, iY = iRadius, iDec = 3-(iRadius << 1); iX <= iY; iX++)
        {
            (pqClass->*oPlot)(iC0X+iX,iC0Y+iY);
            (pqClass->*oPlot)(iC0X+iY,iC0Y+iX);
            (pqClass->*oPlot)(iC1X+iX,iC1Y-iY);
            (pqClass->*oPlot)(iC1X+iY,iC1Y-iX);
            (pqClass->*oPlot)(iC2X-iX,iC2Y-iY);
            (pqClass->*oPlot)(iC2X-iY,iC2Y-iX);
            (pqClass->*oPlot)(iC3X-iX,iC3Y+iY);
            (pqClass->*oPlot)(iC3X-iY,iC3Y+iX);
            if ( iDec >= 0 )
            {
                iDec += -(iY << 2) + 4;
                iY--;
            }
            iDec += (iX << 2) + 6;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotCircularArc (int iX0, int iY0, int iX1, int iY1, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    int iDiffX = iX1 - iX0;
    int iDiffY = iY1 - iY0;
    if ( iDiffX == 0 && iDiffY == 0 )
    {
        (pqClass->*oPlot)(iX0,iY0);
        return;
    }
    int iSqrD = iDiffX*iDiffX + iDiffY*iDiffY;

    int iPX, iPY, iDX0, iDY0, iDX1, iDY1, iC, iVC, iCC, iMinE, iSE, iE, iVE, iCE, iOX, iOY, iX, iY,
        iPrevOX, iPrevOY;

    // plot the initial point and choose the next one
    (pqClass->*oPlot)(iX0,iY0);
    iMinE = Mathi::MAX_INT;
    for (int i0 = -1; i0 <= 1; i0++)
    {
        for (int i1 = -1; i1 <= 1; i1++)
        {
            if ( i0 != 0 || i1 != 0 )
            {
                iPX = iX0 + i0;
                iPY = iY0 + i1;
                iDX0 = iPX - iX0;
                iDY0 = iPY - iY0;
                iC = iDX0*iDiffY - iDY0*iDiffX;
                if ( iC >= 0 )
                {
                    iDX1 = iPX - iX1;
                    iDY1 = iPY - iY1;
                    iSE = iSqrD - iDX0*iDX0 - iDY0*iDY0 - iDX1*iDX1 - iDY1*iDY1;
                    iE = iSE;
                    if ( iE < 0 )
                        iE = -iE;
                    if ( iE < iMinE )
                    {
                        iMinE = iE;
                        iVC = iC;
                        iVE = iSE;
                        iOX = i0;
                        iOY = i1;
                    }
                }
            }
        }
    }
    iX = iX0 + iOX;
    iY = iY0 + iOY;
    iPrevOX = -iOX;
    iPrevOY = -iOY;

    // arc to the endpoint choosing from four possible directions for each move
    int iCVar0 = -iDiffX;
    int iCVar1 = iDiffY - iDiffX;
    int iCVar2 = iDiffY;
    int iCVar3 = iDiffX + iDiffY;
    int iCVar4 = iDiffX;
    int iCVar5 = -iCVar1;
    int iCVar6 = -iDiffY;
    int iCVar7 = -iCVar3;
    int iSumX = iX0 + iX1;
    int iSumY = iY0 + iY1;
    int iEVar0 = (iSumY - 1) << 1;
    int iEVar1 = (iSumX + iSumY) << 1;
    int iEVar2 = (iSumX - 1) << 1;
    int iEVar3 = (iSumX - iSumY) << 1;
    int iEVar4 = (iSumY + 1) << 1;
    int iEVar5 = (iSumX + 1) << 1;
    for (/**/; /**/; /**/)
    {
        (pqClass->*oPlot)(iX,iY);
        if ( iX == iX1 && iY == iY1 )
            break;

        iMinE = Mathi::MAX_INT;
        iCC = iVC;
        iCE = iVE;
        if ( iPrevOX == -1 && iPrevOY == 0 )
        {
            PROC_ARC_MOVE_0;
            PROC_ARC_MOVE_1;
            PROC_ARC_MOVE_2;
            PROC_ARC_MOVE_3;
        }
        else if ( iPrevOX == -1 && iPrevOY == 1 )
        {
            PROC_ARC_MOVE_1;
            PROC_ARC_MOVE_2;
            PROC_ARC_MOVE_3;
            PROC_ARC_MOVE_4;
        }
        else if ( iPrevOX == 0 && iPrevOY == 1 )
        {
            PROC_ARC_MOVE_2;
            PROC_ARC_MOVE_3;
            PROC_ARC_MOVE_4;
            PROC_ARC_MOVE_5;
        }
        else if ( iPrevOX == 1 && iPrevOY == 1 )
        {
            PROC_ARC_MOVE_3;
            PROC_ARC_MOVE_4;
            PROC_ARC_MOVE_5;
            PROC_ARC_MOVE_6;
        }
        else if ( iPrevOX == 1 && iPrevOY == 0 )
        {
            PROC_ARC_MOVE_4;
            PROC_ARC_MOVE_5;
            PROC_ARC_MOVE_6;
            PROC_ARC_MOVE_7;
        }
        else if ( iPrevOX == 1 && iPrevOY == -1 )
        {
            PROC_ARC_MOVE_5;
            PROC_ARC_MOVE_6;
            PROC_ARC_MOVE_7;
            PROC_ARC_MOVE_0;
        }
        else if ( iPrevOX == 0 && iPrevOY == -1 )
        {
            PROC_ARC_MOVE_6;
            PROC_ARC_MOVE_7;
            PROC_ARC_MOVE_0;
            PROC_ARC_MOVE_1;
        }
        else  // iPrevOX = -1 and iPrevOY = -1
        {
            PROC_ARC_MOVE_7;
            PROC_ARC_MOVE_0;
            PROC_ARC_MOVE_1;
            PROC_ARC_MOVE_2;
        }
        iX += iOX;
        iY += iOY;
        iPrevOX = -iOX;
        iPrevOY = -iOY;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotEllipse (int iLeft, int iTop, int iRight, int iBottom, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    assert( iLeft <= iRight && iTop <= iBottom );

    int iEX = iRight - iLeft;
    int iEY = iBottom - iTop;
    if ( iEX == 0 && iEY == 0 )
    {
        (pqClass->*oPlot)(iLeft,iTop);
        return;
    }
    if ( iEX == iEY && (iEX & 1) == 0 )
    {
        int iRadius = iEX >> 1;
        PlotCircle(iLeft+iRadius,iTop+iRadius,iRadius,pqClass,oPlot);
        return;
    }
    double dCX = (iLeft + iRight)*0.5;
    double dCY = (iTop + iBottom)*0.5;
    double dA = iEX*0.5;
    double dB = iEY*0.5;
    double dSqrA = dA*dA;
    double dSqrB = dB*dB;
    double dDiv0 = dB/dA;
    double dDiv1 = dA/dB;
    double dX, dY;
    int iHalfEX = Mathd::RoundToInt(dA);
    int iHalfEY = Mathd::RoundToInt(dB);
    int iPX0, iPY0, iPX1, iPY1;
    for (int i = 0; i <= iHalfEX; i++)
    {
        dX = dA - i;
        dY = Mathd::Sqrt(dSqrA-dX*dX)*dDiv0;

        iPX0 = Mathd::RoundToInt(dCX-dX+0.25);
        iPY0 = Mathd::RoundToInt(dCY-dY+0.25);
        iPX1 = Mathd::RoundToInt(dCX+dX-0.25);
        iPY1 = Mathd::RoundToInt(dCY+dY-0.25);
        (pqClass->*oPlot)(iPX0,iPY0);
        (pqClass->*oPlot)(iPX1,iPY0);
        (pqClass->*oPlot)(iPX1,iPY1);
        (pqClass->*oPlot)(iPX0,iPY1);
    }
    for (int i = 0; i <= iHalfEY; i++)
    {
        dY = dB - i;
        dX = Mathd::Sqrt(dSqrB-dY*dY)*dDiv1;

        iPX0 = Mathd::RoundToInt(dCX-dX+0.25);
        iPY0 = Mathd::RoundToInt(dCY-dY+0.25);
        iPX1 = Mathd::RoundToInt(dCX+dX-0.25);
        iPY1 = Mathd::RoundToInt(dCY+dY-0.25);
        (pqClass->*oPlot)(iPX0,iPY0);
        (pqClass->*oPlot)(iPX1,iPY0);
        (pqClass->*oPlot)(iPX1,iPY1);
        (pqClass->*oPlot)(iPX0,iPY1);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotLine (float fX0, float fY0, float fX1, float fY1, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    PlotLine(
        Mathf::RoundToInt(fX0),
        Mathf::RoundToInt(fY0),
        Mathf::RoundToInt(fX1),
        Mathf::RoundToInt(fY1),pqClass,oPlot);
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotCircularArc (float fX0, float fY0, float fX1, float fY1, ClassT* pqClass,
    void (ClassT::*oPlot)(int,int))
{
    PlotCircularArc(
        Mathf::RoundToInt(fX0),
        Mathf::RoundToInt(fY0),
        Mathf::RoundToInt(fX1),
        Mathf::RoundToInt(fY1),pqClass,oPlot);
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotQuadraticBezierCurve (float fX0, float fY0, float fX1, float fY1,
    float fX2, float fY2, ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    float fVar0 = fX1 - fX0;
    float fVar1 = fX2 - 2.0f*fX1 + fX0;
    float fVar2 = fY1 - fY0;
    float fVar3 = fY2 - 2.0f*fY1 + fY0;
    float fDiv = fVar1*fVar1 + fVar3*fVar3;
    bool bTwo = false;
    if ( Mathf::FAbs(fDiv) > Mathf::ZERO_TOLERANCE )
    {
        float fSt = -(fVar0*fVar1 + fVar2*fVar3)/fDiv;
        if ( 0.0f < fSt && fSt < 1.0f )
        {
            float fX3 = fX0 + fVar0*fSt;
            float fY3 = fY0 + fVar2*fSt;
            float fX5 = fX1 + (fX2 - fX1)*fSt;
            float fY5 = fY1 + (fY2 - fY1)*fSt;
            float fX4 = fX3 + (fX5 - fX3)*fSt;
            float fY4 = fY3 + (fY5 - fY3)*fSt;
            RecurQBCSubdivisions(fX0,fY0,fX3,fY3,fX4,fY4,1,pqClass,oPlot);
            RecurQBCSubdivisions(fX4,fY4,fX5,fY5,fX2,fY2,1,pqClass,oPlot);
            bTwo = true;
        }
    }
    if ( !bTwo )
        RecurQBCSubdivisions(fX0,fY0,fX1,fY1,fX2,fY2,0,pqClass,oPlot);
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotQuadraticBezierCurveHQ (double dX0, double dY0, double dX1, double dY1,
    double dX2, double dY2, ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    double dVar0X, dVar1X, dVar0Y, dVar1Y, dDiff0, dDiff1, dVar0, dVar1, dMinX, dMaxX, dMinY, dMaxY, dInv,
        dS0, dS1, dT, dBX, dBY, dSqrT;
    double adT[2];
    int iMinX, iMaxX, iMinY, iMaxY, iX, iY;
    bool bOk;

    dVar0X = dX0 - dX1;
    dVar1X = dVar0X - dX1 + dX2;
    dVar0Y = dY0 - dY1;
    dVar1Y = dVar0Y - dY1 + dY2;

    // find out the tightest bounding rectangle for the curve
    bOk = false;
    if ( Mathd::FAbs(dVar1X) > Mathd::ZERO_TOLERANCE )
    {
        dT = dVar0X/dVar1X;
        if ( 0.0 <= dT && dT <= 1.0 )
        {
            dSqrT = dT*dT;
            dBX = dVar1X*dSqrT - 2.0*dVar0X*dT + dX0;
            bOk = true;
        }
    }
    if ( !bOk )
        dBX = dX0;
    bOk = false;
    if ( Mathd::FAbs(dVar1Y) > Mathd::ZERO_TOLERANCE )
    {
        dT = dVar0Y/dVar1Y;
        if ( 0.0 <= dT && dT <= 1.0 )
        {
            dSqrT = dT*dT;
            dBY = dVar1Y*dSqrT - 2.0*dVar0Y*dT + dY0;
            bOk = true;
        }
    }
    if ( !bOk )
        dBY = dY0;
    Mathd::MinMax3(dX0,dX2,dBX,dMinX,dMaxX);
    Mathd::MinMax3(dY0,dY2,dBY,dMinY,dMaxY);
    iMinX = Mathd::RoundToInt(dMinX) - 1;
    iMaxX = Mathd::RoundToInt(dMaxX) + 1;
    iMinY = Mathd::RoundToInt(dMinY) - 1;
    iMaxY = Mathd::RoundToInt(dMaxY) + 1;

    // to be sure
    (pqClass->*oPlot)(Mathd::RoundToInt(dX0),Mathd::RoundToInt(dY0));
    (pqClass->*oPlot)(Mathd::RoundToInt(dX2),Mathd::RoundToInt(dY2));

    // scan in x-direction
    dDiff0 = dY1 - dY0;
    dDiff1 = dY2 - dY1;
    if ( Mathd::FAbs(dVar1X) > Mathd::ZERO_TOLERANCE )
    {
        dVar0 = dX1*dX1 - dX2*dX0;
        dInv = -1.0/dVar1X;
        for (iX = iMinX; iX <= iMaxX; iX++)
        {
            dVar1 = Mathd::Sqrt(dVar1X*iX+dVar0);
            adT[0] = (-dVar0X - dVar1)*dInv;
            adT[1] = (-dVar0X + dVar1)*dInv;
            for (int i = 0; i < 2; i++)
            {
                if ( 0.0 <= adT[i] && adT[i] <= 1.0 )
                {
                    dS0 = dDiff0*adT[i] + dY0;
                    dS1 = dDiff1*adT[i] + dY1;
                    iY = Mathd::RoundToInt(dS0+(dS1-dS0)*adT[i]);
                    (pqClass->*oPlot)(iX,iY);
                }
            }
        }
    }
    else if ( Mathd::FAbs(dVar0X) > Mathd::ZERO_TOLERANCE )
    {
        dInv = -0.5/dVar0X;
        for (iX = iMinX; iX <= iMaxX; iX++)
        {
            dT = (iX - dX0)*dInv;
            if ( 0.0 <= dT && dT <= 1.0 )
            {
                dS0 = dDiff0*dT + dY0;
                dS1 = dDiff1*dT + dY1;
                iY = Mathd::RoundToInt(dS0+(dS1-dS0)*dT);
                (pqClass->*oPlot)(iX,iY);
            }
        }
    }

    // scan in y-direction
    dDiff0 = dX1 - dX0;
    dDiff1 = dX2 - dX1;
    if ( Mathd::FAbs(dVar1Y) > Mathd::ZERO_TOLERANCE )
    {
        dVar0 = dY1*dY1 - dY2*dY0;
        dInv = -1.0/dVar1Y;
        for (iY = iMinY; iY <= iMaxY; iY++)
        {
            dVar1 = Mathd::Sqrt(dVar1Y*iY+dVar0);
            adT[0] = (-dVar0Y - dVar1)*dInv;
            adT[1] = (-dVar0Y + dVar1)*dInv;
            for (int i = 0; i < 2; i++)
            {
                if ( 0.0 <= adT[i] && adT[i] <= 1.0 )
                {
                    dS0 = dDiff0*adT[i] + dX0;
                    dS1 = dDiff1*adT[i] + dX1;
                    iX = Mathd::RoundToInt(dS0+(dS1-dS0)*adT[i]);
                    (pqClass->*oPlot)(iX,iY);
                }
            }
        }
    }
    else if ( Mathd::FAbs(dVar0Y) > Mathd::ZERO_TOLERANCE )
    {
        dInv = -0.5/dVar0Y;
        for (iY = iMinY; iY <= iMaxY; iY++)
        {
            dT = (iY - dY0)*dInv;
            if ( 0.0 <= dT && dT <= 1.0 )
            {
                dS0 = dDiff0*dT + dX0;
                dS1 = dDiff1*dT + dX1;
                iX = Mathd::RoundToInt(dS0+(dS1-dS0)*dT);
                (pqClass->*oPlot)(iX,iY);
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotCubicBezierCurve (float fX0, float fY0, float fX1, float fY1,
    float fX2, float fY2, float fX3, float fY3, ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    // Constant s_iMaxRecursionDepth regulates maximal depth of stack's tree (and also serves as guaranty that
    // floating-point computation imperfections will not cause a hang in the loop); s_iMaxRecursionDepth = 1
    // means that a cubic Bezier curve will be drawn maximally by two quadratics.
    //
    // Somewhere starting up from value 3.0 for constant s_fMinMidpointDist the fact of approximation becomes
    // quite noticeable.
    static const int s_iMaxRecursionDepth = 10;
    static const float s_fMinMidpointDist = 1.0f;

    if ( fY0 == fX0 && fX1 == fX0 && fY1 == fX0 && fX2 == fX0 && fY2 == fX0 && fX3 == fX0 && fY3 == fX0 )
    {
        PlotQuadraticBezierCurve(fX0,fX0,fX0,fX0,fX0,fX0,pqClass,oPlot);
        return;
    }

    float fSqrMinMidpointDist = s_fMinMidpointDist*s_fMinMidpointDist;
    float aafRightStack[s_iMaxRecursionDepth][9];  // eight coordinates and the flag
    float fVar0, fVar1, fVar2, fVar3, fQCX, fQCY, fMX, fMY, fMid0X, fMid0Y, fMid1X, fMid1Y, fDX, fDY, fS0X, fS0Y,
        fS1X, fS1Y, fS2X, fS2Y, fS3X, fS3Y, fS4X, fS4Y, fS5X, fS5Y;
    int iLevel, iRI;
    bool bLeft, bNonZero;
    iLevel = 0;
    bLeft = true;
    for (/**/; /**/; /**/)
    {
        // begin finding the intersection point of lines directed by (P0,P1) and (P3,P2)
        fVar0 = fY2 - fY3;
        fVar1 = fX2 - fX3;
        fVar2 = fVar0*(fX1 - fX0) - fVar1*(fY1 - fY0);
        bNonZero = ( Mathf::FAbs(fVar2) > Mathf::ZERO_TOLERANCE );
        if ( bNonZero || iLevel == s_iMaxRecursionDepth )
        {
            if ( bNonZero )
            {
                // intersection point
                fVar3 = (fVar1*(fY0 - fY3) - fVar0*(fX0 - fX3))/fVar2;
                fQCX = fX0 + (fX1 - fX0)*fVar3;
                fQCY = fY0 + (fY1 - fY0)*fVar3;

                // cubic and quadratic curve midpoints
                fMid0X = (fX0 + fX3)*0.125f + (fX1 + fX2)*0.375f;
                fMid0Y = (fY0 + fY3)*0.125f + (fY1 + fY2)*0.375f;
                fMid1X = (fX0 + fX3)*0.25f + fQCX*0.5f;
                fMid1Y = (fY0 + fY3)*0.25f + fQCY*0.5f;
                fDX = fMid0X - fMid1X;
                fDY = fMid0Y - fMid1Y;
            }
            if ( iLevel == s_iMaxRecursionDepth || (bNonZero && fDX*fDX + fDY*fDY <= fSqrMinMidpointDist) )
            {
                // midpoints are close enough or the maximal depth level reached
                if ( bNonZero )
                {
                    fMX = fQCX;
                    fMY = fQCY;
                }
                else
                {
                    fMX = (fX1 + fX2)*0.5f;
                    fMY = (fY1 + fY2)*0.5f;
                }
                PlotQuadraticBezierCurve(fX0,fY0,fMX,fMY,fX3,fY3,pqClass,oPlot);

                // from here always go to the right branch on the same level or upper
                if ( bLeft )
                {
                    if ( iLevel == 0 )
                        return;
                    bLeft = false;
                    iRI = iLevel - 1;
                }
                else
                {
                    do
                    {
                        if ( --iLevel == 0 )
                            return;
                        iRI = iLevel - 1;
                    } while ( aafRightStack[iRI][8] == 1.0f );
                }
                aafRightStack[iRI][8] = 1.0f;
                fX0 = aafRightStack[iRI][0];
                fY0 = aafRightStack[iRI][1];
                fX1 = aafRightStack[iRI][2];
                fY1 = aafRightStack[iRI][3];
                fX2 = aafRightStack[iRI][4];
                fY2 = aafRightStack[iRI][5];
                fX3 = aafRightStack[iRI][6];
                fY3 = aafRightStack[iRI][7];
                continue;
            }
        }

        // Midpoint subdivision
        fS0X = (fX0 + fX1)*0.5f;
        fS0Y = (fY0 + fY1)*0.5f;
        fS1X = (fX1 + fX2)*0.5f;
        fS1Y = (fY1 + fY2)*0.5f;
        fS2X = (fX2 + fX3)*0.5f;
        fS2Y = (fY2 + fY3)*0.5f;
        fS3X = (fS0X + fS1X)*0.5f;
        fS3Y = (fS0Y + fS1Y)*0.5f;
        fS4X = (fS1X + fS2X)*0.5f;
        fS4Y = (fS1Y + fS2Y)*0.5f;
        fS5X = (fS3X + fS4X)*0.5f;
        fS5Y = (fS3Y + fS4Y)*0.5f;

        // the next will be the left branch
        aafRightStack[iLevel][0] = fS5X;
        aafRightStack[iLevel][1] = fS5Y;
        aafRightStack[iLevel][2] = fS4X;
        aafRightStack[iLevel][3] = fS4Y;
        aafRightStack[iLevel][4] = fS2X;
        aafRightStack[iLevel][5] = fS2Y;
        aafRightStack[iLevel][6] = fX3;
        aafRightStack[iLevel][7] = fY3;
        aafRightStack[iLevel][8] = 0.0f;
        fX1 = fS0X;
        fY1 = fS0Y;
        fX2 = fS3X;
        fY2 = fS3Y;
        fX3 = fS5X;
        fY3 = fS5Y;
        iLevel++;
        bLeft = true;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::PlotCubicBezierCurveHQ (double dX0, double dY0, double dX1, double dY1,
    double dX2, double dY2, double dX3, double dY3, ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    // cubic polynomial coefficients
    double dC0X = dX0;
    double dC0Y = dY0;
    double dC1X = 3.0*(dX1 - dX0);
    double dC1Y = 3.0*(dY1 - dY0);
    double dC2X = 3.0*(dX2 - dX1) - dC1X;
    double dC2Y = 3.0*(dY2 - dY1) - dC1Y;
    double dC3X = dX3 - dX0 - dC1X - dC2X;
    double dC3Y = dY3 - dY0 - dC1Y - dC2Y;

    // find out the tightest bounding rectangle for the curve
    PolynomialRootSolvingd qPRSX(Mathd::ZERO_TOLERANCE);
    PolynomialRootSolvingd qPRSY(Mathd::ZERO_TOLERANCE);
    qPRSX.FindA(dC1X,2.0*dC2X,3.0*dC3X);
    qPRSY.FindA(dC1Y,2.0*dC2Y,3.0*dC3Y);
    double adExPX[4];
    double adExPY[4];
    double dT, dSqrT;
    for (int i = 0; i < 2; i++)
    {
        if ( i < qPRSX.GetCount()
        &&   0.0 <= qPRSX.GetRoot(i) && qPRSX.GetRoot(i) <= 1.0 )
        {
            dT = qPRSX.GetRoot(i);
            dSqrT = dT*dT;
            adExPX[i] =
                dC3X*dSqrT*dT +
                dC2X*dSqrT +
                dC1X*dT +
                dC0X;
        }
        else
        {
            adExPX[i] = dX0;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if ( i < qPRSY.GetCount()
        &&   0.0 <= qPRSY.GetRoot(i) && qPRSY.GetRoot(i) <= 1.0 )
        {
            dT = qPRSY.GetRoot(i);
            dSqrT = dT*dT;
            adExPY[i] =
                dC3Y*dSqrT*dT +
                dC2Y*dSqrT +
                dC1Y*dT +
                dC0Y;
        }
        else
        {
            adExPY[i] = dY0;
        }
    }
    adExPX[2] = dX0;
    adExPX[3] = dX3;
    adExPY[2] = dY0;
    adExPY[3] = dY3;
    double dMinX = adExPX[0];
    double dMaxX = adExPX[0];
    double dMinY = adExPY[0];
    double dMaxY = adExPY[0];
    for (int i = 1; i < 4; i++)
    {
        if ( adExPX[i] < dMinX )
            dMinX = adExPX[i];
        else if ( adExPX[i] > dMaxX )
            dMaxX = adExPX[i];
        if ( adExPY[i] < dMinY )
            dMinY = adExPY[i];
        else if ( adExPY[i] > dMaxY )
            dMaxY = adExPY[i];
    }
    int iMinX = Mathd::RoundToInt(dMinX) - 1;
    int iMaxX = Mathd::RoundToInt(dMaxX) + 1;
    int iMinY = Mathd::RoundToInt(dMinY) - 1;
    int iMaxY = Mathd::RoundToInt(dMaxY) + 1;

    // to be sure
    (pqClass->*oPlot)(Mathd::RoundToInt(dX0),Mathd::RoundToInt(dY0));
    (pqClass->*oPlot)(Mathd::RoundToInt(dX3),Mathd::RoundToInt(dY3));

    int iX, iY;

    // scan in x-direction
    for (iX = iMinX; iX <= iMaxX; iX++)
    {
        PolynomialRootSolvingd qPRS(Mathd::ZERO_TOLERANCE);
        qPRS.FindA(dC0X-iX,dC1X,dC2X,dC3X);
        for (int i = 0; i < qPRS.GetCount(); i++)
        {
            dT = qPRS.GetRoot(i);
            if ( 0.0 <= dT && dT <= 1.0 )
            {
                dSqrT = dT*dT;
                iY = Mathd::RoundToInt(
                    dC3Y*dSqrT*dT +
                    dC2Y*dSqrT +
                    dC1Y*dT +
                    dC0Y);
                (pqClass->*oPlot)(iX,iY);
            }
        }
    }

    // scan in y-direction
    for (iY = iMinY; iY <= iMaxY; iY++)
    {
        PolynomialRootSolvingd qPRS(Mathd::ZERO_TOLERANCE);
        qPRS.FindA(dC0Y-iY,dC1Y,dC2Y,dC3Y);
        for (int i = 0; i < qPRS.GetCount(); i++)
        {
            dT = qPRS.GetRoot(i);
            if ( 0.0 <= dT && dT <= 1.0 )
            {
                dSqrT = dT*dT;
                iX = Mathd::RoundToInt(
                    dC3X*dSqrT*dT +
                    dC2X*dSqrT +
                    dC1X*dT +
                    dC0X);
                (pqClass->*oPlot)(iX,iY);
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ClassT>
void DiscretePlotting<ClassT>::RecurQBCSubdivisions (float fX0, float fY0, float fX1, float fY1,
    float fX2, float fY2, int iLevel, ClassT* pqClass, void (ClassT::*oPlot)(int,int))
{
    // Constant s_iMaxRecursionDepth is one of delimiters of the subdivision process; value 4 or 5 is surely
    // enough for quadratic Bezier curves and should not cause any stack overflows.
    //
    // Constant s_fMinColin relates to another criterion when to stop the subdivision: if calculated
    // colinearity is less than this constant, the sub-curve is treated as line-like, two approximating lines
    // are drawn, and subdivision stops.  Due to the variation-diminishing property of Bezier curves, there is
    // no need to fit colinearity value or alter the constant while size/length/curvature of a curve is
    // changing.  The range (40.0, 400.0) for s_fMinColin is likely to provide reasonable space for controlling
    // of drawing quality; the maximal one relates to bigger dog-legging but greater speed.
    static const int s_iMaxRecursionDepth = 4;
    static const float s_fMinColin = 80.0f;

    float fMX = (fX0 + fX2)*0.25f + fX1*0.5f;
    float fMY = (fY0 + fY2)*0.25f + fY1*0.5f;
    if ( iLevel == s_iMaxRecursionDepth || Mathf::FAbs((fMX-fX0)*(fY2-fY0)-(fX2-fX0)*(fMY-fY0)) < s_fMinColin )
    {
        PlotLine(fX0,fY0,fMX,fMY,pqClass,oPlot);
        PlotLine(fMX,fMY,fX2,fY2,pqClass,oPlot);
        return;
    }

    // Midpoint subdivision
    float fS0X = (fX0 + fX1)*0.5f;
    float fS0Y = (fY0 + fY1)*0.5f;
    float fS1X = (fX1 + fX2)*0.5f;
    float fS1Y = (fY1 + fY2)*0.5f;
    float fS2X = (fS0X + fS1X)*0.5f;
    float fS2Y = (fS0Y + fS1Y)*0.5f;

    iLevel++;
    RecurQBCSubdivisions(fX0,fY0,fS0X,fS0Y,fS2X,fS2Y,iLevel,pqClass,oPlot);
    RecurQBCSubdivisions(fS2X,fS2Y,fS1X,fS1Y,fX2,fY2,iLevel,pqClass,oPlot);
}
//------------------------------------------------------------------------------------------------------------------



