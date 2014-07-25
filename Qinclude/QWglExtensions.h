#ifndef QWGLEXTENSIONS_H
#define QWGLEXTENSIONS_H

// The version of the file is: WGL_WGLEXT_VERSION 6

#include "QOpenGLExtensions.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_buffer_region;
extern int QGLNumFunctions_WGL_ARB_buffer_region;
#ifndef WGL_ARB_buffer_region
#define WGL_ARB_buffer_region
#define QGL_DEFINED_WGL_ARB_buffer_region
#define WGL_FRONT_COLOR_BUFFER_BIT_ARB 0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB  0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB       0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB     0x00000008

typedef HANDLE (WINAPI* pfn_wglCreateBufferRegionARB) (HDC, int, UINT);
extern pfn_wglCreateBufferRegionARB QGLwglCreateBufferRegionARB;
#define wglCreateBufferRegionARB QGLwglCreateBufferRegionARB

typedef VOID (WINAPI* pfn_wglDeleteBufferRegionARB) (HANDLE);
extern pfn_wglDeleteBufferRegionARB QGLwglDeleteBufferRegionARB;
#define wglDeleteBufferRegionARB QGLwglDeleteBufferRegionARB

typedef BOOL (WINAPI* pfn_wglSaveBufferRegionARB) (HANDLE, int, int, int, int);
extern pfn_wglSaveBufferRegionARB QGLwglSaveBufferRegionARB;
#define wglSaveBufferRegionARB QGLwglSaveBufferRegionARB

typedef BOOL (WINAPI* pfn_wglRestoreBufferRegionARB) (HANDLE, int, int, int, int, int, int);
extern pfn_wglRestoreBufferRegionARB QGLwglRestoreBufferRegionARB;
#define wglRestoreBufferRegionARB QGLwglRestoreBufferRegionARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_multisample;
extern int QGLNumFunctions_WGL_ARB_multisample;
#ifndef WGL_ARB_multisample
#define WGL_ARB_multisample
#define QGL_DEFINED_WGL_ARB_multisample
#define WGL_SAMPLE_BUFFERS_ARB         0x2041
#define WGL_SAMPLES_ARB                0x2042
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_extensions_string;
extern int QGLNumFunctions_WGL_ARB_extensions_string;
#ifndef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string
#define QGL_DEFINED_WGL_ARB_extensions_string

typedef const char* (WINAPI* pfn_wglGetExtensionsStringARB) (HDC);
extern pfn_wglGetExtensionsStringARB QGLwglGetExtensionsStringARB;
#define wglGetExtensionsStringARB QGLwglGetExtensionsStringARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_pixel_format;
extern int QGLNumFunctions_WGL_ARB_pixel_format;
#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format
#define QGL_DEFINED_WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB    0x2000
#define WGL_DRAW_TO_WINDOW_ARB          0x2001
#define WGL_DRAW_TO_BITMAP_ARB          0x2002
#define WGL_ACCELERATION_ARB            0x2003
#define WGL_NEED_PALETTE_ARB            0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB     0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB      0x2006
#define WGL_SWAP_METHOD_ARB             0x2007
#define WGL_NUMBER_OVERLAYS_ARB         0x2008
#define WGL_NUMBER_UNDERLAYS_ARB        0x2009
#define WGL_TRANSPARENT_ARB             0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB   0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB  0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB             0x200C
#define WGL_SHARE_STENCIL_ARB           0x200D
#define WGL_SHARE_ACCUM_ARB             0x200E
#define WGL_SUPPORT_GDI_ARB             0x200F
#define WGL_SUPPORT_OPENGL_ARB          0x2010
#define WGL_DOUBLE_BUFFER_ARB           0x2011
#define WGL_STEREO_ARB                  0x2012
#define WGL_PIXEL_TYPE_ARB              0x2013
#define WGL_COLOR_BITS_ARB              0x2014
#define WGL_RED_BITS_ARB                0x2015
#define WGL_RED_SHIFT_ARB               0x2016
#define WGL_GREEN_BITS_ARB              0x2017
#define WGL_GREEN_SHIFT_ARB             0x2018
#define WGL_BLUE_BITS_ARB               0x2019
#define WGL_BLUE_SHIFT_ARB              0x201A
#define WGL_ALPHA_BITS_ARB              0x201B
#define WGL_ALPHA_SHIFT_ARB             0x201C
#define WGL_ACCUM_BITS_ARB              0x201D
#define WGL_ACCUM_RED_BITS_ARB          0x201E
#define WGL_ACCUM_GREEN_BITS_ARB        0x201F
#define WGL_ACCUM_BLUE_BITS_ARB         0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB        0x2021
#define WGL_DEPTH_BITS_ARB              0x2022
#define WGL_STENCIL_BITS_ARB            0x2023
#define WGL_AUX_BUFFERS_ARB             0x2024
#define WGL_NO_ACCELERATION_ARB         0x2025
#define WGL_GENERIC_ACCELERATION_ARB    0x2026
#define WGL_FULL_ACCELERATION_ARB       0x2027
#define WGL_SWAP_EXCHANGE_ARB           0x2028
#define WGL_SWAP_COPY_ARB               0x2029
#define WGL_SWAP_UNDEFINED_ARB          0x202A
#define WGL_TYPE_RGBA_ARB               0x202B
#define WGL_TYPE_COLORINDEX_ARB         0x202C

