//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::SetSampleAlphaToCoverage (bool bSet)
{
    SampleAlphaToCoverage = bSet;
    SampleAlphaToCoverageSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::UnsetSampleAlphaToCoverage ()
{
    SampleAlphaToCoverageSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::SetSampleAlphaToOne (bool bSet)
{
    SampleAlphaToOne = bSet;
    SampleAlphaToOneSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::UnsetSampleAlphaToOne ()
{
    SampleAlphaToOneSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::SetSampleCoverage (float fValue, bool bInvert)
{
    SampleCoverageValue = fValue;
    SampleCoverageInvert = bInvert;
    SampleCoverageSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void MultisampleAntialiasingState::UnsetSampleCoverage ()
{
    SampleCoverageSet = false;
}
//------------------------------------------------------------------------------------------------------------------



