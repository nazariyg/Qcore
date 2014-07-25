#include "QIdenUtils.h"

namespace Q
{
//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// special class
class RttiDerivative : public Rtti
{
Q_DECLARE_RTTI;
};

Q_IMPLEMENT_RTTI(Q,Rtti,RttiDerivative);

template
RttiDerivative* StaticCast<RttiDerivative> (Rtti*);

template
const RttiDerivative* StaticCast<RttiDerivative> (const Rtti*);

template
RttiDerivative* DynamicCast<RttiDerivative> (Rtti*);

template
const RttiDerivative* DynamicCast<RttiDerivative> (const Rtti*);
//------------------------------------------------------------------------------------------------------------------
}



