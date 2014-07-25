#ifndef QMULTISAMPLEANTIALIASINGSTATE_H
#define QMULTISAMPLEANTIALIASINGSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM MultisampleAntialiasingState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    MultisampleAntialiasingState ();

    // member access
    void SetSampleAlphaToCoverage (bool bSet);
    void UnsetSampleAlphaToCoverage ();
    void SetSampleAlphaToOne (bool bSet);
    void UnsetSampleAlphaToOne ();
    void SetSampleCoverage (float fValue, bool bInvert);
    void UnsetSampleCoverage ();

    bool SampleAlphaToCoverageSet;      // default: false
    bool SampleAlphaToCoverage;
    bool SampleAlphaToOneSet;           // default: false
    bool SampleAlphaToOne;
    bool SampleCoverageSet;             // default: false
    float SampleCoverageValue;
    bool SampleCoverageInvert;
};

#include "QMultisampleAntialiasingState.inl"

typedef SmartPointer<MultisampleAntialiasingState> MultisampleAntialiasingStatePtr;

}

#endif