typedef BOOL (WINAPI* pfn_wglGetPixelFormatAttribivARB) (HDC, int, int, UINT, const int*, int*);
extern pfn_wglGetPixelFormatAttribivARB QGLwglGetPixelFormatAttribivARB;
#define wglGetPixelFormatAttribivARB QGLwglGetPixelFormatAttribivARB

typedef BOOL (WINAPI* pfn_wglGetPixelFormatAttribfvARB) (HDC, int, int, UINT, const int*, FLOAT*);
extern pfn_wglGetPixelFormatAttribfvARB QGLwglGetPixelFormatAttribfvARB;
#define wglGetPixelFormatAttribfvARB QGLwglGetPixelFormatAttribfvARB

typedef BOOL (WINAPI* pfn_wglChoosePixelFormatARB) (HDC, const int*, const FLOAT*, UINT, int*, UINT*);
extern pfn_wglChoosePixelFormatARB QGLwglChoosePixelFormatARB;
#define wglChoosePixelFormatARB QGLwglChoosePixelFormatARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_make_current_read;
extern int QGLNumFunctions_WGL_ARB_make_current_read;
#ifndef WGL_ARB_make_current_read
#define WGL_ARB_make_current_read
#define QGL_DEFINED_WGL_ARB_make_current_read
#define ERROR_INVALID_PIXEL_TYPE_ARB            0x2043
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB  0x2054

typedef BOOL (WINAPI* pfn_wglMakeContextCurrentARB) (HDC, HDC, HGLRC);
extern pfn_wglMakeContextCurrentARB QGLwglMakeContextCurrentARB;
#define wglMakeContextCurrentARB QGLwglMakeContextCurrentARB

typedef HDC (WINAPI* pfn_wglGetCurrentReadDCARB) (void);
extern pfn_wglGetCurrentReadDCARB QGLwglGetCurrentReadDCARB;
#define wglGetCurrentReadDCARB QGLwglGetCurrentReadDCARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_pbuffer;
extern int QGLNumFunctions_WGL_ARB_pbuffer;
#ifndef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer
#define QGL_DEFINED_WGL_ARB_pbuffer
#define WGL_DRAW_TO_PBUFFER_ARB        0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB     0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB      0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB     0x2030
#define WGL_PBUFFER_LARGEST_ARB        0x2033
#define WGL_PBUFFER_WIDTH_ARB          0x2034
#define WGL_PBUFFER_HEIGHT_ARB         0x2035
#define WGL_PBUFFER_LOST_ARB           0x2036
DECLARE_HANDLE(HPBUFFERARB);

typedef HPBUFFERARB (WINAPI* pfn_wglCreatePbufferARB) (HDC, int, int, int, const int*);
extern pfn_wglCreatePbufferARB QGLwglCreatePbufferARB;
#define wglCreatePbufferARB QGLwglCreatePbufferARB

typedef HDC (WINAPI* pfn_wglGetPbufferDCARB) (HPBUFFERARB);
extern pfn_wglGetPbufferDCARB QGLwglGetPbufferDCARB;
#define wglGetPbufferDCARB QGLwglGetPbufferDCARB

typedef int (WINAPI* pfn_wglReleasePbufferDCARB) (HPBUFFERARB, HDC);
extern pfn_wglReleasePbufferDCARB QGLwglReleasePbufferDCARB;
#define wglReleasePbufferDCARB QGLwglReleasePbufferDCARB

typedef BOOL (WINAPI* pfn_wglDestroyPbufferARB) (HPBUFFERARB);
extern pfn_wglDestroyPbufferARB QGLwglDestroyPbufferARB;
#define wglDestroyPbufferARB QGLwglDestroyPbufferARB

