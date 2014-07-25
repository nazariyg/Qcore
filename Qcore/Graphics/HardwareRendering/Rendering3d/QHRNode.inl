//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRNode<Real>::HRNode (HRNode<Real>* pqParentNode)
    :
    m_pqParentNode(pqParentNode)
{
#ifdef _DEBUG
    m_bAttached = false;
#endif
}
//------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
template <class Real>
HRNode<Real>::~HRNode ()
{
    // must be detached
    assert( !m_bAttached );
}
#endif
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int HRNode<Real>::GetGeometryQuantity () const
{
    return m_qGeometries.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int HRNode<Real>::AddGeometry (const SmartPointer<HRGeometry<Real> >& rspqGeometry)
{
    return m_qGeometries.Push(rspqGeometry);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const SmartPointer<HRGeometry<Real> >& HRNode<Real>::GetGeometry (int i) const
{
    return m_qGeometries[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRNode<Real>::RemoveGeometry (int i)
{
    // notify the Hardware Renderer
    if ( HardwareRendering::ms_pqItself )
        HardwareRendering::ms_pqItself->OnBufferableLeave(*m_qGeometries[i]);

    m_qGeometries.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRNode<Real>* HRNode<Real>::GetParentNode ()
{
    return m_pqParentNode;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int HRNode<Real>::GetNodeQuantity () const
{
    return m_qNodes.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int HRNode<Real>::AttachNode (const SmartPointer<HRNode<Real> >& rspqNode)
{
    rspqNode->m_pqParentNode = this;
    rspqNode->OnAttach();
    return m_qNodes.Push(rspqNode);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const SmartPointer<HRNode<Real> >& HRNode<Real>::GetNode (int i) const
{
    return m_qNodes[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRNode<Real>::DetachNode (int i)
{
    m_qNodes[i]->m_pqParentNode = 0;
    m_qNodes[i]->OnDetach();
    m_qNodes.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRNode<Real>::OnAttach ()
{
    // a node cannot be attached twice or more times
    assert( !m_bAttached );

#ifdef _DEBUG
    // recur
    for (int i = 0; i < m_qNodes.GetQuantity(); i++)
        m_qNodes[i]->OnAttach();

    m_bAttached = true;
#endif
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRNode<Real>::OnDetach ()
{
    // notify the Hardware Renderer
    if ( HardwareRendering::ms_pqItself )
    {
        for (int i = 0; i < m_qGeometries.GetQuantity(); i++)
            HardwareRendering::ms_pqItself->OnBufferableLeave(*m_qGeometries[i]);
    }

    // recur
    for (int i = 0; i < m_qNodes.GetQuantity(); i++)
        m_qNodes[i]->OnDetach();

#ifdef _DEBUG
    m_bAttached = false;
#endif
}
//------------------------------------------------------------------------------------------------------------------



