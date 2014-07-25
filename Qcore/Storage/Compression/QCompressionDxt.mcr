//------------------------------------------------------------------------------------------------------------------
#define ASSIGN_CONTROL_VARIABLES \
{ \
    fInsetFactor = s_fInsetFactor; \
    fSearchRadToInsLen = s_fSearchRadToInsLen; \
    iRadTestPointDensity = s_iRadTestPointDensity; \
    bDoCrawl = s_bDoCrawl; \
    iCrawlMaxRepeats = s_iCrawlMaxRepeats; \
    bDoLightNearbySearch = s_bDoLightNearbySearch; \
    iLightNearbySearchMaxRepeats = s_iLightNearbySearchMaxRepeats; \
    iLightNearbySearchStages = s_iLightNearbySearchStages; \
    for (int iC0 = 0; iC0 < s_iLightNearbySearchStages; iC0++) \
    { \
        for (int iC1 = 0; iC1 < 3; iC1++) \
            aafLightNearbySearchDev[iC0][iC1] = s_aafLightNearbySearchDev[iC0][iC1]; \
    } \
    for (int iC = 0; iC < 3; iC++) \
        afLightNearbySearchMaxDev[iC] = s_afLightNearbySearchMaxDev[iC]; \
    bDoHeavyNearbySearch = s_bDoHeavyNearbySearch; \
    iHeavyNearbySearchMaxRepeats = s_iHeavyNearbySearchMaxRepeats; \
    iHeavyNearbySearchStages = s_iHeavyNearbySearchStages; \
    for (int iC0 = 0; iC0 < s_iHeavyNearbySearchStages; iC0++) \
    { \
        for (int iC1 = 0; iC1 < 3; iC1++) \
            aafHeavyNearbySearchDev[iC0][iC1] = s_aafHeavyNearbySearchDev[iC0][iC1]; \
    } \
    for (int iC = 0; iC < 3; iC++) \
        afHeavyNearbySearchMaxDev[iC] = s_afHeavyNearbySearchMaxDev[iC]; \
}
//------------------------------------------------------------------------------------------------------------------
#define GEN_SEL_888_COLORS \
{ \
    aaucSel888Color[0][0] = aauc888Color[0][0]; \
    aaucSel888Color[0][1] = aauc888Color[0][1]; \
    aaucSel888Color[0][2] = aauc888Color[0][2]; \
    aaucSel888Color[1][0] = aauc888Color[1][0]; \
    aaucSel888Color[1][1] = aauc888Color[1][1]; \
    aaucSel888Color[1][2] = aauc888Color[1][2]; \
    if ( !bDxt1a ) \
    { \
        aaucSel888Color[2][0] = (2*aauc888Color[0][0] + aauc888Color[1][0] + 1)/3; \
        aaucSel888Color[2][1] = (2*aauc888Color[0][1] + aauc888Color[1][1] + 1)/3; \
        aaucSel888Color[2][2] = (2*aauc888Color[0][2] + aauc888Color[1][2] + 1)/3; \
        aaucSel888Color[3][0] = (aauc888Color[0][0] + 2*aauc888Color[1][0] + 1)/3; \
        aaucSel888Color[3][1] = (aauc888Color[0][1] + 2*aauc888Color[1][1] + 1)/3; \
        aaucSel888Color[3][2] = (aauc888Color[0][2] + 2*aauc888Color[1][2] + 1)/3; \
    } \
    else \
    { \
        aaucSel888Color[2][0] = (aauc888Color[0][0] + aauc888Color[1][0])/2; \
        aaucSel888Color[2][1] = (aauc888Color[0][1] + aauc888Color[1][1])/2; \
        aaucSel888Color[2][2] = (aauc888Color[0][2] + aauc888Color[1][2])/2; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



