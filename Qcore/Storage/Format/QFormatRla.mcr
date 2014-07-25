//------------------------------------------------------------------------------------------------------------------
#define PROCESS_ALPHA_FOR_PIXEL \
{ \
    qColorRgba = rspqImage->GetPixelRgba(iX,iRevY); \
    if ( fAlphaValue != 0.0f ) \
    { \
        for (int iC = 0; iC < 3; iC++) \
        { \
            qColorRgba[iC] /= fAlphaValue; \
            if ( qColorRgba[iC] > 1.0f ) \
                qColorRgba[iC] = 1.0f; \
        } \
    } \
    else \
    { \
        for (int iC = 0; iC < 3; iC++) \
            qColorRgba[iC] = 1.0f; \
    } \
    qColorRgba.A() = fAlphaValue; \
    rspqImage->SetPixelRgba(qColorRgba,iX,iRevY); \
}
//------------------------------------------------------------------------------------------------------------------