typedef BOOL (WINAPI* pfn_wglQueryPbufferARB) (HPBUFFERARB, int, int*);
extern pfn_wglQueryPbufferARB QGLwglQueryPbufferARB;
#define wglQueryPbufferARB QGLwglQueryPbufferARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_render_texture;
extern int QGLNumFunctions_WGL_ARB_render_texture;
#ifndef WGL_ARB_render_texture
#define WGL_ARB_render_texture
#define QGL_DEFINED_WGL_ARB_render_texture
#define WGL_BIND_TO_TEXTURE_RGB_ARB         0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB        0x2071
#define WGL_TEXTURE_FORMAT_ARB              0x2072
#define WGL_TEXTURE_TARGET_ARB              0x2073
#define WGL_MIPMAP_TEXTURE_ARB              0x2074
#define WGL_TEXTURE_RGB_ARB                 0x2075
#define WGL_TEXTURE_RGBA_ARB                0x2076
#define WGL_NO_TEXTURE_ARB                  0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB            0x2078
#define WGL_TEXTURE_1D_ARB                  0x2079
#define WGL_TEXTURE_2D_ARB                  0x207A
#define WGL_MIPMAP_LEVEL_ARB                0x207B
#define WGL_CUBE_MAP_FACE_ARB               0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB                  0x2083
#define WGL_FRONT_RIGHT_ARB                 0x2084
#define WGL_BACK_LEFT_ARB                   0x2085
#define WGL_BACK_RIGHT_ARB                  0x2086
#define WGL_AUX0_ARB                        0x2087
#define WGL_AUX1_ARB                        0x2088
#define WGL_AUX2_ARB                        0x2089
#define WGL_AUX3_ARB                        0x208A
#define WGL_AUX4_ARB                        0x208B
#define WGL_AUX5_ARB                        0x208C
#define WGL_AUX6_ARB                        0x208D
#define WGL_AUX7_ARB                        0x208E
#define WGL_AUX8_ARB                        0x208F
#define WGL_AUX9_ARB                        0x2090

typedef BOOL (WINAPI* pfn_wglBindTexImageARB) (HPBUFFERARB, int);
extern pfn_wglBindTexImageARB QGLwglBindTexImageARB;
#define wglBindTexImageARB QGLwglBindTexImageARB

typedef BOOL (WINAPI* pfn_wglReleaseTexImageARB) (HPBUFFERARB, int);
extern pfn_wglReleaseTexImageARB QGLwglReleaseTexImageARB;
#define wglReleaseTexImageARB QGLwglReleaseTexImageARB

typedef BOOL (WINAPI* pfn_wglSetPbufferAttribARB) (HPBUFFERARB, const int*);
extern pfn_wglSetPbufferAttribARB QGLwglSetPbufferAttribARB;
#define wglSetPbufferAttribARB QGLwglSetPbufferAttribARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ARB_pixel_format_float;
extern int QGLNumFunctions_WGL_ARB_pixel_format_float;
#ifndef WGL_ARB_pixel_format_float
#define WGL_ARB_pixel_format_float
#define QGL_DEFINED_WGL_ARB_pixel_format_float
#define WGL_TYPE_RGBA_FLOAT_ARB        0x21A0
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_make_current_read;
extern int QGLNumFunctions_WGL_EXT_make_current_read;
#ifndef WGL_EXT_make_current_read
#define WGL_EXT_make_current_read
#define QGL_DEFINED_WGL_EXT_make_current_read
#define ERROR_INVALID_PIXEL_TYPE_EXT   0x2043

typedef BOOL (WINAPI* pfn_wglMakeContextCurrentEXT) (HDC, HDC, HGLRC);
extern pfn_wglMakeContextCurrentEXT QGLwglMakeContextCurrentEXT;
#define wglMakeContextCurrentEXT QGLwglMakeContextCurrentEXT

typedef HDC (WINAPI* pfn_wglGetCurrentReadDCEXT) (void);
extern pfn_wglGetCurrentReadDCEXT QGLwglGetCurrentReadDCEXT;
#define wglGetCurrentReadDCEXT QGLwglGetCurrentReadDCEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_pixel_format;
extern int QGLNumFunctions_WGL_EXT_pixel_format;
#ifndef WGL_EXT_pixel_format
#define WGL_EXT_pixel_format
#define QGL_DEFINED_WGL_EXT_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_EXT   0x2000
#define WGL_DRAW_TO_WINDOW_EXT         0x2001
#define WGL_DRAW_TO_BITMAP_EXT         0x2002
#define WGL_ACCELERATION_EXT           0x2003
#define WGL_NEED_PALETTE_EXT           0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT    0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT     0x2006
#define WGL_SWAP_METHOD_EXT            0x2007
#define WGL_NUMBER_OVERLAYS_EXT        0x2008
#define WGL_NUMBER_UNDERLAYS_EXT       0x2009
#define WGL_TRANSPARENT_EXT            0x200A
#define WGL_TRANSPARENT_VALUE_EXT      0x200B
#define WGL_SHARE_DEPTH_EXT            0x200C
#define WGL_SHARE_STENCIL_EXT          0x200D
#define WGL_SHARE_ACCUM_EXT            0x200E
#define WGL_SUPPORT_GDI_EXT            0x200F
#define WGL_SUPPORT_OPENGL_EXT         0x2010
#define WGL_DOUBLE_BUFFER_EXT          0x2011
#define WGL_STEREO_EXT                 0x2012
#define WGL_PIXEL_TYPE_EXT             0x2013
#define WGL_COLOR_BITS_EXT             0x2014
#define WGL_RED_BITS_EXT               0x2015
#define WGL_RED_SHIFT_EXT              0x2016
#define WGL_GREEN_BITS_EXT             0x2017
#define WGL_GREEN_SHIFT_EXT            0x2018
#define WGL_BLUE_BITS_EXT              0x2019
#define WGL_BLUE_SHIFT_EXT             0x201A
#define WGL_ALPHA_BITS_EXT             0x201B
#define WGL_ALPHA_SHIFT_EXT            0x201C
#define WGL_ACCUM_BITS_EXT             0x201D
#define WGL_ACCUM_RED_BITS_EXT         0x201E
#define WGL_ACCUM_GREEN_BITS_EXT       0x201F
#define WGL_ACCUM_BLUE_BITS_EXT        0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT       0x2021
#define WGL_DEPTH_BITS_EXT             0x2022
#define WGL_STENCIL_BITS_EXT           0x2023
#define WGL_AUX_BUFFERS_EXT            0x2024
#define WGL_NO_ACCELERATION_EXT        0x2025
#define WGL_GENERIC_ACCELERATION_EXT   0x2026
#define WGL_FULL_ACCELERATION_EXT      0x2027
#define WGL_SWAP_EXCHANGE_EXT          0x2028
#define WGL_SWAP_COPY_EXT              0x2029
#define WGL_SWAP_UNDEFINED_EXT         0x202A
#define WGL_TYPE_RGBA_EXT              0x202B
#define WGL_TYPE_COLORINDEX_EXT        0x202C

