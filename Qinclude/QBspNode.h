#ifndef QBSPNODE_H
#define QBSPNODE_H

// Binary space partition tree node.

#include "QSystem.h"

namespace Q
{

template <class ElementT>
class BspNode
{
public:
    // construction
    BspNode ();  // the pointers to parent, negative, and positive node are initialized to 0s
    BspNode (const ElementT& rtElement, BspNode* pqParent);  // the pointers to negative and positive node are 0s

    // member access
    void SetParentNode (BspNode* pqParent);
    void AttachNegativeChild (BspNode* pqChild);
    void AttachPositiveChild (BspNode* pqChild);
    void DetachNegativeChild ();
    void DetachPositiveChild ();
    ElementT& GetElement ();
    const ElementT& GetElement () const;
    BspNode* GetParentNode ();
    BspNode* GetNegativeChild ();
    BspNode* GetPositiveChild ();

    // To find the negative-most or the positive-most child from the node's viewpoint.  If there is no such,
    // the pointer to itself is returned.
    BspNode* GetNegativemostChild ();
    BspNode* GetPositivemostChild ();

    // Walking upon the tree.  If there is no next or previous node, then the function returns 0.
    BspNode* GetNextNode ();
    BspNode* GetPriorNode ();

private:
    ElementT m_tElement;
    BspNode* m_pqParent;
    BspNode* m_pqNegative;
    BspNode* m_pqPositive;
};

#include "QBspNode.inl"

}

#endif



