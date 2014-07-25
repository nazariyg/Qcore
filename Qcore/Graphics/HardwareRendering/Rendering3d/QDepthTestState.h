#ifndef QDEPTHTESTSTATE_H
#define QDEPTHTESTSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM DepthTestState : public StateBase
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
    DepthTestState (FunctionType eFunction);

    FunctionType Function;
};

typedef SmartPointer<DepthTestState> DepthTestStatePtr;

}

#endif



