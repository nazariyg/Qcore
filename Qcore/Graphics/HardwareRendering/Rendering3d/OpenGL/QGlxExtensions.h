#ifndef QGLXEXTENSIONS_H
#define QGLXEXTENSIONS_H

// The version of the file is: GLX_GLXEXT_VERSION 10

#include "QOpenGLExtensions.h"

#ifdef __glxext_h_
#error glxext.h included before QGlxExtensions.h
#endif

#define __glxext_h_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_VERSION_1_0;
extern int QGLNumFunctions_GLX_VERSION_1_0;
#ifndef GLX_VERSION_1_0
#define GLX_VERSION_1_0
#define QGL_DEFINED_GLX_VERSION_1_0
#define GLX_USE_GL            1
#define GLX_BUFFER_SIZE       2
#define GLX_LEVEL             3
#define GLX_RGBA              4
#define GLX_DOUBLEBUFFER      5
#define GLX_STEREO            6
#define GLX_AUX_BUFFERS       7
#define GLX_RED_SIZE          8
#define GLX_GREEN_SIZE        9
#define GLX_BLUE_SIZE        10
#define GLX_ALPHA_SIZE       11
#define GLX_DEPTH_SIZE       12
#define GLX_STENCIL_SIZE     13
#define GLX_ACCUM_RED_SIZE   14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE  16
#define GLX_ACCUM_ALPHA_SIZE 17
#define GLX_BAD_SCREEN        1
#define GLX_BAD_ATTRIBUTE     2
#define GLX_NO_EXTENSION      3
#define GLX_BAD_VISUAL        4
#define GLX_BAD_CONTEXT       5
#define GLX_BAD_VALUE         6
#define GLX_BAD_ENUM          7
typedef XID GLXDrawable;
typedef XID GLXPixmap;
typedef struct __GLXcontextRec* GLXContext;
extern Bool glXQueryExtension (Display*, int*, int*);
extern Bool glXQueryVersion (Display*, int*, int*);
extern int glXGetConfig (Display*, XVisualInfo*, int, int*);
extern XVisualInfo* glXChooseVisual (Display*, int, int*);
extern GLXPixmap glXCreateGLXPixmap (Display*, XVisualInfo*, Pixmap);
extern void glXDestroyGLXPixmap (Display*, GLXPixmap);
extern GLXContext glXCreateContext (Display*, XVisualInfo*, GLXContext, Bool);
extern void glXDestroyContext (Display*, GLXContext);
extern Bool glXIsDirect (Display*, GLXContext);
extern void glXCopyContext (Display*, GLXContext, GLXContext, GLuint);
extern Bool glXMakeCurrent (Display*, GLXDrawable, GLXContext);
extern GLXContext glXGetCurrentContext (void);
extern GLXDrawable glXGetCurrentDrawable (void);
extern void glXWaitGL (void);
extern void glXWaitX (void);
extern void glXSwapBuffers (Display*, GLXDrawable);
extern void glXUseXFont (Font, int, int, int);
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_VERSION_1_1;
extern int QGLNumFunctions_GLX_VERSION_1_1;
#ifndef GLX_VERSION_1_1
#define GLX_VERSION_1_1
#define QGL_DEFINED_GLX_VERSION_1_1
#define GLX_VENDOR     1
#define GLX_VERSION    2
#define GLX_EXTENSIONS 3
extern const char* glXQueryExtensionsString (Display*, int);
extern const char* glXGetClientString (Display*, int);
extern const char* glXQueryServerString (Display*, int, int);
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_VERSION_1_2;
extern int QGLNumFunctions_GLX_VERSION_1_2;
#ifndef GLX_VERSION_1_2
#define GLX_VERSION_1_2
#define QGL_DEFINED_GLX_VERSION_1_2

