#include "QDiscreteCosineTransformi.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
void DiscreteCosineTransformi::Apply8x8ForwardOrthogonal2d (short* asCoefElement)
{
    int iVar00, iVar01, iVar02, iVar03, iVar04, iVar05, iVar06, iVar07, iVar08, iVar09, iVar10, iVar11, iVar12,
        iVar13, iVar14, iVar15, iVar16;
    short* psCoefEl;

    psCoefEl = asCoefElement;
    for (int i = 7; i >= 0; i--)
    {
        iVar00 = psCoefEl[0] + psCoefEl[7];
        iVar07 = psCoefEl[0] - psCoefEl[7];
        iVar01 = psCoefEl[1] + psCoefEl[6];
        iVar06 = psCoefEl[1] - psCoefEl[6];
        iVar02 = psCoefEl[2] + psCoefEl[5];
        iVar05 = psCoefEl[2] - psCoefEl[5];
        iVar03 = psCoefEl[3] + psCoefEl[4];
        iVar04 = psCoefEl[3] - psCoefEl[4];
        iVar08 = iVar00 + iVar03;
        iVar11 = iVar00 - iVar03;
        iVar09 = iVar01 + iVar02;
        iVar10 = iVar01 - iVar02;
        psCoefEl[0] = (iVar08 + iVar09) << 2;
        psCoefEl[4] = (iVar08 - iVar09) << 2;
        iVar12 = (iVar10 + iVar11)*4433;
        psCoefEl[2] = (iVar12 + iVar11* 6270 + 1024) >> 11;
        psCoefEl[6] = (iVar12 - iVar10*15137 + 1024) >> 11;
        iVar12 = iVar04 + iVar07;
        iVar13 = iVar05 + iVar06;
        iVar14 = iVar04 + iVar06;
        iVar15 = iVar05 + iVar07;
        iVar16 = (iVar14 + iVar15)*9633;
        iVar04 *=   2446;
        iVar05 *=  16819;
        iVar06 *=  25172;
        iVar07 *=  12299;
        iVar12 *=  -7373;
        iVar13 *= -20995;
        iVar14 *= -16069;
        iVar15 *=  -3196;
        iVar14 += iVar16;
        iVar15 += iVar16;
        psCoefEl[7] = (iVar04 + iVar12 + iVar14 + 1024) >> 11;
        psCoefEl[5] = (iVar05 + iVar13 + iVar15 + 1024) >> 11;
        psCoefEl[3] = (iVar06 + iVar13 + iVar14 + 1024) >> 11;
        psCoefEl[1] = (iVar07 + iVar12 + iVar15 + 1024) >> 11;

        psCoefEl += 8;
    }

    psCoefEl = asCoefElement;
    for (int i = 7; i >= 0; i--)
    {
        iVar00 = psCoefEl[ 0] + psCoefEl[56];
        iVar07 = psCoefEl[ 0] - psCoefEl[56];
        iVar01 = psCoefEl[ 8] + psCoefEl[48];
        iVar06 = psCoefEl[ 8] - psCoefEl[48];
        iVar02 = psCoefEl[16] + psCoefEl[40];
        iVar05 = psCoefEl[16] - psCoefEl[40];
        iVar03 = psCoefEl[24] + psCoefEl[32];
        iVar04 = psCoefEl[24] - psCoefEl[32];
        iVar08 = iVar00 + iVar03;
        iVar11 = iVar00 - iVar03;
        iVar09 = iVar01 + iVar02;
        iVar10 = iVar01 - iVar02;
        psCoefEl[ 0] = (iVar08 + iVar09 + 2) >> 2;
        psCoefEl[32] = (iVar08 - iVar09 + 2) >> 2;
        iVar12 = (iVar10 + iVar11)*4433;
        psCoefEl[16] = (iVar12 + iVar11* 6270 + 16384) >> 15;
        psCoefEl[48] = (iVar12 - iVar10*15137 + 16384) >> 15;
        iVar12 = iVar04 + iVar07;
        iVar13 = iVar05 + iVar06;
        iVar14 = iVar04 + iVar06;
        iVar15 = iVar05 + iVar07;
        iVar16 = (iVar14 + iVar15)*9633;
        iVar04 *=   2446;
        iVar05 *=  16819;
        iVar06 *=  25172;
        iVar07 *=  12299;
        iVar12 *=  -7373;
        iVar13 *= -20995;
        iVar14 *= -16069;
        iVar15 *=  -3196;
        iVar14 += iVar16;
        iVar15 += iVar16;
        psCoefEl[56] = (iVar04 + iVar12 + iVar14 + 16384) >> 15;
        psCoefEl[40] = (iVar05 + iVar13 + iVar15 + 16384) >> 15;
        psCoefEl[24] = (iVar06 + iVar13 + iVar14 + 16384) >> 15;
        psCoefEl[ 8] = (iVar07 + iVar12 + iVar15 + 16384) >> 15;

        psCoefEl++;
    }

    for (int i = 0; i < 64; i++)
        asCoefElement[i] >>= 3;
}
//------------------------------------------------------------------------------------------------------------------
void DiscreteCosineTransformi::Get8x8InverseOrthogonal2dWithDequantizationAndClamping (char* acCoefElement,
    const short* asFreqElement, const int* aiQuantTable)
{
    short asBlockBuffer[64];
    unsigned char aucBlockBuffer[64];
    short* psBB = asBlockBuffer;
    const short* psFreqEl = asFreqElement;

    int iVar00, iVar01, iVar02, iVar03, iVar04, iVar05, iVar06, iVar07, iVar08, iVar09, iVar10, iVar11, iVar12, iDc;
    unsigned char ucDc;
    unsigned char* pucCoefEl;
    for (int i = 8; i > 0; i--)
    {
        if ( psFreqEl[ 8] == 0 && psFreqEl[16] == 0 && psFreqEl[24] == 0 && psFreqEl[32] == 0
        &&   psFreqEl[40] == 0 && psFreqEl[48] == 0 && psFreqEl[56] == 0 )
        {
            iDc = psFreqEl[0]*aiQuantTable[0] << 2;
            psBB[ 0] = iDc;
            psBB[ 8] = iDc;
            psBB[16] = iDc;
            psBB[24] = iDc;
            psBB[32] = iDc;
            psBB[40] = iDc;
            psBB[48] = iDc;
            psBB[56] = iDc;

            psFreqEl++;
            aiQuantTable++;
            psBB++;
            continue;
        }

        iVar09 = psFreqEl[16]*aiQuantTable[16];
        iVar10 = psFreqEl[48]*aiQuantTable[48];
        iVar08 = (iVar09 + iVar10)*4433;
        iVar02 = iVar08 - iVar10*15137;
        iVar03 = iVar08 + iVar09* 6270;
        iVar09 = psFreqEl[ 0]*aiQuantTable[ 0];
        iVar10 = psFreqEl[32]*aiQuantTable[32];
        iVar00 = (iVar09 + iVar10) << 13;
        iVar01 = (iVar09 - iVar10) << 13;
        iVar04 = iVar00 + iVar03;
        iVar07 = iVar00 - iVar03;
        iVar05 = iVar01 + iVar02;
        iVar06 = iVar01 - iVar02;
        iVar00 = psFreqEl[56]*aiQuantTable[56];
        iVar01 = psFreqEl[40]*aiQuantTable[40];
        iVar02 = psFreqEl[24]*aiQuantTable[24];
        iVar03 = psFreqEl[ 8]*aiQuantTable[ 8];
        iVar08 = iVar00 + iVar03;
        iVar09 = iVar01 + iVar02;
        iVar10 = iVar00 + iVar02;
        iVar11 = iVar01 + iVar03;
        iVar12 = (iVar10 + iVar11)*9633;
        iVar00 *=   2446;
        iVar01 *=  16819;
        iVar02 *=  25172;
        iVar03 *=  12299;
        iVar08 *=  -7373;
        iVar09 *= -20995;
        iVar10 *= -16069;
        iVar11 *=  -3196;
        iVar10 += iVar12;
        iVar11 += iVar12;
        iVar00 += iVar08 + iVar10;
        iVar01 += iVar09 + iVar11;
        iVar02 += iVar09 + iVar10;
        iVar03 += iVar08 + iVar11;
        psBB[ 0] = (iVar04 + iVar03 + 1024) >> 11;
        psBB[ 8] = (iVar05 + iVar02 + 1024) >> 11;
        psBB[16] = (iVar06 + iVar01 + 1024) >> 11;
        psBB[24] = (iVar07 + iVar00 + 1024) >> 11;
        psBB[32] = (iVar07 - iVar00 + 1024) >> 11;
        psBB[40] = (iVar06 - iVar01 + 1024) >> 11;
        psBB[48] = (iVar05 - iVar02 + 1024) >> 11;
        psBB[56] = (iVar04 - iVar03 + 1024) >> 11;

        psFreqEl++;
        aiQuantTable++;
        psBB++;
    }
    pucCoefEl = aucBlockBuffer;
    psBB = asBlockBuffer;

    for (int i = 0; i < 8; i++)
    {
        if ( psBB[1] == 0 && psBB[2] == 0 && psBB[3] == 0 && psBB[4] == 0
        &&   psBB[5] == 0 && psBB[6] == 0 && psBB[7] == 0 )
        {
            ucDc = GetClampedToUChar((psBB[0]+16) >> 5);
            pucCoefEl[0] = ucDc;
            pucCoefEl[1] = ucDc;
            pucCoefEl[2] = ucDc;
            pucCoefEl[3] = ucDc;
            pucCoefEl[4] = ucDc;
            pucCoefEl[5] = ucDc;
            pucCoefEl[6] = ucDc;
            pucCoefEl[7] = ucDc;

            pucCoefEl += 8;
            psBB += 8;
            continue;
        }

        iVar09 = psBB[2];
        iVar10 = psBB[6];
        iVar08 = (iVar09 + iVar10)*4433;
        iVar02 = iVar08 - iVar10*15137;
        iVar03 = iVar08 + iVar09* 6270;
        iVar00 = (psBB[0] + psBB[4]) << 13;
        iVar01 = (psBB[0] - psBB[4]) << 13;
        iVar04 = iVar00 + iVar03;
        iVar07 = iVar00 - iVar03;
        iVar05 = iVar01 + iVar02;
        iVar06 = iVar01 - iVar02;
        iVar00 = psBB[7];
        iVar01 = psBB[5];
        iVar02 = psBB[3];
        iVar03 = psBB[1];
        iVar08 = iVar00 + iVar03;
        iVar09 = iVar01 + iVar02;
        iVar10 = iVar00 + iVar02;
        iVar11 = iVar01 + iVar03;
        iVar12 = (iVar10 + iVar11)*9633;
        iVar00 *=   2446;
        iVar01 *=  16819;
        iVar02 *=  25172;
        iVar03 *=  12299;
        iVar08 *=  -7373;
        iVar09 *= -20995;
        iVar10 *= -16069;
        iVar11 *=  -3196;
        iVar10 += iVar12;
        iVar11 += iVar12;
        iVar00 += iVar08 + iVar10;
        iVar01 += iVar09 + iVar11;
        iVar02 += iVar09 + iVar10;
        iVar03 += iVar08 + iVar11;
        pucCoefEl[0] = GetClampedToUChar((iVar04+iVar03+131072) >> 18);
        pucCoefEl[1] = GetClampedToUChar((iVar05+iVar02+131072) >> 18);
        pucCoefEl[2] = GetClampedToUChar((iVar06+iVar01+131072) >> 18);
        pucCoefEl[3] = GetClampedToUChar((iVar07+iVar00+131072) >> 18);
        pucCoefEl[4] = GetClampedToUChar((iVar07-iVar00+131072) >> 18);
        pucCoefEl[5] = GetClampedToUChar((iVar06-iVar01+131072) >> 18);
        pucCoefEl[6] = GetClampedToUChar((iVar05-iVar02+131072) >> 18);
        pucCoefEl[7] = GetClampedToUChar((iVar04-iVar03+131072) >> 18);

        pucCoefEl += 8;
        psBB += 8;
    }

    for (int i = 0; i < 64; i++)
        acCoefElement[i] = aucBlockBuffer[i] - 128;
}
//------------------------------------------------------------------------------------------------------------------



