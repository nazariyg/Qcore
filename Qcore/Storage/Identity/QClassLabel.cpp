#include "QClassLabel.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ClassLabel::ClassLabel (const char* acName, const ClassLabel* pqBaseClass)
    :
    m_acName(acName),
    m_pqBaseClass(pqBaseClass)
{
}
//------------------------------------------------------------------------------------------------------------------
bool ClassLabel::IsDerived (const ClassLabel& rqClass) const
{
    const ClassLabel* pqSearch = this;
    while ( pqSearch )
    {
        if ( &rqClass == pqSearch )
            return true;
        pqSearch = pqSearch->m_pqBaseClass;
    }
    return false;
}
//------------------------------------------------------------------------------------------------------------------



