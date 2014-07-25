#ifndef _WIN32

#include "QGlxExtensions.h"

//------------------------------------------------------------------------------------------------------------------
void* GetFunctionPointer (const char* acFunction)
{
    return (void*)(*glXGetProcAddressARB)(acFunction);
}
//------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_VERSION_1_0;
int QGLNumFunctions_GLX_VERSION_1_0;
#ifdef QGL_DEFINED_GLX_VERSION_1_0
static void Init_GLX_VERSION_1_0 ()
{
    QGLNumFunctions_GLX_VERSION_1_0 = 17;
    QGLExists_GLX_VERSION_1_0 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_VERSION_1_0 ()
{
    QGLNumFunctions_GLX_VERSION_1_0 = 0;
    QGLExists_GLX_VERSION_1_0 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_VERSION_1_1;
int QGLNumFunctions_GLX_VERSION_1_1;
#ifdef QGL_DEFINED_GLX_VERSION_1_1
static void Init_GLX_VERSION_1_1 ()
{
    QGLNumFunctions_GLX_VERSION_1_1 = 3;
    QGLExists_GLX_VERSION_1_1 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_VERSION_1_1 ()
{
    QGLNumFunctions_GLX_VERSION_1_1 = 0;
    QGLExists_GLX_VERSION_1_1 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_VERSION_1_2;
int QGLNumFunctions_GLX_VERSION_1_2;
#ifdef QGL_DEFINED_GLX_VERSION_1_2
pfn_glXGetCurrentDisplay QGLglXGetCurrentDisplay = 0;

static void Init_GLX_VERSION_1_2 ()
{
    QGLNumFunctions_GLX_VERSION_1_2 = 0;

    QGLglXGetCurrentDisplay = (pfn_glXGetCurrentDisplay)GetFunctionPointer("glXGetCurrentDisplay");
    if ( QGLglXGetCurrentDisplay )
        ++QGLNumFunctions_GLX_VERSION_1_2;

    if ( QGLNumFunctions_GLX_VERSION_1_2 == 1 )
        QGLExists_GLX_VERSION_1_2 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_VERSION_1_2 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_VERSION_1_2 ()
{
    QGLNumFunctions_GLX_VERSION_1_2 = 0;
    QGLExists_GLX_VERSION_1_2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_VERSION_1_3;
int QGLNumFunctions_GLX_VERSION_1_3;
#ifdef QGL_DEFINED_GLX_VERSION_1_3
pfn_glXGetFBConfigs QGLglXGetFBConfigs = 0;
pfn_glXChooseFBConfig QGLglXChooseFBConfig = 0;
pfn_glXGetFBConfigAttrib QGLglXGetFBConfigAttrib = 0;
pfn_glXGetVisualFromFBConfig QGLglXGetVisualFromFBConfig = 0;
pfn_glXCreateWindow QGLglXCreateWindow = 0;
pfn_glXDestroyWindow QGLglXDestroyWindow = 0;
pfn_glXCreatePixmap QGLglXCreatePixmap = 0;
pfn_glXDestroyPixmap QGLglXDestroyPixmap = 0;
pfn_glXCreatePbuffer QGLglXCreatePbuffer = 0;
pfn_glXDestroyPbuffer QGLglXDestroyPbuffer = 0;
pfn_glXQueryDrawable QGLglXQueryDrawable = 0;
pfn_glXCreateNewContext QGLglXCreateNewContext = 0;
pfn_glXMakeContextCurrent QGLglXMakeContextCurrent = 0;
pfn_glXGetCurrentReadDrawable QGLglXGetCurrentReadDrawable = 0;
pfn_glXQueryContext QGLglXQueryContext = 0;
pfn_glXSelectEvent QGLglXSelectEvent = 0;
pfn_glXGetSelectedEvent QGLglXGetSelectedEvent = 0;

static void Init_GLX_VERSION_1_3 ()
{
    QGLNumFunctions_GLX_VERSION_1_3 = 0;

    QGLglXGetFBConfigs = (pfn_glXGetFBConfigs)GetFunctionPointer("glXGetFBConfigs");
    if ( QGLglXGetFBConfigs )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXChooseFBConfig = (pfn_glXChooseFBConfig)GetFunctionPointer("glXChooseFBConfig");
    if ( QGLglXChooseFBConfig )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXGetFBConfigAttrib = (pfn_glXGetFBConfigAttrib)GetFunctionPointer("glXGetFBConfigAttrib");
    if ( QGLglXGetFBConfigAttrib )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXGetVisualFromFBConfig = (pfn_glXGetVisualFromFBConfig)GetFunctionPointer("glXGetVisualFromFBConfig");
    if ( QGLglXGetVisualFromFBConfig )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXCreateWindow = (pfn_glXCreateWindow)GetFunctionPointer("glXCreateWindow");
    if ( QGLglXCreateWindow )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXDestroyWindow = (pfn_glXDestroyWindow)GetFunctionPointer("glXDestroyWindow");
    if ( QGLglXDestroyWindow )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXCreatePixmap = (pfn_glXCreatePixmap)GetFunctionPointer("glXCreatePixmap");
    if ( QGLglXCreatePixmap )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXDestroyPixmap = (pfn_glXDestroyPixmap)GetFunctionPointer("glXDestroyPixmap");
    if ( QGLglXDestroyPixmap )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXCreatePbuffer = (pfn_glXCreatePbuffer)GetFunctionPointer("glXCreatePbuffer");
    if ( QGLglXCreatePbuffer )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXDestroyPbuffer = (pfn_glXDestroyPbuffer)GetFunctionPointer("glXDestroyPbuffer");
    if ( QGLglXDestroyPbuffer )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXQueryDrawable = (pfn_glXQueryDrawable)GetFunctionPointer("glXQueryDrawable");
    if ( QGLglXQueryDrawable )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXCreateNewContext = (pfn_glXCreateNewContext)GetFunctionPointer("glXCreateNewContext");
    if ( QGLglXCreateNewContext )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXMakeContextCurrent = (pfn_glXMakeContextCurrent)GetFunctionPointer("glXMakeContextCurrent");
    if ( QGLglXMakeContextCurrent )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXGetCurrentReadDrawable = (pfn_glXGetCurrentReadDrawable)GetFunctionPointer("glXGetCurrentReadDrawable");
    if ( QGLglXGetCurrentReadDrawable )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXQueryContext = (pfn_glXQueryContext)GetFunctionPointer("glXQueryContext");
    if ( QGLglXQueryContext )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXSelectEvent = (pfn_glXSelectEvent)GetFunctionPointer("glXSelectEvent");
    if ( QGLglXSelectEvent )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    QGLglXGetSelectedEvent = (pfn_glXGetSelectedEvent)GetFunctionPointer("glXGetSelectedEvent");
    if ( QGLglXGetSelectedEvent )
        ++QGLNumFunctions_GLX_VERSION_1_3;

    if ( QGLNumFunctions_GLX_VERSION_1_3 == 17 )
        QGLExists_GLX_VERSION_1_3 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_VERSION_1_3 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_VERSION_1_3 ()
{
    QGLNumFunctions_GLX_VERSION_1_3 = 0;
    QGLExists_GLX_VERSION_1_3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_VERSION_1_4;
int QGLNumFunctions_GLX_VERSION_1_4;
#ifdef QGL_DEFINED_GLX_VERSION_1_4
static void Init_GLX_VERSION_1_4 ()
{
    QGLNumFunctions_GLX_VERSION_1_4 = 0;
    QGLExists_GLX_VERSION_1_4 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_VERSION_1_4 ()
{
    QGLNumFunctions_GLX_VERSION_1_4 = 0;
    QGLExists_GLX_VERSION_1_4 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_ARB_get_proc_address;
int QGLNumFunctions_GLX_ARB_get_proc_address;
#ifdef QGL_DEFINED_GLX_ARB_get_proc_address
static void Init_GLX_ARB_get_proc_address ()
{
    QGLNumFunctions_GLX_ARB_get_proc_address = 0;
    QGLExists_GLX_ARB_get_proc_address = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_ARB_get_proc_address ()
{
    QGLNumFunctions_GLX_ARB_get_proc_address = 0;
    QGLExists_GLX_ARB_get_proc_address = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_ARB_multisample;
int QGLNumFunctions_GLX_ARB_multisample;
#ifdef QGL_DEFINED_GLX_ARB_multisample
static void Init_GLX_ARB_multisample ()
{
    QGLNumFunctions_GLX_ARB_multisample = 0;
    QGLExists_GLX_ARB_multisample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_ARB_multisample ()
{
    QGLNumFunctions_GLX_ARB_multisample = 0;
    QGLExists_GLX_ARB_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_ARB_fbconfig_float;
int QGLNumFunctions_GLX_ARB_fbconfig_float;
#ifdef QGL_DEFINED_GLX_ARB_fbconfig_float
static void Init_GLX_ARB_fbconfig_float ()
{
    QGLNumFunctions_GLX_ARB_fbconfig_float = 0;
    QGLExists_GLX_ARB_fbconfig_float = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_ARB_fbconfig_float ()
{
    QGLNumFunctions_GLX_ARB_fbconfig_float = 0;
    QGLExists_GLX_ARB_fbconfig_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIS_multisample;
int QGLNumFunctions_GLX_SGIS_multisample;
#ifdef QGL_DEFINED_GLX_SGIS_multisample
static void Init_GLX_SGIS_multisample ()
{
    QGLNumFunctions_GLX_SGIS_multisample = 0;
    QGLExists_GLX_SGIS_multisample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_SGIS_multisample ()
{
    QGLNumFunctions_GLX_SGIS_multisample = 0;
    QGLExists_GLX_SGIS_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_EXT_visual_info;
int QGLNumFunctions_GLX_EXT_visual_info;
#ifdef QGL_DEFINED_GLX_EXT_visual_info
static void Init_GLX_EXT_visual_info ()
{
    QGLNumFunctions_GLX_EXT_visual_info = 0;
    QGLExists_GLX_EXT_visual_info = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_EXT_visual_info ()
{
    QGLNumFunctions_GLX_EXT_visual_info = 0;
    QGLExists_GLX_EXT_visual_info = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGI_swap_control;
int QGLNumFunctions_GLX_SGI_swap_control;
#ifdef QGL_DEFINED_GLX_SGI_swap_control
pfn_glXSwapIntervalSGI QGLglXSwapIntervalSGI = 0;

static void Init_GLX_SGI_swap_control ()
{
    QGLNumFunctions_GLX_SGI_swap_control = 0;

    QGLglXSwapIntervalSGI = (pfn_glXSwapIntervalSGI)GetFunctionPointer("glXSwapIntervalSGI");
    if ( QGLglXSwapIntervalSGI )
        ++QGLNumFunctions_GLX_SGI_swap_control;

    if ( QGLNumFunctions_GLX_SGI_swap_control == 1 )
        QGLExists_GLX_SGI_swap_control = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGI_swap_control = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGI_swap_control ()
{
    QGLNumFunctions_GLX_SGI_swap_control = 0;
    QGLExists_GLX_SGI_swap_control = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGI_video_sync;
int QGLNumFunctions_GLX_SGI_video_sync;
#ifdef QGL_DEFINED_GLX_SGI_video_sync
pfn_glXGetVideoSyncSGI QGLglXGetVideoSyncSGI = 0;
pfn_glXWaitVideoSyncSGI QGLglXWaitVideoSyncSGI = 0;

static void Init_GLX_SGI_video_sync ()
{
    QGLNumFunctions_GLX_SGI_video_sync = 0;

    QGLglXGetVideoSyncSGI = (pfn_glXGetVideoSyncSGI)GetFunctionPointer("glXGetVideoSyncSGI");
    if ( QGLglXGetVideoSyncSGI )
        ++QGLNumFunctions_GLX_SGI_video_sync;

    QGLglXWaitVideoSyncSGI = (pfn_glXWaitVideoSyncSGI)GetFunctionPointer("glXWaitVideoSyncSGI");
    if ( QGLglXWaitVideoSyncSGI )
        ++QGLNumFunctions_GLX_SGI_video_sync;

    if ( QGLNumFunctions_GLX_SGI_video_sync == 2 )
        QGLExists_GLX_SGI_video_sync = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGI_video_sync = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGI_video_sync ()
{
    QGLNumFunctions_GLX_SGI_video_sync = 0;
    QGLExists_GLX_SGI_video_sync = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGI_make_current_read;
int QGLNumFunctions_GLX_SGI_make_current_read;
#ifdef QGL_DEFINED_GLX_SGI_make_current_read
pfn_glXGetCurrentReadDrawableSGI QGLglXGetCurrentReadDrawableSGI = 0;
pfn_glXMakeCurrentReadSGI QGLglXMakeCurrentReadSGI = 0;

static void Init_GLX_SGI_make_current_read ()
{
    QGLNumFunctions_GLX_SGI_make_current_read = 0;

    QGLglXGetCurrentReadDrawableSGI = (pfn_glXGetCurrentReadDrawableSGI)GetFunctionPointer("glXGetCurrentReadDrawableSGI");
    if ( QGLglXGetCurrentReadDrawableSGI )
        ++QGLNumFunctions_GLX_SGI_make_current_read;

    QGLglXMakeCurrentReadSGI = (pfn_glXMakeCurrentReadSGI)GetFunctionPointer("glXMakeCurrentReadSGI");
    if ( QGLglXMakeCurrentReadSGI )
        ++QGLNumFunctions_GLX_SGI_make_current_read;

    if ( QGLNumFunctions_GLX_SGI_make_current_read == 2 )
        QGLExists_GLX_SGI_make_current_read = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGI_make_current_read = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGI_make_current_read ()
{
    QGLNumFunctions_GLX_SGI_make_current_read = 0;
    QGLExists_GLX_SGI_make_current_read = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_EXT_visual_rating;
int QGLNumFunctions_GLX_EXT_visual_rating;
#ifdef QGL_DEFINED_GLX_EXT_visual_rating
static void Init_GLX_EXT_visual_rating ()
{
    QGLNumFunctions_GLX_EXT_visual_rating = 0;
    QGLExists_GLX_EXT_visual_rating = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_EXT_visual_rating ()
{
    QGLNumFunctions_GLX_EXT_visual_rating = 0;
    QGLExists_GLX_EXT_visual_rating = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_EXT_import_context;
int QGLNumFunctions_GLX_EXT_import_context;
#ifdef QGL_DEFINED_GLX_EXT_import_context
pfn_glXGetCurrentDisplayEXT QGLglXGetCurrentDisplayEXT = 0;
pfn_glXQueryContextInfoEXT QGLglXQueryContextInfoEXT = 0;
pfn_glXGetContextIDEXT QGLglXGetContextIDEXT = 0;
pfn_glXImportContextEXT QGLglXImportContextEXT = 0;
pfn_glXFreeContextEXT QGLglXFreeContextEXT = 0;

static void Init_GLX_EXT_import_context ()
{
    QGLNumFunctions_GLX_EXT_import_context = 0;

    QGLglXGetCurrentDisplayEXT = (pfn_glXGetCurrentDisplayEXT)GetFunctionPointer("glXGetCurrentDisplayEXT");
    if ( QGLglXGetCurrentDisplayEXT )
        ++QGLNumFunctions_GLX_EXT_import_context;

    QGLglXQueryContextInfoEXT = (pfn_glXQueryContextInfoEXT)GetFunctionPointer("glXQueryContextInfoEXT");
    if ( QGLglXQueryContextInfoEXT )
        ++QGLNumFunctions_GLX_EXT_import_context;

    QGLglXGetContextIDEXT = (pfn_glXGetContextIDEXT)GetFunctionPointer("glXGetContextIDEXT");
    if ( QGLglXGetContextIDEXT )
        ++QGLNumFunctions_GLX_EXT_import_context;

    QGLglXImportContextEXT = (pfn_glXImportContextEXT)GetFunctionPointer("glXImportContextEXT");
    if ( QGLglXImportContextEXT )
        ++QGLNumFunctions_GLX_EXT_import_context;

    QGLglXFreeContextEXT = (pfn_glXFreeContextEXT)GetFunctionPointer("glXFreeContextEXT");
    if ( QGLglXFreeContextEXT )
        ++QGLNumFunctions_GLX_EXT_import_context;

    if ( QGLNumFunctions_GLX_EXT_import_context == 5 )
        QGLExists_GLX_EXT_import_context = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_EXT_import_context = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_EXT_import_context ()
{
    QGLNumFunctions_GLX_EXT_import_context = 0;
    QGLExists_GLX_EXT_import_context = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_fbconfig;
int QGLNumFunctions_GLX_SGIX_fbconfig;
#ifdef QGL_DEFINED_GLX_SGIX_fbconfig
pfn_glXGetFBConfigAttribSGIX QGLglXGetFBConfigAttribSGIX = 0;
pfn_glXChooseFBConfigSGIX QGLglXChooseFBConfigSGIX = 0;
pfn_glXCreateGLXPixmapWithConfigSGIX QGLglXCreateGLXPixmapWithConfigSGIX = 0;
pfn_glXCreateContextWithConfigSGIX QGLglXCreateContextWithConfigSGIX = 0;
pfn_glXGetVisualFromFBConfigSGIX QGLglXGetVisualFromFBConfigSGIX = 0;
pfn_glXGetFBConfigFromVisualSGIX QGLglXGetFBConfigFromVisualSGIX = 0;

static void Init_GLX_SGIX_fbconfig ()
{
    QGLNumFunctions_GLX_SGIX_fbconfig = 0;

    QGLglXGetFBConfigAttribSGIX = (pfn_glXGetFBConfigAttribSGIX)GetFunctionPointer("glXGetFBConfigAttribSGIX");
    if ( QGLglXGetFBConfigAttribSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    QGLglXChooseFBConfigSGIX = (pfn_glXChooseFBConfigSGIX)GetFunctionPointer("glXChooseFBConfigSGIX");
    if ( QGLglXChooseFBConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    QGLglXCreateGLXPixmapWithConfigSGIX = (pfn_glXCreateGLXPixmapWithConfigSGIX)GetFunctionPointer("glXCreateGLXPixmapWithConfigSGIX");
    if ( QGLglXCreateGLXPixmapWithConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    QGLglXCreateContextWithConfigSGIX = (pfn_glXCreateContextWithConfigSGIX)GetFunctionPointer("glXCreateContextWithConfigSGIX");
    if ( QGLglXCreateContextWithConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    QGLglXGetVisualFromFBConfigSGIX = (pfn_glXGetVisualFromFBConfigSGIX)GetFunctionPointer("glXGetVisualFromFBConfigSGIX");
    if ( QGLglXGetVisualFromFBConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    QGLglXGetFBConfigFromVisualSGIX = (pfn_glXGetFBConfigFromVisualSGIX)GetFunctionPointer("glXGetFBConfigFromVisualSGIX");
    if ( QGLglXGetFBConfigFromVisualSGIX )
        ++QGLNumFunctions_GLX_SGIX_fbconfig;

    if ( QGLNumFunctions_GLX_SGIX_fbconfig == 6 )
        QGLExists_GLX_SGIX_fbconfig = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_fbconfig = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_fbconfig ()
{
    QGLNumFunctions_GLX_SGIX_fbconfig = 0;
    QGLExists_GLX_SGIX_fbconfig = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_pbuffer;
int QGLNumFunctions_GLX_SGIX_pbuffer;
#ifdef QGL_DEFINED_GLX_SGIX_pbuffer
pfn_glXCreateGLXPbufferSGIX QGLglXCreateGLXPbufferSGIX = 0;
pfn_glXDestroyGLXPbufferSGIX QGLglXDestroyGLXPbufferSGIX = 0;
pfn_glXQueryGLXPbufferSGIX QGLglXQueryGLXPbufferSGIX = 0;
pfn_glXSelectEventSGIX QGLglXSelectEventSGIX = 0;
pfn_glXGetSelectedEventSGIX QGLglXGetSelectedEventSGIX = 0;

static void Init_GLX_SGIX_pbuffer ()
{
    QGLNumFunctions_GLX_SGIX_pbuffer = 0;

    QGLglXCreateGLXPbufferSGIX = (pfn_glXCreateGLXPbufferSGIX)GetFunctionPointer("glXCreateGLXPbufferSGIX");
    if ( QGLglXCreateGLXPbufferSGIX )
        ++QGLNumFunctions_GLX_SGIX_pbuffer;

    QGLglXDestroyGLXPbufferSGIX = (pfn_glXDestroyGLXPbufferSGIX)GetFunctionPointer("glXDestroyGLXPbufferSGIX");
    if ( QGLglXDestroyGLXPbufferSGIX )
        ++QGLNumFunctions_GLX_SGIX_pbuffer;

    QGLglXQueryGLXPbufferSGIX = (pfn_glXQueryGLXPbufferSGIX)GetFunctionPointer("glXQueryGLXPbufferSGIX");
    if ( QGLglXQueryGLXPbufferSGIX )
        ++QGLNumFunctions_GLX_SGIX_pbuffer;

    QGLglXSelectEventSGIX = (pfn_glXSelectEventSGIX)GetFunctionPointer("glXSelectEventSGIX");
    if ( QGLglXSelectEventSGIX )
        ++QGLNumFunctions_GLX_SGIX_pbuffer;

    QGLglXGetSelectedEventSGIX = (pfn_glXGetSelectedEventSGIX)GetFunctionPointer("glXGetSelectedEventSGIX");
    if ( QGLglXGetSelectedEventSGIX )
        ++QGLNumFunctions_GLX_SGIX_pbuffer;

    if ( QGLNumFunctions_GLX_SGIX_pbuffer == 5 )
        QGLExists_GLX_SGIX_pbuffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_pbuffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_pbuffer ()
{
    QGLNumFunctions_GLX_SGIX_pbuffer = 0;
    QGLExists_GLX_SGIX_pbuffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGI_cushion;
int QGLNumFunctions_GLX_SGI_cushion;
#ifdef QGL_DEFINED_GLX_SGI_cushion
pfn_glXCushionSGI QGLglXCushionSGI = 0;

static void Init_GLX_SGI_cushion ()
{
    QGLNumFunctions_GLX_SGI_cushion = 0;

    QGLglXCushionSGI = (pfn_glXCushionSGI)GetFunctionPointer("glXCushionSGI");
    if ( QGLglXCushionSGI )
        ++QGLNumFunctions_GLX_SGI_cushion;

    if ( QGLNumFunctions_GLX_SGI_cushion == 1 )
        QGLExists_GLX_SGI_cushion = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGI_cushion = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGI_cushion ()
{
    QGLNumFunctions_GLX_SGI_cushion = 0;
    QGLExists_GLX_SGI_cushion = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_video_resize;
int QGLNumFunctions_GLX_SGIX_video_resize;
#ifdef QGL_DEFINED_GLX_SGIX_video_resize
pfn_glXBindChannelToWindowSGIX QGLglXBindChannelToWindowSGIX = 0;
pfn_glXChannelRectSGIX QGLglXChannelRectSGIX = 0;
pfn_glXQueryChannelRectSGIX QGLglXQueryChannelRectSGIX = 0;
pfn_glXQueryChannelDeltasSGIX QGLglXQueryChannelDeltasSGIX = 0;
pfn_glXChannelRectSyncSGIX QGLglXChannelRectSyncSGIX = 0;

static void Init_GLX_SGIX_video_resize ()
{
    QGLNumFunctions_GLX_SGIX_video_resize = 0;

    QGLglXBindChannelToWindowSGIX = (pfn_glXBindChannelToWindowSGIX)GetFunctionPointer("glXBindChannelToWindowSGIX");
    if ( QGLglXBindChannelToWindowSGIX )
        ++QGLNumFunctions_GLX_SGIX_video_resize;

    QGLglXChannelRectSGIX = (pfn_glXChannelRectSGIX)GetFunctionPointer("glXChannelRectSGIX");
    if ( QGLglXChannelRectSGIX )
        ++QGLNumFunctions_GLX_SGIX_video_resize;

    QGLglXQueryChannelRectSGIX = (pfn_glXQueryChannelRectSGIX)GetFunctionPointer("glXQueryChannelRectSGIX");
    if ( QGLglXQueryChannelRectSGIX )
        ++QGLNumFunctions_GLX_SGIX_video_resize;

    QGLglXQueryChannelDeltasSGIX = (pfn_glXQueryChannelDeltasSGIX)GetFunctionPointer("glXQueryChannelDeltasSGIX");
    if ( QGLglXQueryChannelDeltasSGIX )
        ++QGLNumFunctions_GLX_SGIX_video_resize;

    QGLglXChannelRectSyncSGIX = (pfn_glXChannelRectSyncSGIX)GetFunctionPointer("glXChannelRectSyncSGIX");
    if ( QGLglXChannelRectSyncSGIX )
        ++QGLNumFunctions_GLX_SGIX_video_resize;

    if ( QGLNumFunctions_GLX_SGIX_video_resize == 5 )
        QGLExists_GLX_SGIX_video_resize = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_video_resize = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_video_resize ()
{
    QGLNumFunctions_GLX_SGIX_video_resize = 0;
    QGLExists_GLX_SGIX_video_resize = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_dmbuffer;
int QGLNumFunctions_GLX_SGIX_dmbuffer;
#ifdef QGL_DEFINED_GLX_SGIX_dmbuffer
static void Init_GLX_SGIX_dmbuffer ()
{
    QGLNumFunctions_GLX_SGIX_dmbuffer = 0;
    QGLExists_GLX_SGIX_dmbuffer = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_SGIX_dmbuffer ()
{
    QGLNumFunctions_GLX_SGIX_dmbuffer = 0;
    QGLExists_GLX_SGIX_dmbuffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_swap_group;
int QGLNumFunctions_GLX_SGIX_swap_group;
#ifdef QGL_DEFINED_GLX_SGIX_swap_group
pfn_glXJoinSwapGroupSGIX QGLglXJoinSwapGroupSGIX = 0;

static void Init_GLX_SGIX_swap_group ()
{
    QGLNumFunctions_GLX_SGIX_swap_group = 0;

    QGLglXJoinSwapGroupSGIX = (pfn_glXJoinSwapGroupSGIX)GetFunctionPointer("glXJoinSwapGroupSGIX");
    if ( QGLglXJoinSwapGroupSGIX )
        ++QGLNumFunctions_GLX_SGIX_swap_group;

    if ( QGLNumFunctions_GLX_SGIX_swap_group == 1 )
        QGLExists_GLX_SGIX_swap_group = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_swap_group = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_swap_group ()
{
    QGLNumFunctions_GLX_SGIX_swap_group = 0;
    QGLExists_GLX_SGIX_swap_group = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_swap_barrier;
int QGLNumFunctions_GLX_SGIX_swap_barrier;
#ifdef QGL_DEFINED_GLX_SGIX_swap_barrier
pfn_glXBindSwapBarrierSGIX QGLglXBindSwapBarrierSGIX = 0;
pfn_glXQueryMaxSwapBarriersSGIX QGLglXQueryMaxSwapBarriersSGIX = 0;

static void Init_GLX_SGIX_swap_barrier ()
{
    QGLNumFunctions_GLX_SGIX_swap_barrier = 0;

    QGLglXBindSwapBarrierSGIX = (pfn_glXBindSwapBarrierSGIX)GetFunctionPointer("glXBindSwapBarrierSGIX");
    if ( QGLglXBindSwapBarrierSGIX )
        ++QGLNumFunctions_GLX_SGIX_swap_barrier;

    QGLglXQueryMaxSwapBarriersSGIX = (pfn_glXQueryMaxSwapBarriersSGIX)GetFunctionPointer("glXQueryMaxSwapBarriersSGIX");
    if ( QGLglXQueryMaxSwapBarriersSGIX )
        ++QGLNumFunctions_GLX_SGIX_swap_barrier;

    if ( QGLNumFunctions_GLX_SGIX_swap_barrier == 2 )
        QGLExists_GLX_SGIX_swap_barrier = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_swap_barrier = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_swap_barrier ()
{
    QGLNumFunctions_GLX_SGIX_swap_barrier = 0;
    QGLExists_GLX_SGIX_swap_barrier = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIS_blended_overlay;
int QGLNumFunctions_GLX_SGIS_blended_overlay;
#ifdef QGL_DEFINED_GLX_SGIS_blended_overlay
static void Init_GLX_SGIS_blended_overlay ()
{
    QGLNumFunctions_GLX_SGIS_blended_overlay = 0;
    QGLExists_GLX_SGIS_blended_overlay = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_SGIS_blended_overlay ()
{
    QGLNumFunctions_GLX_SGIS_blended_overlay = 0;
    QGLExists_GLX_SGIS_blended_overlay = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIS_shared_multisample;
int QGLNumFunctions_GLX_SGIS_shared_multisample;
#ifdef QGL_DEFINED_GLX_SGIS_shared_multisample
static void Init_GLX_SGIS_shared_multisample ()
{
    QGLNumFunctions_GLX_SGIS_shared_multisample = 0;
    QGLExists_GLX_SGIS_shared_multisample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_SGIS_shared_multisample ()
{
    QGLNumFunctions_GLX_SGIS_shared_multisample = 0;
    QGLExists_GLX_SGIS_shared_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SUN_get_transparent_index;
int QGLNumFunctions_GLX_SUN_get_transparent_index;
#ifdef QGL_DEFINED_GLX_SUN_get_transparent_index
pfn_glXGetTransparentIndexSUN QGLglXGetTransparentIndexSUN = 0;

static void Init_GLX_SUN_get_transparent_index ()
{
    QGLNumFunctions_GLX_SUN_get_transparent_index = 0;

    QGLglXGetTransparentIndexSUN = (pfn_glXGetTransparentIndexSUN)GetFunctionPointer("glXGetTransparentIndexSUN");
    if ( QGLglXGetTransparentIndexSUN )
        ++QGLNumFunctions_GLX_SUN_get_transparent_index;

    if ( QGLNumFunctions_GLX_SUN_get_transparent_index == 1 )
        QGLExists_GLX_SUN_get_transparent_index = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SUN_get_transparent_index = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SUN_get_transparent_index ()
{
    QGLNumFunctions_GLX_SUN_get_transparent_index = 0;
    QGLExists_GLX_SUN_get_transparent_index = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_3DFX_multisample;
int QGLNumFunctions_GLX_3DFX_multisample;
#ifdef QGL_DEFINED_GLX_3DFX_multisample
static void Init_GLX_3DFX_multisample ()
{
    QGLNumFunctions_GLX_3DFX_multisample = 0;
    QGLExists_GLX_3DFX_multisample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_3DFX_multisample ()
{
    QGLNumFunctions_GLX_3DFX_multisample = 0;
    QGLExists_GLX_3DFX_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_MESA_copy_sub_buffer;
int QGLNumFunctions_GLX_MESA_copy_sub_buffer;
#ifdef QGL_DEFINED_GLX_MESA_copy_sub_buffer
pfn_glXCopySubBufferMESA QGLglXCopySubBufferMESA = 0;

static void Init_GLX_MESA_copy_sub_buffer ()
{
    QGLNumFunctions_GLX_MESA_copy_sub_buffer = 0;

    QGLglXCopySubBufferMESA = (pfn_glXCopySubBufferMESA)GetFunctionPointer("glXCopySubBufferMESA");
    if ( QGLglXCopySubBufferMESA )
        ++QGLNumFunctions_GLX_MESA_copy_sub_buffer;

    if ( QGLNumFunctions_GLX_MESA_copy_sub_buffer == 1 )
        QGLExists_GLX_MESA_copy_sub_buffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_MESA_copy_sub_buffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_MESA_copy_sub_buffer ()
{
    QGLNumFunctions_GLX_MESA_copy_sub_buffer = 0;
    QGLExists_GLX_MESA_copy_sub_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_MESA_pixmap_colormap;
int QGLNumFunctions_GLX_MESA_pixmap_colormap;
#ifdef QGL_DEFINED_GLX_MESA_pixmap_colormap
pfn_glXCreateGLXPixmapMESA QGLglXCreateGLXPixmapMESA = 0;

static void Init_GLX_MESA_pixmap_colormap ()
{
    QGLNumFunctions_GLX_MESA_pixmap_colormap = 0;

    QGLglXCreateGLXPixmapMESA = (pfn_glXCreateGLXPixmapMESA)GetFunctionPointer("glXCreateGLXPixmapMESA");
    if ( QGLglXCreateGLXPixmapMESA )
        ++QGLNumFunctions_GLX_MESA_pixmap_colormap;

    if ( QGLNumFunctions_GLX_MESA_pixmap_colormap == 1 )
        QGLExists_GLX_MESA_pixmap_colormap = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_MESA_pixmap_colormap = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_MESA_pixmap_colormap ()
{
    QGLNumFunctions_GLX_MESA_pixmap_colormap = 0;
    QGLExists_GLX_MESA_pixmap_colormap = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_MESA_release_buffers;
int QGLNumFunctions_GLX_MESA_release_buffers;
#ifdef QGL_DEFINED_GLX_MESA_release_buffers
pfn_glXReleaseBuffersMESA QGLglXReleaseBuffersMESA = 0;

static void Init_GLX_MESA_release_buffers ()
{
    QGLNumFunctions_GLX_MESA_release_buffers = 0;

    QGLglXReleaseBuffersMESA = (pfn_glXReleaseBuffersMESA)GetFunctionPointer("glXReleaseBuffersMESA");
    if ( QGLglXReleaseBuffersMESA )
        ++QGLNumFunctions_GLX_MESA_release_buffers;

    if ( QGLNumFunctions_GLX_MESA_release_buffers == 1 )
        QGLExists_GLX_MESA_release_buffers = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_MESA_release_buffers = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_MESA_release_buffers ()
{
    QGLNumFunctions_GLX_MESA_release_buffers = 0;
    QGLExists_GLX_MESA_release_buffers = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_MESA_set_3dfx_mode;
int QGLNumFunctions_GLX_MESA_set_3dfx_mode;
#ifdef QGL_DEFINED_GLX_MESA_set_3dfx_mode
pfn_glXSet3DfxModeMESA QGLglXSet3DfxModeMESA = 0;

static void Init_GLX_MESA_set_3dfx_mode ()
{
    QGLNumFunctions_GLX_MESA_set_3dfx_mode = 0;

    QGLglXSet3DfxModeMESA = (pfn_glXSet3DfxModeMESA)GetFunctionPointer("glXSet3DfxModeMESA");
    if ( QGLglXSet3DfxModeMESA )
        ++QGLNumFunctions_GLX_MESA_set_3dfx_mode;

    if ( QGLNumFunctions_GLX_MESA_set_3dfx_mode == 1 )
        QGLExists_GLX_MESA_set_3dfx_mode = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_MESA_set_3dfx_mode = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_MESA_set_3dfx_mode ()
{
    QGLNumFunctions_GLX_MESA_set_3dfx_mode = 0;
    QGLExists_GLX_MESA_set_3dfx_mode = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_visual_select_group;
int QGLNumFunctions_GLX_SGIX_visual_select_group;
#ifdef QGL_DEFINED_GLX_SGIX_visual_select_group
static void Init_GLX_SGIX_visual_select_group ()
{
    QGLNumFunctions_GLX_SGIX_visual_select_group = 0;
    QGLExists_GLX_SGIX_visual_select_group = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_SGIX_visual_select_group ()
{
    QGLNumFunctions_GLX_SGIX_visual_select_group = 0;
    QGLExists_GLX_SGIX_visual_select_group = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_OML_swap_method;
int QGLNumFunctions_GLX_OML_swap_method;
#ifdef QGL_DEFINED_GLX_OML_swap_method
static void Init_GLX_OML_swap_method ()
{
    QGLNumFunctions_GLX_OML_swap_method = 0;
    QGLExists_GLX_OML_swap_method = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_OML_swap_method ()
{
    QGLNumFunctions_GLX_OML_swap_method = 0;
    QGLExists_GLX_OML_swap_method = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_OML_sync_control;
int QGLNumFunctions_GLX_OML_sync_control;
#ifdef QGL_DEFINED_GLX_OML_sync_control
pfn_glXGetSyncValuesOML QGLglXGetSyncValuesOML = 0;
pfn_glXGetMscRateOML QGLglXGetMscRateOML = 0;
pfn_glXSwapBuffersMscOML QGLglXSwapBuffersMscOML = 0;
pfn_glXWaitForMscOML QGLglXWaitForMscOML = 0;
pfn_glXWaitForSbcOML QGLglXWaitForSbcOML = 0;

static void Init_GLX_OML_sync_control ()
{
    QGLNumFunctions_GLX_OML_sync_control = 0;

    QGLglXGetSyncValuesOML = (pfn_glXGetSyncValuesOML)GetFunctionPointer("glXGetSyncValuesOML");
    if ( QGLglXGetSyncValuesOML )
        ++QGLNumFunctions_GLX_OML_sync_control;

    QGLglXGetMscRateOML = (pfn_glXGetMscRateOML)GetFunctionPointer("glXGetMscRateOML");
    if ( QGLglXGetMscRateOML )
        ++QGLNumFunctions_GLX_OML_sync_control;

    QGLglXSwapBuffersMscOML = (pfn_glXSwapBuffersMscOML)GetFunctionPointer("glXSwapBuffersMscOML");
    if ( QGLglXSwapBuffersMscOML )
        ++QGLNumFunctions_GLX_OML_sync_control;

    QGLglXWaitForMscOML = (pfn_glXWaitForMscOML)GetFunctionPointer("glXWaitForMscOML");
    if ( QGLglXWaitForMscOML )
        ++QGLNumFunctions_GLX_OML_sync_control;

    QGLglXWaitForSbcOML = (pfn_glXWaitForSbcOML)GetFunctionPointer("glXWaitForSbcOML");
    if ( QGLglXWaitForSbcOML )
        ++QGLNumFunctions_GLX_OML_sync_control;

    if ( QGLNumFunctions_GLX_OML_sync_control == 5 )
        QGLExists_GLX_OML_sync_control = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_OML_sync_control = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_OML_sync_control ()
{
    QGLNumFunctions_GLX_OML_sync_control = 0;
    QGLExists_GLX_OML_sync_control = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_NV_float_buffer;
int QGLNumFunctions_GLX_NV_float_buffer;
#ifdef QGL_DEFINED_GLX_NV_float_buffer
static void Init_GLX_NV_float_buffer ()
{
    QGLNumFunctions_GLX_NV_float_buffer = 0;
    QGLExists_GLX_NV_float_buffer = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GLX_NV_float_buffer ()
{
    QGLNumFunctions_GLX_NV_float_buffer = 0;
    QGLExists_GLX_NV_float_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_SGIX_hyperpipe;
int QGLNumFunctions_GLX_SGIX_hyperpipe;
#ifdef QGL_DEFINED_GLX_SGIX_hyperpipe
pfn_glXQueryHyperpipeNetworkSGIX QGLglXQueryHyperpipeNetworkSGIX = 0;
pfn_glXHyperpipeConfigSGIX QGLglXHyperpipeConfigSGIX = 0;
pfn_glXQueryHyperpipeConfigSGIX QGLglXQueryHyperpipeConfigSGIX = 0;
pfn_glXDestroyHyperpipeConfigSGIX QGLglXDestroyHyperpipeConfigSGIX = 0;
pfn_glXBindHyperpipeSGIX QGLglXBindHyperpipeSGIX = 0;
pfn_glXQueryHyperpipeBestAttribSGIX QGLglXQueryHyperpipeBestAttribSGIX = 0;
pfn_glXHyperpipeAttribSGIX QGLglXHyperpipeAttribSGIX = 0;
pfn_glXQueryHyperpipeAttribSGIX QGLglXQueryHyperpipeAttribSGIX = 0;

static void Init_GLX_SGIX_hyperpipe ()
{
    QGLNumFunctions_GLX_SGIX_hyperpipe = 0;

    QGLglXQueryHyperpipeNetworkSGIX = (pfn_glXQueryHyperpipeNetworkSGIX)GetFunctionPointer("glXQueryHyperpipeNetworkSGIX");
    if ( QGLglXQueryHyperpipeNetworkSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXHyperpipeConfigSGIX = (pfn_glXHyperpipeConfigSGIX)GetFunctionPointer("glXHyperpipeConfigSGIX");
    if ( QGLglXHyperpipeConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXQueryHyperpipeConfigSGIX = (pfn_glXQueryHyperpipeConfigSGIX)GetFunctionPointer("glXQueryHyperpipeConfigSGIX");
    if ( QGLglXQueryHyperpipeConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXDestroyHyperpipeConfigSGIX = (pfn_glXDestroyHyperpipeConfigSGIX)GetFunctionPointer("glXDestroyHyperpipeConfigSGIX");
    if ( QGLglXDestroyHyperpipeConfigSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXBindHyperpipeSGIX = (pfn_glXBindHyperpipeSGIX)GetFunctionPointer("glXBindHyperpipeSGIX");
    if ( QGLglXBindHyperpipeSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXQueryHyperpipeBestAttribSGIX = (pfn_glXQueryHyperpipeBestAttribSGIX)GetFunctionPointer("glXQueryHyperpipeBestAttribSGIX");
    if ( QGLglXQueryHyperpipeBestAttribSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXHyperpipeAttribSGIX = (pfn_glXHyperpipeAttribSGIX)GetFunctionPointer("glXHyperpipeAttribSGIX");
    if ( QGLglXHyperpipeAttribSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    QGLglXQueryHyperpipeAttribSGIX = (pfn_glXQueryHyperpipeAttribSGIX)GetFunctionPointer("glXQueryHyperpipeAttribSGIX");
    if ( QGLglXQueryHyperpipeAttribSGIX )
        ++QGLNumFunctions_GLX_SGIX_hyperpipe;

    if ( QGLNumFunctions_GLX_SGIX_hyperpipe == 8 )
        QGLExists_GLX_SGIX_hyperpipe = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_SGIX_hyperpipe = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_SGIX_hyperpipe ()
{
    QGLNumFunctions_GLX_SGIX_hyperpipe = 0;
    QGLExists_GLX_SGIX_hyperpipe = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GLX_MESA_agp_offset;
int QGLNumFunctions_GLX_MESA_agp_offset;
#ifdef QGL_DEFINED_GLX_MESA_agp_offset
pfn_glXGetAGPOffsetMesa QGLglXGetAGPOffsetMesa = 0;

static void Init_GLX_MESA_agp_offset ()
{
    QGLNumFunctions_GLX_MESA_agp_offset = 0;

    QGLglXGetAGPOffsetMesa = (pfn_glXGetAGPOffsetMesa)GetFunctionPointer("glXGetAGPOffsetMesa");
    if ( QGLglXGetAGPOffsetMesa )
        ++QGLNumFunctions_GLX_MESA_agp_offset;

    if ( QGLNumFunctions_GLX_MESA_agp_offset == 1 )
        QGLExists_GLX_MESA_agp_offset = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GLX_MESA_agp_offset = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GLX_MESA_agp_offset ()
{
    QGLNumFunctions_GLX_MESA_agp_offset = 0;
    QGLExists_GLX_MESA_agp_offset = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
void InitializeGlx ()
{
    Init_GLX_VERSION_1_0();
    Init_GLX_VERSION_1_1();
    Init_GLX_VERSION_1_2();
    Init_GLX_VERSION_1_3();
    Init_GLX_VERSION_1_4();
    Init_GLX_ARB_get_proc_address();
    Init_GLX_ARB_multisample();
    Init_GLX_ARB_fbconfig_float();
    Init_GLX_SGIS_multisample();
    Init_GLX_EXT_visual_info();
    Init_GLX_SGI_swap_control();
    Init_GLX_SGI_video_sync();
    Init_GLX_SGI_make_current_read();
    Init_GLX_EXT_visual_rating();
    Init_GLX_EXT_import_context();
    Init_GLX_SGIX_fbconfig();
    Init_GLX_SGIX_pbuffer();
    Init_GLX_SGI_cushion();
    Init_GLX_SGIX_video_resize();
    Init_GLX_SGIX_dmbuffer();
    Init_GLX_SGIX_swap_group();
    Init_GLX_SGIX_swap_barrier();
    Init_GLX_SGIS_blended_overlay();
    Init_GLX_SGIS_shared_multisample();
    Init_GLX_SUN_get_transparent_index();
    Init_GLX_3DFX_multisample();
    Init_GLX_MESA_copy_sub_buffer();
    Init_GLX_MESA_pixmap_colormap();
    Init_GLX_MESA_release_buffers();
    Init_GLX_MESA_set_3dfx_mode();
    Init_GLX_SGIX_visual_select_group();
    Init_GLX_OML_swap_method();
    Init_GLX_OML_sync_control();
    Init_GLX_NV_float_buffer();
    Init_GLX_SGIX_hyperpipe();
    Init_GLX_MESA_agp_offset();
}
//------------------------------------------------------------------------------------------------------------------
#endif



