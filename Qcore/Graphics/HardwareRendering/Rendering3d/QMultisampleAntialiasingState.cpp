#include "QMultisampleAntialiasingState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,MultisampleAntialiasingState);

//------------------------------------------------------------------------------------------------------------------
MultisampleAntialiasingState::MultisampleAntialiasingState ()
    :
    SampleAlphaToCoverageSet(false),
    SampleAlphaToOneSet(false),
    SampleCoverageSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------



