#include "QColorModelConversion.h"
using namespace Q;

// support for GetConvertedToYcc
const float ColorModelConversion::RgbToYccRInY ( 0.299000f);
const float ColorModelConversion::RgbToYccGInY ( 0.587000f);
const float ColorModelConversion::RgbToYccBInY ( 0.114000f);
const float ColorModelConversion::RgbToYccRInCb(-0.168736f);
const float ColorModelConversion::RgbToYccGInCb(-0.331264f);
const float ColorModelConversion::RgbToYccBInCb( 0.500000f);
const float ColorModelConversion::RgbToYccRInCr( 0.500000f);
const float ColorModelConversion::RgbToYccGInCr(-0.418688f);
const float ColorModelConversion::RgbToYccBInCr(-0.081312f);

// support for GetConvertedToRgb
const float ColorModelConversion::YccToRgbCrInR( 1.402000f);
const float ColorModelConversion::YccToRgbCbInG(-0.344136f);
const float ColorModelConversion::YccToRgbCrInG(-0.714136f);
const float ColorModelConversion::YccToRgbCbInB( 1.772000f);

// support for XYZ color model related conversions
const float ColorModelConversion::RgbToXyzDefaultGamma(2.200000f);
const float ColorModelConversion::XyzToRgbDefaultGamma(0.454545f);
const Matrix3f ColorModelConversion::RgbToXyzDefaultMatrix(
     0.412424f,  0.357579f,  0.180464f,
     0.212656f,  0.715158f,  0.072186f,
     0.019332f,  0.119193f,  0.950444f);
const Matrix3f ColorModelConversion::XyzToRgbDefaultMatrix(
     3.240710f, -1.537260f, -0.498571f,
    -0.969258f,  1.875990f,  0.041556f,
     0.055635f, -0.203996f,  1.057070f);

// support for L*a*b* color model related conversions
const float ColorModelConversion::DefaultXw(0.31273f);
const float ColorModelConversion::DefaultYw(0.32902f);

