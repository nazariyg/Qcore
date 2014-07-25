#ifndef QOPENGLEXTENSIONS_H
#define QOPENGLEXTENSIONS_H

// The version of the file is: GL_GLEXT_VERSION 27

// The following block of code is used to prevent gl.h from being included on Linux. The OpenGL definitions
// are, therefore, provided by this extension wrapper.  This block of code and the definitions in the
// GL_VERSION_1_1 block are not necessary on _WIN32.

#if defined(__gl_h_) || defined(__GL_H__)
#error gl.h included before QOpenGLExtensions.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error glext.h included before QOpenGLExtensions.h
#endif
#if defined(__gl_ATI_h_)
#error glATI.h included before QOpenGLExtensions.h
#endif

#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

//------------------------------------------------------------------------------------------------------------------
#if defined(_WIN32)

#ifndef APIENTRY
#define QGL_DEFINED_APIENTRY
#if (defined(_MSC_VER) && (_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif

#ifndef CALLBACK
#define QGL_DEFINED_CALLBACK
#if (defined(_MSC_VER) && (_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED)
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif

#ifndef WINGDIAPI
#define QGL_DEFINED_WINGDIAPI
#define WINGDIAPI __declspec(dllimport)
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#else  // Apple, Linux
#define APIENTRY
#define CALLBACK
#define WINGDIAPI extern
#endif

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#include <stddef.h>
#endif

#define QGL_EXT_FULLY_EXISTS     2
#define QGL_EXT_PARTIALLY_EXISTS 1
#define QGL_EXT_DOES_NOT_EXIST   0

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_1_1;
extern int QGLNumFunctions_GL_VERSION_1_1;
#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1
#define QGL_DEFINED_GL_VERSION_1_1
#define GL_ACCUM                            0x0100
#define GL_LOAD                             0x0101
#define GL_RETURN                           0x0102
#define GL_MULT                             0x0103
#define GL_ADD                              0x0104
#define GL_NEVER                            0x0200
#define GL_LESS                             0x0201
#define GL_EQUAL                            0x0202
#define GL_LEQUAL                           0x0203
#define GL_GREATER                          0x0204
#define GL_NOTEQUAL                         0x0205
#define GL_GEQUAL                           0x0206
#define GL_ALWAYS                           0x0207
#define GL_CURRENT_BIT                      0x00000001
#define GL_POINT_BIT                        0x00000002
#define GL_LINE_BIT                         0x00000004
#define GL_POLYGON_BIT                      0x00000008
#define GL_POLYGON_STIPPLE_BIT              0x00000010
#define GL_PIXEL_MODE_BIT                   0x00000020
#define GL_LIGHTING_BIT                     0x00000040
#define GL_FOG_BIT                          0x00000080
#define GL_DEPTH_BUFFER_BIT                 0x00000100
#define GL_ACCUM_BUFFER_BIT                 0x00000200
#define GL_STENCIL_BUFFER_BIT               0x00000400
#define GL_VIEWPORT_BIT                     0x00000800
#define GL_TRANSFORM_BIT                    0x00001000
#define GL_ENABLE_BIT                       0x00002000
#define GL_COLOR_BUFFER_BIT                 0x00004000
#define GL_HINT_BIT                         0x00008000
#define GL_EVAL_BIT                         0x00010000
#define GL_LIST_BIT                         0x00020000
#define GL_TEXTURE_BIT                      0x00040000
#define GL_SCISSOR_BIT                      0x00080000
#define GL_ALL_ATTRIB_BITS                  0x000fffff
#define GL_POINTS                           0x0000
#define GL_LINES                            0x0001
#define GL_LINE_LOOP                        0x0002
#define GL_LINE_STRIP                       0x0003
#define GL_TRIANGLES                        0x0004
#define GL_TRIANGLE_STRIP                   0x0005
#define GL_TRIANGLE_FAN                     0x0006
#define GL_QUADS                            0x0007
#define GL_QUAD_STRIP                       0x0008
#define GL_POLYGON                          0x0009
#define GL_ZERO                             0
#define GL_ONE                              1
#define GL_SRC_COLOR                        0x0300
#define GL_ONE_MINUS_SRC_COLOR              0x0301
#define GL_SRC_ALPHA                        0x0302
#define GL_ONE_MINUS_SRC_ALPHA              0x0303
#define GL_DST_ALPHA                        0x0304
#define GL_ONE_MINUS_DST_ALPHA              0x0305
#define GL_DST_COLOR                        0x0306
#define GL_ONE_MINUS_DST_COLOR              0x0307
#define GL_SRC_ALPHA_SATURATE               0x0308
#define GL_TRUE                             1
#define GL_FALSE                            0
#define GL_CLIP_PLANE0                      0x3000
#define GL_CLIP_PLANE1                      0x3001
#define GL_CLIP_PLANE2                      0x3002
#define GL_CLIP_PLANE3                      0x3003
#define GL_CLIP_PLANE4                      0x3004
#define GL_CLIP_PLANE5                      0x3005
#define GL_BYTE                             0x1400
#define GL_UNSIGNED_BYTE                    0x1401
#define GL_SHORT                            0x1402
#define GL_UNSIGNED_SHORT                   0x1403
#define GL_INT                              0x1404
#define GL_UNSIGNED_INT                     0x1405
#define GL_FLOAT                            0x1406
#define GL_2_BYTES                          0x1407
#define GL_3_BYTES                          0x1408
#define GL_4_BYTES                          0x1409
#define GL_DOUBLE                           0x140A
#define GL_NONE                             0
#define GL_FRONT_LEFT                       0x0400
#define GL_FRONT_RIGHT                      0x0401
#define GL_BACK_LEFT                        0x0402
#define GL_BACK_RIGHT                       0x0403
#define GL_FRONT                            0x0404
#define GL_BACK                             0x0405
#define GL_LEFT                             0x0406
#define GL_RIGHT                            0x0407
#define GL_FRONT_AND_BACK                   0x0408
#define GL_AUX0                             0x0409
#define GL_AUX1                             0x040A
#define GL_AUX2                             0x040B
#define GL_AUX3                             0x040C
#define GL_NO_ERROR                         0
#define GL_INVALID_ENUM                     0x0500
#define GL_INVALID_VALUE                    0x0501
#define GL_INVALID_OPERATION                0x0502
#define GL_STACK_OVERFLOW                   0x0503
#define GL_STACK_UNDERFLOW                  0x0504
#define GL_OUT_OF_MEMORY                    0x0505
#define GL_2D                               0x0600
#define GL_3D                               0x0601
#define GL_3D_COLOR                         0x0602
#define GL_3D_COLOR_TEXTURE                 0x0603
#define GL_4D_COLOR_TEXTURE                 0x0604
#define GL_PASS_THROUGH_TOKEN               0x0700
#define GL_POINT_TOKEN                      0x0701
#define GL_LINE_TOKEN                       0x0702
#define GL_POLYGON_TOKEN                    0x0703
#define GL_BITMAP_TOKEN                     0x0704
#define GL_DRAW_PIXEL_TOKEN                 0x0705
#define GL_COPY_PIXEL_TOKEN                 0x0706
#define GL_LINE_RESET_TOKEN                 0x0707
#define GL_EXP                              0x0800
#define GL_EXP2                             0x0801
#define GL_CW                               0x0900
#define GL_CCW                              0x0901
#define GL_COEFF                            0x0A00
#define GL_ORDER                            0x0A01
#define GL_DOMAIN                           0x0A02
#define GL_CURRENT_COLOR                    0x0B00
#define GL_CURRENT_INDEX                    0x0B01
#define GL_CURRENT_NORMAL                   0x0B02
#define GL_CURRENT_TEXTURE_COORDS           0x0B03
#define GL_CURRENT_RASTER_COLOR             0x0B04
#define GL_CURRENT_RASTER_INDEX             0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS    0x0B06
#define GL_CURRENT_RASTER_POSITION          0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID    0x0B08
#define GL_CURRENT_RASTER_DISTANCE          0x0B09
#define GL_POINT_SMOOTH                     0x0B10
#define GL_POINT_SIZE                       0x0B11
#define GL_POINT_SIZE_RANGE                 0x0B12
#define GL_POINT_SIZE_GRANULARITY           0x0B13
#define GL_LINE_SMOOTH                      0x0B20
#define GL_LINE_WIDTH                       0x0B21
#define GL_LINE_WIDTH_RANGE                 0x0B22
#define GL_LINE_WIDTH_GRANULARITY           0x0B23
#define GL_LINE_STIPPLE                     0x0B24
#define GL_LINE_STIPPLE_PATTERN             0x0B25
#define GL_LINE_STIPPLE_REPEAT              0x0B26
#define GL_LIST_MODE                        0x0B30
#define GL_MAX_LIST_NESTING                 0x0B31
#define GL_LIST_BASE                        0x0B32
#define GL_LIST_INDEX                       0x0B33
#define GL_POLYGON_MODE                     0x0B40
#define GL_POLYGON_SMOOTH                   0x0B41
#define GL_POLYGON_STIPPLE                  0x0B42
#define GL_EDGE_FLAG                        0x0B43
#define GL_CULL_FACE                        0x0B44
#define GL_CULL_FACE_MODE                   0x0B45
#define GL_FRONT_FACE                       0x0B46
#define GL_LIGHTING                         0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER         0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE             0x0B52
#define GL_LIGHT_MODEL_AMBIENT              0x0B53
#define GL_SHADE_MODEL                      0x0B54
#define GL_COLOR_MATERIAL_FACE              0x0B55
#define GL_COLOR_MATERIAL_PARAMETER         0x0B56
#define GL_COLOR_MATERIAL                   0x0B57
#define GL_FOG                              0x0B60
#define GL_FOG_INDEX                        0x0B61
#define GL_FOG_DENSITY                      0x0B62
#define GL_FOG_START                        0x0B63
#define GL_FOG_END                          0x0B64
#define GL_FOG_MODE                         0x0B65
#define GL_FOG_COLOR                        0x0B66
#define GL_DEPTH_RANGE                      0x0B70
#define GL_DEPTH_TEST                       0x0B71
#define GL_DEPTH_WRITEMASK                  0x0B72
#define GL_DEPTH_CLEAR_VALUE                0x0B73
#define GL_DEPTH_FUNC                       0x0B74
#define GL_ACCUM_CLEAR_VALUE                0x0B80
#define GL_STENCIL_TEST                     0x0B90
#define GL_STENCIL_CLEAR_VALUE              0x0B91
#define GL_STENCIL_FUNC                     0x0B92
#define GL_STENCIL_VALUE_MASK               0x0B93
#define GL_STENCIL_FAIL                     0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL          0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS          0x0B96
#define GL_STENCIL_REF                      0x0B97
#define GL_STENCIL_WRITEMASK                0x0B98
#define GL_MATRIX_MODE                      0x0BA0
#define GL_NORMALIZE                        0x0BA1
#define GL_VIEWPORT                         0x0BA2
#define GL_MODELVIEW_STACK_DEPTH            0x0BA3
#define GL_PROJECTION_STACK_DEPTH           0x0BA4
#define GL_TEXTURE_STACK_DEPTH              0x0BA5
#define GL_MODELVIEW_MATRIX                 0x0BA6
#define GL_PROJECTION_MATRIX                0x0BA7
#define GL_TEXTURE_MATRIX                   0x0BA8
#define GL_ATTRIB_STACK_DEPTH               0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH        0x0BB1
#define GL_ALPHA_TEST                       0x0BC0
#define GL_ALPHA_TEST_FUNC                  0x0BC1
#define GL_ALPHA_TEST_REF                   0x0BC2
#define GL_DITHER                           0x0BD0
#define GL_BLEND_DST                        0x0BE0
#define GL_BLEND_SRC                        0x0BE1
#define GL_BLEND                            0x0BE2
#define GL_LOGIC_OP_MODE                    0x0BF0
#define GL_INDEX_LOGIC_OP                   0x0BF1
#define GL_COLOR_LOGIC_OP                   0x0BF2
#define GL_AUX_BUFFERS                      0x0C00
#define GL_DRAW_BUFFER                      0x0C01
#define GL_READ_BUFFER                      0x0C02
#define GL_SCISSOR_BOX                      0x0C10
#define GL_SCISSOR_TEST                     0x0C11
#define GL_INDEX_CLEAR_VALUE                0x0C20
#define GL_INDEX_WRITEMASK                  0x0C21
#define GL_COLOR_CLEAR_VALUE                0x0C22
#define GL_COLOR_WRITEMASK                  0x0C23
#define GL_INDEX_MODE                       0x0C30
#define GL_RGBA_MODE                        0x0C31
#define GL_DOUBLEBUFFER                     0x0C32
#define GL_STEREO                           0x0C33
#define GL_RENDER_MODE                      0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT      0x0C50
#define GL_POINT_SMOOTH_HINT                0x0C51
#define GL_LINE_SMOOTH_HINT                 0x0C52
#define GL_POLYGON_SMOOTH_HINT              0x0C53
#define GL_FOG_HINT                         0x0C54
#define GL_TEXTURE_GEN_S                    0x0C60
#define GL_TEXTURE_GEN_T                    0x0C61
#define GL_TEXTURE_GEN_R                    0x0C62
#define GL_TEXTURE_GEN_Q                    0x0C63
#define GL_PIXEL_MAP_I_TO_I                 0x0C70
#define GL_PIXEL_MAP_S_TO_S                 0x0C71
#define GL_PIXEL_MAP_I_TO_R                 0x0C72
#define GL_PIXEL_MAP_I_TO_G                 0x0C73
#define GL_PIXEL_MAP_I_TO_B                 0x0C74
#define GL_PIXEL_MAP_I_TO_A                 0x0C75
#define GL_PIXEL_MAP_R_TO_R                 0x0C76
#define GL_PIXEL_MAP_G_TO_G                 0x0C77
#define GL_PIXEL_MAP_B_TO_B                 0x0C78
#define GL_PIXEL_MAP_A_TO_A                 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE            0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE            0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE            0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE            0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE            0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE            0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE            0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE            0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE            0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE            0x0CB9
#define GL_UNPACK_SWAP_BYTES                0x0CF0
#define GL_UNPACK_LSB_FIRST                 0x0CF1
#define GL_UNPACK_ROW_LENGTH                0x0CF2
#define GL_UNPACK_SKIP_ROWS                 0x0CF3
#define GL_UNPACK_SKIP_PIXELS               0x0CF4
#define GL_UNPACK_ALIGNMENT                 0x0CF5
#define GL_PACK_SWAP_BYTES                  0x0D00
#define GL_PACK_LSB_FIRST                   0x0D01
#define GL_PACK_ROW_LENGTH                  0x0D02
#define GL_PACK_SKIP_ROWS                   0x0D03
#define GL_PACK_SKIP_PIXELS                 0x0D04
#define GL_PACK_ALIGNMENT                   0x0D05
#define GL_MAP_COLOR                        0x0D10
#define GL_MAP_STENCIL                      0x0D11
#define GL_INDEX_SHIFT                      0x0D12
#define GL_INDEX_OFFSET                     0x0D13
#define GL_RED_SCALE                        0x0D14
#define GL_RED_BIAS                         0x0D15
#define GL_ZOOM_X                           0x0D16
#define GL_ZOOM_Y                           0x0D17
#define GL_GREEN_SCALE                      0x0D18
#define GL_GREEN_BIAS                       0x0D19
#define GL_BLUE_SCALE                       0x0D1A
#define GL_BLUE_BIAS                        0x0D1B
#define GL_ALPHA_SCALE                      0x0D1C
#define GL_ALPHA_BIAS                       0x0D1D
#define GL_DEPTH_SCALE                      0x0D1E
#define GL_DEPTH_BIAS                       0x0D1F
#define GL_MAX_EVAL_ORDER                   0x0D30
#define GL_MAX_LIGHTS                       0x0D31
#define GL_MAX_CLIP_PLANES                  0x0D32
#define GL_MAX_TEXTURE_SIZE                 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE              0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH           0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH        0x0D36
#define GL_MAX_NAME_STACK_DEPTH             0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH       0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH          0x0D39
#define GL_MAX_VIEWPORT_DIMS                0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH    0x0D3B
#define GL_SUBPIXEL_BITS                    0x0D50
#define GL_INDEX_BITS                       0x0D51
#define GL_RED_BITS                         0x0D52
#define GL_GREEN_BITS                       0x0D53
#define GL_BLUE_BITS                        0x0D54
#define GL_ALPHA_BITS                       0x0D55
#define GL_DEPTH_BITS                       0x0D56
#define GL_STENCIL_BITS                     0x0D57
#define GL_ACCUM_RED_BITS                   0x0D58
#define GL_ACCUM_GREEN_BITS                 0x0D59
#define GL_ACCUM_BLUE_BITS                  0x0D5A
#define GL_ACCUM_ALPHA_BITS                 0x0D5B
#define GL_NAME_STACK_DEPTH                 0x0D70
#define GL_AUTO_NORMAL                      0x0D80
#define GL_MAP1_COLOR_4                     0x0D90
#define GL_MAP1_INDEX                       0x0D91
#define GL_MAP1_NORMAL                      0x0D92
#define GL_MAP1_TEXTURE_COORD_1             0x0D93
#define GL_MAP1_TEXTURE_COORD_2             0x0D94
#define GL_MAP1_TEXTURE_COORD_3             0x0D95
#define GL_MAP1_TEXTURE_COORD_4             0x0D96
#define GL_MAP1_VERTEX_3                    0x0D97
#define GL_MAP1_VERTEX_4                    0x0D98
#define GL_MAP2_COLOR_4                     0x0DB0
#define GL_MAP2_INDEX                       0x0DB1
#define GL_MAP2_NORMAL                      0x0DB2
#define GL_MAP2_TEXTURE_COORD_1             0x0DB3
#define GL_MAP2_TEXTURE_COORD_2             0x0DB4
#define GL_MAP2_TEXTURE_COORD_3             0x0DB5
#define GL_MAP2_TEXTURE_COORD_4             0x0DB6
#define GL_MAP2_VERTEX_3                    0x0DB7
#define GL_MAP2_VERTEX_4                    0x0DB8
#define GL_MAP1_GRID_DOMAIN                 0x0DD0
#define GL_MAP1_GRID_SEGMENTS               0x0DD1
#define GL_MAP2_GRID_DOMAIN                 0x0DD2
#define GL_MAP2_GRID_SEGMENTS               0x0DD3
#define GL_TEXTURE_1D                       0x0DE0
#define GL_TEXTURE_2D                       0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER          0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE             0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE             0x0DF2
#define GL_SELECTION_BUFFER_POINTER         0x0DF3
#define GL_SELECTION_BUFFER_SIZE            0x0DF4
#define GL_TEXTURE_WIDTH                    0x1000
#define GL_TEXTURE_HEIGHT                   0x1001
#define GL_TEXTURE_INTERNAL_FORMAT          0x1003
#define GL_TEXTURE_BORDER_COLOR             0x1004
#define GL_TEXTURE_BORDER                   0x1005
#define GL_DONT_CARE                        0x1100
#define GL_FASTEST                          0x1101
#define GL_NICEST                           0x1102
#define GL_LIGHT0                           0x4000
#define GL_LIGHT1                           0x4001
#define GL_LIGHT2                           0x4002
#define GL_LIGHT3                           0x4003
#define GL_LIGHT4                           0x4004
#define GL_LIGHT5                           0x4005
#define GL_LIGHT6                           0x4006
#define GL_LIGHT7                           0x4007
#define GL_AMBIENT                          0x1200
#define GL_DIFFUSE                          0x1201
#define GL_SPECULAR                         0x1202
#define GL_POSITION                         0x1203
#define GL_SPOT_DIRECTION                   0x1204
#define GL_SPOT_EXPONENT                    0x1205
#define GL_SPOT_CUTOFF                      0x1206
#define GL_CONSTANT_ATTENUATION             0x1207
#define GL_LINEAR_ATTENUATION               0x1208
#define GL_QUADRATIC_ATTENUATION            0x1209
#define GL_COMPILE                          0x1300
#define GL_COMPILE_AND_EXECUTE              0x1301
#define GL_CLEAR                            0x1500
#define GL_AND                              0x1501
#define GL_AND_REVERSE                      0x1502
#define GL_COPY                             0x1503
#define GL_AND_INVERTED                     0x1504
#define GL_NOOP                             0x1505
#define GL_XOR                              0x1506
#define GL_OR                               0x1507
#define GL_NOR                              0x1508
#define GL_EQUIV                            0x1509
#define GL_INVERT                           0x150A
#define GL_OR_REVERSE                       0x150B
#define GL_COPY_INVERTED                    0x150C
#define GL_OR_INVERTED                      0x150D
#define GL_NAND                             0x150E
#define GL_SET                              0x150F
#define GL_EMISSION                         0x1600
#define GL_SHININESS                        0x1601
#define GL_AMBIENT_AND_DIFFUSE              0x1602
#define GL_COLOR_INDEXES                    0x1603
#define GL_MODELVIEW                        0x1700
#define GL_PROJECTION                       0x1701
#define GL_TEXTURE                          0x1702
#define GL_COLOR                            0x1800
#define GL_DEPTH                            0x1801
#define GL_STENCIL                          0x1802
#define GL_COLOR_INDEX                      0x1900
#define GL_STENCIL_INDEX                    0x1901
#define GL_DEPTH_COMPONENT                  0x1902
#define GL_RED                              0x1903
#define GL_GREEN                            0x1904
#define GL_BLUE                             0x1905
#define GL_ALPHA                            0x1906
#define GL_RGB                              0x1907
#define GL_RGBA                             0x1908
#define GL_LUMINANCE                        0x1909
#define GL_LUMINANCE_ALPHA                  0x190A
#define GL_BITMAP                           0x1A00
#define GL_POINT                            0x1B00
#define GL_LINE                             0x1B01
#define GL_FILL                             0x1B02
#define GL_RENDER                           0x1C00
#define GL_FEEDBACK                         0x1C01
#define GL_SELECT                           0x1C02
#define GL_FLAT                             0x1D00
#define GL_SMOOTH                           0x1D01
#define GL_KEEP                             0x1E00
#define GL_REPLACE                          0x1E01
#define GL_INCR                             0x1E02
#define GL_DECR                             0x1E03
#define GL_VENDOR                           0x1F00
#define GL_RENDERER                         0x1F01
#define GL_VERSION                          0x1F02
#define GL_EXTENSIONS                       0x1F03
#define GL_S                                0x2000
#define GL_T                                0x2001
#define GL_R                                0x2002
#define GL_Q                                0x2003
#define GL_MODULATE                         0x2100
#define GL_DECAL                            0x2101
#define GL_TEXTURE_ENV_MODE                 0x2200
#define GL_TEXTURE_ENV_COLOR                0x2201
#define GL_TEXTURE_ENV                      0x2300
#define GL_EYE_LINEAR                       0x2400
#define GL_OBJECT_LINEAR                    0x2401
#define GL_SPHERE_MAP                       0x2402
#define GL_TEXTURE_GEN_MODE                 0x2500
#define GL_OBJECT_PLANE                     0x2501
#define GL_EYE_PLANE                        0x2502
#define GL_NEAREST                          0x2600
#define GL_LINEAR                           0x2601
#define GL_NEAREST_MIPMAP_NEAREST           0x2700
#define GL_LINEAR_MIPMAP_NEAREST            0x2701
#define GL_NEAREST_MIPMAP_LINEAR            0x2702
#define GL_LINEAR_MIPMAP_LINEAR             0x2703
#define GL_TEXTURE_MAG_FILTER               0x2800
#define GL_TEXTURE_MIN_FILTER               0x2801
#define GL_TEXTURE_WRAP_S                   0x2802
#define GL_TEXTURE_WRAP_T                   0x2803
#define GL_CLAMP                            0x2900
#define GL_REPEAT                           0x2901
#define GL_CLIENT_PIXEL_STORE_BIT           0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT          0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS           0xffffffff
#define GL_POLYGON_OFFSET_FACTOR            0x8038
#define GL_POLYGON_OFFSET_UNITS             0x2A00
#define GL_POLYGON_OFFSET_POINT             0x2A01
#define GL_POLYGON_OFFSET_LINE              0x2A02
#define GL_POLYGON_OFFSET_FILL              0x8037
#define GL_ALPHA4                           0x803B
#define GL_ALPHA8                           0x803C
#define GL_ALPHA12                          0x803D
#define GL_ALPHA16                          0x803E
#define GL_LUMINANCE4                       0x803F
#define GL_LUMINANCE8                       0x8040
#define GL_LUMINANCE12                      0x8041
#define GL_LUMINANCE16                      0x8042
#define GL_LUMINANCE4_ALPHA4                0x8043
#define GL_LUMINANCE6_ALPHA2                0x8044
#define GL_LUMINANCE8_ALPHA8                0x8045
#define GL_LUMINANCE12_ALPHA4               0x8046
#define GL_LUMINANCE12_ALPHA12              0x8047
#define GL_LUMINANCE16_ALPHA16              0x8048
#define GL_INTENSITY                        0x8049
#define GL_INTENSITY4                       0x804A
#define GL_INTENSITY8                       0x804B
#define GL_INTENSITY12                      0x804C
#define GL_INTENSITY16                      0x804D
#define GL_R3_G3_B2                         0x2A10
#define GL_RGB4                             0x804F
#define GL_RGB5                             0x8050
#define GL_RGB8                             0x8051
#define GL_RGB10                            0x8052
#define GL_RGB12                            0x8053
#define GL_RGB16                            0x8054
#define GL_RGBA2                            0x8055
#define GL_RGBA4                            0x8056
#define GL_RGB5_A1                          0x8057
#define GL_RGBA8                            0x8058
#define GL_RGB10_A2                         0x8059
#define GL_RGBA12                           0x805A
#define GL_RGBA16                           0x805B
#define GL_TEXTURE_RED_SIZE                 0x805C
#define GL_TEXTURE_GREEN_SIZE               0x805D
#define GL_TEXTURE_BLUE_SIZE                0x805E
#define GL_TEXTURE_ALPHA_SIZE               0x805F
#define GL_TEXTURE_LUMINANCE_SIZE           0x8060
#define GL_TEXTURE_INTENSITY_SIZE           0x8061
#define GL_PROXY_TEXTURE_1D                 0x8063
#define GL_PROXY_TEXTURE_2D                 0x8064
#define GL_TEXTURE_PRIORITY                 0x8066
#define GL_TEXTURE_RESIDENT                 0x8067
#define GL_TEXTURE_BINDING_1D               0x8068
#define GL_TEXTURE_BINDING_2D               0x8069
#define GL_VERTEX_ARRAY                     0x8074
#define GL_NORMAL_ARRAY                     0x8075
#define GL_COLOR_ARRAY                      0x8076
#define GL_INDEX_ARRAY                      0x8077
#define GL_TEXTURE_COORD_ARRAY              0x8078
#define GL_EDGE_FLAG_ARRAY                  0x8079
#define GL_VERTEX_ARRAY_SIZE                0x807A
#define GL_VERTEX_ARRAY_TYPE                0x807B
#define GL_VERTEX_ARRAY_STRIDE              0x807C
#define GL_NORMAL_ARRAY_TYPE                0x807E
#define GL_NORMAL_ARRAY_STRIDE              0x807F
#define GL_COLOR_ARRAY_SIZE                 0x8081
#define GL_COLOR_ARRAY_TYPE                 0x8082
#define GL_COLOR_ARRAY_STRIDE               0x8083
#define GL_INDEX_ARRAY_TYPE                 0x8085
#define GL_INDEX_ARRAY_STRIDE               0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE         0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE         0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE       0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE           0x808C
#define GL_VERTEX_ARRAY_POINTER             0x808E
#define GL_NORMAL_ARRAY_POINTER             0x808F
#define GL_COLOR_ARRAY_POINTER              0x8090
#define GL_INDEX_ARRAY_POINTER              0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER      0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER          0x8093
#define GL_V2F                              0x2A20
#define GL_V3F                              0x2A21
#define GL_C4UB_V2F                         0x2A22
#define GL_C4UB_V3F                         0x2A23
#define GL_C3F_V3F                          0x2A24
#define GL_N3F_V3F                          0x2A25
#define GL_C4F_N3F_V3F                      0x2A26
#define GL_T2F_V3F                          0x2A27
#define GL_T4F_V4F                          0x2A28
#define GL_T2F_C4UB_V3F                     0x2A29
#define GL_T2F_C3F_V3F                      0x2A2A
#define GL_T2F_N3F_V3F                      0x2A2B
#define GL_T2F_C4F_N3F_V3F                  0x2A2C
#define GL_T4F_C4F_N3F_V4F                  0x2A2D
#define GL_LOGIC_OP                         GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS               GL_TEXTURE_INTERNAL_FORMAT
#define GL_COLOR_INDEX1_EXT                 0x80E2
#define GL_COLOR_INDEX2_EXT                 0x80E3
#define GL_COLOR_INDEX4_EXT                 0x80E4
#define GL_COLOR_INDEX8_EXT                 0x80E5
#define GL_COLOR_INDEX12_EXT                0x80E6
#define GL_COLOR_INDEX16_EXT                0x80E7
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
WINGDIAPI void APIENTRY glAccum (GLenum, GLfloat);
WINGDIAPI void APIENTRY glAlphaFunc (GLenum, GLclampf);
WINGDIAPI GLboolean APIENTRY glAreTexturesResident (GLsizei, const GLuint*, GLboolean*);
WINGDIAPI void APIENTRY glArrayElement (GLint);
WINGDIAPI void APIENTRY glBegin (GLenum);
WINGDIAPI void APIENTRY glBindTexture (GLenum, GLuint);
WINGDIAPI void APIENTRY glBitmap (GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte*);
WINGDIAPI void APIENTRY glBlendFunc (GLenum, GLenum);
WINGDIAPI void APIENTRY glCallList (GLuint);
WINGDIAPI void APIENTRY glCallLists (GLsizei, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glClear (GLbitfield);
WINGDIAPI void APIENTRY glClearAccum (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glClearColor (GLclampf, GLclampf, GLclampf, GLclampf);
WINGDIAPI void APIENTRY glClearDepth (GLclampd);
WINGDIAPI void APIENTRY glClearIndex (GLfloat);
WINGDIAPI void APIENTRY glClearStencil (GLint);
WINGDIAPI void APIENTRY glClipPlane (GLenum, const GLdouble*);
WINGDIAPI void APIENTRY glColor3b (GLbyte, GLbyte, GLbyte);
WINGDIAPI void APIENTRY glColor3bv (const GLbyte*);
WINGDIAPI void APIENTRY glColor3d (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glColor3dv (const GLdouble*);
WINGDIAPI void APIENTRY glColor3f (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glColor3fv (const GLfloat*);
WINGDIAPI void APIENTRY glColor3i (GLint, GLint, GLint);
WINGDIAPI void APIENTRY glColor3iv (const GLint*);
WINGDIAPI void APIENTRY glColor3s (GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glColor3sv (const GLshort*);
WINGDIAPI void APIENTRY glColor3ub (GLubyte, GLubyte, GLubyte);
WINGDIAPI void APIENTRY glColor3ubv (const GLubyte*);
WINGDIAPI void APIENTRY glColor3ui (GLuint, GLuint, GLuint);
WINGDIAPI void APIENTRY glColor3uiv (const GLuint*);
WINGDIAPI void APIENTRY glColor3us (GLushort, GLushort, GLushort);
WINGDIAPI void APIENTRY glColor3usv (const GLushort*);
WINGDIAPI void APIENTRY glColor4b (GLbyte, GLbyte, GLbyte, GLbyte);
WINGDIAPI void APIENTRY glColor4bv (const GLbyte*);
WINGDIAPI void APIENTRY glColor4d (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glColor4dv (const GLdouble*);
WINGDIAPI void APIENTRY glColor4f (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glColor4fv (const GLfloat*);
WINGDIAPI void APIENTRY glColor4i (GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glColor4iv (const GLint*);
WINGDIAPI void APIENTRY glColor4s (GLshort, GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glColor4sv (const GLshort*);
WINGDIAPI void APIENTRY glColor4ub (GLubyte, GLubyte, GLubyte, GLubyte);
WINGDIAPI void APIENTRY glColor4ubv (const GLubyte*);
WINGDIAPI void APIENTRY glColor4ui (GLuint, GLuint, GLuint, GLuint);
WINGDIAPI void APIENTRY glColor4uiv (const GLuint*);
WINGDIAPI void APIENTRY glColor4us (GLushort, GLushort, GLushort, GLushort);
WINGDIAPI void APIENTRY glColor4usv (const GLushort*);
WINGDIAPI void APIENTRY glColorMask (GLboolean, GLboolean, GLboolean, GLboolean);
WINGDIAPI void APIENTRY glColorMaterial (GLenum, GLenum);
WINGDIAPI void APIENTRY glColorPointer (GLint, GLenum, GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glCopyPixels (GLint, GLint, GLsizei, GLsizei, GLenum);
WINGDIAPI void APIENTRY glCopyTexImage1D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
WINGDIAPI void APIENTRY glCopyTexImage2D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
WINGDIAPI void APIENTRY glCopyTexSubImage1D (GLenum, GLint, GLint, GLint, GLint, GLsizei);
WINGDIAPI void APIENTRY glCopyTexSubImage2D (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
WINGDIAPI void APIENTRY glCullFace (GLenum);
WINGDIAPI void APIENTRY glDeleteLists (GLuint, GLsizei);
WINGDIAPI void APIENTRY glDeleteTextures (GLsizei, const GLuint*);
WINGDIAPI void APIENTRY glDepthFunc (GLenum);
WINGDIAPI void APIENTRY glDepthMask (GLboolean);
WINGDIAPI void APIENTRY glDepthRange (GLclampd, GLclampd);
WINGDIAPI void APIENTRY glDisable (GLenum);
WINGDIAPI void APIENTRY glDisableClientState (GLenum);
WINGDIAPI void APIENTRY glDrawArrays (GLenum, GLint, GLsizei);
WINGDIAPI void APIENTRY glDrawBuffer (GLenum);
WINGDIAPI void APIENTRY glDrawElements (GLenum, GLsizei, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glDrawPixels (GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glEdgeFlag (GLboolean);
WINGDIAPI void APIENTRY glEdgeFlagPointer (GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glEdgeFlagv (const GLboolean*);
WINGDIAPI void APIENTRY glEnable (GLenum);
WINGDIAPI void APIENTRY glEnableClientState (GLenum);
WINGDIAPI void APIENTRY glEnd (void);
WINGDIAPI void APIENTRY glEndList (void);
WINGDIAPI void APIENTRY glEvalCoord1d (GLdouble);
WINGDIAPI void APIENTRY glEvalCoord1dv (const GLdouble*);
WINGDIAPI void APIENTRY glEvalCoord1f (GLfloat);
WINGDIAPI void APIENTRY glEvalCoord1fv (const GLfloat*);
WINGDIAPI void APIENTRY glEvalCoord2d (GLdouble, GLdouble);
WINGDIAPI void APIENTRY glEvalCoord2dv (const GLdouble*);
WINGDIAPI void APIENTRY glEvalCoord2f (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glEvalCoord2fv (const GLfloat*);
WINGDIAPI void APIENTRY glEvalMesh1 (GLenum, GLint, GLint);
WINGDIAPI void APIENTRY glEvalMesh2 (GLenum, GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glEvalPoint1 (GLint);
WINGDIAPI void APIENTRY glEvalPoint2 (GLint, GLint);
WINGDIAPI void APIENTRY glFeedbackBuffer (GLsizei, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glFinish (void);
WINGDIAPI void APIENTRY glFlush (void);
WINGDIAPI void APIENTRY glFogf (GLenum, GLfloat);
WINGDIAPI void APIENTRY glFogfv (GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glFogi (GLenum, GLint);
WINGDIAPI void APIENTRY glFogiv (GLenum, const GLint*);
WINGDIAPI void APIENTRY glFrontFace (GLenum);
WINGDIAPI void APIENTRY glFrustum (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI GLuint APIENTRY glGenLists (GLsizei);
WINGDIAPI void APIENTRY glGenTextures (GLsizei, GLuint*);
WINGDIAPI void APIENTRY glGetBooleanv (GLenum, GLboolean*);
WINGDIAPI void APIENTRY glGetClipPlane (GLenum, GLdouble*);
WINGDIAPI void APIENTRY glGetDoublev (GLenum, GLdouble*);
WINGDIAPI GLenum APIENTRY glGetError (void);
WINGDIAPI void APIENTRY glGetFloatv (GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetIntegerv (GLenum, GLint*);
WINGDIAPI void APIENTRY glGetLightfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetLightiv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetMapdv (GLenum, GLenum, GLdouble*);
WINGDIAPI void APIENTRY glGetMapfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetMapiv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetMaterialfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetMaterialiv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetPixelMapfv (GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetPixelMapuiv (GLenum, GLuint*);
WINGDIAPI void APIENTRY glGetPixelMapusv (GLenum, GLushort*);
WINGDIAPI void APIENTRY glGetPointerv (GLenum, GLvoid**);
WINGDIAPI void APIENTRY glGetPolygonStipple (GLubyte*);
WINGDIAPI const GLubyte* APIENTRY glGetString (GLenum);
WINGDIAPI void APIENTRY glGetTexEnvfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetTexEnviv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetTexGendv (GLenum, GLenum, GLdouble*);
WINGDIAPI void APIENTRY glGetTexGenfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetTexGeniv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetTexImage (GLenum, GLint, GLenum, GLenum, GLvoid*);
WINGDIAPI void APIENTRY glGetTexLevelParameterfv (GLenum, GLint, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetTexLevelParameteriv (GLenum, GLint, GLenum, GLint*);
WINGDIAPI void APIENTRY glGetTexParameterfv (GLenum, GLenum, GLfloat*);
WINGDIAPI void APIENTRY glGetTexParameteriv (GLenum, GLenum, GLint*);
WINGDIAPI void APIENTRY glHint (GLenum, GLenum);
WINGDIAPI void APIENTRY glIndexMask (GLuint);
WINGDIAPI void APIENTRY glIndexPointer (GLenum, GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glIndexd (GLdouble);
WINGDIAPI void APIENTRY glIndexdv (const GLdouble*);
WINGDIAPI void APIENTRY glIndexf (GLfloat);
WINGDIAPI void APIENTRY glIndexfv (const GLfloat*);
WINGDIAPI void APIENTRY glIndexi (GLint);
WINGDIAPI void APIENTRY glIndexiv (const GLint*);
WINGDIAPI void APIENTRY glIndexs (GLshort);
WINGDIAPI void APIENTRY glIndexsv (const GLshort*);
WINGDIAPI void APIENTRY glIndexub (GLubyte);
WINGDIAPI void APIENTRY glIndexubv (const GLubyte*);
WINGDIAPI void APIENTRY glInitNames (void);
WINGDIAPI void APIENTRY glInterleavedArrays (GLenum, GLsizei, const GLvoid*);
WINGDIAPI GLboolean APIENTRY glIsEnabled (GLenum);
WINGDIAPI GLboolean APIENTRY glIsList (GLuint);
WINGDIAPI GLboolean APIENTRY glIsTexture (GLuint);
WINGDIAPI void APIENTRY glLightModelf (GLenum, GLfloat);
WINGDIAPI void APIENTRY glLightModelfv (GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glLightModeli (GLenum, GLint);
WINGDIAPI void APIENTRY glLightModeliv (GLenum, const GLint*);
WINGDIAPI void APIENTRY glLightf (GLenum, GLenum, GLfloat);
WINGDIAPI void APIENTRY glLightfv (GLenum, GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glLighti (GLenum, GLenum, GLint);
WINGDIAPI void APIENTRY glLightiv (GLenum, GLenum, const GLint*);
WINGDIAPI void APIENTRY glLineStipple (GLint, GLushort);
WINGDIAPI void APIENTRY glLineWidth (GLfloat);
WINGDIAPI void APIENTRY glListBase (GLuint);
WINGDIAPI void APIENTRY glLoadIdentity (void);
WINGDIAPI void APIENTRY glLoadMatrixd (const GLdouble*);
WINGDIAPI void APIENTRY glLoadMatrixf (const GLfloat*);
WINGDIAPI void APIENTRY glLoadName (GLuint);
WINGDIAPI void APIENTRY glLogicOp (GLenum);
WINGDIAPI void APIENTRY glMap1d (GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble*);
WINGDIAPI void APIENTRY glMap1f (GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat*);
WINGDIAPI void APIENTRY glMap2d (GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble*);
WINGDIAPI void APIENTRY glMap2f (GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat*);
WINGDIAPI void APIENTRY glMapGrid1d (GLint, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glMapGrid1f (GLint, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glMapGrid2d (GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glMapGrid2f (GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glMaterialf (GLenum, GLenum, GLfloat);
WINGDIAPI void APIENTRY glMaterialfv (GLenum, GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glMateriali (GLenum, GLenum, GLint);
WINGDIAPI void APIENTRY glMaterialiv (GLenum, GLenum, const GLint*);
WINGDIAPI void APIENTRY glMatrixMode (GLenum);
WINGDIAPI void APIENTRY glMultMatrixd (const GLdouble*);
WINGDIAPI void APIENTRY glMultMatrixf (const GLfloat*);
WINGDIAPI void APIENTRY glNewList (GLuint, GLenum);
WINGDIAPI void APIENTRY glNormal3b (GLbyte, GLbyte, GLbyte);
WINGDIAPI void APIENTRY glNormal3bv (const GLbyte*);
WINGDIAPI void APIENTRY glNormal3d (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glNormal3dv (const GLdouble*);
WINGDIAPI void APIENTRY glNormal3f (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glNormal3fv (const GLfloat*);
WINGDIAPI void APIENTRY glNormal3i (GLint, GLint, GLint);
WINGDIAPI void APIENTRY glNormal3iv (const GLint*);
WINGDIAPI void APIENTRY glNormal3s (GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glNormal3sv (const GLshort*);
WINGDIAPI void APIENTRY glNormalPointer (GLenum, GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glOrtho (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glPassThrough (GLfloat);
WINGDIAPI void APIENTRY glPixelMapfv (GLenum, GLsizei, const GLfloat*);
WINGDIAPI void APIENTRY glPixelMapuiv (GLenum, GLsizei, const GLuint*);
WINGDIAPI void APIENTRY glPixelMapusv (GLenum, GLsizei, const GLushort*);
WINGDIAPI void APIENTRY glPixelStoref (GLenum, GLfloat);
WINGDIAPI void APIENTRY glPixelStorei (GLenum, GLint);
WINGDIAPI void APIENTRY glPixelTransferf (GLenum, GLfloat);
WINGDIAPI void APIENTRY glPixelTransferi (GLenum, GLint);
WINGDIAPI void APIENTRY glPixelZoom (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glPointSize (GLfloat);
WINGDIAPI void APIENTRY glPolygonMode (GLenum, GLenum);
WINGDIAPI void APIENTRY glPolygonOffset (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glPolygonStipple (const GLubyte*);
WINGDIAPI void APIENTRY glPopAttrib (void);
WINGDIAPI void APIENTRY glPopClientAttrib (void);
WINGDIAPI void APIENTRY glPopMatrix (void);
WINGDIAPI void APIENTRY glPopName (void);
WINGDIAPI void APIENTRY glPrioritizeTextures (GLsizei, const GLuint*, const GLclampf*);
WINGDIAPI void APIENTRY glPushAttrib (GLbitfield);
WINGDIAPI void APIENTRY glPushClientAttrib (GLbitfield);
WINGDIAPI void APIENTRY glPushMatrix (void);
WINGDIAPI void APIENTRY glPushName (GLuint);
WINGDIAPI void APIENTRY glRasterPos2d (GLdouble, GLdouble);
WINGDIAPI void APIENTRY glRasterPos2dv (const GLdouble*);
WINGDIAPI void APIENTRY glRasterPos2f (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glRasterPos2fv (const GLfloat*);
WINGDIAPI void APIENTRY glRasterPos2i (GLint, GLint);
WINGDIAPI void APIENTRY glRasterPos2iv (const GLint*);
WINGDIAPI void APIENTRY glRasterPos2s (GLshort, GLshort);
WINGDIAPI void APIENTRY glRasterPos2sv (const GLshort *);
WINGDIAPI void APIENTRY glRasterPos3d (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glRasterPos3dv (const GLdouble*);
WINGDIAPI void APIENTRY glRasterPos3f (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glRasterPos3fv (const GLfloat*);
WINGDIAPI void APIENTRY glRasterPos3i (GLint, GLint, GLint);
WINGDIAPI void APIENTRY glRasterPos3iv (const GLint*);
WINGDIAPI void APIENTRY glRasterPos3s (GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glRasterPos3sv (const GLshort*);
WINGDIAPI void APIENTRY glRasterPos4d (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glRasterPos4dv (const GLdouble*);
WINGDIAPI void APIENTRY glRasterPos4f (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glRasterPos4fv (const GLfloat*);
WINGDIAPI void APIENTRY glRasterPos4i (GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glRasterPos4iv (const GLint*);
WINGDIAPI void APIENTRY glRasterPos4s (GLshort, GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glRasterPos4sv (const GLshort*);
WINGDIAPI void APIENTRY glReadBuffer (GLenum);
WINGDIAPI void APIENTRY glReadPixels (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*);
WINGDIAPI void APIENTRY glRectd (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glRectdv (const GLdouble*, const GLdouble*);
WINGDIAPI void APIENTRY glRectf (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glRectfv (const GLfloat*, const GLfloat*);
WINGDIAPI void APIENTRY glRecti (GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glRectiv (const GLint*, const GLint*);
WINGDIAPI void APIENTRY glRects (GLshort, GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glRectsv (const GLshort*, const GLshort*);
WINGDIAPI GLint APIENTRY glRenderMode (GLenum);
WINGDIAPI void APIENTRY glRotated (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glRotatef (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glScaled (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glScalef (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glScissor (GLint, GLint, GLsizei, GLsizei);
WINGDIAPI void APIENTRY glSelectBuffer (GLsizei, GLuint*);
WINGDIAPI void APIENTRY glShadeModel (GLenum);
WINGDIAPI void APIENTRY glStencilFunc (GLenum, GLint, GLuint);
WINGDIAPI void APIENTRY glStencilMask (GLuint);
WINGDIAPI void APIENTRY glStencilOp (GLenum, GLenum, GLenum);
WINGDIAPI void APIENTRY glTexCoord1d (GLdouble);
WINGDIAPI void APIENTRY glTexCoord1dv (const GLdouble*);
WINGDIAPI void APIENTRY glTexCoord1f (GLfloat);
WINGDIAPI void APIENTRY glTexCoord1fv (const GLfloat*);
WINGDIAPI void APIENTRY glTexCoord1i (GLint);
WINGDIAPI void APIENTRY glTexCoord1iv (const GLint*);
WINGDIAPI void APIENTRY glTexCoord1s (GLshort);
WINGDIAPI void APIENTRY glTexCoord1sv (const GLshort*);
WINGDIAPI void APIENTRY glTexCoord2d (GLdouble, GLdouble);
WINGDIAPI void APIENTRY glTexCoord2dv (const GLdouble*);
WINGDIAPI void APIENTRY glTexCoord2f (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glTexCoord2fv (const GLfloat*);
WINGDIAPI void APIENTRY glTexCoord2i (GLint, GLint);
WINGDIAPI void APIENTRY glTexCoord2iv (const GLint*);
WINGDIAPI void APIENTRY glTexCoord2s (GLshort, GLshort);
WINGDIAPI void APIENTRY glTexCoord2sv (const GLshort*);
WINGDIAPI void APIENTRY glTexCoord3d (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glTexCoord3dv (const GLdouble*);
WINGDIAPI void APIENTRY glTexCoord3f (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glTexCoord3fv (const GLfloat*);
WINGDIAPI void APIENTRY glTexCoord3i (GLint, GLint, GLint);
WINGDIAPI void APIENTRY glTexCoord3iv (const GLint*);
WINGDIAPI void APIENTRY glTexCoord3s (GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glTexCoord3sv (const GLshort*);
WINGDIAPI void APIENTRY glTexCoord4d (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glTexCoord4dv (const GLdouble*);
WINGDIAPI void APIENTRY glTexCoord4f (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glTexCoord4fv (const GLfloat*);
WINGDIAPI void APIENTRY glTexCoord4i (GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glTexCoord4iv (const GLint*);
WINGDIAPI void APIENTRY glTexCoord4s (GLshort, GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glTexCoord4sv (const GLshort*);
WINGDIAPI void APIENTRY glTexCoordPointer (GLint, GLenum, GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glTexEnvf (GLenum, GLenum, GLfloat);
WINGDIAPI void APIENTRY glTexEnvfv (GLenum, GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glTexEnvi (GLenum, GLenum, GLint);
WINGDIAPI void APIENTRY glTexEnviv (GLenum, GLenum, const GLint*);
WINGDIAPI void APIENTRY glTexGend (GLenum, GLenum, GLdouble);
WINGDIAPI void APIENTRY glTexGendv (GLenum, GLenum, const GLdouble*);
WINGDIAPI void APIENTRY glTexGenf (GLenum, GLenum, GLfloat);
WINGDIAPI void APIENTRY glTexGenfv (GLenum, GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glTexGeni (GLenum, GLenum, GLint);
WINGDIAPI void APIENTRY glTexGeniv (GLenum, GLenum, const GLint*);
WINGDIAPI void APIENTRY glTexImage1D (GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glTexImage2D (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glTexParameterf (GLenum, GLenum, GLfloat);
WINGDIAPI void APIENTRY glTexParameterfv (GLenum, GLenum, const GLfloat*);
WINGDIAPI void APIENTRY glTexParameteri (GLenum, GLenum, GLint);
WINGDIAPI void APIENTRY glTexParameteriv (GLenum, GLenum, const GLint*);
WINGDIAPI void APIENTRY glTexSubImage1D (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glTexSubImage2D (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
WINGDIAPI void APIENTRY glTranslated (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glTranslatef (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glVertex2d (GLdouble, GLdouble);
WINGDIAPI void APIENTRY glVertex2dv (const GLdouble*);
WINGDIAPI void APIENTRY glVertex2f (GLfloat, GLfloat);
WINGDIAPI void APIENTRY glVertex2fv (const GLfloat*);
WINGDIAPI void APIENTRY glVertex2i (GLint, GLint);
WINGDIAPI void APIENTRY glVertex2iv (const GLint*);
WINGDIAPI void APIENTRY glVertex2s (GLshort, GLshort);
WINGDIAPI void APIENTRY glVertex2sv (const GLshort*);
WINGDIAPI void APIENTRY glVertex3d (GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glVertex3dv (const GLdouble*);
WINGDIAPI void APIENTRY glVertex3f (GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glVertex3fv (const GLfloat*);
WINGDIAPI void APIENTRY glVertex3i (GLint, GLint, GLint);
WINGDIAPI void APIENTRY glVertex3iv (const GLint*);
WINGDIAPI void APIENTRY glVertex3s (GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glVertex3sv (const GLshort*);
WINGDIAPI void APIENTRY glVertex4d (GLdouble, GLdouble, GLdouble, GLdouble);
WINGDIAPI void APIENTRY glVertex4dv (const GLdouble*);
WINGDIAPI void APIENTRY glVertex4f (GLfloat, GLfloat, GLfloat, GLfloat);
WINGDIAPI void APIENTRY glVertex4fv (const GLfloat*);
WINGDIAPI void APIENTRY glVertex4i (GLint, GLint, GLint, GLint);
WINGDIAPI void APIENTRY glVertex4iv (const GLint*);
WINGDIAPI void APIENTRY glVertex4s (GLshort, GLshort, GLshort, GLshort);
WINGDIAPI void APIENTRY glVertex4sv (const GLshort*);
WINGDIAPI void APIENTRY glVertexPointer (GLint, GLenum, GLsizei, const GLvoid*);
WINGDIAPI void APIENTRY glViewport (GLint, GLint, GLsizei, GLsizei);
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_1_2;
extern int QGLNumFunctions_GL_VERSION_1_2;
#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2
#define QGL_DEFINED_GL_VERSION_1_2
#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_RESCALE_NORMAL                 0x803A
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8
#define GL_SINGLE_COLOR                   0x81F9
#define GL_SEPARATE_SPECULAR_COLOR        0x81FA
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E

typedef void (APIENTRY* pfn_glBlendColor) (GLclampf, GLclampf, GLclampf, GLclampf);
extern pfn_glBlendColor QGLglBlendColor;
#define glBlendColor QGLglBlendColor

typedef void (APIENTRY* pfn_glBlendEquation) (GLenum);
extern pfn_glBlendEquation QGLglBlendEquation;
#define glBlendEquation QGLglBlendEquation

typedef void (APIENTRY* pfn_glDrawRangeElements) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*);
extern pfn_glDrawRangeElements QGLglDrawRangeElements;
#define glDrawRangeElements QGLglDrawRangeElements

typedef void (APIENTRY* pfn_glColorTable) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glColorTable QGLglColorTable;
#define glColorTable QGLglColorTable

typedef void (APIENTRY* pfn_glColorTableParameterfv) (GLenum, GLenum, const GLfloat*);
extern pfn_glColorTableParameterfv QGLglColorTableParameterfv;
#define glColorTableParameterfv QGLglColorTableParameterfv

typedef void (APIENTRY* pfn_glColorTableParameteriv) (GLenum, GLenum, const GLint*);
extern pfn_glColorTableParameteriv QGLglColorTableParameteriv;
#define glColorTableParameteriv QGLglColorTableParameteriv

typedef void (APIENTRY* pfn_glCopyColorTable) (GLenum, GLenum, GLint, GLint, GLsizei);
extern pfn_glCopyColorTable QGLglCopyColorTable;
#define glCopyColorTable QGLglCopyColorTable

typedef void (APIENTRY* pfn_glGetColorTable) (GLenum, GLenum, GLenum, GLvoid*);
extern pfn_glGetColorTable QGLglGetColorTable;
#define glGetColorTable QGLglGetColorTable

typedef void (APIENTRY* pfn_glGetColorTableParameterfv) (GLenum, GLenum, GLfloat*);
extern pfn_glGetColorTableParameterfv QGLglGetColorTableParameterfv;
#define glGetColorTableParameterfv QGLglGetColorTableParameterfv

typedef void (APIENTRY* pfn_glGetColorTableParameteriv) (GLenum, GLenum, GLint*);
extern pfn_glGetColorTableParameteriv QGLglGetColorTableParameteriv;
#define glGetColorTableParameteriv QGLglGetColorTableParameteriv

typedef void (APIENTRY* pfn_glColorSubTable) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glColorSubTable QGLglColorSubTable;
#define glColorSubTable QGLglColorSubTable

typedef void (APIENTRY* pfn_glCopyColorSubTable) (GLenum, GLsizei, GLint, GLint, GLsizei);
extern pfn_glCopyColorSubTable QGLglCopyColorSubTable;
#define glCopyColorSubTable QGLglCopyColorSubTable

typedef void (APIENTRY* pfn_glConvolutionFilter1D) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glConvolutionFilter1D QGLglConvolutionFilter1D;
#define glConvolutionFilter1D QGLglConvolutionFilter1D

typedef void (APIENTRY* pfn_glConvolutionFilter2D) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glConvolutionFilter2D QGLglConvolutionFilter2D;
#define glConvolutionFilter2D QGLglConvolutionFilter2D

typedef void (APIENTRY* pfn_glConvolutionParameterf) (GLenum, GLenum, GLfloat);
extern pfn_glConvolutionParameterf QGLglConvolutionParameterf;
#define glConvolutionParameterf QGLglConvolutionParameterf

typedef void (APIENTRY* pfn_glConvolutionParameterfv) (GLenum, GLenum, const GLfloat*);
extern pfn_glConvolutionParameterfv QGLglConvolutionParameterfv;
#define glConvolutionParameterfv QGLglConvolutionParameterfv

typedef void (APIENTRY* pfn_glConvolutionParameteri) (GLenum, GLenum, GLint);
extern pfn_glConvolutionParameteri QGLglConvolutionParameteri;
#define glConvolutionParameteri QGLglConvolutionParameteri

typedef void (APIENTRY* pfn_glConvolutionParameteriv) (GLenum, GLenum, const GLint*);
extern pfn_glConvolutionParameteriv QGLglConvolutionParameteriv;
#define glConvolutionParameteriv QGLglConvolutionParameteriv

typedef void (APIENTRY* pfn_glCopyConvolutionFilter1D) (GLenum, GLenum, GLint, GLint, GLsizei);
extern pfn_glCopyConvolutionFilter1D QGLglCopyConvolutionFilter1D;
#define glCopyConvolutionFilter1D QGLglCopyConvolutionFilter1D

typedef void (APIENTRY* pfn_glCopyConvolutionFilter2D) (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
extern pfn_glCopyConvolutionFilter2D QGLglCopyConvolutionFilter2D;
#define glCopyConvolutionFilter2D QGLglCopyConvolutionFilter2D

typedef void (APIENTRY* pfn_glGetConvolutionFilter) (GLenum, GLenum, GLenum, GLvoid*);
extern pfn_glGetConvolutionFilter QGLglGetConvolutionFilter;
#define glGetConvolutionFilter QGLglGetConvolutionFilter

typedef void (APIENTRY* pfn_glGetConvolutionParameterfv) (GLenum, GLenum, GLfloat*);
extern pfn_glGetConvolutionParameterfv QGLglGetConvolutionParameterfv;
#define glGetConvolutionParameterfv QGLglGetConvolutionParameterfv

typedef void (APIENTRY* pfn_glGetConvolutionParameteriv) (GLenum, GLenum, GLint*);
extern pfn_glGetConvolutionParameteriv QGLglGetConvolutionParameteriv;
#define glGetConvolutionParameteriv QGLglGetConvolutionParameteriv

typedef void (APIENTRY* pfn_glGetSeparableFilter) (GLenum, GLenum, GLenum, GLvoid*, GLvoid*, GLvoid*);
extern pfn_glGetSeparableFilter QGLglGetSeparableFilter;
#define glGetSeparableFilter QGLglGetSeparableFilter

typedef void (APIENTRY* pfn_glSeparableFilter2D) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*, const GLvoid*);
extern pfn_glSeparableFilter2D QGLglSeparableFilter2D;
#define glSeparableFilter2D QGLglSeparableFilter2D

typedef void (APIENTRY* pfn_glGetHistogram) (GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern pfn_glGetHistogram QGLglGetHistogram;
#define glGetHistogram QGLglGetHistogram

typedef void (APIENTRY* pfn_glGetHistogramParameterfv) (GLenum, GLenum, GLfloat*);
extern pfn_glGetHistogramParameterfv QGLglGetHistogramParameterfv;
#define glGetHistogramParameterfv QGLglGetHistogramParameterfv

typedef void (APIENTRY* pfn_glGetHistogramParameteriv) (GLenum, GLenum, GLint*);
extern pfn_glGetHistogramParameteriv QGLglGetHistogramParameteriv;
#define glGetHistogramParameteriv QGLglGetHistogramParameteriv

typedef void (APIENTRY* pfn_glGetMinmax) (GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern pfn_glGetMinmax QGLglGetMinmax;
#define glGetMinmax QGLglGetMinmax

typedef void (APIENTRY* pfn_glGetMinmaxParameterfv) (GLenum, GLenum, GLfloat*);
extern pfn_glGetMinmaxParameterfv QGLglGetMinmaxParameterfv;
#define glGetMinmaxParameterfv QGLglGetMinmaxParameterfv

typedef void (APIENTRY* pfn_glGetMinmaxParameteriv) (GLenum, GLenum, GLint*);
extern pfn_glGetMinmaxParameteriv QGLglGetMinmaxParameteriv;
#define glGetMinmaxParameteriv QGLglGetMinmaxParameteriv

typedef void (APIENTRY* pfn_glHistogram) (GLenum, GLsizei, GLenum, GLboolean);
extern pfn_glHistogram QGLglHistogram;
#define glHistogram QGLglHistogram

typedef void (APIENTRY* pfn_glMinmax) (GLenum, GLenum, GLboolean);
extern pfn_glMinmax QGLglMinmax;
#define glMinmax QGLglMinmax

typedef void (APIENTRY* pfn_glResetHistogram) (GLenum);
extern pfn_glResetHistogram QGLglResetHistogram;
#define glResetHistogram QGLglResetHistogram

typedef void (APIENTRY* pfn_glResetMinmax) (GLenum);
extern pfn_glResetMinmax QGLglResetMinmax;
#define glResetMinmax QGLglResetMinmax

typedef void (APIENTRY* pfn_glTexImage3D) (GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
extern pfn_glTexImage3D QGLglTexImage3D;
#define glTexImage3D QGLglTexImage3D

typedef void (APIENTRY* pfn_glTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glTexSubImage3D QGLglTexSubImage3D;
#define glTexSubImage3D QGLglTexSubImage3D

typedef void (APIENTRY* pfn_glCopyTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
extern pfn_glCopyTexSubImage3D QGLglCopyTexSubImage3D;
#define glCopyTexSubImage3D QGLglCopyTexSubImage3D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_1_3;
extern int QGLNumFunctions_GL_VERSION_1_3;
#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3
#define QGL_DEFINED_GL_VERSION_1_3
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX     0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX    0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX       0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX         0x84E6
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_MULTISAMPLE_BIT                0x20000000
#define GL_NORMAL_MAP                     0x8511
#define GL_REFLECTION_MAP                 0x8512
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_COMPRESSED_ALPHA               0x84E9
#define GL_COMPRESSED_LUMINANCE           0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA     0x84EB
#define GL_COMPRESSED_INTENSITY           0x84EC
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_SOURCE0_RGB                    0x8580
#define GL_SOURCE1_RGB                    0x8581
#define GL_SOURCE2_RGB                    0x8582
#define GL_SOURCE0_ALPHA                  0x8588
#define GL_SOURCE1_ALPHA                  0x8589
#define GL_SOURCE2_ALPHA                  0x858A
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_SUBTRACT                       0x84E7
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF

typedef void (APIENTRY* pfn_glActiveTexture) (GLenum);
extern pfn_glActiveTexture QGLglActiveTexture;
#define glActiveTexture QGLglActiveTexture

typedef void (APIENTRY* pfn_glClientActiveTexture) (GLenum);
extern pfn_glClientActiveTexture QGLglClientActiveTexture;
#define glClientActiveTexture QGLglClientActiveTexture

typedef void (APIENTRY* pfn_glMultiTexCoord1d) (GLenum, GLdouble);
extern pfn_glMultiTexCoord1d QGLglMultiTexCoord1d;
#define glMultiTexCoord1d QGLglMultiTexCoord1d

typedef void (APIENTRY* pfn_glMultiTexCoord1dv) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord1dv QGLglMultiTexCoord1dv;
#define glMultiTexCoord1dv QGLglMultiTexCoord1dv

typedef void (APIENTRY* pfn_glMultiTexCoord1f) (GLenum, GLfloat);
extern pfn_glMultiTexCoord1f QGLglMultiTexCoord1f;
#define glMultiTexCoord1f QGLglMultiTexCoord1f

typedef void (APIENTRY* pfn_glMultiTexCoord1fv) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord1fv QGLglMultiTexCoord1fv;
#define glMultiTexCoord1fv QGLglMultiTexCoord1fv

typedef void (APIENTRY* pfn_glMultiTexCoord1i) (GLenum, GLint);
extern pfn_glMultiTexCoord1i QGLglMultiTexCoord1i;
#define glMultiTexCoord1i QGLglMultiTexCoord1i

typedef void (APIENTRY* pfn_glMultiTexCoord1iv) (GLenum, const GLint*);
extern pfn_glMultiTexCoord1iv QGLglMultiTexCoord1iv;
#define glMultiTexCoord1iv QGLglMultiTexCoord1iv

typedef void (APIENTRY* pfn_glMultiTexCoord1s) (GLenum, GLshort);
extern pfn_glMultiTexCoord1s QGLglMultiTexCoord1s;
#define glMultiTexCoord1s QGLglMultiTexCoord1s

typedef void (APIENTRY* pfn_glMultiTexCoord1sv) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord1sv QGLglMultiTexCoord1sv;
#define glMultiTexCoord1sv QGLglMultiTexCoord1sv

typedef void (APIENTRY* pfn_glMultiTexCoord2d) (GLenum, GLdouble, GLdouble);
extern pfn_glMultiTexCoord2d QGLglMultiTexCoord2d;
#define glMultiTexCoord2d QGLglMultiTexCoord2d

typedef void (APIENTRY* pfn_glMultiTexCoord2dv) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord2dv QGLglMultiTexCoord2dv;
#define glMultiTexCoord2dv QGLglMultiTexCoord2dv

typedef void (APIENTRY* pfn_glMultiTexCoord2f) (GLenum, GLfloat, GLfloat);
extern pfn_glMultiTexCoord2f QGLglMultiTexCoord2f;
#define glMultiTexCoord2f QGLglMultiTexCoord2f

typedef void (APIENTRY* pfn_glMultiTexCoord2fv) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord2fv QGLglMultiTexCoord2fv;
#define glMultiTexCoord2fv QGLglMultiTexCoord2fv

typedef void (APIENTRY* pfn_glMultiTexCoord2i) (GLenum, GLint, GLint);
extern pfn_glMultiTexCoord2i QGLglMultiTexCoord2i;
#define glMultiTexCoord2i QGLglMultiTexCoord2i

typedef void (APIENTRY* pfn_glMultiTexCoord2iv) (GLenum, const GLint*);
extern pfn_glMultiTexCoord2iv QGLglMultiTexCoord2iv;
#define glMultiTexCoord2iv QGLglMultiTexCoord2iv

typedef void (APIENTRY* pfn_glMultiTexCoord2s) (GLenum, GLshort, GLshort);
extern pfn_glMultiTexCoord2s QGLglMultiTexCoord2s;
#define glMultiTexCoord2s QGLglMultiTexCoord2s

typedef void (APIENTRY* pfn_glMultiTexCoord2sv) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord2sv QGLglMultiTexCoord2sv;
#define glMultiTexCoord2sv QGLglMultiTexCoord2sv

typedef void (APIENTRY* pfn_glMultiTexCoord3d) (GLenum, GLdouble, GLdouble, GLdouble);
extern pfn_glMultiTexCoord3d QGLglMultiTexCoord3d;
#define glMultiTexCoord3d QGLglMultiTexCoord3d

typedef void (APIENTRY* pfn_glMultiTexCoord3dv) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord3dv QGLglMultiTexCoord3dv;
#define glMultiTexCoord3dv QGLglMultiTexCoord3dv

typedef void (APIENTRY* pfn_glMultiTexCoord3f) (GLenum, GLfloat, GLfloat, GLfloat);
extern pfn_glMultiTexCoord3f QGLglMultiTexCoord3f;
#define glMultiTexCoord3f QGLglMultiTexCoord3f

typedef void (APIENTRY* pfn_glMultiTexCoord3fv) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord3fv QGLglMultiTexCoord3fv;
#define glMultiTexCoord3fv QGLglMultiTexCoord3fv

typedef void (APIENTRY* pfn_glMultiTexCoord3i) (GLenum, GLint, GLint, GLint);
extern pfn_glMultiTexCoord3i QGLglMultiTexCoord3i;
#define glMultiTexCoord3i QGLglMultiTexCoord3i

typedef void (APIENTRY* pfn_glMultiTexCoord3iv) (GLenum, const GLint*);
extern pfn_glMultiTexCoord3iv QGLglMultiTexCoord3iv;
#define glMultiTexCoord3iv QGLglMultiTexCoord3iv

typedef void (APIENTRY* pfn_glMultiTexCoord3s) (GLenum, GLshort, GLshort, GLshort);
extern pfn_glMultiTexCoord3s QGLglMultiTexCoord3s;
#define glMultiTexCoord3s QGLglMultiTexCoord3s

typedef void (APIENTRY* pfn_glMultiTexCoord3sv) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord3sv QGLglMultiTexCoord3sv;
#define glMultiTexCoord3sv QGLglMultiTexCoord3sv

typedef void (APIENTRY* pfn_glMultiTexCoord4d) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glMultiTexCoord4d QGLglMultiTexCoord4d;
#define glMultiTexCoord4d QGLglMultiTexCoord4d

typedef void (APIENTRY* pfn_glMultiTexCoord4dv) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord4dv QGLglMultiTexCoord4dv;
#define glMultiTexCoord4dv QGLglMultiTexCoord4dv

typedef void (APIENTRY* pfn_glMultiTexCoord4f) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glMultiTexCoord4f QGLglMultiTexCoord4f;
#define glMultiTexCoord4f QGLglMultiTexCoord4f

typedef void (APIENTRY* pfn_glMultiTexCoord4fv) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord4fv QGLglMultiTexCoord4fv;
#define glMultiTexCoord4fv QGLglMultiTexCoord4fv

typedef void (APIENTRY* pfn_glMultiTexCoord4i) (GLenum, GLint, GLint, GLint, GLint);
extern pfn_glMultiTexCoord4i QGLglMultiTexCoord4i;
#define glMultiTexCoord4i QGLglMultiTexCoord4i

typedef void (APIENTRY* pfn_glMultiTexCoord4iv) (GLenum, const GLint*);
extern pfn_glMultiTexCoord4iv QGLglMultiTexCoord4iv;
#define glMultiTexCoord4iv QGLglMultiTexCoord4iv

typedef void (APIENTRY* pfn_glMultiTexCoord4s) (GLenum, GLshort, GLshort, GLshort, GLshort);
extern pfn_glMultiTexCoord4s QGLglMultiTexCoord4s;
#define glMultiTexCoord4s QGLglMultiTexCoord4s

typedef void (APIENTRY* pfn_glMultiTexCoord4sv) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord4sv QGLglMultiTexCoord4sv;
#define glMultiTexCoord4sv QGLglMultiTexCoord4sv

typedef void (APIENTRY* pfn_glLoadTransposeMatrixf) (const GLfloat*);
extern pfn_glLoadTransposeMatrixf QGLglLoadTransposeMatrixf;
#define glLoadTransposeMatrixf QGLglLoadTransposeMatrixf

typedef void (APIENTRY* pfn_glLoadTransposeMatrixd) (const GLdouble*);
extern pfn_glLoadTransposeMatrixd QGLglLoadTransposeMatrixd;
#define glLoadTransposeMatrixd QGLglLoadTransposeMatrixd

typedef void (APIENTRY* pfn_glMultTransposeMatrixf) (const GLfloat*);
extern pfn_glMultTransposeMatrixf QGLglMultTransposeMatrixf;
#define glMultTransposeMatrixf QGLglMultTransposeMatrixf

typedef void (APIENTRY* pfn_glMultTransposeMatrixd) (const GLdouble*);
extern pfn_glMultTransposeMatrixd QGLglMultTransposeMatrixd;
#define glMultTransposeMatrixd QGLglMultTransposeMatrixd

typedef void (APIENTRY* pfn_glSampleCoverage) (GLclampf, GLboolean);
extern pfn_glSampleCoverage QGLglSampleCoverage;
#define glSampleCoverage QGLglSampleCoverage

typedef void (APIENTRY* pfn_glCompressedTexImage3D) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage3D QGLglCompressedTexImage3D;
#define glCompressedTexImage3D QGLglCompressedTexImage3D

typedef void (APIENTRY* pfn_glCompressedTexImage2D) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage2D QGLglCompressedTexImage2D;
#define glCompressedTexImage2D QGLglCompressedTexImage2D

typedef void (APIENTRY* pfn_glCompressedTexImage1D) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage1D QGLglCompressedTexImage1D;
#define glCompressedTexImage1D QGLglCompressedTexImage1D

typedef void (APIENTRY* pfn_glCompressedTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage3D QGLglCompressedTexSubImage3D;
#define glCompressedTexSubImage3D QGLglCompressedTexSubImage3D

typedef void (APIENTRY* pfn_glCompressedTexSubImage2D) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage2D QGLglCompressedTexSubImage2D;
#define glCompressedTexSubImage2D QGLglCompressedTexSubImage2D

typedef void (APIENTRY* pfn_glCompressedTexSubImage1D) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage1D QGLglCompressedTexSubImage1D;
#define glCompressedTexSubImage1D QGLglCompressedTexSubImage1D

typedef void (APIENTRY* pfn_glGetCompressedTexImage) (GLenum, GLint, GLvoid*);
extern pfn_glGetCompressedTexImage QGLglGetCompressedTexImage;
#define glGetCompressedTexImage QGLglGetCompressedTexImage
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_1_4;
extern int QGLNumFunctions_GL_VERSION_1_4;
#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4
#define QGL_DEFINED_GL_VERSION_1_4
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_POINT_SIZE_MIN                 0x8126
#define GL_POINT_SIZE_MAX                 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_POINT_DISTANCE_ATTENUATION     0x8129
#define GL_GENERATE_MIPMAP                0x8191
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
#define GL_FOG_COORDINATE_SOURCE          0x8450
#define GL_FOG_COORDINATE                 0x8451
#define GL_FRAGMENT_DEPTH                 0x8452
#define GL_CURRENT_FOG_COORDINATE         0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE      0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE    0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER   0x8456
#define GL_FOG_COORDINATE_ARRAY           0x8457
#define GL_COLOR_SUM                      0x8458
#define GL_CURRENT_SECONDARY_COLOR        0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE     0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE     0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE   0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER  0x845D
#define GL_SECONDARY_COLOR_ARRAY          0x845E
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_FILTER_CONTROL         0x8500
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_DEPTH_TEXTURE_MODE             0x884B
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_COMPARE_R_TO_TEXTURE           0x884E

typedef void (APIENTRY* pfn_glBlendFuncSeparate) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glBlendFuncSeparate QGLglBlendFuncSeparate;
#define glBlendFuncSeparate QGLglBlendFuncSeparate

typedef void (APIENTRY* pfn_glFogCoordf) (GLfloat);
extern pfn_glFogCoordf QGLglFogCoordf;
#define glFogCoordf QGLglFogCoordf

typedef void (APIENTRY* pfn_glFogCoordfv) (const GLfloat*);
extern pfn_glFogCoordfv QGLglFogCoordfv;
#define glFogCoordfv QGLglFogCoordfv

typedef void (APIENTRY* pfn_glFogCoordd) (GLdouble);
extern pfn_glFogCoordd QGLglFogCoordd;
#define glFogCoordd QGLglFogCoordd

typedef void (APIENTRY* pfn_glFogCoorddv) (const GLdouble*);
extern pfn_glFogCoorddv QGLglFogCoorddv;
#define glFogCoorddv QGLglFogCoorddv

typedef void (APIENTRY* pfn_glFogCoordPointer) (GLenum, GLsizei, const GLvoid*);
extern pfn_glFogCoordPointer QGLglFogCoordPointer;
#define glFogCoordPointer QGLglFogCoordPointer

typedef void (APIENTRY* pfn_glMultiDrawArrays) (GLenum, GLint*, GLsizei*, GLsizei);
extern pfn_glMultiDrawArrays QGLglMultiDrawArrays;
#define glMultiDrawArrays QGLglMultiDrawArrays

typedef void (APIENTRY* pfn_glMultiDrawElements) (GLenum, const GLsizei*, GLenum, const GLvoid**, GLsizei);
extern pfn_glMultiDrawElements QGLglMultiDrawElements;
#define glMultiDrawElements QGLglMultiDrawElements

typedef void (APIENTRY* pfn_glPointParameterf) (GLenum, GLfloat);
extern pfn_glPointParameterf QGLglPointParameterf;
#define glPointParameterf QGLglPointParameterf

typedef void (APIENTRY* pfn_glPointParameterfv) (GLenum, const GLfloat*);
extern pfn_glPointParameterfv QGLglPointParameterfv;
#define glPointParameterfv QGLglPointParameterfv

typedef void (APIENTRY* pfn_glPointParameteri) (GLenum, GLint);
extern pfn_glPointParameteri QGLglPointParameteri;
#define glPointParameteri QGLglPointParameteri

typedef void (APIENTRY* pfn_glPointParameteriv) (GLenum, const GLint*);
extern pfn_glPointParameteriv QGLglPointParameteriv;
#define glPointParameteriv QGLglPointParameteriv

typedef void (APIENTRY* pfn_glSecondaryColor3b) (GLbyte, GLbyte, GLbyte);
extern pfn_glSecondaryColor3b QGLglSecondaryColor3b;
#define glSecondaryColor3b QGLglSecondaryColor3b

typedef void (APIENTRY* pfn_glSecondaryColor3bv) (const GLbyte*);
extern pfn_glSecondaryColor3bv QGLglSecondaryColor3bv;
#define glSecondaryColor3bv QGLglSecondaryColor3bv

typedef void (APIENTRY* pfn_glSecondaryColor3d) (GLdouble, GLdouble, GLdouble);
extern pfn_glSecondaryColor3d QGLglSecondaryColor3d;
#define glSecondaryColor3d QGLglSecondaryColor3d

typedef void (APIENTRY* pfn_glSecondaryColor3dv) (const GLdouble*);
extern pfn_glSecondaryColor3dv QGLglSecondaryColor3dv;
#define glSecondaryColor3dv QGLglSecondaryColor3dv

typedef void (APIENTRY* pfn_glSecondaryColor3f) (GLfloat, GLfloat, GLfloat);
extern pfn_glSecondaryColor3f QGLglSecondaryColor3f;
#define glSecondaryColor3f QGLglSecondaryColor3f

typedef void (APIENTRY* pfn_glSecondaryColor3fv) (const GLfloat*);
extern pfn_glSecondaryColor3fv QGLglSecondaryColor3fv;
#define glSecondaryColor3fv QGLglSecondaryColor3fv

typedef void (APIENTRY* pfn_glSecondaryColor3i) (GLint, GLint, GLint);
extern pfn_glSecondaryColor3i QGLglSecondaryColor3i;
#define glSecondaryColor3i QGLglSecondaryColor3i

typedef void (APIENTRY* pfn_glSecondaryColor3iv) (const GLint*);
extern pfn_glSecondaryColor3iv QGLglSecondaryColor3iv;
#define glSecondaryColor3iv QGLglSecondaryColor3iv

typedef void (APIENTRY* pfn_glSecondaryColor3s) (GLshort, GLshort, GLshort);
extern pfn_glSecondaryColor3s QGLglSecondaryColor3s;
#define glSecondaryColor3s QGLglSecondaryColor3s

typedef void (APIENTRY* pfn_glSecondaryColor3sv) (const GLshort*);
extern pfn_glSecondaryColor3sv QGLglSecondaryColor3sv;
#define glSecondaryColor3sv QGLglSecondaryColor3sv

typedef void (APIENTRY* pfn_glSecondaryColor3ub) (GLubyte, GLubyte, GLubyte);
extern pfn_glSecondaryColor3ub QGLglSecondaryColor3ub;
#define glSecondaryColor3ub QGLglSecondaryColor3ub

typedef void (APIENTRY* pfn_glSecondaryColor3ubv) (const GLubyte*);
extern pfn_glSecondaryColor3ubv QGLglSecondaryColor3ubv;
#define glSecondaryColor3ubv QGLglSecondaryColor3ubv

typedef void (APIENTRY* pfn_glSecondaryColor3ui) (GLuint, GLuint, GLuint);
extern pfn_glSecondaryColor3ui QGLglSecondaryColor3ui;
#define glSecondaryColor3ui QGLglSecondaryColor3ui

typedef void (APIENTRY* pfn_glSecondaryColor3uiv) (const GLuint*);
extern pfn_glSecondaryColor3uiv QGLglSecondaryColor3uiv;
#define glSecondaryColor3uiv QGLglSecondaryColor3uiv

typedef void (APIENTRY* pfn_glSecondaryColor3us) (GLushort, GLushort, GLushort);
extern pfn_glSecondaryColor3us QGLglSecondaryColor3us;
#define glSecondaryColor3us QGLglSecondaryColor3us

typedef void (APIENTRY* pfn_glSecondaryColor3usv) (const GLushort*);
extern pfn_glSecondaryColor3usv QGLglSecondaryColor3usv;
#define glSecondaryColor3usv QGLglSecondaryColor3usv

typedef void (APIENTRY* pfn_glSecondaryColorPointer) (GLint, GLenum, GLsizei, const GLvoid*);
extern pfn_glSecondaryColorPointer QGLglSecondaryColorPointer;
#define glSecondaryColorPointer QGLglSecondaryColorPointer

typedef void (APIENTRY* pfn_glWindowPos2d) (GLdouble, GLdouble);
extern pfn_glWindowPos2d QGLglWindowPos2d;
#define glWindowPos2d QGLglWindowPos2d

typedef void (APIENTRY* pfn_glWindowPos2dv) (const GLdouble*);
extern pfn_glWindowPos2dv QGLglWindowPos2dv;
#define glWindowPos2dv QGLglWindowPos2dv

typedef void (APIENTRY* pfn_glWindowPos2f) (GLfloat, GLfloat);
extern pfn_glWindowPos2f QGLglWindowPos2f;
#define glWindowPos2f QGLglWindowPos2f

typedef void (APIENTRY* pfn_glWindowPos2fv) (const GLfloat*);
extern pfn_glWindowPos2fv QGLglWindowPos2fv;
#define glWindowPos2fv QGLglWindowPos2fv

typedef void (APIENTRY* pfn_glWindowPos2i) (GLint, GLint);
extern pfn_glWindowPos2i QGLglWindowPos2i;
#define glWindowPos2i QGLglWindowPos2i

typedef void (APIENTRY* pfn_glWindowPos2iv) (const GLint*);
extern pfn_glWindowPos2iv QGLglWindowPos2iv;
#define glWindowPos2iv QGLglWindowPos2iv

typedef void (APIENTRY* pfn_glWindowPos2s) (GLshort, GLshort);
extern pfn_glWindowPos2s QGLglWindowPos2s;
#define glWindowPos2s QGLglWindowPos2s

typedef void (APIENTRY* pfn_glWindowPos2sv) (const GLshort*);
extern pfn_glWindowPos2sv QGLglWindowPos2sv;
#define glWindowPos2sv QGLglWindowPos2sv

typedef void (APIENTRY* pfn_glWindowPos3d) (GLdouble, GLdouble, GLdouble);
extern pfn_glWindowPos3d QGLglWindowPos3d;
#define glWindowPos3d QGLglWindowPos3d

typedef void (APIENTRY* pfn_glWindowPos3dv) (const GLdouble*);
extern pfn_glWindowPos3dv QGLglWindowPos3dv;
#define glWindowPos3dv QGLglWindowPos3dv

typedef void (APIENTRY* pfn_glWindowPos3f) (GLfloat, GLfloat, GLfloat);
extern pfn_glWindowPos3f QGLglWindowPos3f;
#define glWindowPos3f QGLglWindowPos3f

typedef void (APIENTRY* pfn_glWindowPos3fv) (const GLfloat*);
extern pfn_glWindowPos3fv QGLglWindowPos3fv;
#define glWindowPos3fv QGLglWindowPos3fv

typedef void (APIENTRY* pfn_glWindowPos3i) (GLint, GLint, GLint);
extern pfn_glWindowPos3i QGLglWindowPos3i;
#define glWindowPos3i QGLglWindowPos3i

typedef void (APIENTRY* pfn_glWindowPos3iv) (const GLint*);
extern pfn_glWindowPos3iv QGLglWindowPos3iv;
#define glWindowPos3iv QGLglWindowPos3iv

typedef void (APIENTRY* pfn_glWindowPos3s) (GLshort, GLshort, GLshort);
extern pfn_glWindowPos3s QGLglWindowPos3s;
#define glWindowPos3s QGLglWindowPos3s

typedef void (APIENTRY* pfn_glWindowPos3sv) (const GLshort*);
extern pfn_glWindowPos3sv QGLglWindowPos3sv;
#define glWindowPos3sv QGLglWindowPos3sv
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_1_5;
extern int QGLNumFunctions_GL_VERSION_1_5;
#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5
#define QGL_DEFINED_GL_VERSION_1_5
#define GL_BUFFER_SIZE                          0x8764
#define GL_BUFFER_USAGE                         0x8765
#define GL_QUERY_COUNTER_BITS                   0x8864
#define GL_CURRENT_QUERY                        0x8865
#define GL_QUERY_RESULT                         0x8866
#define GL_QUERY_RESULT_AVAILABLE               0x8867
#define GL_ARRAY_BUFFER                         0x8892
#define GL_ELEMENT_ARRAY_BUFFER                 0x8893
#define GL_ARRAY_BUFFER_BINDING                 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING         0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING          0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING          0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING           0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING           0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING   0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING       0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING  0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING          0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING   0x889F
#define GL_READ_ONLY                            0x88B8
#define GL_WRITE_ONLY                           0x88B9
#define GL_READ_WRITE                           0x88BA
#define GL_BUFFER_ACCESS                        0x88BB
#define GL_BUFFER_MAPPED                        0x88BC
#define GL_BUFFER_MAP_POINTER                   0x88BD
#define GL_STREAM_DRAW                          0x88E0
#define GL_STREAM_READ                          0x88E1
#define GL_STREAM_COPY                          0x88E2
#define GL_STATIC_DRAW                          0x88E4
#define GL_STATIC_READ                          0x88E5
#define GL_STATIC_COPY                          0x88E6
#define GL_DYNAMIC_DRAW                         0x88E8
#define GL_DYNAMIC_READ                         0x88E9
#define GL_DYNAMIC_COPY                         0x88EA
#define GL_SAMPLES_PASSED                       0x8914
#define GL_FOG_COORD_SRC                        GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD                            GL_FOG_COORDINATE
#define GL_CURRENT_FOG_COORD                    GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_TYPE                 GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORD_ARRAY_STRIDE               GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_POINTER              GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY                      GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING       GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC0_RGB                             GL_SOURCE0_RGB
#define GL_SRC1_RGB                             GL_SOURCE1_RGB
#define GL_SRC2_RGB                             GL_SOURCE2_RGB
#define GL_SRC0_ALPHA                           GL_SOURCE0_ALPHA
#define GL_SRC1_ALPHA                           GL_SOURCE1_ALPHA
#define GL_SRC2_ALPHA                           GL_SOURCE2_ALPHA
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef void (APIENTRY* pfn_glGenQueries) (GLsizei, GLuint*);
extern pfn_glGenQueries QGLglGenQueries;
#define glGenQueries QGLglGenQueries

typedef void (APIENTRY* pfn_glDeleteQueries) (GLsizei, const GLuint*);
extern pfn_glDeleteQueries QGLglDeleteQueries;
#define glDeleteQueries QGLglDeleteQueries

typedef GLboolean (APIENTRY* pfn_glIsQuery) (GLuint);
extern pfn_glIsQuery QGLglIsQuery;
#define glIsQuery QGLglIsQuery

typedef void (APIENTRY* pfn_glBeginQuery) (GLenum, GLuint);
extern pfn_glBeginQuery QGLglBeginQuery;
#define glBeginQuery QGLglBeginQuery

typedef void (APIENTRY* pfn_glEndQuery) (GLenum);
extern pfn_glEndQuery QGLglEndQuery;
#define glEndQuery QGLglEndQuery

typedef void (APIENTRY* pfn_glGetQueryiv) (GLenum, GLenum, GLint*);
extern pfn_glGetQueryiv QGLglGetQueryiv;
#define glGetQueryiv QGLglGetQueryiv

typedef void (APIENTRY* pfn_glGetQueryObjectiv) (GLuint, GLenum, GLint*);
extern pfn_glGetQueryObjectiv QGLglGetQueryObjectiv;
#define glGetQueryObjectiv QGLglGetQueryObjectiv

typedef void (APIENTRY* pfn_glGetQueryObjectuiv) (GLuint, GLenum, GLuint*);
extern pfn_glGetQueryObjectuiv QGLglGetQueryObjectuiv;
#define glGetQueryObjectuiv QGLglGetQueryObjectuiv

typedef void (APIENTRY* pfn_glBindBuffer) (GLenum, GLuint);
extern pfn_glBindBuffer QGLglBindBuffer;
#define glBindBuffer QGLglBindBuffer

typedef void (APIENTRY* pfn_glDeleteBuffers) (GLsizei, const GLuint*);
extern pfn_glDeleteBuffers QGLglDeleteBuffers;
#define glDeleteBuffers QGLglDeleteBuffers

typedef void (APIENTRY* pfn_glGenBuffers) (GLsizei, GLuint*);
extern pfn_glGenBuffers QGLglGenBuffers;
#define glGenBuffers QGLglGenBuffers

typedef GLboolean (APIENTRY* pfn_glIsBuffer) (GLuint);
extern pfn_glIsBuffer QGLglIsBuffer;
#define glIsBuffer QGLglIsBuffer

typedef void (APIENTRY* pfn_glBufferData) (GLenum, GLsizeiptr, const GLvoid*, GLenum);
extern pfn_glBufferData QGLglBufferData;
#define glBufferData QGLglBufferData

typedef void (APIENTRY* pfn_glBufferSubData) (GLenum, GLintptr, GLsizeiptr, const GLvoid*);
extern pfn_glBufferSubData QGLglBufferSubData;
#define glBufferSubData QGLglBufferSubData

typedef void (APIENTRY* pfn_glGetBufferSubData) (GLenum, GLintptr, GLsizeiptr, GLvoid*);
extern pfn_glGetBufferSubData QGLglGetBufferSubData;
#define glGetBufferSubData QGLglGetBufferSubData

typedef GLvoid* (APIENTRY* pfn_glMapBuffer) (GLenum, GLenum);
extern pfn_glMapBuffer QGLglMapBuffer;
#define glMapBuffer QGLglMapBuffer

typedef GLboolean (APIENTRY* pfn_glUnmapBuffer) (GLenum);
extern pfn_glUnmapBuffer QGLglUnmapBuffer;
#define glUnmapBuffer QGLglUnmapBuffer

typedef void (APIENTRY* pfn_glGetBufferParameteriv) (GLenum, GLenum, GLint*);
extern pfn_glGetBufferParameteriv QGLglGetBufferParameteriv;
#define glGetBufferParameteriv QGLglGetBufferParameteriv

typedef void (APIENTRY* pfn_glGetBufferPointerv) (GLenum, GLenum, GLvoid**);
extern pfn_glGetBufferPointerv QGLglGetBufferPointerv;
#define glGetBufferPointerv QGLglGetBufferPointerv
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_VERSION_2_0;
extern int QGLNumFunctions_GL_VERSION_2_0;
#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0
#define QGL_DEFINED_GL_VERSION_2_0
#define GL_BLEND_EQUATION_RGB               GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED      0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE         0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE       0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE         0x8625
#define GL_CURRENT_VERTEX_ATTRIB            0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE        0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE          0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER      0x8645
#define GL_STENCIL_BACK_FUNC                0x8800
#define GL_STENCIL_BACK_FAIL                0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL     0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS     0x8803
#define GL_MAX_DRAW_BUFFERS                 0x8824
#define GL_DRAW_BUFFER0                     0x8825
#define GL_DRAW_BUFFER1                     0x8826
#define GL_DRAW_BUFFER2                     0x8827
#define GL_DRAW_BUFFER3                     0x8828
#define GL_DRAW_BUFFER4                     0x8829
#define GL_DRAW_BUFFER5                     0x882A
#define GL_DRAW_BUFFER6                     0x882B
#define GL_DRAW_BUFFER7                     0x882C
#define GL_DRAW_BUFFER8                     0x882D
#define GL_DRAW_BUFFER9                     0x882E
#define GL_DRAW_BUFFER10                    0x882F
#define GL_DRAW_BUFFER11                    0x8830
#define GL_DRAW_BUFFER12                    0x8831
#define GL_DRAW_BUFFER13                    0x8832
#define GL_DRAW_BUFFER14                    0x8833
#define GL_DRAW_BUFFER15                    0x8834
#define GL_BLEND_EQUATION_ALPHA             0x883D
#define GL_POINT_SPRITE                     0x8861
#define GL_COORD_REPLACE                    0x8862
#define GL_MAX_VERTEX_ATTRIBS               0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED   0x886A
#define GL_MAX_TEXTURE_COORDS               0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS          0x8872
#define GL_FRAGMENT_SHADER                  0x8B30
#define GL_VERTEX_SHADER                    0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS  0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS    0x8B4A
#define GL_MAX_VARYING_FLOATS               0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS   0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                      0x8B4F
#define GL_FLOAT_VEC2                       0x8B50
#define GL_FLOAT_VEC3                       0x8B51
#define GL_FLOAT_VEC4                       0x8B52
#define GL_INT_VEC2                         0x8B53
#define GL_INT_VEC3                         0x8B54
#define GL_INT_VEC4                         0x8B55
#define GL_BOOL                             0x8B56
#define GL_BOOL_VEC2                        0x8B57
#define GL_BOOL_VEC3                        0x8B58
#define GL_BOOL_VEC4                        0x8B59
#define GL_FLOAT_MAT2                       0x8B5A
#define GL_FLOAT_MAT3                       0x8B5B
#define GL_FLOAT_MAT4                       0x8B5C
#define GL_SAMPLER_1D                       0x8B5D
#define GL_SAMPLER_2D                       0x8B5E
#define GL_SAMPLER_3D                       0x8B5F
#define GL_SAMPLER_CUBE                     0x8B60
#define GL_SAMPLER_1D_SHADOW                0x8B61
#define GL_SAMPLER_2D_SHADOW                0x8B62
#define GL_DELETE_STATUS                    0x8B80
#define GL_COMPILE_STATUS                   0x8B81
#define GL_LINK_STATUS                      0x8B82
#define GL_VALIDATE_STATUS                  0x8B83
#define GL_INFO_LOG_LENGTH                  0x8B84
#define GL_ATTACHED_SHADERS                 0x8B85
#define GL_ACTIVE_UNIFORMS                  0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH        0x8B87
#define GL_SHADER_SOURCE_LENGTH             0x8B88
#define GL_ACTIVE_ATTRIBUTES                0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH      0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT  0x8B8B
#define GL_SHADING_LANGUAGE_VERSION         0x8B8C
#define GL_CURRENT_PROGRAM                  0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN        0x8CA0
#define GL_LOWER_LEFT                       0x8CA1
#define GL_UPPER_LEFT                       0x8CA2
#define GL_STENCIL_BACK_REF                 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK          0x8CA4
#define GL_STENCIL_BACK_WRITEMASK           0x8CA5
typedef char GLchar;

typedef void (APIENTRY* pfn_glBlendEquationSeparate) (GLenum, GLenum);
extern pfn_glBlendEquationSeparate QGLglBlendEquationSeparate;
#define glBlendEquationSeparate QGLglBlendEquationSeparate

typedef void (APIENTRY* pfn_glDrawBuffers) (GLsizei, const GLenum*);
extern pfn_glDrawBuffers QGLglDrawBuffers;
#define glDrawBuffers QGLglDrawBuffers

typedef void (APIENTRY* pfn_glStencilOpSeparate) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glStencilOpSeparate QGLglStencilOpSeparate;
#define glStencilOpSeparate QGLglStencilOpSeparate

typedef void (APIENTRY* pfn_glStencilFuncSeparate) (GLenum, GLenum, GLint, GLuint);
extern pfn_glStencilFuncSeparate QGLglStencilFuncSeparate;
#define glStencilFuncSeparate QGLglStencilFuncSeparate

typedef void (APIENTRY* pfn_glStencilMaskSeparate) (GLenum, GLuint);
extern pfn_glStencilMaskSeparate QGLglStencilMaskSeparate;
#define glStencilMaskSeparate QGLglStencilMaskSeparate

typedef void (APIENTRY* pfn_glAttachShader) (GLuint, GLuint);
extern pfn_glAttachShader QGLglAttachShader;
#define glAttachShader QGLglAttachShader

typedef void (APIENTRY* pfn_glBindAttribLocation) (GLuint, GLuint, const GLchar*);
extern pfn_glBindAttribLocation QGLglBindAttribLocation;
#define glBindAttribLocation QGLglBindAttribLocation

typedef void (APIENTRY* pfn_glCompileShader) (GLuint);
extern pfn_glCompileShader QGLglCompileShader;
#define glCompileShader QGLglCompileShader

typedef GLuint (APIENTRY* pfn_glCreateProgram) (void);
extern pfn_glCreateProgram QGLglCreateProgram;
#define glCreateProgram QGLglCreateProgram

typedef GLuint (APIENTRY* pfn_glCreateShader) (GLenum);
extern pfn_glCreateShader QGLglCreateShader;
#define glCreateShader QGLglCreateShader

typedef void (APIENTRY* pfn_glDeleteProgram) (GLuint);
extern pfn_glDeleteProgram QGLglDeleteProgram;
#define glDeleteProgram QGLglDeleteProgram

typedef void (APIENTRY* pfn_glDeleteShader) (GLuint);
extern pfn_glDeleteShader QGLglDeleteShader;
#define glDeleteShader QGLglDeleteShader

typedef void (APIENTRY* pfn_glDetachShader) (GLuint, GLuint);
extern pfn_glDetachShader QGLglDetachShader;
#define glDetachShader QGLglDetachShader

typedef void (APIENTRY* pfn_glDisableVertexAttribArray) (GLuint);
extern pfn_glDisableVertexAttribArray QGLglDisableVertexAttribArray;
#define glDisableVertexAttribArray QGLglDisableVertexAttribArray

typedef void (APIENTRY* pfn_glEnableVertexAttribArray) (GLuint);
extern pfn_glEnableVertexAttribArray QGLglEnableVertexAttribArray;
#define glEnableVertexAttribArray QGLglEnableVertexAttribArray

typedef void (APIENTRY* pfn_glGetActiveAttrib) (GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
extern pfn_glGetActiveAttrib QGLglGetActiveAttrib;
#define glGetActiveAttrib QGLglGetActiveAttrib

typedef void (APIENTRY* pfn_glGetActiveUniform) (GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
extern pfn_glGetActiveUniform QGLglGetActiveUniform;
#define glGetActiveUniform QGLglGetActiveUniform

typedef void (APIENTRY* pfn_glGetAttachedShaders) (GLuint, GLsizei, GLsizei*, GLuint*);
extern pfn_glGetAttachedShaders QGLglGetAttachedShaders;
#define glGetAttachedShaders QGLglGetAttachedShaders

typedef GLint (APIENTRY* pfn_glGetAttribLocation) (GLuint, const GLchar*);
extern pfn_glGetAttribLocation QGLglGetAttribLocation;
#define glGetAttribLocation QGLglGetAttribLocation

typedef void (APIENTRY* pfn_glGetProgramiv) (GLuint, GLenum, GLint*);
extern pfn_glGetProgramiv QGLglGetProgramiv;
#define glGetProgramiv QGLglGetProgramiv

typedef void (APIENTRY* pfn_glGetProgramInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
extern pfn_glGetProgramInfoLog QGLglGetProgramInfoLog;
#define glGetProgramInfoLog QGLglGetProgramInfoLog

typedef void (APIENTRY* pfn_glGetShaderiv) (GLuint, GLenum, GLint*);
extern pfn_glGetShaderiv QGLglGetShaderiv;
#define glGetShaderiv QGLglGetShaderiv

typedef void (APIENTRY* pfn_glGetShaderInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
extern pfn_glGetShaderInfoLog QGLglGetShaderInfoLog;
#define glGetShaderInfoLog QGLglGetShaderInfoLog

typedef void (APIENTRY* pfn_glGetShaderSource) (GLuint, GLsizei, GLsizei*, GLchar*);
extern pfn_glGetShaderSource QGLglGetShaderSource;
#define glGetShaderSource QGLglGetShaderSource

typedef GLint (APIENTRY* pfn_glGetUniformLocation) (GLuint, const GLchar*);
extern pfn_glGetUniformLocation QGLglGetUniformLocation;
#define glGetUniformLocation QGLglGetUniformLocation

typedef void (APIENTRY* pfn_glGetUniformfv) (GLuint, GLint, GLfloat*);
extern pfn_glGetUniformfv QGLglGetUniformfv;
#define glGetUniformfv QGLglGetUniformfv

typedef void (APIENTRY* pfn_glGetUniformiv) (GLuint, GLint, GLint*);
extern pfn_glGetUniformiv QGLglGetUniformiv;
#define glGetUniformiv QGLglGetUniformiv

typedef void (APIENTRY* pfn_glGetVertexAttribdv) (GLuint, GLenum, GLdouble*);
extern pfn_glGetVertexAttribdv QGLglGetVertexAttribdv;
#define glGetVertexAttribdv QGLglGetVertexAttribdv

typedef void (APIENTRY* pfn_glGetVertexAttribfv) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVertexAttribfv QGLglGetVertexAttribfv;
#define glGetVertexAttribfv QGLglGetVertexAttribfv

typedef void (APIENTRY* pfn_glGetVertexAttribiv) (GLuint, GLenum, GLint*);
extern pfn_glGetVertexAttribiv QGLglGetVertexAttribiv;
#define glGetVertexAttribiv QGLglGetVertexAttribiv

typedef void (APIENTRY* pfn_glGetVertexAttribPointerv) (GLuint, GLenum, GLvoid**);
extern pfn_glGetVertexAttribPointerv QGLglGetVertexAttribPointerv;
#define glGetVertexAttribPointerv QGLglGetVertexAttribPointerv

typedef GLboolean (APIENTRY* pfn_glIsProgram) (GLuint);
extern pfn_glIsProgram QGLglIsProgram;
#define glIsProgram QGLglIsProgram

typedef GLboolean (APIENTRY* pfn_glIsShader) (GLuint);
extern pfn_glIsShader QGLglIsShader;
#define glIsShader QGLglIsShader

typedef void (APIENTRY* pfn_glLinkProgram) (GLuint);
extern pfn_glLinkProgram QGLglLinkProgram;
#define glLinkProgram QGLglLinkProgram

typedef void (APIENTRY* pfn_glShaderSource) (GLuint, GLsizei, const GLchar**, const GLint*);
extern pfn_glShaderSource QGLglShaderSource;
#define glShaderSource QGLglShaderSource

typedef void (APIENTRY* pfn_glUseProgram) (GLuint);
extern pfn_glUseProgram QGLglUseProgram;
#define glUseProgram QGLglUseProgram

typedef void (APIENTRY* pfn_glUniform1f) (GLint, GLfloat);
extern pfn_glUniform1f QGLglUniform1f;
#define glUniform1f QGLglUniform1f

typedef void (APIENTRY* pfn_glUniform2f) (GLint, GLfloat, GLfloat);
extern pfn_glUniform2f QGLglUniform2f;
#define glUniform2f QGLglUniform2f

typedef void (APIENTRY* pfn_glUniform3f) (GLint, GLfloat, GLfloat, GLfloat);
extern pfn_glUniform3f QGLglUniform3f;
#define glUniform3f QGLglUniform3f

typedef void (APIENTRY* pfn_glUniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glUniform4f QGLglUniform4f;
#define glUniform4f QGLglUniform4f

typedef void (APIENTRY* pfn_glUniform1i) (GLint, GLint);
extern pfn_glUniform1i QGLglUniform1i;
#define glUniform1i QGLglUniform1i

typedef void (APIENTRY* pfn_glUniform2i) (GLint, GLint, GLint);
extern pfn_glUniform2i QGLglUniform2i;
#define glUniform2i QGLglUniform2i

typedef void (APIENTRY* pfn_glUniform3i) (GLint, GLint, GLint, GLint);
extern pfn_glUniform3i QGLglUniform3i;
#define glUniform3i QGLglUniform3i

typedef void (APIENTRY* pfn_glUniform4i) (GLint, GLint, GLint, GLint, GLint);
extern pfn_glUniform4i QGLglUniform4i;
#define glUniform4i QGLglUniform4i

typedef void (APIENTRY* pfn_glUniform1fv) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform1fv QGLglUniform1fv;
#define glUniform1fv QGLglUniform1fv

typedef void (APIENTRY* pfn_glUniform2fv) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform2fv QGLglUniform2fv;
#define glUniform2fv QGLglUniform2fv

typedef void (APIENTRY* pfn_glUniform3fv) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform3fv QGLglUniform3fv;
#define glUniform3fv QGLglUniform3fv

typedef void (APIENTRY* pfn_glUniform4fv) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform4fv QGLglUniform4fv;
#define glUniform4fv QGLglUniform4fv

typedef void (APIENTRY* pfn_glUniform1iv) (GLint, GLsizei, const GLint*);
extern pfn_glUniform1iv QGLglUniform1iv;
#define glUniform1iv QGLglUniform1iv

typedef void (APIENTRY* pfn_glUniform2iv) (GLint, GLsizei, const GLint*);
extern pfn_glUniform2iv QGLglUniform2iv;
#define glUniform2iv QGLglUniform2iv

typedef void (APIENTRY* pfn_glUniform3iv) (GLint, GLsizei, const GLint*);
extern pfn_glUniform3iv QGLglUniform3iv;
#define glUniform3iv QGLglUniform3iv

typedef void (APIENTRY* pfn_glUniform4iv) (GLint, GLsizei, const GLint*);
extern pfn_glUniform4iv QGLglUniform4iv;
#define glUniform4iv QGLglUniform4iv

typedef void (APIENTRY* pfn_glUniformMatrix2fv) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix2fv QGLglUniformMatrix2fv;
#define glUniformMatrix2fv QGLglUniformMatrix2fv

typedef void (APIENTRY* pfn_glUniformMatrix3fv) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix3fv QGLglUniformMatrix3fv;
#define glUniformMatrix3fv QGLglUniformMatrix3fv

typedef void (APIENTRY* pfn_glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix4fv QGLglUniformMatrix4fv;
#define glUniformMatrix4fv QGLglUniformMatrix4fv

typedef void (APIENTRY* pfn_glValidateProgram) (GLuint);
extern pfn_glValidateProgram QGLglValidateProgram;
#define glValidateProgram QGLglValidateProgram

typedef void (APIENTRY* pfn_glVertexAttrib1d) (GLuint, GLdouble);
extern pfn_glVertexAttrib1d QGLglVertexAttrib1d;
#define glVertexAttrib1d QGLglVertexAttrib1d

typedef void (APIENTRY* pfn_glVertexAttrib1dv) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib1dv QGLglVertexAttrib1dv;
#define glVertexAttrib1dv QGLglVertexAttrib1dv

typedef void (APIENTRY* pfn_glVertexAttrib1f) (GLuint, GLfloat);
extern pfn_glVertexAttrib1f QGLglVertexAttrib1f;
#define glVertexAttrib1f QGLglVertexAttrib1f

typedef void (APIENTRY* pfn_glVertexAttrib1fv) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib1fv QGLglVertexAttrib1fv;
#define glVertexAttrib1fv QGLglVertexAttrib1fv

typedef void (APIENTRY* pfn_glVertexAttrib1s) (GLuint, GLshort);
extern pfn_glVertexAttrib1s QGLglVertexAttrib1s;
#define glVertexAttrib1s QGLglVertexAttrib1s

typedef void (APIENTRY* pfn_glVertexAttrib1sv) (GLuint, const GLshort*);
extern pfn_glVertexAttrib1sv QGLglVertexAttrib1sv;
#define glVertexAttrib1sv QGLglVertexAttrib1sv

typedef void (APIENTRY* pfn_glVertexAttrib2d) (GLuint, GLdouble, GLdouble);
extern pfn_glVertexAttrib2d QGLglVertexAttrib2d;
#define glVertexAttrib2d QGLglVertexAttrib2d

typedef void (APIENTRY* pfn_glVertexAttrib2dv) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib2dv QGLglVertexAttrib2dv;
#define glVertexAttrib2dv QGLglVertexAttrib2dv

typedef void (APIENTRY* pfn_glVertexAttrib2f) (GLuint, GLfloat, GLfloat);
extern pfn_glVertexAttrib2f QGLglVertexAttrib2f;
#define glVertexAttrib2f QGLglVertexAttrib2f

typedef void (APIENTRY* pfn_glVertexAttrib2fv) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib2fv QGLglVertexAttrib2fv;
#define glVertexAttrib2fv QGLglVertexAttrib2fv

typedef void (APIENTRY* pfn_glVertexAttrib2s) (GLuint, GLshort, GLshort);
extern pfn_glVertexAttrib2s QGLglVertexAttrib2s;
#define glVertexAttrib2s QGLglVertexAttrib2s

typedef void (APIENTRY* pfn_glVertexAttrib2sv) (GLuint, const GLshort*);
extern pfn_glVertexAttrib2sv QGLglVertexAttrib2sv;
#define glVertexAttrib2sv QGLglVertexAttrib2sv

typedef void (APIENTRY* pfn_glVertexAttrib3d) (GLuint, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib3d QGLglVertexAttrib3d;
#define glVertexAttrib3d QGLglVertexAttrib3d

typedef void (APIENTRY* pfn_glVertexAttrib3dv) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib3dv QGLglVertexAttrib3dv;
#define glVertexAttrib3dv QGLglVertexAttrib3dv

typedef void (APIENTRY* pfn_glVertexAttrib3f) (GLuint, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib3f QGLglVertexAttrib3f;
#define glVertexAttrib3f QGLglVertexAttrib3f

typedef void (APIENTRY* pfn_glVertexAttrib3fv) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib3fv QGLglVertexAttrib3fv;
#define glVertexAttrib3fv QGLglVertexAttrib3fv

typedef void (APIENTRY* pfn_glVertexAttrib3s) (GLuint, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib3s QGLglVertexAttrib3s;
#define glVertexAttrib3s QGLglVertexAttrib3s

typedef void (APIENTRY* pfn_glVertexAttrib3sv) (GLuint, const GLshort*);
extern pfn_glVertexAttrib3sv QGLglVertexAttrib3sv;
#define glVertexAttrib3sv QGLglVertexAttrib3sv

typedef void (APIENTRY* pfn_glVertexAttrib4Nbv) (GLuint, const GLbyte*);
extern pfn_glVertexAttrib4Nbv QGLglVertexAttrib4Nbv;
#define glVertexAttrib4Nbv QGLglVertexAttrib4Nbv

typedef void (APIENTRY* pfn_glVertexAttrib4Niv) (GLuint, const GLint*);
extern pfn_glVertexAttrib4Niv QGLglVertexAttrib4Niv;
#define glVertexAttrib4Niv QGLglVertexAttrib4Niv

typedef void (APIENTRY* pfn_glVertexAttrib4Nsv) (GLuint, const GLshort*);
extern pfn_glVertexAttrib4Nsv QGLglVertexAttrib4Nsv;
#define glVertexAttrib4Nsv QGLglVertexAttrib4Nsv

typedef void (APIENTRY* pfn_glVertexAttrib4Nub) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
extern pfn_glVertexAttrib4Nub QGLglVertexAttrib4Nub;
#define glVertexAttrib4Nub QGLglVertexAttrib4Nub

typedef void (APIENTRY* pfn_glVertexAttrib4Nubv) (GLuint, const GLubyte*);
extern pfn_glVertexAttrib4Nubv QGLglVertexAttrib4Nubv;
#define glVertexAttrib4Nubv QGLglVertexAttrib4Nubv

typedef void (APIENTRY* pfn_glVertexAttrib4Nuiv) (GLuint, const GLuint*);
extern pfn_glVertexAttrib4Nuiv QGLglVertexAttrib4Nuiv;
#define glVertexAttrib4Nuiv QGLglVertexAttrib4Nuiv

typedef void (APIENTRY* pfn_glVertexAttrib4Nusv) (GLuint, const GLushort*);
extern pfn_glVertexAttrib4Nusv QGLglVertexAttrib4Nusv;
#define glVertexAttrib4Nusv QGLglVertexAttrib4Nusv

typedef void (APIENTRY* pfn_glVertexAttrib4bv) (GLuint, const GLbyte*);
extern pfn_glVertexAttrib4bv QGLglVertexAttrib4bv;
#define glVertexAttrib4bv QGLglVertexAttrib4bv

typedef void (APIENTRY* pfn_glVertexAttrib4d) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib4d QGLglVertexAttrib4d;
#define glVertexAttrib4d QGLglVertexAttrib4d

typedef void (APIENTRY* pfn_glVertexAttrib4dv) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib4dv QGLglVertexAttrib4dv;
#define glVertexAttrib4dv QGLglVertexAttrib4dv

typedef void (APIENTRY* pfn_glVertexAttrib4f) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib4f QGLglVertexAttrib4f;
#define glVertexAttrib4f QGLglVertexAttrib4f

typedef void (APIENTRY* pfn_glVertexAttrib4fv) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib4fv QGLglVertexAttrib4fv;
#define glVertexAttrib4fv QGLglVertexAttrib4fv

typedef void (APIENTRY* pfn_glVertexAttrib4iv) (GLuint, const GLint*);
extern pfn_glVertexAttrib4iv QGLglVertexAttrib4iv;
#define glVertexAttrib4iv QGLglVertexAttrib4iv

typedef void (APIENTRY* pfn_glVertexAttrib4s) (GLuint, GLshort, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib4s QGLglVertexAttrib4s;
#define glVertexAttrib4s QGLglVertexAttrib4s

typedef void (APIENTRY* pfn_glVertexAttrib4sv) (GLuint, const GLshort*);
extern pfn_glVertexAttrib4sv QGLglVertexAttrib4sv;
#define glVertexAttrib4sv QGLglVertexAttrib4sv

typedef void (APIENTRY* pfn_glVertexAttrib4ubv) (GLuint, const GLubyte*);
extern pfn_glVertexAttrib4ubv QGLglVertexAttrib4ubv;
#define glVertexAttrib4ubv QGLglVertexAttrib4ubv

typedef void (APIENTRY* pfn_glVertexAttrib4uiv) (GLuint, const GLuint*);
extern pfn_glVertexAttrib4uiv QGLglVertexAttrib4uiv;
#define glVertexAttrib4uiv QGLglVertexAttrib4uiv

typedef void (APIENTRY* pfn_glVertexAttrib4usv) (GLuint, const GLushort*);
extern pfn_glVertexAttrib4usv QGLglVertexAttrib4usv;
#define glVertexAttrib4usv QGLglVertexAttrib4usv

typedef void (APIENTRY* pfn_glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern pfn_glVertexAttribPointer QGLglVertexAttribPointer;
#define glVertexAttribPointer QGLglVertexAttribPointer
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_imaging;
extern int QGLNumFunctions_GL_ARB_imaging;
#ifndef GL_ARB_imaging
#define GL_ARB_imaging
#define QGL_DEFINED_GL_ARB_imaging
#define GL_CONSTANT_COLOR                       0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR             0x8002
#define GL_CONSTANT_ALPHA                       0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA             0x8004
#define GL_BLEND_COLOR                          0x8005
#define GL_FUNC_ADD                             0x8006
#define GL_MIN                                  0x8007
#define GL_MAX                                  0x8008
#define GL_BLEND_EQUATION                       0x8009
#define GL_FUNC_SUBTRACT                        0x800A
#define GL_FUNC_REVERSE_SUBTRACT                0x800B
#define GL_CONVOLUTION_1D                       0x8010
#define GL_CONVOLUTION_2D                       0x8011
#define GL_SEPARABLE_2D                         0x8012
#define GL_CONVOLUTION_BORDER_MODE              0x8013
#define GL_CONVOLUTION_FILTER_SCALE             0x8014
#define GL_CONVOLUTION_FILTER_BIAS              0x8015
#define GL_REDUCE                               0x8016
#define GL_CONVOLUTION_FORMAT                   0x8017
#define GL_CONVOLUTION_WIDTH                    0x8018
#define GL_CONVOLUTION_HEIGHT                   0x8019
#define GL_MAX_CONVOLUTION_WIDTH                0x801A
#define GL_MAX_CONVOLUTION_HEIGHT               0x801B
#define GL_POST_CONVOLUTION_RED_SCALE           0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE         0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE          0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE         0x801F
#define GL_POST_CONVOLUTION_RED_BIAS            0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS          0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS           0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS          0x8023
#define GL_HISTOGRAM                            0x8024
#define GL_PROXY_HISTOGRAM                      0x8025
#define GL_HISTOGRAM_WIDTH                      0x8026
#define GL_HISTOGRAM_FORMAT                     0x8027
#define GL_HISTOGRAM_RED_SIZE                   0x8028
#define GL_HISTOGRAM_GREEN_SIZE                 0x8029
#define GL_HISTOGRAM_BLUE_SIZE                  0x802A
#define GL_HISTOGRAM_ALPHA_SIZE                 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE             0x802C
#define GL_HISTOGRAM_SINK                       0x802D
#define GL_MINMAX                               0x802E
#define GL_MINMAX_FORMAT                        0x802F
#define GL_MINMAX_SINK                          0x8030
#define GL_TABLE_TOO_LARGE                      0x8031
#define GL_COLOR_MATRIX                         0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH             0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH         0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE          0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE        0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE         0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE        0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS           0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS         0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS          0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS         0x80BB
#define GL_COLOR_TABLE                          0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE         0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE        0x80D2
#define GL_PROXY_COLOR_TABLE                    0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE   0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE  0x80D5
#define GL_COLOR_TABLE_SCALE                    0x80D6
#define GL_COLOR_TABLE_BIAS                     0x80D7
#define GL_COLOR_TABLE_FORMAT                   0x80D8
#define GL_COLOR_TABLE_WIDTH                    0x80D9
#define GL_COLOR_TABLE_RED_SIZE                 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE               0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE                0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE               0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE           0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE           0x80DF
#define GL_CONSTANT_BORDER                      0x8151
#define GL_REPLICATE_BORDER                     0x8153
#define GL_CONVOLUTION_BORDER_COLOR             0x8154
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_multitexture;
extern int QGLNumFunctions_GL_ARB_multitexture;
#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture
#define QGL_DEFINED_GL_ARB_multitexture
#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3
#define GL_TEXTURE4_ARB                   0x84C4
#define GL_TEXTURE5_ARB                   0x84C5
#define GL_TEXTURE6_ARB                   0x84C6
#define GL_TEXTURE7_ARB                   0x84C7
#define GL_TEXTURE8_ARB                   0x84C8
#define GL_TEXTURE9_ARB                   0x84C9
#define GL_TEXTURE10_ARB                  0x84CA
#define GL_TEXTURE11_ARB                  0x84CB
#define GL_TEXTURE12_ARB                  0x84CC
#define GL_TEXTURE13_ARB                  0x84CD
#define GL_TEXTURE14_ARB                  0x84CE
#define GL_TEXTURE15_ARB                  0x84CF
#define GL_TEXTURE16_ARB                  0x84D0
#define GL_TEXTURE17_ARB                  0x84D1
#define GL_TEXTURE18_ARB                  0x84D2
#define GL_TEXTURE19_ARB                  0x84D3
#define GL_TEXTURE20_ARB                  0x84D4
#define GL_TEXTURE21_ARB                  0x84D5
#define GL_TEXTURE22_ARB                  0x84D6
#define GL_TEXTURE23_ARB                  0x84D7
#define GL_TEXTURE24_ARB                  0x84D8
#define GL_TEXTURE25_ARB                  0x84D9
#define GL_TEXTURE26_ARB                  0x84DA
#define GL_TEXTURE27_ARB                  0x84DB
#define GL_TEXTURE28_ARB                  0x84DC
#define GL_TEXTURE29_ARB                  0x84DD
#define GL_TEXTURE30_ARB                  0x84DE
#define GL_TEXTURE31_ARB                  0x84DF
#define GL_ACTIVE_TEXTURE_ARB             0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB      0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB          0x84E2

typedef void (APIENTRY* pfn_glActiveTextureARB) (GLenum);
extern pfn_glActiveTextureARB QGLglActiveTextureARB;
#define glActiveTextureARB QGLglActiveTextureARB

typedef void (APIENTRY* pfn_glClientActiveTextureARB) (GLenum);
extern pfn_glClientActiveTextureARB QGLglClientActiveTextureARB;
#define glClientActiveTextureARB QGLglClientActiveTextureARB

typedef void (APIENTRY* pfn_glMultiTexCoord1dARB) (GLenum, GLdouble);
extern pfn_glMultiTexCoord1dARB QGLglMultiTexCoord1dARB;
#define glMultiTexCoord1dARB QGLglMultiTexCoord1dARB

typedef void (APIENTRY* pfn_glMultiTexCoord1dvARB) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord1dvARB QGLglMultiTexCoord1dvARB;
#define glMultiTexCoord1dvARB QGLglMultiTexCoord1dvARB

typedef void (APIENTRY* pfn_glMultiTexCoord1fARB) (GLenum, GLfloat);
extern pfn_glMultiTexCoord1fARB QGLglMultiTexCoord1fARB;
#define glMultiTexCoord1fARB QGLglMultiTexCoord1fARB

typedef void (APIENTRY* pfn_glMultiTexCoord1fvARB) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord1fvARB QGLglMultiTexCoord1fvARB;
#define glMultiTexCoord1fvARB QGLglMultiTexCoord1fvARB

typedef void (APIENTRY* pfn_glMultiTexCoord1iARB) (GLenum, GLint);
extern pfn_glMultiTexCoord1iARB QGLglMultiTexCoord1iARB;
#define glMultiTexCoord1iARB QGLglMultiTexCoord1iARB

typedef void (APIENTRY* pfn_glMultiTexCoord1ivARB) (GLenum, const GLint*);
extern pfn_glMultiTexCoord1ivARB QGLglMultiTexCoord1ivARB;
#define glMultiTexCoord1ivARB QGLglMultiTexCoord1ivARB

typedef void (APIENTRY* pfn_glMultiTexCoord1sARB) (GLenum, GLshort);
extern pfn_glMultiTexCoord1sARB QGLglMultiTexCoord1sARB;
#define glMultiTexCoord1sARB QGLglMultiTexCoord1sARB

typedef void (APIENTRY* pfn_glMultiTexCoord1svARB) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord1svARB QGLglMultiTexCoord1svARB;
#define glMultiTexCoord1svARB QGLglMultiTexCoord1svARB

typedef void (APIENTRY* pfn_glMultiTexCoord2dARB) (GLenum, GLdouble, GLdouble);
extern pfn_glMultiTexCoord2dARB QGLglMultiTexCoord2dARB;
#define glMultiTexCoord2dARB QGLglMultiTexCoord2dARB

typedef void (APIENTRY* pfn_glMultiTexCoord2dvARB) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord2dvARB QGLglMultiTexCoord2dvARB;
#define glMultiTexCoord2dvARB QGLglMultiTexCoord2dvARB

typedef void (APIENTRY* pfn_glMultiTexCoord2fARB) (GLenum, GLfloat, GLfloat);
extern pfn_glMultiTexCoord2fARB QGLglMultiTexCoord2fARB;
#define glMultiTexCoord2fARB QGLglMultiTexCoord2fARB

typedef void (APIENTRY* pfn_glMultiTexCoord2fvARB) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord2fvARB QGLglMultiTexCoord2fvARB;
#define glMultiTexCoord2fvARB QGLglMultiTexCoord2fvARB

typedef void (APIENTRY* pfn_glMultiTexCoord2iARB) (GLenum, GLint, GLint);
extern pfn_glMultiTexCoord2iARB QGLglMultiTexCoord2iARB;
#define glMultiTexCoord2iARB QGLglMultiTexCoord2iARB

typedef void (APIENTRY* pfn_glMultiTexCoord2ivARB) (GLenum, const GLint*);
extern pfn_glMultiTexCoord2ivARB QGLglMultiTexCoord2ivARB;
#define glMultiTexCoord2ivARB QGLglMultiTexCoord2ivARB

typedef void (APIENTRY* pfn_glMultiTexCoord2sARB) (GLenum, GLshort, GLshort);
extern pfn_glMultiTexCoord2sARB QGLglMultiTexCoord2sARB;
#define glMultiTexCoord2sARB QGLglMultiTexCoord2sARB

typedef void (APIENTRY* pfn_glMultiTexCoord2svARB) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord2svARB QGLglMultiTexCoord2svARB;
#define glMultiTexCoord2svARB QGLglMultiTexCoord2svARB

typedef void (APIENTRY* pfn_glMultiTexCoord3dARB) (GLenum, GLdouble, GLdouble, GLdouble);
extern pfn_glMultiTexCoord3dARB QGLglMultiTexCoord3dARB;
#define glMultiTexCoord3dARB QGLglMultiTexCoord3dARB

typedef void (APIENTRY* pfn_glMultiTexCoord3dvARB) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord3dvARB QGLglMultiTexCoord3dvARB;
#define glMultiTexCoord3dvARB QGLglMultiTexCoord3dvARB

typedef void (APIENTRY* pfn_glMultiTexCoord3fARB) (GLenum, GLfloat, GLfloat, GLfloat);
extern pfn_glMultiTexCoord3fARB QGLglMultiTexCoord3fARB;
#define glMultiTexCoord3fARB QGLglMultiTexCoord3fARB

typedef void (APIENTRY* pfn_glMultiTexCoord3fvARB) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord3fvARB QGLglMultiTexCoord3fvARB;
#define glMultiTexCoord3fvARB QGLglMultiTexCoord3fvARB

typedef void (APIENTRY* pfn_glMultiTexCoord3iARB) (GLenum, GLint, GLint, GLint);
extern pfn_glMultiTexCoord3iARB QGLglMultiTexCoord3iARB;
#define glMultiTexCoord3iARB QGLglMultiTexCoord3iARB

typedef void (APIENTRY* pfn_glMultiTexCoord3ivARB) (GLenum, const GLint*);
extern pfn_glMultiTexCoord3ivARB QGLglMultiTexCoord3ivARB;
#define glMultiTexCoord3ivARB QGLglMultiTexCoord3ivARB

typedef void (APIENTRY* pfn_glMultiTexCoord3sARB) (GLenum, GLshort, GLshort, GLshort);
extern pfn_glMultiTexCoord3sARB QGLglMultiTexCoord3sARB;
#define glMultiTexCoord3sARB QGLglMultiTexCoord3sARB

typedef void (APIENTRY* pfn_glMultiTexCoord3svARB) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord3svARB QGLglMultiTexCoord3svARB;
#define glMultiTexCoord3svARB QGLglMultiTexCoord3svARB

typedef void (APIENTRY* pfn_glMultiTexCoord4dARB) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glMultiTexCoord4dARB QGLglMultiTexCoord4dARB;
#define glMultiTexCoord4dARB QGLglMultiTexCoord4dARB

typedef void (APIENTRY* pfn_glMultiTexCoord4dvARB) (GLenum, const GLdouble*);
extern pfn_glMultiTexCoord4dvARB QGLglMultiTexCoord4dvARB;
#define glMultiTexCoord4dvARB QGLglMultiTexCoord4dvARB

typedef void (APIENTRY* pfn_glMultiTexCoord4fARB) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glMultiTexCoord4fARB QGLglMultiTexCoord4fARB;
#define glMultiTexCoord4fARB QGLglMultiTexCoord4fARB

typedef void (APIENTRY* pfn_glMultiTexCoord4fvARB) (GLenum, const GLfloat*);
extern pfn_glMultiTexCoord4fvARB QGLglMultiTexCoord4fvARB;
#define glMultiTexCoord4fvARB QGLglMultiTexCoord4fvARB

typedef void (APIENTRY* pfn_glMultiTexCoord4iARB) (GLenum, GLint, GLint, GLint, GLint);
extern pfn_glMultiTexCoord4iARB QGLglMultiTexCoord4iARB;
#define glMultiTexCoord4iARB QGLglMultiTexCoord4iARB

typedef void (APIENTRY* pfn_glMultiTexCoord4ivARB) (GLenum, const GLint*);
extern pfn_glMultiTexCoord4ivARB QGLglMultiTexCoord4ivARB;
#define glMultiTexCoord4ivARB QGLglMultiTexCoord4ivARB

typedef void (APIENTRY* pfn_glMultiTexCoord4sARB) (GLenum, GLshort, GLshort, GLshort, GLshort);
extern pfn_glMultiTexCoord4sARB QGLglMultiTexCoord4sARB;
#define glMultiTexCoord4sARB QGLglMultiTexCoord4sARB

typedef void (APIENTRY* pfn_glMultiTexCoord4svARB) (GLenum, const GLshort*);
extern pfn_glMultiTexCoord4svARB QGLglMultiTexCoord4svARB;
#define glMultiTexCoord4svARB QGLglMultiTexCoord4svARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_transpose_matrix;
extern int QGLNumFunctions_GL_ARB_transpose_matrix;
#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix
#define QGL_DEFINED_GL_ARB_transpose_matrix
#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB   0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB  0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB     0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB       0x84E6

typedef void (APIENTRY* pfn_glLoadTransposeMatrixfARB) (const GLfloat*);
extern pfn_glLoadTransposeMatrixfARB QGLglLoadTransposeMatrixfARB;
#define glLoadTransposeMatrixfARB QGLglLoadTransposeMatrixfARB

typedef void (APIENTRY* pfn_glLoadTransposeMatrixdARB) (const GLdouble*);
extern pfn_glLoadTransposeMatrixdARB QGLglLoadTransposeMatrixdARB;
#define glLoadTransposeMatrixdARB QGLglLoadTransposeMatrixdARB

typedef void (APIENTRY* pfn_glMultTransposeMatrixfARB) (const GLfloat*);
extern pfn_glMultTransposeMatrixfARB QGLglMultTransposeMatrixfARB;
#define glMultTransposeMatrixfARB QGLglMultTransposeMatrixfARB

typedef void (APIENTRY* pfn_glMultTransposeMatrixdARB) (const GLdouble*);
extern pfn_glMultTransposeMatrixdARB QGLglMultTransposeMatrixdARB;
#define glMultTransposeMatrixdARB QGLglMultTransposeMatrixdARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_multisample;
extern int QGLNumFunctions_GL_ARB_multisample;
#ifndef GL_ARB_multisample
#define GL_ARB_multisample
#define QGL_DEFINED_GL_ARB_multisample
#define GL_MULTISAMPLE_ARB                0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB   0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB        0x809F
#define GL_SAMPLE_COVERAGE_ARB            0x80A0
#define GL_SAMPLE_BUFFERS_ARB             0x80A8
#define GL_SAMPLES_ARB                    0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB      0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB     0x80AB
#define GL_MULTISAMPLE_BIT_ARB            0x20000000

typedef void (APIENTRY* pfn_glSampleCoverageARB) (GLclampf, GLboolean);
extern pfn_glSampleCoverageARB QGLglSampleCoverageARB;
#define glSampleCoverageARB QGLglSampleCoverageARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_env_add;
extern int QGLNumFunctions_GL_ARB_texture_env_add;
#ifndef GL_ARB_texture_env_add
#define GL_ARB_texture_env_add
#define QGL_DEFINED_GL_ARB_texture_env_add
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_cube_map;
extern int QGLNumFunctions_GL_ARB_texture_cube_map;
#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map
#define QGL_DEFINED_GL_ARB_texture_cube_map
#define GL_NORMAL_MAP_ARB                   0x8511
#define GL_REFLECTION_MAP_ARB               0x8512
#define GL_TEXTURE_CUBE_MAP_ARB             0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB     0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB  0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB  0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB  0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB  0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB  0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB  0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB       0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB    0x851C
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_compression;
extern int QGLNumFunctions_GL_ARB_texture_compression;
#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression
#define QGL_DEFINED_GL_ARB_texture_compression
#define GL_COMPRESSED_ALPHA_ARB                 0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB             0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB       0x84EB
#define GL_COMPRESSED_INTENSITY_ARB             0x84EC
#define GL_COMPRESSED_RGB_ARB                   0x84ED
#define GL_COMPRESSED_RGBA_ARB                  0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB         0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB    0x86A0
#define GL_TEXTURE_COMPRESSED_ARB               0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB   0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB       0x86A3

typedef void (APIENTRY* pfn_glCompressedTexImage3DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage3DARB QGLglCompressedTexImage3DARB;
#define glCompressedTexImage3DARB QGLglCompressedTexImage3DARB

typedef void (APIENTRY* pfn_glCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage2DARB QGLglCompressedTexImage2DARB;
#define glCompressedTexImage2DARB QGLglCompressedTexImage2DARB

typedef void (APIENTRY* pfn_glCompressedTexImage1DARB) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*);
extern pfn_glCompressedTexImage1DARB QGLglCompressedTexImage1DARB;
#define glCompressedTexImage1DARB QGLglCompressedTexImage1DARB

typedef void (APIENTRY* pfn_glCompressedTexSubImage3DARB) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage3DARB QGLglCompressedTexSubImage3DARB;
#define glCompressedTexSubImage3DARB QGLglCompressedTexSubImage3DARB

typedef void (APIENTRY* pfn_glCompressedTexSubImage2DARB) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage2DARB QGLglCompressedTexSubImage2DARB;
#define glCompressedTexSubImage2DARB QGLglCompressedTexSubImage2DARB

typedef void (APIENTRY* pfn_glCompressedTexSubImage1DARB) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glCompressedTexSubImage1DARB QGLglCompressedTexSubImage1DARB;
#define glCompressedTexSubImage1DARB QGLglCompressedTexSubImage1DARB

typedef void (APIENTRY* pfn_glGetCompressedTexImageARB) (GLenum, GLint, GLvoid*);
extern pfn_glGetCompressedTexImageARB QGLglGetCompressedTexImageARB;
#define glGetCompressedTexImageARB QGLglGetCompressedTexImageARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_border_clamp;
extern int QGLNumFunctions_GL_ARB_texture_border_clamp;
#ifndef GL_ARB_texture_border_clamp
#define GL_ARB_texture_border_clamp
#define QGL_DEFINED_GL_ARB_texture_border_clamp
#define GL_CLAMP_TO_BORDER_ARB            0x812D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_point_parameters;
extern int QGLNumFunctions_GL_ARB_point_parameters;
#ifndef GL_ARB_point_parameters
#define GL_ARB_point_parameters
#define QGL_DEFINED_GL_ARB_point_parameters
#define GL_POINT_SIZE_MIN_ARB             0x8126
#define GL_POINT_SIZE_MAX_ARB             0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB  0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

typedef void (APIENTRY* pfn_glPointParameterfARB) (GLenum, GLfloat);
extern pfn_glPointParameterfARB QGLglPointParameterfARB;
#define glPointParameterfARB QGLglPointParameterfARB

typedef void (APIENTRY* pfn_glPointParameterfvARB) (GLenum, const GLfloat*);
extern pfn_glPointParameterfvARB QGLglPointParameterfvARB;
#define glPointParameterfvARB QGLglPointParameterfvARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_vertex_blend;
extern int QGLNumFunctions_GL_ARB_vertex_blend;
#ifndef GL_ARB_vertex_blend
#define GL_ARB_vertex_blend
#define QGL_DEFINED_GL_ARB_vertex_blend
#define GL_MAX_VERTEX_UNITS_ARB           0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB        0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB           0x86A6
#define GL_VERTEX_BLEND_ARB               0x86A7
#define GL_CURRENT_WEIGHT_ARB             0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB          0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB        0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB          0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB       0x86AC
#define GL_WEIGHT_ARRAY_ARB               0x86AD
#define GL_MODELVIEW0_ARB                 0x1700
#define GL_MODELVIEW1_ARB                 0x850A
#define GL_MODELVIEW2_ARB                 0x8722
#define GL_MODELVIEW3_ARB                 0x8723
#define GL_MODELVIEW4_ARB                 0x8724
#define GL_MODELVIEW5_ARB                 0x8725
#define GL_MODELVIEW6_ARB                 0x8726
#define GL_MODELVIEW7_ARB                 0x8727
#define GL_MODELVIEW8_ARB                 0x8728
#define GL_MODELVIEW9_ARB                 0x8729
#define GL_MODELVIEW10_ARB                0x872A
#define GL_MODELVIEW11_ARB                0x872B
#define GL_MODELVIEW12_ARB                0x872C
#define GL_MODELVIEW13_ARB                0x872D
#define GL_MODELVIEW14_ARB                0x872E
#define GL_MODELVIEW15_ARB                0x872F
#define GL_MODELVIEW16_ARB                0x8730
#define GL_MODELVIEW17_ARB                0x8731
#define GL_MODELVIEW18_ARB                0x8732
#define GL_MODELVIEW19_ARB                0x8733
#define GL_MODELVIEW20_ARB                0x8734
#define GL_MODELVIEW21_ARB                0x8735
#define GL_MODELVIEW22_ARB                0x8736
#define GL_MODELVIEW23_ARB                0x8737
#define GL_MODELVIEW24_ARB                0x8738
#define GL_MODELVIEW25_ARB                0x8739
#define GL_MODELVIEW26_ARB                0x873A
#define GL_MODELVIEW27_ARB                0x873B
#define GL_MODELVIEW28_ARB                0x873C
#define GL_MODELVIEW29_ARB                0x873D
#define GL_MODELVIEW30_ARB                0x873E
#define GL_MODELVIEW31_ARB                0x873F

typedef void (APIENTRY* pfn_glWeightbvARB) (GLint, const GLbyte*);
extern pfn_glWeightbvARB QGLglWeightbvARB;
#define glWeightbvARB QGLglWeightbvARB

typedef void (APIENTRY* pfn_glWeightsvARB) (GLint, const GLshort*);
extern pfn_glWeightsvARB QGLglWeightsvARB;
#define glWeightsvARB QGLglWeightsvARB

typedef void (APIENTRY* pfn_glWeightivARB) (GLint, const GLint*);
extern pfn_glWeightivARB QGLglWeightivARB;
#define glWeightivARB QGLglWeightivARB

typedef void (APIENTRY* pfn_glWeightfvARB) (GLint, const GLfloat*);
extern pfn_glWeightfvARB QGLglWeightfvARB;
#define glWeightfvARB QGLglWeightfvARB

typedef void (APIENTRY* pfn_glWeightdvARB) (GLint, const GLdouble*);
extern pfn_glWeightdvARB QGLglWeightdvARB;
#define glWeightdvARB QGLglWeightdvARB

typedef void (APIENTRY* pfn_glWeightubvARB) (GLint, const GLubyte*);
extern pfn_glWeightubvARB QGLglWeightubvARB;
#define glWeightubvARB QGLglWeightubvARB

typedef void (APIENTRY* pfn_glWeightusvARB) (GLint, const GLushort*);
extern pfn_glWeightusvARB QGLglWeightusvARB;
#define glWeightusvARB QGLglWeightusvARB

typedef void (APIENTRY* pfn_glWeightuivARB) (GLint, const GLuint*);
extern pfn_glWeightuivARB QGLglWeightuivARB;
#define glWeightuivARB QGLglWeightuivARB

typedef void (APIENTRY* pfn_glWeightPointerARB) (GLint, GLenum, GLsizei, const GLvoid*);
extern pfn_glWeightPointerARB QGLglWeightPointerARB;
#define glWeightPointerARB QGLglWeightPointerARB

typedef void (APIENTRY* pfn_glVertexBlendARB) (GLint);
extern pfn_glVertexBlendARB QGLglVertexBlendARB;
#define glVertexBlendARB QGLglVertexBlendARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_matrix_palette;
extern int QGLNumFunctions_GL_ARB_matrix_palette;
#ifndef GL_ARB_matrix_palette
#define GL_ARB_matrix_palette
#define QGL_DEFINED_GL_ARB_matrix_palette
#define GL_MATRIX_PALETTE_ARB                   0x8840
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB   0x8841
#define GL_MAX_PALETTE_MATRICES_ARB             0x8842
#define GL_CURRENT_PALETTE_MATRIX_ARB           0x8843
#define GL_MATRIX_INDEX_ARRAY_ARB               0x8844
#define GL_CURRENT_MATRIX_INDEX_ARB             0x8845
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB          0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB          0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB        0x8848
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB       0x8849

typedef void (APIENTRY* pfn_glCurrentPaletteMatrixARB) (GLint);
extern pfn_glCurrentPaletteMatrixARB QGLglCurrentPaletteMatrixARB;
#define glCurrentPaletteMatrixARB QGLglCurrentPaletteMatrixARB

typedef void (APIENTRY* pfn_glMatrixIndexubvARB) (GLint, const GLubyte*);
extern pfn_glMatrixIndexubvARB QGLglMatrixIndexubvARB;
#define glMatrixIndexubvARB QGLglMatrixIndexubvARB

typedef void (APIENTRY* pfn_glMatrixIndexusvARB) (GLint, const GLushort*);
extern pfn_glMatrixIndexusvARB QGLglMatrixIndexusvARB;
#define glMatrixIndexusvARB QGLglMatrixIndexusvARB

typedef void (APIENTRY* pfn_glMatrixIndexuivARB) (GLint, const GLuint*);
extern pfn_glMatrixIndexuivARB QGLglMatrixIndexuivARB;
#define glMatrixIndexuivARB QGLglMatrixIndexuivARB

typedef void (APIENTRY* pfn_glMatrixIndexPointerARB) (GLint, GLenum, GLsizei, const GLvoid*);
extern pfn_glMatrixIndexPointerARB QGLglMatrixIndexPointerARB;
#define glMatrixIndexPointerARB QGLglMatrixIndexPointerARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_env_combine;
extern int QGLNumFunctions_GL_ARB_texture_env_combine;
#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine
#define QGL_DEFINED_GL_ARB_texture_env_combine
#define GL_COMBINE_ARB                    0x8570
#define GL_COMBINE_RGB_ARB                0x8571
#define GL_COMBINE_ALPHA_ARB              0x8572
#define GL_SOURCE0_RGB_ARB                0x8580
#define GL_SOURCE1_RGB_ARB                0x8581
#define GL_SOURCE2_RGB_ARB                0x8582
#define GL_SOURCE0_ALPHA_ARB              0x8588
#define GL_SOURCE1_ALPHA_ARB              0x8589
#define GL_SOURCE2_ALPHA_ARB              0x858A
#define GL_OPERAND0_RGB_ARB               0x8590
#define GL_OPERAND1_RGB_ARB               0x8591
#define GL_OPERAND2_RGB_ARB               0x8592
#define GL_OPERAND0_ALPHA_ARB             0x8598
#define GL_OPERAND1_ALPHA_ARB             0x8599
#define GL_OPERAND2_ALPHA_ARB             0x859A
#define GL_RGB_SCALE_ARB                  0x8573
#define GL_ADD_SIGNED_ARB                 0x8574
#define GL_INTERPOLATE_ARB                0x8575
#define GL_SUBTRACT_ARB                   0x84E7
#define GL_CONSTANT_ARB                   0x8576
#define GL_PRIMARY_COLOR_ARB              0x8577
#define GL_PREVIOUS_ARB                   0x8578
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_env_crossbar;
extern int QGLNumFunctions_GL_ARB_texture_env_crossbar;
#ifndef GL_ARB_texture_env_crossbar
#define GL_ARB_texture_env_crossbar
#define QGL_DEFINED_GL_ARB_texture_env_crossbar
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_env_dot3;
extern int QGLNumFunctions_GL_ARB_texture_env_dot3;
#ifndef GL_ARB_texture_env_dot3
#define GL_ARB_texture_env_dot3
#define QGL_DEFINED_GL_ARB_texture_env_dot3
#define GL_DOT3_RGB_ARB                   0x86AE
#define GL_DOT3_RGBA_ARB                  0x86AF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_mirrored_repeat;
extern int QGLNumFunctions_GL_ARB_texture_mirrored_repeat;
#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat
#define QGL_DEFINED_GL_ARB_texture_mirrored_repeat
#define GL_MIRRORED_REPEAT_ARB            0x8370
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_depth_texture;
extern int QGLNumFunctions_GL_ARB_depth_texture;
#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture
#define QGL_DEFINED_GL_ARB_depth_texture
#define GL_DEPTH_COMPONENT16_ARB          0x81A5
#define GL_DEPTH_COMPONENT24_ARB          0x81A6
#define GL_DEPTH_COMPONENT32_ARB          0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB         0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB         0x884B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_shadow;
extern int QGLNumFunctions_GL_ARB_shadow;
#ifndef GL_ARB_shadow
#define GL_ARB_shadow
#define QGL_DEFINED_GL_ARB_shadow
#define GL_TEXTURE_COMPARE_MODE_ARB       0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB       0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB       0x884E
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_shadow_ambient;
extern int QGLNumFunctions_GL_ARB_shadow_ambient;
#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient
#define QGL_DEFINED_GL_ARB_shadow_ambient
#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_window_pos;
extern int QGLNumFunctions_GL_ARB_window_pos;
#ifndef GL_ARB_window_pos
#define GL_ARB_window_pos
#define QGL_DEFINED_GL_ARB_window_pos

typedef void (APIENTRY* pfn_glWindowPos2dARB) (GLdouble, GLdouble);
extern pfn_glWindowPos2dARB QGLglWindowPos2dARB;
#define glWindowPos2dARB QGLglWindowPos2dARB

typedef void (APIENTRY* pfn_glWindowPos2dvARB) (const GLdouble*);
extern pfn_glWindowPos2dvARB QGLglWindowPos2dvARB;
#define glWindowPos2dvARB QGLglWindowPos2dvARB

typedef void (APIENTRY* pfn_glWindowPos2fARB) (GLfloat, GLfloat);
extern pfn_glWindowPos2fARB QGLglWindowPos2fARB;
#define glWindowPos2fARB QGLglWindowPos2fARB

typedef void (APIENTRY* pfn_glWindowPos2fvARB) (const GLfloat*);
extern pfn_glWindowPos2fvARB QGLglWindowPos2fvARB;
#define glWindowPos2fvARB QGLglWindowPos2fvARB

typedef void (APIENTRY* pfn_glWindowPos2iARB) (GLint, GLint);
extern pfn_glWindowPos2iARB QGLglWindowPos2iARB;
#define glWindowPos2iARB QGLglWindowPos2iARB

typedef void (APIENTRY* pfn_glWindowPos2ivARB) (const GLint*);
extern pfn_glWindowPos2ivARB QGLglWindowPos2ivARB;
#define glWindowPos2ivARB QGLglWindowPos2ivARB

typedef void (APIENTRY* pfn_glWindowPos2sARB) (GLshort, GLshort);
extern pfn_glWindowPos2sARB QGLglWindowPos2sARB;
#define glWindowPos2sARB QGLglWindowPos2sARB

typedef void (APIENTRY* pfn_glWindowPos2svARB) (const GLshort*);
extern pfn_glWindowPos2svARB QGLglWindowPos2svARB;
#define glWindowPos2svARB QGLglWindowPos2svARB

typedef void (APIENTRY* pfn_glWindowPos3dARB) (GLdouble, GLdouble, GLdouble);
extern pfn_glWindowPos3dARB QGLglWindowPos3dARB;
#define glWindowPos3dARB QGLglWindowPos3dARB

typedef void (APIENTRY* pfn_glWindowPos3dvARB) (const GLdouble*);
extern pfn_glWindowPos3dvARB QGLglWindowPos3dvARB;
#define glWindowPos3dvARB QGLglWindowPos3dvARB

typedef void (APIENTRY* pfn_glWindowPos3fARB) (GLfloat, GLfloat, GLfloat);
extern pfn_glWindowPos3fARB QGLglWindowPos3fARB;
#define glWindowPos3fARB QGLglWindowPos3fARB

typedef void (APIENTRY* pfn_glWindowPos3fvARB) (const GLfloat*);
extern pfn_glWindowPos3fvARB QGLglWindowPos3fvARB;
#define glWindowPos3fvARB QGLglWindowPos3fvARB

typedef void (APIENTRY* pfn_glWindowPos3iARB) (GLint, GLint, GLint);
extern pfn_glWindowPos3iARB QGLglWindowPos3iARB;
#define glWindowPos3iARB QGLglWindowPos3iARB

typedef void (APIENTRY* pfn_glWindowPos3ivARB) (const GLint*);
extern pfn_glWindowPos3ivARB QGLglWindowPos3ivARB;
#define glWindowPos3ivARB QGLglWindowPos3ivARB

typedef void (APIENTRY* pfn_glWindowPos3sARB) (GLshort, GLshort, GLshort);
extern pfn_glWindowPos3sARB QGLglWindowPos3sARB;
#define glWindowPos3sARB QGLglWindowPos3sARB

typedef void (APIENTRY* pfn_glWindowPos3svARB) (const GLshort*);
extern pfn_glWindowPos3svARB QGLglWindowPos3svARB;
#define glWindowPos3svARB QGLglWindowPos3svARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_vertex_program;
extern int QGLNumFunctions_GL_ARB_vertex_program;
#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program
#define QGL_DEFINED_GL_ARB_vertex_program
#define GL_COLOR_SUM_ARB                            0x8458
#define GL_VERTEX_PROGRAM_ARB                       0x8620
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB          0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB             0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB           0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB             0x8625
#define GL_CURRENT_VERTEX_ATTRIB_ARB                0x8626
#define GL_PROGRAM_LENGTH_ARB                       0x8627
#define GL_PROGRAM_STRING_ARB                       0x8628
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB       0x862E
#define GL_MAX_PROGRAM_MATRICES_ARB                 0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB           0x8640
#define GL_CURRENT_MATRIX_ARB                       0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB            0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB              0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB          0x8645
#define GL_PROGRAM_ERROR_POSITION_ARB               0x864B
#define GL_PROGRAM_BINDING_ARB                      0x8677
#define GL_MAX_VERTEX_ATTRIBS_ARB                   0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB       0x886A
#define GL_PROGRAM_ERROR_STRING_ARB                 0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB                 0x8875
#define GL_PROGRAM_FORMAT_ARB                       0x8876
#define GL_PROGRAM_INSTRUCTIONS_ARB                 0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB             0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB          0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB      0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB                  0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB              0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB           0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB       0x88A7
#define GL_PROGRAM_PARAMETERS_ARB                   0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB               0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB            0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB        0x88AB
#define GL_PROGRAM_ATTRIBS_ARB                      0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB                  0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB               0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB           0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB            0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB        0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB     0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB         0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB           0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB          0x88B6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB             0x88B7
#define GL_MATRIX0_ARB                              0x88C0
#define GL_MATRIX1_ARB                              0x88C1
#define GL_MATRIX2_ARB                              0x88C2
#define GL_MATRIX3_ARB                              0x88C3
#define GL_MATRIX4_ARB                              0x88C4
#define GL_MATRIX5_ARB                              0x88C5
#define GL_MATRIX6_ARB                              0x88C6
#define GL_MATRIX7_ARB                              0x88C7
#define GL_MATRIX8_ARB                              0x88C8
#define GL_MATRIX9_ARB                              0x88C9
#define GL_MATRIX10_ARB                             0x88CA
#define GL_MATRIX11_ARB                             0x88CB
#define GL_MATRIX12_ARB                             0x88CC
#define GL_MATRIX13_ARB                             0x88CD
#define GL_MATRIX14_ARB                             0x88CE
#define GL_MATRIX15_ARB                             0x88CF
#define GL_MATRIX16_ARB                             0x88D0
#define GL_MATRIX17_ARB                             0x88D1
#define GL_MATRIX18_ARB                             0x88D2
#define GL_MATRIX19_ARB                             0x88D3
#define GL_MATRIX20_ARB                             0x88D4
#define GL_MATRIX21_ARB                             0x88D5
#define GL_MATRIX22_ARB                             0x88D6
#define GL_MATRIX23_ARB                             0x88D7
#define GL_MATRIX24_ARB                             0x88D8
#define GL_MATRIX25_ARB                             0x88D9
#define GL_MATRIX26_ARB                             0x88DA
#define GL_MATRIX27_ARB                             0x88DB
#define GL_MATRIX28_ARB                             0x88DC
#define GL_MATRIX29_ARB                             0x88DD
#define GL_MATRIX30_ARB                             0x88DE
#define GL_MATRIX31_ARB                             0x88DF

typedef void (APIENTRY* pfn_glVertexAttrib1dARB) (GLuint, GLdouble);
extern pfn_glVertexAttrib1dARB QGLglVertexAttrib1dARB;
#define glVertexAttrib1dARB QGLglVertexAttrib1dARB

typedef void (APIENTRY* pfn_glVertexAttrib1dvARB) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib1dvARB QGLglVertexAttrib1dvARB;
#define glVertexAttrib1dvARB QGLglVertexAttrib1dvARB

typedef void (APIENTRY* pfn_glVertexAttrib1fARB) (GLuint, GLfloat);
extern pfn_glVertexAttrib1fARB QGLglVertexAttrib1fARB;
#define glVertexAttrib1fARB QGLglVertexAttrib1fARB

typedef void (APIENTRY* pfn_glVertexAttrib1fvARB) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib1fvARB QGLglVertexAttrib1fvARB;
#define glVertexAttrib1fvARB QGLglVertexAttrib1fvARB

typedef void (APIENTRY* pfn_glVertexAttrib1sARB) (GLuint, GLshort);
extern pfn_glVertexAttrib1sARB QGLglVertexAttrib1sARB;
#define glVertexAttrib1sARB QGLglVertexAttrib1sARB

typedef void (APIENTRY* pfn_glVertexAttrib1svARB) (GLuint, const GLshort*);
extern pfn_glVertexAttrib1svARB QGLglVertexAttrib1svARB;
#define glVertexAttrib1svARB QGLglVertexAttrib1svARB

typedef void (APIENTRY* pfn_glVertexAttrib2dARB) (GLuint, GLdouble, GLdouble);
extern pfn_glVertexAttrib2dARB QGLglVertexAttrib2dARB;
#define glVertexAttrib2dARB QGLglVertexAttrib2dARB

typedef void (APIENTRY* pfn_glVertexAttrib2dvARB) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib2dvARB QGLglVertexAttrib2dvARB;
#define glVertexAttrib2dvARB QGLglVertexAttrib2dvARB

typedef void (APIENTRY* pfn_glVertexAttrib2fARB) (GLuint, GLfloat, GLfloat);
extern pfn_glVertexAttrib2fARB QGLglVertexAttrib2fARB;
#define glVertexAttrib2fARB QGLglVertexAttrib2fARB

typedef void (APIENTRY* pfn_glVertexAttrib2fvARB) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib2fvARB QGLglVertexAttrib2fvARB;
#define glVertexAttrib2fvARB QGLglVertexAttrib2fvARB

typedef void (APIENTRY* pfn_glVertexAttrib2sARB) (GLuint, GLshort, GLshort);
extern pfn_glVertexAttrib2sARB QGLglVertexAttrib2sARB;
#define glVertexAttrib2sARB QGLglVertexAttrib2sARB

typedef void (APIENTRY* pfn_glVertexAttrib2svARB) (GLuint, const GLshort*);
extern pfn_glVertexAttrib2svARB QGLglVertexAttrib2svARB;
#define glVertexAttrib2svARB QGLglVertexAttrib2svARB

typedef void (APIENTRY* pfn_glVertexAttrib3dARB) (GLuint, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib3dARB QGLglVertexAttrib3dARB;
#define glVertexAttrib3dARB QGLglVertexAttrib3dARB

typedef void (APIENTRY* pfn_glVertexAttrib3dvARB) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib3dvARB QGLglVertexAttrib3dvARB;
#define glVertexAttrib3dvARB QGLglVertexAttrib3dvARB

typedef void (APIENTRY* pfn_glVertexAttrib3fARB) (GLuint, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib3fARB QGLglVertexAttrib3fARB;
#define glVertexAttrib3fARB QGLglVertexAttrib3fARB

typedef void (APIENTRY* pfn_glVertexAttrib3fvARB) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib3fvARB QGLglVertexAttrib3fvARB;
#define glVertexAttrib3fvARB QGLglVertexAttrib3fvARB

typedef void (APIENTRY* pfn_glVertexAttrib3sARB) (GLuint, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib3sARB QGLglVertexAttrib3sARB;
#define glVertexAttrib3sARB QGLglVertexAttrib3sARB

typedef void (APIENTRY* pfn_glVertexAttrib3svARB) (GLuint, const GLshort*);
extern pfn_glVertexAttrib3svARB QGLglVertexAttrib3svARB;
#define glVertexAttrib3svARB QGLglVertexAttrib3svARB

typedef void (APIENTRY* pfn_glVertexAttrib4NbvARB) (GLuint, const GLbyte*);
extern pfn_glVertexAttrib4NbvARB QGLglVertexAttrib4NbvARB;
#define glVertexAttrib4NbvARB QGLglVertexAttrib4NbvARB

typedef void (APIENTRY* pfn_glVertexAttrib4NivARB) (GLuint, const GLint*);
extern pfn_glVertexAttrib4NivARB QGLglVertexAttrib4NivARB;
#define glVertexAttrib4NivARB QGLglVertexAttrib4NivARB

typedef void (APIENTRY* pfn_glVertexAttrib4NsvARB) (GLuint, const GLshort*);
extern pfn_glVertexAttrib4NsvARB QGLglVertexAttrib4NsvARB;
#define glVertexAttrib4NsvARB QGLglVertexAttrib4NsvARB

typedef void (APIENTRY* pfn_glVertexAttrib4NubARB) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
extern pfn_glVertexAttrib4NubARB QGLglVertexAttrib4NubARB;
#define glVertexAttrib4NubARB QGLglVertexAttrib4NubARB

typedef void (APIENTRY* pfn_glVertexAttrib4NubvARB) (GLuint, const GLubyte*);
extern pfn_glVertexAttrib4NubvARB QGLglVertexAttrib4NubvARB;
#define glVertexAttrib4NubvARB QGLglVertexAttrib4NubvARB

typedef void (APIENTRY* pfn_glVertexAttrib4NuivARB) (GLuint, const GLuint*);
extern pfn_glVertexAttrib4NuivARB QGLglVertexAttrib4NuivARB;
#define glVertexAttrib4NuivARB QGLglVertexAttrib4NuivARB

typedef void (APIENTRY* pfn_glVertexAttrib4NusvARB) (GLuint, const GLushort*);
extern pfn_glVertexAttrib4NusvARB QGLglVertexAttrib4NusvARB;
#define glVertexAttrib4NusvARB QGLglVertexAttrib4NusvARB

typedef void (APIENTRY* pfn_glVertexAttrib4bvARB) (GLuint, const GLbyte*);
extern pfn_glVertexAttrib4bvARB QGLglVertexAttrib4bvARB;
#define glVertexAttrib4bvARB QGLglVertexAttrib4bvARB

typedef void (APIENTRY* pfn_glVertexAttrib4dARB) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib4dARB QGLglVertexAttrib4dARB;
#define glVertexAttrib4dARB QGLglVertexAttrib4dARB

typedef void (APIENTRY* pfn_glVertexAttrib4dvARB) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib4dvARB QGLglVertexAttrib4dvARB;
#define glVertexAttrib4dvARB QGLglVertexAttrib4dvARB

typedef void (APIENTRY* pfn_glVertexAttrib4fARB) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib4fARB QGLglVertexAttrib4fARB;
#define glVertexAttrib4fARB QGLglVertexAttrib4fARB

typedef void (APIENTRY* pfn_glVertexAttrib4fvARB) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib4fvARB QGLglVertexAttrib4fvARB;
#define glVertexAttrib4fvARB QGLglVertexAttrib4fvARB

typedef void (APIENTRY* pfn_glVertexAttrib4ivARB) (GLuint, const GLint*);
extern pfn_glVertexAttrib4ivARB QGLglVertexAttrib4ivARB;
#define glVertexAttrib4ivARB QGLglVertexAttrib4ivARB

typedef void (APIENTRY* pfn_glVertexAttrib4sARB) (GLuint, GLshort, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib4sARB QGLglVertexAttrib4sARB;
#define glVertexAttrib4sARB QGLglVertexAttrib4sARB

typedef void (APIENTRY* pfn_glVertexAttrib4svARB) (GLuint, const GLshort*);
extern pfn_glVertexAttrib4svARB QGLglVertexAttrib4svARB;
#define glVertexAttrib4svARB QGLglVertexAttrib4svARB

typedef void (APIENTRY* pfn_glVertexAttrib4ubvARB) (GLuint, const GLubyte*);
extern pfn_glVertexAttrib4ubvARB QGLglVertexAttrib4ubvARB;
#define glVertexAttrib4ubvARB QGLglVertexAttrib4ubvARB

typedef void (APIENTRY* pfn_glVertexAttrib4uivARB) (GLuint, const GLuint*);
extern pfn_glVertexAttrib4uivARB QGLglVertexAttrib4uivARB;
#define glVertexAttrib4uivARB QGLglVertexAttrib4uivARB

typedef void (APIENTRY* pfn_glVertexAttrib4usvARB) (GLuint, const GLushort*);
extern pfn_glVertexAttrib4usvARB QGLglVertexAttrib4usvARB;
#define glVertexAttrib4usvARB QGLglVertexAttrib4usvARB

typedef void (APIENTRY* pfn_glVertexAttribPointerARB) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern pfn_glVertexAttribPointerARB QGLglVertexAttribPointerARB;
#define glVertexAttribPointerARB QGLglVertexAttribPointerARB

typedef void (APIENTRY* pfn_glEnableVertexAttribArrayARB) (GLuint);
extern pfn_glEnableVertexAttribArrayARB QGLglEnableVertexAttribArrayARB;
#define glEnableVertexAttribArrayARB QGLglEnableVertexAttribArrayARB

typedef void (APIENTRY* pfn_glDisableVertexAttribArrayARB) (GLuint);
extern pfn_glDisableVertexAttribArrayARB QGLglDisableVertexAttribArrayARB;
#define glDisableVertexAttribArrayARB QGLglDisableVertexAttribArrayARB

typedef void (APIENTRY* pfn_glProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid*);
extern pfn_glProgramStringARB QGLglProgramStringARB;
#define glProgramStringARB QGLglProgramStringARB

typedef void (APIENTRY* pfn_glBindProgramARB) (GLenum, GLuint);
extern pfn_glBindProgramARB QGLglBindProgramARB;
#define glBindProgramARB QGLglBindProgramARB

typedef void (APIENTRY* pfn_glDeleteProgramsARB) (GLsizei, const GLuint*);
extern pfn_glDeleteProgramsARB QGLglDeleteProgramsARB;
#define glDeleteProgramsARB QGLglDeleteProgramsARB

typedef void (APIENTRY* pfn_glGenProgramsARB) (GLsizei, GLuint*);
extern pfn_glGenProgramsARB QGLglGenProgramsARB;
#define glGenProgramsARB QGLglGenProgramsARB

typedef void (APIENTRY* pfn_glProgramEnvParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glProgramEnvParameter4dARB QGLglProgramEnvParameter4dARB;
#define glProgramEnvParameter4dARB QGLglProgramEnvParameter4dARB

typedef void (APIENTRY* pfn_glProgramEnvParameter4dvARB) (GLenum, GLuint, const GLdouble*);
extern pfn_glProgramEnvParameter4dvARB QGLglProgramEnvParameter4dvARB;
#define glProgramEnvParameter4dvARB QGLglProgramEnvParameter4dvARB

typedef void (APIENTRY* pfn_glProgramEnvParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glProgramEnvParameter4fARB QGLglProgramEnvParameter4fARB;
#define glProgramEnvParameter4fARB QGLglProgramEnvParameter4fARB

typedef void (APIENTRY* pfn_glProgramEnvParameter4fvARB) (GLenum, GLuint, const GLfloat*);
extern pfn_glProgramEnvParameter4fvARB QGLglProgramEnvParameter4fvARB;
#define glProgramEnvParameter4fvARB QGLglProgramEnvParameter4fvARB

typedef void (APIENTRY* pfn_glProgramLocalParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glProgramLocalParameter4dARB QGLglProgramLocalParameter4dARB;
#define glProgramLocalParameter4dARB QGLglProgramLocalParameter4dARB

typedef void (APIENTRY* pfn_glProgramLocalParameter4dvARB) (GLenum, GLuint, const GLdouble*);
extern pfn_glProgramLocalParameter4dvARB QGLglProgramLocalParameter4dvARB;
#define glProgramLocalParameter4dvARB QGLglProgramLocalParameter4dvARB

typedef void (APIENTRY* pfn_glProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glProgramLocalParameter4fARB QGLglProgramLocalParameter4fARB;
#define glProgramLocalParameter4fARB QGLglProgramLocalParameter4fARB

typedef void (APIENTRY* pfn_glProgramLocalParameter4fvARB) (GLenum, GLuint, const GLfloat*);
extern pfn_glProgramLocalParameter4fvARB QGLglProgramLocalParameter4fvARB;
#define glProgramLocalParameter4fvARB QGLglProgramLocalParameter4fvARB

typedef void (APIENTRY* pfn_glGetProgramEnvParameterdvARB) (GLenum, GLuint, GLdouble*);
extern pfn_glGetProgramEnvParameterdvARB QGLglGetProgramEnvParameterdvARB;
#define glGetProgramEnvParameterdvARB QGLglGetProgramEnvParameterdvARB

typedef void (APIENTRY* pfn_glGetProgramEnvParameterfvARB) (GLenum, GLuint, GLfloat*);
extern pfn_glGetProgramEnvParameterfvARB QGLglGetProgramEnvParameterfvARB;
#define glGetProgramEnvParameterfvARB QGLglGetProgramEnvParameterfvARB

typedef void (APIENTRY* pfn_glGetProgramLocalParameterdvARB) (GLenum, GLuint, GLdouble*);
extern pfn_glGetProgramLocalParameterdvARB QGLglGetProgramLocalParameterdvARB;
#define glGetProgramLocalParameterdvARB QGLglGetProgramLocalParameterdvARB

typedef void (APIENTRY* pfn_glGetProgramLocalParameterfvARB) (GLenum, GLuint, GLfloat*);
extern pfn_glGetProgramLocalParameterfvARB QGLglGetProgramLocalParameterfvARB;
#define glGetProgramLocalParameterfvARB QGLglGetProgramLocalParameterfvARB

typedef void (APIENTRY* pfn_glGetProgramivARB) (GLenum, GLenum, GLint*);
extern pfn_glGetProgramivARB QGLglGetProgramivARB;
#define glGetProgramivARB QGLglGetProgramivARB

typedef void (APIENTRY* pfn_glGetProgramStringARB) (GLenum, GLenum, GLvoid*);
extern pfn_glGetProgramStringARB QGLglGetProgramStringARB;
#define glGetProgramStringARB QGLglGetProgramStringARB

typedef void (APIENTRY* pfn_glGetVertexAttribdvARB) (GLuint, GLenum, GLdouble*);
extern pfn_glGetVertexAttribdvARB QGLglGetVertexAttribdvARB;
#define glGetVertexAttribdvARB QGLglGetVertexAttribdvARB

typedef void (APIENTRY* pfn_glGetVertexAttribfvARB) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVertexAttribfvARB QGLglGetVertexAttribfvARB;
#define glGetVertexAttribfvARB QGLglGetVertexAttribfvARB

typedef void (APIENTRY* pfn_glGetVertexAttribivARB) (GLuint, GLenum, GLint*);
extern pfn_glGetVertexAttribivARB QGLglGetVertexAttribivARB;
#define glGetVertexAttribivARB QGLglGetVertexAttribivARB

typedef void (APIENTRY* pfn_glGetVertexAttribPointervARB) (GLuint, GLenum, GLvoid**);
extern pfn_glGetVertexAttribPointervARB QGLglGetVertexAttribPointervARB;
#define glGetVertexAttribPointervARB QGLglGetVertexAttribPointervARB

typedef GLboolean (APIENTRY* pfn_glIsProgramARB) (GLuint);
extern pfn_glIsProgramARB QGLglIsProgramARB;
#define glIsProgramARB QGLglIsProgramARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_fragment_program;
extern int QGLNumFunctions_GL_ARB_fragment_program;
#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program
#define QGL_DEFINED_GL_ARB_fragment_program
#define GL_FRAGMENT_PROGRAM_ARB                     0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB             0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB             0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB             0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB      0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB      0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB      0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB         0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB         0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB         0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB  0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB  0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB  0x8810
#define GL_MAX_TEXTURE_COORDS_ARB                   0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB              0x8872
/* All ARB_fragment_program entry points are shared with ARB_vertex_program. */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_vertex_buffer_object;
extern int QGLNumFunctions_GL_ARB_vertex_buffer_object;
#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object
#define QGL_DEFINED_GL_ARB_vertex_buffer_object
#define GL_BUFFER_SIZE_ARB                          0x8764
#define GL_BUFFER_USAGE_ARB                         0x8765
#define GL_ARRAY_BUFFER_ARB                         0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB                 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB         0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB          0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB          0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB           0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB           0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB   0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB       0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB  0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB          0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB   0x889F
#define GL_READ_ONLY_ARB                            0x88B8
#define GL_WRITE_ONLY_ARB                           0x88B9
#define GL_READ_WRITE_ARB                           0x88BA
#define GL_BUFFER_ACCESS_ARB                        0x88BB
#define GL_BUFFER_MAPPED_ARB                        0x88BC
#define GL_BUFFER_MAP_POINTER_ARB                   0x88BD
#define GL_STREAM_DRAW_ARB                          0x88E0
#define GL_STREAM_READ_ARB                          0x88E1
#define GL_STREAM_COPY_ARB                          0x88E2
#define GL_STATIC_DRAW_ARB                          0x88E4
#define GL_STATIC_READ_ARB                          0x88E5
#define GL_STATIC_COPY_ARB                          0x88E6
#define GL_DYNAMIC_DRAW_ARB                         0x88E8
#define GL_DYNAMIC_READ_ARB                         0x88E9
#define GL_DYNAMIC_COPY_ARB                         0x88EA
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void (APIENTRY* pfn_glBindBufferARB) (GLenum, GLuint);
extern pfn_glBindBufferARB QGLglBindBufferARB;
#define glBindBufferARB QGLglBindBufferARB

typedef void (APIENTRY* pfn_glDeleteBuffersARB) (GLsizei, const GLuint*);
extern pfn_glDeleteBuffersARB QGLglDeleteBuffersARB;
#define glDeleteBuffersARB QGLglDeleteBuffersARB

typedef void (APIENTRY* pfn_glGenBuffersARB) (GLsizei, GLuint*);
extern pfn_glGenBuffersARB QGLglGenBuffersARB;
#define glGenBuffersARB QGLglGenBuffersARB

typedef GLboolean (APIENTRY* pfn_glIsBufferARB) (GLuint);
extern pfn_glIsBufferARB QGLglIsBufferARB;
#define glIsBufferARB QGLglIsBufferARB

typedef void (APIENTRY* pfn_glBufferDataARB) (GLenum, GLsizeiptrARB, const GLvoid*, GLenum);
extern pfn_glBufferDataARB QGLglBufferDataARB;
#define glBufferDataARB QGLglBufferDataARB

typedef void (APIENTRY* pfn_glBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid*);
extern pfn_glBufferSubDataARB QGLglBufferSubDataARB;
#define glBufferSubDataARB QGLglBufferSubDataARB

typedef void (APIENTRY* pfn_glGetBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, GLvoid*);
extern pfn_glGetBufferSubDataARB QGLglGetBufferSubDataARB;
#define glGetBufferSubDataARB QGLglGetBufferSubDataARB

typedef GLvoid* (APIENTRY* pfn_glMapBufferARB) (GLenum, GLenum);
extern pfn_glMapBufferARB QGLglMapBufferARB;
#define glMapBufferARB QGLglMapBufferARB

typedef GLboolean (APIENTRY* pfn_glUnmapBufferARB) (GLenum);
extern pfn_glUnmapBufferARB QGLglUnmapBufferARB;
#define glUnmapBufferARB QGLglUnmapBufferARB

typedef void (APIENTRY* pfn_glGetBufferParameterivARB) (GLenum, GLenum, GLint*);
extern pfn_glGetBufferParameterivARB QGLglGetBufferParameterivARB;
#define glGetBufferParameterivARB QGLglGetBufferParameterivARB

typedef void (APIENTRY* pfn_glGetBufferPointervARB) (GLenum, GLenum, GLvoid**);
extern pfn_glGetBufferPointervARB QGLglGetBufferPointervARB;
#define glGetBufferPointervARB QGLglGetBufferPointervARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_occlusion_query;
extern int QGLNumFunctions_GL_ARB_occlusion_query;
#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query
#define QGL_DEFINED_GL_ARB_occlusion_query
#define GL_QUERY_COUNTER_BITS_ARB         0x8864
#define GL_CURRENT_QUERY_ARB              0x8865
#define GL_QUERY_RESULT_ARB               0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB     0x8867
#define GL_SAMPLES_PASSED_ARB             0x8914

typedef void (APIENTRY* pfn_glGenQueriesARB) (GLsizei, GLuint*);
extern pfn_glGenQueriesARB QGLglGenQueriesARB;
#define glGenQueriesARB QGLglGenQueriesARB

typedef void (APIENTRY* pfn_glDeleteQueriesARB) (GLsizei, const GLuint*);
extern pfn_glDeleteQueriesARB QGLglDeleteQueriesARB;
#define glDeleteQueriesARB QGLglDeleteQueriesARB

typedef GLboolean (APIENTRY* pfn_glIsQueryARB) (GLuint);
extern pfn_glIsQueryARB QGLglIsQueryARB;
#define glIsQueryARB QGLglIsQueryARB

typedef void (APIENTRY* pfn_glBeginQueryARB) (GLenum, GLuint);
extern pfn_glBeginQueryARB QGLglBeginQueryARB;
#define glBeginQueryARB QGLglBeginQueryARB

typedef void (APIENTRY* pfn_glEndQueryARB) (GLenum);
extern pfn_glEndQueryARB QGLglEndQueryARB;
#define glEndQueryARB QGLglEndQueryARB

typedef void (APIENTRY* pfn_glGetQueryivARB) (GLenum, GLenum, GLint*);
extern pfn_glGetQueryivARB QGLglGetQueryivARB;
#define glGetQueryivARB QGLglGetQueryivARB

typedef void (APIENTRY* pfn_glGetQueryObjectivARB) (GLuint, GLenum, GLint*);
extern pfn_glGetQueryObjectivARB QGLglGetQueryObjectivARB;
#define glGetQueryObjectivARB QGLglGetQueryObjectivARB

typedef void (APIENTRY* pfn_glGetQueryObjectuivARB) (GLuint, GLenum, GLuint*);
extern pfn_glGetQueryObjectuivARB QGLglGetQueryObjectuivARB;
#define glGetQueryObjectuivARB QGLglGetQueryObjectuivARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_shader_objects;
extern int QGLNumFunctions_GL_ARB_shader_objects;
#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects
#define QGL_DEFINED_GL_ARB_shader_objects
#define GL_PROGRAM_OBJECT_ARB                   0x8B40
#define GL_SHADER_OBJECT_ARB                    0x8B48
#define GL_OBJECT_TYPE_ARB                      0x8B4E
#define GL_OBJECT_SUBTYPE_ARB                   0x8B4F
#define GL_FLOAT_VEC2_ARB                       0x8B50
#define GL_FLOAT_VEC3_ARB                       0x8B51
#define GL_FLOAT_VEC4_ARB                       0x8B52
#define GL_INT_VEC2_ARB                         0x8B53
#define GL_INT_VEC3_ARB                         0x8B54
#define GL_INT_VEC4_ARB                         0x8B55
#define GL_BOOL_ARB                             0x8B56
#define GL_BOOL_VEC2_ARB                        0x8B57
#define GL_BOOL_VEC3_ARB                        0x8B58
#define GL_BOOL_VEC4_ARB                        0x8B59
#define GL_FLOAT_MAT2_ARB                       0x8B5A
#define GL_FLOAT_MAT3_ARB                       0x8B5B
#define GL_FLOAT_MAT4_ARB                       0x8B5C
#define GL_SAMPLER_1D_ARB                       0x8B5D
#define GL_SAMPLER_2D_ARB                       0x8B5E
#define GL_SAMPLER_3D_ARB                       0x8B5F
#define GL_SAMPLER_CUBE_ARB                     0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB                0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB                0x8B62
#define GL_SAMPLER_2D_RECT_ARB                  0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB           0x8B64
#define GL_OBJECT_DELETE_STATUS_ARB             0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB            0x8B81
#define GL_OBJECT_LINK_STATUS_ARB               0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB           0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB           0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB          0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB           0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB      0x8B88
typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void (APIENTRY* pfn_glDeleteObjectARB) (GLhandleARB);
extern pfn_glDeleteObjectARB QGLglDeleteObjectARB;
#define glDeleteObjectARB QGLglDeleteObjectARB

typedef GLhandleARB (APIENTRY* pfn_glGetHandleARB) (GLenum);
extern pfn_glGetHandleARB QGLglGetHandleARB;
#define glGetHandleARB QGLglGetHandleARB

typedef void (APIENTRY* pfn_glDetachObjectARB) (GLhandleARB, GLhandleARB);
extern pfn_glDetachObjectARB QGLglDetachObjectARB;
#define glDetachObjectARB QGLglDetachObjectARB

typedef GLhandleARB (APIENTRY* pfn_glCreateShaderObjectARB) (GLenum);
extern pfn_glCreateShaderObjectARB QGLglCreateShaderObjectARB;
#define glCreateShaderObjectARB QGLglCreateShaderObjectARB

typedef void (APIENTRY* pfn_glShaderSourceARB) (GLhandleARB, GLsizei, const GLcharARB**, const GLint*);
extern pfn_glShaderSourceARB QGLglShaderSourceARB;
#define glShaderSourceARB QGLglShaderSourceARB

typedef void (APIENTRY* pfn_glCompileShaderARB) (GLhandleARB);
extern pfn_glCompileShaderARB QGLglCompileShaderARB;
#define glCompileShaderARB QGLglCompileShaderARB

typedef GLhandleARB (APIENTRY* pfn_glCreateProgramObjectARB) (void);
extern pfn_glCreateProgramObjectARB QGLglCreateProgramObjectARB;
#define glCreateProgramObjectARB QGLglCreateProgramObjectARB

typedef void (APIENTRY* pfn_glAttachObjectARB) (GLhandleARB, GLhandleARB);
extern pfn_glAttachObjectARB QGLglAttachObjectARB;
#define glAttachObjectARB QGLglAttachObjectARB

typedef void (APIENTRY* pfn_glLinkProgramARB) (GLhandleARB);
extern pfn_glLinkProgramARB QGLglLinkProgramARB;
#define glLinkProgramARB QGLglLinkProgramARB

typedef void (APIENTRY* pfn_glUseProgramObjectARB) (GLhandleARB);
extern pfn_glUseProgramObjectARB QGLglUseProgramObjectARB;
#define glUseProgramObjectARB QGLglUseProgramObjectARB

typedef void (APIENTRY* pfn_glValidateProgramARB) (GLhandleARB);
extern pfn_glValidateProgramARB QGLglValidateProgramARB;
#define glValidateProgramARB QGLglValidateProgramARB

typedef void (APIENTRY* pfn_glUniform1fARB) (GLint, GLfloat);
extern pfn_glUniform1fARB QGLglUniform1fARB;
#define glUniform1fARB QGLglUniform1fARB

typedef void (APIENTRY* pfn_glUniform2fARB) (GLint, GLfloat, GLfloat);
extern pfn_glUniform2fARB QGLglUniform2fARB;
#define glUniform2fARB QGLglUniform2fARB

typedef void (APIENTRY* pfn_glUniform3fARB) (GLint, GLfloat, GLfloat, GLfloat);
extern pfn_glUniform3fARB QGLglUniform3fARB;
#define glUniform3fARB QGLglUniform3fARB

typedef void (APIENTRY* pfn_glUniform4fARB) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glUniform4fARB QGLglUniform4fARB;
#define glUniform4fARB QGLglUniform4fARB

typedef void (APIENTRY* pfn_glUniform1iARB) (GLint, GLint);
extern pfn_glUniform1iARB QGLglUniform1iARB;
#define glUniform1iARB QGLglUniform1iARB

typedef void (APIENTRY* pfn_glUniform2iARB) (GLint, GLint, GLint);
extern pfn_glUniform2iARB QGLglUniform2iARB;
#define glUniform2iARB QGLglUniform2iARB

typedef void (APIENTRY* pfn_glUniform3iARB) (GLint, GLint, GLint, GLint);
extern pfn_glUniform3iARB QGLglUniform3iARB;
#define glUniform3iARB QGLglUniform3iARB

typedef void (APIENTRY* pfn_glUniform4iARB) (GLint, GLint, GLint, GLint, GLint);
extern pfn_glUniform4iARB QGLglUniform4iARB;
#define glUniform4iARB QGLglUniform4iARB

typedef void (APIENTRY* pfn_glUniform1fvARB) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform1fvARB QGLglUniform1fvARB;
#define glUniform1fvARB QGLglUniform1fvARB

typedef void (APIENTRY* pfn_glUniform2fvARB) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform2fvARB QGLglUniform2fvARB;
#define glUniform2fvARB QGLglUniform2fvARB

typedef void (APIENTRY* pfn_glUniform3fvARB) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform3fvARB QGLglUniform3fvARB;
#define glUniform3fvARB QGLglUniform3fvARB

typedef void (APIENTRY* pfn_glUniform4fvARB) (GLint, GLsizei, const GLfloat*);
extern pfn_glUniform4fvARB QGLglUniform4fvARB;
#define glUniform4fvARB QGLglUniform4fvARB

typedef void (APIENTRY* pfn_glUniform1ivARB) (GLint, GLsizei, const GLint*);
extern pfn_glUniform1ivARB QGLglUniform1ivARB;
#define glUniform1ivARB QGLglUniform1ivARB

typedef void (APIENTRY* pfn_glUniform2ivARB) (GLint, GLsizei, const GLint*);
extern pfn_glUniform2ivARB QGLglUniform2ivARB;
#define glUniform2ivARB QGLglUniform2ivARB

typedef void (APIENTRY* pfn_glUniform3ivARB) (GLint, GLsizei, const GLint*);
extern pfn_glUniform3ivARB QGLglUniform3ivARB;
#define glUniform3ivARB QGLglUniform3ivARB

typedef void (APIENTRY* pfn_glUniform4ivARB) (GLint, GLsizei, const GLint*);
extern pfn_glUniform4ivARB QGLglUniform4ivARB;
#define glUniform4ivARB QGLglUniform4ivARB

typedef void (APIENTRY* pfn_glUniformMatrix2fvARB) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix2fvARB QGLglUniformMatrix2fvARB;
#define glUniformMatrix2fvARB QGLglUniformMatrix2fvARB

typedef void (APIENTRY* pfn_glUniformMatrix3fvARB) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix3fvARB QGLglUniformMatrix3fvARB;
#define glUniformMatrix3fvARB QGLglUniformMatrix3fvARB

typedef void (APIENTRY* pfn_glUniformMatrix4fvARB) (GLint, GLsizei, GLboolean, const GLfloat*);
extern pfn_glUniformMatrix4fvARB QGLglUniformMatrix4fvARB;
#define glUniformMatrix4fvARB QGLglUniformMatrix4fvARB

typedef void (APIENTRY* pfn_glGetObjectParameterfvARB) (GLhandleARB, GLenum, GLfloat*);
extern pfn_glGetObjectParameterfvARB QGLglGetObjectParameterfvARB;
#define glGetObjectParameterfvARB QGLglGetObjectParameterfvARB

typedef void (APIENTRY* pfn_glGetObjectParameterivARB) (GLhandleARB, GLenum, GLint*);
extern pfn_glGetObjectParameterivARB QGLglGetObjectParameterivARB;
#define glGetObjectParameterivARB QGLglGetObjectParameterivARB

typedef void (APIENTRY* pfn_glGetInfoLogARB) (GLhandleARB, GLsizei, GLsizei*, GLcharARB*);
extern pfn_glGetInfoLogARB QGLglGetInfoLogARB;
#define glGetInfoLogARB QGLglGetInfoLogARB

typedef void (APIENTRY* pfn_glGetAttachedObjectsARB) (GLhandleARB, GLsizei, GLsizei*, GLhandleARB*);
extern pfn_glGetAttachedObjectsARB QGLglGetAttachedObjectsARB;
#define glGetAttachedObjectsARB QGLglGetAttachedObjectsARB

typedef GLint (APIENTRY* pfn_glGetUniformLocationARB) (GLhandleARB, const GLcharARB*);
extern pfn_glGetUniformLocationARB QGLglGetUniformLocationARB;
#define glGetUniformLocationARB QGLglGetUniformLocationARB

typedef void (APIENTRY* pfn_glGetActiveUniformARB) (GLhandleARB, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLcharARB*);
extern pfn_glGetActiveUniformARB QGLglGetActiveUniformARB;
#define glGetActiveUniformARB QGLglGetActiveUniformARB

typedef void (APIENTRY* pfn_glGetUniformfvARB) (GLhandleARB, GLint, GLfloat*);
extern pfn_glGetUniformfvARB QGLglGetUniformfvARB;
#define glGetUniformfvARB QGLglGetUniformfvARB

typedef void (APIENTRY* pfn_glGetUniformivARB) (GLhandleARB, GLint, GLint*);
extern pfn_glGetUniformivARB QGLglGetUniformivARB;
#define glGetUniformivARB QGLglGetUniformivARB

typedef void (APIENTRY* pfn_glGetShaderSourceARB) (GLhandleARB, GLsizei, GLsizei*, GLcharARB*);
extern pfn_glGetShaderSourceARB QGLglGetShaderSourceARB;
#define glGetShaderSourceARB QGLglGetShaderSourceARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_vertex_shader;
extern int QGLNumFunctions_GL_ARB_vertex_shader;
#ifndef GL_ARB_vertex_shader
#define GL_ARB_vertex_shader
#define QGL_DEFINED_GL_ARB_vertex_shader
#define GL_VERTEX_SHADER_ARB                        0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB        0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB                   0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB       0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB     0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB             0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB   0x8B8A

typedef void (APIENTRY* pfn_glBindAttribLocationARB) (GLhandleARB, GLuint, const GLcharARB*);
extern pfn_glBindAttribLocationARB QGLglBindAttribLocationARB;
#define glBindAttribLocationARB QGLglBindAttribLocationARB

typedef void (APIENTRY* pfn_glGetActiveAttribARB) (GLhandleARB, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLcharARB*);
extern pfn_glGetActiveAttribARB QGLglGetActiveAttribARB;
#define glGetActiveAttribARB QGLglGetActiveAttribARB

typedef GLint (APIENTRY* pfn_glGetAttribLocationARB) (GLhandleARB, const GLcharARB*);
extern pfn_glGetAttribLocationARB QGLglGetAttribLocationARB;
#define glGetAttribLocationARB QGLglGetAttribLocationARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_fragment_shader;
extern int QGLNumFunctions_GL_ARB_fragment_shader;
#ifndef GL_ARB_fragment_shader
#define GL_ARB_fragment_shader
#define QGL_DEFINED_GL_ARB_fragment_shader
#define GL_FRAGMENT_SHADER_ARB                  0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB  0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB  0x8B8B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_shading_language_100;
extern int QGLNumFunctions_GL_ARB_shading_language_100;
#ifndef GL_ARB_shading_language_100
#define GL_ARB_shading_language_100
#define QGL_DEFINED_GL_ARB_shading_language_100
#define GL_SHADING_LANGUAGE_VERSION_ARB   0x8B8C
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_non_power_of_two;
extern int QGLNumFunctions_GL_ARB_texture_non_power_of_two;
#ifndef GL_ARB_texture_non_power_of_two
#define GL_ARB_texture_non_power_of_two
#define QGL_DEFINED_GL_ARB_texture_non_power_of_two
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_point_sprite;
extern int QGLNumFunctions_GL_ARB_point_sprite;
#ifndef GL_ARB_point_sprite
#define GL_ARB_point_sprite
#define QGL_DEFINED_GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_fragment_program_shadow;
extern int QGLNumFunctions_GL_ARB_fragment_program_shadow;
#ifndef GL_ARB_fragment_program_shadow
#define GL_ARB_fragment_program_shadow
#define QGL_DEFINED_GL_ARB_fragment_program_shadow
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_draw_buffers;
extern int QGLNumFunctions_GL_ARB_draw_buffers;
#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers
#define QGL_DEFINED_GL_ARB_draw_buffers
#define GL_MAX_DRAW_BUFFERS_ARB           0x8824
#define GL_DRAW_BUFFER0_ARB               0x8825
#define GL_DRAW_BUFFER1_ARB               0x8826
#define GL_DRAW_BUFFER2_ARB               0x8827
#define GL_DRAW_BUFFER3_ARB               0x8828
#define GL_DRAW_BUFFER4_ARB               0x8829
#define GL_DRAW_BUFFER5_ARB               0x882A
#define GL_DRAW_BUFFER6_ARB               0x882B
#define GL_DRAW_BUFFER7_ARB               0x882C
#define GL_DRAW_BUFFER8_ARB               0x882D
#define GL_DRAW_BUFFER9_ARB               0x882E
#define GL_DRAW_BUFFER10_ARB              0x882F
#define GL_DRAW_BUFFER11_ARB              0x8830
#define GL_DRAW_BUFFER12_ARB              0x8831
#define GL_DRAW_BUFFER13_ARB              0x8832
#define GL_DRAW_BUFFER14_ARB              0x8833
#define GL_DRAW_BUFFER15_ARB              0x8834

typedef void (APIENTRY* pfn_glDrawBuffersARB) (GLsizei, const GLenum*);
extern pfn_glDrawBuffersARB QGLglDrawBuffersARB;
#define glDrawBuffersARB QGLglDrawBuffersARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_rectangle;
extern int QGLNumFunctions_GL_ARB_texture_rectangle;
#ifndef GL_ARB_texture_rectangle
#define GL_ARB_texture_rectangle
#define QGL_DEFINED_GL_ARB_texture_rectangle
#define GL_TEXTURE_RECTANGLE_ARB          0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB  0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB    0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_color_buffer_float;
extern int QGLNumFunctions_GL_ARB_color_buffer_float;
#ifndef GL_ARB_color_buffer_float
#define GL_ARB_color_buffer_float
#define QGL_DEFINED_GL_ARB_color_buffer_float
#define GL_RGBA_FLOAT_MODE_ARB            0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB         0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB       0x891B
#define GL_CLAMP_READ_COLOR_ARB           0x891C
#define GL_FIXED_ONLY_ARB                 0x891D

typedef void (APIENTRY* pfn_glClampColorARB) (GLenum, GLenum);
extern pfn_glClampColorARB QGLglClampColorARB;
#define glClampColorARB QGLglClampColorARB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_half_float_pixel;
extern int QGLNumFunctions_GL_ARB_half_float_pixel;
#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel
#define QGL_DEFINED_GL_ARB_half_float_pixel
#define GL_HALF_FLOAT_ARB                 0x140B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_texture_float;
extern int QGLNumFunctions_GL_ARB_texture_float;
#ifndef GL_ARB_texture_float
#define GL_ARB_texture_float
#define QGL_DEFINED_GL_ARB_texture_float
#define GL_TEXTURE_RED_TYPE_ARB           0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB         0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB          0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB         0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB     0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB     0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB         0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB        0x8C17
#define GL_RGBA32F_ARB                    0x8814
#define GL_RGB32F_ARB                     0x8815
#define GL_ALPHA32F_ARB                   0x8816
#define GL_INTENSITY32F_ARB               0x8817
#define GL_LUMINANCE32F_ARB               0x8818
#define GL_LUMINANCE_ALPHA32F_ARB         0x8819
#define GL_RGBA16F_ARB                    0x881A
#define GL_RGB16F_ARB                     0x881B
#define GL_ALPHA16F_ARB                   0x881C
#define GL_INTENSITY16F_ARB               0x881D
#define GL_LUMINANCE16F_ARB               0x881E
#define GL_LUMINANCE_ALPHA16F_ARB         0x881F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ARB_pixel_buffer_object;
extern int QGLNumFunctions_GL_ARB_pixel_buffer_object;
#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object
#define QGL_DEFINED_GL_ARB_pixel_buffer_object
#define GL_PIXEL_PACK_BUFFER_ARB            0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB          0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB    0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB  0x88EF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_abgr;
extern int QGLNumFunctions_GL_EXT_abgr;
#ifndef GL_EXT_abgr
#define GL_EXT_abgr
#define QGL_DEFINED_GL_EXT_abgr
#define GL_ABGR_EXT                       0x8000
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_color;
extern int QGLNumFunctions_GL_EXT_blend_color;
#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color
#define QGL_DEFINED_GL_EXT_blend_color
#define GL_CONSTANT_COLOR_EXT             0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT   0x8002
#define GL_CONSTANT_ALPHA_EXT             0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT   0x8004
#define GL_BLEND_COLOR_EXT                0x8005

typedef void (APIENTRY* pfn_glBlendColorEXT) (GLclampf, GLclampf, GLclampf, GLclampf);
extern pfn_glBlendColorEXT QGLglBlendColorEXT;
#define glBlendColorEXT QGLglBlendColorEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_polygon_offset;
extern int QGLNumFunctions_GL_EXT_polygon_offset;
#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset
#define QGL_DEFINED_GL_EXT_polygon_offset
#define GL_POLYGON_OFFSET_EXT             0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT      0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT        0x8039

typedef void (APIENTRY* pfn_glPolygonOffsetEXT) (GLfloat, GLfloat);
extern pfn_glPolygonOffsetEXT QGLglPolygonOffsetEXT;
#define glPolygonOffsetEXT QGLglPolygonOffsetEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture;
extern int QGLNumFunctions_GL_EXT_texture;
#ifndef GL_EXT_texture
#define GL_EXT_texture
#define QGL_DEFINED_GL_EXT_texture
#define GL_ALPHA4_EXT                     0x803B
#define GL_ALPHA8_EXT                     0x803C
#define GL_ALPHA12_EXT                    0x803D
#define GL_ALPHA16_EXT                    0x803E
#define GL_LUMINANCE4_EXT                 0x803F
#define GL_LUMINANCE8_EXT                 0x8040
#define GL_LUMINANCE12_EXT                0x8041
#define GL_LUMINANCE16_EXT                0x8042
#define GL_LUMINANCE4_ALPHA4_EXT          0x8043
#define GL_LUMINANCE6_ALPHA2_EXT          0x8044
#define GL_LUMINANCE8_ALPHA8_EXT          0x8045
#define GL_LUMINANCE12_ALPHA4_EXT         0x8046
#define GL_LUMINANCE12_ALPHA12_EXT        0x8047
#define GL_LUMINANCE16_ALPHA16_EXT        0x8048
#define GL_INTENSITY_EXT                  0x8049
#define GL_INTENSITY4_EXT                 0x804A
#define GL_INTENSITY8_EXT                 0x804B
#define GL_INTENSITY12_EXT                0x804C
#define GL_INTENSITY16_EXT                0x804D
#define GL_RGB2_EXT                       0x804E
#define GL_RGB4_EXT                       0x804F
#define GL_RGB5_EXT                       0x8050
#define GL_RGB8_EXT                       0x8051
#define GL_RGB10_EXT                      0x8052
#define GL_RGB12_EXT                      0x8053
#define GL_RGB16_EXT                      0x8054
#define GL_RGBA2_EXT                      0x8055
#define GL_RGBA4_EXT                      0x8056
#define GL_RGB5_A1_EXT                    0x8057
#define GL_RGBA8_EXT                      0x8058
#define GL_RGB10_A2_EXT                   0x8059
#define GL_RGBA12_EXT                     0x805A
#define GL_RGBA16_EXT                     0x805B
#define GL_TEXTURE_RED_SIZE_EXT           0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT         0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT          0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT         0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT     0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT     0x8061
#define GL_REPLACE_EXT                    0x8062
#define GL_PROXY_TEXTURE_1D_EXT           0x8063
#define GL_PROXY_TEXTURE_2D_EXT           0x8064
#define GL_TEXTURE_TOO_LARGE_EXT          0x8065
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture3D;
extern int QGLNumFunctions_GL_EXT_texture3D;
#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D
#define QGL_DEFINED_GL_EXT_texture3D
#define GL_PACK_SKIP_IMAGES_EXT           0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT          0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT         0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT        0x806E
#define GL_TEXTURE_3D_EXT                 0x806F
#define GL_PROXY_TEXTURE_3D_EXT           0x8070
#define GL_TEXTURE_DEPTH_EXT              0x8071
#define GL_TEXTURE_WRAP_R_EXT             0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT        0x8073

typedef void (APIENTRY* pfn_glTexImage3DEXT) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
extern pfn_glTexImage3DEXT QGLglTexImage3DEXT;
#define glTexImage3DEXT QGLglTexImage3DEXT

typedef void (APIENTRY* pfn_glTexSubImage3DEXT) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glTexSubImage3DEXT QGLglTexSubImage3DEXT;
#define glTexSubImage3DEXT QGLglTexSubImage3DEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture_filter4;
extern int QGLNumFunctions_GL_SGIS_texture_filter4;
#ifndef GL_SGIS_texture_filter4
#define GL_SGIS_texture_filter4
#define QGL_DEFINED_GL_SGIS_texture_filter4
#define GL_FILTER4_SGIS                   0x8146
#define GL_TEXTURE_FILTER4_SIZE_SGIS      0x8147

typedef void (APIENTRY* pfn_glGetTexFilterFuncSGIS) (GLenum, GLenum, GLfloat*);
extern pfn_glGetTexFilterFuncSGIS QGLglGetTexFilterFuncSGIS;
#define glGetTexFilterFuncSGIS QGLglGetTexFilterFuncSGIS

typedef void (APIENTRY* pfn_glTexFilterFuncSGIS) (GLenum, GLenum, GLsizei, const GLfloat*);
extern pfn_glTexFilterFuncSGIS QGLglTexFilterFuncSGIS;
#define glTexFilterFuncSGIS QGLglTexFilterFuncSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_subtexture;
extern int QGLNumFunctions_GL_EXT_subtexture;
#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture
#define QGL_DEFINED_GL_EXT_subtexture

typedef void (APIENTRY* pfn_glTexSubImage1DEXT) (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glTexSubImage1DEXT QGLglTexSubImage1DEXT;
#define glTexSubImage1DEXT QGLglTexSubImage1DEXT

typedef void (APIENTRY* pfn_glTexSubImage2DEXT) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glTexSubImage2DEXT QGLglTexSubImage2DEXT;
#define glTexSubImage2DEXT QGLglTexSubImage2DEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_copy_texture;
extern int QGLNumFunctions_GL_EXT_copy_texture;
#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture
#define QGL_DEFINED_GL_EXT_copy_texture

typedef void (APIENTRY* pfn_glCopyTexImage1DEXT) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
extern pfn_glCopyTexImage1DEXT QGLglCopyTexImage1DEXT;
#define glCopyTexImage1DEXT QGLglCopyTexImage1DEXT

typedef void (APIENTRY* pfn_glCopyTexImage2DEXT) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
extern pfn_glCopyTexImage2DEXT QGLglCopyTexImage2DEXT;
#define glCopyTexImage2DEXT QGLglCopyTexImage2DEXT

typedef void (APIENTRY* pfn_glCopyTexSubImage1DEXT) (GLenum, GLint, GLint, GLint, GLint, GLsizei);
extern pfn_glCopyTexSubImage1DEXT QGLglCopyTexSubImage1DEXT;
#define glCopyTexSubImage1DEXT QGLglCopyTexSubImage1DEXT

typedef void (APIENTRY* pfn_glCopyTexSubImage2DEXT) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
extern pfn_glCopyTexSubImage2DEXT QGLglCopyTexSubImage2DEXT;
#define glCopyTexSubImage2DEXT QGLglCopyTexSubImage2DEXT

typedef void (APIENTRY* pfn_glCopyTexSubImage3DEXT) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
extern pfn_glCopyTexSubImage3DEXT QGLglCopyTexSubImage3DEXT;
#define glCopyTexSubImage3DEXT QGLglCopyTexSubImage3DEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_histogram;
extern int QGLNumFunctions_GL_EXT_histogram;
#ifndef GL_EXT_histogram
#define GL_EXT_histogram
#define QGL_DEFINED_GL_EXT_histogram
#define GL_HISTOGRAM_EXT                  0x8024
#define GL_PROXY_HISTOGRAM_EXT            0x8025
#define GL_HISTOGRAM_WIDTH_EXT            0x8026
#define GL_HISTOGRAM_FORMAT_EXT           0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT         0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT       0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT        0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT       0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT   0x802C
#define GL_HISTOGRAM_SINK_EXT             0x802D
#define GL_MINMAX_EXT                     0x802E
#define GL_MINMAX_FORMAT_EXT              0x802F
#define GL_MINMAX_SINK_EXT                0x8030
#define GL_TABLE_TOO_LARGE_EXT            0x8031

typedef void (APIENTRY* pfn_glGetHistogramEXT) (GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern pfn_glGetHistogramEXT QGLglGetHistogramEXT;
#define glGetHistogramEXT QGLglGetHistogramEXT

typedef void (APIENTRY* pfn_glGetHistogramParameterfvEXT) (GLenum, GLenum, GLfloat*);
extern pfn_glGetHistogramParameterfvEXT QGLglGetHistogramParameterfvEXT;
#define glGetHistogramParameterfvEXT QGLglGetHistogramParameterfvEXT

typedef void (APIENTRY* pfn_glGetHistogramParameterivEXT) (GLenum, GLenum, GLint*);
extern pfn_glGetHistogramParameterivEXT QGLglGetHistogramParameterivEXT;
#define glGetHistogramParameterivEXT QGLglGetHistogramParameterivEXT

typedef void (APIENTRY* pfn_glGetMinmaxEXT) (GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern pfn_glGetMinmaxEXT QGLglGetMinmaxEXT;
#define glGetMinmaxEXT QGLglGetMinmaxEXT

typedef void (APIENTRY* pfn_glGetMinmaxParameterfvEXT) (GLenum, GLenum, GLfloat*);
extern pfn_glGetMinmaxParameterfvEXT QGLglGetMinmaxParameterfvEXT;
#define glGetMinmaxParameterfvEXT QGLglGetMinmaxParameterfvEXT

typedef void (APIENTRY* pfn_glGetMinmaxParameterivEXT) (GLenum, GLenum, GLint*);
extern pfn_glGetMinmaxParameterivEXT QGLglGetMinmaxParameterivEXT;
#define glGetMinmaxParameterivEXT QGLglGetMinmaxParameterivEXT

typedef void (APIENTRY* pfn_glHistogramEXT) (GLenum, GLsizei, GLenum, GLboolean);
extern pfn_glHistogramEXT QGLglHistogramEXT;
#define glHistogramEXT QGLglHistogramEXT

typedef void (APIENTRY* pfn_glMinmaxEXT) (GLenum, GLenum, GLboolean);
extern pfn_glMinmaxEXT QGLglMinmaxEXT;
#define glMinmaxEXT QGLglMinmaxEXT

typedef void (APIENTRY* pfn_glResetHistogramEXT) (GLenum);
extern pfn_glResetHistogramEXT QGLglResetHistogramEXT;
#define glResetHistogramEXT QGLglResetHistogramEXT

typedef void (APIENTRY* pfn_glResetMinmaxEXT) (GLenum);
extern pfn_glResetMinmaxEXT QGLglResetMinmaxEXT;
#define glResetMinmaxEXT QGLglResetMinmaxEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_convolution;
extern int QGLNumFunctions_GL_EXT_convolution;
#ifndef GL_EXT_convolution
#define GL_EXT_convolution
#define QGL_DEFINED_GL_EXT_convolution
#define GL_CONVOLUTION_1D_EXT               0x8010
#define GL_CONVOLUTION_2D_EXT               0x8011
#define GL_SEPARABLE_2D_EXT                 0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT      0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT     0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT      0x8015
#define GL_REDUCE_EXT                       0x8016
#define GL_CONVOLUTION_FORMAT_EXT           0x8017
#define GL_CONVOLUTION_WIDTH_EXT            0x8018
#define GL_CONVOLUTION_HEIGHT_EXT           0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT        0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT       0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT   0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT  0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT    0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT  0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT   0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT  0x8023

typedef void (APIENTRY* pfn_glConvolutionFilter1DEXT) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glConvolutionFilter1DEXT QGLglConvolutionFilter1DEXT;
#define glConvolutionFilter1DEXT QGLglConvolutionFilter1DEXT

typedef void (APIENTRY* pfn_glConvolutionFilter2DEXT) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glConvolutionFilter2DEXT QGLglConvolutionFilter2DEXT;
#define glConvolutionFilter2DEXT QGLglConvolutionFilter2DEXT

typedef void (APIENTRY* pfn_glConvolutionParameterfEXT) (GLenum, GLenum, GLfloat);
extern pfn_glConvolutionParameterfEXT QGLglConvolutionParameterfEXT;
#define glConvolutionParameterfEXT QGLglConvolutionParameterfEXT

typedef void (APIENTRY* pfn_glConvolutionParameterfvEXT) (GLenum, GLenum, const GLfloat*);
extern pfn_glConvolutionParameterfvEXT QGLglConvolutionParameterfvEXT;
#define glConvolutionParameterfvEXT QGLglConvolutionParameterfvEXT

typedef void (APIENTRY* pfn_glConvolutionParameteriEXT) (GLenum, GLenum, GLint);
extern pfn_glConvolutionParameteriEXT QGLglConvolutionParameteriEXT;
#define glConvolutionParameteriEXT QGLglConvolutionParameteriEXT

typedef void (APIENTRY* pfn_glConvolutionParameterivEXT) (GLenum, GLenum, const GLint*);
extern pfn_glConvolutionParameterivEXT QGLglConvolutionParameterivEXT;
#define glConvolutionParameterivEXT QGLglConvolutionParameterivEXT

typedef void (APIENTRY* pfn_glCopyConvolutionFilter1DEXT) (GLenum, GLenum, GLint, GLint, GLsizei);
extern pfn_glCopyConvolutionFilter1DEXT QGLglCopyConvolutionFilter1DEXT;
#define glCopyConvolutionFilter1DEXT QGLglCopyConvolutionFilter1DEXT

typedef void (APIENTRY* pfn_glCopyConvolutionFilter2DEXT) (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
extern pfn_glCopyConvolutionFilter2DEXT QGLglCopyConvolutionFilter2DEXT;
#define glCopyConvolutionFilter2DEXT QGLglCopyConvolutionFilter2DEXT

typedef void (APIENTRY* pfn_glGetConvolutionFilterEXT) (GLenum, GLenum, GLenum, GLvoid*);
extern pfn_glGetConvolutionFilterEXT QGLglGetConvolutionFilterEXT;
#define glGetConvolutionFilterEXT QGLglGetConvolutionFilterEXT

typedef void (APIENTRY* pfn_glGetConvolutionParameterfvEXT) (GLenum, GLenum, GLfloat*);
extern pfn_glGetConvolutionParameterfvEXT QGLglGetConvolutionParameterfvEXT;
#define glGetConvolutionParameterfvEXT QGLglGetConvolutionParameterfvEXT

typedef void (APIENTRY* pfn_glGetConvolutionParameterivEXT) (GLenum, GLenum, GLint*);
extern pfn_glGetConvolutionParameterivEXT QGLglGetConvolutionParameterivEXT;
#define glGetConvolutionParameterivEXT QGLglGetConvolutionParameterivEXT

typedef void (APIENTRY* pfn_glGetSeparableFilterEXT) (GLenum, GLenum, GLenum, GLvoid*, GLvoid*, GLvoid*);
extern pfn_glGetSeparableFilterEXT QGLglGetSeparableFilterEXT;
#define glGetSeparableFilterEXT QGLglGetSeparableFilterEXT

typedef void (APIENTRY* pfn_glSeparableFilter2DEXT) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*, const GLvoid*);
extern pfn_glSeparableFilter2DEXT QGLglSeparableFilter2DEXT;
#define glSeparableFilter2DEXT QGLglSeparableFilter2DEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_color_matrix;
extern int QGLNumFunctions_GL_EXT_color_matrix;
#ifndef GL_EXT_color_matrix
#define GL_EXT_color_matrix
#define QGL_DEFINED_GL_EXT_color_matrix
#define GL_COLOR_MATRIX_SGI                     0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI         0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI     0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI      0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI    0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI     0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI    0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI       0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI     0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI      0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI     0x80BB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGI_color_table;
extern int QGLNumFunctions_GL_SGI_color_table;
#ifndef GL_SGI_color_table
#define GL_SGI_color_table
#define QGL_DEFINED_GL_SGI_color_table
#define GL_COLOR_TABLE_SGI                          0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI         0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI        0x80D2
#define GL_PROXY_COLOR_TABLE_SGI                    0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI   0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI  0x80D5
#define GL_COLOR_TABLE_SCALE_SGI                    0x80D6
#define GL_COLOR_TABLE_BIAS_SGI                     0x80D7
#define GL_COLOR_TABLE_FORMAT_SGI                   0x80D8
#define GL_COLOR_TABLE_WIDTH_SGI                    0x80D9
#define GL_COLOR_TABLE_RED_SIZE_SGI                 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_SGI               0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_SGI                0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI               0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI           0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI           0x80DF

typedef void (APIENTRY* pfn_glColorTableSGI) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glColorTableSGI QGLglColorTableSGI;
#define glColorTableSGI QGLglColorTableSGI

typedef void (APIENTRY* pfn_glColorTableParameterfvSGI) (GLenum, GLenum, const GLfloat*);
extern pfn_glColorTableParameterfvSGI QGLglColorTableParameterfvSGI;
#define glColorTableParameterfvSGI QGLglColorTableParameterfvSGI

typedef void (APIENTRY* pfn_glColorTableParameterivSGI) (GLenum, GLenum, const GLint*);
extern pfn_glColorTableParameterivSGI QGLglColorTableParameterivSGI;
#define glColorTableParameterivSGI QGLglColorTableParameterivSGI

typedef void (APIENTRY* pfn_glCopyColorTableSGI) (GLenum, GLenum, GLint, GLint, GLsizei);
extern pfn_glCopyColorTableSGI QGLglCopyColorTableSGI;
#define glCopyColorTableSGI QGLglCopyColorTableSGI

typedef void (APIENTRY* pfn_glGetColorTableSGI) (GLenum, GLenum, GLenum, GLvoid*);
extern pfn_glGetColorTableSGI QGLglGetColorTableSGI;
#define glGetColorTableSGI QGLglGetColorTableSGI

typedef void (APIENTRY* pfn_glGetColorTableParameterfvSGI) (GLenum, GLenum, GLfloat*);
extern pfn_glGetColorTableParameterfvSGI QGLglGetColorTableParameterfvSGI;
#define glGetColorTableParameterfvSGI QGLglGetColorTableParameterfvSGI

typedef void (APIENTRY* pfn_glGetColorTableParameterivSGI) (GLenum, GLenum, GLint*);
extern pfn_glGetColorTableParameterivSGI QGLglGetColorTableParameterivSGI;
#define glGetColorTableParameterivSGI QGLglGetColorTableParameterivSGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_pixel_texture;
extern int QGLNumFunctions_GL_SGIX_pixel_texture;
#ifndef GL_SGIX_pixel_texture
#define GL_SGIX_pixel_texture
#define QGL_DEFINED_GL_SGIX_pixel_texture
#define GL_PIXEL_TEX_GEN_SGIX             0x8139
#define GL_PIXEL_TEX_GEN_MODE_SGIX        0x832B

typedef void (APIENTRY* pfn_glPixelTexGenSGIX) (GLenum);
extern pfn_glPixelTexGenSGIX QGLglPixelTexGenSGIX;
#define glPixelTexGenSGIX QGLglPixelTexGenSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_pixel_texture;
extern int QGLNumFunctions_GL_SGIS_pixel_texture;
#ifndef GL_SGIS_pixel_texture
#define GL_SGIS_pixel_texture
#define QGL_DEFINED_GL_SGIS_pixel_texture
#define GL_PIXEL_TEXTURE_SGIS               0x8353
#define GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS   0x8354
#define GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS 0x8355
#define GL_PIXEL_GROUP_COLOR_SGIS           0x8356

typedef void (APIENTRY* pfn_glPixelTexGenParameteriSGIS) (GLenum, GLint);
extern pfn_glPixelTexGenParameteriSGIS QGLglPixelTexGenParameteriSGIS;
#define glPixelTexGenParameteriSGIS QGLglPixelTexGenParameteriSGIS

typedef void (APIENTRY* pfn_glPixelTexGenParameterivSGIS) (GLenum, const GLint*);
extern pfn_glPixelTexGenParameterivSGIS QGLglPixelTexGenParameterivSGIS;
#define glPixelTexGenParameterivSGIS QGLglPixelTexGenParameterivSGIS

typedef void (APIENTRY* pfn_glPixelTexGenParameterfSGIS) (GLenum, GLfloat);
extern pfn_glPixelTexGenParameterfSGIS QGLglPixelTexGenParameterfSGIS;
#define glPixelTexGenParameterfSGIS QGLglPixelTexGenParameterfSGIS

typedef void (APIENTRY* pfn_glPixelTexGenParameterfvSGIS) (GLenum, const GLfloat*);
extern pfn_glPixelTexGenParameterfvSGIS QGLglPixelTexGenParameterfvSGIS;
#define glPixelTexGenParameterfvSGIS QGLglPixelTexGenParameterfvSGIS

typedef void (APIENTRY* pfn_glGetPixelTexGenParameterivSGIS) (GLenum, GLint*);
extern pfn_glGetPixelTexGenParameterivSGIS QGLglGetPixelTexGenParameterivSGIS;
#define glGetPixelTexGenParameterivSGIS QGLglGetPixelTexGenParameterivSGIS

typedef void (APIENTRY* pfn_glGetPixelTexGenParameterfvSGIS) (GLenum, GLfloat*);
extern pfn_glGetPixelTexGenParameterfvSGIS QGLglGetPixelTexGenParameterfvSGIS;
#define glGetPixelTexGenParameterfvSGIS QGLglGetPixelTexGenParameterfvSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture4D;
extern int QGLNumFunctions_GL_SGIS_texture4D;
#ifndef GL_SGIS_texture4D
#define GL_SGIS_texture4D
#define QGL_DEFINED_GL_SGIS_texture4D
#define GL_PACK_SKIP_VOLUMES_SGIS         0x8130
#define GL_PACK_IMAGE_DEPTH_SGIS          0x8131
#define GL_UNPACK_SKIP_VOLUMES_SGIS       0x8132
#define GL_UNPACK_IMAGE_DEPTH_SGIS        0x8133
#define GL_TEXTURE_4D_SGIS                0x8134
#define GL_PROXY_TEXTURE_4D_SGIS          0x8135
#define GL_TEXTURE_4DSIZE_SGIS            0x8136
#define GL_TEXTURE_WRAP_Q_SGIS            0x8137
#define GL_MAX_4D_TEXTURE_SIZE_SGIS       0x8138
#define GL_TEXTURE_4D_BINDING_SGIS        0x814F

typedef void (APIENTRY* pfn_glTexImage4DSGIS) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
extern pfn_glTexImage4DSGIS QGLglTexImage4DSGIS;
#define glTexImage4DSGIS QGLglTexImage4DSGIS

typedef void (APIENTRY* pfn_glTexSubImage4DSGIS) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glTexSubImage4DSGIS QGLglTexSubImage4DSGIS;
#define glTexSubImage4DSGIS QGLglTexSubImage4DSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGI_texture_color_table;
extern int QGLNumFunctions_GL_SGI_texture_color_table;
#ifndef GL_SGI_texture_color_table
#define GL_SGI_texture_color_table
#define QGL_DEFINED_GL_SGI_texture_color_table
#define GL_TEXTURE_COLOR_TABLE_SGI        0x80BC
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI  0x80BD
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_cmyka;
extern int QGLNumFunctions_GL_EXT_cmyka;
#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka
#define QGL_DEFINED_GL_EXT_cmyka
#define GL_CMYK_EXT                       0x800C
#define GL_CMYKA_EXT                      0x800D
#define GL_PACK_CMYK_HINT_EXT             0x800E
#define GL_UNPACK_CMYK_HINT_EXT           0x800F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_object;
extern int QGLNumFunctions_GL_EXT_texture_object;
#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object
#define QGL_DEFINED_GL_EXT_texture_object
#define GL_TEXTURE_PRIORITY_EXT           0x8066
#define GL_TEXTURE_RESIDENT_EXT           0x8067
#define GL_TEXTURE_1D_BINDING_EXT         0x8068
#define GL_TEXTURE_2D_BINDING_EXT         0x8069
#define GL_TEXTURE_3D_BINDING_EXT         0x806A

typedef GLboolean (APIENTRY* pfn_glAreTexturesResidentEXT) (GLsizei, const GLuint*, GLboolean*);
extern pfn_glAreTexturesResidentEXT QGLglAreTexturesResidentEXT;
#define glAreTexturesResidentEXT QGLglAreTexturesResidentEXT

typedef void (APIENTRY* pfn_glBindTextureEXT) (GLenum, GLuint);
extern pfn_glBindTextureEXT QGLglBindTextureEXT;
#define glBindTextureEXT QGLglBindTextureEXT

typedef void (APIENTRY* pfn_glDeleteTexturesEXT) (GLsizei, const GLuint*);
extern pfn_glDeleteTexturesEXT QGLglDeleteTexturesEXT;
#define glDeleteTexturesEXT QGLglDeleteTexturesEXT

typedef void (APIENTRY* pfn_glGenTexturesEXT) (GLsizei, GLuint*);
extern pfn_glGenTexturesEXT QGLglGenTexturesEXT;
#define glGenTexturesEXT QGLglGenTexturesEXT

typedef GLboolean (APIENTRY* pfn_glIsTextureEXT) (GLuint);
extern pfn_glIsTextureEXT QGLglIsTextureEXT;
#define glIsTextureEXT QGLglIsTextureEXT

typedef void (APIENTRY* pfn_glPrioritizeTexturesEXT) (GLsizei, const GLuint*, const GLclampf*);
extern pfn_glPrioritizeTexturesEXT QGLglPrioritizeTexturesEXT;
#define glPrioritizeTexturesEXT QGLglPrioritizeTexturesEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_detail_texture;
extern int QGLNumFunctions_GL_SGIS_detail_texture;
#ifndef GL_SGIS_detail_texture
#define GL_SGIS_detail_texture
#define QGL_DEFINED_GL_SGIS_detail_texture
#define GL_DETAIL_TEXTURE_2D_SGIS           0x8095
#define GL_DETAIL_TEXTURE_2D_BINDING_SGIS   0x8096
#define GL_LINEAR_DETAIL_SGIS               0x8097
#define GL_LINEAR_DETAIL_ALPHA_SGIS         0x8098
#define GL_LINEAR_DETAIL_COLOR_SGIS         0x8099
#define GL_DETAIL_TEXTURE_LEVEL_SGIS        0x809A
#define GL_DETAIL_TEXTURE_MODE_SGIS         0x809B
#define GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS  0x809C

typedef void (APIENTRY* pfn_glDetailTexFuncSGIS) (GLenum, GLsizei, const GLfloat*);
extern pfn_glDetailTexFuncSGIS QGLglDetailTexFuncSGIS;
#define glDetailTexFuncSGIS QGLglDetailTexFuncSGIS

typedef void (APIENTRY* pfn_glGetDetailTexFuncSGIS) (GLenum, GLfloat*);
extern pfn_glGetDetailTexFuncSGIS QGLglGetDetailTexFuncSGIS;
#define glGetDetailTexFuncSGIS QGLglGetDetailTexFuncSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_sharpen_texture;
extern int QGLNumFunctions_GL_SGIS_sharpen_texture;
#ifndef GL_SGIS_sharpen_texture
#define GL_SGIS_sharpen_texture
#define QGL_DEFINED_GL_SGIS_sharpen_texture
#define GL_LINEAR_SHARPEN_SGIS              0x80AD
#define GL_LINEAR_SHARPEN_ALPHA_SGIS        0x80AE
#define GL_LINEAR_SHARPEN_COLOR_SGIS        0x80AF
#define GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS 0x80B0

typedef void (APIENTRY* pfn_glSharpenTexFuncSGIS) (GLenum, GLsizei, const GLfloat*);
extern pfn_glSharpenTexFuncSGIS QGLglSharpenTexFuncSGIS;
#define glSharpenTexFuncSGIS QGLglSharpenTexFuncSGIS

typedef void (APIENTRY* pfn_glGetSharpenTexFuncSGIS) (GLenum, GLfloat*);
extern pfn_glGetSharpenTexFuncSGIS QGLglGetSharpenTexFuncSGIS;
#define glGetSharpenTexFuncSGIS QGLglGetSharpenTexFuncSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_packed_pixels;
extern int QGLNumFunctions_GL_EXT_packed_pixels;
#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels
#define QGL_DEFINED_GL_EXT_packed_pixels
#define GL_UNSIGNED_BYTE_3_3_2_EXT        0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT     0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT       0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT    0x8036
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture_lod;
extern int QGLNumFunctions_GL_SGIS_texture_lod;
#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod
#define QGL_DEFINED_GL_SGIS_texture_lod
#define GL_TEXTURE_MIN_LOD_SGIS           0x813A
#define GL_TEXTURE_MAX_LOD_SGIS           0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS        0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS         0x813D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_multisample;
extern int QGLNumFunctions_GL_SGIS_multisample;
#ifndef GL_SGIS_multisample
#define GL_SGIS_multisample
#define QGL_DEFINED_GL_SGIS_multisample
#define GL_MULTISAMPLE_SGIS               0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS      0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS       0x809F
#define GL_SAMPLE_MASK_SGIS               0x80A0
#define GL_1PASS_SGIS                     0x80A1
#define GL_2PASS_0_SGIS                   0x80A2
#define GL_2PASS_1_SGIS                   0x80A3
#define GL_4PASS_0_SGIS                   0x80A4
#define GL_4PASS_1_SGIS                   0x80A5
#define GL_4PASS_2_SGIS                   0x80A6
#define GL_4PASS_3_SGIS                   0x80A7
#define GL_SAMPLE_BUFFERS_SGIS            0x80A8
#define GL_SAMPLES_SGIS                   0x80A9
#define GL_SAMPLE_MASK_VALUE_SGIS         0x80AA
#define GL_SAMPLE_MASK_INVERT_SGIS        0x80AB
#define GL_SAMPLE_PATTERN_SGIS            0x80AC

typedef void (APIENTRY* pfn_glSampleMaskSGIS) (GLclampf, GLboolean);
extern pfn_glSampleMaskSGIS QGLglSampleMaskSGIS;
#define glSampleMaskSGIS QGLglSampleMaskSGIS

typedef void (APIENTRY* pfn_glSamplePatternSGIS) (GLenum);
extern pfn_glSamplePatternSGIS QGLglSamplePatternSGIS;
#define glSamplePatternSGIS QGLglSamplePatternSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_rescale_normal;
extern int QGLNumFunctions_GL_EXT_rescale_normal;
#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal
#define QGL_DEFINED_GL_EXT_rescale_normal
#define GL_RESCALE_NORMAL_EXT             0x803A
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_vertex_array;
extern int QGLNumFunctions_GL_EXT_vertex_array;
#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array
#define QGL_DEFINED_GL_EXT_vertex_array
#define GL_VERTEX_ARRAY_EXT                 0x8074
#define GL_NORMAL_ARRAY_EXT                 0x8075
#define GL_COLOR_ARRAY_EXT                  0x8076
#define GL_INDEX_ARRAY_EXT                  0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT          0x8078
#define GL_EDGE_FLAG_ARRAY_EXT              0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT            0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT            0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT          0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT           0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT            0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT          0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT           0x8080
#define GL_COLOR_ARRAY_SIZE_EXT             0x8081
#define GL_COLOR_ARRAY_TYPE_EXT             0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT           0x8083
#define GL_COLOR_ARRAY_COUNT_EXT            0x8084
#define GL_INDEX_ARRAY_TYPE_EXT             0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT           0x8086
#define GL_INDEX_ARRAY_COUNT_EXT            0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT     0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT     0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT   0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT    0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT       0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT        0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT         0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT         0x808F
#define GL_COLOR_ARRAY_POINTER_EXT          0x8090
#define GL_INDEX_ARRAY_POINTER_EXT          0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT  0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT      0x8093

typedef void (APIENTRY* pfn_glArrayElementEXT) (GLint);
extern pfn_glArrayElementEXT QGLglArrayElementEXT;
#define glArrayElementEXT QGLglArrayElementEXT

typedef void (APIENTRY* pfn_glColorPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid*);
extern pfn_glColorPointerEXT QGLglColorPointerEXT;
#define glColorPointerEXT QGLglColorPointerEXT

typedef void (APIENTRY* pfn_glDrawArraysEXT) (GLenum, GLint, GLsizei);
extern pfn_glDrawArraysEXT QGLglDrawArraysEXT;
#define glDrawArraysEXT QGLglDrawArraysEXT

typedef void (APIENTRY* pfn_glEdgeFlagPointerEXT) (GLsizei, GLsizei, const GLboolean*);
extern pfn_glEdgeFlagPointerEXT QGLglEdgeFlagPointerEXT;
#define glEdgeFlagPointerEXT QGLglEdgeFlagPointerEXT

typedef void (APIENTRY* pfn_glGetPointervEXT) (GLenum, GLvoid**);
extern pfn_glGetPointervEXT QGLglGetPointervEXT;
#define glGetPointervEXT QGLglGetPointervEXT

typedef void (APIENTRY* pfn_glIndexPointerEXT) (GLenum, GLsizei, GLsizei, const GLvoid*);
extern pfn_glIndexPointerEXT QGLglIndexPointerEXT;
#define glIndexPointerEXT QGLglIndexPointerEXT

typedef void (APIENTRY* pfn_glNormalPointerEXT) (GLenum, GLsizei, GLsizei, const GLvoid*);
extern pfn_glNormalPointerEXT QGLglNormalPointerEXT;
#define glNormalPointerEXT QGLglNormalPointerEXT

typedef void (APIENTRY* pfn_glTexCoordPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid*);
extern pfn_glTexCoordPointerEXT QGLglTexCoordPointerEXT;
#define glTexCoordPointerEXT QGLglTexCoordPointerEXT

typedef void (APIENTRY* pfn_glVertexPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid*);
extern pfn_glVertexPointerEXT QGLglVertexPointerEXT;
#define glVertexPointerEXT QGLglVertexPointerEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_misc_attribute;
extern int QGLNumFunctions_GL_EXT_misc_attribute;
#ifndef GL_EXT_misc_attribute
#define GL_EXT_misc_attribute
#define QGL_DEFINED_GL_EXT_misc_attribute
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_generate_mipmap;
extern int QGLNumFunctions_GL_SGIS_generate_mipmap;
#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap
#define QGL_DEFINED_GL_SGIS_generate_mipmap
#define GL_GENERATE_MIPMAP_SGIS           0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS      0x8192
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_clipmap;
extern int QGLNumFunctions_GL_SGIX_clipmap;
#ifndef GL_SGIX_clipmap
#define GL_SGIX_clipmap
#define QGL_DEFINED_GL_SGIX_clipmap
#define GL_LINEAR_CLIPMAP_LINEAR_SGIX     0x8170
#define GL_TEXTURE_CLIPMAP_CENTER_SGIX    0x8171
#define GL_TEXTURE_CLIPMAP_FRAME_SGIX     0x8172
#define GL_TEXTURE_CLIPMAP_OFFSET_SGIX    0x8173
#define GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8174
#define GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX 0x8175
#define GL_TEXTURE_CLIPMAP_DEPTH_SGIX     0x8176
#define GL_MAX_CLIPMAP_DEPTH_SGIX         0x8177
#define GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8178
#define GL_NEAREST_CLIPMAP_NEAREST_SGIX   0x844D
#define GL_NEAREST_CLIPMAP_LINEAR_SGIX    0x844E
#define GL_LINEAR_CLIPMAP_NEAREST_SGIX    0x844F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_shadow;
extern int QGLNumFunctions_GL_SGIX_shadow;
#ifndef GL_SGIX_shadow
#define GL_SGIX_shadow
#define QGL_DEFINED_GL_SGIX_shadow
#define GL_TEXTURE_COMPARE_SGIX           0x819A
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX  0x819B
#define GL_TEXTURE_LEQUAL_R_SGIX          0x819C
#define GL_TEXTURE_GEQUAL_R_SGIX          0x819D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture_edge_clamp;
extern int QGLNumFunctions_GL_SGIS_texture_edge_clamp;
#ifndef GL_SGIS_texture_edge_clamp
#define GL_SGIS_texture_edge_clamp
#define QGL_DEFINED_GL_SGIS_texture_edge_clamp
#define GL_CLAMP_TO_EDGE_SGIS             0x812F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture_border_clamp;
extern int QGLNumFunctions_GL_SGIS_texture_border_clamp;
#ifndef GL_SGIS_texture_border_clamp
#define GL_SGIS_texture_border_clamp
#define QGL_DEFINED_GL_SGIS_texture_border_clamp
#define GL_CLAMP_TO_BORDER_SGIS           0x812D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_minmax;
extern int QGLNumFunctions_GL_EXT_blend_minmax;
#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax
#define QGL_DEFINED_GL_EXT_blend_minmax
#define GL_FUNC_ADD_EXT                   0x8006
#define GL_MIN_EXT                        0x8007
#define GL_MAX_EXT                        0x8008
#define GL_BLEND_EQUATION_EXT             0x8009

typedef void (APIENTRY* pfn_glBlendEquationEXT) (GLenum);
extern pfn_glBlendEquationEXT QGLglBlendEquationEXT;
#define glBlendEquationEXT QGLglBlendEquationEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_subtract;
extern int QGLNumFunctions_GL_EXT_blend_subtract;
#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract
#define QGL_DEFINED_GL_EXT_blend_subtract
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_logic_op;
extern int QGLNumFunctions_GL_EXT_blend_logic_op;
#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op
#define QGL_DEFINED_GL_EXT_blend_logic_op
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_interlace;
extern int QGLNumFunctions_GL_SGIX_interlace;
#ifndef GL_SGIX_interlace
#define GL_SGIX_interlace
#define QGL_DEFINED_GL_SGIX_interlace
#define GL_INTERLACE_SGIX                 0x8094
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_pixel_tiles;
extern int QGLNumFunctions_GL_SGIX_pixel_tiles;
#ifndef GL_SGIX_pixel_tiles
#define GL_SGIX_pixel_tiles
#define QGL_DEFINED_GL_SGIX_pixel_tiles
#define GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX   0x813E
#define GL_PIXEL_TILE_CACHE_INCREMENT_SGIX  0x813F
#define GL_PIXEL_TILE_WIDTH_SGIX            0x8140
#define GL_PIXEL_TILE_HEIGHT_SGIX           0x8141
#define GL_PIXEL_TILE_GRID_WIDTH_SGIX       0x8142
#define GL_PIXEL_TILE_GRID_HEIGHT_SGIX      0x8143
#define GL_PIXEL_TILE_GRID_DEPTH_SGIX       0x8144
#define GL_PIXEL_TILE_CACHE_SIZE_SGIX       0x8145
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_select;
extern int QGLNumFunctions_GL_SGIX_texture_select;
#ifndef GL_SGIX_texture_select
#define GL_SGIX_texture_select
#define QGL_DEFINED_GL_SGIX_texture_select
#define GL_DUAL_ALPHA4_SGIS               0x8110
#define GL_DUAL_ALPHA8_SGIS               0x8111
#define GL_DUAL_ALPHA12_SGIS              0x8112
#define GL_DUAL_ALPHA16_SGIS              0x8113
#define GL_DUAL_LUMINANCE4_SGIS           0x8114
#define GL_DUAL_LUMINANCE8_SGIS           0x8115
#define GL_DUAL_LUMINANCE12_SGIS          0x8116
#define GL_DUAL_LUMINANCE16_SGIS          0x8117
#define GL_DUAL_INTENSITY4_SGIS           0x8118
#define GL_DUAL_INTENSITY8_SGIS           0x8119
#define GL_DUAL_INTENSITY12_SGIS          0x811A
#define GL_DUAL_INTENSITY16_SGIS          0x811B
#define GL_DUAL_LUMINANCE_ALPHA4_SGIS     0x811C
#define GL_DUAL_LUMINANCE_ALPHA8_SGIS     0x811D
#define GL_QUAD_ALPHA4_SGIS               0x811E
#define GL_QUAD_ALPHA8_SGIS               0x811F
#define GL_QUAD_LUMINANCE4_SGIS           0x8120
#define GL_QUAD_LUMINANCE8_SGIS           0x8121
#define GL_QUAD_INTENSITY4_SGIS           0x8122
#define GL_QUAD_INTENSITY8_SGIS           0x8123
#define GL_DUAL_TEXTURE_SELECT_SGIS       0x8124
#define GL_QUAD_TEXTURE_SELECT_SGIS       0x8125
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_sprite;
extern int QGLNumFunctions_GL_SGIX_sprite;
#ifndef GL_SGIX_sprite
#define GL_SGIX_sprite
#define QGL_DEFINED_GL_SGIX_sprite
#define GL_SPRITE_SGIX                    0x8148
#define GL_SPRITE_MODE_SGIX               0x8149
#define GL_SPRITE_AXIS_SGIX               0x814A
#define GL_SPRITE_TRANSLATION_SGIX        0x814B
#define GL_SPRITE_AXIAL_SGIX              0x814C
#define GL_SPRITE_OBJECT_ALIGNED_SGIX     0x814D
#define GL_SPRITE_EYE_ALIGNED_SGIX        0x814E

typedef void (APIENTRY* pfn_glSpriteParameterfSGIX) (GLenum, GLfloat);
extern pfn_glSpriteParameterfSGIX QGLglSpriteParameterfSGIX;
#define glSpriteParameterfSGIX QGLglSpriteParameterfSGIX

typedef void (APIENTRY* pfn_glSpriteParameterfvSGIX) (GLenum, const GLfloat*);
extern pfn_glSpriteParameterfvSGIX QGLglSpriteParameterfvSGIX;
#define glSpriteParameterfvSGIX QGLglSpriteParameterfvSGIX

typedef void (APIENTRY* pfn_glSpriteParameteriSGIX) (GLenum, GLint);
extern pfn_glSpriteParameteriSGIX QGLglSpriteParameteriSGIX;
#define glSpriteParameteriSGIX QGLglSpriteParameteriSGIX

typedef void (APIENTRY* pfn_glSpriteParameterivSGIX) (GLenum, const GLint*);
extern pfn_glSpriteParameterivSGIX QGLglSpriteParameterivSGIX;
#define glSpriteParameterivSGIX QGLglSpriteParameterivSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_multi_buffer;
extern int QGLNumFunctions_GL_SGIX_texture_multi_buffer;
#ifndef GL_SGIX_texture_multi_buffer
#define GL_SGIX_texture_multi_buffer
#define QGL_DEFINED_GL_SGIX_texture_multi_buffer
#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_point_parameters;
extern int QGLNumFunctions_GL_EXT_point_parameters;
#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters
#define QGL_DEFINED_GL_EXT_point_parameters
#define GL_POINT_SIZE_MIN_EXT             0x8126
#define GL_POINT_SIZE_MAX_EXT             0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT  0x8128
#define GL_DISTANCE_ATTENUATION_EXT       0x8129

typedef void (APIENTRY* pfn_glPointParameterfEXT) (GLenum, GLfloat);
extern pfn_glPointParameterfEXT QGLglPointParameterfEXT;
#define glPointParameterfEXT QGLglPointParameterfEXT

typedef void (APIENTRY* pfn_glPointParameterfvEXT) (GLenum, const GLfloat*);
extern pfn_glPointParameterfvEXT QGLglPointParameterfvEXT;
#define glPointParameterfvEXT QGLglPointParameterfvEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_point_parameters;
extern int QGLNumFunctions_GL_SGIS_point_parameters;
#ifndef GL_SGIS_point_parameters
#define GL_SGIS_point_parameters
#define QGL_DEFINED_GL_SGIS_point_parameters
#define GL_POINT_SIZE_MIN_SGIS            0x8126
#define GL_POINT_SIZE_MAX_SGIS            0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_SGIS 0x8128
#define GL_DISTANCE_ATTENUATION_SGIS      0x8129

typedef void (APIENTRY* pfn_glPointParameterfSGIS) (GLenum, GLfloat);
extern pfn_glPointParameterfSGIS QGLglPointParameterfSGIS;
#define glPointParameterfSGIS QGLglPointParameterfSGIS

typedef void (APIENTRY* pfn_glPointParameterfvSGIS) (GLenum, const GLfloat*);
extern pfn_glPointParameterfvSGIS QGLglPointParameterfvSGIS;
#define glPointParameterfvSGIS QGLglPointParameterfvSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_instruments;
extern int QGLNumFunctions_GL_SGIX_instruments;
#ifndef GL_SGIX_instruments
#define GL_SGIX_instruments
#define QGL_DEFINED_GL_SGIX_instruments
#define GL_INSTRUMENT_BUFFER_POINTER_SGIX 0x8180
#define GL_INSTRUMENT_MEASUREMENTS_SGIX   0x8181

typedef GLint (APIENTRY* pfn_glGetInstrumentsSGIX) (void);
extern pfn_glGetInstrumentsSGIX QGLglGetInstrumentsSGIX;
#define glGetInstrumentsSGIX QGLglGetInstrumentsSGIX

typedef void (APIENTRY* pfn_glInstrumentsBufferSGIX) (GLsizei, GLint*);
extern pfn_glInstrumentsBufferSGIX QGLglInstrumentsBufferSGIX;
#define glInstrumentsBufferSGIX QGLglInstrumentsBufferSGIX

typedef GLint (APIENTRY* pfn_glPollInstrumentsSGIX) (GLint*);
extern pfn_glPollInstrumentsSGIX QGLglPollInstrumentsSGIX;
#define glPollInstrumentsSGIX QGLglPollInstrumentsSGIX

typedef void (APIENTRY* pfn_glReadInstrumentsSGIX) (GLint);
extern pfn_glReadInstrumentsSGIX QGLglReadInstrumentsSGIX;
#define glReadInstrumentsSGIX QGLglReadInstrumentsSGIX

typedef void (APIENTRY* pfn_glStartInstrumentsSGIX) (void);
extern pfn_glStartInstrumentsSGIX QGLglStartInstrumentsSGIX;
#define glStartInstrumentsSGIX QGLglStartInstrumentsSGIX

typedef void (APIENTRY* pfn_glStopInstrumentsSGIX) (GLint);
extern pfn_glStopInstrumentsSGIX QGLglStopInstrumentsSGIX;
#define glStopInstrumentsSGIX QGLglStopInstrumentsSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_scale_bias;
extern int QGLNumFunctions_GL_SGIX_texture_scale_bias;
#ifndef GL_SGIX_texture_scale_bias
#define GL_SGIX_texture_scale_bias
#define QGL_DEFINED_GL_SGIX_texture_scale_bias
#define GL_POST_TEXTURE_FILTER_BIAS_SGIX  0x8179
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_framezoom;
extern int QGLNumFunctions_GL_SGIX_framezoom;
#ifndef GL_SGIX_framezoom
#define GL_SGIX_framezoom
#define QGL_DEFINED_GL_SGIX_framezoom
#define GL_FRAMEZOOM_SGIX                 0x818B
#define GL_FRAMEZOOM_FACTOR_SGIX          0x818C
#define GL_MAX_FRAMEZOOM_FACTOR_SGIX      0x818D

typedef void (APIENTRY* pfn_glFrameZoomSGIX) (GLint);
extern pfn_glFrameZoomSGIX QGLglFrameZoomSGIX;
#define glFrameZoomSGIX QGLglFrameZoomSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_tag_sample_buffer;
extern int QGLNumFunctions_GL_SGIX_tag_sample_buffer;
#ifndef GL_SGIX_tag_sample_buffer
#define GL_SGIX_tag_sample_buffer
#define QGL_DEFINED_GL_SGIX_tag_sample_buffer

typedef void (APIENTRY* pfn_glTagSampleBufferSGIX) (void);
extern pfn_glTagSampleBufferSGIX QGLglTagSampleBufferSGIX;
#define glTagSampleBufferSGIX QGLglTagSampleBufferSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_FfdMaskSGIX;
extern int QGLNumFunctions_GL_FfdMaskSGIX;
#ifndef GL_FfdMaskSGIX
#define GL_FfdMaskSGIX
#define QGL_DEFINED_GL_FfdMaskSGIX
#define GL_TEXTURE_DEFORMATION_BIT_SGIX   0x00000001
#define GL_GEOMETRY_DEFORMATION_BIT_SGIX  0x00000002
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_polynomial_ffd;
extern int QGLNumFunctions_GL_SGIX_polynomial_ffd;
#ifndef GL_SGIX_polynomial_ffd
#define GL_SGIX_polynomial_ffd
#define QGL_DEFINED_GL_SGIX_polynomial_ffd
#define GL_GEOMETRY_DEFORMATION_SGIX      0x8194
#define GL_TEXTURE_DEFORMATION_SGIX       0x8195
#define GL_DEFORMATIONS_MASK_SGIX         0x8196
#define GL_MAX_DEFORMATION_ORDER_SGIX     0x8197

typedef void (APIENTRY* pfn_glDeformationMap3dSGIX) (GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble*);
extern pfn_glDeformationMap3dSGIX QGLglDeformationMap3dSGIX;
#define glDeformationMap3dSGIX QGLglDeformationMap3dSGIX

typedef void (APIENTRY* pfn_glDeformationMap3fSGIX) (GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat*);
extern pfn_glDeformationMap3fSGIX QGLglDeformationMap3fSGIX;
#define glDeformationMap3fSGIX QGLglDeformationMap3fSGIX

typedef void (APIENTRY* pfn_glDeformSGIX) (GLbitfield);
extern pfn_glDeformSGIX QGLglDeformSGIX;
#define glDeformSGIX QGLglDeformSGIX

typedef void (APIENTRY* pfn_glLoadIdentityDeformationMapSGIX) (GLbitfield);
extern pfn_glLoadIdentityDeformationMapSGIX QGLglLoadIdentityDeformationMapSGIX;
#define glLoadIdentityDeformationMapSGIX QGLglLoadIdentityDeformationMapSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_reference_plane;
extern int QGLNumFunctions_GL_SGIX_reference_plane;
#ifndef GL_SGIX_reference_plane
#define GL_SGIX_reference_plane
#define QGL_DEFINED_GL_SGIX_reference_plane
#define GL_REFERENCE_PLANE_SGIX           0x817D
#define GL_REFERENCE_PLANE_EQUATION_SGIX  0x817E

typedef void (APIENTRY* pfn_glReferencePlaneSGIX) (const GLdouble*);
extern pfn_glReferencePlaneSGIX QGLglReferencePlaneSGIX;
#define glReferencePlaneSGIX QGLglReferencePlaneSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_flush_raster;
extern int QGLNumFunctions_GL_SGIX_flush_raster;
#ifndef GL_SGIX_flush_raster
#define GL_SGIX_flush_raster
#define QGL_DEFINED_GL_SGIX_flush_raster

typedef void (APIENTRY* pfn_glFlushRasterSGIX) (void);
extern pfn_glFlushRasterSGIX QGLglFlushRasterSGIX;
#define glFlushRasterSGIX QGLglFlushRasterSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_depth_texture;
extern int QGLNumFunctions_GL_SGIX_depth_texture;
#ifndef GL_SGIX_depth_texture
#define GL_SGIX_depth_texture
#define QGL_DEFINED_GL_SGIX_depth_texture
#define GL_DEPTH_COMPONENT16_SGIX         0x81A5
#define GL_DEPTH_COMPONENT24_SGIX         0x81A6
#define GL_DEPTH_COMPONENT32_SGIX         0x81A7
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_fog_function;
extern int QGLNumFunctions_GL_SGIS_fog_function;
#ifndef GL_SGIS_fog_function
#define GL_SGIS_fog_function
#define QGL_DEFINED_GL_SGIS_fog_function
#define GL_FOG_FUNC_SGIS                  0x812A
#define GL_FOG_FUNC_POINTS_SGIS           0x812B
#define GL_MAX_FOG_FUNC_POINTS_SGIS       0x812C

typedef void (APIENTRY* pfn_glFogFuncSGIS) (GLsizei, const GLfloat*);
extern pfn_glFogFuncSGIS QGLglFogFuncSGIS;
#define glFogFuncSGIS QGLglFogFuncSGIS

typedef void (APIENTRY* pfn_glGetFogFuncSGIS) (GLfloat*);
extern pfn_glGetFogFuncSGIS QGLglGetFogFuncSGIS;
#define glGetFogFuncSGIS QGLglGetFogFuncSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_fog_offset;
extern int QGLNumFunctions_GL_SGIX_fog_offset;
#ifndef GL_SGIX_fog_offset
#define GL_SGIX_fog_offset
#define QGL_DEFINED_GL_SGIX_fog_offset
#define GL_FOG_OFFSET_SGIX                0x8198
#define GL_FOG_OFFSET_VALUE_SGIX          0x8199
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_HP_image_transform;
extern int QGLNumFunctions_GL_HP_image_transform;
#ifndef GL_HP_image_transform
#define GL_HP_image_transform
#define QGL_DEFINED_GL_HP_image_transform
#define GL_IMAGE_SCALE_X_HP               0x8155
#define GL_IMAGE_SCALE_Y_HP               0x8156
#define GL_IMAGE_TRANSLATE_X_HP           0x8157
#define GL_IMAGE_TRANSLATE_Y_HP           0x8158
#define GL_IMAGE_ROTATE_ANGLE_HP          0x8159
#define GL_IMAGE_ROTATE_ORIGIN_X_HP       0x815A
#define GL_IMAGE_ROTATE_ORIGIN_Y_HP       0x815B
#define GL_IMAGE_MAG_FILTER_HP            0x815C
#define GL_IMAGE_MIN_FILTER_HP            0x815D
#define GL_IMAGE_CUBIC_WEIGHT_HP          0x815E
#define GL_CUBIC_HP                       0x815F
#define GL_AVERAGE_HP                     0x8160
#define GL_IMAGE_TRANSFORM_2D_HP          0x8161
#define GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8162
#define GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8163

typedef void (APIENTRY* pfn_glImageTransformParameteriHP) (GLenum, GLenum, GLint);
extern pfn_glImageTransformParameteriHP QGLglImageTransformParameteriHP;
#define glImageTransformParameteriHP QGLglImageTransformParameteriHP

typedef void (APIENTRY* pfn_glImageTransformParameterfHP) (GLenum, GLenum, GLfloat);
extern pfn_glImageTransformParameterfHP QGLglImageTransformParameterfHP;
#define glImageTransformParameterfHP QGLglImageTransformParameterfHP

typedef void (APIENTRY* pfn_glImageTransformParameterivHP) (GLenum, GLenum, const GLint*);
extern pfn_glImageTransformParameterivHP QGLglImageTransformParameterivHP;
#define glImageTransformParameterivHP QGLglImageTransformParameterivHP

typedef void (APIENTRY* pfn_glImageTransformParameterfvHP) (GLenum, GLenum, const GLfloat*);
extern pfn_glImageTransformParameterfvHP QGLglImageTransformParameterfvHP;
#define glImageTransformParameterfvHP QGLglImageTransformParameterfvHP

typedef void (APIENTRY* pfn_glGetImageTransformParameterivHP) (GLenum, GLenum, GLint*);
extern pfn_glGetImageTransformParameterivHP QGLglGetImageTransformParameterivHP;
#define glGetImageTransformParameterivHP QGLglGetImageTransformParameterivHP

typedef void (APIENTRY* pfn_glGetImageTransformParameterfvHP) (GLenum, GLenum, GLfloat*);
extern pfn_glGetImageTransformParameterfvHP QGLglGetImageTransformParameterfvHP;
#define glGetImageTransformParameterfvHP QGLglGetImageTransformParameterfvHP
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_HP_convolution_border_modes;
extern int QGLNumFunctions_GL_HP_convolution_border_modes;
#ifndef GL_HP_convolution_border_modes
#define GL_HP_convolution_border_modes
#define QGL_DEFINED_GL_HP_convolution_border_modes
#define GL_IGNORE_BORDER_HP               0x8150
#define GL_CONSTANT_BORDER_HP             0x8151
#define GL_REPLICATE_BORDER_HP            0x8153
#define GL_CONVOLUTION_BORDER_COLOR_HP    0x8154
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INGR_palette_buffer;
extern int QGLNumFunctions_GL_INGR_palette_buffer;
#ifndef GL_INGR_palette_buffer
#define GL_INGR_palette_buffer
#define QGL_DEFINED_GL_INGR_palette_buffer
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_add_env;
extern int QGLNumFunctions_GL_SGIX_texture_add_env;
#ifndef GL_SGIX_texture_add_env
#define GL_SGIX_texture_add_env
#define QGL_DEFINED_GL_SGIX_texture_add_env
#define GL_TEXTURE_ENV_BIAS_SGIX          0x80BE
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_color_subtable;
extern int QGLNumFunctions_GL_EXT_color_subtable;
#ifndef GL_EXT_color_subtable
#define GL_EXT_color_subtable
#define QGL_DEFINED_GL_EXT_color_subtable

typedef void (APIENTRY* pfn_glColorSubTableEXT) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glColorSubTableEXT QGLglColorSubTableEXT;
#define glColorSubTableEXT QGLglColorSubTableEXT

typedef void (APIENTRY* pfn_glCopyColorSubTableEXT) (GLenum, GLsizei, GLint, GLint, GLsizei);
extern pfn_glCopyColorSubTableEXT QGLglCopyColorSubTableEXT;
#define glCopyColorSubTableEXT QGLglCopyColorSubTableEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_PGI_vertex_hints;
extern int QGLNumFunctions_GL_PGI_vertex_hints;
#ifndef GL_PGI_vertex_hints
#define GL_PGI_vertex_hints
#define QGL_DEFINED_GL_PGI_vertex_hints
#define GL_VERTEX_DATA_HINT_PGI             0x1A22A
#define GL_VERTEX_CONSISTENT_HINT_PGI       0x1A22B
#define GL_MATERIAL_SIDE_HINT_PGI           0x1A22C
#define GL_MAX_VERTEX_HINT_PGI              0x1A22D
#define GL_COLOR3_BIT_PGI                   0x00010000
#define GL_COLOR4_BIT_PGI                   0x00020000
#define GL_EDGEFLAG_BIT_PGI                 0x00040000
#define GL_INDEX_BIT_PGI                    0x00080000
#define GL_MAT_AMBIENT_BIT_PGI              0x00100000
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI  0x00200000
#define GL_MAT_DIFFUSE_BIT_PGI              0x00400000
#define GL_MAT_EMISSION_BIT_PGI             0x00800000
#define GL_MAT_COLOR_INDEXES_BIT_PGI        0x01000000
#define GL_MAT_SHININESS_BIT_PGI            0x02000000
#define GL_MAT_SPECULAR_BIT_PGI             0x04000000
#define GL_NORMAL_BIT_PGI                   0x08000000
#define GL_TEXCOORD1_BIT_PGI                0x10000000
#define GL_TEXCOORD2_BIT_PGI                0x20000000
#define GL_TEXCOORD3_BIT_PGI                0x40000000
#define GL_TEXCOORD4_BIT_PGI                0x80000000
#define GL_VERTEX23_BIT_PGI                 0x00000004
#define GL_VERTEX4_BIT_PGI                  0x00000008
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_PGI_misc_hints;
extern int QGLNumFunctions_GL_PGI_misc_hints;
#ifndef GL_PGI_misc_hints
#define GL_PGI_misc_hints
#define QGL_DEFINED_GL_PGI_misc_hints
#define GL_PREFER_DOUBLEBUFFER_HINT_PGI   0x1A1F8
#define GL_CONSERVE_MEMORY_HINT_PGI       0x1A1FD
#define GL_RECLAIM_MEMORY_HINT_PGI        0x1A1FE
#define GL_NATIVE_GRAPHICS_HANDLE_PGI     0x1A202
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI 0x1A203
#define GL_NATIVE_GRAPHICS_END_HINT_PGI   0x1A204
#define GL_ALWAYS_FAST_HINT_PGI           0x1A20C
#define GL_ALWAYS_SOFT_HINT_PGI           0x1A20D
#define GL_ALLOW_DRAW_OBJ_HINT_PGI        0x1A20E
#define GL_ALLOW_DRAW_WIN_HINT_PGI        0x1A20F
#define GL_ALLOW_DRAW_FRG_HINT_PGI        0x1A210
#define GL_ALLOW_DRAW_MEM_HINT_PGI        0x1A211
#define GL_STRICT_DEPTHFUNC_HINT_PGI      0x1A216
#define GL_STRICT_LIGHTING_HINT_PGI       0x1A217
#define GL_STRICT_SCISSOR_HINT_PGI        0x1A218
#define GL_FULL_STIPPLE_HINT_PGI          0x1A219
#define GL_CLIP_NEAR_HINT_PGI             0x1A220
#define GL_CLIP_FAR_HINT_PGI              0x1A221
#define GL_WIDE_LINE_HINT_PGI             0x1A222
#define GL_BACK_NORMALS_HINT_PGI          0x1A223

typedef void (APIENTRY* pfn_glHintPGI) (GLenum, GLint);
extern pfn_glHintPGI QGLglHintPGI;
#define glHintPGI QGLglHintPGI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_paletted_texture;
extern int QGLNumFunctions_GL_EXT_paletted_texture;
#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture
#define QGL_DEFINED_GL_EXT_paletted_texture
#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT         0x80ED

typedef void (APIENTRY* pfn_glColorTableEXT) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern pfn_glColorTableEXT QGLglColorTableEXT;
#define glColorTableEXT QGLglColorTableEXT

typedef void (APIENTRY* pfn_glGetColorTableEXT) (GLenum, GLenum, GLenum, GLvoid*);
extern pfn_glGetColorTableEXT QGLglGetColorTableEXT;
#define glGetColorTableEXT QGLglGetColorTableEXT

typedef void (APIENTRY* pfn_glGetColorTableParameterivEXT) (GLenum, GLenum, GLint*);
extern pfn_glGetColorTableParameterivEXT QGLglGetColorTableParameterivEXT;
#define glGetColorTableParameterivEXT QGLglGetColorTableParameterivEXT

typedef void (APIENTRY* pfn_glGetColorTableParameterfvEXT) (GLenum, GLenum, GLfloat*);
extern pfn_glGetColorTableParameterfvEXT QGLglGetColorTableParameterfvEXT;
#define glGetColorTableParameterfvEXT QGLglGetColorTableParameterfvEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_clip_volume_hint;
extern int QGLNumFunctions_GL_EXT_clip_volume_hint;
#ifndef GL_EXT_clip_volume_hint
#define GL_EXT_clip_volume_hint
#define QGL_DEFINED_GL_EXT_clip_volume_hint
#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT  0x80F0
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_list_priority;
extern int QGLNumFunctions_GL_SGIX_list_priority;
#ifndef GL_SGIX_list_priority
#define GL_SGIX_list_priority
#define QGL_DEFINED_GL_SGIX_list_priority
#define GL_LIST_PRIORITY_SGIX             0x8182

typedef void (APIENTRY* pfn_glGetListParameterfvSGIX) (GLuint, GLenum, GLfloat*);
extern pfn_glGetListParameterfvSGIX QGLglGetListParameterfvSGIX;
#define glGetListParameterfvSGIX QGLglGetListParameterfvSGIX

typedef void (APIENTRY* pfn_glGetListParameterivSGIX) (GLuint, GLenum, GLint*);
extern pfn_glGetListParameterivSGIX QGLglGetListParameterivSGIX;
#define glGetListParameterivSGIX QGLglGetListParameterivSGIX

typedef void (APIENTRY* pfn_glListParameterfSGIX) (GLuint, GLenum, GLfloat);
extern pfn_glListParameterfSGIX QGLglListParameterfSGIX;
#define glListParameterfSGIX QGLglListParameterfSGIX

typedef void (APIENTRY* pfn_glListParameterfvSGIX) (GLuint, GLenum, const GLfloat*);
extern pfn_glListParameterfvSGIX QGLglListParameterfvSGIX;
#define glListParameterfvSGIX QGLglListParameterfvSGIX

typedef void (APIENTRY* pfn_glListParameteriSGIX) (GLuint, GLenum, GLint);
extern pfn_glListParameteriSGIX QGLglListParameteriSGIX;
#define glListParameteriSGIX QGLglListParameteriSGIX

typedef void (APIENTRY* pfn_glListParameterivSGIX) (GLuint, GLenum, const GLint*);
extern pfn_glListParameterivSGIX QGLglListParameterivSGIX;
#define glListParameterivSGIX QGLglListParameterivSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_ir_instrument1;
extern int QGLNumFunctions_GL_SGIX_ir_instrument1;
#ifndef GL_SGIX_ir_instrument1
#define GL_SGIX_ir_instrument1
#define QGL_DEFINED_GL_SGIX_ir_instrument1
#define GL_IR_INSTRUMENT1_SGIX            0x817F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_calligraphic_fragment;
extern int QGLNumFunctions_GL_SGIX_calligraphic_fragment;
#ifndef GL_SGIX_calligraphic_fragment
#define GL_SGIX_calligraphic_fragment
#define QGL_DEFINED_GL_SGIX_calligraphic_fragment
#define GL_CALLIGRAPHIC_FRAGMENT_SGIX     0x8183
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_lod_bias;
extern int QGLNumFunctions_GL_SGIX_texture_lod_bias;
#ifndef GL_SGIX_texture_lod_bias
#define GL_SGIX_texture_lod_bias
#define QGL_DEFINED_GL_SGIX_texture_lod_bias
#define GL_TEXTURE_LOD_BIAS_S_SGIX        0x818E
#define GL_TEXTURE_LOD_BIAS_T_SGIX        0x818F
#define GL_TEXTURE_LOD_BIAS_R_SGIX        0x8190
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_shadow_ambient;
extern int QGLNumFunctions_GL_SGIX_shadow_ambient;
#ifndef GL_SGIX_shadow_ambient
#define GL_SGIX_shadow_ambient
#define QGL_DEFINED_GL_SGIX_shadow_ambient
#define GL_SHADOW_AMBIENT_SGIX            0x80BF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_index_texture;
extern int QGLNumFunctions_GL_EXT_index_texture;
#ifndef GL_EXT_index_texture
#define GL_EXT_index_texture
#define QGL_DEFINED_GL_EXT_index_texture
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_index_material;
extern int QGLNumFunctions_GL_EXT_index_material;
#ifndef GL_EXT_index_material
#define GL_EXT_index_material
#define QGL_DEFINED_GL_EXT_index_material
#define GL_INDEX_MATERIAL_EXT             0x81B8
#define GL_INDEX_MATERIAL_PARAMETER_EXT   0x81B9
#define GL_INDEX_MATERIAL_FACE_EXT        0x81BA

typedef void (APIENTRY* pfn_glIndexMaterialEXT) (GLenum, GLenum);
extern pfn_glIndexMaterialEXT QGLglIndexMaterialEXT;
#define glIndexMaterialEXT QGLglIndexMaterialEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_index_func;
extern int QGLNumFunctions_GL_EXT_index_func;
#ifndef GL_EXT_index_func
#define GL_EXT_index_func
#define QGL_DEFINED_GL_EXT_index_func
#define GL_INDEX_TEST_EXT                 0x81B5
#define GL_INDEX_TEST_FUNC_EXT            0x81B6
#define GL_INDEX_TEST_REF_EXT             0x81B7

typedef void (APIENTRY* pfn_glIndexFuncEXT) (GLenum, GLclampf);
extern pfn_glIndexFuncEXT QGLglIndexFuncEXT;
#define glIndexFuncEXT QGLglIndexFuncEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_index_array_formats;
extern int QGLNumFunctions_GL_EXT_index_array_formats;
#ifndef GL_EXT_index_array_formats
#define GL_EXT_index_array_formats
#define QGL_DEFINED_GL_EXT_index_array_formats
#define GL_IUI_V2F_EXT                    0x81AD
#define GL_IUI_V3F_EXT                    0x81AE
#define GL_IUI_N3F_V2F_EXT                0x81AF
#define GL_IUI_N3F_V3F_EXT                0x81B0
#define GL_T2F_IUI_V2F_EXT                0x81B1
#define GL_T2F_IUI_V3F_EXT                0x81B2
#define GL_T2F_IUI_N3F_V2F_EXT            0x81B3
#define GL_T2F_IUI_N3F_V3F_EXT            0x81B4
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_compiled_vertex_array;
extern int QGLNumFunctions_GL_EXT_compiled_vertex_array;
#ifndef GL_EXT_compiled_vertex_array
#define GL_EXT_compiled_vertex_array
#define QGL_DEFINED_GL_EXT_compiled_vertex_array
#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT   0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT   0x81A9

typedef void (APIENTRY* pfn_glLockArraysEXT) (GLint, GLsizei);
extern pfn_glLockArraysEXT QGLglLockArraysEXT;
#define glLockArraysEXT QGLglLockArraysEXT

typedef void (APIENTRY* pfn_glUnlockArraysEXT) (void);
extern pfn_glUnlockArraysEXT QGLglUnlockArraysEXT;
#define glUnlockArraysEXT QGLglUnlockArraysEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_cull_vertex;
extern int QGLNumFunctions_GL_EXT_cull_vertex;
#ifndef GL_EXT_cull_vertex
#define GL_EXT_cull_vertex
#define QGL_DEFINED_GL_EXT_cull_vertex
#define GL_CULL_VERTEX_EXT                  0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT     0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT  0x81AC

typedef void (APIENTRY* pfn_glCullParameterdvEXT) (GLenum, GLdouble*);
extern pfn_glCullParameterdvEXT QGLglCullParameterdvEXT;
#define glCullParameterdvEXT QGLglCullParameterdvEXT

typedef void (APIENTRY* pfn_glCullParameterfvEXT) (GLenum, GLfloat*);
extern pfn_glCullParameterfvEXT QGLglCullParameterfvEXT;
#define glCullParameterfvEXT QGLglCullParameterfvEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_ycrcb;
extern int QGLNumFunctions_GL_SGIX_ycrcb;
#ifndef GL_SGIX_ycrcb
#define GL_SGIX_ycrcb
#define QGL_DEFINED_GL_SGIX_ycrcb
#define GL_YCRCB_422_SGIX                 0x81BB
#define GL_YCRCB_444_SGIX                 0x81BC
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_fragment_lighting;
extern int QGLNumFunctions_GL_SGIX_fragment_lighting;
#ifndef GL_SGIX_fragment_lighting
#define GL_SGIX_fragment_lighting
#define QGL_DEFINED_GL_SGIX_fragment_lighting
#define GL_FRAGMENT_LIGHTING_SGIX                           0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_SGIX                     0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX                0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX           0x8403
#define GL_MAX_FRAGMENT_LIGHTS_SGIX                         0x8404
#define GL_MAX_ACTIVE_LIGHTS_SGIX                           0x8405
#define GL_CURRENT_RASTER_NORMAL_SGIX                       0x8406
#define GL_LIGHT_ENV_MODE_SGIX                              0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX           0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX               0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX                0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX   0x840B
#define GL_FRAGMENT_LIGHT0_SGIX                             0x840C
#define GL_FRAGMENT_LIGHT1_SGIX                             0x840D
#define GL_FRAGMENT_LIGHT2_SGIX                             0x840E
#define GL_FRAGMENT_LIGHT3_SGIX                             0x840F
#define GL_FRAGMENT_LIGHT4_SGIX                             0x8410
#define GL_FRAGMENT_LIGHT5_SGIX                             0x8411
#define GL_FRAGMENT_LIGHT6_SGIX                             0x8412
#define GL_FRAGMENT_LIGHT7_SGIX                             0x8413

typedef void (APIENTRY* pfn_glFragmentColorMaterialSGIX) (GLenum, GLenum);
extern pfn_glFragmentColorMaterialSGIX QGLglFragmentColorMaterialSGIX;
#define glFragmentColorMaterialSGIX QGLglFragmentColorMaterialSGIX

typedef void (APIENTRY* pfn_glFragmentLightfSGIX) (GLenum, GLenum, GLfloat);
extern pfn_glFragmentLightfSGIX QGLglFragmentLightfSGIX;
#define glFragmentLightfSGIX QGLglFragmentLightfSGIX

typedef void (APIENTRY* pfn_glFragmentLightfvSGIX) (GLenum, GLenum, const GLfloat*);
extern pfn_glFragmentLightfvSGIX QGLglFragmentLightfvSGIX;
#define glFragmentLightfvSGIX QGLglFragmentLightfvSGIX

typedef void (APIENTRY* pfn_glFragmentLightiSGIX) (GLenum, GLenum, GLint);
extern pfn_glFragmentLightiSGIX QGLglFragmentLightiSGIX;
#define glFragmentLightiSGIX QGLglFragmentLightiSGIX

typedef void (APIENTRY* pfn_glFragmentLightivSGIX) (GLenum, GLenum, const GLint*);
extern pfn_glFragmentLightivSGIX QGLglFragmentLightivSGIX;
#define glFragmentLightivSGIX QGLglFragmentLightivSGIX

typedef void (APIENTRY* pfn_glFragmentLightModelfSGIX) (GLenum, GLfloat);
extern pfn_glFragmentLightModelfSGIX QGLglFragmentLightModelfSGIX;
#define glFragmentLightModelfSGIX QGLglFragmentLightModelfSGIX

typedef void (APIENTRY* pfn_glFragmentLightModelfvSGIX) (GLenum, const GLfloat*);
extern pfn_glFragmentLightModelfvSGIX QGLglFragmentLightModelfvSGIX;
#define glFragmentLightModelfvSGIX QGLglFragmentLightModelfvSGIX

typedef void (APIENTRY* pfn_glFragmentLightModeliSGIX) (GLenum, GLint);
extern pfn_glFragmentLightModeliSGIX QGLglFragmentLightModeliSGIX;
#define glFragmentLightModeliSGIX QGLglFragmentLightModeliSGIX

typedef void (APIENTRY* pfn_glFragmentLightModelivSGIX) (GLenum, const GLint*);
extern pfn_glFragmentLightModelivSGIX QGLglFragmentLightModelivSGIX;
#define glFragmentLightModelivSGIX QGLglFragmentLightModelivSGIX

typedef void (APIENTRY* pfn_glFragmentMaterialfSGIX) (GLenum, GLenum, GLfloat);
extern pfn_glFragmentMaterialfSGIX QGLglFragmentMaterialfSGIX;
#define glFragmentMaterialfSGIX QGLglFragmentMaterialfSGIX

typedef void (APIENTRY* pfn_glFragmentMaterialfvSGIX) (GLenum, GLenum, const GLfloat*);
extern pfn_glFragmentMaterialfvSGIX QGLglFragmentMaterialfvSGIX;
#define glFragmentMaterialfvSGIX QGLglFragmentMaterialfvSGIX

typedef void (APIENTRY* pfn_glFragmentMaterialiSGIX) (GLenum, GLenum, GLint);
extern pfn_glFragmentMaterialiSGIX QGLglFragmentMaterialiSGIX;
#define glFragmentMaterialiSGIX QGLglFragmentMaterialiSGIX

typedef void (APIENTRY* pfn_glFragmentMaterialivSGIX) (GLenum, GLenum, const GLint*);
extern pfn_glFragmentMaterialivSGIX QGLglFragmentMaterialivSGIX;
#define glFragmentMaterialivSGIX QGLglFragmentMaterialivSGIX

typedef void (APIENTRY* pfn_glGetFragmentLightfvSGIX) (GLenum, GLenum, GLfloat*);
extern pfn_glGetFragmentLightfvSGIX QGLglGetFragmentLightfvSGIX;
#define glGetFragmentLightfvSGIX QGLglGetFragmentLightfvSGIX

typedef void (APIENTRY* pfn_glGetFragmentLightivSGIX) (GLenum, GLenum, GLint*);
extern pfn_glGetFragmentLightivSGIX QGLglGetFragmentLightivSGIX;
#define glGetFragmentLightivSGIX QGLglGetFragmentLightivSGIX

typedef void (APIENTRY* pfn_glGetFragmentMaterialfvSGIX) (GLenum, GLenum, GLfloat*);
extern pfn_glGetFragmentMaterialfvSGIX QGLglGetFragmentMaterialfvSGIX;
#define glGetFragmentMaterialfvSGIX QGLglGetFragmentMaterialfvSGIX

typedef void (APIENTRY* pfn_glGetFragmentMaterialivSGIX) (GLenum, GLenum, GLint*);
extern pfn_glGetFragmentMaterialivSGIX QGLglGetFragmentMaterialivSGIX;
#define glGetFragmentMaterialivSGIX QGLglGetFragmentMaterialivSGIX

typedef void (APIENTRY* pfn_glLightEnviSGIX) (GLenum, GLint);
extern pfn_glLightEnviSGIX QGLglLightEnviSGIX;
#define glLightEnviSGIX QGLglLightEnviSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_IBM_rasterpos_clip;
extern int QGLNumFunctions_GL_IBM_rasterpos_clip;
#ifndef GL_IBM_rasterpos_clip
#define GL_IBM_rasterpos_clip
#define QGL_DEFINED_GL_IBM_rasterpos_clip
#define GL_RASTER_POSITION_UNCLIPPED_IBM  0x19262
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_HP_texture_lighting;
extern int QGLNumFunctions_GL_HP_texture_lighting;
#ifndef GL_HP_texture_lighting
#define GL_HP_texture_lighting
#define QGL_DEFINED_GL_HP_texture_lighting
#define GL_TEXTURE_LIGHTING_MODE_HP       0x8167
#define GL_TEXTURE_POST_SPECULAR_HP       0x8168
#define GL_TEXTURE_PRE_SPECULAR_HP        0x8169
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_draw_range_elements;
extern int QGLNumFunctions_GL_EXT_draw_range_elements;
#ifndef GL_EXT_draw_range_elements
#define GL_EXT_draw_range_elements
#define QGL_DEFINED_GL_EXT_draw_range_elements
#define GL_MAX_ELEMENTS_VERTICES_EXT      0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT       0x80E9

typedef void (APIENTRY* pfn_glDrawRangeElementsEXT) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*);
extern pfn_glDrawRangeElementsEXT QGLglDrawRangeElementsEXT;
#define glDrawRangeElementsEXT QGLglDrawRangeElementsEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_WIN_phong_shading;
extern int QGLNumFunctions_GL_WIN_phong_shading;
#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading
#define QGL_DEFINED_GL_WIN_phong_shading
#define GL_PHONG_WIN                      0x80EA
#define GL_PHONG_HINT_WIN                 0x80EB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_WIN_specular_fog;
extern int QGLNumFunctions_GL_WIN_specular_fog;
#ifndef GL_WIN_specular_fog
#define GL_WIN_specular_fog
#define QGL_DEFINED_GL_WIN_specular_fog
#define GL_FOG_SPECULAR_TEXTURE_WIN       0x80EC
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_light_texture;
extern int QGLNumFunctions_GL_EXT_light_texture;
#ifndef GL_EXT_light_texture
#define GL_EXT_light_texture
#define QGL_DEFINED_GL_EXT_light_texture
#define GL_FRAGMENT_MATERIAL_EXT          0x8349
#define GL_FRAGMENT_NORMAL_EXT            0x834A
#define GL_FRAGMENT_COLOR_EXT             0x834C
#define GL_ATTENUATION_EXT                0x834D
#define GL_SHADOW_ATTENUATION_EXT         0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT   0x834F
#define GL_TEXTURE_LIGHT_EXT              0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT      0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352
/* reuse GL_FRAGMENT_DEPTH_EXT */

typedef void (APIENTRY* pfn_glApplyTextureEXT) (GLenum);
extern pfn_glApplyTextureEXT QGLglApplyTextureEXT;
#define glApplyTextureEXT QGLglApplyTextureEXT

typedef void (APIENTRY* pfn_glTextureLightEXT) (GLenum);
extern pfn_glTextureLightEXT QGLglTextureLightEXT;
#define glTextureLightEXT QGLglTextureLightEXT

typedef void (APIENTRY* pfn_glTextureMaterialEXT) (GLenum, GLenum);
extern pfn_glTextureMaterialEXT QGLglTextureMaterialEXT;
#define glTextureMaterialEXT QGLglTextureMaterialEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_blend_alpha_minmax;
extern int QGLNumFunctions_GL_SGIX_blend_alpha_minmax;
#ifndef GL_SGIX_blend_alpha_minmax
#define GL_SGIX_blend_alpha_minmax
#define QGL_DEFINED_GL_SGIX_blend_alpha_minmax
#define GL_ALPHA_MIN_SGIX                 0x8320
#define GL_ALPHA_MAX_SGIX                 0x8321
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_impact_pixel_texture;
extern int QGLNumFunctions_GL_SGIX_impact_pixel_texture;
#ifndef GL_SGIX_impact_pixel_texture
#define GL_SGIX_impact_pixel_texture
#define QGL_DEFINED_GL_SGIX_impact_pixel_texture
#define GL_PIXEL_TEX_GEN_Q_CEILING_SGIX         0x8184
#define GL_PIXEL_TEX_GEN_Q_ROUND_SGIX           0x8185
#define GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX           0x8186
#define GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX     0x8187
#define GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX  0x8188
#define GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX          0x8189
#define GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX          0x818A
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_bgra;
extern int QGLNumFunctions_GL_EXT_bgra;
#ifndef GL_EXT_bgra
#define GL_EXT_bgra
#define QGL_DEFINED_GL_EXT_bgra
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_async;
extern int QGLNumFunctions_GL_SGIX_async;
#ifndef GL_SGIX_async
#define GL_SGIX_async
#define QGL_DEFINED_GL_SGIX_async
#define GL_ASYNC_MARKER_SGIX              0x8329

typedef void (APIENTRY* pfn_glAsyncMarkerSGIX) (GLuint);
extern pfn_glAsyncMarkerSGIX QGLglAsyncMarkerSGIX;
#define glAsyncMarkerSGIX QGLglAsyncMarkerSGIX

typedef GLint (APIENTRY* pfn_glFinishAsyncSGIX) (GLuint*);
extern pfn_glFinishAsyncSGIX QGLglFinishAsyncSGIX;
#define glFinishAsyncSGIX QGLglFinishAsyncSGIX

typedef GLint (APIENTRY* pfn_glPollAsyncSGIX) (GLuint*);
extern pfn_glPollAsyncSGIX QGLglPollAsyncSGIX;
#define glPollAsyncSGIX QGLglPollAsyncSGIX

typedef GLuint (APIENTRY* pfn_glGenAsyncMarkersSGIX) (GLsizei);
extern pfn_glGenAsyncMarkersSGIX QGLglGenAsyncMarkersSGIX;
#define glGenAsyncMarkersSGIX QGLglGenAsyncMarkersSGIX

typedef void (APIENTRY* pfn_glDeleteAsyncMarkersSGIX) (GLuint, GLsizei);
extern pfn_glDeleteAsyncMarkersSGIX QGLglDeleteAsyncMarkersSGIX;
#define glDeleteAsyncMarkersSGIX QGLglDeleteAsyncMarkersSGIX

typedef GLboolean (APIENTRY* pfn_glIsAsyncMarkerSGIX) (GLuint);
extern pfn_glIsAsyncMarkerSGIX QGLglIsAsyncMarkerSGIX;
#define glIsAsyncMarkerSGIX QGLglIsAsyncMarkerSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_async_pixel;
extern int QGLNumFunctions_GL_SGIX_async_pixel;
#ifndef GL_SGIX_async_pixel
#define GL_SGIX_async_pixel
#define QGL_DEFINED_GL_SGIX_async_pixel
#define GL_ASYNC_TEX_IMAGE_SGIX           0x835C
#define GL_ASYNC_DRAW_PIXELS_SGIX         0x835D
#define GL_ASYNC_READ_PIXELS_SGIX         0x835E
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX       0x835F
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX     0x8360
#define GL_MAX_ASYNC_READ_PIXELS_SGIX     0x8361
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_async_histogram;
extern int QGLNumFunctions_GL_SGIX_async_histogram;
#ifndef GL_SGIX_async_histogram
#define GL_SGIX_async_histogram
#define QGL_DEFINED_GL_SGIX_async_histogram
#define GL_ASYNC_HISTOGRAM_SGIX           0x832C
#define GL_MAX_ASYNC_HISTOGRAM_SGIX       0x832D
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INTEL_texture_scissor;
extern int QGLNumFunctions_GL_INTEL_texture_scissor;
#ifndef GL_INTEL_texture_scissor
#define GL_INTEL_texture_scissor
#define QGL_DEFINED_GL_INTEL_texture_scissor
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INTEL_parallel_arrays;
extern int QGLNumFunctions_GL_INTEL_parallel_arrays;
#ifndef GL_INTEL_parallel_arrays
#define GL_INTEL_parallel_arrays
#define QGL_DEFINED_GL_INTEL_parallel_arrays
#define GL_PARALLEL_ARRAYS_INTEL                        0x83F4
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL         0x83F5
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL         0x83F6
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL          0x83F7
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL  0x83F8

typedef void (APIENTRY* pfn_glVertexPointervINTEL) (GLint, GLenum, const GLvoid**);
extern pfn_glVertexPointervINTEL QGLglVertexPointervINTEL;
#define glVertexPointervINTEL QGLglVertexPointervINTEL

typedef void (APIENTRY* pfn_glNormalPointervINTEL) (GLenum, const GLvoid**);
extern pfn_glNormalPointervINTEL QGLglNormalPointervINTEL;
#define glNormalPointervINTEL QGLglNormalPointervINTEL

typedef void (APIENTRY* pfn_glColorPointervINTEL) (GLint, GLenum, const GLvoid**);
extern pfn_glColorPointervINTEL QGLglColorPointervINTEL;
#define glColorPointervINTEL QGLglColorPointervINTEL

typedef void (APIENTRY* pfn_glTexCoordPointervINTEL) (GLint, GLenum, const GLvoid**);
extern pfn_glTexCoordPointervINTEL QGLglTexCoordPointervINTEL;
#define glTexCoordPointervINTEL QGLglTexCoordPointervINTEL
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_HP_occlusion_test;
extern int QGLNumFunctions_GL_HP_occlusion_test;
#ifndef GL_HP_occlusion_test
#define GL_HP_occlusion_test
#define QGL_DEFINED_GL_HP_occlusion_test
#define GL_OCCLUSION_TEST_HP              0x8165
#define GL_OCCLUSION_TEST_RESULT_HP       0x8166
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_pixel_transform;
extern int QGLNumFunctions_GL_EXT_pixel_transform;
#ifndef GL_EXT_pixel_transform
#define GL_EXT_pixel_transform
#define QGL_DEFINED_GL_EXT_pixel_transform
#define GL_PIXEL_TRANSFORM_2D_EXT         0x8330
#define GL_PIXEL_MAG_FILTER_EXT           0x8331
#define GL_PIXEL_MIN_FILTER_EXT           0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT         0x8333
#define GL_CUBIC_EXT                      0x8334
#define GL_AVERAGE_EXT                    0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT  0x8338

typedef void (APIENTRY* pfn_glPixelTransformParameteriEXT) (GLenum, GLenum, GLint);
extern pfn_glPixelTransformParameteriEXT QGLglPixelTransformParameteriEXT;
#define glPixelTransformParameteriEXT QGLglPixelTransformParameteriEXT

typedef void (APIENTRY* pfn_glPixelTransformParameterfEXT) (GLenum, GLenum, GLfloat);
extern pfn_glPixelTransformParameterfEXT QGLglPixelTransformParameterfEXT;
#define glPixelTransformParameterfEXT QGLglPixelTransformParameterfEXT

typedef void (APIENTRY* pfn_glPixelTransformParameterivEXT) (GLenum, GLenum, const GLint*);
extern pfn_glPixelTransformParameterivEXT QGLglPixelTransformParameterivEXT;
#define glPixelTransformParameterivEXT QGLglPixelTransformParameterivEXT

typedef void (APIENTRY* pfn_glPixelTransformParameterfvEXT) (GLenum, GLenum, const GLfloat*);
extern pfn_glPixelTransformParameterfvEXT QGLglPixelTransformParameterfvEXT;
#define glPixelTransformParameterfvEXT QGLglPixelTransformParameterfvEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_pixel_transform_color_table;
extern int QGLNumFunctions_GL_EXT_pixel_transform_color_table;
#ifndef GL_EXT_pixel_transform_color_table
#define GL_EXT_pixel_transform_color_table
#define QGL_DEFINED_GL_EXT_pixel_transform_color_table
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_shared_texture_palette;
extern int QGLNumFunctions_GL_EXT_shared_texture_palette;
#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette
#define QGL_DEFINED_GL_EXT_shared_texture_palette
#define GL_SHARED_TEXTURE_PALETTE_EXT     0x81FB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_separate_specular_color;
extern int QGLNumFunctions_GL_EXT_separate_specular_color;
#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color
#define QGL_DEFINED_GL_EXT_separate_specular_color
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT  0x81F8
#define GL_SINGLE_COLOR_EXT               0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT    0x81FA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_secondary_color;
extern int QGLNumFunctions_GL_EXT_secondary_color;
#ifndef GL_EXT_secondary_color
#define GL_EXT_secondary_color
#define QGL_DEFINED_GL_EXT_secondary_color
#define GL_COLOR_SUM_EXT                        0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT          0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT       0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT       0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT     0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT    0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT            0x845E

typedef void (APIENTRY* pfn_glSecondaryColor3bEXT) (GLbyte, GLbyte, GLbyte);
extern pfn_glSecondaryColor3bEXT QGLglSecondaryColor3bEXT;
#define glSecondaryColor3bEXT QGLglSecondaryColor3bEXT

typedef void (APIENTRY* pfn_glSecondaryColor3bvEXT) (const GLbyte*);
extern pfn_glSecondaryColor3bvEXT QGLglSecondaryColor3bvEXT;
#define glSecondaryColor3bvEXT QGLglSecondaryColor3bvEXT

typedef void (APIENTRY* pfn_glSecondaryColor3dEXT) (GLdouble, GLdouble, GLdouble);
extern pfn_glSecondaryColor3dEXT QGLglSecondaryColor3dEXT;
#define glSecondaryColor3dEXT QGLglSecondaryColor3dEXT

typedef void (APIENTRY* pfn_glSecondaryColor3dvEXT) (const GLdouble*);
extern pfn_glSecondaryColor3dvEXT QGLglSecondaryColor3dvEXT;
#define glSecondaryColor3dvEXT QGLglSecondaryColor3dvEXT

typedef void (APIENTRY* pfn_glSecondaryColor3fEXT) (GLfloat, GLfloat, GLfloat);
extern pfn_glSecondaryColor3fEXT QGLglSecondaryColor3fEXT;
#define glSecondaryColor3fEXT QGLglSecondaryColor3fEXT

typedef void (APIENTRY* pfn_glSecondaryColor3fvEXT) (const GLfloat*);
extern pfn_glSecondaryColor3fvEXT QGLglSecondaryColor3fvEXT;
#define glSecondaryColor3fvEXT QGLglSecondaryColor3fvEXT

typedef void (APIENTRY* pfn_glSecondaryColor3iEXT) (GLint, GLint, GLint);
extern pfn_glSecondaryColor3iEXT QGLglSecondaryColor3iEXT;
#define glSecondaryColor3iEXT QGLglSecondaryColor3iEXT

typedef void (APIENTRY* pfn_glSecondaryColor3ivEXT) (const GLint*);
extern pfn_glSecondaryColor3ivEXT QGLglSecondaryColor3ivEXT;
#define glSecondaryColor3ivEXT QGLglSecondaryColor3ivEXT

typedef void (APIENTRY* pfn_glSecondaryColor3sEXT) (GLshort, GLshort, GLshort);
extern pfn_glSecondaryColor3sEXT QGLglSecondaryColor3sEXT;
#define glSecondaryColor3sEXT QGLglSecondaryColor3sEXT

typedef void (APIENTRY* pfn_glSecondaryColor3svEXT) (const GLshort*);
extern pfn_glSecondaryColor3svEXT QGLglSecondaryColor3svEXT;
#define glSecondaryColor3svEXT QGLglSecondaryColor3svEXT

typedef void (APIENTRY* pfn_glSecondaryColor3ubEXT) (GLubyte, GLubyte, GLubyte);
extern pfn_glSecondaryColor3ubEXT QGLglSecondaryColor3ubEXT;
#define glSecondaryColor3ubEXT QGLglSecondaryColor3ubEXT

typedef void (APIENTRY* pfn_glSecondaryColor3ubvEXT) (const GLubyte*);
extern pfn_glSecondaryColor3ubvEXT QGLglSecondaryColor3ubvEXT;
#define glSecondaryColor3ubvEXT QGLglSecondaryColor3ubvEXT

typedef void (APIENTRY* pfn_glSecondaryColor3uiEXT) (GLuint, GLuint, GLuint);
extern pfn_glSecondaryColor3uiEXT QGLglSecondaryColor3uiEXT;
#define glSecondaryColor3uiEXT QGLglSecondaryColor3uiEXT

typedef void (APIENTRY* pfn_glSecondaryColor3uivEXT) (const GLuint*);
extern pfn_glSecondaryColor3uivEXT QGLglSecondaryColor3uivEXT;
#define glSecondaryColor3uivEXT QGLglSecondaryColor3uivEXT

typedef void (APIENTRY* pfn_glSecondaryColor3usEXT) (GLushort, GLushort, GLushort);
extern pfn_glSecondaryColor3usEXT QGLglSecondaryColor3usEXT;
#define glSecondaryColor3usEXT QGLglSecondaryColor3usEXT

typedef void (APIENTRY* pfn_glSecondaryColor3usvEXT) (const GLushort*);
extern pfn_glSecondaryColor3usvEXT QGLglSecondaryColor3usvEXT;
#define glSecondaryColor3usvEXT QGLglSecondaryColor3usvEXT

typedef void (APIENTRY* pfn_glSecondaryColorPointerEXT) (GLint, GLenum, GLsizei, const GLvoid*);
extern pfn_glSecondaryColorPointerEXT QGLglSecondaryColorPointerEXT;
#define glSecondaryColorPointerEXT QGLglSecondaryColorPointerEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_perturb_normal;
extern int QGLNumFunctions_GL_EXT_texture_perturb_normal;
#ifndef GL_EXT_texture_perturb_normal
#define GL_EXT_texture_perturb_normal
#define QGL_DEFINED_GL_EXT_texture_perturb_normal
#define GL_PERTURB_EXT                    0x85AE
#define GL_TEXTURE_NORMAL_EXT             0x85AF

typedef void (APIENTRY* pfn_glTextureNormalEXT) (GLenum);
extern pfn_glTextureNormalEXT QGLglTextureNormalEXT;
#define glTextureNormalEXT QGLglTextureNormalEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_multi_draw_arrays;
extern int QGLNumFunctions_GL_EXT_multi_draw_arrays;
#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays
#define QGL_DEFINED_GL_EXT_multi_draw_arrays

typedef void (APIENTRY* pfn_glMultiDrawArraysEXT) (GLenum, GLint*, GLsizei*, GLsizei);
extern pfn_glMultiDrawArraysEXT QGLglMultiDrawArraysEXT;
#define glMultiDrawArraysEXT QGLglMultiDrawArraysEXT

typedef void (APIENTRY* pfn_glMultiDrawElementsEXT) (GLenum, const GLsizei*, GLenum, const GLvoid**, GLsizei);
extern pfn_glMultiDrawElementsEXT QGLglMultiDrawElementsEXT;
#define glMultiDrawElementsEXT QGLglMultiDrawElementsEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_fog_coord;
extern int QGLNumFunctions_GL_EXT_fog_coord;
#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord
#define QGL_DEFINED_GL_EXT_fog_coord
#define GL_FOG_COORDINATE_SOURCE_EXT        0x8450
#define GL_FOG_COORDINATE_EXT               0x8451
#define GL_FRAGMENT_DEPTH_EXT               0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT       0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT    0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT  0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT         0x8457

typedef void (APIENTRY* pfn_glFogCoordfEXT) (GLfloat);
extern pfn_glFogCoordfEXT QGLglFogCoordfEXT;
#define glFogCoordfEXT QGLglFogCoordfEXT

typedef void (APIENTRY* pfn_glFogCoordfvEXT) (const GLfloat*);
extern pfn_glFogCoordfvEXT QGLglFogCoordfvEXT;
#define glFogCoordfvEXT QGLglFogCoordfvEXT

typedef void (APIENTRY* pfn_glFogCoorddEXT) (GLdouble);
extern pfn_glFogCoorddEXT QGLglFogCoorddEXT;
#define glFogCoorddEXT QGLglFogCoorddEXT

typedef void (APIENTRY* pfn_glFogCoorddvEXT) (const GLdouble*);
extern pfn_glFogCoorddvEXT QGLglFogCoorddvEXT;
#define glFogCoorddvEXT QGLglFogCoorddvEXT

typedef void (APIENTRY* pfn_glFogCoordPointerEXT) (GLenum, GLsizei, const GLvoid*);
extern pfn_glFogCoordPointerEXT QGLglFogCoordPointerEXT;
#define glFogCoordPointerEXT QGLglFogCoordPointerEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_REND_screen_coordinates;
extern int QGLNumFunctions_GL_REND_screen_coordinates;
#ifndef GL_REND_screen_coordinates
#define GL_REND_screen_coordinates
#define QGL_DEFINED_GL_REND_screen_coordinates
#define GL_SCREEN_COORDINATES_REND        0x8490
#define GL_INVERTED_SCREEN_W_REND         0x8491
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_coordinate_frame;
extern int QGLNumFunctions_GL_EXT_coordinate_frame;
#ifndef GL_EXT_coordinate_frame
#define GL_EXT_coordinate_frame
#define QGL_DEFINED_GL_EXT_coordinate_frame
#define GL_TANGENT_ARRAY_EXT              0x8439
#define GL_BINORMAL_ARRAY_EXT             0x843A
#define GL_CURRENT_TANGENT_EXT            0x843B
#define GL_CURRENT_BINORMAL_EXT           0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT         0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT       0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT        0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT      0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT      0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT     0x8443
#define GL_MAP1_TANGENT_EXT               0x8444
#define GL_MAP2_TANGENT_EXT               0x8445
#define GL_MAP1_BINORMAL_EXT              0x8446
#define GL_MAP2_BINORMAL_EXT              0x8447

typedef void (APIENTRY* pfn_glTangent3bEXT) (GLbyte, GLbyte, GLbyte);
extern pfn_glTangent3bEXT QGLglTangent3bEXT;
#define glTangent3bEXT QGLglTangent3bEXT

typedef void (APIENTRY* pfn_glTangent3bvEXT) (const GLbyte*);
extern pfn_glTangent3bvEXT QGLglTangent3bvEXT;
#define glTangent3bvEXT QGLglTangent3bvEXT

typedef void (APIENTRY* pfn_glTangent3dEXT) (GLdouble, GLdouble, GLdouble);
extern pfn_glTangent3dEXT QGLglTangent3dEXT;
#define glTangent3dEXT QGLglTangent3dEXT

typedef void (APIENTRY* pfn_glTangent3dvEXT) (const GLdouble*);
extern pfn_glTangent3dvEXT QGLglTangent3dvEXT;
#define glTangent3dvEXT QGLglTangent3dvEXT

typedef void (APIENTRY* pfn_glTangent3fEXT) (GLfloat, GLfloat, GLfloat);
extern pfn_glTangent3fEXT QGLglTangent3fEXT;
#define glTangent3fEXT QGLglTangent3fEXT

typedef void (APIENTRY* pfn_glTangent3fvEXT) (const GLfloat*);
extern pfn_glTangent3fvEXT QGLglTangent3fvEXT;
#define glTangent3fvEXT QGLglTangent3fvEXT

typedef void (APIENTRY* pfn_glTangent3iEXT) (GLint, GLint, GLint);
extern pfn_glTangent3iEXT QGLglTangent3iEXT;
#define glTangent3iEXT QGLglTangent3iEXT

typedef void (APIENTRY* pfn_glTangent3ivEXT) (const GLint*);
extern pfn_glTangent3ivEXT QGLglTangent3ivEXT;
#define glTangent3ivEXT QGLglTangent3ivEXT

typedef void (APIENTRY* pfn_glTangent3sEXT) (GLshort, GLshort, GLshort);
extern pfn_glTangent3sEXT QGLglTangent3sEXT;
#define glTangent3sEXT QGLglTangent3sEXT

typedef void (APIENTRY* pfn_glTangent3svEXT) (const GLshort*);
extern pfn_glTangent3svEXT QGLglTangent3svEXT;
#define glTangent3svEXT QGLglTangent3svEXT

typedef void (APIENTRY* pfn_glBinormal3bEXT) (GLbyte, GLbyte, GLbyte);
extern pfn_glBinormal3bEXT QGLglBinormal3bEXT;
#define glBinormal3bEXT QGLglBinormal3bEXT

typedef void (APIENTRY* pfn_glBinormal3bvEXT) (const GLbyte*);
extern pfn_glBinormal3bvEXT QGLglBinormal3bvEXT;
#define glBinormal3bvEXT QGLglBinormal3bvEXT

typedef void (APIENTRY* pfn_glBinormal3dEXT) (GLdouble, GLdouble, GLdouble);
extern pfn_glBinormal3dEXT QGLglBinormal3dEXT;
#define glBinormal3dEXT QGLglBinormal3dEXT

typedef void (APIENTRY* pfn_glBinormal3dvEXT) (const GLdouble*);
extern pfn_glBinormal3dvEXT QGLglBinormal3dvEXT;
#define glBinormal3dvEXT QGLglBinormal3dvEXT

typedef void (APIENTRY* pfn_glBinormal3fEXT) (GLfloat, GLfloat, GLfloat);
extern pfn_glBinormal3fEXT QGLglBinormal3fEXT;
#define glBinormal3fEXT QGLglBinormal3fEXT

typedef void (APIENTRY* pfn_glBinormal3fvEXT) (const GLfloat*);
extern pfn_glBinormal3fvEXT QGLglBinormal3fvEXT;
#define glBinormal3fvEXT QGLglBinormal3fvEXT

typedef void (APIENTRY* pfn_glBinormal3iEXT) (GLint, GLint, GLint);
extern pfn_glBinormal3iEXT QGLglBinormal3iEXT;
#define glBinormal3iEXT QGLglBinormal3iEXT

typedef void (APIENTRY* pfn_glBinormal3ivEXT) (const GLint*);
extern pfn_glBinormal3ivEXT QGLglBinormal3ivEXT;
#define glBinormal3ivEXT QGLglBinormal3ivEXT

typedef void (APIENTRY* pfn_glBinormal3sEXT) (GLshort, GLshort, GLshort);
extern pfn_glBinormal3sEXT QGLglBinormal3sEXT;
#define glBinormal3sEXT QGLglBinormal3sEXT

typedef void (APIENTRY* pfn_glBinormal3svEXT) (const GLshort*);
extern pfn_glBinormal3svEXT QGLglBinormal3svEXT;
#define glBinormal3svEXT QGLglBinormal3svEXT

typedef void (APIENTRY* pfn_glTangentPointerEXT) (GLenum, GLsizei, const GLvoid*);
extern pfn_glTangentPointerEXT QGLglTangentPointerEXT;
#define glTangentPointerEXT QGLglTangentPointerEXT

typedef void (APIENTRY* pfn_glBinormalPointerEXT) (GLenum, GLsizei, const GLvoid*);
extern pfn_glBinormalPointerEXT QGLglBinormalPointerEXT;
#define glBinormalPointerEXT QGLglBinormalPointerEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_env_combine;
extern int QGLNumFunctions_GL_EXT_texture_env_combine;
#ifndef GL_EXT_texture_env_combine
#define GL_EXT_texture_env_combine
#define QGL_DEFINED_GL_EXT_texture_env_combine
#define GL_COMBINE_EXT                    0x8570
#define GL_COMBINE_RGB_EXT                0x8571
#define GL_COMBINE_ALPHA_EXT              0x8572
#define GL_RGB_SCALE_EXT                  0x8573
#define GL_ADD_SIGNED_EXT                 0x8574
#define GL_INTERPOLATE_EXT                0x8575
#define GL_CONSTANT_EXT                   0x8576
#define GL_PRIMARY_COLOR_EXT              0x8577
#define GL_PREVIOUS_EXT                   0x8578
#define GL_SOURCE0_RGB_EXT                0x8580
#define GL_SOURCE1_RGB_EXT                0x8581
#define GL_SOURCE2_RGB_EXT                0x8582
#define GL_SOURCE0_ALPHA_EXT              0x8588
#define GL_SOURCE1_ALPHA_EXT              0x8589
#define GL_SOURCE2_ALPHA_EXT              0x858A
#define GL_OPERAND0_RGB_EXT               0x8590
#define GL_OPERAND1_RGB_EXT               0x8591
#define GL_OPERAND2_RGB_EXT               0x8592
#define GL_OPERAND0_ALPHA_EXT             0x8598
#define GL_OPERAND1_ALPHA_EXT             0x8599
#define GL_OPERAND2_ALPHA_EXT             0x859A
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_specular_vector;
extern int QGLNumFunctions_GL_APPLE_specular_vector;
#ifndef GL_APPLE_specular_vector
#define GL_APPLE_specular_vector
#define QGL_DEFINED_GL_APPLE_specular_vector
#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE 0x85B0
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_transform_hint;
extern int QGLNumFunctions_GL_APPLE_transform_hint;
#ifndef GL_APPLE_transform_hint
#define GL_APPLE_transform_hint
#define QGL_DEFINED_GL_APPLE_transform_hint
#define GL_TRANSFORM_HINT_APPLE           0x85B1
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_fog_scale;
extern int QGLNumFunctions_GL_SGIX_fog_scale;
#ifndef GL_SGIX_fog_scale
#define GL_SGIX_fog_scale
#define QGL_DEFINED_GL_SGIX_fog_scale
#define GL_FOG_SCALE_SGIX                 0x81FC
#define GL_FOG_SCALE_VALUE_SGIX           0x81FD
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUNX_constant_data;
extern int QGLNumFunctions_GL_SUNX_constant_data;
#ifndef GL_SUNX_constant_data
#define GL_SUNX_constant_data
#define QGL_DEFINED_GL_SUNX_constant_data
#define GL_UNPACK_CONSTANT_DATA_SUNX      0x81D5
#define GL_TEXTURE_CONSTANT_DATA_SUNX     0x81D6

typedef void (APIENTRY* pfn_glFinishTextureSUNX) (void);
extern pfn_glFinishTextureSUNX QGLglFinishTextureSUNX;
#define glFinishTextureSUNX QGLglFinishTextureSUNX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_global_alpha;
extern int QGLNumFunctions_GL_SUN_global_alpha;
#ifndef GL_SUN_global_alpha
#define GL_SUN_global_alpha
#define QGL_DEFINED_GL_SUN_global_alpha
#define GL_GLOBAL_ALPHA_SUN               0x81D9
#define GL_GLOBAL_ALPHA_FACTOR_SUN        0x81DA

typedef void (APIENTRY* pfn_glGlobalAlphaFactorbSUN) (GLbyte);
extern pfn_glGlobalAlphaFactorbSUN QGLglGlobalAlphaFactorbSUN;
#define glGlobalAlphaFactorbSUN QGLglGlobalAlphaFactorbSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactorsSUN) (GLshort);
extern pfn_glGlobalAlphaFactorsSUN QGLglGlobalAlphaFactorsSUN;
#define glGlobalAlphaFactorsSUN QGLglGlobalAlphaFactorsSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactoriSUN) (GLint);
extern pfn_glGlobalAlphaFactoriSUN QGLglGlobalAlphaFactoriSUN;
#define glGlobalAlphaFactoriSUN QGLglGlobalAlphaFactoriSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactorfSUN) (GLfloat);
extern pfn_glGlobalAlphaFactorfSUN QGLglGlobalAlphaFactorfSUN;
#define glGlobalAlphaFactorfSUN QGLglGlobalAlphaFactorfSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactordSUN) (GLdouble);
extern pfn_glGlobalAlphaFactordSUN QGLglGlobalAlphaFactordSUN;
#define glGlobalAlphaFactordSUN QGLglGlobalAlphaFactordSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactorubSUN) (GLubyte);
extern pfn_glGlobalAlphaFactorubSUN QGLglGlobalAlphaFactorubSUN;
#define glGlobalAlphaFactorubSUN QGLglGlobalAlphaFactorubSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactorusSUN) (GLushort);
extern pfn_glGlobalAlphaFactorusSUN QGLglGlobalAlphaFactorusSUN;
#define glGlobalAlphaFactorusSUN QGLglGlobalAlphaFactorusSUN

typedef void (APIENTRY* pfn_glGlobalAlphaFactoruiSUN) (GLuint);
extern pfn_glGlobalAlphaFactoruiSUN QGLglGlobalAlphaFactoruiSUN;
#define glGlobalAlphaFactoruiSUN QGLglGlobalAlphaFactoruiSUN
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_triangle_list;
extern int QGLNumFunctions_GL_SUN_triangle_list;
#ifndef GL_SUN_triangle_list
#define GL_SUN_triangle_list
#define QGL_DEFINED_GL_SUN_triangle_list
#define GL_RESTART_SUN                          0x0001
#define GL_REPLACE_MIDDLE_SUN                   0x0002
#define GL_REPLACE_OLDEST_SUN                   0x0003
#define GL_TRIANGLE_LIST_SUN                    0x81D7
#define GL_REPLACEMENT_CODE_SUN                 0x81D8
#define GL_REPLACEMENT_CODE_ARRAY_SUN           0x85C0
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN      0x85C1
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN    0x85C2
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN   0x85C3
#define GL_R1UI_V3F_SUN                         0x85C4
#define GL_R1UI_C4UB_V3F_SUN                    0x85C5
#define GL_R1UI_C3F_V3F_SUN                     0x85C6
#define GL_R1UI_N3F_V3F_SUN                     0x85C7
#define GL_R1UI_C4F_N3F_V3F_SUN                 0x85C8
#define GL_R1UI_T2F_V3F_SUN                     0x85C9
#define GL_R1UI_T2F_N3F_V3F_SUN                 0x85CA
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN             0x85CB

typedef void (APIENTRY* pfn_glReplacementCodeuiSUN) (GLuint);
extern pfn_glReplacementCodeuiSUN QGLglReplacementCodeuiSUN;
#define glReplacementCodeuiSUN QGLglReplacementCodeuiSUN

typedef void (APIENTRY* pfn_glReplacementCodeusSUN) (GLushort);
extern pfn_glReplacementCodeusSUN QGLglReplacementCodeusSUN;
#define glReplacementCodeusSUN QGLglReplacementCodeusSUN

typedef void (APIENTRY* pfn_glReplacementCodeubSUN) (GLubyte);
extern pfn_glReplacementCodeubSUN QGLglReplacementCodeubSUN;
#define glReplacementCodeubSUN QGLglReplacementCodeubSUN

typedef void (APIENTRY* pfn_glReplacementCodeuivSUN) (const GLuint*);
extern pfn_glReplacementCodeuivSUN QGLglReplacementCodeuivSUN;
#define glReplacementCodeuivSUN QGLglReplacementCodeuivSUN

typedef void (APIENTRY* pfn_glReplacementCodeusvSUN) (const GLushort*);
extern pfn_glReplacementCodeusvSUN QGLglReplacementCodeusvSUN;
#define glReplacementCodeusvSUN QGLglReplacementCodeusvSUN

typedef void (APIENTRY* pfn_glReplacementCodeubvSUN) (const GLubyte*);
extern pfn_glReplacementCodeubvSUN QGLglReplacementCodeubvSUN;
#define glReplacementCodeubvSUN QGLglReplacementCodeubvSUN

typedef void (APIENTRY* pfn_glReplacementCodePointerSUN) (GLenum, GLsizei, const GLvoid**);
extern pfn_glReplacementCodePointerSUN QGLglReplacementCodePointerSUN;
#define glReplacementCodePointerSUN QGLglReplacementCodePointerSUN
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_vertex;
extern int QGLNumFunctions_GL_SUN_vertex;
#ifndef GL_SUN_vertex
#define GL_SUN_vertex
#define QGL_DEFINED_GL_SUN_vertex

typedef void (APIENTRY* pfn_glColor4ubVertex2fSUN) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat);
extern pfn_glColor4ubVertex2fSUN QGLglColor4ubVertex2fSUN;
#define glColor4ubVertex2fSUN QGLglColor4ubVertex2fSUN

typedef void (APIENTRY* pfn_glColor4ubVertex2fvSUN) (const GLubyte*, const GLfloat*);
extern pfn_glColor4ubVertex2fvSUN QGLglColor4ubVertex2fvSUN;
#define glColor4ubVertex2fvSUN QGLglColor4ubVertex2fvSUN

typedef void (APIENTRY* pfn_glColor4ubVertex3fSUN) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern pfn_glColor4ubVertex3fSUN QGLglColor4ubVertex3fSUN;
#define glColor4ubVertex3fSUN QGLglColor4ubVertex3fSUN

typedef void (APIENTRY* pfn_glColor4ubVertex3fvSUN) (const GLubyte*, const GLfloat*);
extern pfn_glColor4ubVertex3fvSUN QGLglColor4ubVertex3fvSUN;
#define glColor4ubVertex3fvSUN QGLglColor4ubVertex3fvSUN

typedef void (APIENTRY* pfn_glColor3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glColor3fVertex3fSUN QGLglColor3fVertex3fSUN;
#define glColor3fVertex3fSUN QGLglColor3fVertex3fSUN

typedef void (APIENTRY* pfn_glColor3fVertex3fvSUN) (const GLfloat*, const GLfloat*);
extern pfn_glColor3fVertex3fvSUN QGLglColor3fVertex3fvSUN;
#define glColor3fVertex3fvSUN QGLglColor3fVertex3fvSUN

typedef void (APIENTRY* pfn_glNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glNormal3fVertex3fSUN QGLglNormal3fVertex3fSUN;
#define glNormal3fVertex3fSUN QGLglNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glNormal3fVertex3fvSUN) (const GLfloat*, const GLfloat*);
extern pfn_glNormal3fVertex3fvSUN QGLglNormal3fVertex3fvSUN;
#define glNormal3fVertex3fvSUN QGLglNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glColor4fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glColor4fNormal3fVertex3fSUN QGLglColor4fNormal3fVertex3fSUN;
#define glColor4fNormal3fVertex3fSUN QGLglColor4fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glColor4fNormal3fVertex3fvSUN) (const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glColor4fNormal3fVertex3fvSUN QGLglColor4fNormal3fVertex3fvSUN;
#define glColor4fNormal3fVertex3fvSUN QGLglColor4fNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord2fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord2fVertex3fSUN QGLglTexCoord2fVertex3fSUN;
#define glTexCoord2fVertex3fSUN QGLglTexCoord2fVertex3fSUN

typedef void (APIENTRY* pfn_glTexCoord2fVertex3fvSUN) (const GLfloat*, const GLfloat*);
extern pfn_glTexCoord2fVertex3fvSUN QGLglTexCoord2fVertex3fvSUN;
#define glTexCoord2fVertex3fvSUN QGLglTexCoord2fVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord4fVertex4fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord4fVertex4fSUN QGLglTexCoord4fVertex4fSUN;
#define glTexCoord4fVertex4fSUN QGLglTexCoord4fVertex4fSUN

typedef void (APIENTRY* pfn_glTexCoord4fVertex4fvSUN) (const GLfloat*, const GLfloat*);
extern pfn_glTexCoord4fVertex4fvSUN QGLglTexCoord4fVertex4fvSUN;
#define glTexCoord4fVertex4fvSUN QGLglTexCoord4fVertex4fvSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor4ubVertex3fSUN) (GLfloat, GLfloat, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord2fColor4ubVertex3fSUN QGLglTexCoord2fColor4ubVertex3fSUN;
#define glTexCoord2fColor4ubVertex3fSUN QGLglTexCoord2fColor4ubVertex3fSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor4ubVertex3fvSUN) (const GLfloat*, const GLubyte*, const GLfloat*);
extern pfn_glTexCoord2fColor4ubVertex3fvSUN QGLglTexCoord2fColor4ubVertex3fvSUN;
#define glTexCoord2fColor4ubVertex3fvSUN QGLglTexCoord2fColor4ubVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord2fColor3fVertex3fSUN QGLglTexCoord2fColor3fVertex3fSUN;
#define glTexCoord2fColor3fVertex3fSUN QGLglTexCoord2fColor3fVertex3fSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor3fVertex3fvSUN) (const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glTexCoord2fColor3fVertex3fvSUN QGLglTexCoord2fColor3fVertex3fvSUN;
#define glTexCoord2fColor3fVertex3fvSUN QGLglTexCoord2fColor3fVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord2fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord2fNormal3fVertex3fSUN QGLglTexCoord2fNormal3fVertex3fSUN;
#define glTexCoord2fNormal3fVertex3fSUN QGLglTexCoord2fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glTexCoord2fNormal3fVertex3fvSUN) (const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glTexCoord2fNormal3fVertex3fvSUN QGLglTexCoord2fNormal3fVertex3fvSUN;
#define glTexCoord2fNormal3fVertex3fvSUN QGLglTexCoord2fNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor4fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord2fColor4fNormal3fVertex3fSUN QGLglTexCoord2fColor4fNormal3fVertex3fSUN;
#define glTexCoord2fColor4fNormal3fVertex3fSUN QGLglTexCoord2fColor4fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLfloat*, const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glTexCoord2fColor4fNormal3fVertex3fvSUN QGLglTexCoord2fColor4fNormal3fVertex3fvSUN;
#define glTexCoord2fColor4fNormal3fVertex3fvSUN QGLglTexCoord2fColor4fNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glTexCoord4fColor4fNormal3fVertex4fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glTexCoord4fColor4fNormal3fVertex4fSUN QGLglTexCoord4fColor4fNormal3fVertex4fSUN;
#define glTexCoord4fColor4fNormal3fVertex4fSUN QGLglTexCoord4fColor4fNormal3fVertex4fSUN

typedef void (APIENTRY* pfn_glTexCoord4fColor4fNormal3fVertex4fvSUN) (const GLfloat*, const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glTexCoord4fColor4fNormal3fVertex4fvSUN QGLglTexCoord4fColor4fNormal3fVertex4fvSUN;
#define glTexCoord4fColor4fNormal3fVertex4fvSUN QGLglTexCoord4fColor4fNormal3fVertex4fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiVertex3fSUN QGLglReplacementCodeuiVertex3fSUN;
#define glReplacementCodeuiVertex3fSUN QGLglReplacementCodeuiVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiVertex3fvSUN) (const GLuint*, const GLfloat*);
extern pfn_glReplacementCodeuiVertex3fvSUN QGLglReplacementCodeuiVertex3fvSUN;
#define glReplacementCodeuiVertex3fvSUN QGLglReplacementCodeuiVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor4ubVertex3fSUN) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiColor4ubVertex3fSUN QGLglReplacementCodeuiColor4ubVertex3fSUN;
#define glReplacementCodeuiColor4ubVertex3fSUN QGLglReplacementCodeuiColor4ubVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor4ubVertex3fvSUN) (const GLuint*, const GLubyte*, const GLfloat*);
extern pfn_glReplacementCodeuiColor4ubVertex3fvSUN QGLglReplacementCodeuiColor4ubVertex3fvSUN;
#define glReplacementCodeuiColor4ubVertex3fvSUN QGLglReplacementCodeuiColor4ubVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiColor3fVertex3fSUN QGLglReplacementCodeuiColor3fVertex3fSUN;
#define glReplacementCodeuiColor3fVertex3fSUN QGLglReplacementCodeuiColor3fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor3fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiColor3fVertex3fvSUN QGLglReplacementCodeuiColor3fVertex3fvSUN;
#define glReplacementCodeuiColor3fVertex3fvSUN QGLglReplacementCodeuiColor3fVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiNormal3fVertex3fSUN QGLglReplacementCodeuiNormal3fVertex3fSUN;
#define glReplacementCodeuiNormal3fVertex3fSUN QGLglReplacementCodeuiNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiNormal3fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiNormal3fVertex3fvSUN QGLglReplacementCodeuiNormal3fVertex3fvSUN;
#define glReplacementCodeuiNormal3fVertex3fvSUN QGLglReplacementCodeuiNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor4fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fSUN;
#define glReplacementCodeuiColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiColor4fNormal3fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fvSUN;
#define glReplacementCodeuiColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiTexCoord2fVertex3fSUN QGLglReplacementCodeuiTexCoord2fVertex3fSUN;
#define glReplacementCodeuiTexCoord2fVertex3fSUN QGLglReplacementCodeuiTexCoord2fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiTexCoord2fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fVertex3fvSUN;
#define glReplacementCodeuiTexCoord2fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN

typedef void (APIENTRY* pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLuint*, const GLfloat*, const GLfloat*, const GLfloat*, const GLfloat*);
extern pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_func_separate;
extern int QGLNumFunctions_GL_EXT_blend_func_separate;
#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate
#define QGL_DEFINED_GL_EXT_blend_func_separate
#define GL_BLEND_DST_RGB_EXT              0x80C8
#define GL_BLEND_SRC_RGB_EXT              0x80C9
#define GL_BLEND_DST_ALPHA_EXT            0x80CA
#define GL_BLEND_SRC_ALPHA_EXT            0x80CB

typedef void (APIENTRY* pfn_glBlendFuncSeparateEXT) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glBlendFuncSeparateEXT QGLglBlendFuncSeparateEXT;
#define glBlendFuncSeparateEXT QGLglBlendFuncSeparateEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INGR_blend_func_separate;
extern int QGLNumFunctions_GL_INGR_blend_func_separate;
#ifndef GL_INGR_blend_func_separate
#define GL_INGR_blend_func_separate
#define QGL_DEFINED_GL_INGR_blend_func_separate

typedef void (APIENTRY* pfn_glBlendFuncSeparateINGR) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glBlendFuncSeparateINGR QGLglBlendFuncSeparateINGR;
#define glBlendFuncSeparateINGR QGLglBlendFuncSeparateINGR
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INGR_color_clamp;
extern int QGLNumFunctions_GL_INGR_color_clamp;
#ifndef GL_INGR_color_clamp
#define GL_INGR_color_clamp
#define QGL_DEFINED_GL_INGR_color_clamp
#define GL_RED_MIN_CLAMP_INGR             0x8560
#define GL_GREEN_MIN_CLAMP_INGR           0x8561
#define GL_BLUE_MIN_CLAMP_INGR            0x8562
#define GL_ALPHA_MIN_CLAMP_INGR           0x8563
#define GL_RED_MAX_CLAMP_INGR             0x8564
#define GL_GREEN_MAX_CLAMP_INGR           0x8565
#define GL_BLUE_MAX_CLAMP_INGR            0x8566
#define GL_ALPHA_MAX_CLAMP_INGR           0x8567
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_INGR_interlace_read;
extern int QGLNumFunctions_GL_INGR_interlace_read;
#ifndef GL_INGR_interlace_read
#define GL_INGR_interlace_read
#define QGL_DEFINED_GL_INGR_interlace_read
#define GL_INTERLACE_READ_INGR            0x8568
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_stencil_wrap;
extern int QGLNumFunctions_GL_EXT_stencil_wrap;
#ifndef GL_EXT_stencil_wrap
#define GL_EXT_stencil_wrap
#define QGL_DEFINED_GL_EXT_stencil_wrap
#define GL_INCR_WRAP_EXT                  0x8507
#define GL_DECR_WRAP_EXT                  0x8508
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_422_pixels;
extern int QGLNumFunctions_GL_EXT_422_pixels;
#ifndef GL_EXT_422_pixels
#define GL_EXT_422_pixels
#define QGL_DEFINED_GL_EXT_422_pixels
#define GL_422_EXT                        0x80CC
#define GL_422_REV_EXT                    0x80CD
#define GL_422_AVERAGE_EXT                0x80CE
#define GL_422_REV_AVERAGE_EXT            0x80CF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texgen_reflection;
extern int QGLNumFunctions_GL_NV_texgen_reflection;
#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection
#define QGL_DEFINED_GL_NV_texgen_reflection
#define GL_NORMAL_MAP_NV                  0x8511
#define GL_REFLECTION_MAP_NV              0x8512
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_cube_map;
extern int QGLNumFunctions_GL_EXT_texture_cube_map;
#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map
#define QGL_DEFINED_GL_EXT_texture_cube_map
#define GL_NORMAL_MAP_EXT                 0x8511
#define GL_REFLECTION_MAP_EXT             0x8512
#define GL_TEXTURE_CUBE_MAP_EXT           0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT   0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT     0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT  0x851C
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_convolution_border_modes;
extern int QGLNumFunctions_GL_SUN_convolution_border_modes;
#ifndef GL_SUN_convolution_border_modes
#define GL_SUN_convolution_border_modes
#define QGL_DEFINED_GL_SUN_convolution_border_modes
#define GL_WRAP_BORDER_SUN                0x81D4
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_env_add;
extern int QGLNumFunctions_GL_EXT_texture_env_add;
#ifndef GL_EXT_texture_env_add
#define GL_EXT_texture_env_add
#define QGL_DEFINED_GL_EXT_texture_env_add
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_lod_bias;
extern int QGLNumFunctions_GL_EXT_texture_lod_bias;
#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias
#define QGL_DEFINED_GL_EXT_texture_lod_bias
#define GL_MAX_TEXTURE_LOD_BIAS_EXT       0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT     0x8500
#define GL_TEXTURE_LOD_BIAS_EXT           0x8501
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_filter_anisotropic;
extern int QGLNumFunctions_GL_EXT_texture_filter_anisotropic;
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic
#define QGL_DEFINED_GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_vertex_weighting;
extern int QGLNumFunctions_GL_EXT_vertex_weighting;
#ifndef GL_EXT_vertex_weighting
#define GL_EXT_vertex_weighting
#define QGL_DEFINED_GL_EXT_vertex_weighting
#define GL_MODELVIEW0_STACK_DEPTH_EXT       GL_MODELVIEW_STACK_DEPTH
#define GL_MODELVIEW1_STACK_DEPTH_EXT       0x8502
#define GL_MODELVIEW0_MATRIX_EXT            GL_MODELVIEW_MATRIX
#define GL_MODELVIEW1_MATRIX_EXT            0x8506
#define GL_VERTEX_WEIGHTING_EXT             0x8509
#define GL_MODELVIEW0_EXT                   GL_MODELVIEW
#define GL_MODELVIEW1_EXT                   0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT        0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT          0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT     0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT     0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT   0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT  0x8510

typedef void (APIENTRY* pfn_glVertexWeightfEXT) (GLfloat);
extern pfn_glVertexWeightfEXT QGLglVertexWeightfEXT;
#define glVertexWeightfEXT QGLglVertexWeightfEXT

typedef void (APIENTRY* pfn_glVertexWeightfvEXT) (const GLfloat*);
extern pfn_glVertexWeightfvEXT QGLglVertexWeightfvEXT;
#define glVertexWeightfvEXT QGLglVertexWeightfvEXT

typedef void (APIENTRY* pfn_glVertexWeightPointerEXT) (GLsizei, GLenum, GLsizei, const GLvoid*);
extern pfn_glVertexWeightPointerEXT QGLglVertexWeightPointerEXT;
#define glVertexWeightPointerEXT QGLglVertexWeightPointerEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_light_max_exponent;
extern int QGLNumFunctions_GL_NV_light_max_exponent;
#ifndef GL_NV_light_max_exponent
#define GL_NV_light_max_exponent
#define QGL_DEFINED_GL_NV_light_max_exponent
#define GL_MAX_SHININESS_NV               0x8504
#define GL_MAX_SPOT_EXPONENT_NV           0x8505
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_array_range;
extern int QGLNumFunctions_GL_NV_vertex_array_range;
#ifndef GL_NV_vertex_array_range
#define GL_NV_vertex_array_range
#define QGL_DEFINED_GL_NV_vertex_array_range
#define GL_VERTEX_ARRAY_RANGE_NV          0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV   0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV    0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV  0x8521

typedef void (APIENTRY* pfn_glFlushVertexArrayRangeNV) (void);
extern pfn_glFlushVertexArrayRangeNV QGLglFlushVertexArrayRangeNV;
#define glFlushVertexArrayRangeNV QGLglFlushVertexArrayRangeNV

typedef void (APIENTRY* pfn_glVertexArrayRangeNV) (GLsizei, const GLvoid*);
extern pfn_glVertexArrayRangeNV QGLglVertexArrayRangeNV;
#define glVertexArrayRangeNV QGLglVertexArrayRangeNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_register_combiners;
extern int QGLNumFunctions_GL_NV_register_combiners;
#ifndef GL_NV_register_combiners
#define GL_NV_register_combiners
#define QGL_DEFINED_GL_NV_register_combiners
#define GL_REGISTER_COMBINERS_NV          0x8522
#define GL_VARIABLE_A_NV                  0x8523
#define GL_VARIABLE_B_NV                  0x8524
#define GL_VARIABLE_C_NV                  0x8525
#define GL_VARIABLE_D_NV                  0x8526
#define GL_VARIABLE_E_NV                  0x8527
#define GL_VARIABLE_F_NV                  0x8528
#define GL_VARIABLE_G_NV                  0x8529
#define GL_CONSTANT_COLOR0_NV             0x852A
#define GL_CONSTANT_COLOR1_NV             0x852B
#define GL_PRIMARY_COLOR_NV               0x852C
#define GL_SECONDARY_COLOR_NV             0x852D
#define GL_SPARE0_NV                      0x852E
#define GL_SPARE1_NV                      0x852F
#define GL_DISCARD_NV                     0x8530
#define GL_E_TIMES_F_NV                   0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#define GL_UNSIGNED_IDENTITY_NV           0x8536
#define GL_UNSIGNED_INVERT_NV             0x8537
#define GL_EXPAND_NORMAL_NV               0x8538
#define GL_EXPAND_NEGATE_NV               0x8539
#define GL_HALF_BIAS_NORMAL_NV            0x853A
#define GL_HALF_BIAS_NEGATE_NV            0x853B
#define GL_SIGNED_IDENTITY_NV             0x853C
#define GL_SIGNED_NEGATE_NV               0x853D
#define GL_SCALE_BY_TWO_NV                0x853E
#define GL_SCALE_BY_FOUR_NV               0x853F
#define GL_SCALE_BY_ONE_HALF_NV           0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV   0x8541
#define GL_COMBINER_INPUT_NV              0x8542
#define GL_COMBINER_MAPPING_NV            0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV    0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV     0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV     0x8546
#define GL_COMBINER_MUX_SUM_NV            0x8547
#define GL_COMBINER_SCALE_NV              0x8548
#define GL_COMBINER_BIAS_NV               0x8549
#define GL_COMBINER_AB_OUTPUT_NV          0x854A
#define GL_COMBINER_CD_OUTPUT_NV          0x854B
#define GL_COMBINER_SUM_OUTPUT_NV         0x854C
#define GL_MAX_GENERAL_COMBINERS_NV       0x854D
#define GL_NUM_GENERAL_COMBINERS_NV       0x854E
#define GL_COLOR_SUM_CLAMP_NV             0x854F
#define GL_COMBINER0_NV                   0x8550
#define GL_COMBINER1_NV                   0x8551
#define GL_COMBINER2_NV                   0x8552
#define GL_COMBINER3_NV                   0x8553
#define GL_COMBINER4_NV                   0x8554
#define GL_COMBINER5_NV                   0x8555
#define GL_COMBINER6_NV                   0x8556
#define GL_COMBINER7_NV                   0x8557
/* reuse GL_TEXTURE0_ARB */
/* reuse GL_TEXTURE1_ARB */
/* reuse GL_ZERO */
/* reuse GL_NONE */
/* reuse GL_FOG */

typedef void (APIENTRY* pfn_glCombinerParameterfvNV) (GLenum, const GLfloat*);
extern pfn_glCombinerParameterfvNV QGLglCombinerParameterfvNV;
#define glCombinerParameterfvNV QGLglCombinerParameterfvNV

typedef void (APIENTRY* pfn_glCombinerParameterfNV) (GLenum, GLfloat);
extern pfn_glCombinerParameterfNV QGLglCombinerParameterfNV;
#define glCombinerParameterfNV QGLglCombinerParameterfNV

typedef void (APIENTRY* pfn_glCombinerParameterivNV) (GLenum, const GLint*);
extern pfn_glCombinerParameterivNV QGLglCombinerParameterivNV;
#define glCombinerParameterivNV QGLglCombinerParameterivNV

typedef void (APIENTRY* pfn_glCombinerParameteriNV) (GLenum, GLint);
extern pfn_glCombinerParameteriNV QGLglCombinerParameteriNV;
#define glCombinerParameteriNV QGLglCombinerParameteriNV

typedef void (APIENTRY* pfn_glCombinerInputNV) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum);
extern pfn_glCombinerInputNV QGLglCombinerInputNV;
#define glCombinerInputNV QGLglCombinerInputNV

typedef void (APIENTRY* pfn_glCombinerOutputNV) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLboolean, GLboolean, GLboolean);
extern pfn_glCombinerOutputNV QGLglCombinerOutputNV;
#define glCombinerOutputNV QGLglCombinerOutputNV

typedef void (APIENTRY* pfn_glFinalCombinerInputNV) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glFinalCombinerInputNV QGLglFinalCombinerInputNV;
#define glFinalCombinerInputNV QGLglFinalCombinerInputNV

typedef void (APIENTRY* pfn_glGetCombinerInputParameterfvNV) (GLenum, GLenum, GLenum, GLenum, GLfloat*);
extern pfn_glGetCombinerInputParameterfvNV QGLglGetCombinerInputParameterfvNV;
#define glGetCombinerInputParameterfvNV QGLglGetCombinerInputParameterfvNV

typedef void (APIENTRY* pfn_glGetCombinerInputParameterivNV) (GLenum, GLenum, GLenum, GLenum, GLint*);
extern pfn_glGetCombinerInputParameterivNV QGLglGetCombinerInputParameterivNV;
#define glGetCombinerInputParameterivNV QGLglGetCombinerInputParameterivNV

typedef void (APIENTRY* pfn_glGetCombinerOutputParameterfvNV) (GLenum, GLenum, GLenum, GLfloat*);
extern pfn_glGetCombinerOutputParameterfvNV QGLglGetCombinerOutputParameterfvNV;
#define glGetCombinerOutputParameterfvNV QGLglGetCombinerOutputParameterfvNV

typedef void (APIENTRY* pfn_glGetCombinerOutputParameterivNV) (GLenum, GLenum, GLenum, GLint*);
extern pfn_glGetCombinerOutputParameterivNV QGLglGetCombinerOutputParameterivNV;
#define glGetCombinerOutputParameterivNV QGLglGetCombinerOutputParameterivNV

typedef void (APIENTRY* pfn_glGetFinalCombinerInputParameterfvNV) (GLenum, GLenum, GLfloat*);
extern pfn_glGetFinalCombinerInputParameterfvNV QGLglGetFinalCombinerInputParameterfvNV;
#define glGetFinalCombinerInputParameterfvNV QGLglGetFinalCombinerInputParameterfvNV

typedef void (APIENTRY* pfn_glGetFinalCombinerInputParameterivNV) (GLenum, GLenum, GLint*);
extern pfn_glGetFinalCombinerInputParameterivNV QGLglGetFinalCombinerInputParameterivNV;
#define glGetFinalCombinerInputParameterivNV QGLglGetFinalCombinerInputParameterivNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_fog_distance;
extern int QGLNumFunctions_GL_NV_fog_distance;
#ifndef GL_NV_fog_distance
#define GL_NV_fog_distance
#define QGL_DEFINED_GL_NV_fog_distance
#define GL_FOG_DISTANCE_MODE_NV           0x855A
#define GL_EYE_RADIAL_NV                  0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV          0x855C
/* reuse GL_EYE_PLANE */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texgen_emboss;
extern int QGLNumFunctions_GL_NV_texgen_emboss;
#ifndef GL_NV_texgen_emboss
#define GL_NV_texgen_emboss
#define QGL_DEFINED_GL_NV_texgen_emboss
#define GL_EMBOSS_LIGHT_NV                0x855D
#define GL_EMBOSS_CONSTANT_NV             0x855E
#define GL_EMBOSS_MAP_NV                  0x855F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_blend_square;
extern int QGLNumFunctions_GL_NV_blend_square;
#ifndef GL_NV_blend_square
#define GL_NV_blend_square
#define QGL_DEFINED_GL_NV_blend_square
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_env_combine4;
extern int QGLNumFunctions_GL_NV_texture_env_combine4;
#ifndef GL_NV_texture_env_combine4
#define GL_NV_texture_env_combine4
#define QGL_DEFINED_GL_NV_texture_env_combine4
#define GL_COMBINE4_NV                    0x8503
#define GL_SOURCE3_RGB_NV                 0x8583
#define GL_SOURCE3_ALPHA_NV               0x858B
#define GL_OPERAND3_RGB_NV                0x8593
#define GL_OPERAND3_ALPHA_NV              0x859B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_MESA_resize_buffers;
extern int QGLNumFunctions_GL_MESA_resize_buffers;
#ifndef GL_MESA_resize_buffers
#define GL_MESA_resize_buffers
#define QGL_DEFINED_GL_MESA_resize_buffers

typedef void (APIENTRY* pfn_glResizeBuffersMESA) (void);
extern pfn_glResizeBuffersMESA QGLglResizeBuffersMESA;
#define glResizeBuffersMESA QGLglResizeBuffersMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_MESA_window_pos;
extern int QGLNumFunctions_GL_MESA_window_pos;
#ifndef GL_MESA_window_pos
#define GL_MESA_window_pos
#define QGL_DEFINED_GL_MESA_window_pos

typedef void (APIENTRY* pfn_glWindowPos2dMESA) (GLdouble, GLdouble);
extern pfn_glWindowPos2dMESA QGLglWindowPos2dMESA;
#define glWindowPos2dMESA QGLglWindowPos2dMESA

typedef void (APIENTRY* pfn_glWindowPos2dvMESA) (const GLdouble*);
extern pfn_glWindowPos2dvMESA QGLglWindowPos2dvMESA;
#define glWindowPos2dvMESA QGLglWindowPos2dvMESA

typedef void (APIENTRY* pfn_glWindowPos2fMESA) (GLfloat, GLfloat);
extern pfn_glWindowPos2fMESA QGLglWindowPos2fMESA;
#define glWindowPos2fMESA QGLglWindowPos2fMESA

typedef void (APIENTRY* pfn_glWindowPos2fvMESA) (const GLfloat*);
extern pfn_glWindowPos2fvMESA QGLglWindowPos2fvMESA;
#define glWindowPos2fvMESA QGLglWindowPos2fvMESA

typedef void (APIENTRY* pfn_glWindowPos2iMESA) (GLint, GLint);
extern pfn_glWindowPos2iMESA QGLglWindowPos2iMESA;
#define glWindowPos2iMESA QGLglWindowPos2iMESA

typedef void (APIENTRY* pfn_glWindowPos2ivMESA) (const GLint*);
extern pfn_glWindowPos2ivMESA QGLglWindowPos2ivMESA;
#define glWindowPos2ivMESA QGLglWindowPos2ivMESA

typedef void (APIENTRY* pfn_glWindowPos2sMESA) (GLshort, GLshort);
extern pfn_glWindowPos2sMESA QGLglWindowPos2sMESA;
#define glWindowPos2sMESA QGLglWindowPos2sMESA

typedef void (APIENTRY* pfn_glWindowPos2svMESA) (const GLshort*);
extern pfn_glWindowPos2svMESA QGLglWindowPos2svMESA;
#define glWindowPos2svMESA QGLglWindowPos2svMESA

typedef void (APIENTRY* pfn_glWindowPos3dMESA) (GLdouble, GLdouble, GLdouble);
extern pfn_glWindowPos3dMESA QGLglWindowPos3dMESA;
#define glWindowPos3dMESA QGLglWindowPos3dMESA

typedef void (APIENTRY* pfn_glWindowPos3dvMESA) (const GLdouble*);
extern pfn_glWindowPos3dvMESA QGLglWindowPos3dvMESA;
#define glWindowPos3dvMESA QGLglWindowPos3dvMESA

typedef void (APIENTRY* pfn_glWindowPos3fMESA) (GLfloat, GLfloat, GLfloat);
extern pfn_glWindowPos3fMESA QGLglWindowPos3fMESA;
#define glWindowPos3fMESA QGLglWindowPos3fMESA

typedef void (APIENTRY* pfn_glWindowPos3fvMESA) (const GLfloat*);
extern pfn_glWindowPos3fvMESA QGLglWindowPos3fvMESA;
#define glWindowPos3fvMESA QGLglWindowPos3fvMESA

typedef void (APIENTRY* pfn_glWindowPos3iMESA) (GLint, GLint, GLint);
extern pfn_glWindowPos3iMESA QGLglWindowPos3iMESA;
#define glWindowPos3iMESA QGLglWindowPos3iMESA

typedef void (APIENTRY* pfn_glWindowPos3ivMESA) (const GLint*);
extern pfn_glWindowPos3ivMESA QGLglWindowPos3ivMESA;
#define glWindowPos3ivMESA QGLglWindowPos3ivMESA

typedef void (APIENTRY* pfn_glWindowPos3sMESA) (GLshort, GLshort, GLshort);
extern pfn_glWindowPos3sMESA QGLglWindowPos3sMESA;
#define glWindowPos3sMESA QGLglWindowPos3sMESA

typedef void (APIENTRY* pfn_glWindowPos3svMESA) (const GLshort*);
extern pfn_glWindowPos3svMESA QGLglWindowPos3svMESA;
#define glWindowPos3svMESA QGLglWindowPos3svMESA

typedef void (APIENTRY* pfn_glWindowPos4dMESA) (GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glWindowPos4dMESA QGLglWindowPos4dMESA;
#define glWindowPos4dMESA QGLglWindowPos4dMESA

typedef void (APIENTRY* pfn_glWindowPos4dvMESA) (const GLdouble*);
extern pfn_glWindowPos4dvMESA QGLglWindowPos4dvMESA;
#define glWindowPos4dvMESA QGLglWindowPos4dvMESA

typedef void (APIENTRY* pfn_glWindowPos4fMESA) (GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glWindowPos4fMESA QGLglWindowPos4fMESA;
#define glWindowPos4fMESA QGLglWindowPos4fMESA

typedef void (APIENTRY* pfn_glWindowPos4fvMESA) (const GLfloat*);
extern pfn_glWindowPos4fvMESA QGLglWindowPos4fvMESA;
#define glWindowPos4fvMESA QGLglWindowPos4fvMESA

typedef void (APIENTRY* pfn_glWindowPos4iMESA) (GLint, GLint, GLint, GLint);
extern pfn_glWindowPos4iMESA QGLglWindowPos4iMESA;
#define glWindowPos4iMESA QGLglWindowPos4iMESA

typedef void (APIENTRY* pfn_glWindowPos4ivMESA) (const GLint*);
extern pfn_glWindowPos4ivMESA QGLglWindowPos4ivMESA;
#define glWindowPos4ivMESA QGLglWindowPos4ivMESA

typedef void (APIENTRY* pfn_glWindowPos4sMESA) (GLshort, GLshort, GLshort, GLshort);
extern pfn_glWindowPos4sMESA QGLglWindowPos4sMESA;
#define glWindowPos4sMESA QGLglWindowPos4sMESA

typedef void (APIENTRY* pfn_glWindowPos4svMESA) (const GLshort*);
extern pfn_glWindowPos4svMESA QGLglWindowPos4svMESA;
#define glWindowPos4svMESA QGLglWindowPos4svMESA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_compression_s3tc;
extern int QGLNumFunctions_GL_EXT_texture_compression_s3tc;
#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc
#define QGL_DEFINED_GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_IBM_cull_vertex;
extern int QGLNumFunctions_GL_IBM_cull_vertex;
#ifndef GL_IBM_cull_vertex
#define GL_IBM_cull_vertex
#define QGL_DEFINED_GL_IBM_cull_vertex
#define GL_CULL_VERTEX_IBM                103050
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_IBM_multimode_draw_arrays;
extern int QGLNumFunctions_GL_IBM_multimode_draw_arrays;
#ifndef GL_IBM_multimode_draw_arrays
#define GL_IBM_multimode_draw_arrays
#define QGL_DEFINED_GL_IBM_multimode_draw_arrays

typedef void (APIENTRY* pfn_glMultiModeDrawArraysIBM) (const GLenum*, const GLint*, const GLsizei*, GLsizei, GLint);
extern pfn_glMultiModeDrawArraysIBM QGLglMultiModeDrawArraysIBM;
#define glMultiModeDrawArraysIBM QGLglMultiModeDrawArraysIBM

typedef void (APIENTRY* pfn_glMultiModeDrawElementsIBM) (const GLenum*, const GLsizei*, GLenum, const GLvoid* const*, GLsizei, GLint);
extern pfn_glMultiModeDrawElementsIBM QGLglMultiModeDrawElementsIBM;
#define glMultiModeDrawElementsIBM QGLglMultiModeDrawElementsIBM
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_IBM_vertex_array_lists;
extern int QGLNumFunctions_GL_IBM_vertex_array_lists;
#ifndef GL_IBM_vertex_array_lists
#define GL_IBM_vertex_array_lists
#define QGL_DEFINED_GL_IBM_vertex_array_lists
#define GL_VERTEX_ARRAY_LIST_IBM                    103070
#define GL_NORMAL_ARRAY_LIST_IBM                    103071
#define GL_COLOR_ARRAY_LIST_IBM                     103072
#define GL_INDEX_ARRAY_LIST_IBM                     103073
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM             103074
#define GL_EDGE_FLAG_ARRAY_LIST_IBM                 103075
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM            103076
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM           103077
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM             103080
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM             103081
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM              103082
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM              103083
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM      103084
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM          103085
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM     103086
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM    103087

typedef void (APIENTRY* pfn_glColorPointerListIBM) (GLint, GLenum, GLint, const GLvoid**, GLint);
extern pfn_glColorPointerListIBM QGLglColorPointerListIBM;
#define glColorPointerListIBM QGLglColorPointerListIBM

typedef void (APIENTRY* pfn_glSecondaryColorPointerListIBM) (GLint, GLenum, GLint, const GLvoid**, GLint);
extern pfn_glSecondaryColorPointerListIBM QGLglSecondaryColorPointerListIBM;
#define glSecondaryColorPointerListIBM QGLglSecondaryColorPointerListIBM

typedef void (APIENTRY* pfn_glEdgeFlagPointerListIBM) (GLint, const GLboolean**, GLint);
extern pfn_glEdgeFlagPointerListIBM QGLglEdgeFlagPointerListIBM;
#define glEdgeFlagPointerListIBM QGLglEdgeFlagPointerListIBM

typedef void (APIENTRY* pfn_glFogCoordPointerListIBM) (GLenum, GLint, const GLvoid**, GLint);
extern pfn_glFogCoordPointerListIBM QGLglFogCoordPointerListIBM;
#define glFogCoordPointerListIBM QGLglFogCoordPointerListIBM

typedef void (APIENTRY* pfn_glIndexPointerListIBM) (GLenum, GLint, const GLvoid**, GLint);
extern pfn_glIndexPointerListIBM QGLglIndexPointerListIBM;
#define glIndexPointerListIBM QGLglIndexPointerListIBM

typedef void (APIENTRY* pfn_glNormalPointerListIBM) (GLenum, GLint, const GLvoid**, GLint);
extern pfn_glNormalPointerListIBM QGLglNormalPointerListIBM;
#define glNormalPointerListIBM QGLglNormalPointerListIBM

typedef void (APIENTRY* pfn_glTexCoordPointerListIBM) (GLint, GLenum, GLint, const GLvoid**, GLint);
extern pfn_glTexCoordPointerListIBM QGLglTexCoordPointerListIBM;
#define glTexCoordPointerListIBM QGLglTexCoordPointerListIBM

typedef void (APIENTRY* pfn_glVertexPointerListIBM) (GLint, GLenum, GLint, const GLvoid**, GLint);
extern pfn_glVertexPointerListIBM QGLglVertexPointerListIBM;
#define glVertexPointerListIBM QGLglVertexPointerListIBM
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_subsample;
extern int QGLNumFunctions_GL_SGIX_subsample;
#ifndef GL_SGIX_subsample
#define GL_SGIX_subsample
#define QGL_DEFINED_GL_SGIX_subsample
#define GL_PACK_SUBSAMPLE_RATE_SGIX       0x85A0
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX     0x85A1
#define GL_PIXEL_SUBSAMPLE_4444_SGIX      0x85A2
#define GL_PIXEL_SUBSAMPLE_2424_SGIX      0x85A3
#define GL_PIXEL_SUBSAMPLE_4242_SGIX      0x85A4
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_ycrcb_subsample;
extern int QGLNumFunctions_GL_SGIX_ycrcb_subsample;
#ifndef GL_SGIX_ycrcb_subsample
#define GL_SGIX_ycrcb_subsample
#define QGL_DEFINED_GL_SGIX_ycrcb_subsample
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_ycrcba;
extern int QGLNumFunctions_GL_SGIX_ycrcba;
#ifndef GL_SGIX_ycrcba
#define GL_SGIX_ycrcba
#define QGL_DEFINED_GL_SGIX_ycrcba
#define GL_YCRCB_SGIX                     0x8318
#define GL_YCRCBA_SGIX                    0x8319
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_depth_pass_instrument;
extern int QGLNumFunctions_GL_SGIX_depth_pass_instrument;
#ifndef GL_SGIX_depth_pass_instrument
#define GL_SGIX_depth_pass_instrument
#define QGL_DEFINED_GL_SGIX_depth_pass_instrument
#define GL_DEPTH_PASS_INSTRUMENT_SGIX     0x8310
#define GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX 0x8311
#define GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX 0x8312
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_3DFX_texture_compression_FXT1;
extern int QGLNumFunctions_GL_3DFX_texture_compression_FXT1;
#ifndef GL_3DFX_texture_compression_FXT1
#define GL_3DFX_texture_compression_FXT1
#define QGL_DEFINED_GL_3DFX_texture_compression_FXT1
#define GL_COMPRESSED_RGB_FXT1_3DFX       0x86B0
#define GL_COMPRESSED_RGBA_FXT1_3DFX      0x86B1
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_3DFX_multisample;
extern int QGLNumFunctions_GL_3DFX_multisample;
#ifndef GL_3DFX_multisample
#define GL_3DFX_multisample
#define QGL_DEFINED_GL_3DFX_multisample
#define GL_MULTISAMPLE_3DFX               0x86B2
#define GL_SAMPLE_BUFFERS_3DFX            0x86B3
#define GL_SAMPLES_3DFX                   0x86B4
#define GL_MULTISAMPLE_BIT_3DFX           0x20000000
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_3DFX_tbuffer;
extern int QGLNumFunctions_GL_3DFX_tbuffer;
#ifndef GL_3DFX_tbuffer
#define GL_3DFX_tbuffer
#define QGL_DEFINED_GL_3DFX_tbuffer

typedef void (APIENTRY* pfn_glTbufferMask3DFX) (GLuint);
extern pfn_glTbufferMask3DFX QGLglTbufferMask3DFX;
#define glTbufferMask3DFX QGLglTbufferMask3DFX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_multisample;
extern int QGLNumFunctions_GL_EXT_multisample;
#ifndef GL_EXT_multisample
#define GL_EXT_multisample
#define QGL_DEFINED_GL_EXT_multisample
#define GL_MULTISAMPLE_EXT                0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT        0x809F
#define GL_SAMPLE_MASK_EXT                0x80A0
#define GL_1PASS_EXT                      0x80A1
#define GL_2PASS_0_EXT                    0x80A2
#define GL_2PASS_1_EXT                    0x80A3
#define GL_4PASS_0_EXT                    0x80A4
#define GL_4PASS_1_EXT                    0x80A5
#define GL_4PASS_2_EXT                    0x80A6
#define GL_4PASS_3_EXT                    0x80A7
#define GL_SAMPLE_BUFFERS_EXT             0x80A8
#define GL_SAMPLES_EXT                    0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT          0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT         0x80AB
#define GL_SAMPLE_PATTERN_EXT             0x80AC
#define GL_MULTISAMPLE_BIT_EXT            0x20000000

typedef void (APIENTRY* pfn_glSampleMaskEXT) (GLclampf, GLboolean);
extern pfn_glSampleMaskEXT QGLglSampleMaskEXT;
#define glSampleMaskEXT QGLglSampleMaskEXT

typedef void (APIENTRY* pfn_glSamplePatternEXT) (GLenum);
extern pfn_glSamplePatternEXT QGLglSamplePatternEXT;
#define glSamplePatternEXT QGLglSamplePatternEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_vertex_preclip;
extern int QGLNumFunctions_GL_SGIX_vertex_preclip;
#ifndef GL_SGIX_vertex_preclip
#define GL_SGIX_vertex_preclip
#define QGL_DEFINED_GL_SGIX_vertex_preclip
#define GL_VERTEX_PRECLIP_SGIX            0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX       0x83EF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_convolution_accuracy;
extern int QGLNumFunctions_GL_SGIX_convolution_accuracy;
#ifndef GL_SGIX_convolution_accuracy
#define GL_SGIX_convolution_accuracy
#define QGL_DEFINED_GL_SGIX_convolution_accuracy
#define GL_CONVOLUTION_HINT_SGIX          0x8316
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_resample;
extern int QGLNumFunctions_GL_SGIX_resample;
#ifndef GL_SGIX_resample
#define GL_SGIX_resample
#define QGL_DEFINED_GL_SGIX_resample
#define GL_PACK_RESAMPLE_SGIX             0x842C
#define GL_UNPACK_RESAMPLE_SGIX           0x842D
#define GL_RESAMPLE_REPLICATE_SGIX        0x842E
#define GL_RESAMPLE_ZERO_FILL_SGIX        0x842F
#define GL_RESAMPLE_DECIMATE_SGIX         0x8430
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_point_line_texgen;
extern int QGLNumFunctions_GL_SGIS_point_line_texgen;
#ifndef GL_SGIS_point_line_texgen
#define GL_SGIS_point_line_texgen
#define QGL_DEFINED_GL_SGIS_point_line_texgen
#define GL_EYE_DISTANCE_TO_POINT_SGIS     0x81F0
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS  0x81F1
#define GL_EYE_DISTANCE_TO_LINE_SGIS      0x81F2
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS   0x81F3
#define GL_EYE_POINT_SGIS                 0x81F4
#define GL_OBJECT_POINT_SGIS              0x81F5
#define GL_EYE_LINE_SGIS                  0x81F6
#define GL_OBJECT_LINE_SGIS               0x81F7
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIS_texture_color_mask;
extern int QGLNumFunctions_GL_SGIS_texture_color_mask;
#ifndef GL_SGIS_texture_color_mask
#define GL_SGIS_texture_color_mask
#define QGL_DEFINED_GL_SGIS_texture_color_mask
#define GL_TEXTURE_COLOR_WRITEMASK_SGIS   0x81EF

typedef void (APIENTRY* pfn_glTextureColorMaskSGIS) (GLboolean, GLboolean, GLboolean, GLboolean);
extern pfn_glTextureColorMaskSGIS QGLglTextureColorMaskSGIS;
#define glTextureColorMaskSGIS QGLglTextureColorMaskSGIS
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_igloo_interface;
extern int QGLNumFunctions_GL_SGIX_igloo_interface;
#ifndef GL_SGIX_igloo_interface
#define GL_SGIX_igloo_interface
#define QGL_DEFINED_GL_SGIX_igloo_interface

typedef void (APIENTRY* pfn_glIglooInterfaceSGIX) (GLenum, const GLvoid*);
extern pfn_glIglooInterfaceSGIX QGLglIglooInterfaceSGIX;
#define glIglooInterfaceSGIX QGLglIglooInterfaceSGIX
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_env_dot3;
extern int QGLNumFunctions_GL_EXT_texture_env_dot3;
#ifndef GL_EXT_texture_env_dot3
#define GL_EXT_texture_env_dot3
#define QGL_DEFINED_GL_EXT_texture_env_dot3
#define GL_DOT3_RGB_EXT                   0x8740
#define GL_DOT3_RGBA_EXT                  0x8741
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_texture_mirror_once;
extern int QGLNumFunctions_GL_ATI_texture_mirror_once;
#ifndef GL_ATI_texture_mirror_once
#define GL_ATI_texture_mirror_once
#define QGL_DEFINED_GL_ATI_texture_mirror_once
#define GL_MIRROR_CLAMP_ATI               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI       0x8743
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_fence;
extern int QGLNumFunctions_GL_NV_fence;
#ifndef GL_NV_fence
#define GL_NV_fence
#define QGL_DEFINED_GL_NV_fence
#define GL_ALL_COMPLETED_NV               0x84F2
#define GL_FENCE_STATUS_NV                0x84F3
#define GL_FENCE_CONDITION_NV             0x84F4

typedef void (APIENTRY* pfn_glDeleteFencesNV) (GLsizei, const GLuint*);
extern pfn_glDeleteFencesNV QGLglDeleteFencesNV;
#define glDeleteFencesNV QGLglDeleteFencesNV

typedef void (APIENTRY* pfn_glGenFencesNV) (GLsizei, GLuint*);
extern pfn_glGenFencesNV QGLglGenFencesNV;
#define glGenFencesNV QGLglGenFencesNV

typedef GLboolean (APIENTRY* pfn_glIsFenceNV) (GLuint);
extern pfn_glIsFenceNV QGLglIsFenceNV;
#define glIsFenceNV QGLglIsFenceNV

typedef GLboolean (APIENTRY* pfn_glTestFenceNV) (GLuint);
extern pfn_glTestFenceNV QGLglTestFenceNV;
#define glTestFenceNV QGLglTestFenceNV

typedef void (APIENTRY* pfn_glGetFenceivNV) (GLuint, GLenum, GLint*);
extern pfn_glGetFenceivNV QGLglGetFenceivNV;
#define glGetFenceivNV QGLglGetFenceivNV

typedef void (APIENTRY* pfn_glFinishFenceNV) (GLuint);
extern pfn_glFinishFenceNV QGLglFinishFenceNV;
#define glFinishFenceNV QGLglFinishFenceNV

typedef void (APIENTRY* pfn_glSetFenceNV) (GLuint, GLenum);
extern pfn_glSetFenceNV QGLglSetFenceNV;
#define glSetFenceNV QGLglSetFenceNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_IBM_texture_mirrored_repeat;
extern int QGLNumFunctions_GL_IBM_texture_mirrored_repeat;
#ifndef GL_IBM_texture_mirrored_repeat
#define GL_IBM_texture_mirrored_repeat
#define QGL_DEFINED_GL_IBM_texture_mirrored_repeat
#define GL_MIRRORED_REPEAT_IBM            0x8370
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_evaluators;
extern int QGLNumFunctions_GL_NV_evaluators;
#ifndef GL_NV_evaluators
#define GL_NV_evaluators
#define QGL_DEFINED_GL_NV_evaluators
#define GL_EVAL_2D_NV                       0x86C0
#define GL_EVAL_TRIANGULAR_2D_NV            0x86C1
#define GL_MAP_TESSELLATION_NV              0x86C2
#define GL_MAP_ATTRIB_U_ORDER_NV            0x86C3
#define GL_MAP_ATTRIB_V_ORDER_NV            0x86C4
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV  0x86C5
#define GL_EVAL_VERTEX_ATTRIB0_NV           0x86C6
#define GL_EVAL_VERTEX_ATTRIB1_NV           0x86C7
#define GL_EVAL_VERTEX_ATTRIB2_NV           0x86C8
#define GL_EVAL_VERTEX_ATTRIB3_NV           0x86C9
#define GL_EVAL_VERTEX_ATTRIB4_NV           0x86CA
#define GL_EVAL_VERTEX_ATTRIB5_NV           0x86CB
#define GL_EVAL_VERTEX_ATTRIB6_NV           0x86CC
#define GL_EVAL_VERTEX_ATTRIB7_NV           0x86CD
#define GL_EVAL_VERTEX_ATTRIB8_NV           0x86CE
#define GL_EVAL_VERTEX_ATTRIB9_NV           0x86CF
#define GL_EVAL_VERTEX_ATTRIB10_NV          0x86D0
#define GL_EVAL_VERTEX_ATTRIB11_NV          0x86D1
#define GL_EVAL_VERTEX_ATTRIB12_NV          0x86D2
#define GL_EVAL_VERTEX_ATTRIB13_NV          0x86D3
#define GL_EVAL_VERTEX_ATTRIB14_NV          0x86D4
#define GL_EVAL_VERTEX_ATTRIB15_NV          0x86D5
#define GL_MAX_MAP_TESSELLATION_NV          0x86D6
#define GL_MAX_RATIONAL_EVAL_ORDER_NV       0x86D7

typedef void (APIENTRY* pfn_glMapControlPointsNV) (GLenum, GLuint, GLenum, GLsizei, GLsizei, GLint, GLint, GLboolean, const GLvoid*);
extern pfn_glMapControlPointsNV QGLglMapControlPointsNV;
#define glMapControlPointsNV QGLglMapControlPointsNV

typedef void (APIENTRY* pfn_glMapParameterivNV) (GLenum, GLenum, const GLint*);
extern pfn_glMapParameterivNV QGLglMapParameterivNV;
#define glMapParameterivNV QGLglMapParameterivNV

typedef void (APIENTRY* pfn_glMapParameterfvNV) (GLenum, GLenum, const GLfloat*);
extern pfn_glMapParameterfvNV QGLglMapParameterfvNV;
#define glMapParameterfvNV QGLglMapParameterfvNV

typedef void (APIENTRY* pfn_glGetMapControlPointsNV) (GLenum, GLuint, GLenum, GLsizei, GLsizei, GLboolean, GLvoid*);
extern pfn_glGetMapControlPointsNV QGLglGetMapControlPointsNV;
#define glGetMapControlPointsNV QGLglGetMapControlPointsNV

typedef void (APIENTRY* pfn_glGetMapParameterivNV) (GLenum, GLenum, GLint*);
extern pfn_glGetMapParameterivNV QGLglGetMapParameterivNV;
#define glGetMapParameterivNV QGLglGetMapParameterivNV

typedef void (APIENTRY* pfn_glGetMapParameterfvNV) (GLenum, GLenum, GLfloat*);
extern pfn_glGetMapParameterfvNV QGLglGetMapParameterfvNV;
#define glGetMapParameterfvNV QGLglGetMapParameterfvNV

typedef void (APIENTRY* pfn_glGetMapAttribParameterivNV) (GLenum, GLuint, GLenum, GLint*);
extern pfn_glGetMapAttribParameterivNV QGLglGetMapAttribParameterivNV;
#define glGetMapAttribParameterivNV QGLglGetMapAttribParameterivNV

typedef void (APIENTRY* pfn_glGetMapAttribParameterfvNV) (GLenum, GLuint, GLenum, GLfloat*);
extern pfn_glGetMapAttribParameterfvNV QGLglGetMapAttribParameterfvNV;
#define glGetMapAttribParameterfvNV QGLglGetMapAttribParameterfvNV

typedef void (APIENTRY* pfn_glEvalMapsNV) (GLenum, GLenum);
extern pfn_glEvalMapsNV QGLglEvalMapsNV;
#define glEvalMapsNV QGLglEvalMapsNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_packed_depth_stencil;
extern int QGLNumFunctions_GL_NV_packed_depth_stencil;
#ifndef GL_NV_packed_depth_stencil
#define GL_NV_packed_depth_stencil
#define QGL_DEFINED_GL_NV_packed_depth_stencil
#define GL_DEPTH_STENCIL_NV               0x84F9
#define GL_UNSIGNED_INT_24_8_NV           0x84FA
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_register_combiners2;
extern int QGLNumFunctions_GL_NV_register_combiners2;
#ifndef GL_NV_register_combiners2
#define GL_NV_register_combiners2
#define QGL_DEFINED_GL_NV_register_combiners2
#define GL_PER_STAGE_CONSTANTS_NV         0x8535

typedef void (APIENTRY* pfn_glCombinerStageParameterfvNV) (GLenum, GLenum, const GLfloat*);
extern pfn_glCombinerStageParameterfvNV QGLglCombinerStageParameterfvNV;
#define glCombinerStageParameterfvNV QGLglCombinerStageParameterfvNV

typedef void (APIENTRY* pfn_glGetCombinerStageParameterfvNV) (GLenum, GLenum, GLfloat*);
extern pfn_glGetCombinerStageParameterfvNV QGLglGetCombinerStageParameterfvNV;
#define glGetCombinerStageParameterfvNV QGLglGetCombinerStageParameterfvNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_compression_vtc;
extern int QGLNumFunctions_GL_NV_texture_compression_vtc;
#ifndef GL_NV_texture_compression_vtc
#define GL_NV_texture_compression_vtc
#define QGL_DEFINED_GL_NV_texture_compression_vtc
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_rectangle;
extern int QGLNumFunctions_GL_NV_texture_rectangle;
#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle
#define QGL_DEFINED_GL_NV_texture_rectangle
#define GL_TEXTURE_RECTANGLE_NV           0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV   0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV     0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV  0x84F8
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_shader;
extern int QGLNumFunctions_GL_NV_texture_shader;
#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader
#define QGL_DEFINED_GL_NV_texture_shader
#define GL_OFFSET_TEXTURE_RECTANGLE_NV                  0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV            0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV             0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV         0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV                    0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV                0x86DB
#define GL_DSDT_MAG_INTENSITY_NV                        0x86DC
#define GL_SHADER_CONSISTENT_NV                         0x86DD
#define GL_TEXTURE_SHADER_NV                            0x86DE
#define GL_SHADER_OPERATION_NV                          0x86DF
#define GL_CULL_MODES_NV                                0x86E0
#define GL_OFFSET_TEXTURE_MATRIX_NV                     0x86E1
#define GL_OFFSET_TEXTURE_SCALE_NV                      0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV                       0x86E3
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV                  GL_OFFSET_TEXTURE_MATRIX_NV
#define GL_OFFSET_TEXTURE_2D_SCALE_NV                   GL_OFFSET_TEXTURE_SCALE_NV
#define GL_OFFSET_TEXTURE_2D_BIAS_NV                    GL_OFFSET_TEXTURE_BIAS_NV
#define GL_PREVIOUS_TEXTURE_INPUT_NV                    0x86E4
#define GL_CONST_EYE_NV                                 0x86E5
#define GL_PASS_THROUGH_NV                              0x86E6
#define GL_CULL_FRAGMENT_NV                             0x86E7
#define GL_OFFSET_TEXTURE_2D_NV                         0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV                   0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV                   0x86EA
#define GL_DOT_PRODUCT_NV                               0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV                 0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV                    0x86EE
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV              0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV              0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV              0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV    0x86F3
#define GL_HILO_NV                                      0x86F4
#define GL_DSDT_NV                                      0x86F5
#define GL_DSDT_MAG_NV                                  0x86F6
#define GL_DSDT_MAG_VIB_NV                              0x86F7
#define GL_HILO16_NV                                    0x86F8
#define GL_SIGNED_HILO_NV                               0x86F9
#define GL_SIGNED_HILO16_NV                             0x86FA
#define GL_SIGNED_RGBA_NV                               0x86FB
#define GL_SIGNED_RGBA8_NV                              0x86FC
#define GL_SIGNED_RGB_NV                                0x86FE
#define GL_SIGNED_RGB8_NV                               0x86FF
#define GL_SIGNED_LUMINANCE_NV                          0x8701
#define GL_SIGNED_LUMINANCE8_NV                         0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV                    0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV                  0x8704
#define GL_SIGNED_ALPHA_NV                              0x8705
#define GL_SIGNED_ALPHA8_NV                             0x8706
#define GL_SIGNED_INTENSITY_NV                          0x8707
#define GL_SIGNED_INTENSITY8_NV                         0x8708
#define GL_DSDT8_NV                                     0x8709
#define GL_DSDT8_MAG8_NV                                0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV                     0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV                 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV               0x870D
#define GL_HI_SCALE_NV                                  0x870E
#define GL_LO_SCALE_NV                                  0x870F
#define GL_DS_SCALE_NV                                  0x8710
#define GL_DT_SCALE_NV                                  0x8711
#define GL_MAGNITUDE_SCALE_NV                           0x8712
#define GL_VIBRANCE_SCALE_NV                            0x8713
#define GL_HI_BIAS_NV                                   0x8714
#define GL_LO_BIAS_NV                                   0x8715
#define GL_DS_BIAS_NV                                   0x8716
#define GL_DT_BIAS_NV                                   0x8717
#define GL_MAGNITUDE_BIAS_NV                            0x8718
#define GL_VIBRANCE_BIAS_NV                             0x8719
#define GL_TEXTURE_BORDER_VALUES_NV                     0x871A
#define GL_TEXTURE_HI_SIZE_NV                           0x871B
#define GL_TEXTURE_LO_SIZE_NV                           0x871C
#define GL_TEXTURE_DS_SIZE_NV                           0x871D
#define GL_TEXTURE_DT_SIZE_NV                           0x871E
#define GL_TEXTURE_MAG_SIZE_NV                          0x871F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_shader2;
extern int QGLNumFunctions_GL_NV_texture_shader2;
#ifndef GL_NV_texture_shader2
#define GL_NV_texture_shader2
#define QGL_DEFINED_GL_NV_texture_shader2
#define GL_DOT_PRODUCT_TEXTURE_3D_NV      0x86EF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_array_range2;
extern int QGLNumFunctions_GL_NV_vertex_array_range2;
#ifndef GL_NV_vertex_array_range2
#define GL_NV_vertex_array_range2
#define QGL_DEFINED_GL_NV_vertex_array_range2
#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_program;
extern int QGLNumFunctions_GL_NV_vertex_program;
#ifndef GL_NV_vertex_program
#define GL_NV_vertex_program
#define QGL_DEFINED_GL_NV_vertex_program
#define GL_VERTEX_PROGRAM_NV                0x8620
#define GL_VERTEX_STATE_PROGRAM_NV          0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV             0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV           0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV             0x8625
#define GL_CURRENT_ATTRIB_NV                0x8626
#define GL_PROGRAM_LENGTH_NV                0x8627
#define GL_PROGRAM_STRING_NV                0x8628
#define GL_MODELVIEW_PROJECTION_NV          0x8629
#define GL_IDENTITY_NV                      0x862A
#define GL_INVERSE_NV                       0x862B
#define GL_TRANSPOSE_NV                     0x862C
#define GL_INVERSE_TRANSPOSE_NV             0x862D
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV  0x862E
#define GL_MAX_TRACK_MATRICES_NV            0x862F
#define GL_MATRIX0_NV                       0x8630
#define GL_MATRIX1_NV                       0x8631
#define GL_MATRIX2_NV                       0x8632
#define GL_MATRIX3_NV                       0x8633
#define GL_MATRIX4_NV                       0x8634
#define GL_MATRIX5_NV                       0x8635
#define GL_MATRIX6_NV                       0x8636
#define GL_MATRIX7_NV                       0x8637
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV    0x8640
#define GL_CURRENT_MATRIX_NV                0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV     0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV       0x8643
#define GL_PROGRAM_PARAMETER_NV             0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV          0x8645
#define GL_PROGRAM_TARGET_NV                0x8646
#define GL_PROGRAM_RESIDENT_NV              0x8647
#define GL_TRACK_MATRIX_NV                  0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV        0x8649
#define GL_VERTEX_PROGRAM_BINDING_NV        0x864A
#define GL_PROGRAM_ERROR_POSITION_NV        0x864B
#define GL_VERTEX_ATTRIB_ARRAY0_NV          0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV          0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV          0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV          0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV          0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV          0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV          0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV          0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV          0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV          0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV         0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV         0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV         0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV         0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV         0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV         0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV         0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV         0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV         0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV         0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV         0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV         0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV         0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV         0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV         0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV         0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV        0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV        0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV        0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV        0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV        0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV        0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV         0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV         0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV         0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV         0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV         0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV         0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV         0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV         0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV         0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV         0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV        0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV        0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV        0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV        0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV        0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV        0x867F

typedef GLboolean (APIENTRY* pfn_glAreProgramsResidentNV) (GLsizei, const GLuint*, GLboolean*);
extern pfn_glAreProgramsResidentNV QGLglAreProgramsResidentNV;
#define glAreProgramsResidentNV QGLglAreProgramsResidentNV

typedef void (APIENTRY* pfn_glBindProgramNV) (GLenum, GLuint);
extern pfn_glBindProgramNV QGLglBindProgramNV;
#define glBindProgramNV QGLglBindProgramNV

typedef void (APIENTRY* pfn_glDeleteProgramsNV) (GLsizei, const GLuint*);
extern pfn_glDeleteProgramsNV QGLglDeleteProgramsNV;
#define glDeleteProgramsNV QGLglDeleteProgramsNV

typedef void (APIENTRY* pfn_glExecuteProgramNV) (GLenum, GLuint, const GLfloat*);
extern pfn_glExecuteProgramNV QGLglExecuteProgramNV;
#define glExecuteProgramNV QGLglExecuteProgramNV

typedef void (APIENTRY* pfn_glGenProgramsNV) (GLsizei, GLuint*);
extern pfn_glGenProgramsNV QGLglGenProgramsNV;
#define glGenProgramsNV QGLglGenProgramsNV

typedef void (APIENTRY* pfn_glGetProgramParameterdvNV) (GLenum, GLuint, GLenum, GLdouble*);
extern pfn_glGetProgramParameterdvNV QGLglGetProgramParameterdvNV;
#define glGetProgramParameterdvNV QGLglGetProgramParameterdvNV

typedef void (APIENTRY* pfn_glGetProgramParameterfvNV) (GLenum, GLuint, GLenum, GLfloat*);
extern pfn_glGetProgramParameterfvNV QGLglGetProgramParameterfvNV;
#define glGetProgramParameterfvNV QGLglGetProgramParameterfvNV

typedef void (APIENTRY* pfn_glGetProgramivNV) (GLuint, GLenum, GLint*);
extern pfn_glGetProgramivNV QGLglGetProgramivNV;
#define glGetProgramivNV QGLglGetProgramivNV

typedef void (APIENTRY* pfn_glGetProgramStringNV) (GLuint, GLenum, GLubyte*);
extern pfn_glGetProgramStringNV QGLglGetProgramStringNV;
#define glGetProgramStringNV QGLglGetProgramStringNV

typedef void (APIENTRY* pfn_glGetTrackMatrixivNV) (GLenum, GLuint, GLenum, GLint*);
extern pfn_glGetTrackMatrixivNV QGLglGetTrackMatrixivNV;
#define glGetTrackMatrixivNV QGLglGetTrackMatrixivNV

typedef void (APIENTRY* pfn_glGetVertexAttribdvNV) (GLuint, GLenum, GLdouble*);
extern pfn_glGetVertexAttribdvNV QGLglGetVertexAttribdvNV;
#define glGetVertexAttribdvNV QGLglGetVertexAttribdvNV

typedef void (APIENTRY* pfn_glGetVertexAttribfvNV) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVertexAttribfvNV QGLglGetVertexAttribfvNV;
#define glGetVertexAttribfvNV QGLglGetVertexAttribfvNV

typedef void (APIENTRY* pfn_glGetVertexAttribivNV) (GLuint, GLenum, GLint*);
extern pfn_glGetVertexAttribivNV QGLglGetVertexAttribivNV;
#define glGetVertexAttribivNV QGLglGetVertexAttribivNV

typedef void (APIENTRY* pfn_glGetVertexAttribPointervNV) (GLuint, GLenum, GLvoid**);
extern pfn_glGetVertexAttribPointervNV QGLglGetVertexAttribPointervNV;
#define glGetVertexAttribPointervNV QGLglGetVertexAttribPointervNV

typedef GLboolean (APIENTRY* pfn_glIsProgramNV) (GLuint);
extern pfn_glIsProgramNV QGLglIsProgramNV;
#define glIsProgramNV QGLglIsProgramNV

typedef void (APIENTRY* pfn_glLoadProgramNV) (GLenum, GLuint, GLsizei, const GLubyte*);
extern pfn_glLoadProgramNV QGLglLoadProgramNV;
#define glLoadProgramNV QGLglLoadProgramNV

typedef void (APIENTRY* pfn_glProgramParameter4dNV) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glProgramParameter4dNV QGLglProgramParameter4dNV;
#define glProgramParameter4dNV QGLglProgramParameter4dNV

typedef void (APIENTRY* pfn_glProgramParameter4dvNV) (GLenum, GLuint, const GLdouble*);
extern pfn_glProgramParameter4dvNV QGLglProgramParameter4dvNV;
#define glProgramParameter4dvNV QGLglProgramParameter4dvNV

typedef void (APIENTRY* pfn_glProgramParameter4fNV) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glProgramParameter4fNV QGLglProgramParameter4fNV;
#define glProgramParameter4fNV QGLglProgramParameter4fNV

typedef void (APIENTRY* pfn_glProgramParameter4fvNV) (GLenum, GLuint, const GLfloat*);
extern pfn_glProgramParameter4fvNV QGLglProgramParameter4fvNV;
#define glProgramParameter4fvNV QGLglProgramParameter4fvNV

typedef void (APIENTRY* pfn_glProgramParameters4dvNV) (GLenum, GLuint, GLuint, const GLdouble*);
extern pfn_glProgramParameters4dvNV QGLglProgramParameters4dvNV;
#define glProgramParameters4dvNV QGLglProgramParameters4dvNV

typedef void (APIENTRY* pfn_glProgramParameters4fvNV) (GLenum, GLuint, GLuint, const GLfloat*);
extern pfn_glProgramParameters4fvNV QGLglProgramParameters4fvNV;
#define glProgramParameters4fvNV QGLglProgramParameters4fvNV

typedef void (APIENTRY* pfn_glRequestResidentProgramsNV) (GLsizei, const GLuint*);
extern pfn_glRequestResidentProgramsNV QGLglRequestResidentProgramsNV;
#define glRequestResidentProgramsNV QGLglRequestResidentProgramsNV

typedef void (APIENTRY* pfn_glTrackMatrixNV) (GLenum, GLuint, GLenum, GLenum);
extern pfn_glTrackMatrixNV QGLglTrackMatrixNV;
#define glTrackMatrixNV QGLglTrackMatrixNV

typedef void (APIENTRY* pfn_glVertexAttribPointerNV) (GLuint, GLint, GLenum, GLsizei, const GLvoid*);
extern pfn_glVertexAttribPointerNV QGLglVertexAttribPointerNV;
#define glVertexAttribPointerNV QGLglVertexAttribPointerNV

typedef void (APIENTRY* pfn_glVertexAttrib1dNV) (GLuint, GLdouble);
extern pfn_glVertexAttrib1dNV QGLglVertexAttrib1dNV;
#define glVertexAttrib1dNV QGLglVertexAttrib1dNV

typedef void (APIENTRY* pfn_glVertexAttrib1dvNV) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib1dvNV QGLglVertexAttrib1dvNV;
#define glVertexAttrib1dvNV QGLglVertexAttrib1dvNV

typedef void (APIENTRY* pfn_glVertexAttrib1fNV) (GLuint, GLfloat);
extern pfn_glVertexAttrib1fNV QGLglVertexAttrib1fNV;
#define glVertexAttrib1fNV QGLglVertexAttrib1fNV

typedef void (APIENTRY* pfn_glVertexAttrib1fvNV) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib1fvNV QGLglVertexAttrib1fvNV;
#define glVertexAttrib1fvNV QGLglVertexAttrib1fvNV

typedef void (APIENTRY* pfn_glVertexAttrib1sNV) (GLuint, GLshort);
extern pfn_glVertexAttrib1sNV QGLglVertexAttrib1sNV;
#define glVertexAttrib1sNV QGLglVertexAttrib1sNV

typedef void (APIENTRY* pfn_glVertexAttrib1svNV) (GLuint, const GLshort*);
extern pfn_glVertexAttrib1svNV QGLglVertexAttrib1svNV;
#define glVertexAttrib1svNV QGLglVertexAttrib1svNV

typedef void (APIENTRY* pfn_glVertexAttrib2dNV) (GLuint, GLdouble, GLdouble);
extern pfn_glVertexAttrib2dNV QGLglVertexAttrib2dNV;
#define glVertexAttrib2dNV QGLglVertexAttrib2dNV

typedef void (APIENTRY* pfn_glVertexAttrib2dvNV) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib2dvNV QGLglVertexAttrib2dvNV;
#define glVertexAttrib2dvNV QGLglVertexAttrib2dvNV

typedef void (APIENTRY* pfn_glVertexAttrib2fNV) (GLuint, GLfloat, GLfloat);
extern pfn_glVertexAttrib2fNV QGLglVertexAttrib2fNV;
#define glVertexAttrib2fNV QGLglVertexAttrib2fNV

typedef void (APIENTRY* pfn_glVertexAttrib2fvNV) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib2fvNV QGLglVertexAttrib2fvNV;
#define glVertexAttrib2fvNV QGLglVertexAttrib2fvNV

typedef void (APIENTRY* pfn_glVertexAttrib2sNV) (GLuint, GLshort, GLshort);
extern pfn_glVertexAttrib2sNV QGLglVertexAttrib2sNV;
#define glVertexAttrib2sNV QGLglVertexAttrib2sNV

typedef void (APIENTRY* pfn_glVertexAttrib2svNV) (GLuint, const GLshort*);
extern pfn_glVertexAttrib2svNV QGLglVertexAttrib2svNV;
#define glVertexAttrib2svNV QGLglVertexAttrib2svNV

typedef void (APIENTRY* pfn_glVertexAttrib3dNV) (GLuint, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib3dNV QGLglVertexAttrib3dNV;
#define glVertexAttrib3dNV QGLglVertexAttrib3dNV

typedef void (APIENTRY* pfn_glVertexAttrib3dvNV) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib3dvNV QGLglVertexAttrib3dvNV;
#define glVertexAttrib3dvNV QGLglVertexAttrib3dvNV

typedef void (APIENTRY* pfn_glVertexAttrib3fNV) (GLuint, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib3fNV QGLglVertexAttrib3fNV;
#define glVertexAttrib3fNV QGLglVertexAttrib3fNV

typedef void (APIENTRY* pfn_glVertexAttrib3fvNV) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib3fvNV QGLglVertexAttrib3fvNV;
#define glVertexAttrib3fvNV QGLglVertexAttrib3fvNV

typedef void (APIENTRY* pfn_glVertexAttrib3sNV) (GLuint, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib3sNV QGLglVertexAttrib3sNV;
#define glVertexAttrib3sNV QGLglVertexAttrib3sNV

typedef void (APIENTRY* pfn_glVertexAttrib3svNV) (GLuint, const GLshort*);
extern pfn_glVertexAttrib3svNV QGLglVertexAttrib3svNV;
#define glVertexAttrib3svNV QGLglVertexAttrib3svNV

typedef void (APIENTRY* pfn_glVertexAttrib4dNV) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexAttrib4dNV QGLglVertexAttrib4dNV;
#define glVertexAttrib4dNV QGLglVertexAttrib4dNV

typedef void (APIENTRY* pfn_glVertexAttrib4dvNV) (GLuint, const GLdouble*);
extern pfn_glVertexAttrib4dvNV QGLglVertexAttrib4dvNV;
#define glVertexAttrib4dvNV QGLglVertexAttrib4dvNV

typedef void (APIENTRY* pfn_glVertexAttrib4fNV) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexAttrib4fNV QGLglVertexAttrib4fNV;
#define glVertexAttrib4fNV QGLglVertexAttrib4fNV

typedef void (APIENTRY* pfn_glVertexAttrib4fvNV) (GLuint, const GLfloat*);
extern pfn_glVertexAttrib4fvNV QGLglVertexAttrib4fvNV;
#define glVertexAttrib4fvNV QGLglVertexAttrib4fvNV

typedef void (APIENTRY* pfn_glVertexAttrib4sNV) (GLuint, GLshort, GLshort, GLshort, GLshort);
extern pfn_glVertexAttrib4sNV QGLglVertexAttrib4sNV;
#define glVertexAttrib4sNV QGLglVertexAttrib4sNV

typedef void (APIENTRY* pfn_glVertexAttrib4svNV) (GLuint, const GLshort*);
extern pfn_glVertexAttrib4svNV QGLglVertexAttrib4svNV;
#define glVertexAttrib4svNV QGLglVertexAttrib4svNV

typedef void (APIENTRY* pfn_glVertexAttrib4ubNV) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
extern pfn_glVertexAttrib4ubNV QGLglVertexAttrib4ubNV;
#define glVertexAttrib4ubNV QGLglVertexAttrib4ubNV

typedef void (APIENTRY* pfn_glVertexAttrib4ubvNV) (GLuint, const GLubyte*);
extern pfn_glVertexAttrib4ubvNV QGLglVertexAttrib4ubvNV;
#define glVertexAttrib4ubvNV QGLglVertexAttrib4ubvNV

typedef void (APIENTRY* pfn_glVertexAttribs1dvNV) (GLuint, GLsizei, const GLdouble*);
extern pfn_glVertexAttribs1dvNV QGLglVertexAttribs1dvNV;
#define glVertexAttribs1dvNV QGLglVertexAttribs1dvNV

typedef void (APIENTRY* pfn_glVertexAttribs1fvNV) (GLuint, GLsizei, const GLfloat*);
extern pfn_glVertexAttribs1fvNV QGLglVertexAttribs1fvNV;
#define glVertexAttribs1fvNV QGLglVertexAttribs1fvNV

typedef void (APIENTRY* pfn_glVertexAttribs1svNV) (GLuint, GLsizei, const GLshort*);
extern pfn_glVertexAttribs1svNV QGLglVertexAttribs1svNV;
#define glVertexAttribs1svNV QGLglVertexAttribs1svNV

typedef void (APIENTRY* pfn_glVertexAttribs2dvNV) (GLuint, GLsizei, const GLdouble*);
extern pfn_glVertexAttribs2dvNV QGLglVertexAttribs2dvNV;
#define glVertexAttribs2dvNV QGLglVertexAttribs2dvNV

typedef void (APIENTRY* pfn_glVertexAttribs2fvNV) (GLuint, GLsizei, const GLfloat*);
extern pfn_glVertexAttribs2fvNV QGLglVertexAttribs2fvNV;
#define glVertexAttribs2fvNV QGLglVertexAttribs2fvNV

typedef void (APIENTRY* pfn_glVertexAttribs2svNV) (GLuint, GLsizei, const GLshort*);
extern pfn_glVertexAttribs2svNV QGLglVertexAttribs2svNV;
#define glVertexAttribs2svNV QGLglVertexAttribs2svNV

typedef void (APIENTRY* pfn_glVertexAttribs3dvNV) (GLuint, GLsizei, const GLdouble*);
extern pfn_glVertexAttribs3dvNV QGLglVertexAttribs3dvNV;
#define glVertexAttribs3dvNV QGLglVertexAttribs3dvNV

typedef void (APIENTRY* pfn_glVertexAttribs3fvNV) (GLuint, GLsizei, const GLfloat*);
extern pfn_glVertexAttribs3fvNV QGLglVertexAttribs3fvNV;
#define glVertexAttribs3fvNV QGLglVertexAttribs3fvNV

typedef void (APIENTRY* pfn_glVertexAttribs3svNV) (GLuint, GLsizei, const GLshort*);
extern pfn_glVertexAttribs3svNV QGLglVertexAttribs3svNV;
#define glVertexAttribs3svNV QGLglVertexAttribs3svNV

typedef void (APIENTRY* pfn_glVertexAttribs4dvNV) (GLuint, GLsizei, const GLdouble*);
extern pfn_glVertexAttribs4dvNV QGLglVertexAttribs4dvNV;
#define glVertexAttribs4dvNV QGLglVertexAttribs4dvNV

typedef void (APIENTRY* pfn_glVertexAttribs4fvNV) (GLuint, GLsizei, const GLfloat*);
extern pfn_glVertexAttribs4fvNV QGLglVertexAttribs4fvNV;
#define glVertexAttribs4fvNV QGLglVertexAttribs4fvNV

typedef void (APIENTRY* pfn_glVertexAttribs4svNV) (GLuint, GLsizei, const GLshort*);
extern pfn_glVertexAttribs4svNV QGLglVertexAttribs4svNV;
#define glVertexAttribs4svNV QGLglVertexAttribs4svNV

typedef void (APIENTRY* pfn_glVertexAttribs4ubvNV) (GLuint, GLsizei, const GLubyte*);
extern pfn_glVertexAttribs4ubvNV QGLglVertexAttribs4ubvNV;
#define glVertexAttribs4ubvNV QGLglVertexAttribs4ubvNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_texture_coordinate_clamp;
extern int QGLNumFunctions_GL_SGIX_texture_coordinate_clamp;
#ifndef GL_SGIX_texture_coordinate_clamp
#define GL_SGIX_texture_coordinate_clamp
#define QGL_DEFINED_GL_SGIX_texture_coordinate_clamp
#define GL_TEXTURE_MAX_CLAMP_S_SGIX       0x8369
#define GL_TEXTURE_MAX_CLAMP_T_SGIX       0x836A
#define GL_TEXTURE_MAX_CLAMP_R_SGIX       0x836B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SGIX_scalebias_hint;
extern int QGLNumFunctions_GL_SGIX_scalebias_hint;
#ifndef GL_SGIX_scalebias_hint
#define GL_SGIX_scalebias_hint
#define QGL_DEFINED_GL_SGIX_scalebias_hint
#define GL_SCALEBIAS_HINT_SGIX            0x8322
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_OML_interlace;
extern int QGLNumFunctions_GL_OML_interlace;
#ifndef GL_OML_interlace
#define GL_OML_interlace
#define QGL_DEFINED_GL_OML_interlace
#define GL_INTERLACE_OML                  0x8980
#define GL_INTERLACE_READ_OML             0x8981
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_OML_subsample;
extern int QGLNumFunctions_GL_OML_subsample;
#ifndef GL_OML_subsample
#define GL_OML_subsample
#define QGL_DEFINED_GL_OML_subsample
#define GL_FORMAT_SUBSAMPLE_24_24_OML     0x8982
#define GL_FORMAT_SUBSAMPLE_244_244_OML   0x8983
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_OML_resample;
extern int QGLNumFunctions_GL_OML_resample;
#ifndef GL_OML_resample
#define GL_OML_resample
#define QGL_DEFINED_GL_OML_resample
#define GL_PACK_RESAMPLE_OML              0x8984
#define GL_UNPACK_RESAMPLE_OML            0x8985
#define GL_RESAMPLE_REPLICATE_OML         0x8986
#define GL_RESAMPLE_ZERO_FILL_OML         0x8987
#define GL_RESAMPLE_AVERAGE_OML           0x8988
#define GL_RESAMPLE_DECIMATE_OML          0x8989
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_copy_depth_to_color;
extern int QGLNumFunctions_GL_NV_copy_depth_to_color;
#ifndef GL_NV_copy_depth_to_color
#define GL_NV_copy_depth_to_color
#define QGL_DEFINED_GL_NV_copy_depth_to_color
#define GL_DEPTH_STENCIL_TO_RGBA_NV       0x886E
#define GL_DEPTH_STENCIL_TO_BGRA_NV       0x886F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_envmap_bumpmap;
extern int QGLNumFunctions_GL_ATI_envmap_bumpmap;
#ifndef GL_ATI_envmap_bumpmap
#define GL_ATI_envmap_bumpmap
#define QGL_DEFINED_GL_ATI_envmap_bumpmap
#define GL_BUMP_ROT_MATRIX_ATI            0x8775
#define GL_BUMP_ROT_MATRIX_SIZE_ATI       0x8776
#define GL_BUMP_NUM_TEX_UNITS_ATI         0x8777
#define GL_BUMP_TEX_UNITS_ATI             0x8778
#define GL_DUDV_ATI                       0x8779
#define GL_DU8DV8_ATI                     0x877A
#define GL_BUMP_ENVMAP_ATI                0x877B
#define GL_BUMP_TARGET_ATI                0x877C

typedef void (APIENTRY* pfn_glTexBumpParameterivATI) (GLenum, const GLint*);
extern pfn_glTexBumpParameterivATI QGLglTexBumpParameterivATI;
#define glTexBumpParameterivATI QGLglTexBumpParameterivATI

typedef void (APIENTRY* pfn_glTexBumpParameterfvATI) (GLenum, const GLfloat*);
extern pfn_glTexBumpParameterfvATI QGLglTexBumpParameterfvATI;
#define glTexBumpParameterfvATI QGLglTexBumpParameterfvATI

typedef void (APIENTRY* pfn_glGetTexBumpParameterivATI) (GLenum, GLint*);
extern pfn_glGetTexBumpParameterivATI QGLglGetTexBumpParameterivATI;
#define glGetTexBumpParameterivATI QGLglGetTexBumpParameterivATI

typedef void (APIENTRY* pfn_glGetTexBumpParameterfvATI) (GLenum, GLfloat*);
extern pfn_glGetTexBumpParameterfvATI QGLglGetTexBumpParameterfvATI;
#define glGetTexBumpParameterfvATI QGLglGetTexBumpParameterfvATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_fragment_shader;
extern int QGLNumFunctions_GL_ATI_fragment_shader;
#ifndef GL_ATI_fragment_shader
#define GL_ATI_fragment_shader
#define QGL_DEFINED_GL_ATI_fragment_shader
#define GL_FRAGMENT_SHADER_ATI                      0x8920
#define GL_REG_0_ATI                                0x8921
#define GL_REG_1_ATI                                0x8922
#define GL_REG_2_ATI                                0x8923
#define GL_REG_3_ATI                                0x8924
#define GL_REG_4_ATI                                0x8925
#define GL_REG_5_ATI                                0x8926
#define GL_REG_6_ATI                                0x8927
#define GL_REG_7_ATI                                0x8928
#define GL_REG_8_ATI                                0x8929
#define GL_REG_9_ATI                                0x892A
#define GL_REG_10_ATI                               0x892B
#define GL_REG_11_ATI                               0x892C
#define GL_REG_12_ATI                               0x892D
#define GL_REG_13_ATI                               0x892E
#define GL_REG_14_ATI                               0x892F
#define GL_REG_15_ATI                               0x8930
#define GL_REG_16_ATI                               0x8931
#define GL_REG_17_ATI                               0x8932
#define GL_REG_18_ATI                               0x8933
#define GL_REG_19_ATI                               0x8934
#define GL_REG_20_ATI                               0x8935
#define GL_REG_21_ATI                               0x8936
#define GL_REG_22_ATI                               0x8937
#define GL_REG_23_ATI                               0x8938
#define GL_REG_24_ATI                               0x8939
#define GL_REG_25_ATI                               0x893A
#define GL_REG_26_ATI                               0x893B
#define GL_REG_27_ATI                               0x893C
#define GL_REG_28_ATI                               0x893D
#define GL_REG_29_ATI                               0x893E
#define GL_REG_30_ATI                               0x893F
#define GL_REG_31_ATI                               0x8940
#define GL_CON_0_ATI                                0x8941
#define GL_CON_1_ATI                                0x8942
#define GL_CON_2_ATI                                0x8943
#define GL_CON_3_ATI                                0x8944
#define GL_CON_4_ATI                                0x8945
#define GL_CON_5_ATI                                0x8946
#define GL_CON_6_ATI                                0x8947
#define GL_CON_7_ATI                                0x8948
#define GL_CON_8_ATI                                0x8949
#define GL_CON_9_ATI                                0x894A
#define GL_CON_10_ATI                               0x894B
#define GL_CON_11_ATI                               0x894C
#define GL_CON_12_ATI                               0x894D
#define GL_CON_13_ATI                               0x894E
#define GL_CON_14_ATI                               0x894F
#define GL_CON_15_ATI                               0x8950
#define GL_CON_16_ATI                               0x8951
#define GL_CON_17_ATI                               0x8952
#define GL_CON_18_ATI                               0x8953
#define GL_CON_19_ATI                               0x8954
#define GL_CON_20_ATI                               0x8955
#define GL_CON_21_ATI                               0x8956
#define GL_CON_22_ATI                               0x8957
#define GL_CON_23_ATI                               0x8958
#define GL_CON_24_ATI                               0x8959
#define GL_CON_25_ATI                               0x895A
#define GL_CON_26_ATI                               0x895B
#define GL_CON_27_ATI                               0x895C
#define GL_CON_28_ATI                               0x895D
#define GL_CON_29_ATI                               0x895E
#define GL_CON_30_ATI                               0x895F
#define GL_CON_31_ATI                               0x8960
#define GL_MOV_ATI                                  0x8961
#define GL_ADD_ATI                                  0x8963
#define GL_MUL_ATI                                  0x8964
#define GL_SUB_ATI                                  0x8965
#define GL_DOT3_ATI                                 0x8966
#define GL_DOT4_ATI                                 0x8967
#define GL_MAD_ATI                                  0x8968
#define GL_LERP_ATI                                 0x8969
#define GL_CND_ATI                                  0x896A
#define GL_CND0_ATI                                 0x896B
#define GL_DOT2_ADD_ATI                             0x896C
#define GL_SECONDARY_INTERPOLATOR_ATI               0x896D
#define GL_NUM_FRAGMENT_REGISTERS_ATI               0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_ATI               0x896F
#define GL_NUM_PASSES_ATI                           0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI            0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI               0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI    0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_ATI              0x8974
#define GL_COLOR_ALPHA_PAIRING_ATI                  0x8975
#define GL_SWIZZLE_STR_ATI                          0x8976
#define GL_SWIZZLE_STQ_ATI                          0x8977
#define GL_SWIZZLE_STR_DR_ATI                       0x8978
#define GL_SWIZZLE_STQ_DQ_ATI                       0x8979
#define GL_SWIZZLE_STRQ_ATI                         0x897A
#define GL_SWIZZLE_STRQ_DQ_ATI                      0x897B
#define GL_RED_BIT_ATI                              0x00000001
#define GL_GREEN_BIT_ATI                            0x00000002
#define GL_BLUE_BIT_ATI                             0x00000004
#define GL_2X_BIT_ATI                               0x00000001
#define GL_4X_BIT_ATI                               0x00000002
#define GL_8X_BIT_ATI                               0x00000004
#define GL_HALF_BIT_ATI                             0x00000008
#define GL_QUARTER_BIT_ATI                          0x00000010
#define GL_EIGHTH_BIT_ATI                           0x00000020
#define GL_SATURATE_BIT_ATI                         0x00000040
#define GL_COMP_BIT_ATI                             0x00000002
#define GL_NEGATE_BIT_ATI                           0x00000004
#define GL_BIAS_BIT_ATI                             0x00000008

typedef GLuint (APIENTRY* pfn_glGenFragmentShadersATI) (GLuint);
extern pfn_glGenFragmentShadersATI QGLglGenFragmentShadersATI;
#define glGenFragmentShadersATI QGLglGenFragmentShadersATI

typedef void (APIENTRY* pfn_glBindFragmentShaderATI) (GLuint);
extern pfn_glBindFragmentShaderATI QGLglBindFragmentShaderATI;
#define glBindFragmentShaderATI QGLglBindFragmentShaderATI

typedef void (APIENTRY* pfn_glDeleteFragmentShaderATI) (GLuint);
extern pfn_glDeleteFragmentShaderATI QGLglDeleteFragmentShaderATI;
#define glDeleteFragmentShaderATI QGLglDeleteFragmentShaderATI

typedef void (APIENTRY* pfn_glBeginFragmentShaderATI) (void);
extern pfn_glBeginFragmentShaderATI QGLglBeginFragmentShaderATI;
#define glBeginFragmentShaderATI QGLglBeginFragmentShaderATI

typedef void (APIENTRY* pfn_glEndFragmentShaderATI) (void);
extern pfn_glEndFragmentShaderATI QGLglEndFragmentShaderATI;
#define glEndFragmentShaderATI QGLglEndFragmentShaderATI

typedef void (APIENTRY* pfn_glPassTexCoordATI) (GLuint, GLuint, GLenum);
extern pfn_glPassTexCoordATI QGLglPassTexCoordATI;
#define glPassTexCoordATI QGLglPassTexCoordATI

typedef void (APIENTRY* pfn_glSampleMapATI) (GLuint, GLuint, GLenum);
extern pfn_glSampleMapATI QGLglSampleMapATI;
#define glSampleMapATI QGLglSampleMapATI

typedef void (APIENTRY* pfn_glColorFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glColorFragmentOp1ATI QGLglColorFragmentOp1ATI;
#define glColorFragmentOp1ATI QGLglColorFragmentOp1ATI

typedef void (APIENTRY* pfn_glColorFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glColorFragmentOp2ATI QGLglColorFragmentOp2ATI;
#define glColorFragmentOp2ATI QGLglColorFragmentOp2ATI

typedef void (APIENTRY* pfn_glColorFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glColorFragmentOp3ATI QGLglColorFragmentOp3ATI;
#define glColorFragmentOp3ATI QGLglColorFragmentOp3ATI

typedef void (APIENTRY* pfn_glAlphaFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glAlphaFragmentOp1ATI QGLglAlphaFragmentOp1ATI;
#define glAlphaFragmentOp1ATI QGLglAlphaFragmentOp1ATI

typedef void (APIENTRY* pfn_glAlphaFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glAlphaFragmentOp2ATI QGLglAlphaFragmentOp2ATI;
#define glAlphaFragmentOp2ATI QGLglAlphaFragmentOp2ATI

typedef void (APIENTRY* pfn_glAlphaFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern pfn_glAlphaFragmentOp3ATI QGLglAlphaFragmentOp3ATI;
#define glAlphaFragmentOp3ATI QGLglAlphaFragmentOp3ATI

typedef void (APIENTRY* pfn_glSetFragmentShaderConstantATI) (GLuint, const GLfloat*);
extern pfn_glSetFragmentShaderConstantATI QGLglSetFragmentShaderConstantATI;
#define glSetFragmentShaderConstantATI QGLglSetFragmentShaderConstantATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_pn_triangles;
extern int QGLNumFunctions_GL_ATI_pn_triangles;
#ifndef GL_ATI_pn_triangles
#define GL_ATI_pn_triangles
#define QGL_DEFINED_GL_ATI_pn_triangles
#define GL_PN_TRIANGLES_ATI                         0x87F0
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI   0x87F1
#define GL_PN_TRIANGLES_POINT_MODE_ATI              0x87F2
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI             0x87F3
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI       0x87F4
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI       0x87F5
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI        0x87F6
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI      0x87F7
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI   0x87F8

typedef void (APIENTRY* pfn_glPNTrianglesiATI) (GLenum, GLint);
extern pfn_glPNTrianglesiATI QGLglPNTrianglesiATI;
#define glPNTrianglesiATI QGLglPNTrianglesiATI

typedef void (APIENTRY* pfn_glPNTrianglesfATI) (GLenum, GLfloat);
extern pfn_glPNTrianglesfATI QGLglPNTrianglesfATI;
#define glPNTrianglesfATI QGLglPNTrianglesfATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_vertex_array_object;
extern int QGLNumFunctions_GL_ATI_vertex_array_object;
#ifndef GL_ATI_vertex_array_object
#define GL_ATI_vertex_array_object
#define QGL_DEFINED_GL_ATI_vertex_array_object
#define GL_STATIC_ATI                     0x8760
#define GL_DYNAMIC_ATI                    0x8761
#define GL_PRESERVE_ATI                   0x8762
#define GL_DISCARD_ATI                    0x8763
#define GL_OBJECT_BUFFER_SIZE_ATI         0x8764
#define GL_OBJECT_BUFFER_USAGE_ATI        0x8765
#define GL_ARRAY_OBJECT_BUFFER_ATI        0x8766
#define GL_ARRAY_OBJECT_OFFSET_ATI        0x8767

typedef GLuint (APIENTRY* pfn_glNewObjectBufferATI) (GLsizei, const GLvoid*, GLenum);
extern pfn_glNewObjectBufferATI QGLglNewObjectBufferATI;
#define glNewObjectBufferATI QGLglNewObjectBufferATI

typedef GLboolean (APIENTRY* pfn_glIsObjectBufferATI) (GLuint);
extern pfn_glIsObjectBufferATI QGLglIsObjectBufferATI;
#define glIsObjectBufferATI QGLglIsObjectBufferATI

typedef void (APIENTRY* pfn_glUpdateObjectBufferATI) (GLuint, GLuint, GLsizei, const GLvoid*, GLenum);
extern pfn_glUpdateObjectBufferATI QGLglUpdateObjectBufferATI;
#define glUpdateObjectBufferATI QGLglUpdateObjectBufferATI

typedef void (APIENTRY* pfn_glGetObjectBufferfvATI) (GLuint, GLenum, GLfloat*);
extern pfn_glGetObjectBufferfvATI QGLglGetObjectBufferfvATI;
#define glGetObjectBufferfvATI QGLglGetObjectBufferfvATI

typedef void (APIENTRY* pfn_glGetObjectBufferivATI) (GLuint, GLenum, GLint*);
extern pfn_glGetObjectBufferivATI QGLglGetObjectBufferivATI;
#define glGetObjectBufferivATI QGLglGetObjectBufferivATI

typedef void (APIENTRY* pfn_glFreeObjectBufferATI) (GLuint);
extern pfn_glFreeObjectBufferATI QGLglFreeObjectBufferATI;
#define glFreeObjectBufferATI QGLglFreeObjectBufferATI

typedef void (APIENTRY* pfn_glArrayObjectATI) (GLenum, GLint, GLenum, GLsizei, GLuint, GLuint);
extern pfn_glArrayObjectATI QGLglArrayObjectATI;
#define glArrayObjectATI QGLglArrayObjectATI

typedef void (APIENTRY* pfn_glGetArrayObjectfvATI) (GLenum, GLenum, GLfloat*);
extern pfn_glGetArrayObjectfvATI QGLglGetArrayObjectfvATI;
#define glGetArrayObjectfvATI QGLglGetArrayObjectfvATI

typedef void (APIENTRY* pfn_glGetArrayObjectivATI) (GLenum, GLenum, GLint*);
extern pfn_glGetArrayObjectivATI QGLglGetArrayObjectivATI;
#define glGetArrayObjectivATI QGLglGetArrayObjectivATI

typedef void (APIENTRY* pfn_glVariantArrayObjectATI) (GLuint, GLenum, GLsizei, GLuint, GLuint);
extern pfn_glVariantArrayObjectATI QGLglVariantArrayObjectATI;
#define glVariantArrayObjectATI QGLglVariantArrayObjectATI

typedef void (APIENTRY* pfn_glGetVariantArrayObjectfvATI) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVariantArrayObjectfvATI QGLglGetVariantArrayObjectfvATI;
#define glGetVariantArrayObjectfvATI QGLglGetVariantArrayObjectfvATI

typedef void (APIENTRY* pfn_glGetVariantArrayObjectivATI) (GLuint, GLenum, GLint*);
extern pfn_glGetVariantArrayObjectivATI QGLglGetVariantArrayObjectivATI;
#define glGetVariantArrayObjectivATI QGLglGetVariantArrayObjectivATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_vertex_shader;
extern int QGLNumFunctions_GL_EXT_vertex_shader;
#ifndef GL_EXT_vertex_shader
#define GL_EXT_vertex_shader
#define QGL_DEFINED_GL_EXT_vertex_shader
#define GL_VERTEX_SHADER_EXT                                0x8780
#define GL_VERTEX_SHADER_BINDING_EXT                        0x8781
#define GL_OP_INDEX_EXT                                     0x8782
#define GL_OP_NEGATE_EXT                                    0x8783
#define GL_OP_DOT3_EXT                                      0x8784
#define GL_OP_DOT4_EXT                                      0x8785
#define GL_OP_MUL_EXT                                       0x8786
#define GL_OP_ADD_EXT                                       0x8787
#define GL_OP_MADD_EXT                                      0x8788
#define GL_OP_FRAC_EXT                                      0x8789
#define GL_OP_MAX_EXT                                       0x878A
#define GL_OP_MIN_EXT                                       0x878B
#define GL_OP_SET_GE_EXT                                    0x878C
#define GL_OP_SET_LT_EXT                                    0x878D
#define GL_OP_CLAMP_EXT                                     0x878E
#define GL_OP_FLOOR_EXT                                     0x878F
#define GL_OP_ROUND_EXT                                     0x8790
#define GL_OP_EXP_BASE_2_EXT                                0x8791
#define GL_OP_LOG_BASE_2_EXT                                0x8792
#define GL_OP_POWER_EXT                                     0x8793
#define GL_OP_RECIP_EXT                                     0x8794
#define GL_OP_RECIP_SQRT_EXT                                0x8795
#define GL_OP_SUB_EXT                                       0x8796
#define GL_OP_CROSS_PRODUCT_EXT                             0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT                           0x8798
#define GL_OP_MOV_EXT                                       0x8799
#define GL_OUTPUT_VERTEX_EXT                                0x879A
#define GL_OUTPUT_COLOR0_EXT                                0x879B
#define GL_OUTPUT_COLOR1_EXT                                0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT                        0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT                        0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT                        0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT                        0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT                        0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT                        0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT                        0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT                        0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT                        0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT                        0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT                       0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT                       0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT                       0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT                       0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT                       0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT                       0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT                       0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT                       0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT                       0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT                       0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT                       0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT                       0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT                       0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT                       0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT                       0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT                       0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT                       0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT                       0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT                       0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT                       0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT                       0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT                       0x87BC
#define GL_OUTPUT_FOG_EXT                                   0x87BD
#define GL_SCALAR_EXT                                       0x87BE
#define GL_VECTOR_EXT                                       0x87BF
#define GL_MATRIX_EXT                                       0x87C0
#define GL_VARIANT_EXT                                      0x87C1
#define GL_INVARIANT_EXT                                    0x87C2
#define GL_LOCAL_CONSTANT_EXT                               0x87C3
#define GL_LOCAL_EXT                                        0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT               0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT                   0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT                 0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT            0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT                     0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT     0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT         0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT  0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT       0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT           0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT                   0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT                       0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT                     0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT                0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT                         0x87D3
#define GL_VERTEX_SHADER_OPTIMIZED_EXT                      0x87D4
#define GL_X_EXT                                            0x87D5
#define GL_Y_EXT                                            0x87D6
#define GL_Z_EXT                                            0x87D7
#define GL_W_EXT                                            0x87D8
#define GL_NEGATIVE_X_EXT                                   0x87D9
#define GL_NEGATIVE_Y_EXT                                   0x87DA
#define GL_NEGATIVE_Z_EXT                                   0x87DB
#define GL_NEGATIVE_W_EXT                                   0x87DC
#define GL_ZERO_EXT                                         0x87DD
#define GL_ONE_EXT                                          0x87DE
#define GL_NEGATIVE_ONE_EXT                                 0x87DF
#define GL_NORMALIZED_RANGE_EXT                             0x87E0
#define GL_FULL_RANGE_EXT                                   0x87E1
#define GL_CURRENT_VERTEX_EXT                               0x87E2
#define GL_MVP_MATRIX_EXT                                   0x87E3
#define GL_VARIANT_VALUE_EXT                                0x87E4
#define GL_VARIANT_DATATYPE_EXT                             0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT                         0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT                           0x87E7
#define GL_VARIANT_ARRAY_EXT                                0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT                        0x87E9
#define GL_INVARIANT_VALUE_EXT                              0x87EA
#define GL_INVARIANT_DATATYPE_EXT                           0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT                         0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT                      0x87ED

typedef void (APIENTRY* pfn_glBeginVertexShaderEXT) (void);
extern pfn_glBeginVertexShaderEXT QGLglBeginVertexShaderEXT;
#define glBeginVertexShaderEXT QGLglBeginVertexShaderEXT

typedef void (APIENTRY* pfn_glEndVertexShaderEXT) (void);
extern pfn_glEndVertexShaderEXT QGLglEndVertexShaderEXT;
#define glEndVertexShaderEXT QGLglEndVertexShaderEXT

typedef void (APIENTRY* pfn_glBindVertexShaderEXT) (GLuint);
extern pfn_glBindVertexShaderEXT QGLglBindVertexShaderEXT;
#define glBindVertexShaderEXT QGLglBindVertexShaderEXT

typedef GLuint (APIENTRY* pfn_glGenVertexShadersEXT) (GLuint);
extern pfn_glGenVertexShadersEXT QGLglGenVertexShadersEXT;
#define glGenVertexShadersEXT QGLglGenVertexShadersEXT

typedef void (APIENTRY* pfn_glDeleteVertexShaderEXT) (GLuint);
extern pfn_glDeleteVertexShaderEXT QGLglDeleteVertexShaderEXT;
#define glDeleteVertexShaderEXT QGLglDeleteVertexShaderEXT

typedef void (APIENTRY* pfn_glShaderOp1EXT) (GLenum, GLuint, GLuint);
extern pfn_glShaderOp1EXT QGLglShaderOp1EXT;
#define glShaderOp1EXT QGLglShaderOp1EXT

typedef void (APIENTRY* pfn_glShaderOp2EXT) (GLenum, GLuint, GLuint, GLuint);
extern pfn_glShaderOp2EXT QGLglShaderOp2EXT;
#define glShaderOp2EXT QGLglShaderOp2EXT

typedef void (APIENTRY* pfn_glShaderOp3EXT) (GLenum, GLuint, GLuint, GLuint, GLuint);
extern pfn_glShaderOp3EXT QGLglShaderOp3EXT;
#define glShaderOp3EXT QGLglShaderOp3EXT

typedef void (APIENTRY* pfn_glSwizzleEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
extern pfn_glSwizzleEXT QGLglSwizzleEXT;
#define glSwizzleEXT QGLglSwizzleEXT

typedef void (APIENTRY* pfn_glWriteMaskEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
extern pfn_glWriteMaskEXT QGLglWriteMaskEXT;
#define glWriteMaskEXT QGLglWriteMaskEXT

typedef void (APIENTRY* pfn_glInsertComponentEXT) (GLuint, GLuint, GLuint);
extern pfn_glInsertComponentEXT QGLglInsertComponentEXT;
#define glInsertComponentEXT QGLglInsertComponentEXT

typedef void (APIENTRY* pfn_glExtractComponentEXT) (GLuint, GLuint, GLuint);
extern pfn_glExtractComponentEXT QGLglExtractComponentEXT;
#define glExtractComponentEXT QGLglExtractComponentEXT

typedef GLuint (APIENTRY* pfn_glGenSymbolsEXT) (GLenum, GLenum, GLenum, GLuint);
extern pfn_glGenSymbolsEXT QGLglGenSymbolsEXT;
#define glGenSymbolsEXT QGLglGenSymbolsEXT

typedef void (APIENTRY* pfn_glSetInvariantEXT) (GLuint, GLenum, const GLvoid*);
extern pfn_glSetInvariantEXT QGLglSetInvariantEXT;
#define glSetInvariantEXT QGLglSetInvariantEXT

typedef void (APIENTRY* pfn_glSetLocalConstantEXT) (GLuint, GLenum, const GLvoid*);
extern pfn_glSetLocalConstantEXT QGLglSetLocalConstantEXT;
#define glSetLocalConstantEXT QGLglSetLocalConstantEXT

typedef void (APIENTRY* pfn_glVariantbvEXT) (GLuint, const GLbyte*);
extern pfn_glVariantbvEXT QGLglVariantbvEXT;
#define glVariantbvEXT QGLglVariantbvEXT

typedef void (APIENTRY* pfn_glVariantsvEXT) (GLuint, const GLshort*);
extern pfn_glVariantsvEXT QGLglVariantsvEXT;
#define glVariantsvEXT QGLglVariantsvEXT

typedef void (APIENTRY* pfn_glVariantivEXT) (GLuint, const GLint*);
extern pfn_glVariantivEXT QGLglVariantivEXT;
#define glVariantivEXT QGLglVariantivEXT

typedef void (APIENTRY* pfn_glVariantfvEXT) (GLuint, const GLfloat*);
extern pfn_glVariantfvEXT QGLglVariantfvEXT;
#define glVariantfvEXT QGLglVariantfvEXT

typedef void (APIENTRY* pfn_glVariantdvEXT) (GLuint, const GLdouble*);
extern pfn_glVariantdvEXT QGLglVariantdvEXT;
#define glVariantdvEXT QGLglVariantdvEXT

typedef void (APIENTRY* pfn_glVariantubvEXT) (GLuint, const GLubyte*);
extern pfn_glVariantubvEXT QGLglVariantubvEXT;
#define glVariantubvEXT QGLglVariantubvEXT

typedef void (APIENTRY* pfn_glVariantusvEXT) (GLuint, const GLushort*);
extern pfn_glVariantusvEXT QGLglVariantusvEXT;
#define glVariantusvEXT QGLglVariantusvEXT

typedef void (APIENTRY* pfn_glVariantuivEXT) (GLuint, const GLuint*);
extern pfn_glVariantuivEXT QGLglVariantuivEXT;
#define glVariantuivEXT QGLglVariantuivEXT

typedef void (APIENTRY* pfn_glVariantPointerEXT) (GLuint, GLenum, GLuint, const GLvoid*);
extern pfn_glVariantPointerEXT QGLglVariantPointerEXT;
#define glVariantPointerEXT QGLglVariantPointerEXT

typedef void (APIENTRY* pfn_glEnableVariantClientStateEXT) (GLuint);
extern pfn_glEnableVariantClientStateEXT QGLglEnableVariantClientStateEXT;
#define glEnableVariantClientStateEXT QGLglEnableVariantClientStateEXT

typedef void (APIENTRY* pfn_glDisableVariantClientStateEXT) (GLuint);
extern pfn_glDisableVariantClientStateEXT QGLglDisableVariantClientStateEXT;
#define glDisableVariantClientStateEXT QGLglDisableVariantClientStateEXT

typedef GLuint (APIENTRY* pfn_glBindLightParameterEXT) (GLenum, GLenum);
extern pfn_glBindLightParameterEXT QGLglBindLightParameterEXT;
#define glBindLightParameterEXT QGLglBindLightParameterEXT

typedef GLuint (APIENTRY* pfn_glBindMaterialParameterEXT) (GLenum, GLenum);
extern pfn_glBindMaterialParameterEXT QGLglBindMaterialParameterEXT;
#define glBindMaterialParameterEXT QGLglBindMaterialParameterEXT

typedef GLuint (APIENTRY* pfn_glBindTexGenParameterEXT) (GLenum, GLenum, GLenum);
extern pfn_glBindTexGenParameterEXT QGLglBindTexGenParameterEXT;
#define glBindTexGenParameterEXT QGLglBindTexGenParameterEXT

typedef GLuint (APIENTRY* pfn_glBindTextureUnitParameterEXT) (GLenum, GLenum);
extern pfn_glBindTextureUnitParameterEXT QGLglBindTextureUnitParameterEXT;
#define glBindTextureUnitParameterEXT QGLglBindTextureUnitParameterEXT

typedef GLuint (APIENTRY* pfn_glBindParameterEXT) (GLenum);
extern pfn_glBindParameterEXT QGLglBindParameterEXT;
#define glBindParameterEXT QGLglBindParameterEXT

typedef GLboolean (APIENTRY* pfn_glIsVariantEnabledEXT) (GLuint, GLenum);
extern pfn_glIsVariantEnabledEXT QGLglIsVariantEnabledEXT;
#define glIsVariantEnabledEXT QGLglIsVariantEnabledEXT

typedef void (APIENTRY* pfn_glGetVariantBooleanvEXT) (GLuint, GLenum, GLboolean*);
extern pfn_glGetVariantBooleanvEXT QGLglGetVariantBooleanvEXT;
#define glGetVariantBooleanvEXT QGLglGetVariantBooleanvEXT

typedef void (APIENTRY* pfn_glGetVariantIntegervEXT) (GLuint, GLenum, GLint*);
extern pfn_glGetVariantIntegervEXT QGLglGetVariantIntegervEXT;
#define glGetVariantIntegervEXT QGLglGetVariantIntegervEXT

typedef void (APIENTRY* pfn_glGetVariantFloatvEXT) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVariantFloatvEXT QGLglGetVariantFloatvEXT;
#define glGetVariantFloatvEXT QGLglGetVariantFloatvEXT

typedef void (APIENTRY* pfn_glGetVariantPointervEXT) (GLuint, GLenum, GLvoid**);
extern pfn_glGetVariantPointervEXT QGLglGetVariantPointervEXT;
#define glGetVariantPointervEXT QGLglGetVariantPointervEXT

typedef void (APIENTRY* pfn_glGetInvariantBooleanvEXT) (GLuint, GLenum, GLboolean*);
extern pfn_glGetInvariantBooleanvEXT QGLglGetInvariantBooleanvEXT;
#define glGetInvariantBooleanvEXT QGLglGetInvariantBooleanvEXT

typedef void (APIENTRY* pfn_glGetInvariantIntegervEXT) (GLuint, GLenum, GLint*);
extern pfn_glGetInvariantIntegervEXT QGLglGetInvariantIntegervEXT;
#define glGetInvariantIntegervEXT QGLglGetInvariantIntegervEXT

typedef void (APIENTRY* pfn_glGetInvariantFloatvEXT) (GLuint, GLenum, GLfloat*);
extern pfn_glGetInvariantFloatvEXT QGLglGetInvariantFloatvEXT;
#define glGetInvariantFloatvEXT QGLglGetInvariantFloatvEXT

typedef void (APIENTRY* pfn_glGetLocalConstantBooleanvEXT) (GLuint, GLenum, GLboolean*);
extern pfn_glGetLocalConstantBooleanvEXT QGLglGetLocalConstantBooleanvEXT;
#define glGetLocalConstantBooleanvEXT QGLglGetLocalConstantBooleanvEXT

typedef void (APIENTRY* pfn_glGetLocalConstantIntegervEXT) (GLuint, GLenum, GLint*);
extern pfn_glGetLocalConstantIntegervEXT QGLglGetLocalConstantIntegervEXT;
#define glGetLocalConstantIntegervEXT QGLglGetLocalConstantIntegervEXT

typedef void (APIENTRY* pfn_glGetLocalConstantFloatvEXT) (GLuint, GLenum, GLfloat*);
extern pfn_glGetLocalConstantFloatvEXT QGLglGetLocalConstantFloatvEXT;
#define glGetLocalConstantFloatvEXT QGLglGetLocalConstantFloatvEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_vertex_streams;
extern int QGLNumFunctions_GL_ATI_vertex_streams;
#ifndef GL_ATI_vertex_streams
#define GL_ATI_vertex_streams
#define QGL_DEFINED_GL_ATI_vertex_streams
#define GL_MAX_VERTEX_STREAMS_ATI         0x876B
#define GL_VERTEX_STREAM0_ATI             0x876C
#define GL_VERTEX_STREAM1_ATI             0x876D
#define GL_VERTEX_STREAM2_ATI             0x876E
#define GL_VERTEX_STREAM3_ATI             0x876F
#define GL_VERTEX_STREAM4_ATI             0x8770
#define GL_VERTEX_STREAM5_ATI             0x8771
#define GL_VERTEX_STREAM6_ATI             0x8772
#define GL_VERTEX_STREAM7_ATI             0x8773
#define GL_VERTEX_SOURCE_ATI              0x8774

typedef void (APIENTRY* pfn_glVertexStream1sATI) (GLenum, GLshort);
extern pfn_glVertexStream1sATI QGLglVertexStream1sATI;
#define glVertexStream1sATI QGLglVertexStream1sATI

typedef void (APIENTRY* pfn_glVertexStream1svATI) (GLenum, const GLshort*);
extern pfn_glVertexStream1svATI QGLglVertexStream1svATI;
#define glVertexStream1svATI QGLglVertexStream1svATI

typedef void (APIENTRY* pfn_glVertexStream1iATI) (GLenum, GLint);
extern pfn_glVertexStream1iATI QGLglVertexStream1iATI;
#define glVertexStream1iATI QGLglVertexStream1iATI

typedef void (APIENTRY* pfn_glVertexStream1ivATI) (GLenum, const GLint*);
extern pfn_glVertexStream1ivATI QGLglVertexStream1ivATI;
#define glVertexStream1ivATI QGLglVertexStream1ivATI

typedef void (APIENTRY* pfn_glVertexStream1fATI) (GLenum, GLfloat);
extern pfn_glVertexStream1fATI QGLglVertexStream1fATI;
#define glVertexStream1fATI QGLglVertexStream1fATI

typedef void (APIENTRY* pfn_glVertexStream1fvATI) (GLenum, const GLfloat*);
extern pfn_glVertexStream1fvATI QGLglVertexStream1fvATI;
#define glVertexStream1fvATI QGLglVertexStream1fvATI

typedef void (APIENTRY* pfn_glVertexStream1dATI) (GLenum, GLdouble);
extern pfn_glVertexStream1dATI QGLglVertexStream1dATI;
#define glVertexStream1dATI QGLglVertexStream1dATI

typedef void (APIENTRY* pfn_glVertexStream1dvATI) (GLenum, const GLdouble*);
extern pfn_glVertexStream1dvATI QGLglVertexStream1dvATI;
#define glVertexStream1dvATI QGLglVertexStream1dvATI

typedef void (APIENTRY* pfn_glVertexStream2sATI) (GLenum, GLshort, GLshort);
extern pfn_glVertexStream2sATI QGLglVertexStream2sATI;
#define glVertexStream2sATI QGLglVertexStream2sATI

typedef void (APIENTRY* pfn_glVertexStream2svATI) (GLenum, const GLshort*);
extern pfn_glVertexStream2svATI QGLglVertexStream2svATI;
#define glVertexStream2svATI QGLglVertexStream2svATI

typedef void (APIENTRY* pfn_glVertexStream2iATI) (GLenum, GLint, GLint);
extern pfn_glVertexStream2iATI QGLglVertexStream2iATI;
#define glVertexStream2iATI QGLglVertexStream2iATI

typedef void (APIENTRY* pfn_glVertexStream2ivATI) (GLenum, const GLint*);
extern pfn_glVertexStream2ivATI QGLglVertexStream2ivATI;
#define glVertexStream2ivATI QGLglVertexStream2ivATI

typedef void (APIENTRY* pfn_glVertexStream2fATI) (GLenum, GLfloat, GLfloat);
extern pfn_glVertexStream2fATI QGLglVertexStream2fATI;
#define glVertexStream2fATI QGLglVertexStream2fATI

typedef void (APIENTRY* pfn_glVertexStream2fvATI) (GLenum, const GLfloat*);
extern pfn_glVertexStream2fvATI QGLglVertexStream2fvATI;
#define glVertexStream2fvATI QGLglVertexStream2fvATI

typedef void (APIENTRY* pfn_glVertexStream2dATI) (GLenum, GLdouble, GLdouble);
extern pfn_glVertexStream2dATI QGLglVertexStream2dATI;
#define glVertexStream2dATI QGLglVertexStream2dATI

typedef void (APIENTRY* pfn_glVertexStream2dvATI) (GLenum, const GLdouble*);
extern pfn_glVertexStream2dvATI QGLglVertexStream2dvATI;
#define glVertexStream2dvATI QGLglVertexStream2dvATI

typedef void (APIENTRY* pfn_glVertexStream3sATI) (GLenum, GLshort, GLshort, GLshort);
extern pfn_glVertexStream3sATI QGLglVertexStream3sATI;
#define glVertexStream3sATI QGLglVertexStream3sATI

typedef void (APIENTRY* pfn_glVertexStream3svATI) (GLenum, const GLshort*);
extern pfn_glVertexStream3svATI QGLglVertexStream3svATI;
#define glVertexStream3svATI QGLglVertexStream3svATI

typedef void (APIENTRY* pfn_glVertexStream3iATI) (GLenum, GLint, GLint, GLint);
extern pfn_glVertexStream3iATI QGLglVertexStream3iATI;
#define glVertexStream3iATI QGLglVertexStream3iATI

typedef void (APIENTRY* pfn_glVertexStream3ivATI) (GLenum, const GLint*);
extern pfn_glVertexStream3ivATI QGLglVertexStream3ivATI;
#define glVertexStream3ivATI QGLglVertexStream3ivATI

typedef void (APIENTRY* pfn_glVertexStream3fATI) (GLenum, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexStream3fATI QGLglVertexStream3fATI;
#define glVertexStream3fATI QGLglVertexStream3fATI

typedef void (APIENTRY* pfn_glVertexStream3fvATI) (GLenum, const GLfloat*);
extern pfn_glVertexStream3fvATI QGLglVertexStream3fvATI;
#define glVertexStream3fvATI QGLglVertexStream3fvATI

typedef void (APIENTRY* pfn_glVertexStream3dATI) (GLenum, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexStream3dATI QGLglVertexStream3dATI;
#define glVertexStream3dATI QGLglVertexStream3dATI

typedef void (APIENTRY* pfn_glVertexStream3dvATI) (GLenum, const GLdouble*);
extern pfn_glVertexStream3dvATI QGLglVertexStream3dvATI;
#define glVertexStream3dvATI QGLglVertexStream3dvATI

typedef void (APIENTRY* pfn_glVertexStream4sATI) (GLenum, GLshort, GLshort, GLshort, GLshort);
extern pfn_glVertexStream4sATI QGLglVertexStream4sATI;
#define glVertexStream4sATI QGLglVertexStream4sATI

typedef void (APIENTRY* pfn_glVertexStream4svATI) (GLenum, const GLshort*);
extern pfn_glVertexStream4svATI QGLglVertexStream4svATI;
#define glVertexStream4svATI QGLglVertexStream4svATI

typedef void (APIENTRY* pfn_glVertexStream4iATI) (GLenum, GLint, GLint, GLint, GLint);
extern pfn_glVertexStream4iATI QGLglVertexStream4iATI;
#define glVertexStream4iATI QGLglVertexStream4iATI

typedef void (APIENTRY* pfn_glVertexStream4ivATI) (GLenum, const GLint*);
extern pfn_glVertexStream4ivATI QGLglVertexStream4ivATI;
#define glVertexStream4ivATI QGLglVertexStream4ivATI

typedef void (APIENTRY* pfn_glVertexStream4fATI) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glVertexStream4fATI QGLglVertexStream4fATI;
#define glVertexStream4fATI QGLglVertexStream4fATI

typedef void (APIENTRY* pfn_glVertexStream4fvATI) (GLenum, const GLfloat*);
extern pfn_glVertexStream4fvATI QGLglVertexStream4fvATI;
#define glVertexStream4fvATI QGLglVertexStream4fvATI

typedef void (APIENTRY* pfn_glVertexStream4dATI) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glVertexStream4dATI QGLglVertexStream4dATI;
#define glVertexStream4dATI QGLglVertexStream4dATI

typedef void (APIENTRY* pfn_glVertexStream4dvATI) (GLenum, const GLdouble*);
extern pfn_glVertexStream4dvATI QGLglVertexStream4dvATI;
#define glVertexStream4dvATI QGLglVertexStream4dvATI

typedef void (APIENTRY* pfn_glNormalStream3bATI) (GLenum, GLbyte, GLbyte, GLbyte);
extern pfn_glNormalStream3bATI QGLglNormalStream3bATI;
#define glNormalStream3bATI QGLglNormalStream3bATI

typedef void (APIENTRY* pfn_glNormalStream3bvATI) (GLenum, const GLbyte*);
extern pfn_glNormalStream3bvATI QGLglNormalStream3bvATI;
#define glNormalStream3bvATI QGLglNormalStream3bvATI

typedef void (APIENTRY* pfn_glNormalStream3sATI) (GLenum, GLshort, GLshort, GLshort);
extern pfn_glNormalStream3sATI QGLglNormalStream3sATI;
#define glNormalStream3sATI QGLglNormalStream3sATI

typedef void (APIENTRY* pfn_glNormalStream3svATI) (GLenum, const GLshort*);
extern pfn_glNormalStream3svATI QGLglNormalStream3svATI;
#define glNormalStream3svATI QGLglNormalStream3svATI

typedef void (APIENTRY* pfn_glNormalStream3iATI) (GLenum, GLint, GLint, GLint);
extern pfn_glNormalStream3iATI QGLglNormalStream3iATI;
#define glNormalStream3iATI QGLglNormalStream3iATI

typedef void (APIENTRY* pfn_glNormalStream3ivATI) (GLenum, const GLint*);
extern pfn_glNormalStream3ivATI QGLglNormalStream3ivATI;
#define glNormalStream3ivATI QGLglNormalStream3ivATI

typedef void (APIENTRY* pfn_glNormalStream3fATI) (GLenum, GLfloat, GLfloat, GLfloat);
extern pfn_glNormalStream3fATI QGLglNormalStream3fATI;
#define glNormalStream3fATI QGLglNormalStream3fATI

typedef void (APIENTRY* pfn_glNormalStream3fvATI) (GLenum, const GLfloat*);
extern pfn_glNormalStream3fvATI QGLglNormalStream3fvATI;
#define glNormalStream3fvATI QGLglNormalStream3fvATI

typedef void (APIENTRY* pfn_glNormalStream3dATI) (GLenum, GLdouble, GLdouble, GLdouble);
extern pfn_glNormalStream3dATI QGLglNormalStream3dATI;
#define glNormalStream3dATI QGLglNormalStream3dATI

typedef void (APIENTRY* pfn_glNormalStream3dvATI) (GLenum, const GLdouble*);
extern pfn_glNormalStream3dvATI QGLglNormalStream3dvATI;
#define glNormalStream3dvATI QGLglNormalStream3dvATI

typedef void (APIENTRY* pfn_glClientActiveVertexStreamATI) (GLenum);
extern pfn_glClientActiveVertexStreamATI QGLglClientActiveVertexStreamATI;
#define glClientActiveVertexStreamATI QGLglClientActiveVertexStreamATI

typedef void (APIENTRY* pfn_glVertexBlendEnviATI) (GLenum, GLint);
extern pfn_glVertexBlendEnviATI QGLglVertexBlendEnviATI;
#define glVertexBlendEnviATI QGLglVertexBlendEnviATI

typedef void (APIENTRY* pfn_glVertexBlendEnvfATI) (GLenum, GLfloat);
extern pfn_glVertexBlendEnvfATI QGLglVertexBlendEnvfATI;
#define glVertexBlendEnvfATI QGLglVertexBlendEnvfATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_element_array;
extern int QGLNumFunctions_GL_ATI_element_array;
#ifndef GL_ATI_element_array
#define GL_ATI_element_array
#define QGL_DEFINED_GL_ATI_element_array
#define GL_ELEMENT_ARRAY_ATI              0x8768
#define GL_ELEMENT_ARRAY_TYPE_ATI         0x8769
#define GL_ELEMENT_ARRAY_POINTER_ATI      0x876A

typedef void (APIENTRY* pfn_glElementPointerATI) (GLenum, const GLvoid*);
extern pfn_glElementPointerATI QGLglElementPointerATI;
#define glElementPointerATI QGLglElementPointerATI

typedef void (APIENTRY* pfn_glDrawElementArrayATI) (GLenum, GLsizei);
extern pfn_glDrawElementArrayATI QGLglDrawElementArrayATI;
#define glDrawElementArrayATI QGLglDrawElementArrayATI

typedef void (APIENTRY* pfn_glDrawRangeElementArrayATI) (GLenum, GLuint, GLuint, GLsizei);
extern pfn_glDrawRangeElementArrayATI QGLglDrawRangeElementArrayATI;
#define glDrawRangeElementArrayATI QGLglDrawRangeElementArrayATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_mesh_array;
extern int QGLNumFunctions_GL_SUN_mesh_array;
#ifndef GL_SUN_mesh_array
#define GL_SUN_mesh_array
#define QGL_DEFINED_GL_SUN_mesh_array
#define GL_QUAD_MESH_SUN                  0x8614
#define GL_TRIANGLE_MESH_SUN              0x8615

typedef void (APIENTRY* pfn_glDrawMeshArraysSUN) (GLenum, GLint, GLsizei, GLsizei);
extern pfn_glDrawMeshArraysSUN QGLglDrawMeshArraysSUN;
#define glDrawMeshArraysSUN QGLglDrawMeshArraysSUN
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_SUN_slice_accum;
extern int QGLNumFunctions_GL_SUN_slice_accum;
#ifndef GL_SUN_slice_accum
#define GL_SUN_slice_accum
#define QGL_DEFINED_GL_SUN_slice_accum
#define GL_SLICE_ACCUM_SUN                0x85CC
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_multisample_filter_hint;
extern int QGLNumFunctions_GL_NV_multisample_filter_hint;
#ifndef GL_NV_multisample_filter_hint
#define GL_NV_multisample_filter_hint
#define QGL_DEFINED_GL_NV_multisample_filter_hint
#define GL_MULTISAMPLE_FILTER_HINT_NV     0x8534
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_depth_clamp;
extern int QGLNumFunctions_GL_NV_depth_clamp;
#ifndef GL_NV_depth_clamp
#define GL_NV_depth_clamp
#define QGL_DEFINED_GL_NV_depth_clamp
#define GL_DEPTH_CLAMP_NV                 0x864F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_occlusion_query;
extern int QGLNumFunctions_GL_NV_occlusion_query;
#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query
#define QGL_DEFINED_GL_NV_occlusion_query
#define GL_PIXEL_COUNTER_BITS_NV          0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV  0x8865
#define GL_PIXEL_COUNT_NV                 0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV       0x8867

typedef void (APIENTRY* pfn_glGenOcclusionQueriesNV) (GLsizei, GLuint*);
extern pfn_glGenOcclusionQueriesNV QGLglGenOcclusionQueriesNV;
#define glGenOcclusionQueriesNV QGLglGenOcclusionQueriesNV

typedef void (APIENTRY* pfn_glDeleteOcclusionQueriesNV) (GLsizei, const GLuint*);
extern pfn_glDeleteOcclusionQueriesNV QGLglDeleteOcclusionQueriesNV;
#define glDeleteOcclusionQueriesNV QGLglDeleteOcclusionQueriesNV

typedef GLboolean (APIENTRY* pfn_glIsOcclusionQueryNV) (GLuint);
extern pfn_glIsOcclusionQueryNV QGLglIsOcclusionQueryNV;
#define glIsOcclusionQueryNV QGLglIsOcclusionQueryNV

typedef void (APIENTRY* pfn_glBeginOcclusionQueryNV) (GLuint);
extern pfn_glBeginOcclusionQueryNV QGLglBeginOcclusionQueryNV;
#define glBeginOcclusionQueryNV QGLglBeginOcclusionQueryNV

typedef void (APIENTRY* pfn_glEndOcclusionQueryNV) (void);
extern pfn_glEndOcclusionQueryNV QGLglEndOcclusionQueryNV;
#define glEndOcclusionQueryNV QGLglEndOcclusionQueryNV

typedef void (APIENTRY* pfn_glGetOcclusionQueryivNV) (GLuint, GLenum, GLint*);
extern pfn_glGetOcclusionQueryivNV QGLglGetOcclusionQueryivNV;
#define glGetOcclusionQueryivNV QGLglGetOcclusionQueryivNV

typedef void (APIENTRY* pfn_glGetOcclusionQueryuivNV) (GLuint, GLenum, GLuint*);
extern pfn_glGetOcclusionQueryuivNV QGLglGetOcclusionQueryuivNV;
#define glGetOcclusionQueryuivNV QGLglGetOcclusionQueryuivNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_point_sprite;
extern int QGLNumFunctions_GL_NV_point_sprite;
#ifndef GL_NV_point_sprite
#define GL_NV_point_sprite
#define QGL_DEFINED_GL_NV_point_sprite
#define GL_POINT_SPRITE_NV                0x8861
#define GL_COORD_REPLACE_NV               0x8862
#define GL_POINT_SPRITE_R_MODE_NV         0x8863

typedef void (APIENTRY* pfn_glPointParameteriNV) (GLenum, GLint);
extern pfn_glPointParameteriNV QGLglPointParameteriNV;
#define glPointParameteriNV QGLglPointParameteriNV

typedef void (APIENTRY* pfn_glPointParameterivNV) (GLenum, const GLint*);
extern pfn_glPointParameterivNV QGLglPointParameterivNV;
#define glPointParameterivNV QGLglPointParameterivNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_shader3;
extern int QGLNumFunctions_GL_NV_texture_shader3;
#ifndef GL_NV_texture_shader3
#define GL_NV_texture_shader3
#define QGL_DEFINED_GL_NV_texture_shader3
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV              0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV        0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV       0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV                    0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV             0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV         0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV  0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV                 0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV                  0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV            0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV                  0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV                    0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV          0x885D
#define GL_HILO8_NV                                     0x885E
#define GL_SIGNED_HILO8_NV                              0x885F
#define GL_FORCE_BLUE_TO_ONE_NV                         0x8860
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_program1_1;
extern int QGLNumFunctions_GL_NV_vertex_program1_1;
#ifndef GL_NV_vertex_program1_1
#define GL_NV_vertex_program1_1
#define QGL_DEFINED_GL_NV_vertex_program1_1
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_shadow_funcs;
extern int QGLNumFunctions_GL_EXT_shadow_funcs;
#ifndef GL_EXT_shadow_funcs
#define GL_EXT_shadow_funcs
#define QGL_DEFINED_GL_EXT_shadow_funcs
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_stencil_two_side;
extern int QGLNumFunctions_GL_EXT_stencil_two_side;
#ifndef GL_EXT_stencil_two_side
#define GL_EXT_stencil_two_side
#define QGL_DEFINED_GL_EXT_stencil_two_side
#define GL_STENCIL_TEST_TWO_SIDE_EXT      0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT        0x8911

typedef void (APIENTRY* pfn_glActiveStencilFaceEXT) (GLenum);
extern pfn_glActiveStencilFaceEXT QGLglActiveStencilFaceEXT;
#define glActiveStencilFaceEXT QGLglActiveStencilFaceEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_text_fragment_shader;
extern int QGLNumFunctions_GL_ATI_text_fragment_shader;
#ifndef GL_ATI_text_fragment_shader
#define GL_ATI_text_fragment_shader
#define QGL_DEFINED_GL_ATI_text_fragment_shader
#define GL_TEXT_FRAGMENT_SHADER_ATI       0x8200
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_client_storage;
extern int QGLNumFunctions_GL_APPLE_client_storage;
#ifndef GL_APPLE_client_storage
#define GL_APPLE_client_storage
#define QGL_DEFINED_GL_APPLE_client_storage
#define GL_UNPACK_CLIENT_STORAGE_APPLE    0x85B2
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_element_array;
extern int QGLNumFunctions_GL_APPLE_element_array;
#ifndef GL_APPLE_element_array
#define GL_APPLE_element_array
#define QGL_DEFINED_GL_APPLE_element_array
#define GL_ELEMENT_ARRAY_APPLE            0x8768
#define GL_ELEMENT_ARRAY_TYPE_APPLE       0x8769
#define GL_ELEMENT_ARRAY_POINTER_APPLE    0x876A

typedef void (APIENTRY* pfn_glElementPointerAPPLE) (GLenum, const GLvoid*);
extern pfn_glElementPointerAPPLE QGLglElementPointerAPPLE;
#define glElementPointerAPPLE QGLglElementPointerAPPLE

typedef void (APIENTRY* pfn_glDrawElementArrayAPPLE) (GLenum, GLint, GLsizei);
extern pfn_glDrawElementArrayAPPLE QGLglDrawElementArrayAPPLE;
#define glDrawElementArrayAPPLE QGLglDrawElementArrayAPPLE

typedef void (APIENTRY* pfn_glDrawRangeElementArrayAPPLE) (GLenum, GLuint, GLuint, GLint, GLsizei);
extern pfn_glDrawRangeElementArrayAPPLE QGLglDrawRangeElementArrayAPPLE;
#define glDrawRangeElementArrayAPPLE QGLglDrawRangeElementArrayAPPLE

typedef void (APIENTRY* pfn_glMultiDrawElementArrayAPPLE) (GLenum, const GLint*, const GLsizei*, GLsizei);
extern pfn_glMultiDrawElementArrayAPPLE QGLglMultiDrawElementArrayAPPLE;
#define glMultiDrawElementArrayAPPLE QGLglMultiDrawElementArrayAPPLE

typedef void (APIENTRY* pfn_glMultiDrawRangeElementArrayAPPLE) (GLenum, GLuint, GLuint, const GLint*, const GLsizei*, GLsizei);
extern pfn_glMultiDrawRangeElementArrayAPPLE QGLglMultiDrawRangeElementArrayAPPLE;
#define glMultiDrawRangeElementArrayAPPLE QGLglMultiDrawRangeElementArrayAPPLE
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_fence;
extern int QGLNumFunctions_GL_APPLE_fence;
#ifndef GL_APPLE_fence
#define GL_APPLE_fence
#define QGL_DEFINED_GL_APPLE_fence
#define GL_DRAW_PIXELS_APPLE              0x8A0A
#define GL_FENCE_APPLE                    0x8A0B

typedef void (APIENTRY* pfn_glGenFencesAPPLE) (GLsizei, GLuint*);
extern pfn_glGenFencesAPPLE QGLglGenFencesAPPLE;
#define glGenFencesAPPLE QGLglGenFencesAPPLE

typedef void (APIENTRY* pfn_glDeleteFencesAPPLE) (GLsizei, const GLuint*);
extern pfn_glDeleteFencesAPPLE QGLglDeleteFencesAPPLE;
#define glDeleteFencesAPPLE QGLglDeleteFencesAPPLE

typedef void (APIENTRY* pfn_glSetFenceAPPLE) (GLuint);
extern pfn_glSetFenceAPPLE QGLglSetFenceAPPLE;
#define glSetFenceAPPLE QGLglSetFenceAPPLE

typedef GLboolean (APIENTRY* pfn_glIsFenceAPPLE) (GLuint);
extern pfn_glIsFenceAPPLE QGLglIsFenceAPPLE;
#define glIsFenceAPPLE QGLglIsFenceAPPLE

typedef GLboolean (APIENTRY* pfn_glTestFenceAPPLE) (GLuint);
extern pfn_glTestFenceAPPLE QGLglTestFenceAPPLE;
#define glTestFenceAPPLE QGLglTestFenceAPPLE

typedef void (APIENTRY* pfn_glFinishFenceAPPLE) (GLuint);
extern pfn_glFinishFenceAPPLE QGLglFinishFenceAPPLE;
#define glFinishFenceAPPLE QGLglFinishFenceAPPLE

typedef GLboolean (APIENTRY* pfn_glTestObjectAPPLE) (GLenum, GLuint);
extern pfn_glTestObjectAPPLE QGLglTestObjectAPPLE;
#define glTestObjectAPPLE QGLglTestObjectAPPLE

typedef void (APIENTRY* pfn_glFinishObjectAPPLE) (GLenum, GLint);
extern pfn_glFinishObjectAPPLE QGLglFinishObjectAPPLE;
#define glFinishObjectAPPLE QGLglFinishObjectAPPLE
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_vertex_array_object;
extern int QGLNumFunctions_GL_APPLE_vertex_array_object;
#ifndef GL_APPLE_vertex_array_object
#define GL_APPLE_vertex_array_object
#define QGL_DEFINED_GL_APPLE_vertex_array_object
#define GL_VERTEX_ARRAY_BINDING_APPLE     0x85B5

typedef void (APIENTRY* pfn_glBindVertexArrayAPPLE) (GLuint);
extern pfn_glBindVertexArrayAPPLE QGLglBindVertexArrayAPPLE;
#define glBindVertexArrayAPPLE QGLglBindVertexArrayAPPLE

typedef void (APIENTRY* pfn_glDeleteVertexArraysAPPLE) (GLsizei, const GLuint*);
extern pfn_glDeleteVertexArraysAPPLE QGLglDeleteVertexArraysAPPLE;
#define glDeleteVertexArraysAPPLE QGLglDeleteVertexArraysAPPLE

typedef void (APIENTRY* pfn_glGenVertexArraysAPPLE) (GLsizei, const GLuint*);
extern pfn_glGenVertexArraysAPPLE QGLglGenVertexArraysAPPLE;
#define glGenVertexArraysAPPLE QGLglGenVertexArraysAPPLE

typedef GLboolean (APIENTRY* pfn_glIsVertexArrayAPPLE) (GLuint);
extern pfn_glIsVertexArrayAPPLE QGLglIsVertexArrayAPPLE;
#define glIsVertexArrayAPPLE QGLglIsVertexArrayAPPLE
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_vertex_array_range;
extern int QGLNumFunctions_GL_APPLE_vertex_array_range;
#ifndef GL_APPLE_vertex_array_range
#define GL_APPLE_vertex_array_range
#define QGL_DEFINED_GL_APPLE_vertex_array_range
#define GL_VERTEX_ARRAY_RANGE_APPLE         0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE  0x851E
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE  0x851F
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE 0x8521
#define GL_STORAGE_CACHED_APPLE             0x85BE
#define GL_STORAGE_SHARED_APPLE             0x85BF

typedef void (APIENTRY* pfn_glVertexArrayRangeAPPLE) (GLsizei, GLvoid*);
extern pfn_glVertexArrayRangeAPPLE QGLglVertexArrayRangeAPPLE;
#define glVertexArrayRangeAPPLE QGLglVertexArrayRangeAPPLE

typedef void (APIENTRY* pfn_glFlushVertexArrayRangeAPPLE) (GLsizei, GLvoid*);
extern pfn_glFlushVertexArrayRangeAPPLE QGLglFlushVertexArrayRangeAPPLE;
#define glFlushVertexArrayRangeAPPLE QGLglFlushVertexArrayRangeAPPLE

typedef void (APIENTRY* pfn_glVertexArrayParameteriAPPLE) (GLenum, GLint);
extern pfn_glVertexArrayParameteriAPPLE QGLglVertexArrayParameteriAPPLE;
#define glVertexArrayParameteriAPPLE QGLglVertexArrayParameteriAPPLE
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_APPLE_ycbcr_422;
extern int QGLNumFunctions_GL_APPLE_ycbcr_422;
#ifndef GL_APPLE_ycbcr_422
#define GL_APPLE_ycbcr_422
#define QGL_DEFINED_GL_APPLE_ycbcr_422
#define GL_YCBCR_422_APPLE                0x85B9
#define GL_UNSIGNED_SHORT_8_8_APPLE       0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE   0x85BB
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_S3_s3tc;
extern int QGLNumFunctions_GL_S3_s3tc;
#ifndef GL_S3_s3tc
#define GL_S3_s3tc
#define QGL_DEFINED_GL_S3_s3tc
#define GL_RGB_S3TC                       0x83A0
#define GL_RGB4_S3TC                      0x83A1
#define GL_RGBA_S3TC                      0x83A2
#define GL_RGBA4_S3TC                     0x83A3
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_draw_buffers;
extern int QGLNumFunctions_GL_ATI_draw_buffers;
#ifndef GL_ATI_draw_buffers
#define GL_ATI_draw_buffers
#define QGL_DEFINED_GL_ATI_draw_buffers
#define GL_MAX_DRAW_BUFFERS_ATI           0x8824
#define GL_DRAW_BUFFER0_ATI               0x8825
#define GL_DRAW_BUFFER1_ATI               0x8826
#define GL_DRAW_BUFFER2_ATI               0x8827
#define GL_DRAW_BUFFER3_ATI               0x8828
#define GL_DRAW_BUFFER4_ATI               0x8829
#define GL_DRAW_BUFFER5_ATI               0x882A
#define GL_DRAW_BUFFER6_ATI               0x882B
#define GL_DRAW_BUFFER7_ATI               0x882C
#define GL_DRAW_BUFFER8_ATI               0x882D
#define GL_DRAW_BUFFER9_ATI               0x882E
#define GL_DRAW_BUFFER10_ATI              0x882F
#define GL_DRAW_BUFFER11_ATI              0x8830
#define GL_DRAW_BUFFER12_ATI              0x8831
#define GL_DRAW_BUFFER13_ATI              0x8832
#define GL_DRAW_BUFFER14_ATI              0x8833
#define GL_DRAW_BUFFER15_ATI              0x8834

typedef void (APIENTRY* pfn_glDrawBuffersATI) (GLsizei, const GLenum*);
extern pfn_glDrawBuffersATI QGLglDrawBuffersATI;
#define glDrawBuffersATI QGLglDrawBuffersATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_pixel_format_float;
extern int QGLNumFunctions_GL_ATI_pixel_format_float;
#ifndef GL_ATI_pixel_format_float
#define GL_ATI_pixel_format_float
#define QGL_DEFINED_GL_ATI_pixel_format_float
#define GL_TYPE_RGBA_FLOAT_ATI            0x8820
#define GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI 0x8835
/* This is really a WGL extension, but defines some associated GL enums.
 * ATI does not export "GL_ATI_pixel_format_float" in the GL_EXTENSIONS string.
 */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_texture_env_combine3;
extern int QGLNumFunctions_GL_ATI_texture_env_combine3;
#ifndef GL_ATI_texture_env_combine3
#define GL_ATI_texture_env_combine3
#define QGL_DEFINED_GL_ATI_texture_env_combine3
#define GL_MODULATE_ADD_ATI               0x8744
#define GL_MODULATE_SIGNED_ADD_ATI        0x8745
#define GL_MODULATE_SUBTRACT_ATI          0x8746
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_texture_float;
extern int QGLNumFunctions_GL_ATI_texture_float;
#ifndef GL_ATI_texture_float
#define GL_ATI_texture_float
#define QGL_DEFINED_GL_ATI_texture_float
#define GL_RGBA_FLOAT32_ATI               0x8814
#define GL_RGB_FLOAT32_ATI                0x8815
#define GL_ALPHA_FLOAT32_ATI              0x8816
#define GL_INTENSITY_FLOAT32_ATI          0x8817
#define GL_LUMINANCE_FLOAT32_ATI          0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI    0x8819
#define GL_RGBA_FLOAT16_ATI               0x881A
#define GL_RGB_FLOAT16_ATI                0x881B
#define GL_ALPHA_FLOAT16_ATI              0x881C
#define GL_INTENSITY_FLOAT16_ATI          0x881D
#define GL_LUMINANCE_FLOAT16_ATI          0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI    0x881F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_float_buffer;
extern int QGLNumFunctions_GL_NV_float_buffer;
#ifndef GL_NV_float_buffer
#define GL_NV_float_buffer
#define QGL_DEFINED_GL_NV_float_buffer
#define GL_FLOAT_R_NV                     0x8880
#define GL_FLOAT_RG_NV                    0x8881
#define GL_FLOAT_RGB_NV                   0x8882
#define GL_FLOAT_RGBA_NV                  0x8883
#define GL_FLOAT_R16_NV                   0x8884
#define GL_FLOAT_R32_NV                   0x8885
#define GL_FLOAT_RG16_NV                  0x8886
#define GL_FLOAT_RG32_NV                  0x8887
#define GL_FLOAT_RGB16_NV                 0x8888
#define GL_FLOAT_RGB32_NV                 0x8889
#define GL_FLOAT_RGBA16_NV                0x888A
#define GL_FLOAT_RGBA32_NV                0x888B
#define GL_TEXTURE_FLOAT_COMPONENTS_NV    0x888C
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV     0x888D
#define GL_FLOAT_RGBA_MODE_NV             0x888E
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_fragment_program;
extern int QGLNumFunctions_GL_NV_fragment_program;
#ifndef GL_NV_fragment_program
#define GL_NV_fragment_program
#define QGL_DEFINED_GL_NV_fragment_program
#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#define GL_FRAGMENT_PROGRAM_NV                      0x8870
#define GL_MAX_TEXTURE_COORDS_NV                    0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV               0x8872
#define GL_FRAGMENT_PROGRAM_BINDING_NV              0x8873
#define GL_PROGRAM_ERROR_STRING_NV                  0x8874

typedef void (APIENTRY* pfn_glProgramNamedParameter4fNV) (GLuint, GLsizei, const GLubyte*, GLfloat, GLfloat, GLfloat, GLfloat);
extern pfn_glProgramNamedParameter4fNV QGLglProgramNamedParameter4fNV;
#define glProgramNamedParameter4fNV QGLglProgramNamedParameter4fNV

typedef void (APIENTRY* pfn_glProgramNamedParameter4dNV) (GLuint, GLsizei, const GLubyte*, GLdouble, GLdouble, GLdouble, GLdouble);
extern pfn_glProgramNamedParameter4dNV QGLglProgramNamedParameter4dNV;
#define glProgramNamedParameter4dNV QGLglProgramNamedParameter4dNV

typedef void (APIENTRY* pfn_glProgramNamedParameter4fvNV) (GLuint, GLsizei, const GLubyte*, const GLfloat*);
extern pfn_glProgramNamedParameter4fvNV QGLglProgramNamedParameter4fvNV;
#define glProgramNamedParameter4fvNV QGLglProgramNamedParameter4fvNV

typedef void (APIENTRY* pfn_glProgramNamedParameter4dvNV) (GLuint, GLsizei, const GLubyte*, const GLdouble*);
extern pfn_glProgramNamedParameter4dvNV QGLglProgramNamedParameter4dvNV;
#define glProgramNamedParameter4dvNV QGLglProgramNamedParameter4dvNV

typedef void (APIENTRY* pfn_glGetProgramNamedParameterfvNV) (GLuint, GLsizei, const GLubyte*, GLfloat*);
extern pfn_glGetProgramNamedParameterfvNV QGLglGetProgramNamedParameterfvNV;
#define glGetProgramNamedParameterfvNV QGLglGetProgramNamedParameterfvNV

typedef void (APIENTRY* pfn_glGetProgramNamedParameterdvNV) (GLuint, GLsizei, const GLubyte*, GLdouble*);
extern pfn_glGetProgramNamedParameterdvNV QGLglGetProgramNamedParameterdvNV;
#define glGetProgramNamedParameterdvNV QGLglGetProgramNamedParameterdvNV
/* Some NV_fragment_program entry points are shared with ARB_vertex_program. */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_half_float;
extern int QGLNumFunctions_GL_NV_half_float;
#ifndef GL_NV_half_float
#define GL_NV_half_float
#define QGL_DEFINED_GL_NV_half_float
#define GL_HALF_FLOAT_NV                  0x140B
typedef unsigned short GLhalfNV;

typedef void (APIENTRY* pfn_glVertex2hNV) (GLhalfNV, GLhalfNV);
extern pfn_glVertex2hNV QGLglVertex2hNV;
#define glVertex2hNV QGLglVertex2hNV

typedef void (APIENTRY* pfn_glVertex2hvNV) (const GLhalfNV*);
extern pfn_glVertex2hvNV QGLglVertex2hvNV;
#define glVertex2hvNV QGLglVertex2hvNV

typedef void (APIENTRY* pfn_glVertex3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glVertex3hNV QGLglVertex3hNV;
#define glVertex3hNV QGLglVertex3hNV

typedef void (APIENTRY* pfn_glVertex3hvNV) (const GLhalfNV*);
extern pfn_glVertex3hvNV QGLglVertex3hvNV;
#define glVertex3hvNV QGLglVertex3hvNV

typedef void (APIENTRY* pfn_glVertex4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glVertex4hNV QGLglVertex4hNV;
#define glVertex4hNV QGLglVertex4hNV

typedef void (APIENTRY* pfn_glVertex4hvNV) (const GLhalfNV*);
extern pfn_glVertex4hvNV QGLglVertex4hvNV;
#define glVertex4hvNV QGLglVertex4hvNV

typedef void (APIENTRY* pfn_glNormal3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glNormal3hNV QGLglNormal3hNV;
#define glNormal3hNV QGLglNormal3hNV

typedef void (APIENTRY* pfn_glNormal3hvNV) (const GLhalfNV*);
extern pfn_glNormal3hvNV QGLglNormal3hvNV;
#define glNormal3hvNV QGLglNormal3hvNV

typedef void (APIENTRY* pfn_glColor3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glColor3hNV QGLglColor3hNV;
#define glColor3hNV QGLglColor3hNV

typedef void (APIENTRY* pfn_glColor3hvNV) (const GLhalfNV*);
extern pfn_glColor3hvNV QGLglColor3hvNV;
#define glColor3hvNV QGLglColor3hvNV

typedef void (APIENTRY* pfn_glColor4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glColor4hNV QGLglColor4hNV;
#define glColor4hNV QGLglColor4hNV

typedef void (APIENTRY* pfn_glColor4hvNV) (const GLhalfNV*);
extern pfn_glColor4hvNV QGLglColor4hvNV;
#define glColor4hvNV QGLglColor4hvNV

typedef void (APIENTRY* pfn_glTexCoord1hNV) (GLhalfNV);
extern pfn_glTexCoord1hNV QGLglTexCoord1hNV;
#define glTexCoord1hNV QGLglTexCoord1hNV

typedef void (APIENTRY* pfn_glTexCoord1hvNV) (const GLhalfNV*);
extern pfn_glTexCoord1hvNV QGLglTexCoord1hvNV;
#define glTexCoord1hvNV QGLglTexCoord1hvNV

typedef void (APIENTRY* pfn_glTexCoord2hNV) (GLhalfNV, GLhalfNV);
extern pfn_glTexCoord2hNV QGLglTexCoord2hNV;
#define glTexCoord2hNV QGLglTexCoord2hNV

typedef void (APIENTRY* pfn_glTexCoord2hvNV) (const GLhalfNV*);
extern pfn_glTexCoord2hvNV QGLglTexCoord2hvNV;
#define glTexCoord2hvNV QGLglTexCoord2hvNV

typedef void (APIENTRY* pfn_glTexCoord3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glTexCoord3hNV QGLglTexCoord3hNV;
#define glTexCoord3hNV QGLglTexCoord3hNV

typedef void (APIENTRY* pfn_glTexCoord3hvNV) (const GLhalfNV*);
extern pfn_glTexCoord3hvNV QGLglTexCoord3hvNV;
#define glTexCoord3hvNV QGLglTexCoord3hvNV

typedef void (APIENTRY* pfn_glTexCoord4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glTexCoord4hNV QGLglTexCoord4hNV;
#define glTexCoord4hNV QGLglTexCoord4hNV

typedef void (APIENTRY* pfn_glTexCoord4hvNV) (const GLhalfNV*);
extern pfn_glTexCoord4hvNV QGLglTexCoord4hvNV;
#define glTexCoord4hvNV QGLglTexCoord4hvNV

typedef void (APIENTRY* pfn_glMultiTexCoord1hNV) (GLenum, GLhalfNV);
extern pfn_glMultiTexCoord1hNV QGLglMultiTexCoord1hNV;
#define glMultiTexCoord1hNV QGLglMultiTexCoord1hNV

typedef void (APIENTRY* pfn_glMultiTexCoord1hvNV) (GLenum, const GLhalfNV*);
extern pfn_glMultiTexCoord1hvNV QGLglMultiTexCoord1hvNV;
#define glMultiTexCoord1hvNV QGLglMultiTexCoord1hvNV

typedef void (APIENTRY* pfn_glMultiTexCoord2hNV) (GLenum, GLhalfNV, GLhalfNV);
extern pfn_glMultiTexCoord2hNV QGLglMultiTexCoord2hNV;
#define glMultiTexCoord2hNV QGLglMultiTexCoord2hNV

typedef void (APIENTRY* pfn_glMultiTexCoord2hvNV) (GLenum, const GLhalfNV*);
extern pfn_glMultiTexCoord2hvNV QGLglMultiTexCoord2hvNV;
#define glMultiTexCoord2hvNV QGLglMultiTexCoord2hvNV

typedef void (APIENTRY* pfn_glMultiTexCoord3hNV) (GLenum, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glMultiTexCoord3hNV QGLglMultiTexCoord3hNV;
#define glMultiTexCoord3hNV QGLglMultiTexCoord3hNV

typedef void (APIENTRY* pfn_glMultiTexCoord3hvNV) (GLenum, const GLhalfNV*);
extern pfn_glMultiTexCoord3hvNV QGLglMultiTexCoord3hvNV;
#define glMultiTexCoord3hvNV QGLglMultiTexCoord3hvNV

typedef void (APIENTRY* pfn_glMultiTexCoord4hNV) (GLenum, GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glMultiTexCoord4hNV QGLglMultiTexCoord4hNV;
#define glMultiTexCoord4hNV QGLglMultiTexCoord4hNV

typedef void (APIENTRY* pfn_glMultiTexCoord4hvNV) (GLenum, const GLhalfNV*);
extern pfn_glMultiTexCoord4hvNV QGLglMultiTexCoord4hvNV;
#define glMultiTexCoord4hvNV QGLglMultiTexCoord4hvNV

typedef void (APIENTRY* pfn_glFogCoordhNV) (GLhalfNV);
extern pfn_glFogCoordhNV QGLglFogCoordhNV;
#define glFogCoordhNV QGLglFogCoordhNV

typedef void (APIENTRY* pfn_glFogCoordhvNV) (const GLhalfNV*);
extern pfn_glFogCoordhvNV QGLglFogCoordhvNV;
#define glFogCoordhvNV QGLglFogCoordhvNV

typedef void (APIENTRY* pfn_glSecondaryColor3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glSecondaryColor3hNV QGLglSecondaryColor3hNV;
#define glSecondaryColor3hNV QGLglSecondaryColor3hNV

typedef void (APIENTRY* pfn_glSecondaryColor3hvNV) (const GLhalfNV*);
extern pfn_glSecondaryColor3hvNV QGLglSecondaryColor3hvNV;
#define glSecondaryColor3hvNV QGLglSecondaryColor3hvNV

typedef void (APIENTRY* pfn_glVertexWeighthNV) (GLhalfNV);
extern pfn_glVertexWeighthNV QGLglVertexWeighthNV;
#define glVertexWeighthNV QGLglVertexWeighthNV

typedef void (APIENTRY* pfn_glVertexWeighthvNV) (const GLhalfNV*);
extern pfn_glVertexWeighthvNV QGLglVertexWeighthvNV;
#define glVertexWeighthvNV QGLglVertexWeighthvNV

typedef void (APIENTRY* pfn_glVertexAttrib1hNV) (GLuint, GLhalfNV);
extern pfn_glVertexAttrib1hNV QGLglVertexAttrib1hNV;
#define glVertexAttrib1hNV QGLglVertexAttrib1hNV

typedef void (APIENTRY* pfn_glVertexAttrib1hvNV) (GLuint, const GLhalfNV*);
extern pfn_glVertexAttrib1hvNV QGLglVertexAttrib1hvNV;
#define glVertexAttrib1hvNV QGLglVertexAttrib1hvNV

typedef void (APIENTRY* pfn_glVertexAttrib2hNV) (GLuint, GLhalfNV, GLhalfNV);
extern pfn_glVertexAttrib2hNV QGLglVertexAttrib2hNV;
#define glVertexAttrib2hNV QGLglVertexAttrib2hNV

typedef void (APIENTRY* pfn_glVertexAttrib2hvNV) (GLuint, const GLhalfNV*);
extern pfn_glVertexAttrib2hvNV QGLglVertexAttrib2hvNV;
#define glVertexAttrib2hvNV QGLglVertexAttrib2hvNV

typedef void (APIENTRY* pfn_glVertexAttrib3hNV) (GLuint, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glVertexAttrib3hNV QGLglVertexAttrib3hNV;
#define glVertexAttrib3hNV QGLglVertexAttrib3hNV

typedef void (APIENTRY* pfn_glVertexAttrib3hvNV) (GLuint, const GLhalfNV*);
extern pfn_glVertexAttrib3hvNV QGLglVertexAttrib3hvNV;
#define glVertexAttrib3hvNV QGLglVertexAttrib3hvNV

typedef void (APIENTRY* pfn_glVertexAttrib4hNV) (GLuint, GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
extern pfn_glVertexAttrib4hNV QGLglVertexAttrib4hNV;
#define glVertexAttrib4hNV QGLglVertexAttrib4hNV

typedef void (APIENTRY* pfn_glVertexAttrib4hvNV) (GLuint, const GLhalfNV*);
extern pfn_glVertexAttrib4hvNV QGLglVertexAttrib4hvNV;
#define glVertexAttrib4hvNV QGLglVertexAttrib4hvNV

typedef void (APIENTRY* pfn_glVertexAttribs1hvNV) (GLuint, GLsizei, const GLhalfNV*);
extern pfn_glVertexAttribs1hvNV QGLglVertexAttribs1hvNV;
#define glVertexAttribs1hvNV QGLglVertexAttribs1hvNV

typedef void (APIENTRY* pfn_glVertexAttribs2hvNV) (GLuint, GLsizei, const GLhalfNV*);
extern pfn_glVertexAttribs2hvNV QGLglVertexAttribs2hvNV;
#define glVertexAttribs2hvNV QGLglVertexAttribs2hvNV

typedef void (APIENTRY* pfn_glVertexAttribs3hvNV) (GLuint, GLsizei, const GLhalfNV*);
extern pfn_glVertexAttribs3hvNV QGLglVertexAttribs3hvNV;
#define glVertexAttribs3hvNV QGLglVertexAttribs3hvNV

typedef void (APIENTRY* pfn_glVertexAttribs4hvNV) (GLuint, GLsizei, const GLhalfNV*);
extern pfn_glVertexAttribs4hvNV QGLglVertexAttribs4hvNV;
#define glVertexAttribs4hvNV QGLglVertexAttribs4hvNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_pixel_data_range;
extern int QGLNumFunctions_GL_NV_pixel_data_range;
#ifndef GL_NV_pixel_data_range
#define GL_NV_pixel_data_range
#define QGL_DEFINED_GL_NV_pixel_data_range
#define GL_WRITE_PIXEL_DATA_RANGE_NV            0x8878
#define GL_READ_PIXEL_DATA_RANGE_NV             0x8879
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV     0x887A
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV      0x887B
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV    0x887C
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV     0x887D

typedef void (APIENTRY* pfn_glPixelDataRangeNV) (GLenum, GLsizei, GLvoid*);
extern pfn_glPixelDataRangeNV QGLglPixelDataRangeNV;
#define glPixelDataRangeNV QGLglPixelDataRangeNV

typedef void (APIENTRY* pfn_glFlushPixelDataRangeNV) (GLenum);
extern pfn_glFlushPixelDataRangeNV QGLglFlushPixelDataRangeNV;
#define glFlushPixelDataRangeNV QGLglFlushPixelDataRangeNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_primitive_restart;
extern int QGLNumFunctions_GL_NV_primitive_restart;
#ifndef GL_NV_primitive_restart
#define GL_NV_primitive_restart
#define QGL_DEFINED_GL_NV_primitive_restart
#define GL_PRIMITIVE_RESTART_NV           0x8558
#define GL_PRIMITIVE_RESTART_INDEX_NV     0x8559

typedef void (APIENTRY* pfn_glPrimitiveRestartNV) (void);
extern pfn_glPrimitiveRestartNV QGLglPrimitiveRestartNV;
#define glPrimitiveRestartNV QGLglPrimitiveRestartNV

typedef void (APIENTRY* pfn_glPrimitiveRestartIndexNV) (GLuint);
extern pfn_glPrimitiveRestartIndexNV QGLglPrimitiveRestartIndexNV;
#define glPrimitiveRestartIndexNV QGLglPrimitiveRestartIndexNV
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_texture_expand_normal;
extern int QGLNumFunctions_GL_NV_texture_expand_normal;
#ifndef GL_NV_texture_expand_normal
#define GL_NV_texture_expand_normal
#define QGL_DEFINED_GL_NV_texture_expand_normal
#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV 0x888F
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_program2;
extern int QGLNumFunctions_GL_NV_vertex_program2;
#ifndef GL_NV_vertex_program2
#define GL_NV_vertex_program2
#define QGL_DEFINED_GL_NV_vertex_program2
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_map_object_buffer;
extern int QGLNumFunctions_GL_ATI_map_object_buffer;
#ifndef GL_ATI_map_object_buffer
#define GL_ATI_map_object_buffer
#define QGL_DEFINED_GL_ATI_map_object_buffer

typedef GLvoid* (APIENTRY* pfn_glMapObjectBufferATI) (GLuint);
extern pfn_glMapObjectBufferATI QGLglMapObjectBufferATI;
#define glMapObjectBufferATI QGLglMapObjectBufferATI

typedef void (APIENTRY* pfn_glUnmapObjectBufferATI) (GLuint);
extern pfn_glUnmapObjectBufferATI QGLglUnmapObjectBufferATI;
#define glUnmapObjectBufferATI QGLglUnmapObjectBufferATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_separate_stencil;
extern int QGLNumFunctions_GL_ATI_separate_stencil;
#ifndef GL_ATI_separate_stencil
#define GL_ATI_separate_stencil
#define QGL_DEFINED_GL_ATI_separate_stencil
#define GL_STENCIL_BACK_FUNC_ATI            0x8800
#define GL_STENCIL_BACK_FAIL_ATI            0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI 0x8803

typedef void (APIENTRY* pfn_glStencilOpSeparateATI) (GLenum, GLenum, GLenum, GLenum);
extern pfn_glStencilOpSeparateATI QGLglStencilOpSeparateATI;
#define glStencilOpSeparateATI QGLglStencilOpSeparateATI

typedef void (APIENTRY* pfn_glStencilFuncSeparateATI) (GLenum, GLenum, GLint, GLuint);
extern pfn_glStencilFuncSeparateATI QGLglStencilFuncSeparateATI;
#define glStencilFuncSeparateATI QGLglStencilFuncSeparateATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_ATI_vertex_attrib_array_object;
extern int QGLNumFunctions_GL_ATI_vertex_attrib_array_object;
#ifndef GL_ATI_vertex_attrib_array_object
#define GL_ATI_vertex_attrib_array_object
#define QGL_DEFINED_GL_ATI_vertex_attrib_array_object

typedef void (APIENTRY* pfn_glVertexAttribArrayObjectATI) (GLuint, GLint, GLenum, GLboolean, GLsizei, GLuint, GLuint);
extern pfn_glVertexAttribArrayObjectATI QGLglVertexAttribArrayObjectATI;
#define glVertexAttribArrayObjectATI QGLglVertexAttribArrayObjectATI

typedef void (APIENTRY* pfn_glGetVertexAttribArrayObjectfvATI) (GLuint, GLenum, GLfloat*);
extern pfn_glGetVertexAttribArrayObjectfvATI QGLglGetVertexAttribArrayObjectfvATI;
#define glGetVertexAttribArrayObjectfvATI QGLglGetVertexAttribArrayObjectfvATI

typedef void (APIENTRY* pfn_glGetVertexAttribArrayObjectivATI) (GLuint, GLenum, GLint*);
extern pfn_glGetVertexAttribArrayObjectivATI QGLglGetVertexAttribArrayObjectivATI;
#define glGetVertexAttribArrayObjectivATI QGLglGetVertexAttribArrayObjectivATI
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_OES_read_format;
extern int QGLNumFunctions_GL_OES_read_format;
#ifndef GL_OES_read_format
#define GL_OES_read_format
#define QGL_DEFINED_GL_OES_read_format
#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_depth_bounds_test;
extern int QGLNumFunctions_GL_EXT_depth_bounds_test;
#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test
#define QGL_DEFINED_GL_EXT_depth_bounds_test
#define GL_DEPTH_BOUNDS_TEST_EXT          0x8890
#define GL_DEPTH_BOUNDS_EXT               0x8891

typedef void (APIENTRY* pfn_glDepthBoundsEXT) (GLclampd, GLclampd);
extern pfn_glDepthBoundsEXT QGLglDepthBoundsEXT;
#define glDepthBoundsEXT QGLglDepthBoundsEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_texture_mirror_clamp;
extern int QGLNumFunctions_GL_EXT_texture_mirror_clamp;
#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp
#define QGL_DEFINED_GL_EXT_texture_mirror_clamp
#define GL_MIRROR_CLAMP_EXT               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT       0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT     0x8912
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_blend_equation_separate;
extern int QGLNumFunctions_GL_EXT_blend_equation_separate;
#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate
#define QGL_DEFINED_GL_EXT_blend_equation_separate
#define GL_BLEND_EQUATION_RGB_EXT         GL_BLEND_EQUATION
#define GL_BLEND_EQUATION_ALPHA_EXT       0x883D

typedef void (APIENTRY* pfn_glBlendEquationSeparateEXT) (GLenum, GLenum);
extern pfn_glBlendEquationSeparateEXT QGLglBlendEquationSeparateEXT;
#define glBlendEquationSeparateEXT QGLglBlendEquationSeparateEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_MESA_pack_invert;
extern int QGLNumFunctions_GL_MESA_pack_invert;
#ifndef GL_MESA_pack_invert
#define GL_MESA_pack_invert
#define QGL_DEFINED_GL_MESA_pack_invert
#define GL_PACK_INVERT_MESA               0x8758
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_MESA_ycbcr_texture;
extern int QGLNumFunctions_GL_MESA_ycbcr_texture;
#ifndef GL_MESA_ycbcr_texture
#define GL_MESA_ycbcr_texture
#define QGL_DEFINED_GL_MESA_ycbcr_texture
#define GL_UNSIGNED_SHORT_8_8_MESA        0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_MESA    0x85BB
#define GL_YCBCR_MESA                     0x8757
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_pixel_buffer_object;
extern int QGLNumFunctions_GL_EXT_pixel_buffer_object;
#ifndef GL_EXT_pixel_buffer_object
#define GL_EXT_pixel_buffer_object
#define QGL_DEFINED_GL_EXT_pixel_buffer_object
#define GL_PIXEL_PACK_BUFFER_EXT          0x88EB
#define GL_PIXEL_UNPACK_BUFFER_EXT        0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT  0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_fragment_program_option;
extern int QGLNumFunctions_GL_NV_fragment_program_option;
#ifndef GL_NV_fragment_program_option
#define GL_NV_fragment_program_option
#define QGL_DEFINED_GL_NV_fragment_program_option
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_fragment_program2;
extern int QGLNumFunctions_GL_NV_fragment_program2;
#ifndef GL_NV_fragment_program2
#define GL_NV_fragment_program2
#define QGL_DEFINED_GL_NV_fragment_program2
#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV        0x88F5
#define GL_MAX_PROGRAM_IF_DEPTH_NV          0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV        0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV        0x88F8
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_program2_option;
extern int QGLNumFunctions_GL_NV_vertex_program2_option;
#ifndef GL_NV_vertex_program2_option
#define GL_NV_vertex_program2_option
#define QGL_DEFINED_GL_NV_vertex_program2_option
/* reuse GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV */
/* reuse GL_MAX_PROGRAM_CALL_DEPTH_NV */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_NV_vertex_program3;
extern int QGLNumFunctions_GL_NV_vertex_program3;
#ifndef GL_NV_vertex_program3
#define GL_NV_vertex_program3
#define QGL_DEFINED_GL_NV_vertex_program3
/* reuse GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB */
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_EXT_framebuffer_object;
extern int QGLNumFunctions_GL_EXT_framebuffer_object;
#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object
#define QGL_DEFINED_GL_EXT_framebuffer_object
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT                        0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT                          0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT                         0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT           0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT           0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT         0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT    0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT                         0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENTS_EXT           0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT    0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT  0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT            0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT               0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT           0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT           0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT                      0x8CDD
#define GL_FRAMEBUFFER_STATUS_ERROR_EXT                     0x8CDE
#define GL_MAX_COLOR_ATTACHMENTS_EXT                        0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT                            0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT                            0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT                            0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT                            0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT                            0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT                            0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT                            0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT                            0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT                            0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT                            0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT                           0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT                           0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT                           0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT                           0x8CED
#define GL_COLOR_ATTACHMENT14_EXT                           0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT                           0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT                             0x8D00
#define GL_STENCIL_ATTACHMENT_EXT                           0x8D20
#define GL_FRAMEBUFFER_EXT                                  0x8D40
#define GL_RENDERBUFFER_EXT                                 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT                           0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT                          0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT                 0x8D44
#define GL_STENCIL_INDEX_EXT                                0x8D45
#define GL_STENCIL_INDEX1_EXT                               0x8D46
#define GL_STENCIL_INDEX4_EXT                               0x8D47
#define GL_STENCIL_INDEX8_EXT                               0x8D48
#define GL_STENCIL_INDEX16_EXT                              0x8D49

typedef GLboolean (APIENTRY* pfn_glIsRenderbufferEXT) (GLuint);
extern pfn_glIsRenderbufferEXT QGLglIsRenderbufferEXT;
#define glIsRenderbufferEXT QGLglIsRenderbufferEXT

typedef void (APIENTRY* pfn_glBindRenderbufferEXT) (GLenum, GLuint);
extern pfn_glBindRenderbufferEXT QGLglBindRenderbufferEXT;
#define glBindRenderbufferEXT QGLglBindRenderbufferEXT

typedef void (APIENTRY* pfn_glDeleteRenderbuffersEXT) (GLsizei, const GLuint*);
extern pfn_glDeleteRenderbuffersEXT QGLglDeleteRenderbuffersEXT;
#define glDeleteRenderbuffersEXT QGLglDeleteRenderbuffersEXT

typedef void (APIENTRY* pfn_glGenRenderbuffersEXT) (GLsizei, GLuint*);
extern pfn_glGenRenderbuffersEXT QGLglGenRenderbuffersEXT;
#define glGenRenderbuffersEXT QGLglGenRenderbuffersEXT

typedef void (APIENTRY* pfn_glRenderbufferStorageEXT) (GLenum, GLenum, GLsizei, GLsizei);
extern pfn_glRenderbufferStorageEXT QGLglRenderbufferStorageEXT;
#define glRenderbufferStorageEXT QGLglRenderbufferStorageEXT

typedef void (APIENTRY* pfn_glGetRenderbufferParameterivEXT) (GLenum, GLenum, GLint*);
extern pfn_glGetRenderbufferParameterivEXT QGLglGetRenderbufferParameterivEXT;
#define glGetRenderbufferParameterivEXT QGLglGetRenderbufferParameterivEXT

typedef GLboolean (APIENTRY* pfn_glIsFramebufferEXT) (GLuint);
extern pfn_glIsFramebufferEXT QGLglIsFramebufferEXT;
#define glIsFramebufferEXT QGLglIsFramebufferEXT

typedef void (APIENTRY* pfn_glBindFramebufferEXT) (GLenum, GLuint);
extern pfn_glBindFramebufferEXT QGLglBindFramebufferEXT;
#define glBindFramebufferEXT QGLglBindFramebufferEXT

typedef void (APIENTRY* pfn_glDeleteFramebuffersEXT) (GLsizei, const GLuint*);
extern pfn_glDeleteFramebuffersEXT QGLglDeleteFramebuffersEXT;
#define glDeleteFramebuffersEXT QGLglDeleteFramebuffersEXT

typedef void (APIENTRY* pfn_glGenFramebuffersEXT) (GLsizei, GLuint*);
extern pfn_glGenFramebuffersEXT QGLglGenFramebuffersEXT;
#define glGenFramebuffersEXT QGLglGenFramebuffersEXT

typedef GLenum (APIENTRY* pfn_glCheckFramebufferStatusEXT) (GLenum);
extern pfn_glCheckFramebufferStatusEXT QGLglCheckFramebufferStatusEXT;
#define glCheckFramebufferStatusEXT QGLglCheckFramebufferStatusEXT

typedef void (APIENTRY* pfn_glFramebufferTexture1DEXT) (GLenum, GLenum, GLenum, GLuint, GLint);
extern pfn_glFramebufferTexture1DEXT QGLglFramebufferTexture1DEXT;
#define glFramebufferTexture1DEXT QGLglFramebufferTexture1DEXT

typedef void (APIENTRY* pfn_glFramebufferTexture2DEXT) (GLenum, GLenum, GLenum, GLuint, GLint);
extern pfn_glFramebufferTexture2DEXT QGLglFramebufferTexture2DEXT;
#define glFramebufferTexture2DEXT QGLglFramebufferTexture2DEXT

typedef void (APIENTRY* pfn_glFramebufferTexture3DEXT) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
extern pfn_glFramebufferTexture3DEXT QGLglFramebufferTexture3DEXT;
#define glFramebufferTexture3DEXT QGLglFramebufferTexture3DEXT

typedef void (APIENTRY* pfn_glFramebufferRenderbufferEXT) (GLenum, GLenum, GLenum, GLuint);
extern pfn_glFramebufferRenderbufferEXT QGLglFramebufferRenderbufferEXT;
#define glFramebufferRenderbufferEXT QGLglFramebufferRenderbufferEXT

typedef void (APIENTRY* pfn_glGetFramebufferAttachmentParameterivEXT) (GLenum, GLenum, GLenum, GLint*);
extern pfn_glGetFramebufferAttachmentParameterivEXT QGLglGetFramebufferAttachmentParameterivEXT;
#define glGetFramebufferAttachmentParameterivEXT QGLglGetFramebufferAttachmentParameterivEXT

typedef void (APIENTRY* pfn_glGenerateMipmapEXT) (GLenum);
extern pfn_glGenerateMipmapEXT QGLglGenerateMipmapEXT;
#define glGenerateMipmapEXT QGLglGenerateMipmapEXT
#endif

//------------------------------------------------------------------------------------------------------------------
extern int QGLExists_GL_GREMEDY_string_marker;
extern int QGLNumFunctions_GL_GREMEDY_string_marker;
#ifndef GL_GREMEDY_string_marker
#define GL_GREMEDY_string_marker
#define QGL_DEFINED_GL_GREMEDY_string_marker

typedef void (APIENTRY* pfn_glStringMarkerGREMEDY) (GLsizei, const GLvoid*);
extern pfn_glStringMarkerGREMEDY QGLglStringMarkerGREMEDY;
#define glStringMarkerGREMEDY QGLglStringMarkerGREMEDY
#endif

//------------------------------------------------------------------------------------------------------------------

#include "QGlu.h"

//------------------------------------------------------------------------------------------------------------------
void* GetFunctionPointer (const char*);
void InitializeOpenGLExtensions ();
//------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#ifdef QGL_DEFINED_APIENTRY
#undef QGL_DEFINED_APIENTRY
#undef APIENTRY
#endif

#ifdef QGL_DEFINED_CALLBACK
#undef QGL_DEFINED_CALLBACK
#undef CALLBACK
#endif

#ifdef QGL_DEFINED_WINGDIAPI
#undef QGL_DEFINED_WINGDIAPI
#undef WINGDIAPI
#endif

#endif