typedef Display* (* pfn_glXGetCurrentDisplay) (void);
extern pfn_glXGetCurrentDisplay QGLglXGetCurrentDisplay;
#define glXGetCurrentDisplay QGLglXGetCurrentDisplay
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_VERSION_1_3;
extern int QGLNumFunctions_GLX_VERSION_1_3;
#ifndef GLX_VERSION_1_3
#define GLX_VERSION_1_3
#define QGL_DEFINED_GLX_VERSION_1_3
#define GLX_WINDOW_BIT                     0x00000001
#define GLX_PIXMAP_BIT                     0x00000002
#define GLX_PBUFFER_BIT                    0x00000004
#define GLX_RGBA_BIT                       0x00000001
#define GLX_COLOR_INDEX_BIT                0x00000002
#define GLX_PBUFFER_CLOBBER_MASK           0x08000000
#define GLX_FRONT_LEFT_BUFFER_BIT          0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT         0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT           0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT          0x00000008
#define GLX_AUX_BUFFERS_BIT                0x00000010
#define GLX_DEPTH_BUFFER_BIT               0x00000020
#define GLX_STENCIL_BUFFER_BIT             0x00000040
#define GLX_ACCUM_BUFFER_BIT               0x00000080
#define GLX_CONFIG_CAVEAT                  0x20
#define GLX_X_VISUAL_TYPE                  0x22
#define GLX_TRANSPARENT_TYPE               0x23
#define GLX_TRANSPARENT_INDEX_VALUE        0x24
#define GLX_TRANSPARENT_RED_VALUE          0x25
#define GLX_TRANSPARENT_GREEN_VALUE        0x26
#define GLX_TRANSPARENT_BLUE_VALUE         0x27
#define GLX_TRANSPARENT_ALPHA_VALUE        0x28
#define GLX_DONT_CARE                      0xFFFFFFFF
#define GLX_NONE                           0x8000
#define GLX_SLOW_CONFIG                    0x8001
#define GLX_TRUE_COLOR                     0x8002
#define GLX_DIRECT_COLOR                   0x8003
#define GLX_PSEUDO_COLOR                   0x8004
#define GLX_STATIC_COLOR                   0x8005
#define GLX_GRAY_SCALE                     0x8006
#define GLX_STATIC_GRAY                    0x8007
#define GLX_TRANSPARENT_RGB                0x8008
#define GLX_TRANSPARENT_INDEX              0x8009
#define GLX_VISUAL_ID                      0x800B
#define GLX_SCREEN                         0x800C
#define GLX_NON_CONFORMANT_CONFIG          0x800D
#define GLX_DRAWABLE_TYPE                  0x8010
#define GLX_RENDER_TYPE                    0x8011
#define GLX_X_RENDERABLE                   0x8012
#define GLX_FBCONFIG_ID                    0x8013
#define GLX_RGBA_TYPE                      0x8014
#define GLX_COLOR_INDEX_TYPE               0x8015
#define GLX_MAX_PBUFFER_WIDTH              0x8016
#define GLX_MAX_PBUFFER_HEIGHT             0x8017
#define GLX_MAX_PBUFFER_PIXELS             0x8018
#define GLX_PRESERVED_CONTENTS             0x801B
#define GLX_LARGEST_PBUFFER                0x801C
#define GLX_WIDTH                          0x801D
#define GLX_HEIGHT                         0x801E
#define GLX_EVENT_MASK                     0x801F
#define GLX_DAMAGED                        0x8020
#define GLX_SAVED                          0x8021
#define GLX_WINDOW                         0x8022
#define GLX_PBUFFER                        0x8023
#define GLX_PBUFFER_HEIGHT                 0x8040
#define GLX_PBUFFER_WIDTH                  0x8041
typedef struct __GLXFBConfigRec* GLXFBConfig;
typedef XID GLXWindow;
typedef XID GLXPbuffer;

typedef GLXFBConfig* (* pfn_glXGetFBConfigs) (Display*, int, int*);
extern pfn_glXGetFBConfigs QGLglXGetFBConfigs;
#define glXGetFBConfigs QGLglXGetFBConfigs

typedef GLXFBConfig* (* pfn_glXChooseFBConfig) (Display*, int, const int*, int*);
extern pfn_glXChooseFBConfig QGLglXChooseFBConfig;
#define glXChooseFBConfig QGLglXChooseFBConfig

typedef int (* pfn_glXGetFBConfigAttrib) (Display*, GLXFBConfig, int, int*);
extern pfn_glXGetFBConfigAttrib QGLglXGetFBConfigAttrib;
#define glXGetFBConfigAttrib QGLglXGetFBConfigAttrib

typedef XVisualInfo* (* pfn_glXGetVisualFromFBConfig) (Display*, GLXFBConfig);
extern pfn_glXGetVisualFromFBConfig QGLglXGetVisualFromFBConfig;
#define glXGetVisualFromFBConfig QGLglXGetVisualFromFBConfig

typedef GLXWindow (* pfn_glXCreateWindow) (Display*, GLXFBConfig, Window, const int*);
extern pfn_glXCreateWindow QGLglXCreateWindow;
#define glXCreateWindow QGLglXCreateWindow

typedef void (* pfn_glXDestroyWindow) (Display*, GLXWindow);
extern pfn_glXDestroyWindow QGLglXDestroyWindow;
#define glXDestroyWindow QGLglXDestroyWindow

typedef GLXPixmap (* pfn_glXCreatePixmap) (Display*, GLXFBConfig, Pixmap, const int*);
extern pfn_glXCreatePixmap QGLglXCreatePixmap;
#define glXCreatePixmap QGLglXCreatePixmap

typedef void (* pfn_glXDestroyPixmap) (Display*, GLXPixmap);
extern pfn_glXDestroyPixmap QGLglXDestroyPixmap;
#define glXDestroyPixmap QGLglXDestroyPixmap

typedef GLXPbuffer (* pfn_glXCreatePbuffer) (Display*, GLXFBConfig, const int*);
extern pfn_glXCreatePbuffer QGLglXCreatePbuffer;
#define glXCreatePbuffer QGLglXCreatePbuffer

typedef void (* pfn_glXDestroyPbuffer) (Display*, GLXPbuffer);
extern pfn_glXDestroyPbuffer QGLglXDestroyPbuffer;
#define glXDestroyPbuffer QGLglXDestroyPbuffer

typedef void (* pfn_glXQueryDrawable) (Display*, GLXDrawable, int, unsigned int*);
extern pfn_glXQueryDrawable QGLglXQueryDrawable;
#define glXQueryDrawable QGLglXQueryDrawable

typedef GLXContext (* pfn_glXCreateNewContext) (Display*, GLXFBConfig, int, GLXContext, Bool);
extern pfn_glXCreateNewContext QGLglXCreateNewContext;
#define glXCreateNewContext QGLglXCreateNewContext

typedef Bool (* pfn_glXMakeContextCurrent) (Display*, GLXDrawable, GLXDrawable, GLXContext);
extern pfn_glXMakeContextCurrent QGLglXMakeContextCurrent;
#define glXMakeContextCurrent QGLglXMakeContextCurrent