typedef BOOL (WINAPI* pfn_wglGetPixelFormatAttribivEXT) (HDC, int, int, UINT, int*, int*);
extern pfn_wglGetPixelFormatAttribivEXT QGLwglGetPixelFormatAttribivEXT;
#define wglGetPixelFormatAttribivEXT QGLwglGetPixelFormatAttribivEXT

typedef BOOL (WINAPI* pfn_wglGetPixelFormatAttribfvEXT) (HDC, int, int, UINT, int*, FLOAT*);
extern pfn_wglGetPixelFormatAttribfvEXT QGLwglGetPixelFormatAttribfvEXT;
#define wglGetPixelFormatAttribfvEXT QGLwglGetPixelFormatAttribfvEXT

typedef BOOL (WINAPI* pfn_wglChoosePixelFormatEXT) (HDC, const int*, const FLOAT*, UINT, int*, UINT*);
extern pfn_wglChoosePixelFormatEXT QGLwglChoosePixelFormatEXT;
#define wglChoosePixelFormatEXT QGLwglChoosePixelFormatEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_pbuffer;
extern int QGLNumFunctions_WGL_EXT_pbuffer;
#ifndef WGL_EXT_pbuffer
#define WGL_EXT_pbuffer
#define QGL_DEFINED_WGL_EXT_pbuffer
#define WGL_DRAW_TO_PBUFFER_EXT        0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT     0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT      0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT     0x2030
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT  0x2031
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT 0x2032
#define WGL_PBUFFER_LARGEST_EXT        0x2033
#define WGL_PBUFFER_WIDTH_EXT          0x2034
#define WGL_PBUFFER_HEIGHT_EXT         0x2035
DECLARE_HANDLE(HPBUFFEREXT);

typedef HPBUFFEREXT (WINAPI* pfn_wglCreatePbufferEXT) (HDC, int, int, int, const int*);
extern pfn_wglCreatePbufferEXT QGLwglCreatePbufferEXT;
#define wglCreatePbufferEXT QGLwglCreatePbufferEXT

typedef HDC (WINAPI* pfn_wglGetPbufferDCEXT) (HPBUFFEREXT);
extern pfn_wglGetPbufferDCEXT QGLwglGetPbufferDCEXT;
#define wglGetPbufferDCEXT QGLwglGetPbufferDCEXT

typedef int (WINAPI* pfn_wglReleasePbufferDCEXT) (HPBUFFEREXT, HDC);
extern pfn_wglReleasePbufferDCEXT QGLwglReleasePbufferDCEXT;
#define wglReleasePbufferDCEXT QGLwglReleasePbufferDCEXT

typedef BOOL (WINAPI* pfn_wglDestroyPbufferEXT) (HPBUFFEREXT);
extern pfn_wglDestroyPbufferEXT QGLwglDestroyPbufferEXT;
#define wglDestroyPbufferEXT QGLwglDestroyPbufferEXT

