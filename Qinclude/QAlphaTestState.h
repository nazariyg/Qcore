#ifndef QALPHATESTSTATE_H
#define QALPHATESTSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM AlphaTestState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum FunctionType
    {
        FT_NEVER,
        FT_ALWAYS,
        FT_LESS,
        FT_LESS_OR_EQUAL,
        FT_EQUAL,
        FT_GREATER_OR_EQUAL,
        FT_GREATER,
        FT_NOT_EQUAL
    };

    // construction
    AlphaTestState (FunctionType eFunction, float fReference);

    FunctionType Function;
    float Reference;
};

typedef SmartPointer<AlphaTestState> AlphaTestStatePtr;

}

#endif