typedef GLXDrawable (* pfn_glXGetCurrentReadDrawable) (void);
extern pfn_glXGetCurrentReadDrawable QGLglXGetCurrentReadDrawable;
#define glXGetCurrentReadDrawable QGLglXGetCurrentReadDrawable

typedef int (* pfn_glXQueryContext) (Display*, GLXContext, int, int*);
extern pfn_glXQueryContext QGLglXQueryContext;
#define glXQueryContext QGLglXQueryContext

typedef void (* pfn_glXSelectEvent) (Display*, GLXDrawable, unsigned long);
extern pfn_glXSelectEvent QGLglXSelectEvent;
#define glXSelectEvent QGLglXSelectEvent

typedef void (* pfn_glXGetSelectedEvent) (Display*, GLXDrawable, unsigned long*);
extern pfn_glXGetSelectedEvent QGLglXGetSelectedEvent;
#define glXGetSelectedEvent QGLglXGetSelectedEvent
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_VERSION_1_4;
extern int QGLNumFunctions_GLX_VERSION_1_4;
#ifndef GLX_VERSION_1_4
#define GLX_VERSION_1_4
#define QGL_DEFINED_GLX_VERSION_1_4
#define GLX_SAMPLE_BUFFERS                 100000
#define GLX_SAMPLES                        100001
extern void (*glXGetProcAddress (const GLubyte*))(void);
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_ARB_get_proc_address;
extern int QGLNumFunctions_GLX_ARB_get_proc_address;
#ifndef GLX_ARB_get_proc_address
#define GLX_ARB_get_proc_address
#define QGL_DEFINED_GLX_ARB_get_proc_address
extern void (*glXGetProcAddressARB (const GLubyte*))(void);
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_ARB_multisample;
extern int QGLNumFunctions_GLX_ARB_multisample;
#ifndef GLX_ARB_multisample
#define GLX_ARB_multisample
#define QGL_DEFINED_GLX_ARB_multisample
#define GLX_SAMPLE_BUFFERS_ARB             100000
#define GLX_SAMPLES_ARB                    100001
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_ARB_fbconfig_float;
extern int QGLNumFunctions_GLX_ARB_fbconfig_float;
#ifndef GLX_ARB_fbconfig_float
#define GLX_ARB_fbconfig_float
#define QGL_DEFINED_GLX_ARB_fbconfig_float
#define GLX_RGBA_FLOAT_TYPE_ARB            0x20B9
#define GLX_RGBA_FLOAT_BIT_ARB             0x00000004
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIS_multisample;
extern int QGLNumFunctions_GLX_SGIS_multisample;
#ifndef GLX_SGIS_multisample
#define GLX_SGIS_multisample
#define QGL_DEFINED_GLX_SGIS_multisample
#define GLX_SAMPLE_BUFFERS_SGIS            100000
#define GLX_SAMPLES_SGIS                   100001
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_EXT_visual_info;
extern int QGLNumFunctions_GLX_EXT_visual_info;
#ifndef GLX_EXT_visual_info
#define GLX_EXT_visual_info
#define QGL_DEFINED_GLX_EXT_visual_info
#define GLX_X_VISUAL_TYPE_EXT              0x22
#define GLX_TRANSPARENT_TYPE_EXT           0x23
#define GLX_TRANSPARENT_INDEX_VALUE_EXT    0x24
#define GLX_TRANSPARENT_RED_VALUE_EXT      0x25
#define GLX_TRANSPARENT_GREEN_VALUE_EXT    0x26
#define GLX_TRANSPARENT_BLUE_VALUE_EXT     0x27
#define GLX_TRANSPARENT_ALPHA_VALUE_EXT    0x28
#define GLX_NONE_EXT                       0x8000
#define GLX_TRUE_COLOR_EXT                 0x8002
#define GLX_DIRECT_COLOR_EXT               0x8003
#define GLX_PSEUDO_COLOR_EXT               0x8004
#define GLX_STATIC_COLOR_EXT               0x8005
#define GLX_GRAY_SCALE_EXT                 0x8006
#define GLX_STATIC_GRAY_EXT                0x8007
#define GLX_TRANSPARENT_RGB_EXT            0x8008
#define GLX_TRANSPARENT_INDEX_EXT          0x8009
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGI_swap_control;
extern int QGLNumFunctions_GLX_SGI_swap_control;
#ifndef GLX_SGI_swap_control
#define GLX_SGI_swap_control
#define QGL_DEFINED_GLX_SGI_swap_control

typedef int (* pfn_glXSwapIntervalSGI) (int);
extern pfn_glXSwapIntervalSGI QGLglXSwapIntervalSGI;
#define glXSwapIntervalSGI QGLglXSwapIntervalSGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGI_video_sync;
extern int QGLNumFunctions_GLX_SGI_video_sync;
#ifndef GLX_SGI_video_sync
#define GLX_SGI_video_sync
#define QGL_DEFINED_GLX_SGI_video_sync

typedef int (* pfn_glXGetVideoSyncSGI) (unsigned int*);
extern pfn_glXGetVideoSyncSGI QGLglXGetVideoSyncSGI;
#define glXGetVideoSyncSGI QGLglXGetVideoSyncSGI

