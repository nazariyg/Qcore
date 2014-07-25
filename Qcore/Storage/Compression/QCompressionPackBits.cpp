#include "QCompressionPackBits.h"
#include "QArray.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
void CompressionPackBits::Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    rqNewBytes.RemoveAll();

    Array<char> qNewBytesBuffer;
    char cByte0, cByte1, cPrevByte;
    bool bLastRunWasLiteral = false;
    int iBytesLeft, iRunLength, iLastLiteralRunLength, iLastLiteralCounterPos, iCodedByteQuantity = 0;
    for (/**/; /**/; /**/)
    {
        iBytesLeft = iByteQuantity - iCodedByteQuantity;
        if ( iBytesLeft == 0 )
            break;
        if ( iBytesLeft == 1 )
        {
            if ( bLastRunWasLiteral && iLastLiteralRunLength < 128 )
                qNewBytesBuffer[iLastLiteralCounterPos] = iLastLiteralRunLength;
            else
                qNewBytesBuffer.Push(0);
            qNewBytesBuffer.Push(acByte[iCodedByteQuantity]);
            break;
        }

        cByte0 = acByte[iCodedByteQuantity+0];
        cByte1 = acByte[iCodedByteQuantity+1];
        iRunLength = 2;
        if ( cByte0 != cByte1 )
        {
            // literal run
            cPrevByte = cByte1;
            for (int i = iCodedByteQuantity+2; i < iByteQuantity; i++)
            {
                if ( acByte[i] == cPrevByte )
                {
                    iRunLength--;
                    break;
                }
                if ( ++iRunLength == 128 )
                    break;
                cPrevByte = acByte[i];
            }
            if ( bLastRunWasLiteral && iLastLiteralRunLength + iRunLength <= 128 )
            {
                // join two literal runs
                qNewBytesBuffer[iLastLiteralCounterPos] += iRunLength;
                for (int i = 0; i < iRunLength; i++)
                    qNewBytesBuffer.Push(acByte[iCodedByteQuantity++]);
                iLastLiteralRunLength += iRunLength;
            }
            else
            {
                // new literal run
                iLastLiteralCounterPos = qNewBytesBuffer.GetQuantity();
                qNewBytesBuffer.Push(iRunLength-1);
                for (int i = 0; i < iRunLength; i++)
                    qNewBytesBuffer.Push(acByte[iCodedByteQuantity++]);
                iLastLiteralRunLength = iRunLength;
                bLastRunWasLiteral = true;
            }
        }
        else
        {
            // replicate run (at first glance)
            for (int i = iCodedByteQuantity+2; i < iByteQuantity; i++)
            {
                if ( acByte[i] != cByte0 )
                    break;
                if ( ++iRunLength == 128 )
                    break;
            }
            if ( iRunLength == 2 && bLastRunWasLiteral && iLastLiteralRunLength + 2 <= 128 )
            {
                // use the previous literal run instead
                qNewBytesBuffer[iLastLiteralCounterPos] = iLastLiteralRunLength + 1;
                for (int i = 0; i < 2; i++)
                    qNewBytesBuffer.Push(cByte0);
                iLastLiteralRunLength += 2;
                iCodedByteQuantity += 2;
            }
            else
            {
                qNewBytesBuffer.Push(1-iRunLength);
                qNewBytesBuffer.Push(cByte0);
                iCodedByteQuantity += iRunLength;
                bLastRunWasLiteral = false;
            }
        }
    }
    rqNewBytes = qNewBytesBuffer;
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionPackBits::Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    rqNewBytes.RemoveAll();

    Array<char> qNewBytesBuffer;
    int iValue, iRepl;
    for (int i0 = 0; i0 < iByteQuantity; i0++)
    {
        iValue = acByte[i0];
        if ( iValue >= 0 )
        {
            // literal run
            for (int i1 = 0; i1 <= iValue; i1++)
            {
                if ( ++i0 == iByteQuantity )
                {
                    assert( false );
                    return false;
                }
                qNewBytesBuffer.Push(acByte[i0]);
            }
        }
        else if ( iValue != -128 )
        {
            // replicate run
            iRepl = 1 - iValue;
            if ( ++i0 == iByteQuantity )
            {
                assert( false );
                return false;
            }
            iValue = acByte[i0];
            for (int i1 = 0; i1 < iRepl; i1++)
                qNewBytesBuffer.Push(iValue);
        }
    }
    rqNewBytes = qNewBytesBuffer;
    return true;
}
//------------------------------------------------------------------------------------------------------------------



