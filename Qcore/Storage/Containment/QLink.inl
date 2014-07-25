//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
Link<FirstT,SecondT>::Link ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
Link<FirstT,SecondT>::Link (const FirstT& rtFirst, const SecondT& rtSecond)
    :
    First(rtFirst),
    Second(rtSecond)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
Link<FirstT,SecondT>::Link (const Link& rqLink)
    :
    First(rqLink.First),
    Second(rqLink.Second)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator== (const Link& rqLink) const
{
    return ( First == rqLink.First && Second == rqLink.Second );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator!= (const Link& rqLink) const
{
    return ( !(*this == rqLink) );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator< (const Link& rqLink) const
{
    return ( First < rqLink.First || (!(rqLink.First < First) && Second < rqLink.Second) );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator<= (const Link& rqLink) const
{
    return ( !(rqLink < *this) );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator> (const Link& rqLink) const
{
    return ( rqLink < *this );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
bool Link<FirstT,SecondT>::operator>= (const Link& rqLink) const
{
    return ( !(*this < rqLink) );
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
void Link<FirstT,SecondT>::Save (Stream& rqStream) const
{
    Q_IF_EXISTS ( FirstT::Save )
    {
        // has the special member function
        First.Save(rqStream);
    }
    Q_IF_NOT_EXISTS ( FirstT::Save )
    {
        // native or aggregate
        rqStream.Write(First);
    }
    Q_IF_EXISTS ( SecondT::Save )
    {
        // has the special member function
        Second.Save(rqStream);
    }
    Q_IF_NOT_EXISTS ( SecondT::Save )
    {
        // native or aggregate
        rqStream.Write(Second);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
void Link<FirstT,SecondT>::Load (Stream& rqStream)
{
    Q_IF_EXISTS ( FirstT::Load )
    {
        // has the special member function
        First.Load(rqStream);
    }
    Q_IF_NOT_EXISTS ( FirstT::Load )
    {
        // native or aggregate
        rqStream.Read(First);
    }
    Q_IF_EXISTS ( SecondT::Load )
    {
        // has the special member function
        Second.Load(rqStream);
    }
    Q_IF_NOT_EXISTS ( SecondT::Load )
    {
        // native or aggregate
        rqStream.Read(Second);
    }
}
//------------------------------------------------------------------------------------------------------------------