typedef int (* pfn_glXWaitVideoSyncSGI) (int, int, unsigned int*);
extern pfn_glXWaitVideoSyncSGI QGLglXWaitVideoSyncSGI;
#define glXWaitVideoSyncSGI QGLglXWaitVideoSyncSGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGI_make_current_read;
extern int QGLNumFunctions_GLX_SGI_make_current_read;
#ifndef GLX_SGI_make_current_read
#define GLX_SGI_make_current_read
#define QGL_DEFINED_GLX_SGI_make_current_read

typedef Bool (* pfn_glXGetCurrentReadDrawableSGI) (Display*, GLXDrawable, GLXDrawable, GLXContext);
extern pfn_glXGetCurrentReadDrawableSGI QGLglXGetCurrentReadDrawableSGI;
#define glXGetCurrentReadDrawableSGI QGLglXGetCurrentReadDrawableSGI

typedef GLXDrawable (* pfn_glXMakeCurrentReadSGI) (void);
extern pfn_glXMakeCurrentReadSGI QGLglXMakeCurrentReadSGI;
#define glXMakeCurrentReadSGI QGLglXMakeCurrentReadSGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_EXT_visual_rating;
extern int QGLNumFunctions_GLX_EXT_visual_rating;
#ifndef GLX_EXT_visual_rating
#define GLX_EXT_visual_rating
#define QGL_DEFINED_GLX_EXT_visual_rating
#define GLX_VISUAL_CAVEAT_EXT              0x20
#define GLX_SLOW_VISUAL_EXT                0x8001
#define GLX_NON_CONFORMANT_VISUAL_EXT      0x800D
/* reuse GLX_NONE_EXT */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_EXT_import_context;
extern int QGLNumFunctions_GLX_EXT_import_context;
#ifndef GLX_EXT_import_context
#define GLX_EXT_import_context
#define QGL_DEFINED_GLX_EXT_import_context
#define GLX_SHARE_CONTEXT_EXT              0x800A
#define GLX_VISUAL_ID_EXT                  0x800B
#define GLX_SCREEN_EXT                     0x800C
typedef XID GLXContextID;

typedef Display* (* pfn_glXGetCurrentDisplayEXT) (void);
extern pfn_glXGetCurrentDisplayEXT QGLglXGetCurrentDisplayEXT;
#define glXGetCurrentDisplayEXT QGLglXGetCurrentDisplayEXT

typedef int (* pfn_glXQueryContextInfoEXT) (Display*, GLXContext, int, int*);
extern pfn_glXQueryContextInfoEXT QGLglXQueryContextInfoEXT;
#define glXQueryContextInfoEXT QGLglXQueryContextInfoEXT

typedef GLXContextID (* pfn_glXGetContextIDEXT) (const GLXContext);
extern pfn_glXGetContextIDEXT QGLglXGetContextIDEXT;
#define glXGetContextIDEXT QGLglXGetContextIDEXT

typedef GLXContext (* pfn_glXImportContextEXT) (Display*, GLXContextID);
extern pfn_glXImportContextEXT QGLglXImportContextEXT;
#define glXImportContextEXT QGLglXImportContextEXT

typedef void (* pfn_glXFreeContextEXT) (Display*, GLXContext);
extern pfn_glXFreeContextEXT QGLglXFreeContextEXT;
#define glXFreeContextEXT QGLglXFreeContextEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_fbconfig;
extern int QGLNumFunctions_GLX_SGIX_fbconfig;
#ifndef GLX_SGIX_fbconfig
#define GLX_SGIX_fbconfig
#define QGL_DEFINED_GLX_SGIX_fbconfig
#define GLX_WINDOW_BIT_SGIX                0x00000001
#define GLX_PIXMAP_BIT_SGIX                0x00000002
#define GLX_RGBA_BIT_SGIX                  0x00000001
#define GLX_COLOR_INDEX_BIT_SGIX           0x00000002
#define GLX_DRAWABLE_TYPE_SGIX             0x8010
#define GLX_RENDER_TYPE_SGIX               0x8011
#define GLX_X_RENDERABLE_SGIX              0x8012
#define GLX_FBCONFIG_ID_SGIX               0x8013
#define GLX_RGBA_TYPE_SGIX                 0x8014
#define GLX_COLOR_INDEX_TYPE_SGIX          0x8015
/* reuse GLX_SCREEN_EXT */
typedef XID GLXFBConfigIDSGIX;
typedef struct __GLXFBConfigRec* GLXFBConfigSGIX;

typedef int (* pfn_glXGetFBConfigAttribSGIX) (Display*, GLXFBConfigSGIX, int, int*);
extern pfn_glXGetFBConfigAttribSGIX QGLglXGetFBConfigAttribSGIX;
#define glXGetFBConfigAttribSGIX QGLglXGetFBConfigAttribSGIX

typedef GLXFBConfigSGIX* (* pfn_glXChooseFBConfigSGIX) (Display*, int, int*, int*);
extern pfn_glXChooseFBConfigSGIX QGLglXChooseFBConfigSGIX;
#define glXChooseFBConfigSGIX QGLglXChooseFBConfigSGIX

typedef GLXPixmap (* pfn_glXCreateGLXPixmapWithConfigSGIX) (Display*, GLXFBConfigSGIX, Pixmap);
extern pfn_glXCreateGLXPixmapWithConfigSGIX QGLglXCreateGLXPixmapWithConfigSGIX;
#define glXCreateGLXPixmapWithConfigSGIX QGLglXCreateGLXPixmapWithConfigSGIX

