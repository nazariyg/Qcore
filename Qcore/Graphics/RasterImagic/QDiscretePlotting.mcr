//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_0 \
{ \
    iC = iCC + iCVar0; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE + iEVar0 - (iY << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = 0; \
            iOY = 1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_1 \
{ \
    iC = iCC + iCVar1; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE + iEVar1 - ((iX + iY + 1) << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = 1; \
            iOY = 1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_2 \
{ \
    iC = iCC + iCVar2; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE + iEVar2 - (iX << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = 1; \
            iOY = 0; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_3 \
{ \
    iC = iCC + iCVar3; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE + iEVar3 - ((iX - iY + 1) << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = 1; \
            iOY = -1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_4 \
{ \
    iC = iCC + iCVar4; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE - iEVar4 + (iY << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = 0; \
            iOY = -1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_5 \
{ \
    iC = iCC + iCVar5; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE - iEVar1 + ((iX + iY - 1) << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = -1; \
            iOY = -1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_6 \
{ \
    iC = iCC + iCVar6; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE - iEVar5 + (iX << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = -1; \
            iOY = 0; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROC_ARC_MOVE_7 \
{ \
    iC = iCC + iCVar7; \
    if ( iC >= 0 ) \
    { \
        iSE = iCE - iEVar3 + ((iX - iY - 1) << 2); \
        iE = iSE; \
        if ( iE < 0 ) \
            iE = -iE; \
        if ( iE < iMinE ) \
        { \
            iMinE = iE; \
            iVC = iC; \
            iVE = iSE; \
            iOX = -1; \
            iOY = 1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------