typedef BOOL (WINAPI* pfn_wglQueryPbufferEXT) (HPBUFFEREXT, int, int*);
extern pfn_wglQueryPbufferEXT QGLwglQueryPbufferEXT;
#define wglQueryPbufferEXT QGLwglQueryPbufferEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_depth_float;
extern int QGLNumFunctions_WGL_EXT_depth_float;
#ifndef WGL_EXT_depth_float
#define WGL_EXT_depth_float
#define QGL_DEFINED_WGL_EXT_depth_float
#define WGL_DEPTH_FLOAT_EXT            0x2040
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_3DFX_multisample;
extern int QGLNumFunctions_WGL_3DFX_multisample;
#ifndef WGL_3DFX_multisample
#define WGL_3DFX_multisample
#define QGL_DEFINED_WGL_3DFX_multisample
#define WGL_SAMPLE_BUFFERS_3DFX        0x2060
#define WGL_SAMPLES_3DFX               0x2061
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_multisample;
extern int QGLNumFunctions_WGL_EXT_multisample;
#ifndef WGL_EXT_multisample
#define WGL_EXT_multisample
#define QGL_DEFINED_WGL_EXT_multisample
#define WGL_SAMPLE_BUFFERS_EXT         0x2041
#define WGL_SAMPLES_EXT                0x2042
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_digital_video_control;
extern int QGLNumFunctions_WGL_I3D_digital_video_control;
#ifndef WGL_I3D_digital_video_control
#define WGL_I3D_digital_video_control
#define QGL_DEFINED_WGL_I3D_digital_video_control
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D  0x2050
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D        0x2051
#define WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D           0x2052
#define WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D           0x2053

typedef BOOL (WINAPI* pfn_wglGetDigitalVideoParametersI3D) (HDC, int, int*);
extern pfn_wglGetDigitalVideoParametersI3D QGLwglGetDigitalVideoParametersI3D;
#define wglGetDigitalVideoParametersI3D QGLwglGetDigitalVideoParametersI3D

typedef BOOL (WINAPI* pfn_wglSetDigitalVideoParametersI3D) (HDC, int, const int*);
extern pfn_wglSetDigitalVideoParametersI3D QGLwglSetDigitalVideoParametersI3D;
#define wglSetDigitalVideoParametersI3D QGLwglSetDigitalVideoParametersI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_gamma;
extern int QGLNumFunctions_WGL_I3D_gamma;
#ifndef WGL_I3D_gamma
#define WGL_I3D_gamma
#define QGL_DEFINED_WGL_I3D_gamma
#define WGL_GAMMA_TABLE_SIZE_I3D       0x204E
#define WGL_GAMMA_EXCLUDE_DESKTOP_I3D  0x204F

typedef BOOL (WINAPI* pfn_wglGetGammaTableParametersI3D) (HDC, int, int*);
extern pfn_wglGetGammaTableParametersI3D QGLwglGetGammaTableParametersI3D;
#define wglGetGammaTableParametersI3D QGLwglGetGammaTableParametersI3D

typedef BOOL (WINAPI* pfn_wglSetGammaTableParametersI3D) (HDC, int, const int*);
extern pfn_wglSetGammaTableParametersI3D QGLwglSetGammaTableParametersI3D;
#define wglSetGammaTableParametersI3D QGLwglSetGammaTableParametersI3D

typedef BOOL (WINAPI* pfn_wglGetGammaTableI3D) (HDC, int, USHORT*, USHORT*, USHORT*);
extern pfn_wglGetGammaTableI3D QGLwglGetGammaTableI3D;
#define wglGetGammaTableI3D QGLwglGetGammaTableI3D

typedef BOOL (WINAPI* pfn_wglSetGammaTableI3D) (HDC, int, const USHORT*, const USHORT*, const USHORT*);
extern pfn_wglSetGammaTableI3D QGLwglSetGammaTableI3D;
#define wglSetGammaTableI3D QGLwglSetGammaTableI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_genlock;
extern int QGLNumFunctions_WGL_I3D_genlock;
#ifndef WGL_I3D_genlock
#define WGL_I3D_genlock
#define QGL_DEFINED_WGL_I3D_genlock
#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D        0x2044
#define WGL_GENLOCK_SOURCE_EXTENAL_SYNC_I3D     0x2045
#define WGL_GENLOCK_SOURCE_EXTENAL_FIELD_I3D    0x2046
#define WGL_GENLOCK_SOURCE_EXTENAL_TTL_I3D      0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D     0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D    0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D     0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D      0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D        0x204C

typedef BOOL (WINAPI* pfn_wglEnableGenlockI3D) (HDC);
extern pfn_wglEnableGenlockI3D QGLwglEnableGenlockI3D;
#define wglEnableGenlockI3D QGLwglEnableGenlockI3D

typedef BOOL (WINAPI* pfn_wglDisableGenlockI3D) (HDC);
extern pfn_wglDisableGenlockI3D QGLwglDisableGenlockI3D;
#define wglDisableGenlockI3D QGLwglDisableGenlockI3D

typedef BOOL (WINAPI* pfn_wglIsEnabledGenlockI3D) (HDC, BOOL*);
extern pfn_wglIsEnabledGenlockI3D QGLwglIsEnabledGenlockI3D;
#define wglIsEnabledGenlockI3D QGLwglIsEnabledGenlockI3D

typedef BOOL (WINAPI* pfn_wglGenlockSourceI3D) (HDC, UINT);
extern pfn_wglGenlockSourceI3D QGLwglGenlockSourceI3D;
#define wglGenlockSourceI3D QGLwglGenlockSourceI3D