typedef GLXContext (* pfn_glXCreateContextWithConfigSGIX) (Display*, GLXFBConfigSGIX, int, GLXContext, Bool);
extern pfn_glXCreateContextWithConfigSGIX QGLglXCreateContextWithConfigSGIX;
#define glXCreateContextWithConfigSGIX QGLglXCreateContextWithConfigSGIX

typedef XVisualInfo* (* pfn_glXGetVisualFromFBConfigSGIX) (Display*, GLXFBConfigSGIX);
extern pfn_glXGetVisualFromFBConfigSGIX QGLglXGetVisualFromFBConfigSGIX;
#define glXGetVisualFromFBConfigSGIX QGLglXGetVisualFromFBConfigSGIX

typedef GLXFBConfigSGIX (* pfn_glXGetFBConfigFromVisualSGIX) (Display*, XVisualInfo*);
extern pfn_glXGetFBConfigFromVisualSGIX QGLglXGetFBConfigFromVisualSGIX;
#define glXGetFBConfigFromVisualSGIX QGLglXGetFBConfigFromVisualSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_pbuffer;
extern int QGLNumFunctions_GLX_SGIX_pbuffer;
#ifndef GLX_SGIX_pbuffer
#define GLX_SGIX_pbuffer
#define QGL_DEFINED_GLX_SGIX_pbuffer
#define GLX_PBUFFER_BIT_SGIX               0x00000004
#define GLX_BUFFER_CLOBBER_MASK_SGIX       0x08000000
#define GLX_FRONT_LEFT_BUFFER_BIT_SGIX     0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT_SGIX    0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT_SGIX      0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT_SGIX     0x00000008
#define GLX_AUX_BUFFERS_BIT_SGIX           0x00000010
#define GLX_DEPTH_BUFFER_BIT_SGIX          0x00000020
#define GLX_STENCIL_BUFFER_BIT_SGIX        0x00000040
#define GLX_ACCUM_BUFFER_BIT_SGIX          0x00000080
#define GLX_SAMPLE_BUFFERS_BIT_SGIX        0x00000100
#define GLX_MAX_PBUFFER_WIDTH_SGIX         0x8016
#define GLX_MAX_PBUFFER_HEIGHT_SGIX        0x8017
#define GLX_MAX_PBUFFER_PIXELS_SGIX        0x8018
#define GLX_OPTIMAL_PBUFFER_WIDTH_SGIX     0x8019
#define GLX_OPTIMAL_PBUFFER_HEIGHT_SGIX    0x801A
#define GLX_PRESERVED_CONTENTS_SGIX        0x801B
#define GLX_LARGEST_PBUFFER_SGIX           0x801C
#define GLX_WIDTH_SGIX                     0x801D
#define GLX_HEIGHT_SGIX                    0x801E
#define GLX_EVENT_MASK_SGIX                0x801F
#define GLX_DAMAGED_SGIX                   0x8020
#define GLX_SAVED_SGIX                     0x8021
#define GLX_WINDOW_SGIX                    0x8022
#define GLX_PBUFFER_SGIX                   0x8023
typedef XID GLXPbufferSGIX;
typedef struct
{
    int type;
    unsigned long serial;   /* # of last request processed by server */
    Bool send_event;        /* true if this came for SendEvent request */
    Display *display;       /* display the event was read from */
    GLXDrawable drawable;   /* i.d. of Drawable */
    int event_type;         /* GLX_DAMAGED_SGIX or GLX_SAVED_SGIX */
    int draw_type;          /* GLX_WINDOW_SGIX or GLX_PBUFFER_SGIX */
    unsigned int mask;      /* mask indicating which buffers are affected*/
    int x, y;
    int width, height;
    int count;              /* if nonzero, at least this many more */
}
GLXBufferClobberEventSGIX;

typedef GLXPbufferSGIX (* pfn_glXCreateGLXPbufferSGIX) (Display*, GLXFBConfigSGIX, unsigned int, unsigned int, int*);
extern pfn_glXCreateGLXPbufferSGIX QGLglXCreateGLXPbufferSGIX;
#define glXCreateGLXPbufferSGIX QGLglXCreateGLXPbufferSGIX

typedef void (* pfn_glXDestroyGLXPbufferSGIX) (Display*, GLXPbufferSGIX);
extern pfn_glXDestroyGLXPbufferSGIX QGLglXDestroyGLXPbufferSGIX;
#define glXDestroyGLXPbufferSGIX QGLglXDestroyGLXPbufferSGIX

typedef int (* pfn_glXQueryGLXPbufferSGIX) (Display*, GLXPbufferSGIX, int, unsigned int*);
extern pfn_glXQueryGLXPbufferSGIX QGLglXQueryGLXPbufferSGIX;
#define glXQueryGLXPbufferSGIX QGLglXQueryGLXPbufferSGIX

typedef void (* pfn_glXSelectEventSGIX) (Display*, GLXDrawable, unsigned long);
extern pfn_glXSelectEventSGIX QGLglXSelectEventSGIX;
#define glXSelectEventSGIX QGLglXSelectEventSGIX

typedef void (* pfn_glXGetSelectedEventSGIX) (Display*, GLXDrawable, unsigned long*);
extern pfn_glXGetSelectedEventSGIX QGLglXGetSelectedEventSGIX;
#define glXGetSelectedEventSGIX QGLglXGetSelectedEventSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGI_cushion;
extern int QGLNumFunctions_GLX_SGI_cushion;
#ifndef GLX_SGI_cushion
#define GLX_SGI_cushion
#define QGL_DEFINED_GLX_SGI_cushion