//------------------------------------------------------------------------------------------------------------------
ColorCmyk ColorModelConversion::GetConvertedToCmyk (const ColorRgb& rqColorRgb, bool bClampResult)
{
    float fPreC = 1.0f - rqColorRgb.R();
    float fPreM = 1.0f - rqColorRgb.G();
    float fPreY = 1.0f - rqColorRgb.B();
    float fK = Mathf::Min3(fPreC,fPreM,fPreY);
    if ( fK == 1.0f )
    {
        return ColorCmyk(0.0f,0.0f,0.0f,1.0f);
    }
    else
    {
        float fOMK = 1.0f - fK;
        ColorCmyk qColorCmyk(
            (fPreC-fK)/fOMK,
            (fPreM-fK)/fOMK,
            (fPreY-fK)/fOMK,
            fK);
        if ( bClampResult )
            qColorCmyk.Clamp();
        return qColorCmyk;
    }
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorModelConversion::GetConvertedToRgb (const ColorCmyk& rqColorCmyk, bool bClampResult)
{
    float fOMK = 1.0f - rqColorCmyk.K();
    ColorRgb qColorRgb(
        (1.0f-rqColorCmyk.C())*fOMK,
        (1.0f-rqColorCmyk.M())*fOMK,
        (1.0f-rqColorCmyk.Y())*fOMK);
    if ( bClampResult )
        qColorRgb.Clamp();
    return qColorRgb;
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc ColorModelConversion::GetConvertedToYcc (const ColorRgb& rqColorRgb, bool bClampResult)
{
    ColorYcc qColorYcc(
        RgbToYccRInY *rqColorRgb.R()+RgbToYccGInY *rqColorRgb.G()+RgbToYccBInY *rqColorRgb.B(),
        RgbToYccRInCb*rqColorRgb.R()+RgbToYccGInCb*rqColorRgb.G()+RgbToYccBInCb*rqColorRgb.B(),
        RgbToYccRInCr*rqColorRgb.R()+RgbToYccGInCr*rqColorRgb.G()+RgbToYccBInCr*rqColorRgb.B());
    if ( bClampResult )
        qColorYcc.Clamp();
    return qColorYcc;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorModelConversion::GetConvertedToRgb (const ColorYcc& rqColorYcc, bool bClampResult)
{
    ColorRgb qColorRgb(
        rqColorYcc.Y()+YccToRgbCrInR*rqColorYcc.Cr(),
        rqColorYcc.Y()+YccToRgbCbInG*rqColorYcc.Cb()+YccToRgbCrInG*rqColorYcc.Cr(),
        rqColorYcc.Y()+YccToRgbCbInB*rqColorYcc.Cb());
    if ( bClampResult )
        qColorRgb.Clamp();
    return qColorRgb;
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz ColorModelConversion::GetConvertedToXyz (const ColorRgb& rqColorRgb, bool bClampResult, float fGamma,
    const Matrix3f& rqMatrix)
{
    Vector3f qV(
        GetPowered(rqColorRgb.R(),fGamma),
        GetPowered(rqColorRgb.G(),fGamma),
        GetPowered(rqColorRgb.B(),fGamma));
    qV = rqMatrix*qV;
    ColorXyz qColorXyz(
        qV.X(),
        qV.Y(),
        qV.Z());
    if ( bClampResult )
        qColorXyz.Clamp();
    return qColorXyz;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorModelConversion::GetConvertedToRgb (const ColorXyz& rqColorXyz, bool bClampResult, float fGamma,
    const Matrix3f& rqMatrix)
{
    Vector3f qV(
        rqColorXyz.X(),
        rqColorXyz.Y(),
        rqColorXyz.Z());
    qV = rqMatrix*qV;
    ColorRgb qColorRgb(
        GetPowered(qV.X(),fGamma),
        GetPowered(qV.Y(),fGamma),
        GetPowered(qV.Z(),fGamma));
    if ( bClampResult )
        qColorRgb.Clamp();
    return qColorRgb;
}
//------------------------------------------------------------------------------------------------------------------
ColorLab ColorModelConversion::GetConvertedToLab (const ColorRgb& rqColorRgb, bool bClampResult, float fGamma,
    const Matrix3f& rqMatrix, float fXw, float fYw)
{
    ColorXyz qColorXyz = GetConvertedToXyz(rqColorRgb,bClampResult,fGamma,rqMatrix);
    float afFR[3] = {
        qColorXyz.X()*fYw/fXw,
        qColorXyz.Y(),
        qColorXyz.Z()*fYw/(1.0f - fXw - fYw)};
    for (int i = 0; i < 3; i++)
        afFR[i] = ( afFR[i] > 0.008856f ? GetPowered(afFR[i],0.333333f) : 7.787037f*afFR[i] + 0.137931f );
    ColorLab qColorLab(
        116.0f*afFR[1]-16.0f,
        500.0f*(afFR[0]-afFR[1]),
        200.0f*(afFR[1]-afFR[2]));
    if ( bClampResult )
        qColorLab.Clamp();
    return qColorLab;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorModelConversion::GetConvertedToRgb (const ColorLab& rqColorLab, bool bClampResult, float fGamma,
    const Matrix3f& rqMatrix, float fXw, float fYw)
{
    float fFY = (rqColorLab.L() + 16.0f)/116.0f;
    float fFX = fFY + rqColorLab.A()/500.0f;
    float fFZ = fFY - rqColorLab.B()/200.0f;
    float afD[3] = {fFX, fFY, fFZ};
    float afW[3] = {fXw/fYw, 1.0f, (1.0f - fXw - fYw)/fYw};
    for (int i = 0; i < 3; i++)
        afD[i] = ( afD[i] > 0.206897f ? afW[i]*GetPowered(afD[i],3.0f) : (afD[i] - 0.137931f)*0.128419f*afW[i] );
    ColorXyz qColorXyz(afD[0],afD[1],afD[2]);
    if ( bClampResult )
        qColorXyz.Clamp();
    return GetConvertedToRgb(qColorXyz,bClampResult,fGamma,rqMatrix);
}
//------------------------------------------------------------------------------------------------------------------
Matrix3f ColorModelConversion::GenerateRgbToXyzMatrix (
    float fXr, float fYr,
    float fXg, float fYg,
    float fXb, float fYb,
    float fXw, float fYw)
{
    float fVar00 = fYr*fXg;
    float fVar01 = fYr*fXb;
    float fVar02 = fYg*fXb;
    float fVar03 = fXg*fYb;
    float fVar04 = fXr*fYg;
    float fVar05 = fXr*fYb;
    float fVar06 = fYg*fXw;
    float fVar07 = fYb*fXw;
    float fVar08 = fXg*fYw;
    float fVar09 = fXb*fYw;
    float fVar10 = fXr*fYw;
    float fVar11 = fYr*fXw;
    float fInvDen = 1.0f/(fYw*(-fVar00 + fVar01 - fVar02 + fVar03 + fVar04 - fVar05));
    float fAr = (-fVar02 + fVar06 + fVar03 - fVar07 - fVar08 + fVar09)*fInvDen;
    float fAg = (-fVar05 + fVar10 + fVar07 + fVar01 - fVar09 - fVar11)*fInvDen;
    float fAb = (-fVar00 - fVar10 - fVar06 + fVar11 + fVar08 + fVar04)*fInvDen;
    return Matrix3f(
        fAr*fXr,fAg*fXg,fAb*fXb,
        fAr*fYr,fAg*fYg,fAb*fYb,
        fAr*(1.0f-fXr-fYr),fAg*(1.0f-fXg-fYg),fAb*(1.0f-fXb-fYb));
}
//------------------------------------------------------------------------------------------------------------------
Matrix3f ColorModelConversion::GenerateXyzToRgbMatrix (
    float fXr, float fYr,
    float fXg, float fYg,
    float fXb, float fYb,
    float fXw, float fYw)
{
    return GenerateRgbToXyzMatrix(fXr,fYr,fXg,fYg,fXb,fYb,fXw,fYw).GetInversed();
}
//------------------------------------------------------------------------------------------------------------------
float ColorModelConversion::GetPowered (float fBase, float fExponent)
{
    return ( fBase >= 0.0f ? Mathf::Pow(fBase,fExponent) : -Mathf::Pow(-fBase,fExponent) );
}
//------------------------------------------------------------------------------------------------------------------



