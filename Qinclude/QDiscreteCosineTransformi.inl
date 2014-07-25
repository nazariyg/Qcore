//------------------------------------------------------------------------------------------------------------------
inline unsigned char DiscreteCosineTransformi::GetClampedToUChar (int iValue)
{
    iValue += 128;
    if ( iValue < 0 )
        iValue = 0;
    else if ( iValue > 255 )
        iValue = 255;
    return iValue;
}
//------------------------------------------------------------------------------------------------------------------



