#ifndef QDISCRETEPLOTTING_H
#define QDISCRETEPLOTTING_H

// The DiscretePlotting class is for plotting of geometric elements on a raster.  It is made as template in
// order to allow a member function of some class to serve as dot-plotter to be called by the drawing routines.

#include "QPolynomialRootSolving.h"

namespace Q
{

template <class ClassT>
class DiscretePlotting
{
public:
    // plots single point for a zero-extension line
    static void PlotLine (int iX0, int iY0, int iX1, int iY1, ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // iRadius = 0 makes single point in the center of a circle
    static void PlotCircle (int iCX, int iCY, int iRadius, ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // Parameter iRadius designates maximal radius of rectangle's rounded corners; iRadius = 0 makes no rounded
    // corners at all.  Actual radius used in plotting is always less than or equal to half of the shortest
    // side extension.  Single point is plotted for a null-rectangle.
    static void PlotRectangle (int iLeft, int iTop, int iRight, int iBottom, int iRadius, ClassT* pqClass,
        void (ClassT::*oPlot)(int,int));

    // Plots a circular arc from (x0,y0) to (x1,y1) in the clockwise direction always including (x0,y0) and
    // (x1,y1).  Only integer arithmetic is used and only additions, subtractions, and bitwise shifts present
    // in the main loop.  The function plots single point if the starting point of an arc equals to the ending
    // point.
    static void PlotCircularArc (int iX0, int iY0, int iX1, int iY1, ClassT* pqClass,
        void (ClassT::*oPlot)(int,int));

    // If side extensions of the bounding rectangle are equal and are even, the function calls the circle
    // plotting.  When iLeft = iRight and iTop = iBottom, it plots single point.  'double' type of variables
    // within the routine is chosen to maximally avoid ruptures on lengthy contours.
    static void PlotEllipse (int iLeft, int iTop, int iRight, int iBottom,
        ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // floating-point variants of PlotLine and PlotCircularArc (do rounding to integers)
    static void PlotLine (float fX0, float fY0, float fX1, float fY1, ClassT* pqClass,
        void (ClassT::*oPlot)(int,int));
    static void PlotCircularArc (float fX0, float fY0, float fX1, float fY1, ClassT* pqClass,
        void (ClassT::*oPlot)(int,int));

    // Using the Midpoint method, the function recurs subdivisions and approximates quadratic sub-curves with
    // lines.  It plots single point if control points are the same.
    static void PlotQuadraticBezierCurve (float fX0, float fY0, float fX1, float fY1, float fX2, float fY2,
        ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // The function offers drawing of a quadratic Bezier curve with the highest quality on the raster from
    // which control points go as parameters.  Within the tight bounding box has been found, a curve is scanned
    // in X and Y directions with solving of a quadratic equation for each scanline.  The process is quite
    // optimized, leaving one square root computation per scanline as only slower.  'double' type of variables
    // within the routine is chosen to maximally avoid ruptures on lengthy contours.  It plots single point if
    // control points are the same.
    static void PlotQuadraticBezierCurveHQ (double dX0, double dY0, double dX1, double dY1, double dX2, double dY2,
        ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // The function approximates a cubic Bezier curve by sequence of quadratic curves recursively (with stack
    // emulation) subdividing the curve on two sub-curves using the Midpoint method.  Knowing that whole class
    // of Bezier curves has the property that segments (P0,P1) and (Pn-1,Pn-2) are tangents to the curve at the
    // beginning and at the end respectively, a cubic curve can be substituted by a quadratic one which begins
    // and ends at the same points and has the middle control point taken from intersection of lines directed
    // by (P0,P1) and (P3,P2) segments of the cubic curve.  The criterion to estimate approximation's goodness
    // is distance between the middle points (t = 0.5) of the cubic and quadratic curves.  It plots single
    // point if control points are the same.
    static void PlotCubicBezierCurve (float fX0, float fY0, float fX1, float fY1, float fX2, float fY2,
        float fX3, float fY3, ClassT* pqClass, void (ClassT::*oPlot)(int,int));

    // The function draws a cubic Bezier curve with the highest quality on the raster from which control points
    // go as parameters.  Within the tight bounding box has been found, a curve is scanned in X and Y
    // directions with solving of a cubic equation for each scanline.  'double' type of variables within the
    // routine is chosen to maximally avoid ruptures on lengthy contours.  It plots single point if control
    // points are the same.
    static void PlotCubicBezierCurveHQ (double dX0, double dY0, double dX1, double dY1, double dX2, double dY2,
        double dX3, double dY3, ClassT* pqClass, void (ClassT::*oPlot)(int,int));

private:
    static void RecurQBCSubdivisions (float fX0, float fY0, float fX1, float fY1, float fX2, float fY2, int iLevel,
        ClassT* pqClass, void (ClassT::*oPlot)(int,int));
};

#include "QDiscretePlotting.mcr"
#include "QDiscretePlotting.inl"

}

#endif



