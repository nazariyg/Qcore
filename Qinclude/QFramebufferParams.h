#ifndef QFRAMEBUFFERPARAMS_H
#define QFRAMEBUFFERPARAMS_H

#include "QSystem.h"

namespace Q
{

class FramebufferParams
{
public:
    enum ColorModeType
    {
        CMT_COLOR_MODE_RGBA,
        CMT_COLOR_MODE_INDEX
    };

    enum ColorDepthType
    {
        CDT_COLOR_DEPTH_16,
        CDT_COLOR_DEPTH_24,
        CDT_COLOR_DEPTH_32
    };

    enum BufferingType
    {
        BT_BUFFERING_SINGLE,
        BT_BUFFERING_DOUBLE
    };

    enum DepthType
    {
        DT_DEPTH_NONE,
        DT_DEPTH_16,
        DT_DEPTH_24,
        DT_DEPTH_32
    };

    enum StencilType
    {
        ST_STENCIL_NONE,
        ST_STENCIL_8
    };

    enum MultisampleType
    {
        MT_MULTISAMPLE_NONE,
        MT_MULTISAMPLE_2,
        MT_MULTISAMPLE_4
    };

    // construction
    FramebufferParams ()
        :
        ColorMode(CMT_COLOR_MODE_RGBA),
        ColorDepth(CDT_COLOR_DEPTH_32),
        Buffering(BT_BUFFERING_DOUBLE),
        Depth(DT_DEPTH_24),
        Stencil(ST_STENCIL_8),
        Multisample(MT_MULTISAMPLE_NONE)
    {
    };

    ColorModeType ColorMode;
    ColorDepthType ColorDepth;
    BufferingType Buffering;
    DepthType Depth;
    StencilType Stencil;
    MultisampleType Multisample;
};

}

#endif



