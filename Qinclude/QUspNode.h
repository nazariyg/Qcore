#ifndef QUSPNODE_H
#define QUSPNODE_H

// Unary space partition chain node.

#include "QSystem.h"

namespace Q
{

template <class ElementT>
class UspNode
{
public:
    // construction
    UspNode ();  // the pointers to prior and next node are initialized to 0s
    UspNode (const ElementT& rtElement, UspNode* pqPrior);  // the pointer to next node is 0

    // member access
    void SetPriorNode (UspNode* pqPrior);
    void AttachNextNode (UspNode* pqNext);
    void DetachNextNode ();
    ElementT& GetElement ();
    const ElementT& GetElement () const;
    UspNode* GetPriorNode ();
    UspNode* GetNextNode ();

private:
    ElementT m_tElement;
    UspNode* m_pqPrior;
    UspNode* m_pqNext;
};

#include "QUspNode.inl"

}

#endif