typedef void (* pfn_glXCushionSGI) (Display*, Window, float);
extern pfn_glXCushionSGI QGLglXCushionSGI;
#define glXCushionSGI QGLglXCushionSGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_video_resize;
extern int QGLNumFunctions_GLX_SGIX_video_resize;
#ifndef GLX_SGIX_video_resize
#define GLX_SGIX_video_resize
#define QGL_DEFINED_GLX_SGIX_video_resize
#define GLX_SYNC_FRAME_SGIX                0x00000000
#define GLX_SYNC_SWAP_SGIX                 0x00000001

typedef int (* pfn_glXBindChannelToWindowSGIX) (Display*, int, int, Window);
extern pfn_glXBindChannelToWindowSGIX QGLglXBindChannelToWindowSGIX;
#define glXBindChannelToWindowSGIX QGLglXBindChannelToWindowSGIX

typedef int (* pfn_glXChannelRectSGIX) (Display*, int, int, int, int, int, int);
extern pfn_glXChannelRectSGIX QGLglXChannelRectSGIX;
#define glXChannelRectSGIX QGLglXChannelRectSGIX

typedef int (* pfn_glXQueryChannelRectSGIX) (Display*, int, int, int*, int*, int*, int*);
extern pfn_glXQueryChannelRectSGIX QGLglXQueryChannelRectSGIX;
#define glXQueryChannelRectSGIX QGLglXQueryChannelRectSGIX

typedef int (* pfn_glXQueryChannelDeltasSGIX) (Display*, int, int, int*, int*, int*, int*);
extern pfn_glXQueryChannelDeltasSGIX QGLglXQueryChannelDeltasSGIX;
#define glXQueryChannelDeltasSGIX QGLglXQueryChannelDeltasSGIX

typedef int (* pfn_glXChannelRectSyncSGIX) (Display*, int, int, GLenum);
extern pfn_glXChannelRectSyncSGIX QGLglXChannelRectSyncSGIX;
#define glXChannelRectSyncSGIX QGLglXChannelRectSyncSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_dmbuffer;
extern int QGLNumFunctions_GLX_SGIX_dmbuffer;
#ifndef GLX_SGIX_dmbuffer
#define GLX_SGIX_dmbuffer
#define QGL_DEFINED_GLX_SGIX_dmbuffer
/*
The type DMparams and DMbuffer are not defined in glxext.h.  Once ported to
the SGI, I will expose this extension.

#define GLX_DIGITAL_MEDIA_PBUFFER_SGIX     0x8024
typedef Bool (*PFNGLXASSOCIATEDMPBUFFERSGIXPROC)(Display*, GLXPbufferSGIX, DMparams*, DMbuffer);
extern PFNGLXASSOCIATEDMPBUFFERSGIXPROC glXAssociatedMPbufferSGIX;
*/
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_swap_group;
extern int QGLNumFunctions_GLX_SGIX_swap_group;
#ifndef GLX_SGIX_swap_group
#define GLX_SGIX_swap_group
#define QGL_DEFINED_GLX_SGIX_swap_group

typedef void (* pfn_glXJoinSwapGroupSGIX) (Display*, GLXDrawable, GLXDrawable);
extern pfn_glXJoinSwapGroupSGIX QGLglXJoinSwapGroupSGIX;
#define glXJoinSwapGroupSGIX QGLglXJoinSwapGroupSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_swap_barrier;
extern int QGLNumFunctions_GLX_SGIX_swap_barrier;
#ifndef GLX_SGIX_swap_barrier
#define GLX_SGIX_swap_barrier
#define QGL_DEFINED_GLX_SGIX_swap_barrier

typedef void (* pfn_glXBindSwapBarrierSGIX) (Display*, GLXDrawable, int);
extern pfn_glXBindSwapBarrierSGIX QGLglXBindSwapBarrierSGIX;
#define glXBindSwapBarrierSGIX QGLglXBindSwapBarrierSGIX

typedef Bool (* pfn_glXQueryMaxSwapBarriersSGIX) (Display*, int, int*);
extern pfn_glXQueryMaxSwapBarriersSGIX QGLglXQueryMaxSwapBarriersSGIX;
#define glXQueryMaxSwapBarriersSGIX QGLglXQueryMaxSwapBarriersSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIS_blended_overlay;
extern int QGLNumFunctions_GLX_SGIS_blended_overlay;
#ifndef GLX_SGIS_blended_overlay
#define GLX_SGIS_blended_overlay
#define QGL_DEFINED_GLX_SGIS_blended_overlay
#define GLX_BLENDED_RGBA_SGIS              0x8025
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIS_shared_multisample;
extern int QGLNumFunctions_GLX_SGIS_shared_multisample;
#ifndef GLX_SGIS_shared_multisample
#define GLX_SGIS_shared_multisample
#define QGL_DEFINED_GLX_SGIS_shared_multisample
#define GLX_MULTISAMPLE_SUB_RECT_WIDTH_SGIS 0x8026
#define GLX_MULTISAMPLE_SUB_RECT_HEIGHT_SGIS 0x8027
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SUN_get_transparent_index;
extern int QGLNumFunctions_GLX_SUN_get_transparent_index;
#ifndef GLX_SUN_get_transparent_index
#define GLX_SUN_get_transparent_index
#define QGL_DEFINED_GLX_SUN_get_transparent_index

