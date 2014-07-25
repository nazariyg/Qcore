//------------------------------------------------------------------------------------------------------------------
template <class Real, class Operand0, class Operand1>
IntrBase3<Real,Operand0,Operand1>::IntrBase3 (const Operand0& rqOperand0, const Operand1& rqOperand1)
    :
    m_rqOperand0(rqOperand0),
    m_rqOperand1(rqOperand1),
    m_bKnownBB0(false),
    m_bKnownBB1(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class Operand0, class Operand1>
void IntrBase3<Real,Operand0,Operand1>::ImpartBoundingBox0 (const AxisAlignedBox3<Real>& rqBoundingBox)
{
    m_qBoundingBox0 = rqBoundingBox;
    m_bKnownBB0 = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class Operand0, class Operand1>
void IntrBase3<Real,Operand0,Operand1>::ImpartBoundingBox1 (const AxisAlignedBox3<Real>& rqBoundingBox)
{
    m_qBoundingBox1 = rqBoundingBox;
    m_bKnownBB1 = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class Operand0, class Operand1>
bool IntrBase3<Real,Operand0,Operand1>::TestBoundingBoxes () const
{
    if ( m_bKnownBB0 )
    {
        if ( m_bKnownBB1 )
        {
            if ( m_qBoundingBox0.TestIntersection(m_qBoundingBox1) )
                return true;
        }
        else
        {
            if ( m_qBoundingBox0.TestIntersection(GeomUtils3<Real>::GetBoundingBoxOf(m_rqOperand1)) )
                return true;
        }
    }
    else
    {
        if ( m_bKnownBB1 )
        {
            if ( GeomUtils3<Real>::GetBoundingBoxOf(m_rqOperand0).TestIntersection(m_qBoundingBox1) )
                return true;
        }
        else
        {
            if ( GeomUtils3<Real>::GetBoundingBoxOf(m_rqOperand0).TestIntersection(
                 GeomUtils3<Real>::GetBoundingBoxOf(m_rqOperand1)) )
            {
                return true;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------------



