#ifndef QFOGSTATE_H
#define QFOGSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM FogState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum EquationType
    {
        ET_LINEAR,
        ET_EXP,
        ET_EXP2
    };

    enum CoordinateSourceType
    {
        CST_FRAGMENT_DEPTH,
        CST_FOG_COORD
    };

    enum HintType
    {
        HT_FASTEST,
        HT_NICEST
    };

    // construction
    FogState (EquationType eEquation, CoordinateSourceType eCoordinateSource, HintType eHint,
        const ColorRgba& rqColor, float fDensity, float fStart, float fEnd);

    EquationType Equation;
    CoordinateSourceType CoordinateSource;
    HintType Hint;
    ColorRgba Color;
    float Density;
    float Start;
    float End;
};

typedef SmartPointer<FogState> FogStatePtr;

}

#endif