typedef Status (* pfn_glXGetTransparentIndexSUN) (Display*, Window, Window, long*);
extern pfn_glXGetTransparentIndexSUN QGLglXGetTransparentIndexSUN;
#define glXGetTransparentIndexSUN QGLglXGetTransparentIndexSUN
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_3DFX_multisample;
extern int QGLNumFunctions_GLX_3DFX_multisample;
#ifndef GLX_3DFX_multisample
#define GLX_3DFX_multisample
#define QGL_DEFINED_GLX_3DFX_multisample
#define GLX_SAMPLE_BUFFERS_3DFX            0x8050
#define GLX_SAMPLES_3DFX                   0x8051
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_MESA_copy_sub_buffer;
extern int QGLNumFunctions_GLX_MESA_copy_sub_buffer;
#ifndef GLX_MESA_copy_sub_buffer
#define GLX_MESA_copy_sub_buffer
#define QGL_DEFINED_GLX_MESA_copy_sub_buffer

typedef void (* pfn_glXCopySubBufferMESA) (Display*, GLXDrawable, int, int, int, int);
extern pfn_glXCopySubBufferMESA QGLglXCopySubBufferMESA;
#define glXCopySubBufferMESA QGLglXCopySubBufferMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_MESA_pixmap_colormap;
extern int QGLNumFunctions_GLX_MESA_pixmap_colormap;
#ifndef GLX_MESA_pixmap_colormap
#define GLX_MESA_pixmap_colormap
#define QGL_DEFINED_GLX_MESA_pixmap_colormap

typedef GLXPixmap (* pfn_glXCreateGLXPixmapMESA) (Display*, XVisualInfo*, Pixmap, Colormap);
extern pfn_glXCreateGLXPixmapMESA QGLglXCreateGLXPixmapMESA;
#define glXCreateGLXPixmapMESA QGLglXCreateGLXPixmapMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_MESA_release_buffers;
extern int QGLNumFunctions_GLX_MESA_release_buffers;
#ifndef GLX_MESA_release_buffers
#define GLX_MESA_release_buffers
#define QGL_DEFINED_GLX_MESA_release_buffers

typedef Bool (* pfn_glXReleaseBuffersMESA) (Display*, GLXDrawable);
extern pfn_glXReleaseBuffersMESA QGLglXReleaseBuffersMESA;
#define glXReleaseBuffersMESA QGLglXReleaseBuffersMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_MESA_set_3dfx_mode;
extern int QGLNumFunctions_GLX_MESA_set_3dfx_mode;
#ifndef GLX_MESA_set_3dfx_mode
#define GLX_MESA_set_3dfx_mode
#define QGL_DEFINED_GLX_MESA_set_3dfx_mode
#define GLX_3DFX_WINDOW_MODE_MESA          0x1
#define GLX_3DFX_FULLSCREEN_MODE_MESA      0x2

typedef Bool (* pfn_glXSet3DfxModeMESA) (int);
extern pfn_glXSet3DfxModeMESA QGLglXSet3DfxModeMESA;
#define glXSet3DfxModeMESA QGLglXSet3DfxModeMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_visual_select_group;
extern int QGLNumFunctions_GLX_SGIX_visual_select_group;
#ifndef GLX_SGIX_visual_select_group
#define GLX_SGIX_visual_select_group
#define QGL_DEFINED_GLX_SGIX_visual_select_group
#define GLX_VISUAL_SELECT_GROUP_SGIX       0x8028
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_OML_swap_method;
extern int QGLNumFunctions_GLX_OML_swap_method;
#ifndef GLX_OML_swap_method
#define GLX_OML_swap_method
#define QGL_DEFINED_GLX_OML_swap_method
#define GLX_SWAP_METHOD_OML                0x8060
#define GLX_SWAP_EXCHANGE_OML              0x8061
#define GLX_SWAP_COPY_OML                  0x8062
#define GLX_SWAP_UNDEFINED_OML             0x8063
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_OML_sync_control;
extern int QGLNumFunctions_GLX_OML_sync_control;
#ifndef GLX_OML_sync_control
#define GLX_OML_sync_control
#define QGL_DEFINED_GLX_OML_sync_control
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined( __VMS )
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
#else
typedef long long int int64_t;
#endif

typedef Bool (* pfn_glXGetSyncValuesOML) (Display*, GLXDrawable, int64_t*, int64_t*, int64_t*);
extern pfn_glXGetSyncValuesOML QGLglXGetSyncValuesOML;
#define glXGetSyncValuesOML QGLglXGetSyncValuesOML

typedef Bool (* pfn_glXGetMscRateOML) (Display*, GLXDrawable, int32_t*, int32_t*);
extern pfn_glXGetMscRateOML QGLglXGetMscRateOML;
#define glXGetMscRateOML QGLglXGetMscRateOML

typedef int64_t (* pfn_glXSwapBuffersMscOML) (Display*, GLXDrawable, int64_t, int64_t, int64_t);
extern pfn_glXSwapBuffersMscOML QGLglXSwapBuffersMscOML;
#define glXSwapBuffersMscOML QGLglXSwapBuffersMscOML

typedef Bool (* pfn_glXWaitForMscOML) (Display*, GLXDrawable, int64_t, int64_t, int64_t, int64_t*, int64_t*, int64_t*);
extern pfn_glXWaitForMscOML QGLglXWaitForMscOML;
#define glXWaitForMscOML QGLglXWaitForMscOML

