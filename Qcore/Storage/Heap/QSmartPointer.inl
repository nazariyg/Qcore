//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>::SmartPointer (ObjectT* pqObject)
    :
    m_pqObject(pqObject)
{
    if ( m_pqObject )
        m_pqObject->IncrementReferences((void**)&m_pqObject);
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>::SmartPointer (const SmartPointer& rqPointer)
    :
    m_pqObject(rqPointer.m_pqObject)
{
    if ( m_pqObject )
        m_pqObject->IncrementReferences((void**)&m_pqObject);
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>::~SmartPointer ()
{
    if ( m_pqObject )
        m_pqObject->DecrementReferences((void**)&m_pqObject);
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>::operator ObjectT* () const
{
    return m_pqObject;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
ObjectT& SmartPointer<ObjectT>::operator* () const
{
    return *m_pqObject;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
ObjectT* SmartPointer<ObjectT>::operator-> () const
{
    return m_pqObject;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>::operator SmartRttiPointer () const
{
    Q_IF_NOT_EXISTS ( ObjectT::IsExactly )
    {
        assert( false );
    }
    return SmartRttiPointer((HeapRtti*)m_pqObject);
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>& SmartPointer<ObjectT>::operator= (ObjectT* pqObject)
{
    if ( m_pqObject != pqObject )
    {
        if ( pqObject )
            pqObject->IncrementReferences((void**)&m_pqObject);
        if ( m_pqObject )
            m_pqObject->DecrementReferences((void**)&m_pqObject);
        m_pqObject = pqObject;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
SmartPointer<ObjectT>& SmartPointer<ObjectT>::operator= (const SmartPointer& rqPointer)
{
    if ( m_pqObject != rqPointer.m_pqObject )
    {
        if ( rqPointer.m_pqObject )
            rqPointer.m_pqObject->IncrementReferences((void**)&m_pqObject);
        if ( m_pqObject )
            m_pqObject->DecrementReferences((void**)&m_pqObject);
        m_pqObject = rqPointer.m_pqObject;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
bool SmartPointer<ObjectT>::operator== (ObjectT* pqObject) const
{
    return ( m_pqObject == pqObject );
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
bool SmartPointer<ObjectT>::operator!= (ObjectT* pqObject) const
{
    return ( m_pqObject != pqObject );
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
bool SmartPointer<ObjectT>::operator== (const SmartPointer& rqPointer) const
{
    return ( m_pqObject == rqPointer.m_pqObject );
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
bool SmartPointer<ObjectT>::operator!= (const SmartPointer& rqPointer) const
{
    return ( m_pqObject != rqPointer.m_pqObject );
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
inline void SmartPointer<ObjectT>::Save (Stream& rqStream) const
{
    Q_IF_EXISTS ( ObjectT::GetId )
    {
        // has the special member function
        rqStream.Write(m_pqObject->GetId());
    }
    Q_IF_NOT_EXISTS ( ObjectT::GetId )
    {
        assert( false );
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
inline void SmartPointer<ObjectT>::Load (Stream& rqStream)
{
    Q_IF_EXISTS ( ObjectT::GetId )
    {
        // has the special member function
        unsigned int uiId;
        rqStream.Read(uiId);
        rqStream.MapId(uiId,(void**)&m_pqObject);
    }
    Q_IF_NOT_EXISTS ( ObjectT::GetId )
    {
        assert( false );
    }
}
//------------------------------------------------------------------------------------------------------------------



