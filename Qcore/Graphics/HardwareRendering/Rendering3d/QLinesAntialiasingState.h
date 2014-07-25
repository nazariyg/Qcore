#ifndef QLINESANTIALIASINGSTATE_H
#define QLINESANTIALIASINGSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM LinesAntialiasingState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum HintType
    {
        HT_FASTEST,
        HT_NICEST
    };

    // construction
    LinesAntialiasingState (HintType eHint);

    HintType Hint;
};

typedef SmartPointer<LinesAntialiasingState> LinesAntialiasingStatePtr;

}

#endif