typedef Bool (* pfn_glXWaitForSbcOML) (Display*, GLXDrawable, int64_t, int64_t*, int64_t*, int64_t*);
extern pfn_glXWaitForSbcOML QGLglXWaitForSbcOML;
#define glXWaitForSbcOML QGLglXWaitForSbcOML
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_NV_float_buffer;
extern int QGLNumFunctions_GLX_NV_float_buffer;
#ifndef GLX_NV_float_buffer
#define GLX_NV_float_buffer
#define QGL_DEFINED_GLX_NV_float_buffer
#define GLX_FLOAT_COMPONENTS_NV            0x20B0
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_SGIX_hyperpipe;
extern int QGLNumFunctions_GLX_SGIX_hyperpipe;
#ifndef GLX_SGIX_hyperpipe
#define GLX_SGIX_hyperpipe
#define QGL_DEFINED_GLX_SGIX_hyperpipe
#define GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX 80
#define GLX_BAD_HYPERPIPE_CONFIG_SGIX      91
#define GLX_BAD_HYPERPIPE_SGIX             92
#define GLX_HYPERPIPE_DISPLAY_PIPE_SGIX    0x00000001
#define GLX_HYPERPIPE_RENDER_PIPE_SGIX     0x00000002
#define GLX_PIPE_RECT_SGIX                 0x00000001
#define GLX_PIPE_RECT_LIMITS_SGIX          0x00000002
#define GLX_HYPERPIPE_STEREO_SGIX          0x00000003
#define GLX_HYPERPIPE_PIXEL_AVERAGE_SGIX   0x00000004
#define GLX_HYPERPIPE_ID_SGIX              0x8030
typedef struct
{
    char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX];
    int  networkId;
}
GLXHyperpipeNetworkSGIX;
typedef struct
{
    char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX];
    int channel;
    unsigned int participationType;
    int timeSlice;
}
GLXHyperpipeConfigSGIX;
typedef struct
{
    char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX];
    int srcXOrigin, srcYOrigin, srcWidth, srcHeight;
    int destXOrigin, destYOrigin, destWidth, destHeight;
}
GLXPipeRect;
typedef struct
{
    char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX];
    int XOrigin, YOrigin, maxHeight, maxWidth;
}
GLXPipeRectLimits;

typedef GLXHyperpipeNetworkSGIX* (* pfn_glXQueryHyperpipeNetworkSGIX) (Display*, int*);
extern pfn_glXQueryHyperpipeNetworkSGIX QGLglXQueryHyperpipeNetworkSGIX;
#define glXQueryHyperpipeNetworkSGIX QGLglXQueryHyperpipeNetworkSGIX

typedef int (* pfn_glXHyperpipeConfigSGIX) (Display*, int, int, GLXHyperpipeConfigSGIX*, int*);
extern pfn_glXHyperpipeConfigSGIX QGLglXHyperpipeConfigSGIX;
#define glXHyperpipeConfigSGIX QGLglXHyperpipeConfigSGIX

typedef GLXHyperpipeConfigSGIX* (* pfn_glXQueryHyperpipeConfigSGIX) (Display*, int, int*);
extern pfn_glXQueryHyperpipeConfigSGIX QGLglXQueryHyperpipeConfigSGIX;
#define glXQueryHyperpipeConfigSGIX QGLglXQueryHyperpipeConfigSGIX

typedef int (* pfn_glXDestroyHyperpipeConfigSGIX) (Display*, int);
extern pfn_glXDestroyHyperpipeConfigSGIX QGLglXDestroyHyperpipeConfigSGIX;
#define glXDestroyHyperpipeConfigSGIX QGLglXDestroyHyperpipeConfigSGIX

typedef int (* pfn_glXBindHyperpipeSGIX) (Display*, int);
extern pfn_glXBindHyperpipeSGIX QGLglXBindHyperpipeSGIX;
#define glXBindHyperpipeSGIX QGLglXBindHyperpipeSGIX

typedef int (* pfn_glXQueryHyperpipeBestAttribSGIX) (Display*, int, int, int, void*, void*);
extern pfn_glXQueryHyperpipeBestAttribSGIX QGLglXQueryHyperpipeBestAttribSGIX;
#define glXQueryHyperpipeBestAttribSGIX QGLglXQueryHyperpipeBestAttribSGIX

typedef int (* pfn_glXHyperpipeAttribSGIX) (Display*, int, int, int, void*);
extern pfn_glXHyperpipeAttribSGIX QGLglXHyperpipeAttribSGIX;
#define glXHyperpipeAttribSGIX QGLglXHyperpipeAttribSGIX

typedef int (* pfn_glXQueryHyperpipeAttribSGIX) (Display*, int, int, int, void*);
extern pfn_glXQueryHyperpipeAttribSGIX QGLglXQueryHyperpipeAttribSGIX;
#define glXQueryHyperpipeAttribSGIX QGLglXQueryHyperpipeAttribSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GLX_MESA_agp_offset;
extern int QGLNumFunctions_GLX_MESA_agp_offset;
#ifndef GLX_MESA_agp_offset
#define GLX_MESA_agp_offset
#define QGL_DEFINED_GLX_MESA_agp_offset

typedef unsigned int (* pfn_glXGetAGPOffsetMesa) (const void*);
extern pfn_glXGetAGPOffsetMesa QGLglXGetAGPOffsetMesa;
#define glXGetAGPOffsetMesa QGLglXGetAGPOffsetMesa
#endif

//------------------------------------------------------------------------------------------------------------------
void InitializeGlx ();
//------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif



