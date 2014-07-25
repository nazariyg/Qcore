#ifdef _WIN32

#include "QWglExtensions.h"

//------------------------------------------------------------------------------------------------------------------
void* GetFunctionPointer (const char* acFunction)
{
    return (void*)wglGetProcAddress(acFunction);
}
//------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_buffer_region;
int QGLNumFunctions_WGL_ARB_buffer_region;
#ifdef QGL_DEFINED_WGL_ARB_buffer_region
pfn_wglCreateBufferRegionARB QGLwglCreateBufferRegionARB = 0;
pfn_wglDeleteBufferRegionARB QGLwglDeleteBufferRegionARB = 0;
pfn_wglSaveBufferRegionARB QGLwglSaveBufferRegionARB = 0;
pfn_wglRestoreBufferRegionARB QGLwglRestoreBufferRegionARB = 0;

static void Init_WGL_ARB_buffer_region ()
{
    QGLNumFunctions_WGL_ARB_buffer_region = 0;

    QGLwglCreateBufferRegionARB = (pfn_wglCreateBufferRegionARB)GetFunctionPointer("wglCreateBufferRegionARB");
    if ( QGLwglCreateBufferRegionARB )
        ++QGLNumFunctions_WGL_ARB_buffer_region;

    QGLwglDeleteBufferRegionARB = (pfn_wglDeleteBufferRegionARB)GetFunctionPointer("wglDeleteBufferRegionARB");
    if ( QGLwglDeleteBufferRegionARB )
        ++QGLNumFunctions_WGL_ARB_buffer_region;

    QGLwglSaveBufferRegionARB = (pfn_wglSaveBufferRegionARB)GetFunctionPointer("wglSaveBufferRegionARB");
    if ( QGLwglSaveBufferRegionARB )
        ++QGLNumFunctions_WGL_ARB_buffer_region;

    QGLwglRestoreBufferRegionARB = (pfn_wglRestoreBufferRegionARB)GetFunctionPointer("wglRestoreBufferRegionARB");
    if ( QGLwglRestoreBufferRegionARB )
        ++QGLNumFunctions_WGL_ARB_buffer_region;

    if ( QGLNumFunctions_WGL_ARB_buffer_region == 4 )
        QGLExists_WGL_ARB_buffer_region = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_buffer_region = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_buffer_region ()
{
    QGLNumFunctions_WGL_ARB_buffer_region = 0;
    QGLExists_WGL_ARB_buffer_region = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_multisample;
int QGLNumFunctions_WGL_ARB_multisample;
#ifdef QGL_DEFINED_WGL_ARB_multisample

static void Init_WGL_ARB_multisample ()
{
    QGLNumFunctions_WGL_ARB_multisample = 0;

    if ( QGLNumFunctions_WGL_ARB_multisample == 0 )
        QGLExists_WGL_ARB_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_multisample ()
{
    QGLNumFunctions_WGL_ARB_multisample = 0;
    QGLExists_WGL_ARB_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_extensions_string;
int QGLNumFunctions_WGL_ARB_extensions_string;
#ifdef QGL_DEFINED_WGL_ARB_extensions_string
pfn_wglGetExtensionsStringARB QGLwglGetExtensionsStringARB = 0;

static void Init_WGL_ARB_extensions_string ()
{
    QGLNumFunctions_WGL_ARB_extensions_string = 0;

    QGLwglGetExtensionsStringARB = (pfn_wglGetExtensionsStringARB)GetFunctionPointer("wglGetExtensionsStringARB");
    if ( QGLwglGetExtensionsStringARB )
        ++QGLNumFunctions_WGL_ARB_extensions_string;

    if ( QGLNumFunctions_WGL_ARB_extensions_string == 1 )
        QGLExists_WGL_ARB_extensions_string = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_extensions_string = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_extensions_string ()
{
    QGLNumFunctions_WGL_ARB_extensions_string = 0;
    QGLExists_WGL_ARB_extensions_string = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_pixel_format;
int QGLNumFunctions_WGL_ARB_pixel_format;
#ifdef QGL_DEFINED_WGL_ARB_pixel_format
pfn_wglGetPixelFormatAttribivARB QGLwglGetPixelFormatAttribivARB = 0;
pfn_wglGetPixelFormatAttribfvARB QGLwglGetPixelFormatAttribfvARB = 0;
pfn_wglChoosePixelFormatARB QGLwglChoosePixelFormatARB = 0;

static void Init_WGL_ARB_pixel_format ()
{
    QGLNumFunctions_WGL_ARB_pixel_format = 0;

    QGLwglGetPixelFormatAttribivARB = (pfn_wglGetPixelFormatAttribivARB)GetFunctionPointer("wglGetPixelFormatAttribivARB");
    if ( QGLwglGetPixelFormatAttribivARB )
        ++QGLNumFunctions_WGL_ARB_pixel_format;

    QGLwglGetPixelFormatAttribfvARB = (pfn_wglGetPixelFormatAttribfvARB)GetFunctionPointer("wglGetPixelFormatAttribfvARB");
    if ( QGLwglGetPixelFormatAttribfvARB )
        ++QGLNumFunctions_WGL_ARB_pixel_format;

    QGLwglChoosePixelFormatARB = (pfn_wglChoosePixelFormatARB)GetFunctionPointer("wglChoosePixelFormatARB");
    if ( QGLwglChoosePixelFormatARB )
        ++QGLNumFunctions_WGL_ARB_pixel_format;

    if ( QGLNumFunctions_WGL_ARB_pixel_format == 3 )
        QGLExists_WGL_ARB_pixel_format = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_pixel_format = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_pixel_format ()
{
    QGLNumFunctions_WGL_ARB_pixel_format = 0;
    QGLExists_WGL_ARB_pixel_format = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_make_current_read;
int QGLNumFunctions_WGL_ARB_make_current_read;
#ifdef QGL_DEFINED_WGL_ARB_make_current_read
pfn_wglMakeContextCurrentARB QGLwglMakeContextCurrentARB = 0;
pfn_wglGetCurrentReadDCARB QGLwglGetCurrentReadDCARB = 0;

static void Init_WGL_ARB_make_current_read ()
{
    QGLNumFunctions_WGL_ARB_make_current_read = 0;

    QGLwglMakeContextCurrentARB = (pfn_wglMakeContextCurrentARB)GetFunctionPointer("wglMakeContextCurrentARB");
    if ( QGLwglMakeContextCurrentARB )
        ++QGLNumFunctions_WGL_ARB_make_current_read;

    QGLwglGetCurrentReadDCARB = (pfn_wglGetCurrentReadDCARB)GetFunctionPointer("wglGetCurrentReadDCARB");
    if ( QGLwglGetCurrentReadDCARB )
        ++QGLNumFunctions_WGL_ARB_make_current_read;

    if ( QGLNumFunctions_WGL_ARB_make_current_read == 2 )
        QGLExists_WGL_ARB_make_current_read = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_make_current_read = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_make_current_read ()
{
    QGLNumFunctions_WGL_ARB_make_current_read = 0;
    QGLExists_WGL_ARB_make_current_read = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_pbuffer;
int QGLNumFunctions_WGL_ARB_pbuffer;
#ifdef QGL_DEFINED_WGL_ARB_pbuffer
pfn_wglCreatePbufferARB QGLwglCreatePbufferARB = 0;
pfn_wglGetPbufferDCARB QGLwglGetPbufferDCARB = 0;
pfn_wglReleasePbufferDCARB QGLwglReleasePbufferDCARB = 0;
pfn_wglDestroyPbufferARB QGLwglDestroyPbufferARB = 0;
pfn_wglQueryPbufferARB QGLwglQueryPbufferARB = 0;

static void Init_WGL_ARB_pbuffer ()
{
    QGLNumFunctions_WGL_ARB_pbuffer = 0;

    QGLwglCreatePbufferARB = (pfn_wglCreatePbufferARB)GetFunctionPointer("wglCreatePbufferARB");
    if ( QGLwglCreatePbufferARB )
        ++QGLNumFunctions_WGL_ARB_pbuffer;

    QGLwglGetPbufferDCARB = (pfn_wglGetPbufferDCARB)GetFunctionPointer("wglGetPbufferDCARB");
    if ( QGLwglGetPbufferDCARB )
        ++QGLNumFunctions_WGL_ARB_pbuffer;

    QGLwglReleasePbufferDCARB = (pfn_wglReleasePbufferDCARB)GetFunctionPointer("wglReleasePbufferDCARB");
    if ( QGLwglReleasePbufferDCARB )
        ++QGLNumFunctions_WGL_ARB_pbuffer;

    QGLwglDestroyPbufferARB = (pfn_wglDestroyPbufferARB)GetFunctionPointer("wglDestroyPbufferARB");
    if ( QGLwglDestroyPbufferARB )
        ++QGLNumFunctions_WGL_ARB_pbuffer;

    QGLwglQueryPbufferARB = (pfn_wglQueryPbufferARB)GetFunctionPointer("wglQueryPbufferARB");
    if ( QGLwglQueryPbufferARB )
        ++QGLNumFunctions_WGL_ARB_pbuffer;

    if ( QGLNumFunctions_WGL_ARB_pbuffer == 5 )
        QGLExists_WGL_ARB_pbuffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_pbuffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_pbuffer ()
{
    QGLNumFunctions_WGL_ARB_pbuffer = 0;
    QGLExists_WGL_ARB_pbuffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_render_texture;
int QGLNumFunctions_WGL_ARB_render_texture;
#ifdef QGL_DEFINED_WGL_ARB_render_texture
pfn_wglBindTexImageARB QGLwglBindTexImageARB = 0;
pfn_wglReleaseTexImageARB QGLwglReleaseTexImageARB = 0;
pfn_wglSetPbufferAttribARB QGLwglSetPbufferAttribARB = 0;

static void Init_WGL_ARB_render_texture ()
{
    QGLNumFunctions_WGL_ARB_render_texture = 0;

    QGLwglBindTexImageARB = (pfn_wglBindTexImageARB)GetFunctionPointer("wglBindTexImageARB");
    if ( QGLwglBindTexImageARB )
        ++QGLNumFunctions_WGL_ARB_render_texture;

    QGLwglReleaseTexImageARB = (pfn_wglReleaseTexImageARB)GetFunctionPointer("wglReleaseTexImageARB");
    if ( QGLwglReleaseTexImageARB )
        ++QGLNumFunctions_WGL_ARB_render_texture;

    QGLwglSetPbufferAttribARB = (pfn_wglSetPbufferAttribARB)GetFunctionPointer("wglSetPbufferAttribARB");
    if ( QGLwglSetPbufferAttribARB )
        ++QGLNumFunctions_WGL_ARB_render_texture;

    if ( QGLNumFunctions_WGL_ARB_render_texture == 3 )
        QGLExists_WGL_ARB_render_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_render_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_render_texture ()
{
    QGLNumFunctions_WGL_ARB_render_texture = 0;
    QGLExists_WGL_ARB_render_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ARB_pixel_format_float;
int QGLNumFunctions_WGL_ARB_pixel_format_float;
#ifdef QGL_DEFINED_WGL_ARB_pixel_format_float

static void Init_WGL_ARB_pixel_format_float ()
{
    QGLNumFunctions_WGL_ARB_pixel_format_float = 0;

    if ( QGLNumFunctions_WGL_ARB_pixel_format_float == 0 )
        QGLExists_WGL_ARB_pixel_format_float = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ARB_pixel_format_float = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ARB_pixel_format_float ()
{
    QGLNumFunctions_WGL_ARB_pixel_format_float = 0;
    QGLExists_WGL_ARB_pixel_format_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_make_current_read;
int QGLNumFunctions_WGL_EXT_make_current_read;
#ifdef QGL_DEFINED_WGL_EXT_make_current_read
pfn_wglMakeContextCurrentEXT QGLwglMakeContextCurrentEXT = 0;
pfn_wglGetCurrentReadDCEXT QGLwglGetCurrentReadDCEXT = 0;

static void Init_WGL_EXT_make_current_read ()
{
    QGLNumFunctions_WGL_EXT_make_current_read = 0;

    QGLwglMakeContextCurrentEXT = (pfn_wglMakeContextCurrentEXT)GetFunctionPointer("wglMakeContextCurrentEXT");
    if ( QGLwglMakeContextCurrentEXT )
        ++QGLNumFunctions_WGL_EXT_make_current_read;

    QGLwglGetCurrentReadDCEXT = (pfn_wglGetCurrentReadDCEXT)GetFunctionPointer("wglGetCurrentReadDCEXT");
    if ( QGLwglGetCurrentReadDCEXT )
        ++QGLNumFunctions_WGL_EXT_make_current_read;

    if ( QGLNumFunctions_WGL_EXT_make_current_read == 2 )
        QGLExists_WGL_EXT_make_current_read = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_make_current_read = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_make_current_read ()
{
    QGLNumFunctions_WGL_EXT_make_current_read = 0;
    QGLExists_WGL_EXT_make_current_read = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_pixel_format;
int QGLNumFunctions_WGL_EXT_pixel_format;
#ifdef QGL_DEFINED_WGL_EXT_pixel_format
pfn_wglGetPixelFormatAttribivEXT QGLwglGetPixelFormatAttribivEXT = 0;
pfn_wglGetPixelFormatAttribfvEXT QGLwglGetPixelFormatAttribfvEXT = 0;
pfn_wglChoosePixelFormatEXT QGLwglChoosePixelFormatEXT = 0;

static void Init_WGL_EXT_pixel_format ()
{
    QGLNumFunctions_WGL_EXT_pixel_format = 0;

    QGLwglGetPixelFormatAttribivEXT = (pfn_wglGetPixelFormatAttribivEXT)GetFunctionPointer("wglGetPixelFormatAttribivEXT");
    if ( QGLwglGetPixelFormatAttribivEXT )
        ++QGLNumFunctions_WGL_EXT_pixel_format;

    QGLwglGetPixelFormatAttribfvEXT = (pfn_wglGetPixelFormatAttribfvEXT)GetFunctionPointer("wglGetPixelFormatAttribfvEXT");
    if ( QGLwglGetPixelFormatAttribfvEXT )
        ++QGLNumFunctions_WGL_EXT_pixel_format;

    QGLwglChoosePixelFormatEXT = (pfn_wglChoosePixelFormatEXT)GetFunctionPointer("wglChoosePixelFormatEXT");
    if ( QGLwglChoosePixelFormatEXT )
        ++QGLNumFunctions_WGL_EXT_pixel_format;

    if ( QGLNumFunctions_WGL_EXT_pixel_format == 3 )
        QGLExists_WGL_EXT_pixel_format = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_pixel_format = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_pixel_format ()
{
    QGLNumFunctions_WGL_EXT_pixel_format = 0;
    QGLExists_WGL_EXT_pixel_format = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_pbuffer;
int QGLNumFunctions_WGL_EXT_pbuffer;
#ifdef QGL_DEFINED_WGL_EXT_pbuffer
pfn_wglCreatePbufferEXT QGLwglCreatePbufferEXT = 0;
pfn_wglGetPbufferDCEXT QGLwglGetPbufferDCEXT = 0;
pfn_wglReleasePbufferDCEXT QGLwglReleasePbufferDCEXT = 0;
pfn_wglDestroyPbufferEXT QGLwglDestroyPbufferEXT = 0;
pfn_wglQueryPbufferEXT QGLwglQueryPbufferEXT = 0;

static void Init_WGL_EXT_pbuffer ()
{
    QGLNumFunctions_WGL_EXT_pbuffer = 0;

    QGLwglCreatePbufferEXT = (pfn_wglCreatePbufferEXT)GetFunctionPointer("wglCreatePbufferEXT");
    if ( QGLwglCreatePbufferEXT )
        ++QGLNumFunctions_WGL_EXT_pbuffer;

    QGLwglGetPbufferDCEXT = (pfn_wglGetPbufferDCEXT)GetFunctionPointer("wglGetPbufferDCEXT");
    if ( QGLwglGetPbufferDCEXT )
        ++QGLNumFunctions_WGL_EXT_pbuffer;

    QGLwglReleasePbufferDCEXT = (pfn_wglReleasePbufferDCEXT)GetFunctionPointer("wglReleasePbufferDCEXT");
    if ( QGLwglReleasePbufferDCEXT )
        ++QGLNumFunctions_WGL_EXT_pbuffer;

    QGLwglDestroyPbufferEXT = (pfn_wglDestroyPbufferEXT)GetFunctionPointer("wglDestroyPbufferEXT");
    if ( QGLwglDestroyPbufferEXT )
        ++QGLNumFunctions_WGL_EXT_pbuffer;

    QGLwglQueryPbufferEXT = (pfn_wglQueryPbufferEXT)GetFunctionPointer("wglQueryPbufferEXT");
    if ( QGLwglQueryPbufferEXT )
        ++QGLNumFunctions_WGL_EXT_pbuffer;

    if ( QGLNumFunctions_WGL_EXT_pbuffer == 5 )
        QGLExists_WGL_EXT_pbuffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_pbuffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_pbuffer ()
{
    QGLNumFunctions_WGL_EXT_pbuffer = 0;
    QGLExists_WGL_EXT_pbuffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_depth_float;
int QGLNumFunctions_WGL_EXT_depth_float;
#ifdef QGL_DEFINED_WGL_EXT_depth_float

static void Init_WGL_EXT_depth_float ()
{
    QGLNumFunctions_WGL_EXT_depth_float = 0;

    if ( QGLNumFunctions_WGL_EXT_depth_float == 0 )
        QGLExists_WGL_EXT_depth_float = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_depth_float = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_depth_float ()
{
    QGLNumFunctions_WGL_EXT_depth_float = 0;
    QGLExists_WGL_EXT_depth_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_3DFX_multisample;
int QGLNumFunctions_WGL_3DFX_multisample;
#ifdef QGL_DEFINED_WGL_3DFX_multisample

static void Init_WGL_3DFX_multisample ()
{
    QGLNumFunctions_WGL_3DFX_multisample = 0;

    if ( QGLNumFunctions_WGL_3DFX_multisample == 0 )
        QGLExists_WGL_3DFX_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_3DFX_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_3DFX_multisample ()
{
    QGLNumFunctions_WGL_3DFX_multisample = 0;
    QGLExists_WGL_3DFX_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_multisample;
int QGLNumFunctions_WGL_EXT_multisample;
#ifdef QGL_DEFINED_WGL_EXT_multisample

static void Init_WGL_EXT_multisample ()
{
    QGLNumFunctions_WGL_EXT_multisample = 0;

    if ( QGLNumFunctions_WGL_EXT_multisample == 0 )
        QGLExists_WGL_EXT_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_multisample ()
{
    QGLNumFunctions_WGL_EXT_multisample = 0;
    QGLExists_WGL_EXT_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_digital_video_control;
int QGLNumFunctions_WGL_I3D_digital_video_control;
#ifdef QGL_DEFINED_WGL_I3D_digital_video_control
pfn_wglGetDigitalVideoParametersI3D QGLwglGetDigitalVideoParametersI3D = 0;
pfn_wglSetDigitalVideoParametersI3D QGLwglSetDigitalVideoParametersI3D = 0;

static void Init_WGL_I3D_digital_video_control ()
{
    QGLNumFunctions_WGL_I3D_digital_video_control = 0;

    QGLwglGetDigitalVideoParametersI3D = (pfn_wglGetDigitalVideoParametersI3D)GetFunctionPointer("wglGetDigitalVideoParametersI3D");
    if ( QGLwglGetDigitalVideoParametersI3D )
        ++QGLNumFunctions_WGL_I3D_digital_video_control;

    QGLwglSetDigitalVideoParametersI3D = (pfn_wglSetDigitalVideoParametersI3D)GetFunctionPointer("wglSetDigitalVideoParametersI3D");
    if ( QGLwglSetDigitalVideoParametersI3D )
        ++QGLNumFunctions_WGL_I3D_digital_video_control;

    if ( QGLNumFunctions_WGL_I3D_digital_video_control == 2 )
        QGLExists_WGL_I3D_digital_video_control = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_digital_video_control = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_digital_video_control ()
{
    QGLNumFunctions_WGL_I3D_digital_video_control = 0;
    QGLExists_WGL_I3D_digital_video_control = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_gamma;
int QGLNumFunctions_WGL_I3D_gamma;
#ifdef QGL_DEFINED_WGL_I3D_gamma
pfn_wglGetGammaTableParametersI3D QGLwglGetGammaTableParametersI3D = 0;
pfn_wglSetGammaTableParametersI3D QGLwglSetGammaTableParametersI3D = 0;
pfn_wglGetGammaTableI3D QGLwglGetGammaTableI3D = 0;
pfn_wglSetGammaTableI3D QGLwglSetGammaTableI3D = 0;

static void Init_WGL_I3D_gamma ()
{
    QGLNumFunctions_WGL_I3D_gamma = 0;

    QGLwglGetGammaTableParametersI3D = (pfn_wglGetGammaTableParametersI3D)GetFunctionPointer("wglGetGammaTableParametersI3D");
    if ( QGLwglGetGammaTableParametersI3D )
        ++QGLNumFunctions_WGL_I3D_gamma;

    QGLwglSetGammaTableParametersI3D = (pfn_wglSetGammaTableParametersI3D)GetFunctionPointer("wglSetGammaTableParametersI3D");
    if ( QGLwglSetGammaTableParametersI3D )
        ++QGLNumFunctions_WGL_I3D_gamma;

    QGLwglGetGammaTableI3D = (pfn_wglGetGammaTableI3D)GetFunctionPointer("wglGetGammaTableI3D");
    if ( QGLwglGetGammaTableI3D )
        ++QGLNumFunctions_WGL_I3D_gamma;

    QGLwglSetGammaTableI3D = (pfn_wglSetGammaTableI3D)GetFunctionPointer("wglSetGammaTableI3D");
    if ( QGLwglSetGammaTableI3D )
        ++QGLNumFunctions_WGL_I3D_gamma;

    if ( QGLNumFunctions_WGL_I3D_gamma == 4 )
        QGLExists_WGL_I3D_gamma = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_gamma = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_gamma ()
{
    QGLNumFunctions_WGL_I3D_gamma = 0;
    QGLExists_WGL_I3D_gamma = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_genlock;
int QGLNumFunctions_WGL_I3D_genlock;
#ifdef QGL_DEFINED_WGL_I3D_genlock
pfn_wglEnableGenlockI3D QGLwglEnableGenlockI3D = 0;
pfn_wglDisableGenlockI3D QGLwglDisableGenlockI3D = 0;
pfn_wglIsEnabledGenlockI3D QGLwglIsEnabledGenlockI3D = 0;
pfn_wglGenlockSourceI3D QGLwglGenlockSourceI3D = 0;
pfn_wglGetGenlockSourceI3D QGLwglGetGenlockSourceI3D = 0;
pfn_wglGenlockSourceEdgeI3D QGLwglGenlockSourceEdgeI3D = 0;
pfn_wglGetGenlockSourceEdgeI3D QGLwglGetGenlockSourceEdgeI3D = 0;
pfn_wglGenlockSampleRateI3D QGLwglGenlockSampleRateI3D = 0;
pfn_wglGetGenlockSampleRateI3D QGLwglGetGenlockSampleRateI3D = 0;
pfn_wglGenlockSourceDelayI3D QGLwglGenlockSourceDelayI3D = 0;
pfn_wglGetGenlockSourceDelayI3D QGLwglGetGenlockSourceDelayI3D = 0;
pfn_wglQueryGenlockMaxSourceDelayI3D QGLwglQueryGenlockMaxSourceDelayI3D = 0;

static void Init_WGL_I3D_genlock ()
{
    QGLNumFunctions_WGL_I3D_genlock = 0;

    QGLwglEnableGenlockI3D = (pfn_wglEnableGenlockI3D)GetFunctionPointer("wglEnableGenlockI3D");
    if ( QGLwglEnableGenlockI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglDisableGenlockI3D = (pfn_wglDisableGenlockI3D)GetFunctionPointer("wglDisableGenlockI3D");
    if ( QGLwglDisableGenlockI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglIsEnabledGenlockI3D = (pfn_wglIsEnabledGenlockI3D)GetFunctionPointer("wglIsEnabledGenlockI3D");
    if ( QGLwglIsEnabledGenlockI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGenlockSourceI3D = (pfn_wglGenlockSourceI3D)GetFunctionPointer("wglGenlockSourceI3D");
    if ( QGLwglGenlockSourceI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGetGenlockSourceI3D = (pfn_wglGetGenlockSourceI3D)GetFunctionPointer("wglGetGenlockSourceI3D");
    if ( QGLwglGetGenlockSourceI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGenlockSourceEdgeI3D = (pfn_wglGenlockSourceEdgeI3D)GetFunctionPointer("wglGenlockSourceEdgeI3D");
    if ( QGLwglGenlockSourceEdgeI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGetGenlockSourceEdgeI3D = (pfn_wglGetGenlockSourceEdgeI3D)GetFunctionPointer("wglGetGenlockSourceEdgeI3D");
    if ( QGLwglGetGenlockSourceEdgeI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGenlockSampleRateI3D = (pfn_wglGenlockSampleRateI3D)GetFunctionPointer("wglGenlockSampleRateI3D");
    if ( QGLwglGenlockSampleRateI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGetGenlockSampleRateI3D = (pfn_wglGetGenlockSampleRateI3D)GetFunctionPointer("wglGetGenlockSampleRateI3D");
    if ( QGLwglGetGenlockSampleRateI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGenlockSourceDelayI3D = (pfn_wglGenlockSourceDelayI3D)GetFunctionPointer("wglGenlockSourceDelayI3D");
    if ( QGLwglGenlockSourceDelayI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglGetGenlockSourceDelayI3D = (pfn_wglGetGenlockSourceDelayI3D)GetFunctionPointer("wglGetGenlockSourceDelayI3D");
    if ( QGLwglGetGenlockSourceDelayI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    QGLwglQueryGenlockMaxSourceDelayI3D = (pfn_wglQueryGenlockMaxSourceDelayI3D)GetFunctionPointer("wglQueryGenlockMaxSourceDelayI3D");
    if ( QGLwglQueryGenlockMaxSourceDelayI3D )
        ++QGLNumFunctions_WGL_I3D_genlock;

    if ( QGLNumFunctions_WGL_I3D_genlock == 12 )
        QGLExists_WGL_I3D_genlock = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_genlock = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_genlock ()
{
    QGLNumFunctions_WGL_I3D_genlock = 0;
    QGLExists_WGL_I3D_genlock = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_image_buffer;
int QGLNumFunctions_WGL_I3D_image_buffer;
#ifdef QGL_DEFINED_WGL_I3D_image_buffer
pfn_wglCreateImageBufferI3D QGLwglCreateImageBufferI3D = 0;
pfn_wglDestroyImageBufferI3D QGLwglDestroyImageBufferI3D = 0;
pfn_wglAssociateImageBufferEventsI3D QGLwglAssociateImageBufferEventsI3D = 0;
pfn_wglReleaseImageBufferEventsI3D QGLwglReleaseImageBufferEventsI3D = 0;

static void Init_WGL_I3D_image_buffer ()
{
    QGLNumFunctions_WGL_I3D_image_buffer = 0;

    QGLwglCreateImageBufferI3D = (pfn_wglCreateImageBufferI3D)GetFunctionPointer("wglCreateImageBufferI3D");
    if ( QGLwglCreateImageBufferI3D )
        ++QGLNumFunctions_WGL_I3D_image_buffer;

    QGLwglDestroyImageBufferI3D = (pfn_wglDestroyImageBufferI3D)GetFunctionPointer("wglDestroyImageBufferI3D");
    if ( QGLwglDestroyImageBufferI3D )
        ++QGLNumFunctions_WGL_I3D_image_buffer;

    QGLwglAssociateImageBufferEventsI3D = (pfn_wglAssociateImageBufferEventsI3D)GetFunctionPointer("wglAssociateImageBufferEventsI3D");
    if ( QGLwglAssociateImageBufferEventsI3D )
        ++QGLNumFunctions_WGL_I3D_image_buffer;

    QGLwglReleaseImageBufferEventsI3D = (pfn_wglReleaseImageBufferEventsI3D)GetFunctionPointer("wglReleaseImageBufferEventsI3D");
    if ( QGLwglReleaseImageBufferEventsI3D )
        ++QGLNumFunctions_WGL_I3D_image_buffer;

    if ( QGLNumFunctions_WGL_I3D_image_buffer == 4 )
        QGLExists_WGL_I3D_image_buffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_image_buffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_image_buffer ()
{
    QGLNumFunctions_WGL_I3D_image_buffer = 0;
    QGLExists_WGL_I3D_image_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_swap_frame_lock;
int QGLNumFunctions_WGL_I3D_swap_frame_lock;
#ifdef QGL_DEFINED_WGL_I3D_swap_frame_lock
pfn_wglEnableFrameLockI3D QGLwglEnableFrameLockI3D = 0;
pfn_wglDisableFrameLockI3D QGLwglDisableFrameLockI3D = 0;
pfn_wglIsEnabledFrameLockI3D QGLwglIsEnabledFrameLockI3D = 0;
pfn_wglQueryFrameLockMasterI3D QGLwglQueryFrameLockMasterI3D = 0;

static void Init_WGL_I3D_swap_frame_lock ()
{
    QGLNumFunctions_WGL_I3D_swap_frame_lock = 0;

    QGLwglEnableFrameLockI3D = (pfn_wglEnableFrameLockI3D)GetFunctionPointer("wglEnableFrameLockI3D");
    if ( QGLwglEnableFrameLockI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_lock;

    QGLwglDisableFrameLockI3D = (pfn_wglDisableFrameLockI3D)GetFunctionPointer("wglDisableFrameLockI3D");
    if ( QGLwglDisableFrameLockI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_lock;

    QGLwglIsEnabledFrameLockI3D = (pfn_wglIsEnabledFrameLockI3D)GetFunctionPointer("wglIsEnabledFrameLockI3D");
    if ( QGLwglIsEnabledFrameLockI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_lock;

    QGLwglQueryFrameLockMasterI3D = (pfn_wglQueryFrameLockMasterI3D)GetFunctionPointer("wglQueryFrameLockMasterI3D");
    if ( QGLwglQueryFrameLockMasterI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_lock;

    if ( QGLNumFunctions_WGL_I3D_swap_frame_lock == 4 )
        QGLExists_WGL_I3D_swap_frame_lock = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_swap_frame_lock = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_swap_frame_lock ()
{
    QGLNumFunctions_WGL_I3D_swap_frame_lock = 0;
    QGLExists_WGL_I3D_swap_frame_lock = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_I3D_swap_frame_usage;
int QGLNumFunctions_WGL_I3D_swap_frame_usage;
#ifdef QGL_DEFINED_WGL_I3D_swap_frame_usage
pfn_wglGetFrameUsageI3D QGLwglGetFrameUsageI3D = 0;
pfn_wglBeginFrameTrackingI3D QGLwglBeginFrameTrackingI3D = 0;
pfn_wglEndFrameTrackingI3D QGLwglEndFrameTrackingI3D = 0;
pfn_wglQueryFrameTrackingI3D QGLwglQueryFrameTrackingI3D = 0;

static void Init_WGL_I3D_swap_frame_usage ()
{
    QGLNumFunctions_WGL_I3D_swap_frame_usage = 0;

    QGLwglGetFrameUsageI3D = (pfn_wglGetFrameUsageI3D)GetFunctionPointer("wglGetFrameUsageI3D");
    if ( QGLwglGetFrameUsageI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_usage;

    QGLwglBeginFrameTrackingI3D = (pfn_wglBeginFrameTrackingI3D)GetFunctionPointer("wglBeginFrameTrackingI3D");
    if ( QGLwglBeginFrameTrackingI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_usage;

    QGLwglEndFrameTrackingI3D = (pfn_wglEndFrameTrackingI3D)GetFunctionPointer("wglEndFrameTrackingI3D");
    if ( QGLwglEndFrameTrackingI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_usage;

    QGLwglQueryFrameTrackingI3D = (pfn_wglQueryFrameTrackingI3D)GetFunctionPointer("wglQueryFrameTrackingI3D");
    if ( QGLwglQueryFrameTrackingI3D )
        ++QGLNumFunctions_WGL_I3D_swap_frame_usage;

    if ( QGLNumFunctions_WGL_I3D_swap_frame_usage == 4 )
        QGLExists_WGL_I3D_swap_frame_usage = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_I3D_swap_frame_usage = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_I3D_swap_frame_usage ()
{
    QGLNumFunctions_WGL_I3D_swap_frame_usage = 0;
    QGLExists_WGL_I3D_swap_frame_usage = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_NV_render_depth_texture;
int QGLNumFunctions_WGL_NV_render_depth_texture;
#ifdef QGL_DEFINED_WGL_NV_render_depth_texture

static void Init_WGL_NV_render_depth_texture ()
{
    QGLNumFunctions_WGL_NV_render_depth_texture = 0;

    if ( QGLNumFunctions_WGL_NV_render_depth_texture == 0 )
        QGLExists_WGL_NV_render_depth_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_NV_render_depth_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_NV_render_depth_texture ()
{
    QGLNumFunctions_WGL_NV_render_depth_texture = 0;
    QGLExists_WGL_NV_render_depth_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_NV_render_texture_rectangle;
int QGLNumFunctions_WGL_NV_render_texture_rectangle;
#ifdef QGL_DEFINED_WGL_NV_render_texture_rectangle

static void Init_WGL_NV_render_texture_rectangle ()
{
    QGLNumFunctions_WGL_NV_render_texture_rectangle = 0;

    if ( QGLNumFunctions_WGL_NV_render_texture_rectangle == 0 )
        QGLExists_WGL_NV_render_texture_rectangle = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_NV_render_texture_rectangle = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_NV_render_texture_rectangle ()
{
    QGLNumFunctions_WGL_NV_render_texture_rectangle = 0;
    QGLExists_WGL_NV_render_texture_rectangle = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_ATI_pixel_format_float;
int QGLNumFunctions_WGL_ATI_pixel_format_float;
#ifdef QGL_DEFINED_WGL_ATI_pixel_format_float

static void Init_WGL_ATI_pixel_format_float ()
{
    QGLNumFunctions_WGL_ATI_pixel_format_float = 0;

    if ( QGLNumFunctions_WGL_ATI_pixel_format_float == 0 )
        QGLExists_WGL_ATI_pixel_format_float = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_ATI_pixel_format_float = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_ATI_pixel_format_float ()
{
    QGLNumFunctions_WGL_ATI_pixel_format_float = 0;
    QGLExists_WGL_ATI_pixel_format_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_NV_float_buffer;
int QGLNumFunctions_WGL_NV_float_buffer;
#ifdef QGL_DEFINED_WGL_NV_float_buffer

static void Init_WGL_NV_float_buffer ()
{
    QGLNumFunctions_WGL_NV_float_buffer = 0;

    if ( QGLNumFunctions_WGL_NV_float_buffer == 0 )
        QGLExists_WGL_NV_float_buffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_NV_float_buffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_NV_float_buffer ()
{
    QGLNumFunctions_WGL_NV_float_buffer = 0;
    QGLExists_WGL_NV_float_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_display_color_table;
int QGLNumFunctions_WGL_EXT_display_color_table;
#ifdef QGL_DEFINED_WGL_EXT_display_color_table
pfn_wglCreateDisplayColorTableEXT QGLwglCreateDisplayColorTableEXT = 0;
pfn_wglLoadDisplayColorTableEXT QGLwglLoadDisplayColorTableEXT = 0;
pfn_wglBindDisplayColorTableEXT QGLwglBindDisplayColorTableEXT = 0;
pfn_wglDestroyDisplayColorTableEXT QGLwglDestroyDisplayColorTableEXT = 0;

static void Init_WGL_EXT_display_color_table ()
{
    QGLNumFunctions_WGL_EXT_display_color_table = 0;

    QGLwglCreateDisplayColorTableEXT = (pfn_wglCreateDisplayColorTableEXT)GetFunctionPointer("wglCreateDisplayColorTableEXT");
    if ( QGLwglCreateDisplayColorTableEXT )
        ++QGLNumFunctions_WGL_EXT_display_color_table;

    QGLwglLoadDisplayColorTableEXT = (pfn_wglLoadDisplayColorTableEXT)GetFunctionPointer("wglLoadDisplayColorTableEXT");
    if ( QGLwglLoadDisplayColorTableEXT )
        ++QGLNumFunctions_WGL_EXT_display_color_table;

    QGLwglBindDisplayColorTableEXT = (pfn_wglBindDisplayColorTableEXT)GetFunctionPointer("wglBindDisplayColorTableEXT");
    if ( QGLwglBindDisplayColorTableEXT )
        ++QGLNumFunctions_WGL_EXT_display_color_table;

    QGLwglDestroyDisplayColorTableEXT = (pfn_wglDestroyDisplayColorTableEXT)GetFunctionPointer("wglDestroyDisplayColorTableEXT");
    if ( QGLwglDestroyDisplayColorTableEXT )
        ++QGLNumFunctions_WGL_EXT_display_color_table;

    if ( QGLNumFunctions_WGL_EXT_display_color_table == 4 )
        QGLExists_WGL_EXT_display_color_table = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_display_color_table = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_display_color_table ()
{
    QGLNumFunctions_WGL_EXT_display_color_table = 0;
    QGLExists_WGL_EXT_display_color_table = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_extensions_string;
int QGLNumFunctions_WGL_EXT_extensions_string;
#ifdef QGL_DEFINED_WGL_EXT_extensions_string
pfn_wglGetExtensionsStringEXT QGLwglGetExtensionsStringEXT = 0;

static void Init_WGL_EXT_extensions_string ()
{
    QGLNumFunctions_WGL_EXT_extensions_string = 0;

    QGLwglGetExtensionsStringEXT = (pfn_wglGetExtensionsStringEXT)GetFunctionPointer("wglGetExtensionsStringEXT");
    if ( QGLwglGetExtensionsStringEXT )
        ++QGLNumFunctions_WGL_EXT_extensions_string;

    if ( QGLNumFunctions_WGL_EXT_extensions_string == 1 )
        QGLExists_WGL_EXT_extensions_string = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_extensions_string = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_extensions_string ()
{
    QGLNumFunctions_WGL_EXT_extensions_string = 0;
    QGLExists_WGL_EXT_extensions_string = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_EXT_swap_control;
int QGLNumFunctions_WGL_EXT_swap_control;
#ifdef QGL_DEFINED_WGL_EXT_swap_control
pfn_wglSwapIntervalEXT QGLwglSwapIntervalEXT = 0;
pfn_wglGetSwapIntervalEXT QGLwglGetSwapIntervalEXT = 0;

static void Init_WGL_EXT_swap_control ()
{
    QGLNumFunctions_WGL_EXT_swap_control = 0;

    QGLwglSwapIntervalEXT = (pfn_wglSwapIntervalEXT)GetFunctionPointer("wglSwapIntervalEXT");
    if ( QGLwglSwapIntervalEXT )
        ++QGLNumFunctions_WGL_EXT_swap_control;

    QGLwglGetSwapIntervalEXT = (pfn_wglGetSwapIntervalEXT)GetFunctionPointer("wglGetSwapIntervalEXT");
    if ( QGLwglGetSwapIntervalEXT )
        ++QGLNumFunctions_WGL_EXT_swap_control;

    if ( QGLNumFunctions_WGL_EXT_swap_control == 2 )
        QGLExists_WGL_EXT_swap_control = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_EXT_swap_control = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_EXT_swap_control ()
{
    QGLNumFunctions_WGL_EXT_swap_control = 0;
    QGLExists_WGL_EXT_swap_control = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_NV_vertex_array_range;
int QGLNumFunctions_WGL_NV_vertex_array_range;
#ifdef QGL_DEFINED_WGL_NV_vertex_array_range
pfn_wglAllocateMemoryNV QGLwglAllocateMemoryNV = 0;
pfn_wglFreeMemoryNV QGLwglFreeMemoryNV = 0;

static void Init_WGL_NV_vertex_array_range ()
{
    QGLNumFunctions_WGL_NV_vertex_array_range = 0;

    QGLwglAllocateMemoryNV = (pfn_wglAllocateMemoryNV)GetFunctionPointer("wglAllocateMemoryNV");
    if ( QGLwglAllocateMemoryNV )
        ++QGLNumFunctions_WGL_NV_vertex_array_range;

    QGLwglFreeMemoryNV = (pfn_wglFreeMemoryNV)GetFunctionPointer("wglFreeMemoryNV");
    if ( QGLwglFreeMemoryNV )
        ++QGLNumFunctions_WGL_NV_vertex_array_range;

    if ( QGLNumFunctions_WGL_NV_vertex_array_range == 2 )
        QGLExists_WGL_NV_vertex_array_range = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_NV_vertex_array_range = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_NV_vertex_array_range ()
{
    QGLNumFunctions_WGL_NV_vertex_array_range = 0;
    QGLExists_WGL_NV_vertex_array_range = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_WGL_OML_sync_control;
int QGLNumFunctions_WGL_OML_sync_control;
#ifdef QGL_DEFINED_WGL_OML_sync_control
pfn_wglGetSyncValuesOML QGLwglGetSyncValuesOML = 0;
pfn_wglGetMscRateOML QGLwglGetMscRateOML = 0;
pfn_wglSwapBuffersMscOML QGLwglSwapBuffersMscOML = 0;
pfn_wglSwapLayerBuffersMscOML QGLwglSwapLayerBuffersMscOML = 0;
pfn_wglWaitForMscOML QGLwglWaitForMscOML = 0;
pfn_wglWaitForSbcOML QGLwglWaitForSbcOML = 0;

static void Init_WGL_OML_sync_control ()
{
    QGLNumFunctions_WGL_OML_sync_control = 0;

    QGLwglGetSyncValuesOML = (pfn_wglGetSyncValuesOML)GetFunctionPointer("wglGetSyncValuesOML");
    if ( QGLwglGetSyncValuesOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    QGLwglGetMscRateOML = (pfn_wglGetMscRateOML)GetFunctionPointer("wglGetMscRateOML");
    if ( QGLwglGetMscRateOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    QGLwglSwapBuffersMscOML = (pfn_wglSwapBuffersMscOML)GetFunctionPointer("wglSwapBuffersMscOML");
    if ( QGLwglSwapBuffersMscOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    QGLwglSwapLayerBuffersMscOML = (pfn_wglSwapLayerBuffersMscOML)GetFunctionPointer("wglSwapLayerBuffersMscOML");
    if ( QGLwglSwapLayerBuffersMscOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    QGLwglWaitForMscOML = (pfn_wglWaitForMscOML)GetFunctionPointer("wglWaitForMscOML");
    if ( QGLwglWaitForMscOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    QGLwglWaitForSbcOML = (pfn_wglWaitForSbcOML)GetFunctionPointer("wglWaitForSbcOML");
    if ( QGLwglWaitForSbcOML )
        ++QGLNumFunctions_WGL_OML_sync_control;

    if ( QGLNumFunctions_WGL_OML_sync_control == 6 )
        QGLExists_WGL_OML_sync_control = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_WGL_OML_sync_control = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_WGL_OML_sync_control ()
{
    QGLNumFunctions_WGL_OML_sync_control = 0;
    QGLExists_WGL_OML_sync_control = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
void InitializeWgl ()
{
    Init_WGL_ARB_buffer_region();
    Init_WGL_ARB_multisample();
    Init_WGL_ARB_extensions_string();
    Init_WGL_ARB_pixel_format();
    Init_WGL_ARB_make_current_read();
    Init_WGL_ARB_pbuffer();
    Init_WGL_ARB_render_texture();
    Init_WGL_ARB_pixel_format_float();
    Init_WGL_EXT_make_current_read();
    Init_WGL_EXT_pixel_format();
    Init_WGL_EXT_pbuffer();
    Init_WGL_EXT_depth_float();
    Init_WGL_3DFX_multisample();
    Init_WGL_EXT_multisample();
    Init_WGL_I3D_digital_video_control();
    Init_WGL_I3D_gamma();
    Init_WGL_I3D_genlock();
    Init_WGL_I3D_image_buffer();
    Init_WGL_I3D_swap_frame_lock();
    Init_WGL_I3D_swap_frame_usage();
    Init_WGL_NV_render_depth_texture();
    Init_WGL_NV_render_texture_rectangle();
    Init_WGL_ATI_pixel_format_float();
    Init_WGL_NV_float_buffer();
    Init_WGL_EXT_display_color_table();
    Init_WGL_EXT_extensions_string();
    Init_WGL_EXT_swap_control();
    Init_WGL_NV_vertex_array_range();
    Init_WGL_OML_sync_control();
}
//------------------------------------------------------------------------------------------------------------------
#endif



