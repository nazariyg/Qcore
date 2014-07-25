#include "QRasterImageSequence.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
RasterImageSequence::RasterImageSequence (RasterImage::ImageType eFrameImageType, int iScreenWidth,
    int iScreenHeight, int iFrameQuantity)
    :
    m_eFrameImageType(eFrameImageType),
    m_iScreenWidth(iScreenWidth),
    m_iScreenHeight(iScreenHeight),
    m_qFrames(iFrameQuantity)
{
    assert( iScreenWidth >= 0 && iScreenHeight >= 0 );
}
//------------------------------------------------------------------------------------------------------------------



