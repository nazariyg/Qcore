//------------------------------------------------------------------------------------------------------------------
inline bool Recti::Contains (int iX, int iY) const
{
    return ( iX >= X && iY >= Y && iX < X + Width && iY < Y + Height );
}
//------------------------------------------------------------------------------------------------------------------