typedef BOOL (WINAPI* pfn_wglGetGenlockSourceI3D) (HDC, UINT*);
extern pfn_wglGetGenlockSourceI3D QGLwglGetGenlockSourceI3D;
#define wglGetGenlockSourceI3D QGLwglGetGenlockSourceI3D

typedef BOOL (WINAPI* pfn_wglGenlockSourceEdgeI3D) (HDC, UINT);
extern pfn_wglGenlockSourceEdgeI3D QGLwglGenlockSourceEdgeI3D;
#define wglGenlockSourceEdgeI3D QGLwglGenlockSourceEdgeI3D

typedef BOOL (WINAPI* pfn_wglGetGenlockSourceEdgeI3D) (HDC, UINT*);
extern pfn_wglGetGenlockSourceEdgeI3D QGLwglGetGenlockSourceEdgeI3D;
#define wglGetGenlockSourceEdgeI3D QGLwglGetGenlockSourceEdgeI3D

typedef BOOL (WINAPI* pfn_wglGenlockSampleRateI3D) (HDC, UINT);
extern pfn_wglGenlockSampleRateI3D QGLwglGenlockSampleRateI3D;
#define wglGenlockSampleRateI3D QGLwglGenlockSampleRateI3D

typedef BOOL (WINAPI* pfn_wglGetGenlockSampleRateI3D) (HDC, UINT*);
extern pfn_wglGetGenlockSampleRateI3D QGLwglGetGenlockSampleRateI3D;
#define wglGetGenlockSampleRateI3D QGLwglGetGenlockSampleRateI3D

typedef BOOL (WINAPI* pfn_wglGenlockSourceDelayI3D) (HDC, UINT);
extern pfn_wglGenlockSourceDelayI3D QGLwglGenlockSourceDelayI3D;
#define wglGenlockSourceDelayI3D QGLwglGenlockSourceDelayI3D

typedef BOOL (WINAPI* pfn_wglGetGenlockSourceDelayI3D) (HDC, UINT*);
extern pfn_wglGetGenlockSourceDelayI3D QGLwglGetGenlockSourceDelayI3D;
#define wglGetGenlockSourceDelayI3D QGLwglGetGenlockSourceDelayI3D

typedef BOOL (WINAPI* pfn_wglQueryGenlockMaxSourceDelayI3D) (HDC, UINT*, UINT*);
extern pfn_wglQueryGenlockMaxSourceDelayI3D QGLwglQueryGenlockMaxSourceDelayI3D;
#define wglQueryGenlockMaxSourceDelayI3D QGLwglQueryGenlockMaxSourceDelayI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_image_buffer;
extern int QGLNumFunctions_WGL_I3D_image_buffer;
#ifndef WGL_I3D_image_buffer
#define WGL_I3D_image_buffer
#define QGL_DEFINED_WGL_I3D_image_buffer
#define WGL_IMAGE_BUFFER_MIN_ACCESS_I3D 0x00000001
#define WGL_IMAGE_BUFFER_LOCK_I3D       0x00000002

typedef LPVOID (WINAPI* pfn_wglCreateImageBufferI3D) (HDC, DWORD, UINT);
extern pfn_wglCreateImageBufferI3D QGLwglCreateImageBufferI3D;
#define wglCreateImageBufferI3D QGLwglCreateImageBufferI3D

typedef BOOL (WINAPI* pfn_wglDestroyImageBufferI3D) (HDC, LPVOID);
extern pfn_wglDestroyImageBufferI3D QGLwglDestroyImageBufferI3D;
#define wglDestroyImageBufferI3D QGLwglDestroyImageBufferI3D

typedef BOOL (WINAPI* pfn_wglAssociateImageBufferEventsI3D) (HDC, const HANDLE*, const LPVOID*, const DWORD*, UINT);
extern pfn_wglAssociateImageBufferEventsI3D QGLwglAssociateImageBufferEventsI3D;
#define wglAssociateImageBufferEventsI3D QGLwglAssociateImageBufferEventsI3D

typedef BOOL (WINAPI* pfn_wglReleaseImageBufferEventsI3D) (HDC, const LPVOID*, UINT);
extern pfn_wglReleaseImageBufferEventsI3D QGLwglReleaseImageBufferEventsI3D;
#define wglReleaseImageBufferEventsI3D QGLwglReleaseImageBufferEventsI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_swap_frame_lock;
extern int QGLNumFunctions_WGL_I3D_swap_frame_lock;
#ifndef WGL_I3D_swap_frame_lock
#define WGL_I3D_swap_frame_lock
#define QGL_DEFINED_WGL_I3D_swap_frame_lock

typedef BOOL (WINAPI* pfn_wglEnableFrameLockI3D) (void);
extern pfn_wglEnableFrameLockI3D QGLwglEnableFrameLockI3D;
#define wglEnableFrameLockI3D QGLwglEnableFrameLockI3D

