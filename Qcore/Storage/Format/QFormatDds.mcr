//------------------------------------------------------------------------------------------------------------------
#define CALC_BITMASK_PARAMS(mask,shift,length) \
{ \
    unsigned int uiTBitMask = mask; \
 \
    shift = 0; \
    while ( (uiTBitMask & 1) == 0 ) \
    { \
        uiTBitMask >>= 1; \
        if ( ++shift == 32 ) \
            break; \
    } \
 \
    length = 0; \
    while ( (uiTBitMask & 1) == 1 ) \
    { \
        uiTBitMask >>= 1; \
        if ( ++length == 32 ) \
            break; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



