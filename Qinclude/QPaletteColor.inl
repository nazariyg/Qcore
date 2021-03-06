//------------------------------------------------------------------------------------------------------------------
inline unsigned char PaletteColor::R () const
{
    return Components[2];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char& PaletteColor::R ()
{
    return Components[2];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char PaletteColor::G () const
{
    return Components[1];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char& PaletteColor::G ()
{
    return Components[1];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char PaletteColor::B () const
{
    return Components[0];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char& PaletteColor::B ()
{
    return Components[0];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char PaletteColor::A () const
{
    return Components[3];
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned char& PaletteColor::A ()
{
    return Components[3];
}
//------------------------------------------------------------------------------------------------------------------
inline ColorRgb PaletteColor::GetColorRgb () const
{
    return ColorRgb(
        ((float)Components[2])/255,
        ((float)Components[1])/255,
        ((float)Components[0])/255);
}
//------------------------------------------------------------------------------------------------------------------
inline ColorRgba PaletteColor::GetColorRgba () const
{
    return ColorRgba(
        ((float)Components[2])/255,
        ((float)Components[1])/255,
        ((float)Components[0])/255,
        ((float)Components[3])/255);
}
//------------------------------------------------------------------------------------------------------------------