typedef BOOL (WINAPI* pfn_wglDisableFrameLockI3D) (void);
extern pfn_wglDisableFrameLockI3D QGLwglDisableFrameLockI3D;
#define wglDisableFrameLockI3D QGLwglDisableFrameLockI3D

typedef BOOL (WINAPI* pfn_wglIsEnabledFrameLockI3D) (BOOL*);
extern pfn_wglIsEnabledFrameLockI3D QGLwglIsEnabledFrameLockI3D;
#define wglIsEnabledFrameLockI3D QGLwglIsEnabledFrameLockI3D

typedef BOOL (WINAPI* pfn_wglQueryFrameLockMasterI3D) (BOOL*);
extern pfn_wglQueryFrameLockMasterI3D QGLwglQueryFrameLockMasterI3D;
#define wglQueryFrameLockMasterI3D QGLwglQueryFrameLockMasterI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_I3D_swap_frame_usage;
extern int QGLNumFunctions_WGL_I3D_swap_frame_usage;
#ifndef WGL_I3D_swap_frame_usage
#define WGL_I3D_swap_frame_usage
#define QGL_DEFINED_WGL_I3D_swap_frame_usage

typedef BOOL (WINAPI* pfn_wglGetFrameUsageI3D) (float*);
extern pfn_wglGetFrameUsageI3D QGLwglGetFrameUsageI3D;
#define wglGetFrameUsageI3D QGLwglGetFrameUsageI3D

typedef BOOL (WINAPI* pfn_wglBeginFrameTrackingI3D) (void);
extern pfn_wglBeginFrameTrackingI3D QGLwglBeginFrameTrackingI3D;
#define wglBeginFrameTrackingI3D QGLwglBeginFrameTrackingI3D

typedef BOOL (WINAPI* pfn_wglEndFrameTrackingI3D) (void);
extern pfn_wglEndFrameTrackingI3D QGLwglEndFrameTrackingI3D;
#define wglEndFrameTrackingI3D QGLwglEndFrameTrackingI3D

typedef BOOL (WINAPI* pfn_wglQueryFrameTrackingI3D) (DWORD*, DWORD*, float*);
extern pfn_wglQueryFrameTrackingI3D QGLwglQueryFrameTrackingI3D;
#define wglQueryFrameTrackingI3D QGLwglQueryFrameTrackingI3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_NV_render_depth_texture;
extern int QGLNumFunctions_WGL_NV_render_depth_texture;
#ifndef WGL_NV_render_depth_texture
#define WGL_NV_render_depth_texture
#define QGL_DEFINED_WGL_NV_render_depth_texture
#define WGL_BIND_TO_TEXTURE_DEPTH_NV            0x20A3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV  0x20A4
#define WGL_DEPTH_TEXTURE_FORMAT_NV             0x20A5
#define WGL_TEXTURE_DEPTH_COMPONENT_NV          0x20A6
#define WGL_DEPTH_COMPONENT_NV                  0x20A7
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_NV_render_texture_rectangle;
extern int QGLNumFunctions_WGL_NV_render_texture_rectangle;
#ifndef WGL_NV_render_texture_rectangle
#define WGL_NV_render_texture_rectangle
#define QGL_DEFINED_WGL_NV_render_texture_rectangle
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV    0x20A0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV   0x20A1
#define WGL_TEXTURE_RECTANGLE_NV                0x20A2
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_ATI_pixel_format_float;
extern int QGLNumFunctions_WGL_ATI_pixel_format_float;
#ifndef WGL_ATI_pixel_format_float
#define WGL_ATI_pixel_format_float
#define QGL_DEFINED_WGL_ATI_pixel_format_float
#define WGL_TYPE_RGBA_FLOAT_ATI        0x21A0
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_NV_float_buffer;
extern int QGLNumFunctions_WGL_NV_float_buffer;
#ifndef WGL_NV_float_buffer
#define WGL_NV_float_buffer
#define QGL_DEFINED_WGL_NV_float_buffer
#define WGL_FLOAT_COMPONENTS_NV                     0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV    0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV   0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV  0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV 0x20B4
#define WGL_TEXTURE_FLOAT_R_NV                      0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV                     0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV                    0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV                   0x20B8
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_display_color_table;
extern int QGLNumFunctions_WGL_EXT_display_color_table;
#ifndef WGL_EXT_display_color_table
#define WGL_EXT_display_color_table
#define QGL_DEFINED_WGL_EXT_display_color_table

typedef GLboolean (WINAPI* pfn_wglCreateDisplayColorTableEXT) (GLushort);
extern pfn_wglCreateDisplayColorTableEXT QGLwglCreateDisplayColorTableEXT;
#define wglCreateDisplayColorTableEXT QGLwglCreateDisplayColorTableEXT

typedef GLboolean (WINAPI* pfn_wglLoadDisplayColorTableEXT) (const GLushort*, GLuint);
extern pfn_wglLoadDisplayColorTableEXT QGLwglLoadDisplayColorTableEXT;
#define wglLoadDisplayColorTableEXT QGLwglLoadDisplayColorTableEXT

