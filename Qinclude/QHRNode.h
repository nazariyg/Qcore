#ifndef QHRNODE_H
#define QHRNODE_H

#include "QHRGeometry.h"

namespace Q
{

template <class Real>
class HRNode : public Heap, public Transformable3<Real,true>
{
public:
    // construction and destruction
    HRNode (HRNode<Real>* pqParentNode);

#ifdef _DEBUG
    ~HRNode ();
#endif

    // Member access.

    // geometries
    int GetGeometryQuantity () const;
    int AddGeometry (const SmartPointer<HRGeometry<Real> >& rspqGeometry);
    const SmartPointer<HRGeometry<Real> >& GetGeometry (int i) const;
    void RemoveGeometry (int i);

    // nodes
    HRNode<Real>* GetParentNode ();
    int GetNodeQuantity () const;
    int AttachNode (const SmartPointer<HRNode<Real> >& rspqNode);
    const SmartPointer<HRNode<Real> >& GetNode (int i) const;
    void DetachNode (int i);

private:
    friend class OpenGLRendering;

    ModestArray<SmartPointer<HRGeometry<Real> > > m_qGeometries;
    HRNode<Real>* m_pqParentNode;
    ModestArray<SmartPointer<HRNode<Real> > > m_qNodes;

    // OnDetach also notifies the Hardware Renderer so it can unbuffer bufferable objects and free more memory
    void OnAttach ();
    void OnDetach ();

#ifdef _DEBUG
    bool m_bAttached;  // default: false
#endif
};

#include "QHRNode.inl"

typedef HRNode<float> HRNodef;
typedef HRNode<double> HRNoded;

typedef SmartPointer<HRNodef> HRNodefPtr;
typedef SmartPointer<HRNoded> HRNodedPtr;

}

#endif