typedef GLboolean (WINAPI* pfn_wglBindDisplayColorTableEXT) (GLushort);
extern pfn_wglBindDisplayColorTableEXT QGLwglBindDisplayColorTableEXT;
#define wglBindDisplayColorTableEXT QGLwglBindDisplayColorTableEXT

typedef VOID (WINAPI* pfn_wglDestroyDisplayColorTableEXT) (GLushort);
extern pfn_wglDestroyDisplayColorTableEXT QGLwglDestroyDisplayColorTableEXT;
#define wglDestroyDisplayColorTableEXT QGLwglDestroyDisplayColorTableEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_extensions_string;
extern int QGLNumFunctions_WGL_EXT_extensions_string;
#ifndef WGL_EXT_extensions_string
#define WGL_EXT_extensions_string
#define QGL_DEFINED_WGL_EXT_extensions_string

typedef const char* (WINAPI* pfn_wglGetExtensionsStringEXT) (void);
extern pfn_wglGetExtensionsStringEXT QGLwglGetExtensionsStringEXT;
#define wglGetExtensionsStringEXT QGLwglGetExtensionsStringEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_EXT_swap_control;
extern int QGLNumFunctions_WGL_EXT_swap_control;
#ifndef WGL_EXT_swap_control
#define WGL_EXT_swap_control
#define QGL_DEFINED_WGL_EXT_swap_control

typedef BOOL (WINAPI* pfn_wglSwapIntervalEXT) (int);
extern pfn_wglSwapIntervalEXT QGLwglSwapIntervalEXT;
#define wglSwapIntervalEXT QGLwglSwapIntervalEXT

typedef int (WINAPI* pfn_wglGetSwapIntervalEXT) (void);
extern pfn_wglGetSwapIntervalEXT QGLwglGetSwapIntervalEXT;
#define wglGetSwapIntervalEXT QGLwglGetSwapIntervalEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_NV_vertex_array_range;
extern int QGLNumFunctions_WGL_NV_vertex_array_range;
#ifndef WGL_NV_vertex_array_range
#define WGL_NV_vertex_array_range
#define QGL_DEFINED_WGL_NV_vertex_array_range

typedef void* (WINAPI* pfn_wglAllocateMemoryNV) (GLsizei, GLfloat, GLfloat, GLfloat);
extern pfn_wglAllocateMemoryNV QGLwglAllocateMemoryNV;
#define wglAllocateMemoryNV QGLwglAllocateMemoryNV

typedef void (WINAPI* pfn_wglFreeMemoryNV) (void*);
extern pfn_wglFreeMemoryNV QGLwglFreeMemoryNV;
#define wglFreeMemoryNV QGLwglFreeMemoryNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_WGL_OML_sync_control;
extern int QGLNumFunctions_WGL_OML_sync_control;
#ifndef WGL_OML_sync_control
#define WGL_OML_sync_control
#define QGL_DEFINED_WGL_OML_sync_control

typedef BOOL (WINAPI* pfn_wglGetSyncValuesOML) (HDC, INT64*, INT64*, INT64*);
extern pfn_wglGetSyncValuesOML QGLwglGetSyncValuesOML;
#define wglGetSyncValuesOML QGLwglGetSyncValuesOML

typedef BOOL (WINAPI* pfn_wglGetMscRateOML) (HDC, INT32*, INT32*);
extern pfn_wglGetMscRateOML QGLwglGetMscRateOML;
#define wglGetMscRateOML QGLwglGetMscRateOML

typedef INT64 (WINAPI* pfn_wglSwapBuffersMscOML) (HDC, INT64, INT64, INT64);
extern pfn_wglSwapBuffersMscOML QGLwglSwapBuffersMscOML;
#define wglSwapBuffersMscOML QGLwglSwapBuffersMscOML

typedef INT64 (WINAPI* pfn_wglSwapLayerBuffersMscOML) (HDC, int, INT64, INT64, INT64);
extern pfn_wglSwapLayerBuffersMscOML QGLwglSwapLayerBuffersMscOML;
#define wglSwapLayerBuffersMscOML QGLwglSwapLayerBuffersMscOML

typedef BOOL (WINAPI* pfn_wglWaitForMscOML) (HDC, INT64, INT64, INT64, INT64*, INT64*, INT64*);
extern pfn_wglWaitForMscOML QGLwglWaitForMscOML;
#define wglWaitForMscOML QGLwglWaitForMscOML

typedef BOOL (WINAPI* pfn_wglWaitForSbcOML) (HDC, INT64, INT64*, INT64*, INT64*);
extern pfn_wglWaitForSbcOML QGLwglWaitForSbcOML;
#define wglWaitForSbcOML QGLwglWaitForSbcOML
#endif

//------------------------------------------------------------------------------------------------------------------
void InitializeWgl ();
//------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#ifdef QGL_DEFINED_WINAPI
#undef QGL_DEFINED_WINAPI
#undef WINAPI
#endif

#endif



