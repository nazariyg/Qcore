#include "QOpenGLExtensions.h"


//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_1_1;
int QGLNumFunctions_GL_VERSION_1_1;
#ifdef QGL_DEFINED_GL_VERSION_1_1
static void Init_GL_VERSION_1_1 ()
{
    QGLNumFunctions_GL_VERSION_1_1 = 336;
    QGLExists_GL_VERSION_1_1 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_VERSION_1_1 ()
{
    QGLNumFunctions_GL_VERSION_1_1 = 0;
    QGLExists_GL_VERSION_1_1 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_1_2;
int QGLNumFunctions_GL_VERSION_1_2;
#ifdef QGL_DEFINED_GL_VERSION_1_2
pfn_glBlendColor QGLglBlendColor = 0;
pfn_glBlendEquation QGLglBlendEquation = 0;
pfn_glDrawRangeElements QGLglDrawRangeElements = 0;
pfn_glColorTable QGLglColorTable = 0;
pfn_glColorTableParameterfv QGLglColorTableParameterfv = 0;
pfn_glColorTableParameteriv QGLglColorTableParameteriv = 0;
pfn_glCopyColorTable QGLglCopyColorTable = 0;
pfn_glGetColorTable QGLglGetColorTable = 0;
pfn_glGetColorTableParameterfv QGLglGetColorTableParameterfv = 0;
pfn_glGetColorTableParameteriv QGLglGetColorTableParameteriv = 0;
pfn_glColorSubTable QGLglColorSubTable = 0;
pfn_glCopyColorSubTable QGLglCopyColorSubTable = 0;
pfn_glConvolutionFilter1D QGLglConvolutionFilter1D = 0;
pfn_glConvolutionFilter2D QGLglConvolutionFilter2D = 0;
pfn_glConvolutionParameterf QGLglConvolutionParameterf = 0;
pfn_glConvolutionParameterfv QGLglConvolutionParameterfv = 0;
pfn_glConvolutionParameteri QGLglConvolutionParameteri = 0;
pfn_glConvolutionParameteriv QGLglConvolutionParameteriv = 0;
pfn_glCopyConvolutionFilter1D QGLglCopyConvolutionFilter1D = 0;
pfn_glCopyConvolutionFilter2D QGLglCopyConvolutionFilter2D = 0;
pfn_glGetConvolutionFilter QGLglGetConvolutionFilter = 0;
pfn_glGetConvolutionParameterfv QGLglGetConvolutionParameterfv = 0;
pfn_glGetConvolutionParameteriv QGLglGetConvolutionParameteriv = 0;
pfn_glGetSeparableFilter QGLglGetSeparableFilter = 0;
pfn_glSeparableFilter2D QGLglSeparableFilter2D = 0;
pfn_glGetHistogram QGLglGetHistogram = 0;
pfn_glGetHistogramParameterfv QGLglGetHistogramParameterfv = 0;
pfn_glGetHistogramParameteriv QGLglGetHistogramParameteriv = 0;
pfn_glGetMinmax QGLglGetMinmax = 0;
pfn_glGetMinmaxParameterfv QGLglGetMinmaxParameterfv = 0;
pfn_glGetMinmaxParameteriv QGLglGetMinmaxParameteriv = 0;
pfn_glHistogram QGLglHistogram = 0;
pfn_glMinmax QGLglMinmax = 0;
pfn_glResetHistogram QGLglResetHistogram = 0;
pfn_glResetMinmax QGLglResetMinmax = 0;
pfn_glTexImage3D QGLglTexImage3D = 0;
pfn_glTexSubImage3D QGLglTexSubImage3D = 0;
pfn_glCopyTexSubImage3D QGLglCopyTexSubImage3D = 0;

static void Init_GL_VERSION_1_2 ()
{
    QGLNumFunctions_GL_VERSION_1_2 = 0;

    QGLglBlendColor = (pfn_glBlendColor)GetFunctionPointer("glBlendColor");
    if ( QGLglBlendColor )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglBlendEquation = (pfn_glBlendEquation)GetFunctionPointer("glBlendEquation");
    if ( QGLglBlendEquation )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglDrawRangeElements = (pfn_glDrawRangeElements)GetFunctionPointer("glDrawRangeElements");
    if ( QGLglDrawRangeElements )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglColorTable = (pfn_glColorTable)GetFunctionPointer("glColorTable");
    if ( QGLglColorTable )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglColorTableParameterfv = (pfn_glColorTableParameterfv)GetFunctionPointer("glColorTableParameterfv");
    if ( QGLglColorTableParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglColorTableParameteriv = (pfn_glColorTableParameteriv)GetFunctionPointer("glColorTableParameteriv");
    if ( QGLglColorTableParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglCopyColorTable = (pfn_glCopyColorTable)GetFunctionPointer("glCopyColorTable");
    if ( QGLglCopyColorTable )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetColorTable = (pfn_glGetColorTable)GetFunctionPointer("glGetColorTable");
    if ( QGLglGetColorTable )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetColorTableParameterfv = (pfn_glGetColorTableParameterfv)GetFunctionPointer("glGetColorTableParameterfv");
    if ( QGLglGetColorTableParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetColorTableParameteriv = (pfn_glGetColorTableParameteriv)GetFunctionPointer("glGetColorTableParameteriv");
    if ( QGLglGetColorTableParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglColorSubTable = (pfn_glColorSubTable)GetFunctionPointer("glColorSubTable");
    if ( QGLglColorSubTable )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglCopyColorSubTable = (pfn_glCopyColorSubTable)GetFunctionPointer("glCopyColorSubTable");
    if ( QGLglCopyColorSubTable )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionFilter1D = (pfn_glConvolutionFilter1D)GetFunctionPointer("glConvolutionFilter1D");
    if ( QGLglConvolutionFilter1D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionFilter2D = (pfn_glConvolutionFilter2D)GetFunctionPointer("glConvolutionFilter2D");
    if ( QGLglConvolutionFilter2D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionParameterf = (pfn_glConvolutionParameterf)GetFunctionPointer("glConvolutionParameterf");
    if ( QGLglConvolutionParameterf )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionParameterfv = (pfn_glConvolutionParameterfv)GetFunctionPointer("glConvolutionParameterfv");
    if ( QGLglConvolutionParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionParameteri = (pfn_glConvolutionParameteri)GetFunctionPointer("glConvolutionParameteri");
    if ( QGLglConvolutionParameteri )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglConvolutionParameteriv = (pfn_glConvolutionParameteriv)GetFunctionPointer("glConvolutionParameteriv");
    if ( QGLglConvolutionParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglCopyConvolutionFilter1D = (pfn_glCopyConvolutionFilter1D)GetFunctionPointer("glCopyConvolutionFilter1D");
    if ( QGLglCopyConvolutionFilter1D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglCopyConvolutionFilter2D = (pfn_glCopyConvolutionFilter2D)GetFunctionPointer("glCopyConvolutionFilter2D");
    if ( QGLglCopyConvolutionFilter2D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetConvolutionFilter = (pfn_glGetConvolutionFilter)GetFunctionPointer("glGetConvolutionFilter");
    if ( QGLglGetConvolutionFilter )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetConvolutionParameterfv = (pfn_glGetConvolutionParameterfv)GetFunctionPointer("glGetConvolutionParameterfv");
    if ( QGLglGetConvolutionParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetConvolutionParameteriv = (pfn_glGetConvolutionParameteriv)GetFunctionPointer("glGetConvolutionParameteriv");
    if ( QGLglGetConvolutionParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetSeparableFilter = (pfn_glGetSeparableFilter)GetFunctionPointer("glGetSeparableFilter");
    if ( QGLglGetSeparableFilter )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglSeparableFilter2D = (pfn_glSeparableFilter2D)GetFunctionPointer("glSeparableFilter2D");
    if ( QGLglSeparableFilter2D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetHistogram = (pfn_glGetHistogram)GetFunctionPointer("glGetHistogram");
    if ( QGLglGetHistogram )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetHistogramParameterfv = (pfn_glGetHistogramParameterfv)GetFunctionPointer("glGetHistogramParameterfv");
    if ( QGLglGetHistogramParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetHistogramParameteriv = (pfn_glGetHistogramParameteriv)GetFunctionPointer("glGetHistogramParameteriv");
    if ( QGLglGetHistogramParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetMinmax = (pfn_glGetMinmax)GetFunctionPointer("glGetMinmax");
    if ( QGLglGetMinmax )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetMinmaxParameterfv = (pfn_glGetMinmaxParameterfv)GetFunctionPointer("glGetMinmaxParameterfv");
    if ( QGLglGetMinmaxParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglGetMinmaxParameteriv = (pfn_glGetMinmaxParameteriv)GetFunctionPointer("glGetMinmaxParameteriv");
    if ( QGLglGetMinmaxParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglHistogram = (pfn_glHistogram)GetFunctionPointer("glHistogram");
    if ( QGLglHistogram )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglMinmax = (pfn_glMinmax)GetFunctionPointer("glMinmax");
    if ( QGLglMinmax )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglResetHistogram = (pfn_glResetHistogram)GetFunctionPointer("glResetHistogram");
    if ( QGLglResetHistogram )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglResetMinmax = (pfn_glResetMinmax)GetFunctionPointer("glResetMinmax");
    if ( QGLglResetMinmax )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglTexImage3D = (pfn_glTexImage3D)GetFunctionPointer("glTexImage3D");
    if ( QGLglTexImage3D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglTexSubImage3D = (pfn_glTexSubImage3D)GetFunctionPointer("glTexSubImage3D");
    if ( QGLglTexSubImage3D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    QGLglCopyTexSubImage3D = (pfn_glCopyTexSubImage3D)GetFunctionPointer("glCopyTexSubImage3D");
    if ( QGLglCopyTexSubImage3D )
        ++QGLNumFunctions_GL_VERSION_1_2;

    if ( QGLNumFunctions_GL_VERSION_1_2 == 38 )
        QGLExists_GL_VERSION_1_2 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_VERSION_1_2 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_VERSION_1_2 ()
{
    QGLNumFunctions_GL_VERSION_1_2 = 0;
    QGLExists_GL_VERSION_1_2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_1_3;
int QGLNumFunctions_GL_VERSION_1_3;
#ifdef QGL_DEFINED_GL_VERSION_1_3
pfn_glActiveTexture QGLglActiveTexture = 0;
pfn_glClientActiveTexture QGLglClientActiveTexture = 0;
pfn_glMultiTexCoord1d QGLglMultiTexCoord1d = 0;
pfn_glMultiTexCoord1dv QGLglMultiTexCoord1dv = 0;
pfn_glMultiTexCoord1f QGLglMultiTexCoord1f = 0;
pfn_glMultiTexCoord1fv QGLglMultiTexCoord1fv = 0;
pfn_glMultiTexCoord1i QGLglMultiTexCoord1i = 0;
pfn_glMultiTexCoord1iv QGLglMultiTexCoord1iv = 0;
pfn_glMultiTexCoord1s QGLglMultiTexCoord1s = 0;
pfn_glMultiTexCoord1sv QGLglMultiTexCoord1sv = 0;
pfn_glMultiTexCoord2d QGLglMultiTexCoord2d = 0;
pfn_glMultiTexCoord2dv QGLglMultiTexCoord2dv = 0;
pfn_glMultiTexCoord2f QGLglMultiTexCoord2f = 0;
pfn_glMultiTexCoord2fv QGLglMultiTexCoord2fv = 0;
pfn_glMultiTexCoord2i QGLglMultiTexCoord2i = 0;
pfn_glMultiTexCoord2iv QGLglMultiTexCoord2iv = 0;
pfn_glMultiTexCoord2s QGLglMultiTexCoord2s = 0;
pfn_glMultiTexCoord2sv QGLglMultiTexCoord2sv = 0;
pfn_glMultiTexCoord3d QGLglMultiTexCoord3d = 0;
pfn_glMultiTexCoord3dv QGLglMultiTexCoord3dv = 0;
pfn_glMultiTexCoord3f QGLglMultiTexCoord3f = 0;
pfn_glMultiTexCoord3fv QGLglMultiTexCoord3fv = 0;
pfn_glMultiTexCoord3i QGLglMultiTexCoord3i = 0;
pfn_glMultiTexCoord3iv QGLglMultiTexCoord3iv = 0;
pfn_glMultiTexCoord3s QGLglMultiTexCoord3s = 0;
pfn_glMultiTexCoord3sv QGLglMultiTexCoord3sv = 0;
pfn_glMultiTexCoord4d QGLglMultiTexCoord4d = 0;
pfn_glMultiTexCoord4dv QGLglMultiTexCoord4dv = 0;
pfn_glMultiTexCoord4f QGLglMultiTexCoord4f = 0;
pfn_glMultiTexCoord4fv QGLglMultiTexCoord4fv = 0;
pfn_glMultiTexCoord4i QGLglMultiTexCoord4i = 0;
pfn_glMultiTexCoord4iv QGLglMultiTexCoord4iv = 0;
pfn_glMultiTexCoord4s QGLglMultiTexCoord4s = 0;
pfn_glMultiTexCoord4sv QGLglMultiTexCoord4sv = 0;
pfn_glLoadTransposeMatrixf QGLglLoadTransposeMatrixf = 0;
pfn_glLoadTransposeMatrixd QGLglLoadTransposeMatrixd = 0;
pfn_glMultTransposeMatrixf QGLglMultTransposeMatrixf = 0;
pfn_glMultTransposeMatrixd QGLglMultTransposeMatrixd = 0;
pfn_glSampleCoverage QGLglSampleCoverage = 0;
pfn_glCompressedTexImage3D QGLglCompressedTexImage3D = 0;
pfn_glCompressedTexImage2D QGLglCompressedTexImage2D = 0;
pfn_glCompressedTexImage1D QGLglCompressedTexImage1D = 0;
pfn_glCompressedTexSubImage3D QGLglCompressedTexSubImage3D = 0;
pfn_glCompressedTexSubImage2D QGLglCompressedTexSubImage2D = 0;
pfn_glCompressedTexSubImage1D QGLglCompressedTexSubImage1D = 0;
pfn_glGetCompressedTexImage QGLglGetCompressedTexImage = 0;

static void Init_GL_VERSION_1_3 ()
{
    QGLNumFunctions_GL_VERSION_1_3 = 0;

    QGLglActiveTexture = (pfn_glActiveTexture)GetFunctionPointer("glActiveTexture");
    if ( QGLglActiveTexture )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglClientActiveTexture = (pfn_glClientActiveTexture)GetFunctionPointer("glClientActiveTexture");
    if ( QGLglClientActiveTexture )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1d = (pfn_glMultiTexCoord1d)GetFunctionPointer("glMultiTexCoord1d");
    if ( QGLglMultiTexCoord1d )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1dv = (pfn_glMultiTexCoord1dv)GetFunctionPointer("glMultiTexCoord1dv");
    if ( QGLglMultiTexCoord1dv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1f = (pfn_glMultiTexCoord1f)GetFunctionPointer("glMultiTexCoord1f");
    if ( QGLglMultiTexCoord1f )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1fv = (pfn_glMultiTexCoord1fv)GetFunctionPointer("glMultiTexCoord1fv");
    if ( QGLglMultiTexCoord1fv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1i = (pfn_glMultiTexCoord1i)GetFunctionPointer("glMultiTexCoord1i");
    if ( QGLglMultiTexCoord1i )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1iv = (pfn_glMultiTexCoord1iv)GetFunctionPointer("glMultiTexCoord1iv");
    if ( QGLglMultiTexCoord1iv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1s = (pfn_glMultiTexCoord1s)GetFunctionPointer("glMultiTexCoord1s");
    if ( QGLglMultiTexCoord1s )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord1sv = (pfn_glMultiTexCoord1sv)GetFunctionPointer("glMultiTexCoord1sv");
    if ( QGLglMultiTexCoord1sv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2d = (pfn_glMultiTexCoord2d)GetFunctionPointer("glMultiTexCoord2d");
    if ( QGLglMultiTexCoord2d )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2dv = (pfn_glMultiTexCoord2dv)GetFunctionPointer("glMultiTexCoord2dv");
    if ( QGLglMultiTexCoord2dv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2f = (pfn_glMultiTexCoord2f)GetFunctionPointer("glMultiTexCoord2f");
    if ( QGLglMultiTexCoord2f )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2fv = (pfn_glMultiTexCoord2fv)GetFunctionPointer("glMultiTexCoord2fv");
    if ( QGLglMultiTexCoord2fv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2i = (pfn_glMultiTexCoord2i)GetFunctionPointer("glMultiTexCoord2i");
    if ( QGLglMultiTexCoord2i )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2iv = (pfn_glMultiTexCoord2iv)GetFunctionPointer("glMultiTexCoord2iv");
    if ( QGLglMultiTexCoord2iv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2s = (pfn_glMultiTexCoord2s)GetFunctionPointer("glMultiTexCoord2s");
    if ( QGLglMultiTexCoord2s )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord2sv = (pfn_glMultiTexCoord2sv)GetFunctionPointer("glMultiTexCoord2sv");
    if ( QGLglMultiTexCoord2sv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3d = (pfn_glMultiTexCoord3d)GetFunctionPointer("glMultiTexCoord3d");
    if ( QGLglMultiTexCoord3d )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3dv = (pfn_glMultiTexCoord3dv)GetFunctionPointer("glMultiTexCoord3dv");
    if ( QGLglMultiTexCoord3dv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3f = (pfn_glMultiTexCoord3f)GetFunctionPointer("glMultiTexCoord3f");
    if ( QGLglMultiTexCoord3f )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3fv = (pfn_glMultiTexCoord3fv)GetFunctionPointer("glMultiTexCoord3fv");
    if ( QGLglMultiTexCoord3fv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3i = (pfn_glMultiTexCoord3i)GetFunctionPointer("glMultiTexCoord3i");
    if ( QGLglMultiTexCoord3i )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3iv = (pfn_glMultiTexCoord3iv)GetFunctionPointer("glMultiTexCoord3iv");
    if ( QGLglMultiTexCoord3iv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3s = (pfn_glMultiTexCoord3s)GetFunctionPointer("glMultiTexCoord3s");
    if ( QGLglMultiTexCoord3s )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord3sv = (pfn_glMultiTexCoord3sv)GetFunctionPointer("glMultiTexCoord3sv");
    if ( QGLglMultiTexCoord3sv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4d = (pfn_glMultiTexCoord4d)GetFunctionPointer("glMultiTexCoord4d");
    if ( QGLglMultiTexCoord4d )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4dv = (pfn_glMultiTexCoord4dv)GetFunctionPointer("glMultiTexCoord4dv");
    if ( QGLglMultiTexCoord4dv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4f = (pfn_glMultiTexCoord4f)GetFunctionPointer("glMultiTexCoord4f");
    if ( QGLglMultiTexCoord4f )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4fv = (pfn_glMultiTexCoord4fv)GetFunctionPointer("glMultiTexCoord4fv");
    if ( QGLglMultiTexCoord4fv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4i = (pfn_glMultiTexCoord4i)GetFunctionPointer("glMultiTexCoord4i");
    if ( QGLglMultiTexCoord4i )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4iv = (pfn_glMultiTexCoord4iv)GetFunctionPointer("glMultiTexCoord4iv");
    if ( QGLglMultiTexCoord4iv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4s = (pfn_glMultiTexCoord4s)GetFunctionPointer("glMultiTexCoord4s");
    if ( QGLglMultiTexCoord4s )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultiTexCoord4sv = (pfn_glMultiTexCoord4sv)GetFunctionPointer("glMultiTexCoord4sv");
    if ( QGLglMultiTexCoord4sv )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglLoadTransposeMatrixf = (pfn_glLoadTransposeMatrixf)GetFunctionPointer("glLoadTransposeMatrixf");
    if ( QGLglLoadTransposeMatrixf )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglLoadTransposeMatrixd = (pfn_glLoadTransposeMatrixd)GetFunctionPointer("glLoadTransposeMatrixd");
    if ( QGLglLoadTransposeMatrixd )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultTransposeMatrixf = (pfn_glMultTransposeMatrixf)GetFunctionPointer("glMultTransposeMatrixf");
    if ( QGLglMultTransposeMatrixf )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglMultTransposeMatrixd = (pfn_glMultTransposeMatrixd)GetFunctionPointer("glMultTransposeMatrixd");
    if ( QGLglMultTransposeMatrixd )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglSampleCoverage = (pfn_glSampleCoverage)GetFunctionPointer("glSampleCoverage");
    if ( QGLglSampleCoverage )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexImage3D = (pfn_glCompressedTexImage3D)GetFunctionPointer("glCompressedTexImage3D");
    if ( QGLglCompressedTexImage3D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexImage2D = (pfn_glCompressedTexImage2D)GetFunctionPointer("glCompressedTexImage2D");
    if ( QGLglCompressedTexImage2D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexImage1D = (pfn_glCompressedTexImage1D)GetFunctionPointer("glCompressedTexImage1D");
    if ( QGLglCompressedTexImage1D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexSubImage3D = (pfn_glCompressedTexSubImage3D)GetFunctionPointer("glCompressedTexSubImage3D");
    if ( QGLglCompressedTexSubImage3D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexSubImage2D = (pfn_glCompressedTexSubImage2D)GetFunctionPointer("glCompressedTexSubImage2D");
    if ( QGLglCompressedTexSubImage2D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglCompressedTexSubImage1D = (pfn_glCompressedTexSubImage1D)GetFunctionPointer("glCompressedTexSubImage1D");
    if ( QGLglCompressedTexSubImage1D )
        ++QGLNumFunctions_GL_VERSION_1_3;

    QGLglGetCompressedTexImage = (pfn_glGetCompressedTexImage)GetFunctionPointer("glGetCompressedTexImage");
    if ( QGLglGetCompressedTexImage )
        ++QGLNumFunctions_GL_VERSION_1_3;

    if ( QGLNumFunctions_GL_VERSION_1_3 == 46 )
        QGLExists_GL_VERSION_1_3 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_VERSION_1_3 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_VERSION_1_3 ()
{
    QGLNumFunctions_GL_VERSION_1_3 = 0;
    QGLExists_GL_VERSION_1_3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_1_4;
int QGLNumFunctions_GL_VERSION_1_4;
#ifdef QGL_DEFINED_GL_VERSION_1_4
pfn_glBlendFuncSeparate QGLglBlendFuncSeparate = 0;
pfn_glFogCoordf QGLglFogCoordf = 0;
pfn_glFogCoordfv QGLglFogCoordfv = 0;
pfn_glFogCoordd QGLglFogCoordd = 0;
pfn_glFogCoorddv QGLglFogCoorddv = 0;
pfn_glFogCoordPointer QGLglFogCoordPointer = 0;
pfn_glMultiDrawArrays QGLglMultiDrawArrays = 0;
pfn_glMultiDrawElements QGLglMultiDrawElements = 0;
pfn_glPointParameterf QGLglPointParameterf = 0;
pfn_glPointParameterfv QGLglPointParameterfv = 0;
pfn_glPointParameteri QGLglPointParameteri = 0;
pfn_glPointParameteriv QGLglPointParameteriv = 0;
pfn_glSecondaryColor3b QGLglSecondaryColor3b = 0;
pfn_glSecondaryColor3bv QGLglSecondaryColor3bv = 0;
pfn_glSecondaryColor3d QGLglSecondaryColor3d = 0;
pfn_glSecondaryColor3dv QGLglSecondaryColor3dv = 0;
pfn_glSecondaryColor3f QGLglSecondaryColor3f = 0;
pfn_glSecondaryColor3fv QGLglSecondaryColor3fv = 0;
pfn_glSecondaryColor3i QGLglSecondaryColor3i = 0;
pfn_glSecondaryColor3iv QGLglSecondaryColor3iv = 0;
pfn_glSecondaryColor3s QGLglSecondaryColor3s = 0;
pfn_glSecondaryColor3sv QGLglSecondaryColor3sv = 0;
pfn_glSecondaryColor3ub QGLglSecondaryColor3ub = 0;
pfn_glSecondaryColor3ubv QGLglSecondaryColor3ubv = 0;
pfn_glSecondaryColor3ui QGLglSecondaryColor3ui = 0;
pfn_glSecondaryColor3uiv QGLglSecondaryColor3uiv = 0;
pfn_glSecondaryColor3us QGLglSecondaryColor3us = 0;
pfn_glSecondaryColor3usv QGLglSecondaryColor3usv = 0;
pfn_glSecondaryColorPointer QGLglSecondaryColorPointer = 0;
pfn_glWindowPos2d QGLglWindowPos2d = 0;
pfn_glWindowPos2dv QGLglWindowPos2dv = 0;
pfn_glWindowPos2f QGLglWindowPos2f = 0;
pfn_glWindowPos2fv QGLglWindowPos2fv = 0;
pfn_glWindowPos2i QGLglWindowPos2i = 0;
pfn_glWindowPos2iv QGLglWindowPos2iv = 0;
pfn_glWindowPos2s QGLglWindowPos2s = 0;
pfn_glWindowPos2sv QGLglWindowPos2sv = 0;
pfn_glWindowPos3d QGLglWindowPos3d = 0;
pfn_glWindowPos3dv QGLglWindowPos3dv = 0;
pfn_glWindowPos3f QGLglWindowPos3f = 0;
pfn_glWindowPos3fv QGLglWindowPos3fv = 0;
pfn_glWindowPos3i QGLglWindowPos3i = 0;
pfn_glWindowPos3iv QGLglWindowPos3iv = 0;
pfn_glWindowPos3s QGLglWindowPos3s = 0;
pfn_glWindowPos3sv QGLglWindowPos3sv = 0;

static void Init_GL_VERSION_1_4 ()
{
    QGLNumFunctions_GL_VERSION_1_4 = 0;

    QGLglBlendFuncSeparate = (pfn_glBlendFuncSeparate)GetFunctionPointer("glBlendFuncSeparate");
    if ( QGLglBlendFuncSeparate )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglFogCoordf = (pfn_glFogCoordf)GetFunctionPointer("glFogCoordf");
    if ( QGLglFogCoordf )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglFogCoordfv = (pfn_glFogCoordfv)GetFunctionPointer("glFogCoordfv");
    if ( QGLglFogCoordfv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglFogCoordd = (pfn_glFogCoordd)GetFunctionPointer("glFogCoordd");
    if ( QGLglFogCoordd )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglFogCoorddv = (pfn_glFogCoorddv)GetFunctionPointer("glFogCoorddv");
    if ( QGLglFogCoorddv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglFogCoordPointer = (pfn_glFogCoordPointer)GetFunctionPointer("glFogCoordPointer");
    if ( QGLglFogCoordPointer )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglMultiDrawArrays = (pfn_glMultiDrawArrays)GetFunctionPointer("glMultiDrawArrays");
    if ( QGLglMultiDrawArrays )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglMultiDrawElements = (pfn_glMultiDrawElements)GetFunctionPointer("glMultiDrawElements");
    if ( QGLglMultiDrawElements )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglPointParameterf = (pfn_glPointParameterf)GetFunctionPointer("glPointParameterf");
    if ( QGLglPointParameterf )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglPointParameterfv = (pfn_glPointParameterfv)GetFunctionPointer("glPointParameterfv");
    if ( QGLglPointParameterfv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglPointParameteri = (pfn_glPointParameteri)GetFunctionPointer("glPointParameteri");
    if ( QGLglPointParameteri )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglPointParameteriv = (pfn_glPointParameteriv)GetFunctionPointer("glPointParameteriv");
    if ( QGLglPointParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3b = (pfn_glSecondaryColor3b)GetFunctionPointer("glSecondaryColor3b");
    if ( QGLglSecondaryColor3b )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3bv = (pfn_glSecondaryColor3bv)GetFunctionPointer("glSecondaryColor3bv");
    if ( QGLglSecondaryColor3bv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3d = (pfn_glSecondaryColor3d)GetFunctionPointer("glSecondaryColor3d");
    if ( QGLglSecondaryColor3d )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3dv = (pfn_glSecondaryColor3dv)GetFunctionPointer("glSecondaryColor3dv");
    if ( QGLglSecondaryColor3dv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3f = (pfn_glSecondaryColor3f)GetFunctionPointer("glSecondaryColor3f");
    if ( QGLglSecondaryColor3f )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3fv = (pfn_glSecondaryColor3fv)GetFunctionPointer("glSecondaryColor3fv");
    if ( QGLglSecondaryColor3fv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3i = (pfn_glSecondaryColor3i)GetFunctionPointer("glSecondaryColor3i");
    if ( QGLglSecondaryColor3i )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3iv = (pfn_glSecondaryColor3iv)GetFunctionPointer("glSecondaryColor3iv");
    if ( QGLglSecondaryColor3iv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3s = (pfn_glSecondaryColor3s)GetFunctionPointer("glSecondaryColor3s");
    if ( QGLglSecondaryColor3s )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3sv = (pfn_glSecondaryColor3sv)GetFunctionPointer("glSecondaryColor3sv");
    if ( QGLglSecondaryColor3sv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3ub = (pfn_glSecondaryColor3ub)GetFunctionPointer("glSecondaryColor3ub");
    if ( QGLglSecondaryColor3ub )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3ubv = (pfn_glSecondaryColor3ubv)GetFunctionPointer("glSecondaryColor3ubv");
    if ( QGLglSecondaryColor3ubv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3ui = (pfn_glSecondaryColor3ui)GetFunctionPointer("glSecondaryColor3ui");
    if ( QGLglSecondaryColor3ui )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3uiv = (pfn_glSecondaryColor3uiv)GetFunctionPointer("glSecondaryColor3uiv");
    if ( QGLglSecondaryColor3uiv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3us = (pfn_glSecondaryColor3us)GetFunctionPointer("glSecondaryColor3us");
    if ( QGLglSecondaryColor3us )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColor3usv = (pfn_glSecondaryColor3usv)GetFunctionPointer("glSecondaryColor3usv");
    if ( QGLglSecondaryColor3usv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglSecondaryColorPointer = (pfn_glSecondaryColorPointer)GetFunctionPointer("glSecondaryColorPointer");
    if ( QGLglSecondaryColorPointer )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2d = (pfn_glWindowPos2d)GetFunctionPointer("glWindowPos2d");
    if ( QGLglWindowPos2d )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2dv = (pfn_glWindowPos2dv)GetFunctionPointer("glWindowPos2dv");
    if ( QGLglWindowPos2dv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2f = (pfn_glWindowPos2f)GetFunctionPointer("glWindowPos2f");
    if ( QGLglWindowPos2f )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2fv = (pfn_glWindowPos2fv)GetFunctionPointer("glWindowPos2fv");
    if ( QGLglWindowPos2fv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2i = (pfn_glWindowPos2i)GetFunctionPointer("glWindowPos2i");
    if ( QGLglWindowPos2i )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2iv = (pfn_glWindowPos2iv)GetFunctionPointer("glWindowPos2iv");
    if ( QGLglWindowPos2iv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2s = (pfn_glWindowPos2s)GetFunctionPointer("glWindowPos2s");
    if ( QGLglWindowPos2s )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos2sv = (pfn_glWindowPos2sv)GetFunctionPointer("glWindowPos2sv");
    if ( QGLglWindowPos2sv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3d = (pfn_glWindowPos3d)GetFunctionPointer("glWindowPos3d");
    if ( QGLglWindowPos3d )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3dv = (pfn_glWindowPos3dv)GetFunctionPointer("glWindowPos3dv");
    if ( QGLglWindowPos3dv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3f = (pfn_glWindowPos3f)GetFunctionPointer("glWindowPos3f");
    if ( QGLglWindowPos3f )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3fv = (pfn_glWindowPos3fv)GetFunctionPointer("glWindowPos3fv");
    if ( QGLglWindowPos3fv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3i = (pfn_glWindowPos3i)GetFunctionPointer("glWindowPos3i");
    if ( QGLglWindowPos3i )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3iv = (pfn_glWindowPos3iv)GetFunctionPointer("glWindowPos3iv");
    if ( QGLglWindowPos3iv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3s = (pfn_glWindowPos3s)GetFunctionPointer("glWindowPos3s");
    if ( QGLglWindowPos3s )
        ++QGLNumFunctions_GL_VERSION_1_4;

    QGLglWindowPos3sv = (pfn_glWindowPos3sv)GetFunctionPointer("glWindowPos3sv");
    if ( QGLglWindowPos3sv )
        ++QGLNumFunctions_GL_VERSION_1_4;

    if ( QGLNumFunctions_GL_VERSION_1_4 == 45 )
        QGLExists_GL_VERSION_1_4 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_VERSION_1_4 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_VERSION_1_4 ()
{
    QGLNumFunctions_GL_VERSION_1_4 = 0;
    QGLExists_GL_VERSION_1_4 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_1_5;
int QGLNumFunctions_GL_VERSION_1_5;
#ifdef QGL_DEFINED_GL_VERSION_1_5
pfn_glGenQueries QGLglGenQueries = 0;
pfn_glDeleteQueries QGLglDeleteQueries = 0;
pfn_glIsQuery QGLglIsQuery = 0;
pfn_glBeginQuery QGLglBeginQuery = 0;
pfn_glEndQuery QGLglEndQuery = 0;
pfn_glGetQueryiv QGLglGetQueryiv = 0;
pfn_glGetQueryObjectiv QGLglGetQueryObjectiv = 0;
pfn_glGetQueryObjectuiv QGLglGetQueryObjectuiv = 0;
pfn_glBindBuffer QGLglBindBuffer = 0;
pfn_glDeleteBuffers QGLglDeleteBuffers = 0;
pfn_glGenBuffers QGLglGenBuffers = 0;
pfn_glIsBuffer QGLglIsBuffer = 0;
pfn_glBufferData QGLglBufferData = 0;
pfn_glBufferSubData QGLglBufferSubData = 0;
pfn_glGetBufferSubData QGLglGetBufferSubData = 0;
pfn_glMapBuffer QGLglMapBuffer = 0;
pfn_glUnmapBuffer QGLglUnmapBuffer = 0;
pfn_glGetBufferParameteriv QGLglGetBufferParameteriv = 0;
pfn_glGetBufferPointerv QGLglGetBufferPointerv = 0;

static void Init_GL_VERSION_1_5 ()
{
    QGLNumFunctions_GL_VERSION_1_5 = 0;

    QGLglGenQueries = (pfn_glGenQueries)GetFunctionPointer("glGenQueries");
    if ( QGLglGenQueries )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglDeleteQueries = (pfn_glDeleteQueries)GetFunctionPointer("glDeleteQueries");
    if ( QGLglDeleteQueries )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglIsQuery = (pfn_glIsQuery)GetFunctionPointer("glIsQuery");
    if ( QGLglIsQuery )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglBeginQuery = (pfn_glBeginQuery)GetFunctionPointer("glBeginQuery");
    if ( QGLglBeginQuery )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglEndQuery = (pfn_glEndQuery)GetFunctionPointer("glEndQuery");
    if ( QGLglEndQuery )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetQueryiv = (pfn_glGetQueryiv)GetFunctionPointer("glGetQueryiv");
    if ( QGLglGetQueryiv )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetQueryObjectiv = (pfn_glGetQueryObjectiv)GetFunctionPointer("glGetQueryObjectiv");
    if ( QGLglGetQueryObjectiv )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetQueryObjectuiv = (pfn_glGetQueryObjectuiv)GetFunctionPointer("glGetQueryObjectuiv");
    if ( QGLglGetQueryObjectuiv )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglBindBuffer = (pfn_glBindBuffer)GetFunctionPointer("glBindBuffer");
    if ( QGLglBindBuffer )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglDeleteBuffers = (pfn_glDeleteBuffers)GetFunctionPointer("glDeleteBuffers");
    if ( QGLglDeleteBuffers )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGenBuffers = (pfn_glGenBuffers)GetFunctionPointer("glGenBuffers");
    if ( QGLglGenBuffers )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglIsBuffer = (pfn_glIsBuffer)GetFunctionPointer("glIsBuffer");
    if ( QGLglIsBuffer )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglBufferData = (pfn_glBufferData)GetFunctionPointer("glBufferData");
    if ( QGLglBufferData )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglBufferSubData = (pfn_glBufferSubData)GetFunctionPointer("glBufferSubData");
    if ( QGLglBufferSubData )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetBufferSubData = (pfn_glGetBufferSubData)GetFunctionPointer("glGetBufferSubData");
    if ( QGLglGetBufferSubData )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglMapBuffer = (pfn_glMapBuffer)GetFunctionPointer("glMapBuffer");
    if ( QGLglMapBuffer )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglUnmapBuffer = (pfn_glUnmapBuffer)GetFunctionPointer("glUnmapBuffer");
    if ( QGLglUnmapBuffer )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetBufferParameteriv = (pfn_glGetBufferParameteriv)GetFunctionPointer("glGetBufferParameteriv");
    if ( QGLglGetBufferParameteriv )
        ++QGLNumFunctions_GL_VERSION_1_5;

    QGLglGetBufferPointerv = (pfn_glGetBufferPointerv)GetFunctionPointer("glGetBufferPointerv");
    if ( QGLglGetBufferPointerv )
        ++QGLNumFunctions_GL_VERSION_1_5;

    if ( QGLNumFunctions_GL_VERSION_1_5 == 19 )
        QGLExists_GL_VERSION_1_5 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_VERSION_1_5 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_VERSION_1_5 ()
{
    QGLNumFunctions_GL_VERSION_1_5 = 0;
    QGLExists_GL_VERSION_1_5 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_VERSION_2_0;
int QGLNumFunctions_GL_VERSION_2_0;
#ifdef QGL_DEFINED_GL_VERSION_2_0
pfn_glBlendEquationSeparate QGLglBlendEquationSeparate = 0;
pfn_glDrawBuffers QGLglDrawBuffers = 0;
pfn_glStencilOpSeparate QGLglStencilOpSeparate = 0;
pfn_glStencilFuncSeparate QGLglStencilFuncSeparate = 0;
pfn_glStencilMaskSeparate QGLglStencilMaskSeparate = 0;
pfn_glAttachShader QGLglAttachShader = 0;
pfn_glBindAttribLocation QGLglBindAttribLocation = 0;
pfn_glCompileShader QGLglCompileShader = 0;
pfn_glCreateProgram QGLglCreateProgram = 0;
pfn_glCreateShader QGLglCreateShader = 0;
pfn_glDeleteProgram QGLglDeleteProgram = 0;
pfn_glDeleteShader QGLglDeleteShader = 0;
pfn_glDetachShader QGLglDetachShader = 0;
pfn_glDisableVertexAttribArray QGLglDisableVertexAttribArray = 0;
pfn_glEnableVertexAttribArray QGLglEnableVertexAttribArray = 0;
pfn_glGetActiveAttrib QGLglGetActiveAttrib = 0;
pfn_glGetActiveUniform QGLglGetActiveUniform = 0;
pfn_glGetAttachedShaders QGLglGetAttachedShaders = 0;
pfn_glGetAttribLocation QGLglGetAttribLocation = 0;
pfn_glGetProgramiv QGLglGetProgramiv = 0;
pfn_glGetProgramInfoLog QGLglGetProgramInfoLog = 0;
pfn_glGetShaderiv QGLglGetShaderiv = 0;
pfn_glGetShaderInfoLog QGLglGetShaderInfoLog = 0;
pfn_glGetShaderSource QGLglGetShaderSource = 0;
pfn_glGetUniformLocation QGLglGetUniformLocation = 0;
pfn_glGetUniformfv QGLglGetUniformfv = 0;
pfn_glGetUniformiv QGLglGetUniformiv = 0;
pfn_glGetVertexAttribdv QGLglGetVertexAttribdv = 0;
pfn_glGetVertexAttribfv QGLglGetVertexAttribfv = 0;
pfn_glGetVertexAttribiv QGLglGetVertexAttribiv = 0;
pfn_glGetVertexAttribPointerv QGLglGetVertexAttribPointerv = 0;
pfn_glIsProgram QGLglIsProgram = 0;
pfn_glIsShader QGLglIsShader = 0;
pfn_glLinkProgram QGLglLinkProgram = 0;
pfn_glShaderSource QGLglShaderSource = 0;
pfn_glUseProgram QGLglUseProgram = 0;
pfn_glUniform1f QGLglUniform1f = 0;
pfn_glUniform2f QGLglUniform2f = 0;
pfn_glUniform3f QGLglUniform3f = 0;
pfn_glUniform4f QGLglUniform4f = 0;
pfn_glUniform1i QGLglUniform1i = 0;
pfn_glUniform2i QGLglUniform2i = 0;
pfn_glUniform3i QGLglUniform3i = 0;
pfn_glUniform4i QGLglUniform4i = 0;
pfn_glUniform1fv QGLglUniform1fv = 0;
pfn_glUniform2fv QGLglUniform2fv = 0;
pfn_glUniform3fv QGLglUniform3fv = 0;
pfn_glUniform4fv QGLglUniform4fv = 0;
pfn_glUniform1iv QGLglUniform1iv = 0;
pfn_glUniform2iv QGLglUniform2iv = 0;
pfn_glUniform3iv QGLglUniform3iv = 0;
pfn_glUniform4iv QGLglUniform4iv = 0;
pfn_glUniformMatrix2fv QGLglUniformMatrix2fv = 0;
pfn_glUniformMatrix3fv QGLglUniformMatrix3fv = 0;
pfn_glUniformMatrix4fv QGLglUniformMatrix4fv = 0;
pfn_glValidateProgram QGLglValidateProgram = 0;
pfn_glVertexAttrib1d QGLglVertexAttrib1d = 0;
pfn_glVertexAttrib1dv QGLglVertexAttrib1dv = 0;
pfn_glVertexAttrib1f QGLglVertexAttrib1f = 0;
pfn_glVertexAttrib1fv QGLglVertexAttrib1fv = 0;
pfn_glVertexAttrib1s QGLglVertexAttrib1s = 0;
pfn_glVertexAttrib1sv QGLglVertexAttrib1sv = 0;
pfn_glVertexAttrib2d QGLglVertexAttrib2d = 0;
pfn_glVertexAttrib2dv QGLglVertexAttrib2dv = 0;
pfn_glVertexAttrib2f QGLglVertexAttrib2f = 0;
pfn_glVertexAttrib2fv QGLglVertexAttrib2fv = 0;
pfn_glVertexAttrib2s QGLglVertexAttrib2s = 0;
pfn_glVertexAttrib2sv QGLglVertexAttrib2sv = 0;
pfn_glVertexAttrib3d QGLglVertexAttrib3d = 0;
pfn_glVertexAttrib3dv QGLglVertexAttrib3dv = 0;
pfn_glVertexAttrib3f QGLglVertexAttrib3f = 0;
pfn_glVertexAttrib3fv QGLglVertexAttrib3fv = 0;
pfn_glVertexAttrib3s QGLglVertexAttrib3s = 0;
pfn_glVertexAttrib3sv QGLglVertexAttrib3sv = 0;
pfn_glVertexAttrib4Nbv QGLglVertexAttrib4Nbv = 0;
pfn_glVertexAttrib4Niv QGLglVertexAttrib4Niv = 0;
pfn_glVertexAttrib4Nsv QGLglVertexAttrib4Nsv = 0;
pfn_glVertexAttrib4Nub QGLglVertexAttrib4Nub = 0;
pfn_glVertexAttrib4Nubv QGLglVertexAttrib4Nubv = 0;
pfn_glVertexAttrib4Nuiv QGLglVertexAttrib4Nuiv = 0;
pfn_glVertexAttrib4Nusv QGLglVertexAttrib4Nusv = 0;
pfn_glVertexAttrib4bv QGLglVertexAttrib4bv = 0;
pfn_glVertexAttrib4d QGLglVertexAttrib4d = 0;
pfn_glVertexAttrib4dv QGLglVertexAttrib4dv = 0;
pfn_glVertexAttrib4f QGLglVertexAttrib4f = 0;
pfn_glVertexAttrib4fv QGLglVertexAttrib4fv = 0;
pfn_glVertexAttrib4iv QGLglVertexAttrib4iv = 0;
pfn_glVertexAttrib4s QGLglVertexAttrib4s = 0;
pfn_glVertexAttrib4sv QGLglVertexAttrib4sv = 0;
pfn_glVertexAttrib4ubv QGLglVertexAttrib4ubv = 0;
pfn_glVertexAttrib4uiv QGLglVertexAttrib4uiv = 0;
pfn_glVertexAttrib4usv QGLglVertexAttrib4usv = 0;
pfn_glVertexAttribPointer QGLglVertexAttribPointer = 0;

static void Init_GL_VERSION_2_0 ()
{
    QGLNumFunctions_GL_VERSION_2_0 = 0;

    QGLglBlendEquationSeparate = (pfn_glBlendEquationSeparate)GetFunctionPointer("glBlendEquationSeparate");
    if ( QGLglBlendEquationSeparate )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglDrawBuffers = (pfn_glDrawBuffers)GetFunctionPointer("glDrawBuffers");
    if ( QGLglDrawBuffers )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglStencilOpSeparate = (pfn_glStencilOpSeparate)GetFunctionPointer("glStencilOpSeparate");
    if ( QGLglStencilOpSeparate )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglStencilFuncSeparate = (pfn_glStencilFuncSeparate)GetFunctionPointer("glStencilFuncSeparate");
    if ( QGLglStencilFuncSeparate )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglStencilMaskSeparate = (pfn_glStencilMaskSeparate)GetFunctionPointer("glStencilMaskSeparate");
    if ( QGLglStencilMaskSeparate )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglAttachShader = (pfn_glAttachShader)GetFunctionPointer("glAttachShader");
    if ( QGLglAttachShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglBindAttribLocation = (pfn_glBindAttribLocation)GetFunctionPointer("glBindAttribLocation");
    if ( QGLglBindAttribLocation )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglCompileShader = (pfn_glCompileShader)GetFunctionPointer("glCompileShader");
    if ( QGLglCompileShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglCreateProgram = (pfn_glCreateProgram)GetFunctionPointer("glCreateProgram");
    if ( QGLglCreateProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglCreateShader = (pfn_glCreateShader)GetFunctionPointer("glCreateShader");
    if ( QGLglCreateShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglDeleteProgram = (pfn_glDeleteProgram)GetFunctionPointer("glDeleteProgram");
    if ( QGLglDeleteProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglDeleteShader = (pfn_glDeleteShader)GetFunctionPointer("glDeleteShader");
    if ( QGLglDeleteShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglDetachShader = (pfn_glDetachShader)GetFunctionPointer("glDetachShader");
    if ( QGLglDetachShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglDisableVertexAttribArray = (pfn_glDisableVertexAttribArray)GetFunctionPointer("glDisableVertexAttribArray");
    if ( QGLglDisableVertexAttribArray )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglEnableVertexAttribArray = (pfn_glEnableVertexAttribArray)GetFunctionPointer("glEnableVertexAttribArray");
    if ( QGLglEnableVertexAttribArray )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetActiveAttrib = (pfn_glGetActiveAttrib)GetFunctionPointer("glGetActiveAttrib");
    if ( QGLglGetActiveAttrib )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetActiveUniform = (pfn_glGetActiveUniform)GetFunctionPointer("glGetActiveUniform");
    if ( QGLglGetActiveUniform )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetAttachedShaders = (pfn_glGetAttachedShaders)GetFunctionPointer("glGetAttachedShaders");
    if ( QGLglGetAttachedShaders )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetAttribLocation = (pfn_glGetAttribLocation)GetFunctionPointer("glGetAttribLocation");
    if ( QGLglGetAttribLocation )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetProgramiv = (pfn_glGetProgramiv)GetFunctionPointer("glGetProgramiv");
    if ( QGLglGetProgramiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetProgramInfoLog = (pfn_glGetProgramInfoLog)GetFunctionPointer("glGetProgramInfoLog");
    if ( QGLglGetProgramInfoLog )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetShaderiv = (pfn_glGetShaderiv)GetFunctionPointer("glGetShaderiv");
    if ( QGLglGetShaderiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetShaderInfoLog = (pfn_glGetShaderInfoLog)GetFunctionPointer("glGetShaderInfoLog");
    if ( QGLglGetShaderInfoLog )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetShaderSource = (pfn_glGetShaderSource)GetFunctionPointer("glGetShaderSource");
    if ( QGLglGetShaderSource )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetUniformLocation = (pfn_glGetUniformLocation)GetFunctionPointer("glGetUniformLocation");
    if ( QGLglGetUniformLocation )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetUniformfv = (pfn_glGetUniformfv)GetFunctionPointer("glGetUniformfv");
    if ( QGLglGetUniformfv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetUniformiv = (pfn_glGetUniformiv)GetFunctionPointer("glGetUniformiv");
    if ( QGLglGetUniformiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetVertexAttribdv = (pfn_glGetVertexAttribdv)GetFunctionPointer("glGetVertexAttribdv");
    if ( QGLglGetVertexAttribdv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetVertexAttribfv = (pfn_glGetVertexAttribfv)GetFunctionPointer("glGetVertexAttribfv");
    if ( QGLglGetVertexAttribfv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetVertexAttribiv = (pfn_glGetVertexAttribiv)GetFunctionPointer("glGetVertexAttribiv");
    if ( QGLglGetVertexAttribiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglGetVertexAttribPointerv = (pfn_glGetVertexAttribPointerv)GetFunctionPointer("glGetVertexAttribPointerv");
    if ( QGLglGetVertexAttribPointerv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglIsProgram = (pfn_glIsProgram)GetFunctionPointer("glIsProgram");
    if ( QGLglIsProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglIsShader = (pfn_glIsShader)GetFunctionPointer("glIsShader");
    if ( QGLglIsShader )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglLinkProgram = (pfn_glLinkProgram)GetFunctionPointer("glLinkProgram");
    if ( QGLglLinkProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglShaderSource = (pfn_glShaderSource)GetFunctionPointer("glShaderSource");
    if ( QGLglShaderSource )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUseProgram = (pfn_glUseProgram)GetFunctionPointer("glUseProgram");
    if ( QGLglUseProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform1f = (pfn_glUniform1f)GetFunctionPointer("glUniform1f");
    if ( QGLglUniform1f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform2f = (pfn_glUniform2f)GetFunctionPointer("glUniform2f");
    if ( QGLglUniform2f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform3f = (pfn_glUniform3f)GetFunctionPointer("glUniform3f");
    if ( QGLglUniform3f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform4f = (pfn_glUniform4f)GetFunctionPointer("glUniform4f");
    if ( QGLglUniform4f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform1i = (pfn_glUniform1i)GetFunctionPointer("glUniform1i");
    if ( QGLglUniform1i )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform2i = (pfn_glUniform2i)GetFunctionPointer("glUniform2i");
    if ( QGLglUniform2i )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform3i = (pfn_glUniform3i)GetFunctionPointer("glUniform3i");
    if ( QGLglUniform3i )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform4i = (pfn_glUniform4i)GetFunctionPointer("glUniform4i");
    if ( QGLglUniform4i )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform1fv = (pfn_glUniform1fv)GetFunctionPointer("glUniform1fv");
    if ( QGLglUniform1fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform2fv = (pfn_glUniform2fv)GetFunctionPointer("glUniform2fv");
    if ( QGLglUniform2fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform3fv = (pfn_glUniform3fv)GetFunctionPointer("glUniform3fv");
    if ( QGLglUniform3fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform4fv = (pfn_glUniform4fv)GetFunctionPointer("glUniform4fv");
    if ( QGLglUniform4fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform1iv = (pfn_glUniform1iv)GetFunctionPointer("glUniform1iv");
    if ( QGLglUniform1iv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform2iv = (pfn_glUniform2iv)GetFunctionPointer("glUniform2iv");
    if ( QGLglUniform2iv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform3iv = (pfn_glUniform3iv)GetFunctionPointer("glUniform3iv");
    if ( QGLglUniform3iv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniform4iv = (pfn_glUniform4iv)GetFunctionPointer("glUniform4iv");
    if ( QGLglUniform4iv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniformMatrix2fv = (pfn_glUniformMatrix2fv)GetFunctionPointer("glUniformMatrix2fv");
    if ( QGLglUniformMatrix2fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniformMatrix3fv = (pfn_glUniformMatrix3fv)GetFunctionPointer("glUniformMatrix3fv");
    if ( QGLglUniformMatrix3fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglUniformMatrix4fv = (pfn_glUniformMatrix4fv)GetFunctionPointer("glUniformMatrix4fv");
    if ( QGLglUniformMatrix4fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglValidateProgram = (pfn_glValidateProgram)GetFunctionPointer("glValidateProgram");
    if ( QGLglValidateProgram )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1d = (pfn_glVertexAttrib1d)GetFunctionPointer("glVertexAttrib1d");
    if ( QGLglVertexAttrib1d )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1dv = (pfn_glVertexAttrib1dv)GetFunctionPointer("glVertexAttrib1dv");
    if ( QGLglVertexAttrib1dv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1f = (pfn_glVertexAttrib1f)GetFunctionPointer("glVertexAttrib1f");
    if ( QGLglVertexAttrib1f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1fv = (pfn_glVertexAttrib1fv)GetFunctionPointer("glVertexAttrib1fv");
    if ( QGLglVertexAttrib1fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1s = (pfn_glVertexAttrib1s)GetFunctionPointer("glVertexAttrib1s");
    if ( QGLglVertexAttrib1s )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib1sv = (pfn_glVertexAttrib1sv)GetFunctionPointer("glVertexAttrib1sv");
    if ( QGLglVertexAttrib1sv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2d = (pfn_glVertexAttrib2d)GetFunctionPointer("glVertexAttrib2d");
    if ( QGLglVertexAttrib2d )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2dv = (pfn_glVertexAttrib2dv)GetFunctionPointer("glVertexAttrib2dv");
    if ( QGLglVertexAttrib2dv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2f = (pfn_glVertexAttrib2f)GetFunctionPointer("glVertexAttrib2f");
    if ( QGLglVertexAttrib2f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2fv = (pfn_glVertexAttrib2fv)GetFunctionPointer("glVertexAttrib2fv");
    if ( QGLglVertexAttrib2fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2s = (pfn_glVertexAttrib2s)GetFunctionPointer("glVertexAttrib2s");
    if ( QGLglVertexAttrib2s )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib2sv = (pfn_glVertexAttrib2sv)GetFunctionPointer("glVertexAttrib2sv");
    if ( QGLglVertexAttrib2sv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3d = (pfn_glVertexAttrib3d)GetFunctionPointer("glVertexAttrib3d");
    if ( QGLglVertexAttrib3d )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3dv = (pfn_glVertexAttrib3dv)GetFunctionPointer("glVertexAttrib3dv");
    if ( QGLglVertexAttrib3dv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3f = (pfn_glVertexAttrib3f)GetFunctionPointer("glVertexAttrib3f");
    if ( QGLglVertexAttrib3f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3fv = (pfn_glVertexAttrib3fv)GetFunctionPointer("glVertexAttrib3fv");
    if ( QGLglVertexAttrib3fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3s = (pfn_glVertexAttrib3s)GetFunctionPointer("glVertexAttrib3s");
    if ( QGLglVertexAttrib3s )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib3sv = (pfn_glVertexAttrib3sv)GetFunctionPointer("glVertexAttrib3sv");
    if ( QGLglVertexAttrib3sv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nbv = (pfn_glVertexAttrib4Nbv)GetFunctionPointer("glVertexAttrib4Nbv");
    if ( QGLglVertexAttrib4Nbv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Niv = (pfn_glVertexAttrib4Niv)GetFunctionPointer("glVertexAttrib4Niv");
    if ( QGLglVertexAttrib4Niv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nsv = (pfn_glVertexAttrib4Nsv)GetFunctionPointer("glVertexAttrib4Nsv");
    if ( QGLglVertexAttrib4Nsv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nub = (pfn_glVertexAttrib4Nub)GetFunctionPointer("glVertexAttrib4Nub");
    if ( QGLglVertexAttrib4Nub )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nubv = (pfn_glVertexAttrib4Nubv)GetFunctionPointer("glVertexAttrib4Nubv");
    if ( QGLglVertexAttrib4Nubv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nuiv = (pfn_glVertexAttrib4Nuiv)GetFunctionPointer("glVertexAttrib4Nuiv");
    if ( QGLglVertexAttrib4Nuiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4Nusv = (pfn_glVertexAttrib4Nusv)GetFunctionPointer("glVertexAttrib4Nusv");
    if ( QGLglVertexAttrib4Nusv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4bv = (pfn_glVertexAttrib4bv)GetFunctionPointer("glVertexAttrib4bv");
    if ( QGLglVertexAttrib4bv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4d = (pfn_glVertexAttrib4d)GetFunctionPointer("glVertexAttrib4d");
    if ( QGLglVertexAttrib4d )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4dv = (pfn_glVertexAttrib4dv)GetFunctionPointer("glVertexAttrib4dv");
    if ( QGLglVertexAttrib4dv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4f = (pfn_glVertexAttrib4f)GetFunctionPointer("glVertexAttrib4f");
    if ( QGLglVertexAttrib4f )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4fv = (pfn_glVertexAttrib4fv)GetFunctionPointer("glVertexAttrib4fv");
    if ( QGLglVertexAttrib4fv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4iv = (pfn_glVertexAttrib4iv)GetFunctionPointer("glVertexAttrib4iv");
    if ( QGLglVertexAttrib4iv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4s = (pfn_glVertexAttrib4s)GetFunctionPointer("glVertexAttrib4s");
    if ( QGLglVertexAttrib4s )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4sv = (pfn_glVertexAttrib4sv)GetFunctionPointer("glVertexAttrib4sv");
    if ( QGLglVertexAttrib4sv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4ubv = (pfn_glVertexAttrib4ubv)GetFunctionPointer("glVertexAttrib4ubv");
    if ( QGLglVertexAttrib4ubv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4uiv = (pfn_glVertexAttrib4uiv)GetFunctionPointer("glVertexAttrib4uiv");
    if ( QGLglVertexAttrib4uiv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttrib4usv = (pfn_glVertexAttrib4usv)GetFunctionPointer("glVertexAttrib4usv");
    if ( QGLglVertexAttrib4usv )
        ++QGLNumFunctions_GL_VERSION_2_0;

    QGLglVertexAttribPointer = (pfn_glVertexAttribPointer)GetFunctionPointer("glVertexAttribPointer");
    if ( QGLglVertexAttribPointer )
        ++QGLNumFunctions_GL_VERSION_2_0;

    if ( QGLNumFunctions_GL_VERSION_2_0 == 93 )
        QGLExists_GL_VERSION_2_0 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_VERSION_2_0 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_VERSION_2_0 ()
{
    QGLNumFunctions_GL_VERSION_2_0 = 0;
    QGLExists_GL_VERSION_2_0 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_imaging;
int QGLNumFunctions_GL_ARB_imaging;
#ifdef QGL_DEFINED_GL_ARB_imaging
static void Init_GL_ARB_imaging ()
{
    QGLNumFunctions_GL_ARB_imaging = 0;
    QGLExists_GL_ARB_imaging = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_imaging ()
{
    QGLNumFunctions_GL_ARB_imaging = 0;
    QGLExists_GL_ARB_imaging = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_multitexture;
int QGLNumFunctions_GL_ARB_multitexture;
#ifdef QGL_DEFINED_GL_ARB_multitexture
pfn_glActiveTextureARB QGLglActiveTextureARB = 0;
pfn_glClientActiveTextureARB QGLglClientActiveTextureARB = 0;
pfn_glMultiTexCoord1dARB QGLglMultiTexCoord1dARB = 0;
pfn_glMultiTexCoord1dvARB QGLglMultiTexCoord1dvARB = 0;
pfn_glMultiTexCoord1fARB QGLglMultiTexCoord1fARB = 0;
pfn_glMultiTexCoord1fvARB QGLglMultiTexCoord1fvARB = 0;
pfn_glMultiTexCoord1iARB QGLglMultiTexCoord1iARB = 0;
pfn_glMultiTexCoord1ivARB QGLglMultiTexCoord1ivARB = 0;
pfn_glMultiTexCoord1sARB QGLglMultiTexCoord1sARB = 0;
pfn_glMultiTexCoord1svARB QGLglMultiTexCoord1svARB = 0;
pfn_glMultiTexCoord2dARB QGLglMultiTexCoord2dARB = 0;
pfn_glMultiTexCoord2dvARB QGLglMultiTexCoord2dvARB = 0;
pfn_glMultiTexCoord2fARB QGLglMultiTexCoord2fARB = 0;
pfn_glMultiTexCoord2fvARB QGLglMultiTexCoord2fvARB = 0;
pfn_glMultiTexCoord2iARB QGLglMultiTexCoord2iARB = 0;
pfn_glMultiTexCoord2ivARB QGLglMultiTexCoord2ivARB = 0;
pfn_glMultiTexCoord2sARB QGLglMultiTexCoord2sARB = 0;
pfn_glMultiTexCoord2svARB QGLglMultiTexCoord2svARB = 0;
pfn_glMultiTexCoord3dARB QGLglMultiTexCoord3dARB = 0;
pfn_glMultiTexCoord3dvARB QGLglMultiTexCoord3dvARB = 0;
pfn_glMultiTexCoord3fARB QGLglMultiTexCoord3fARB = 0;
pfn_glMultiTexCoord3fvARB QGLglMultiTexCoord3fvARB = 0;
pfn_glMultiTexCoord3iARB QGLglMultiTexCoord3iARB = 0;
pfn_glMultiTexCoord3ivARB QGLglMultiTexCoord3ivARB = 0;
pfn_glMultiTexCoord3sARB QGLglMultiTexCoord3sARB = 0;
pfn_glMultiTexCoord3svARB QGLglMultiTexCoord3svARB = 0;
pfn_glMultiTexCoord4dARB QGLglMultiTexCoord4dARB = 0;
pfn_glMultiTexCoord4dvARB QGLglMultiTexCoord4dvARB = 0;
pfn_glMultiTexCoord4fARB QGLglMultiTexCoord4fARB = 0;
pfn_glMultiTexCoord4fvARB QGLglMultiTexCoord4fvARB = 0;
pfn_glMultiTexCoord4iARB QGLglMultiTexCoord4iARB = 0;
pfn_glMultiTexCoord4ivARB QGLglMultiTexCoord4ivARB = 0;
pfn_glMultiTexCoord4sARB QGLglMultiTexCoord4sARB = 0;
pfn_glMultiTexCoord4svARB QGLglMultiTexCoord4svARB = 0;

static void Init_GL_ARB_multitexture ()
{
    QGLNumFunctions_GL_ARB_multitexture = 0;

    QGLglActiveTextureARB = (pfn_glActiveTextureARB)GetFunctionPointer("glActiveTextureARB");
    if ( QGLglActiveTextureARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglClientActiveTextureARB = (pfn_glClientActiveTextureARB)GetFunctionPointer("glClientActiveTextureARB");
    if ( QGLglClientActiveTextureARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1dARB = (pfn_glMultiTexCoord1dARB)GetFunctionPointer("glMultiTexCoord1dARB");
    if ( QGLglMultiTexCoord1dARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1dvARB = (pfn_glMultiTexCoord1dvARB)GetFunctionPointer("glMultiTexCoord1dvARB");
    if ( QGLglMultiTexCoord1dvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1fARB = (pfn_glMultiTexCoord1fARB)GetFunctionPointer("glMultiTexCoord1fARB");
    if ( QGLglMultiTexCoord1fARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1fvARB = (pfn_glMultiTexCoord1fvARB)GetFunctionPointer("glMultiTexCoord1fvARB");
    if ( QGLglMultiTexCoord1fvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1iARB = (pfn_glMultiTexCoord1iARB)GetFunctionPointer("glMultiTexCoord1iARB");
    if ( QGLglMultiTexCoord1iARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1ivARB = (pfn_glMultiTexCoord1ivARB)GetFunctionPointer("glMultiTexCoord1ivARB");
    if ( QGLglMultiTexCoord1ivARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1sARB = (pfn_glMultiTexCoord1sARB)GetFunctionPointer("glMultiTexCoord1sARB");
    if ( QGLglMultiTexCoord1sARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord1svARB = (pfn_glMultiTexCoord1svARB)GetFunctionPointer("glMultiTexCoord1svARB");
    if ( QGLglMultiTexCoord1svARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2dARB = (pfn_glMultiTexCoord2dARB)GetFunctionPointer("glMultiTexCoord2dARB");
    if ( QGLglMultiTexCoord2dARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2dvARB = (pfn_glMultiTexCoord2dvARB)GetFunctionPointer("glMultiTexCoord2dvARB");
    if ( QGLglMultiTexCoord2dvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2fARB = (pfn_glMultiTexCoord2fARB)GetFunctionPointer("glMultiTexCoord2fARB");
    if ( QGLglMultiTexCoord2fARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2fvARB = (pfn_glMultiTexCoord2fvARB)GetFunctionPointer("glMultiTexCoord2fvARB");
    if ( QGLglMultiTexCoord2fvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2iARB = (pfn_glMultiTexCoord2iARB)GetFunctionPointer("glMultiTexCoord2iARB");
    if ( QGLglMultiTexCoord2iARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2ivARB = (pfn_glMultiTexCoord2ivARB)GetFunctionPointer("glMultiTexCoord2ivARB");
    if ( QGLglMultiTexCoord2ivARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2sARB = (pfn_glMultiTexCoord2sARB)GetFunctionPointer("glMultiTexCoord2sARB");
    if ( QGLglMultiTexCoord2sARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord2svARB = (pfn_glMultiTexCoord2svARB)GetFunctionPointer("glMultiTexCoord2svARB");
    if ( QGLglMultiTexCoord2svARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3dARB = (pfn_glMultiTexCoord3dARB)GetFunctionPointer("glMultiTexCoord3dARB");
    if ( QGLglMultiTexCoord3dARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3dvARB = (pfn_glMultiTexCoord3dvARB)GetFunctionPointer("glMultiTexCoord3dvARB");
    if ( QGLglMultiTexCoord3dvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3fARB = (pfn_glMultiTexCoord3fARB)GetFunctionPointer("glMultiTexCoord3fARB");
    if ( QGLglMultiTexCoord3fARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3fvARB = (pfn_glMultiTexCoord3fvARB)GetFunctionPointer("glMultiTexCoord3fvARB");
    if ( QGLglMultiTexCoord3fvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3iARB = (pfn_glMultiTexCoord3iARB)GetFunctionPointer("glMultiTexCoord3iARB");
    if ( QGLglMultiTexCoord3iARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3ivARB = (pfn_glMultiTexCoord3ivARB)GetFunctionPointer("glMultiTexCoord3ivARB");
    if ( QGLglMultiTexCoord3ivARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3sARB = (pfn_glMultiTexCoord3sARB)GetFunctionPointer("glMultiTexCoord3sARB");
    if ( QGLglMultiTexCoord3sARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord3svARB = (pfn_glMultiTexCoord3svARB)GetFunctionPointer("glMultiTexCoord3svARB");
    if ( QGLglMultiTexCoord3svARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4dARB = (pfn_glMultiTexCoord4dARB)GetFunctionPointer("glMultiTexCoord4dARB");
    if ( QGLglMultiTexCoord4dARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4dvARB = (pfn_glMultiTexCoord4dvARB)GetFunctionPointer("glMultiTexCoord4dvARB");
    if ( QGLglMultiTexCoord4dvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4fARB = (pfn_glMultiTexCoord4fARB)GetFunctionPointer("glMultiTexCoord4fARB");
    if ( QGLglMultiTexCoord4fARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4fvARB = (pfn_glMultiTexCoord4fvARB)GetFunctionPointer("glMultiTexCoord4fvARB");
    if ( QGLglMultiTexCoord4fvARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4iARB = (pfn_glMultiTexCoord4iARB)GetFunctionPointer("glMultiTexCoord4iARB");
    if ( QGLglMultiTexCoord4iARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4ivARB = (pfn_glMultiTexCoord4ivARB)GetFunctionPointer("glMultiTexCoord4ivARB");
    if ( QGLglMultiTexCoord4ivARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4sARB = (pfn_glMultiTexCoord4sARB)GetFunctionPointer("glMultiTexCoord4sARB");
    if ( QGLglMultiTexCoord4sARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    QGLglMultiTexCoord4svARB = (pfn_glMultiTexCoord4svARB)GetFunctionPointer("glMultiTexCoord4svARB");
    if ( QGLglMultiTexCoord4svARB )
        ++QGLNumFunctions_GL_ARB_multitexture;

    if ( QGLNumFunctions_GL_ARB_multitexture == 34 )
        QGLExists_GL_ARB_multitexture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_multitexture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_multitexture ()
{
    QGLNumFunctions_GL_ARB_multitexture = 0;
    QGLExists_GL_ARB_multitexture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_transpose_matrix;
int QGLNumFunctions_GL_ARB_transpose_matrix;
#ifdef QGL_DEFINED_GL_ARB_transpose_matrix
pfn_glLoadTransposeMatrixfARB QGLglLoadTransposeMatrixfARB = 0;
pfn_glLoadTransposeMatrixdARB QGLglLoadTransposeMatrixdARB = 0;
pfn_glMultTransposeMatrixfARB QGLglMultTransposeMatrixfARB = 0;
pfn_glMultTransposeMatrixdARB QGLglMultTransposeMatrixdARB = 0;

static void Init_GL_ARB_transpose_matrix ()
{
    QGLNumFunctions_GL_ARB_transpose_matrix = 0;

    QGLglLoadTransposeMatrixfARB = (pfn_glLoadTransposeMatrixfARB)GetFunctionPointer("glLoadTransposeMatrixfARB");
    if ( QGLglLoadTransposeMatrixfARB )
        ++QGLNumFunctions_GL_ARB_transpose_matrix;

    QGLglLoadTransposeMatrixdARB = (pfn_glLoadTransposeMatrixdARB)GetFunctionPointer("glLoadTransposeMatrixdARB");
    if ( QGLglLoadTransposeMatrixdARB )
        ++QGLNumFunctions_GL_ARB_transpose_matrix;

    QGLglMultTransposeMatrixfARB = (pfn_glMultTransposeMatrixfARB)GetFunctionPointer("glMultTransposeMatrixfARB");
    if ( QGLglMultTransposeMatrixfARB )
        ++QGLNumFunctions_GL_ARB_transpose_matrix;

    QGLglMultTransposeMatrixdARB = (pfn_glMultTransposeMatrixdARB)GetFunctionPointer("glMultTransposeMatrixdARB");
    if ( QGLglMultTransposeMatrixdARB )
        ++QGLNumFunctions_GL_ARB_transpose_matrix;

    if ( QGLNumFunctions_GL_ARB_transpose_matrix == 4 )
        QGLExists_GL_ARB_transpose_matrix = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_transpose_matrix = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_transpose_matrix ()
{
    QGLNumFunctions_GL_ARB_transpose_matrix = 0;
    QGLExists_GL_ARB_transpose_matrix = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_multisample;
int QGLNumFunctions_GL_ARB_multisample;
#ifdef QGL_DEFINED_GL_ARB_multisample
pfn_glSampleCoverageARB QGLglSampleCoverageARB = 0;

static void Init_GL_ARB_multisample ()
{
    QGLNumFunctions_GL_ARB_multisample = 0;

    QGLglSampleCoverageARB = (pfn_glSampleCoverageARB)GetFunctionPointer("glSampleCoverageARB");
    if ( QGLglSampleCoverageARB )
        ++QGLNumFunctions_GL_ARB_multisample;

    if ( QGLNumFunctions_GL_ARB_multisample == 1 )
        QGLExists_GL_ARB_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_multisample ()
{
    QGLNumFunctions_GL_ARB_multisample = 0;
    QGLExists_GL_ARB_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_env_add;
int QGLNumFunctions_GL_ARB_texture_env_add;
#ifdef QGL_DEFINED_GL_ARB_texture_env_add
static void Init_GL_ARB_texture_env_add ()
{
    QGLNumFunctions_GL_ARB_texture_env_add = 0;
    QGLExists_GL_ARB_texture_env_add = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_env_add ()
{
    QGLNumFunctions_GL_ARB_texture_env_add = 0;
    QGLExists_GL_ARB_texture_env_add = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_cube_map;
int QGLNumFunctions_GL_ARB_texture_cube_map;
#ifdef QGL_DEFINED_GL_ARB_texture_cube_map
static void Init_GL_ARB_texture_cube_map ()
{
    QGLNumFunctions_GL_ARB_texture_cube_map = 0;
    QGLExists_GL_ARB_texture_cube_map = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_cube_map ()
{
    QGLNumFunctions_GL_ARB_texture_cube_map = 0;
    QGLExists_GL_ARB_texture_cube_map = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_compression;
int QGLNumFunctions_GL_ARB_texture_compression;
#ifdef QGL_DEFINED_GL_ARB_texture_compression
pfn_glCompressedTexImage3DARB QGLglCompressedTexImage3DARB = 0;
pfn_glCompressedTexImage2DARB QGLglCompressedTexImage2DARB = 0;
pfn_glCompressedTexImage1DARB QGLglCompressedTexImage1DARB = 0;
pfn_glCompressedTexSubImage3DARB QGLglCompressedTexSubImage3DARB = 0;
pfn_glCompressedTexSubImage2DARB QGLglCompressedTexSubImage2DARB = 0;
pfn_glCompressedTexSubImage1DARB QGLglCompressedTexSubImage1DARB = 0;
pfn_glGetCompressedTexImageARB QGLglGetCompressedTexImageARB = 0;

static void Init_GL_ARB_texture_compression ()
{
    QGLNumFunctions_GL_ARB_texture_compression = 0;

    QGLglCompressedTexImage3DARB = (pfn_glCompressedTexImage3DARB)GetFunctionPointer("glCompressedTexImage3DARB");
    if ( QGLglCompressedTexImage3DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglCompressedTexImage2DARB = (pfn_glCompressedTexImage2DARB)GetFunctionPointer("glCompressedTexImage2DARB");
    if ( QGLglCompressedTexImage2DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglCompressedTexImage1DARB = (pfn_glCompressedTexImage1DARB)GetFunctionPointer("glCompressedTexImage1DARB");
    if ( QGLglCompressedTexImage1DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglCompressedTexSubImage3DARB = (pfn_glCompressedTexSubImage3DARB)GetFunctionPointer("glCompressedTexSubImage3DARB");
    if ( QGLglCompressedTexSubImage3DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglCompressedTexSubImage2DARB = (pfn_glCompressedTexSubImage2DARB)GetFunctionPointer("glCompressedTexSubImage2DARB");
    if ( QGLglCompressedTexSubImage2DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglCompressedTexSubImage1DARB = (pfn_glCompressedTexSubImage1DARB)GetFunctionPointer("glCompressedTexSubImage1DARB");
    if ( QGLglCompressedTexSubImage1DARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    QGLglGetCompressedTexImageARB = (pfn_glGetCompressedTexImageARB)GetFunctionPointer("glGetCompressedTexImageARB");
    if ( QGLglGetCompressedTexImageARB )
        ++QGLNumFunctions_GL_ARB_texture_compression;

    if ( QGLNumFunctions_GL_ARB_texture_compression == 7 )
        QGLExists_GL_ARB_texture_compression = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_texture_compression = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_compression ()
{
    QGLNumFunctions_GL_ARB_texture_compression = 0;
    QGLExists_GL_ARB_texture_compression = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_border_clamp;
int QGLNumFunctions_GL_ARB_texture_border_clamp;
#ifdef QGL_DEFINED_GL_ARB_texture_border_clamp
static void Init_GL_ARB_texture_border_clamp ()
{
    QGLNumFunctions_GL_ARB_texture_border_clamp = 0;
    QGLExists_GL_ARB_texture_border_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_border_clamp ()
{
    QGLNumFunctions_GL_ARB_texture_border_clamp = 0;
    QGLExists_GL_ARB_texture_border_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_point_parameters;
int QGLNumFunctions_GL_ARB_point_parameters;
#ifdef QGL_DEFINED_GL_ARB_point_parameters
pfn_glPointParameterfARB QGLglPointParameterfARB = 0;
pfn_glPointParameterfvARB QGLglPointParameterfvARB = 0;

static void Init_GL_ARB_point_parameters ()
{
    QGLNumFunctions_GL_ARB_point_parameters = 0;

    QGLglPointParameterfARB = (pfn_glPointParameterfARB)GetFunctionPointer("glPointParameterfARB");
    if ( QGLglPointParameterfARB )
        ++QGLNumFunctions_GL_ARB_point_parameters;

    QGLglPointParameterfvARB = (pfn_glPointParameterfvARB)GetFunctionPointer("glPointParameterfvARB");
    if ( QGLglPointParameterfvARB )
        ++QGLNumFunctions_GL_ARB_point_parameters;

    if ( QGLNumFunctions_GL_ARB_point_parameters == 2 )
        QGLExists_GL_ARB_point_parameters = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_point_parameters = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_point_parameters ()
{
    QGLNumFunctions_GL_ARB_point_parameters = 0;
    QGLExists_GL_ARB_point_parameters = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_vertex_blend;
int QGLNumFunctions_GL_ARB_vertex_blend;
#ifdef QGL_DEFINED_GL_ARB_vertex_blend
pfn_glWeightbvARB QGLglWeightbvARB = 0;
pfn_glWeightsvARB QGLglWeightsvARB = 0;
pfn_glWeightivARB QGLglWeightivARB = 0;
pfn_glWeightfvARB QGLglWeightfvARB = 0;
pfn_glWeightdvARB QGLglWeightdvARB = 0;
pfn_glWeightubvARB QGLglWeightubvARB = 0;
pfn_glWeightusvARB QGLglWeightusvARB = 0;
pfn_glWeightuivARB QGLglWeightuivARB = 0;
pfn_glWeightPointerARB QGLglWeightPointerARB = 0;
pfn_glVertexBlendARB QGLglVertexBlendARB = 0;

static void Init_GL_ARB_vertex_blend ()
{
    QGLNumFunctions_GL_ARB_vertex_blend = 0;

    QGLglWeightbvARB = (pfn_glWeightbvARB)GetFunctionPointer("glWeightbvARB");
    if ( QGLglWeightbvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightsvARB = (pfn_glWeightsvARB)GetFunctionPointer("glWeightsvARB");
    if ( QGLglWeightsvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightivARB = (pfn_glWeightivARB)GetFunctionPointer("glWeightivARB");
    if ( QGLglWeightivARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightfvARB = (pfn_glWeightfvARB)GetFunctionPointer("glWeightfvARB");
    if ( QGLglWeightfvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightdvARB = (pfn_glWeightdvARB)GetFunctionPointer("glWeightdvARB");
    if ( QGLglWeightdvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightubvARB = (pfn_glWeightubvARB)GetFunctionPointer("glWeightubvARB");
    if ( QGLglWeightubvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightusvARB = (pfn_glWeightusvARB)GetFunctionPointer("glWeightusvARB");
    if ( QGLglWeightusvARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightuivARB = (pfn_glWeightuivARB)GetFunctionPointer("glWeightuivARB");
    if ( QGLglWeightuivARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglWeightPointerARB = (pfn_glWeightPointerARB)GetFunctionPointer("glWeightPointerARB");
    if ( QGLglWeightPointerARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    QGLglVertexBlendARB = (pfn_glVertexBlendARB)GetFunctionPointer("glVertexBlendARB");
    if ( QGLglVertexBlendARB )
        ++QGLNumFunctions_GL_ARB_vertex_blend;

    if ( QGLNumFunctions_GL_ARB_vertex_blend == 10 )
        QGLExists_GL_ARB_vertex_blend = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_vertex_blend = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_vertex_blend ()
{
    QGLNumFunctions_GL_ARB_vertex_blend = 0;
    QGLExists_GL_ARB_vertex_blend = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_matrix_palette;
int QGLNumFunctions_GL_ARB_matrix_palette;
#ifdef QGL_DEFINED_GL_ARB_matrix_palette
pfn_glCurrentPaletteMatrixARB QGLglCurrentPaletteMatrixARB = 0;
pfn_glMatrixIndexubvARB QGLglMatrixIndexubvARB = 0;
pfn_glMatrixIndexusvARB QGLglMatrixIndexusvARB = 0;
pfn_glMatrixIndexuivARB QGLglMatrixIndexuivARB = 0;
pfn_glMatrixIndexPointerARB QGLglMatrixIndexPointerARB = 0;

static void Init_GL_ARB_matrix_palette ()
{
    QGLNumFunctions_GL_ARB_matrix_palette = 0;

    QGLglCurrentPaletteMatrixARB = (pfn_glCurrentPaletteMatrixARB)GetFunctionPointer("glCurrentPaletteMatrixARB");
    if ( QGLglCurrentPaletteMatrixARB )
        ++QGLNumFunctions_GL_ARB_matrix_palette;

    QGLglMatrixIndexubvARB = (pfn_glMatrixIndexubvARB)GetFunctionPointer("glMatrixIndexubvARB");
    if ( QGLglMatrixIndexubvARB )
        ++QGLNumFunctions_GL_ARB_matrix_palette;

    QGLglMatrixIndexusvARB = (pfn_glMatrixIndexusvARB)GetFunctionPointer("glMatrixIndexusvARB");
    if ( QGLglMatrixIndexusvARB )
        ++QGLNumFunctions_GL_ARB_matrix_palette;

    QGLglMatrixIndexuivARB = (pfn_glMatrixIndexuivARB)GetFunctionPointer("glMatrixIndexuivARB");
    if ( QGLglMatrixIndexuivARB )
        ++QGLNumFunctions_GL_ARB_matrix_palette;

    QGLglMatrixIndexPointerARB = (pfn_glMatrixIndexPointerARB)GetFunctionPointer("glMatrixIndexPointerARB");
    if ( QGLglMatrixIndexPointerARB )
        ++QGLNumFunctions_GL_ARB_matrix_palette;

    if ( QGLNumFunctions_GL_ARB_matrix_palette == 5 )
        QGLExists_GL_ARB_matrix_palette = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_matrix_palette = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_matrix_palette ()
{
    QGLNumFunctions_GL_ARB_matrix_palette = 0;
    QGLExists_GL_ARB_matrix_palette = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_env_combine;
int QGLNumFunctions_GL_ARB_texture_env_combine;
#ifdef QGL_DEFINED_GL_ARB_texture_env_combine
static void Init_GL_ARB_texture_env_combine ()
{
    QGLNumFunctions_GL_ARB_texture_env_combine = 0;
    QGLExists_GL_ARB_texture_env_combine = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_env_combine ()
{
    QGLNumFunctions_GL_ARB_texture_env_combine = 0;
    QGLExists_GL_ARB_texture_env_combine = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_env_crossbar;
int QGLNumFunctions_GL_ARB_texture_env_crossbar;
#ifdef QGL_DEFINED_GL_ARB_texture_env_crossbar
static void Init_GL_ARB_texture_env_crossbar ()
{
    QGLNumFunctions_GL_ARB_texture_env_crossbar = 0;
    QGLExists_GL_ARB_texture_env_crossbar = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_env_crossbar ()
{
    QGLNumFunctions_GL_ARB_texture_env_crossbar = 0;
    QGLExists_GL_ARB_texture_env_crossbar = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_env_dot3;
int QGLNumFunctions_GL_ARB_texture_env_dot3;
#ifdef QGL_DEFINED_GL_ARB_texture_env_dot3
static void Init_GL_ARB_texture_env_dot3 ()
{
    QGLNumFunctions_GL_ARB_texture_env_dot3 = 0;
    QGLExists_GL_ARB_texture_env_dot3 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_env_dot3 ()
{
    QGLNumFunctions_GL_ARB_texture_env_dot3 = 0;
    QGLExists_GL_ARB_texture_env_dot3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_mirrored_repeat;
int QGLNumFunctions_GL_ARB_texture_mirrored_repeat;
#ifdef QGL_DEFINED_GL_ARB_texture_mirrored_repeat
static void Init_GL_ARB_texture_mirrored_repeat ()
{
    QGLNumFunctions_GL_ARB_texture_mirrored_repeat = 0;
    QGLExists_GL_ARB_texture_mirrored_repeat = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_mirrored_repeat ()
{
    QGLNumFunctions_GL_ARB_texture_mirrored_repeat = 0;
    QGLExists_GL_ARB_texture_mirrored_repeat = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_depth_texture;
int QGLNumFunctions_GL_ARB_depth_texture;
#ifdef QGL_DEFINED_GL_ARB_depth_texture
static void Init_GL_ARB_depth_texture ()
{
    QGLNumFunctions_GL_ARB_depth_texture = 0;
    QGLExists_GL_ARB_depth_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_depth_texture ()
{
    QGLNumFunctions_GL_ARB_depth_texture = 0;
    QGLExists_GL_ARB_depth_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_shadow;
int QGLNumFunctions_GL_ARB_shadow;
#ifdef QGL_DEFINED_GL_ARB_shadow
static void Init_GL_ARB_shadow ()
{
    QGLNumFunctions_GL_ARB_shadow = 0;
    QGLExists_GL_ARB_shadow = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_shadow ()
{
    QGLNumFunctions_GL_ARB_shadow = 0;
    QGLExists_GL_ARB_shadow = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_shadow_ambient;
int QGLNumFunctions_GL_ARB_shadow_ambient;
#ifdef QGL_DEFINED_GL_ARB_shadow_ambient
static void Init_GL_ARB_shadow_ambient ()
{
    QGLNumFunctions_GL_ARB_shadow_ambient = 0;
    QGLExists_GL_ARB_shadow_ambient = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_shadow_ambient ()
{
    QGLNumFunctions_GL_ARB_shadow_ambient = 0;
    QGLExists_GL_ARB_shadow_ambient = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_window_pos;
int QGLNumFunctions_GL_ARB_window_pos;
#ifdef QGL_DEFINED_GL_ARB_window_pos
pfn_glWindowPos2dARB QGLglWindowPos2dARB = 0;
pfn_glWindowPos2dvARB QGLglWindowPos2dvARB = 0;
pfn_glWindowPos2fARB QGLglWindowPos2fARB = 0;
pfn_glWindowPos2fvARB QGLglWindowPos2fvARB = 0;
pfn_glWindowPos2iARB QGLglWindowPos2iARB = 0;
pfn_glWindowPos2ivARB QGLglWindowPos2ivARB = 0;
pfn_glWindowPos2sARB QGLglWindowPos2sARB = 0;
pfn_glWindowPos2svARB QGLglWindowPos2svARB = 0;
pfn_glWindowPos3dARB QGLglWindowPos3dARB = 0;
pfn_glWindowPos3dvARB QGLglWindowPos3dvARB = 0;
pfn_glWindowPos3fARB QGLglWindowPos3fARB = 0;
pfn_glWindowPos3fvARB QGLglWindowPos3fvARB = 0;
pfn_glWindowPos3iARB QGLglWindowPos3iARB = 0;
pfn_glWindowPos3ivARB QGLglWindowPos3ivARB = 0;
pfn_glWindowPos3sARB QGLglWindowPos3sARB = 0;
pfn_glWindowPos3svARB QGLglWindowPos3svARB = 0;

static void Init_GL_ARB_window_pos ()
{
    QGLNumFunctions_GL_ARB_window_pos = 0;

    QGLglWindowPos2dARB = (pfn_glWindowPos2dARB)GetFunctionPointer("glWindowPos2dARB");
    if ( QGLglWindowPos2dARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2dvARB = (pfn_glWindowPos2dvARB)GetFunctionPointer("glWindowPos2dvARB");
    if ( QGLglWindowPos2dvARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2fARB = (pfn_glWindowPos2fARB)GetFunctionPointer("glWindowPos2fARB");
    if ( QGLglWindowPos2fARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2fvARB = (pfn_glWindowPos2fvARB)GetFunctionPointer("glWindowPos2fvARB");
    if ( QGLglWindowPos2fvARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2iARB = (pfn_glWindowPos2iARB)GetFunctionPointer("glWindowPos2iARB");
    if ( QGLglWindowPos2iARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2ivARB = (pfn_glWindowPos2ivARB)GetFunctionPointer("glWindowPos2ivARB");
    if ( QGLglWindowPos2ivARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2sARB = (pfn_glWindowPos2sARB)GetFunctionPointer("glWindowPos2sARB");
    if ( QGLglWindowPos2sARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos2svARB = (pfn_glWindowPos2svARB)GetFunctionPointer("glWindowPos2svARB");
    if ( QGLglWindowPos2svARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3dARB = (pfn_glWindowPos3dARB)GetFunctionPointer("glWindowPos3dARB");
    if ( QGLglWindowPos3dARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3dvARB = (pfn_glWindowPos3dvARB)GetFunctionPointer("glWindowPos3dvARB");
    if ( QGLglWindowPos3dvARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3fARB = (pfn_glWindowPos3fARB)GetFunctionPointer("glWindowPos3fARB");
    if ( QGLglWindowPos3fARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3fvARB = (pfn_glWindowPos3fvARB)GetFunctionPointer("glWindowPos3fvARB");
    if ( QGLglWindowPos3fvARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3iARB = (pfn_glWindowPos3iARB)GetFunctionPointer("glWindowPos3iARB");
    if ( QGLglWindowPos3iARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3ivARB = (pfn_glWindowPos3ivARB)GetFunctionPointer("glWindowPos3ivARB");
    if ( QGLglWindowPos3ivARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3sARB = (pfn_glWindowPos3sARB)GetFunctionPointer("glWindowPos3sARB");
    if ( QGLglWindowPos3sARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    QGLglWindowPos3svARB = (pfn_glWindowPos3svARB)GetFunctionPointer("glWindowPos3svARB");
    if ( QGLglWindowPos3svARB )
        ++QGLNumFunctions_GL_ARB_window_pos;

    if ( QGLNumFunctions_GL_ARB_window_pos == 16 )
        QGLExists_GL_ARB_window_pos = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_window_pos = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_window_pos ()
{
    QGLNumFunctions_GL_ARB_window_pos = 0;
    QGLExists_GL_ARB_window_pos = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_vertex_program;
int QGLNumFunctions_GL_ARB_vertex_program;
#ifdef QGL_DEFINED_GL_ARB_vertex_program
pfn_glVertexAttrib1dARB QGLglVertexAttrib1dARB = 0;
pfn_glVertexAttrib1dvARB QGLglVertexAttrib1dvARB = 0;
pfn_glVertexAttrib1fARB QGLglVertexAttrib1fARB = 0;
pfn_glVertexAttrib1fvARB QGLglVertexAttrib1fvARB = 0;
pfn_glVertexAttrib1sARB QGLglVertexAttrib1sARB = 0;
pfn_glVertexAttrib1svARB QGLglVertexAttrib1svARB = 0;
pfn_glVertexAttrib2dARB QGLglVertexAttrib2dARB = 0;
pfn_glVertexAttrib2dvARB QGLglVertexAttrib2dvARB = 0;
pfn_glVertexAttrib2fARB QGLglVertexAttrib2fARB = 0;
pfn_glVertexAttrib2fvARB QGLglVertexAttrib2fvARB = 0;
pfn_glVertexAttrib2sARB QGLglVertexAttrib2sARB = 0;
pfn_glVertexAttrib2svARB QGLglVertexAttrib2svARB = 0;
pfn_glVertexAttrib3dARB QGLglVertexAttrib3dARB = 0;
pfn_glVertexAttrib3dvARB QGLglVertexAttrib3dvARB = 0;
pfn_glVertexAttrib3fARB QGLglVertexAttrib3fARB = 0;
pfn_glVertexAttrib3fvARB QGLglVertexAttrib3fvARB = 0;
pfn_glVertexAttrib3sARB QGLglVertexAttrib3sARB = 0;
pfn_glVertexAttrib3svARB QGLglVertexAttrib3svARB = 0;
pfn_glVertexAttrib4NbvARB QGLglVertexAttrib4NbvARB = 0;
pfn_glVertexAttrib4NivARB QGLglVertexAttrib4NivARB = 0;
pfn_glVertexAttrib4NsvARB QGLglVertexAttrib4NsvARB = 0;
pfn_glVertexAttrib4NubARB QGLglVertexAttrib4NubARB = 0;
pfn_glVertexAttrib4NubvARB QGLglVertexAttrib4NubvARB = 0;
pfn_glVertexAttrib4NuivARB QGLglVertexAttrib4NuivARB = 0;
pfn_glVertexAttrib4NusvARB QGLglVertexAttrib4NusvARB = 0;
pfn_glVertexAttrib4bvARB QGLglVertexAttrib4bvARB = 0;
pfn_glVertexAttrib4dARB QGLglVertexAttrib4dARB = 0;
pfn_glVertexAttrib4dvARB QGLglVertexAttrib4dvARB = 0;
pfn_glVertexAttrib4fARB QGLglVertexAttrib4fARB = 0;
pfn_glVertexAttrib4fvARB QGLglVertexAttrib4fvARB = 0;
pfn_glVertexAttrib4ivARB QGLglVertexAttrib4ivARB = 0;
pfn_glVertexAttrib4sARB QGLglVertexAttrib4sARB = 0;
pfn_glVertexAttrib4svARB QGLglVertexAttrib4svARB = 0;
pfn_glVertexAttrib4ubvARB QGLglVertexAttrib4ubvARB = 0;
pfn_glVertexAttrib4uivARB QGLglVertexAttrib4uivARB = 0;
pfn_glVertexAttrib4usvARB QGLglVertexAttrib4usvARB = 0;
pfn_glVertexAttribPointerARB QGLglVertexAttribPointerARB = 0;
pfn_glEnableVertexAttribArrayARB QGLglEnableVertexAttribArrayARB = 0;
pfn_glDisableVertexAttribArrayARB QGLglDisableVertexAttribArrayARB = 0;
pfn_glProgramStringARB QGLglProgramStringARB = 0;
pfn_glBindProgramARB QGLglBindProgramARB = 0;
pfn_glDeleteProgramsARB QGLglDeleteProgramsARB = 0;
pfn_glGenProgramsARB QGLglGenProgramsARB = 0;
pfn_glProgramEnvParameter4dARB QGLglProgramEnvParameter4dARB = 0;
pfn_glProgramEnvParameter4dvARB QGLglProgramEnvParameter4dvARB = 0;
pfn_glProgramEnvParameter4fARB QGLglProgramEnvParameter4fARB = 0;
pfn_glProgramEnvParameter4fvARB QGLglProgramEnvParameter4fvARB = 0;
pfn_glProgramLocalParameter4dARB QGLglProgramLocalParameter4dARB = 0;
pfn_glProgramLocalParameter4dvARB QGLglProgramLocalParameter4dvARB = 0;
pfn_glProgramLocalParameter4fARB QGLglProgramLocalParameter4fARB = 0;
pfn_glProgramLocalParameter4fvARB QGLglProgramLocalParameter4fvARB = 0;
pfn_glGetProgramEnvParameterdvARB QGLglGetProgramEnvParameterdvARB = 0;
pfn_glGetProgramEnvParameterfvARB QGLglGetProgramEnvParameterfvARB = 0;
pfn_glGetProgramLocalParameterdvARB QGLglGetProgramLocalParameterdvARB = 0;
pfn_glGetProgramLocalParameterfvARB QGLglGetProgramLocalParameterfvARB = 0;
pfn_glGetProgramivARB QGLglGetProgramivARB = 0;
pfn_glGetProgramStringARB QGLglGetProgramStringARB = 0;
pfn_glGetVertexAttribdvARB QGLglGetVertexAttribdvARB = 0;
pfn_glGetVertexAttribfvARB QGLglGetVertexAttribfvARB = 0;
pfn_glGetVertexAttribivARB QGLglGetVertexAttribivARB = 0;
pfn_glGetVertexAttribPointervARB QGLglGetVertexAttribPointervARB = 0;
pfn_glIsProgramARB QGLglIsProgramARB = 0;

static void Init_GL_ARB_vertex_program ()
{
    QGLNumFunctions_GL_ARB_vertex_program = 0;

    QGLglVertexAttrib1dARB = (pfn_glVertexAttrib1dARB)GetFunctionPointer("glVertexAttrib1dARB");
    if ( QGLglVertexAttrib1dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib1dvARB = (pfn_glVertexAttrib1dvARB)GetFunctionPointer("glVertexAttrib1dvARB");
    if ( QGLglVertexAttrib1dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib1fARB = (pfn_glVertexAttrib1fARB)GetFunctionPointer("glVertexAttrib1fARB");
    if ( QGLglVertexAttrib1fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib1fvARB = (pfn_glVertexAttrib1fvARB)GetFunctionPointer("glVertexAttrib1fvARB");
    if ( QGLglVertexAttrib1fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib1sARB = (pfn_glVertexAttrib1sARB)GetFunctionPointer("glVertexAttrib1sARB");
    if ( QGLglVertexAttrib1sARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib1svARB = (pfn_glVertexAttrib1svARB)GetFunctionPointer("glVertexAttrib1svARB");
    if ( QGLglVertexAttrib1svARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2dARB = (pfn_glVertexAttrib2dARB)GetFunctionPointer("glVertexAttrib2dARB");
    if ( QGLglVertexAttrib2dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2dvARB = (pfn_glVertexAttrib2dvARB)GetFunctionPointer("glVertexAttrib2dvARB");
    if ( QGLglVertexAttrib2dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2fARB = (pfn_glVertexAttrib2fARB)GetFunctionPointer("glVertexAttrib2fARB");
    if ( QGLglVertexAttrib2fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2fvARB = (pfn_glVertexAttrib2fvARB)GetFunctionPointer("glVertexAttrib2fvARB");
    if ( QGLglVertexAttrib2fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2sARB = (pfn_glVertexAttrib2sARB)GetFunctionPointer("glVertexAttrib2sARB");
    if ( QGLglVertexAttrib2sARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib2svARB = (pfn_glVertexAttrib2svARB)GetFunctionPointer("glVertexAttrib2svARB");
    if ( QGLglVertexAttrib2svARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3dARB = (pfn_glVertexAttrib3dARB)GetFunctionPointer("glVertexAttrib3dARB");
    if ( QGLglVertexAttrib3dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3dvARB = (pfn_glVertexAttrib3dvARB)GetFunctionPointer("glVertexAttrib3dvARB");
    if ( QGLglVertexAttrib3dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3fARB = (pfn_glVertexAttrib3fARB)GetFunctionPointer("glVertexAttrib3fARB");
    if ( QGLglVertexAttrib3fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3fvARB = (pfn_glVertexAttrib3fvARB)GetFunctionPointer("glVertexAttrib3fvARB");
    if ( QGLglVertexAttrib3fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3sARB = (pfn_glVertexAttrib3sARB)GetFunctionPointer("glVertexAttrib3sARB");
    if ( QGLglVertexAttrib3sARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib3svARB = (pfn_glVertexAttrib3svARB)GetFunctionPointer("glVertexAttrib3svARB");
    if ( QGLglVertexAttrib3svARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NbvARB = (pfn_glVertexAttrib4NbvARB)GetFunctionPointer("glVertexAttrib4NbvARB");
    if ( QGLglVertexAttrib4NbvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NivARB = (pfn_glVertexAttrib4NivARB)GetFunctionPointer("glVertexAttrib4NivARB");
    if ( QGLglVertexAttrib4NivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NsvARB = (pfn_glVertexAttrib4NsvARB)GetFunctionPointer("glVertexAttrib4NsvARB");
    if ( QGLglVertexAttrib4NsvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NubARB = (pfn_glVertexAttrib4NubARB)GetFunctionPointer("glVertexAttrib4NubARB");
    if ( QGLglVertexAttrib4NubARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NubvARB = (pfn_glVertexAttrib4NubvARB)GetFunctionPointer("glVertexAttrib4NubvARB");
    if ( QGLglVertexAttrib4NubvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NuivARB = (pfn_glVertexAttrib4NuivARB)GetFunctionPointer("glVertexAttrib4NuivARB");
    if ( QGLglVertexAttrib4NuivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4NusvARB = (pfn_glVertexAttrib4NusvARB)GetFunctionPointer("glVertexAttrib4NusvARB");
    if ( QGLglVertexAttrib4NusvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4bvARB = (pfn_glVertexAttrib4bvARB)GetFunctionPointer("glVertexAttrib4bvARB");
    if ( QGLglVertexAttrib4bvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4dARB = (pfn_glVertexAttrib4dARB)GetFunctionPointer("glVertexAttrib4dARB");
    if ( QGLglVertexAttrib4dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4dvARB = (pfn_glVertexAttrib4dvARB)GetFunctionPointer("glVertexAttrib4dvARB");
    if ( QGLglVertexAttrib4dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4fARB = (pfn_glVertexAttrib4fARB)GetFunctionPointer("glVertexAttrib4fARB");
    if ( QGLglVertexAttrib4fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4fvARB = (pfn_glVertexAttrib4fvARB)GetFunctionPointer("glVertexAttrib4fvARB");
    if ( QGLglVertexAttrib4fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4ivARB = (pfn_glVertexAttrib4ivARB)GetFunctionPointer("glVertexAttrib4ivARB");
    if ( QGLglVertexAttrib4ivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4sARB = (pfn_glVertexAttrib4sARB)GetFunctionPointer("glVertexAttrib4sARB");
    if ( QGLglVertexAttrib4sARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4svARB = (pfn_glVertexAttrib4svARB)GetFunctionPointer("glVertexAttrib4svARB");
    if ( QGLglVertexAttrib4svARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4ubvARB = (pfn_glVertexAttrib4ubvARB)GetFunctionPointer("glVertexAttrib4ubvARB");
    if ( QGLglVertexAttrib4ubvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4uivARB = (pfn_glVertexAttrib4uivARB)GetFunctionPointer("glVertexAttrib4uivARB");
    if ( QGLglVertexAttrib4uivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttrib4usvARB = (pfn_glVertexAttrib4usvARB)GetFunctionPointer("glVertexAttrib4usvARB");
    if ( QGLglVertexAttrib4usvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglVertexAttribPointerARB = (pfn_glVertexAttribPointerARB)GetFunctionPointer("glVertexAttribPointerARB");
    if ( QGLglVertexAttribPointerARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglEnableVertexAttribArrayARB = (pfn_glEnableVertexAttribArrayARB)GetFunctionPointer("glEnableVertexAttribArrayARB");
    if ( QGLglEnableVertexAttribArrayARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglDisableVertexAttribArrayARB = (pfn_glDisableVertexAttribArrayARB)GetFunctionPointer("glDisableVertexAttribArrayARB");
    if ( QGLglDisableVertexAttribArrayARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramStringARB = (pfn_glProgramStringARB)GetFunctionPointer("glProgramStringARB");
    if ( QGLglProgramStringARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglBindProgramARB = (pfn_glBindProgramARB)GetFunctionPointer("glBindProgramARB");
    if ( QGLglBindProgramARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglDeleteProgramsARB = (pfn_glDeleteProgramsARB)GetFunctionPointer("glDeleteProgramsARB");
    if ( QGLglDeleteProgramsARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGenProgramsARB = (pfn_glGenProgramsARB)GetFunctionPointer("glGenProgramsARB");
    if ( QGLglGenProgramsARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramEnvParameter4dARB = (pfn_glProgramEnvParameter4dARB)GetFunctionPointer("glProgramEnvParameter4dARB");
    if ( QGLglProgramEnvParameter4dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramEnvParameter4dvARB = (pfn_glProgramEnvParameter4dvARB)GetFunctionPointer("glProgramEnvParameter4dvARB");
    if ( QGLglProgramEnvParameter4dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramEnvParameter4fARB = (pfn_glProgramEnvParameter4fARB)GetFunctionPointer("glProgramEnvParameter4fARB");
    if ( QGLglProgramEnvParameter4fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramEnvParameter4fvARB = (pfn_glProgramEnvParameter4fvARB)GetFunctionPointer("glProgramEnvParameter4fvARB");
    if ( QGLglProgramEnvParameter4fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramLocalParameter4dARB = (pfn_glProgramLocalParameter4dARB)GetFunctionPointer("glProgramLocalParameter4dARB");
    if ( QGLglProgramLocalParameter4dARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramLocalParameter4dvARB = (pfn_glProgramLocalParameter4dvARB)GetFunctionPointer("glProgramLocalParameter4dvARB");
    if ( QGLglProgramLocalParameter4dvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramLocalParameter4fARB = (pfn_glProgramLocalParameter4fARB)GetFunctionPointer("glProgramLocalParameter4fARB");
    if ( QGLglProgramLocalParameter4fARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglProgramLocalParameter4fvARB = (pfn_glProgramLocalParameter4fvARB)GetFunctionPointer("glProgramLocalParameter4fvARB");
    if ( QGLglProgramLocalParameter4fvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramEnvParameterdvARB = (pfn_glGetProgramEnvParameterdvARB)GetFunctionPointer("glGetProgramEnvParameterdvARB");
    if ( QGLglGetProgramEnvParameterdvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramEnvParameterfvARB = (pfn_glGetProgramEnvParameterfvARB)GetFunctionPointer("glGetProgramEnvParameterfvARB");
    if ( QGLglGetProgramEnvParameterfvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramLocalParameterdvARB = (pfn_glGetProgramLocalParameterdvARB)GetFunctionPointer("glGetProgramLocalParameterdvARB");
    if ( QGLglGetProgramLocalParameterdvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramLocalParameterfvARB = (pfn_glGetProgramLocalParameterfvARB)GetFunctionPointer("glGetProgramLocalParameterfvARB");
    if ( QGLglGetProgramLocalParameterfvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramivARB = (pfn_glGetProgramivARB)GetFunctionPointer("glGetProgramivARB");
    if ( QGLglGetProgramivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetProgramStringARB = (pfn_glGetProgramStringARB)GetFunctionPointer("glGetProgramStringARB");
    if ( QGLglGetProgramStringARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetVertexAttribdvARB = (pfn_glGetVertexAttribdvARB)GetFunctionPointer("glGetVertexAttribdvARB");
    if ( QGLglGetVertexAttribdvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetVertexAttribfvARB = (pfn_glGetVertexAttribfvARB)GetFunctionPointer("glGetVertexAttribfvARB");
    if ( QGLglGetVertexAttribfvARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetVertexAttribivARB = (pfn_glGetVertexAttribivARB)GetFunctionPointer("glGetVertexAttribivARB");
    if ( QGLglGetVertexAttribivARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglGetVertexAttribPointervARB = (pfn_glGetVertexAttribPointervARB)GetFunctionPointer("glGetVertexAttribPointervARB");
    if ( QGLglGetVertexAttribPointervARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    QGLglIsProgramARB = (pfn_glIsProgramARB)GetFunctionPointer("glIsProgramARB");
    if ( QGLglIsProgramARB )
        ++QGLNumFunctions_GL_ARB_vertex_program;

    if ( QGLNumFunctions_GL_ARB_vertex_program == 62 )
        QGLExists_GL_ARB_vertex_program = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_vertex_program = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_vertex_program ()
{
    QGLNumFunctions_GL_ARB_vertex_program = 0;
    QGLExists_GL_ARB_vertex_program = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_fragment_program;
int QGLNumFunctions_GL_ARB_fragment_program;
#ifdef QGL_DEFINED_GL_ARB_fragment_program
static void Init_GL_ARB_fragment_program ()
{
    QGLNumFunctions_GL_ARB_fragment_program = 0;
    QGLExists_GL_ARB_fragment_program = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_fragment_program ()
{
    QGLNumFunctions_GL_ARB_fragment_program = 0;
    QGLExists_GL_ARB_fragment_program = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_vertex_buffer_object;
int QGLNumFunctions_GL_ARB_vertex_buffer_object;
#ifdef QGL_DEFINED_GL_ARB_vertex_buffer_object
pfn_glBindBufferARB QGLglBindBufferARB = 0;
pfn_glDeleteBuffersARB QGLglDeleteBuffersARB = 0;
pfn_glGenBuffersARB QGLglGenBuffersARB = 0;
pfn_glIsBufferARB QGLglIsBufferARB = 0;
pfn_glBufferDataARB QGLglBufferDataARB = 0;
pfn_glBufferSubDataARB QGLglBufferSubDataARB = 0;
pfn_glGetBufferSubDataARB QGLglGetBufferSubDataARB = 0;
pfn_glMapBufferARB QGLglMapBufferARB = 0;
pfn_glUnmapBufferARB QGLglUnmapBufferARB = 0;
pfn_glGetBufferParameterivARB QGLglGetBufferParameterivARB = 0;
pfn_glGetBufferPointervARB QGLglGetBufferPointervARB = 0;

static void Init_GL_ARB_vertex_buffer_object ()
{
    QGLNumFunctions_GL_ARB_vertex_buffer_object = 0;

    QGLglBindBufferARB = (pfn_glBindBufferARB)GetFunctionPointer("glBindBufferARB");
    if ( QGLglBindBufferARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglDeleteBuffersARB = (pfn_glDeleteBuffersARB)GetFunctionPointer("glDeleteBuffersARB");
    if ( QGLglDeleteBuffersARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglGenBuffersARB = (pfn_glGenBuffersARB)GetFunctionPointer("glGenBuffersARB");
    if ( QGLglGenBuffersARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglIsBufferARB = (pfn_glIsBufferARB)GetFunctionPointer("glIsBufferARB");
    if ( QGLglIsBufferARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglBufferDataARB = (pfn_glBufferDataARB)GetFunctionPointer("glBufferDataARB");
    if ( QGLglBufferDataARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglBufferSubDataARB = (pfn_glBufferSubDataARB)GetFunctionPointer("glBufferSubDataARB");
    if ( QGLglBufferSubDataARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglGetBufferSubDataARB = (pfn_glGetBufferSubDataARB)GetFunctionPointer("glGetBufferSubDataARB");
    if ( QGLglGetBufferSubDataARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglMapBufferARB = (pfn_glMapBufferARB)GetFunctionPointer("glMapBufferARB");
    if ( QGLglMapBufferARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglUnmapBufferARB = (pfn_glUnmapBufferARB)GetFunctionPointer("glUnmapBufferARB");
    if ( QGLglUnmapBufferARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglGetBufferParameterivARB = (pfn_glGetBufferParameterivARB)GetFunctionPointer("glGetBufferParameterivARB");
    if ( QGLglGetBufferParameterivARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    QGLglGetBufferPointervARB = (pfn_glGetBufferPointervARB)GetFunctionPointer("glGetBufferPointervARB");
    if ( QGLglGetBufferPointervARB )
        ++QGLNumFunctions_GL_ARB_vertex_buffer_object;

    if ( QGLNumFunctions_GL_ARB_vertex_buffer_object == 11 )
        QGLExists_GL_ARB_vertex_buffer_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_vertex_buffer_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_vertex_buffer_object ()
{
    QGLNumFunctions_GL_ARB_vertex_buffer_object = 0;
    QGLExists_GL_ARB_vertex_buffer_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_occlusion_query;
int QGLNumFunctions_GL_ARB_occlusion_query;
#ifdef QGL_DEFINED_GL_ARB_occlusion_query
pfn_glGenQueriesARB QGLglGenQueriesARB = 0;
pfn_glDeleteQueriesARB QGLglDeleteQueriesARB = 0;
pfn_glIsQueryARB QGLglIsQueryARB = 0;
pfn_glBeginQueryARB QGLglBeginQueryARB = 0;
pfn_glEndQueryARB QGLglEndQueryARB = 0;
pfn_glGetQueryivARB QGLglGetQueryivARB = 0;
pfn_glGetQueryObjectivARB QGLglGetQueryObjectivARB = 0;
pfn_glGetQueryObjectuivARB QGLglGetQueryObjectuivARB = 0;

static void Init_GL_ARB_occlusion_query ()
{
    QGLNumFunctions_GL_ARB_occlusion_query = 0;

    QGLglGenQueriesARB = (pfn_glGenQueriesARB)GetFunctionPointer("glGenQueriesARB");
    if ( QGLglGenQueriesARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglDeleteQueriesARB = (pfn_glDeleteQueriesARB)GetFunctionPointer("glDeleteQueriesARB");
    if ( QGLglDeleteQueriesARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglIsQueryARB = (pfn_glIsQueryARB)GetFunctionPointer("glIsQueryARB");
    if ( QGLglIsQueryARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglBeginQueryARB = (pfn_glBeginQueryARB)GetFunctionPointer("glBeginQueryARB");
    if ( QGLglBeginQueryARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglEndQueryARB = (pfn_glEndQueryARB)GetFunctionPointer("glEndQueryARB");
    if ( QGLglEndQueryARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglGetQueryivARB = (pfn_glGetQueryivARB)GetFunctionPointer("glGetQueryivARB");
    if ( QGLglGetQueryivARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglGetQueryObjectivARB = (pfn_glGetQueryObjectivARB)GetFunctionPointer("glGetQueryObjectivARB");
    if ( QGLglGetQueryObjectivARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    QGLglGetQueryObjectuivARB = (pfn_glGetQueryObjectuivARB)GetFunctionPointer("glGetQueryObjectuivARB");
    if ( QGLglGetQueryObjectuivARB )
        ++QGLNumFunctions_GL_ARB_occlusion_query;

    if ( QGLNumFunctions_GL_ARB_occlusion_query == 8 )
        QGLExists_GL_ARB_occlusion_query = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_occlusion_query = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_occlusion_query ()
{
    QGLNumFunctions_GL_ARB_occlusion_query = 0;
    QGLExists_GL_ARB_occlusion_query = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_shader_objects;
int QGLNumFunctions_GL_ARB_shader_objects;
#ifdef QGL_DEFINED_GL_ARB_shader_objects
pfn_glDeleteObjectARB QGLglDeleteObjectARB = 0;
pfn_glGetHandleARB QGLglGetHandleARB = 0;
pfn_glDetachObjectARB QGLglDetachObjectARB = 0;
pfn_glCreateShaderObjectARB QGLglCreateShaderObjectARB = 0;
pfn_glShaderSourceARB QGLglShaderSourceARB = 0;
pfn_glCompileShaderARB QGLglCompileShaderARB = 0;
pfn_glCreateProgramObjectARB QGLglCreateProgramObjectARB = 0;
pfn_glAttachObjectARB QGLglAttachObjectARB = 0;
pfn_glLinkProgramARB QGLglLinkProgramARB = 0;
pfn_glUseProgramObjectARB QGLglUseProgramObjectARB = 0;
pfn_glValidateProgramARB QGLglValidateProgramARB = 0;
pfn_glUniform1fARB QGLglUniform1fARB = 0;
pfn_glUniform2fARB QGLglUniform2fARB = 0;
pfn_glUniform3fARB QGLglUniform3fARB = 0;
pfn_glUniform4fARB QGLglUniform4fARB = 0;
pfn_glUniform1iARB QGLglUniform1iARB = 0;
pfn_glUniform2iARB QGLglUniform2iARB = 0;
pfn_glUniform3iARB QGLglUniform3iARB = 0;
pfn_glUniform4iARB QGLglUniform4iARB = 0;
pfn_glUniform1fvARB QGLglUniform1fvARB = 0;
pfn_glUniform2fvARB QGLglUniform2fvARB = 0;
pfn_glUniform3fvARB QGLglUniform3fvARB = 0;
pfn_glUniform4fvARB QGLglUniform4fvARB = 0;
pfn_glUniform1ivARB QGLglUniform1ivARB = 0;
pfn_glUniform2ivARB QGLglUniform2ivARB = 0;
pfn_glUniform3ivARB QGLglUniform3ivARB = 0;
pfn_glUniform4ivARB QGLglUniform4ivARB = 0;
pfn_glUniformMatrix2fvARB QGLglUniformMatrix2fvARB = 0;
pfn_glUniformMatrix3fvARB QGLglUniformMatrix3fvARB = 0;
pfn_glUniformMatrix4fvARB QGLglUniformMatrix4fvARB = 0;
pfn_glGetObjectParameterfvARB QGLglGetObjectParameterfvARB = 0;
pfn_glGetObjectParameterivARB QGLglGetObjectParameterivARB = 0;
pfn_glGetInfoLogARB QGLglGetInfoLogARB = 0;
pfn_glGetAttachedObjectsARB QGLglGetAttachedObjectsARB = 0;
pfn_glGetUniformLocationARB QGLglGetUniformLocationARB = 0;
pfn_glGetActiveUniformARB QGLglGetActiveUniformARB = 0;
pfn_glGetUniformfvARB QGLglGetUniformfvARB = 0;
pfn_glGetUniformivARB QGLglGetUniformivARB = 0;
pfn_glGetShaderSourceARB QGLglGetShaderSourceARB = 0;

static void Init_GL_ARB_shader_objects ()
{
    QGLNumFunctions_GL_ARB_shader_objects = 0;

    QGLglDeleteObjectARB = (pfn_glDeleteObjectARB)GetFunctionPointer("glDeleteObjectARB");
    if ( QGLglDeleteObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetHandleARB = (pfn_glGetHandleARB)GetFunctionPointer("glGetHandleARB");
    if ( QGLglGetHandleARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglDetachObjectARB = (pfn_glDetachObjectARB)GetFunctionPointer("glDetachObjectARB");
    if ( QGLglDetachObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglCreateShaderObjectARB = (pfn_glCreateShaderObjectARB)GetFunctionPointer("glCreateShaderObjectARB");
    if ( QGLglCreateShaderObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglShaderSourceARB = (pfn_glShaderSourceARB)GetFunctionPointer("glShaderSourceARB");
    if ( QGLglShaderSourceARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglCompileShaderARB = (pfn_glCompileShaderARB)GetFunctionPointer("glCompileShaderARB");
    if ( QGLglCompileShaderARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglCreateProgramObjectARB = (pfn_glCreateProgramObjectARB)GetFunctionPointer("glCreateProgramObjectARB");
    if ( QGLglCreateProgramObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglAttachObjectARB = (pfn_glAttachObjectARB)GetFunctionPointer("glAttachObjectARB");
    if ( QGLglAttachObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglLinkProgramARB = (pfn_glLinkProgramARB)GetFunctionPointer("glLinkProgramARB");
    if ( QGLglLinkProgramARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUseProgramObjectARB = (pfn_glUseProgramObjectARB)GetFunctionPointer("glUseProgramObjectARB");
    if ( QGLglUseProgramObjectARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglValidateProgramARB = (pfn_glValidateProgramARB)GetFunctionPointer("glValidateProgramARB");
    if ( QGLglValidateProgramARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform1fARB = (pfn_glUniform1fARB)GetFunctionPointer("glUniform1fARB");
    if ( QGLglUniform1fARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform2fARB = (pfn_glUniform2fARB)GetFunctionPointer("glUniform2fARB");
    if ( QGLglUniform2fARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform3fARB = (pfn_glUniform3fARB)GetFunctionPointer("glUniform3fARB");
    if ( QGLglUniform3fARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform4fARB = (pfn_glUniform4fARB)GetFunctionPointer("glUniform4fARB");
    if ( QGLglUniform4fARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform1iARB = (pfn_glUniform1iARB)GetFunctionPointer("glUniform1iARB");
    if ( QGLglUniform1iARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform2iARB = (pfn_glUniform2iARB)GetFunctionPointer("glUniform2iARB");
    if ( QGLglUniform2iARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform3iARB = (pfn_glUniform3iARB)GetFunctionPointer("glUniform3iARB");
    if ( QGLglUniform3iARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform4iARB = (pfn_glUniform4iARB)GetFunctionPointer("glUniform4iARB");
    if ( QGLglUniform4iARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform1fvARB = (pfn_glUniform1fvARB)GetFunctionPointer("glUniform1fvARB");
    if ( QGLglUniform1fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform2fvARB = (pfn_glUniform2fvARB)GetFunctionPointer("glUniform2fvARB");
    if ( QGLglUniform2fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform3fvARB = (pfn_glUniform3fvARB)GetFunctionPointer("glUniform3fvARB");
    if ( QGLglUniform3fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform4fvARB = (pfn_glUniform4fvARB)GetFunctionPointer("glUniform4fvARB");
    if ( QGLglUniform4fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform1ivARB = (pfn_glUniform1ivARB)GetFunctionPointer("glUniform1ivARB");
    if ( QGLglUniform1ivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform2ivARB = (pfn_glUniform2ivARB)GetFunctionPointer("glUniform2ivARB");
    if ( QGLglUniform2ivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform3ivARB = (pfn_glUniform3ivARB)GetFunctionPointer("glUniform3ivARB");
    if ( QGLglUniform3ivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniform4ivARB = (pfn_glUniform4ivARB)GetFunctionPointer("glUniform4ivARB");
    if ( QGLglUniform4ivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniformMatrix2fvARB = (pfn_glUniformMatrix2fvARB)GetFunctionPointer("glUniformMatrix2fvARB");
    if ( QGLglUniformMatrix2fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniformMatrix3fvARB = (pfn_glUniformMatrix3fvARB)GetFunctionPointer("glUniformMatrix3fvARB");
    if ( QGLglUniformMatrix3fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglUniformMatrix4fvARB = (pfn_glUniformMatrix4fvARB)GetFunctionPointer("glUniformMatrix4fvARB");
    if ( QGLglUniformMatrix4fvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetObjectParameterfvARB = (pfn_glGetObjectParameterfvARB)GetFunctionPointer("glGetObjectParameterfvARB");
    if ( QGLglGetObjectParameterfvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetObjectParameterivARB = (pfn_glGetObjectParameterivARB)GetFunctionPointer("glGetObjectParameterivARB");
    if ( QGLglGetObjectParameterivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetInfoLogARB = (pfn_glGetInfoLogARB)GetFunctionPointer("glGetInfoLogARB");
    if ( QGLglGetInfoLogARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetAttachedObjectsARB = (pfn_glGetAttachedObjectsARB)GetFunctionPointer("glGetAttachedObjectsARB");
    if ( QGLglGetAttachedObjectsARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetUniformLocationARB = (pfn_glGetUniformLocationARB)GetFunctionPointer("glGetUniformLocationARB");
    if ( QGLglGetUniformLocationARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetActiveUniformARB = (pfn_glGetActiveUniformARB)GetFunctionPointer("glGetActiveUniformARB");
    if ( QGLglGetActiveUniformARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetUniformfvARB = (pfn_glGetUniformfvARB)GetFunctionPointer("glGetUniformfvARB");
    if ( QGLglGetUniformfvARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetUniformivARB = (pfn_glGetUniformivARB)GetFunctionPointer("glGetUniformivARB");
    if ( QGLglGetUniformivARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    QGLglGetShaderSourceARB = (pfn_glGetShaderSourceARB)GetFunctionPointer("glGetShaderSourceARB");
    if ( QGLglGetShaderSourceARB )
        ++QGLNumFunctions_GL_ARB_shader_objects;

    if ( QGLNumFunctions_GL_ARB_shader_objects == 39 )
        QGLExists_GL_ARB_shader_objects = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_shader_objects = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_shader_objects ()
{
    QGLNumFunctions_GL_ARB_shader_objects = 0;
    QGLExists_GL_ARB_shader_objects = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_vertex_shader;
int QGLNumFunctions_GL_ARB_vertex_shader;
#ifdef QGL_DEFINED_GL_ARB_vertex_shader
pfn_glBindAttribLocationARB QGLglBindAttribLocationARB = 0;
pfn_glGetActiveAttribARB QGLglGetActiveAttribARB = 0;
pfn_glGetAttribLocationARB QGLglGetAttribLocationARB = 0;

static void Init_GL_ARB_vertex_shader ()
{
    QGLNumFunctions_GL_ARB_vertex_shader = 0;

    QGLglBindAttribLocationARB = (pfn_glBindAttribLocationARB)GetFunctionPointer("glBindAttribLocationARB");
    if ( QGLglBindAttribLocationARB )
        ++QGLNumFunctions_GL_ARB_vertex_shader;

    QGLglGetActiveAttribARB = (pfn_glGetActiveAttribARB)GetFunctionPointer("glGetActiveAttribARB");
    if ( QGLglGetActiveAttribARB )
        ++QGLNumFunctions_GL_ARB_vertex_shader;

    QGLglGetAttribLocationARB = (pfn_glGetAttribLocationARB)GetFunctionPointer("glGetAttribLocationARB");
    if ( QGLglGetAttribLocationARB )
        ++QGLNumFunctions_GL_ARB_vertex_shader;

    if ( QGLNumFunctions_GL_ARB_vertex_shader == 3 )
        QGLExists_GL_ARB_vertex_shader = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_vertex_shader = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_vertex_shader ()
{
    QGLNumFunctions_GL_ARB_vertex_shader = 0;
    QGLExists_GL_ARB_vertex_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_fragment_shader;
int QGLNumFunctions_GL_ARB_fragment_shader;
#ifdef QGL_DEFINED_GL_ARB_fragment_shader
static void Init_GL_ARB_fragment_shader ()
{
    QGLNumFunctions_GL_ARB_fragment_shader = 0;
    QGLExists_GL_ARB_fragment_shader = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_fragment_shader ()
{
    QGLNumFunctions_GL_ARB_fragment_shader = 0;
    QGLExists_GL_ARB_fragment_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_shading_language_100;
int QGLNumFunctions_GL_ARB_shading_language_100;
#ifdef QGL_DEFINED_GL_ARB_shading_language_100
static void Init_GL_ARB_shading_language_100 ()
{
    QGLNumFunctions_GL_ARB_shading_language_100 = 0;
    QGLExists_GL_ARB_shading_language_100 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_shading_language_100 ()
{
    QGLNumFunctions_GL_ARB_shading_language_100 = 0;
    QGLExists_GL_ARB_shading_language_100 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_non_power_of_two;
int QGLNumFunctions_GL_ARB_texture_non_power_of_two;
#ifdef QGL_DEFINED_GL_ARB_texture_non_power_of_two
static void Init_GL_ARB_texture_non_power_of_two ()
{
    QGLNumFunctions_GL_ARB_texture_non_power_of_two = 0;
    QGLExists_GL_ARB_texture_non_power_of_two = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_non_power_of_two ()
{
    QGLNumFunctions_GL_ARB_texture_non_power_of_two = 0;
    QGLExists_GL_ARB_texture_non_power_of_two = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_point_sprite;
int QGLNumFunctions_GL_ARB_point_sprite;
#ifdef QGL_DEFINED_GL_ARB_point_sprite
static void Init_GL_ARB_point_sprite ()
{
    QGLNumFunctions_GL_ARB_point_sprite = 0;
    QGLExists_GL_ARB_point_sprite = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_point_sprite ()
{
    QGLNumFunctions_GL_ARB_point_sprite = 0;
    QGLExists_GL_ARB_point_sprite = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_fragment_program_shadow;
int QGLNumFunctions_GL_ARB_fragment_program_shadow;
#ifdef QGL_DEFINED_GL_ARB_fragment_program_shadow
static void Init_GL_ARB_fragment_program_shadow ()
{
    QGLNumFunctions_GL_ARB_fragment_program_shadow = 0;
    QGLExists_GL_ARB_fragment_program_shadow = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_fragment_program_shadow ()
{
    QGLNumFunctions_GL_ARB_fragment_program_shadow = 0;
    QGLExists_GL_ARB_fragment_program_shadow = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_draw_buffers;
int QGLNumFunctions_GL_ARB_draw_buffers;
#ifdef QGL_DEFINED_GL_ARB_draw_buffers
pfn_glDrawBuffersARB QGLglDrawBuffersARB = 0;

static void Init_GL_ARB_draw_buffers ()
{
    QGLNumFunctions_GL_ARB_draw_buffers = 0;

    QGLglDrawBuffersARB = (pfn_glDrawBuffersARB)GetFunctionPointer("glDrawBuffersARB");
    if ( QGLglDrawBuffersARB )
        ++QGLNumFunctions_GL_ARB_draw_buffers;

    if ( QGLNumFunctions_GL_ARB_draw_buffers == 1 )
        QGLExists_GL_ARB_draw_buffers = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_draw_buffers = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_draw_buffers ()
{
    QGLNumFunctions_GL_ARB_draw_buffers = 0;
    QGLExists_GL_ARB_draw_buffers = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_rectangle;
int QGLNumFunctions_GL_ARB_texture_rectangle;
#ifdef QGL_DEFINED_GL_ARB_texture_rectangle
static void Init_GL_ARB_texture_rectangle ()
{
    QGLNumFunctions_GL_ARB_texture_rectangle = 0;
    QGLExists_GL_ARB_texture_rectangle = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_rectangle ()
{
    QGLNumFunctions_GL_ARB_texture_rectangle = 0;
    QGLExists_GL_ARB_texture_rectangle = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_color_buffer_float;
int QGLNumFunctions_GL_ARB_color_buffer_float;
#ifdef QGL_DEFINED_GL_ARB_color_buffer_float
pfn_glClampColorARB QGLglClampColorARB = 0;

static void Init_GL_ARB_color_buffer_float ()
{
    QGLNumFunctions_GL_ARB_color_buffer_float = 0;

    QGLglClampColorARB = (pfn_glClampColorARB)GetFunctionPointer("glClampColorARB");
    if ( QGLglClampColorARB )
        ++QGLNumFunctions_GL_ARB_color_buffer_float;

    if ( QGLNumFunctions_GL_ARB_color_buffer_float == 1 )
        QGLExists_GL_ARB_color_buffer_float = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ARB_color_buffer_float = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ARB_color_buffer_float ()
{
    QGLNumFunctions_GL_ARB_color_buffer_float = 0;
    QGLExists_GL_ARB_color_buffer_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_half_float_pixel;
int QGLNumFunctions_GL_ARB_half_float_pixel;
#ifdef QGL_DEFINED_GL_ARB_half_float_pixel
static void Init_GL_ARB_half_float_pixel ()
{
    QGLNumFunctions_GL_ARB_half_float_pixel = 0;
    QGLExists_GL_ARB_half_float_pixel = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_half_float_pixel ()
{
    QGLNumFunctions_GL_ARB_half_float_pixel = 0;
    QGLExists_GL_ARB_half_float_pixel = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_texture_float;
int QGLNumFunctions_GL_ARB_texture_float;
#ifdef QGL_DEFINED_GL_ARB_texture_float
static void Init_GL_ARB_texture_float ()
{
    QGLNumFunctions_GL_ARB_texture_float = 0;
    QGLExists_GL_ARB_texture_float = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_texture_float ()
{
    QGLNumFunctions_GL_ARB_texture_float = 0;
    QGLExists_GL_ARB_texture_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ARB_pixel_buffer_object;
int QGLNumFunctions_GL_ARB_pixel_buffer_object;
#ifdef QGL_DEFINED_GL_ARB_pixel_buffer_object
static void Init_GL_ARB_pixel_buffer_object ()
{
    QGLNumFunctions_GL_ARB_pixel_buffer_object = 0;
    QGLExists_GL_ARB_pixel_buffer_object = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ARB_pixel_buffer_object ()
{
    QGLNumFunctions_GL_ARB_pixel_buffer_object = 0;
    QGLExists_GL_ARB_pixel_buffer_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_abgr;
int QGLNumFunctions_GL_EXT_abgr;
#ifdef QGL_DEFINED_GL_EXT_abgr
static void Init_GL_EXT_abgr ()
{
    QGLNumFunctions_GL_EXT_abgr = 0;
    QGLExists_GL_EXT_abgr = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_abgr ()
{
    QGLNumFunctions_GL_EXT_abgr = 0;
    QGLExists_GL_EXT_abgr = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_color;
int QGLNumFunctions_GL_EXT_blend_color;
#ifdef QGL_DEFINED_GL_EXT_blend_color
pfn_glBlendColorEXT QGLglBlendColorEXT = 0;

static void Init_GL_EXT_blend_color ()
{
    QGLNumFunctions_GL_EXT_blend_color = 0;

    QGLglBlendColorEXT = (pfn_glBlendColorEXT)GetFunctionPointer("glBlendColorEXT");
    if ( QGLglBlendColorEXT )
        ++QGLNumFunctions_GL_EXT_blend_color;

    if ( QGLNumFunctions_GL_EXT_blend_color == 1 )
        QGLExists_GL_EXT_blend_color = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_blend_color = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_color ()
{
    QGLNumFunctions_GL_EXT_blend_color = 0;
    QGLExists_GL_EXT_blend_color = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_polygon_offset;
int QGLNumFunctions_GL_EXT_polygon_offset;
#ifdef QGL_DEFINED_GL_EXT_polygon_offset
pfn_glPolygonOffsetEXT QGLglPolygonOffsetEXT = 0;

static void Init_GL_EXT_polygon_offset ()
{
    QGLNumFunctions_GL_EXT_polygon_offset = 0;

    QGLglPolygonOffsetEXT = (pfn_glPolygonOffsetEXT)GetFunctionPointer("glPolygonOffsetEXT");
    if ( QGLglPolygonOffsetEXT )
        ++QGLNumFunctions_GL_EXT_polygon_offset;

    if ( QGLNumFunctions_GL_EXT_polygon_offset == 1 )
        QGLExists_GL_EXT_polygon_offset = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_polygon_offset = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_polygon_offset ()
{
    QGLNumFunctions_GL_EXT_polygon_offset = 0;
    QGLExists_GL_EXT_polygon_offset = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture;
int QGLNumFunctions_GL_EXT_texture;
#ifdef QGL_DEFINED_GL_EXT_texture
static void Init_GL_EXT_texture ()
{
    QGLNumFunctions_GL_EXT_texture = 0;
    QGLExists_GL_EXT_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture ()
{
    QGLNumFunctions_GL_EXT_texture = 0;
    QGLExists_GL_EXT_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture3D;
int QGLNumFunctions_GL_EXT_texture3D;
#ifdef QGL_DEFINED_GL_EXT_texture3D
pfn_glTexImage3DEXT QGLglTexImage3DEXT = 0;
pfn_glTexSubImage3DEXT QGLglTexSubImage3DEXT = 0;

static void Init_GL_EXT_texture3D ()
{
    QGLNumFunctions_GL_EXT_texture3D = 0;

    QGLglTexImage3DEXT = (pfn_glTexImage3DEXT)GetFunctionPointer("glTexImage3DEXT");
    if ( QGLglTexImage3DEXT )
        ++QGLNumFunctions_GL_EXT_texture3D;

    QGLglTexSubImage3DEXT = (pfn_glTexSubImage3DEXT)GetFunctionPointer("glTexSubImage3DEXT");
    if ( QGLglTexSubImage3DEXT )
        ++QGLNumFunctions_GL_EXT_texture3D;

    if ( QGLNumFunctions_GL_EXT_texture3D == 2 )
        QGLExists_GL_EXT_texture3D = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_texture3D = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_texture3D ()
{
    QGLNumFunctions_GL_EXT_texture3D = 0;
    QGLExists_GL_EXT_texture3D = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture_filter4;
int QGLNumFunctions_GL_SGIS_texture_filter4;
#ifdef QGL_DEFINED_GL_SGIS_texture_filter4
pfn_glGetTexFilterFuncSGIS QGLglGetTexFilterFuncSGIS = 0;
pfn_glTexFilterFuncSGIS QGLglTexFilterFuncSGIS = 0;

static void Init_GL_SGIS_texture_filter4 ()
{
    QGLNumFunctions_GL_SGIS_texture_filter4 = 0;

    QGLglGetTexFilterFuncSGIS = (pfn_glGetTexFilterFuncSGIS)GetFunctionPointer("glGetTexFilterFuncSGIS");
    if ( QGLglGetTexFilterFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_texture_filter4;

    QGLglTexFilterFuncSGIS = (pfn_glTexFilterFuncSGIS)GetFunctionPointer("glTexFilterFuncSGIS");
    if ( QGLglTexFilterFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_texture_filter4;

    if ( QGLNumFunctions_GL_SGIS_texture_filter4 == 2 )
        QGLExists_GL_SGIS_texture_filter4 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_texture_filter4 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture_filter4 ()
{
    QGLNumFunctions_GL_SGIS_texture_filter4 = 0;
    QGLExists_GL_SGIS_texture_filter4 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_subtexture;
int QGLNumFunctions_GL_EXT_subtexture;
#ifdef QGL_DEFINED_GL_EXT_subtexture
pfn_glTexSubImage1DEXT QGLglTexSubImage1DEXT = 0;
pfn_glTexSubImage2DEXT QGLglTexSubImage2DEXT = 0;

static void Init_GL_EXT_subtexture ()
{
    QGLNumFunctions_GL_EXT_subtexture = 0;

    QGLglTexSubImage1DEXT = (pfn_glTexSubImage1DEXT)GetFunctionPointer("glTexSubImage1DEXT");
    if ( QGLglTexSubImage1DEXT )
        ++QGLNumFunctions_GL_EXT_subtexture;

    QGLglTexSubImage2DEXT = (pfn_glTexSubImage2DEXT)GetFunctionPointer("glTexSubImage2DEXT");
    if ( QGLglTexSubImage2DEXT )
        ++QGLNumFunctions_GL_EXT_subtexture;

    if ( QGLNumFunctions_GL_EXT_subtexture == 2 )
        QGLExists_GL_EXT_subtexture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_subtexture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_subtexture ()
{
    QGLNumFunctions_GL_EXT_subtexture = 0;
    QGLExists_GL_EXT_subtexture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_copy_texture;
int QGLNumFunctions_GL_EXT_copy_texture;
#ifdef QGL_DEFINED_GL_EXT_copy_texture
pfn_glCopyTexImage1DEXT QGLglCopyTexImage1DEXT = 0;
pfn_glCopyTexImage2DEXT QGLglCopyTexImage2DEXT = 0;
pfn_glCopyTexSubImage1DEXT QGLglCopyTexSubImage1DEXT = 0;
pfn_glCopyTexSubImage2DEXT QGLglCopyTexSubImage2DEXT = 0;
pfn_glCopyTexSubImage3DEXT QGLglCopyTexSubImage3DEXT = 0;

static void Init_GL_EXT_copy_texture ()
{
    QGLNumFunctions_GL_EXT_copy_texture = 0;

    QGLglCopyTexImage1DEXT = (pfn_glCopyTexImage1DEXT)GetFunctionPointer("glCopyTexImage1DEXT");
    if ( QGLglCopyTexImage1DEXT )
        ++QGLNumFunctions_GL_EXT_copy_texture;

    QGLglCopyTexImage2DEXT = (pfn_glCopyTexImage2DEXT)GetFunctionPointer("glCopyTexImage2DEXT");
    if ( QGLglCopyTexImage2DEXT )
        ++QGLNumFunctions_GL_EXT_copy_texture;

    QGLglCopyTexSubImage1DEXT = (pfn_glCopyTexSubImage1DEXT)GetFunctionPointer("glCopyTexSubImage1DEXT");
    if ( QGLglCopyTexSubImage1DEXT )
        ++QGLNumFunctions_GL_EXT_copy_texture;

    QGLglCopyTexSubImage2DEXT = (pfn_glCopyTexSubImage2DEXT)GetFunctionPointer("glCopyTexSubImage2DEXT");
    if ( QGLglCopyTexSubImage2DEXT )
        ++QGLNumFunctions_GL_EXT_copy_texture;

    QGLglCopyTexSubImage3DEXT = (pfn_glCopyTexSubImage3DEXT)GetFunctionPointer("glCopyTexSubImage3DEXT");
    if ( QGLglCopyTexSubImage3DEXT )
        ++QGLNumFunctions_GL_EXT_copy_texture;

    if ( QGLNumFunctions_GL_EXT_copy_texture == 5 )
        QGLExists_GL_EXT_copy_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_copy_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_copy_texture ()
{
    QGLNumFunctions_GL_EXT_copy_texture = 0;
    QGLExists_GL_EXT_copy_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_histogram;
int QGLNumFunctions_GL_EXT_histogram;
#ifdef QGL_DEFINED_GL_EXT_histogram
pfn_glGetHistogramEXT QGLglGetHistogramEXT = 0;
pfn_glGetHistogramParameterfvEXT QGLglGetHistogramParameterfvEXT = 0;
pfn_glGetHistogramParameterivEXT QGLglGetHistogramParameterivEXT = 0;
pfn_glGetMinmaxEXT QGLglGetMinmaxEXT = 0;
pfn_glGetMinmaxParameterfvEXT QGLglGetMinmaxParameterfvEXT = 0;
pfn_glGetMinmaxParameterivEXT QGLglGetMinmaxParameterivEXT = 0;
pfn_glHistogramEXT QGLglHistogramEXT = 0;
pfn_glMinmaxEXT QGLglMinmaxEXT = 0;
pfn_glResetHistogramEXT QGLglResetHistogramEXT = 0;
pfn_glResetMinmaxEXT QGLglResetMinmaxEXT = 0;

static void Init_GL_EXT_histogram ()
{
    QGLNumFunctions_GL_EXT_histogram = 0;

    QGLglGetHistogramEXT = (pfn_glGetHistogramEXT)GetFunctionPointer("glGetHistogramEXT");
    if ( QGLglGetHistogramEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglGetHistogramParameterfvEXT = (pfn_glGetHistogramParameterfvEXT)GetFunctionPointer("glGetHistogramParameterfvEXT");
    if ( QGLglGetHistogramParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglGetHistogramParameterivEXT = (pfn_glGetHistogramParameterivEXT)GetFunctionPointer("glGetHistogramParameterivEXT");
    if ( QGLglGetHistogramParameterivEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglGetMinmaxEXT = (pfn_glGetMinmaxEXT)GetFunctionPointer("glGetMinmaxEXT");
    if ( QGLglGetMinmaxEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglGetMinmaxParameterfvEXT = (pfn_glGetMinmaxParameterfvEXT)GetFunctionPointer("glGetMinmaxParameterfvEXT");
    if ( QGLglGetMinmaxParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglGetMinmaxParameterivEXT = (pfn_glGetMinmaxParameterivEXT)GetFunctionPointer("glGetMinmaxParameterivEXT");
    if ( QGLglGetMinmaxParameterivEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglHistogramEXT = (pfn_glHistogramEXT)GetFunctionPointer("glHistogramEXT");
    if ( QGLglHistogramEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglMinmaxEXT = (pfn_glMinmaxEXT)GetFunctionPointer("glMinmaxEXT");
    if ( QGLglMinmaxEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglResetHistogramEXT = (pfn_glResetHistogramEXT)GetFunctionPointer("glResetHistogramEXT");
    if ( QGLglResetHistogramEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    QGLglResetMinmaxEXT = (pfn_glResetMinmaxEXT)GetFunctionPointer("glResetMinmaxEXT");
    if ( QGLglResetMinmaxEXT )
        ++QGLNumFunctions_GL_EXT_histogram;

    if ( QGLNumFunctions_GL_EXT_histogram == 10 )
        QGLExists_GL_EXT_histogram = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_histogram = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_histogram ()
{
    QGLNumFunctions_GL_EXT_histogram = 0;
    QGLExists_GL_EXT_histogram = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_convolution;
int QGLNumFunctions_GL_EXT_convolution;
#ifdef QGL_DEFINED_GL_EXT_convolution
pfn_glConvolutionFilter1DEXT QGLglConvolutionFilter1DEXT = 0;
pfn_glConvolutionFilter2DEXT QGLglConvolutionFilter2DEXT = 0;
pfn_glConvolutionParameterfEXT QGLglConvolutionParameterfEXT = 0;
pfn_glConvolutionParameterfvEXT QGLglConvolutionParameterfvEXT = 0;
pfn_glConvolutionParameteriEXT QGLglConvolutionParameteriEXT = 0;
pfn_glConvolutionParameterivEXT QGLglConvolutionParameterivEXT = 0;
pfn_glCopyConvolutionFilter1DEXT QGLglCopyConvolutionFilter1DEXT = 0;
pfn_glCopyConvolutionFilter2DEXT QGLglCopyConvolutionFilter2DEXT = 0;
pfn_glGetConvolutionFilterEXT QGLglGetConvolutionFilterEXT = 0;
pfn_glGetConvolutionParameterfvEXT QGLglGetConvolutionParameterfvEXT = 0;
pfn_glGetConvolutionParameterivEXT QGLglGetConvolutionParameterivEXT = 0;
pfn_glGetSeparableFilterEXT QGLglGetSeparableFilterEXT = 0;
pfn_glSeparableFilter2DEXT QGLglSeparableFilter2DEXT = 0;

static void Init_GL_EXT_convolution ()
{
    QGLNumFunctions_GL_EXT_convolution = 0;

    QGLglConvolutionFilter1DEXT = (pfn_glConvolutionFilter1DEXT)GetFunctionPointer("glConvolutionFilter1DEXT");
    if ( QGLglConvolutionFilter1DEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglConvolutionFilter2DEXT = (pfn_glConvolutionFilter2DEXT)GetFunctionPointer("glConvolutionFilter2DEXT");
    if ( QGLglConvolutionFilter2DEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglConvolutionParameterfEXT = (pfn_glConvolutionParameterfEXT)GetFunctionPointer("glConvolutionParameterfEXT");
    if ( QGLglConvolutionParameterfEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglConvolutionParameterfvEXT = (pfn_glConvolutionParameterfvEXT)GetFunctionPointer("glConvolutionParameterfvEXT");
    if ( QGLglConvolutionParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglConvolutionParameteriEXT = (pfn_glConvolutionParameteriEXT)GetFunctionPointer("glConvolutionParameteriEXT");
    if ( QGLglConvolutionParameteriEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglConvolutionParameterivEXT = (pfn_glConvolutionParameterivEXT)GetFunctionPointer("glConvolutionParameterivEXT");
    if ( QGLglConvolutionParameterivEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglCopyConvolutionFilter1DEXT = (pfn_glCopyConvolutionFilter1DEXT)GetFunctionPointer("glCopyConvolutionFilter1DEXT");
    if ( QGLglCopyConvolutionFilter1DEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglCopyConvolutionFilter2DEXT = (pfn_glCopyConvolutionFilter2DEXT)GetFunctionPointer("glCopyConvolutionFilter2DEXT");
    if ( QGLglCopyConvolutionFilter2DEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglGetConvolutionFilterEXT = (pfn_glGetConvolutionFilterEXT)GetFunctionPointer("glGetConvolutionFilterEXT");
    if ( QGLglGetConvolutionFilterEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglGetConvolutionParameterfvEXT = (pfn_glGetConvolutionParameterfvEXT)GetFunctionPointer("glGetConvolutionParameterfvEXT");
    if ( QGLglGetConvolutionParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglGetConvolutionParameterivEXT = (pfn_glGetConvolutionParameterivEXT)GetFunctionPointer("glGetConvolutionParameterivEXT");
    if ( QGLglGetConvolutionParameterivEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglGetSeparableFilterEXT = (pfn_glGetSeparableFilterEXT)GetFunctionPointer("glGetSeparableFilterEXT");
    if ( QGLglGetSeparableFilterEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    QGLglSeparableFilter2DEXT = (pfn_glSeparableFilter2DEXT)GetFunctionPointer("glSeparableFilter2DEXT");
    if ( QGLglSeparableFilter2DEXT )
        ++QGLNumFunctions_GL_EXT_convolution;

    if ( QGLNumFunctions_GL_EXT_convolution == 13 )
        QGLExists_GL_EXT_convolution = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_convolution = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_convolution ()
{
    QGLNumFunctions_GL_EXT_convolution = 0;
    QGLExists_GL_EXT_convolution = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_color_matrix;
int QGLNumFunctions_GL_EXT_color_matrix;
#ifdef QGL_DEFINED_GL_EXT_color_matrix
static void Init_GL_EXT_color_matrix ()
{
    QGLNumFunctions_GL_EXT_color_matrix = 0;
    QGLExists_GL_EXT_color_matrix = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_color_matrix ()
{
    QGLNumFunctions_GL_EXT_color_matrix = 0;
    QGLExists_GL_EXT_color_matrix = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGI_color_table;
int QGLNumFunctions_GL_SGI_color_table;
#ifdef QGL_DEFINED_GL_SGI_color_table
pfn_glColorTableSGI QGLglColorTableSGI = 0;
pfn_glColorTableParameterfvSGI QGLglColorTableParameterfvSGI = 0;
pfn_glColorTableParameterivSGI QGLglColorTableParameterivSGI = 0;
pfn_glCopyColorTableSGI QGLglCopyColorTableSGI = 0;
pfn_glGetColorTableSGI QGLglGetColorTableSGI = 0;
pfn_glGetColorTableParameterfvSGI QGLglGetColorTableParameterfvSGI = 0;
pfn_glGetColorTableParameterivSGI QGLglGetColorTableParameterivSGI = 0;

static void Init_GL_SGI_color_table ()
{
    QGLNumFunctions_GL_SGI_color_table = 0;

    QGLglColorTableSGI = (pfn_glColorTableSGI)GetFunctionPointer("glColorTableSGI");
    if ( QGLglColorTableSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglColorTableParameterfvSGI = (pfn_glColorTableParameterfvSGI)GetFunctionPointer("glColorTableParameterfvSGI");
    if ( QGLglColorTableParameterfvSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglColorTableParameterivSGI = (pfn_glColorTableParameterivSGI)GetFunctionPointer("glColorTableParameterivSGI");
    if ( QGLglColorTableParameterivSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglCopyColorTableSGI = (pfn_glCopyColorTableSGI)GetFunctionPointer("glCopyColorTableSGI");
    if ( QGLglCopyColorTableSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglGetColorTableSGI = (pfn_glGetColorTableSGI)GetFunctionPointer("glGetColorTableSGI");
    if ( QGLglGetColorTableSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglGetColorTableParameterfvSGI = (pfn_glGetColorTableParameterfvSGI)GetFunctionPointer("glGetColorTableParameterfvSGI");
    if ( QGLglGetColorTableParameterfvSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    QGLglGetColorTableParameterivSGI = (pfn_glGetColorTableParameterivSGI)GetFunctionPointer("glGetColorTableParameterivSGI");
    if ( QGLglGetColorTableParameterivSGI )
        ++QGLNumFunctions_GL_SGI_color_table;

    if ( QGLNumFunctions_GL_SGI_color_table == 7 )
        QGLExists_GL_SGI_color_table = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGI_color_table = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGI_color_table ()
{
    QGLNumFunctions_GL_SGI_color_table = 0;
    QGLExists_GL_SGI_color_table = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_pixel_texture;
int QGLNumFunctions_GL_SGIX_pixel_texture;
#ifdef QGL_DEFINED_GL_SGIX_pixel_texture
pfn_glPixelTexGenSGIX QGLglPixelTexGenSGIX = 0;

static void Init_GL_SGIX_pixel_texture ()
{
    QGLNumFunctions_GL_SGIX_pixel_texture = 0;

    QGLglPixelTexGenSGIX = (pfn_glPixelTexGenSGIX)GetFunctionPointer("glPixelTexGenSGIX");
    if ( QGLglPixelTexGenSGIX )
        ++QGLNumFunctions_GL_SGIX_pixel_texture;

    if ( QGLNumFunctions_GL_SGIX_pixel_texture == 1 )
        QGLExists_GL_SGIX_pixel_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_pixel_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_pixel_texture ()
{
    QGLNumFunctions_GL_SGIX_pixel_texture = 0;
    QGLExists_GL_SGIX_pixel_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_pixel_texture;
int QGLNumFunctions_GL_SGIS_pixel_texture;
#ifdef QGL_DEFINED_GL_SGIS_pixel_texture
pfn_glPixelTexGenParameteriSGIS QGLglPixelTexGenParameteriSGIS = 0;
pfn_glPixelTexGenParameterivSGIS QGLglPixelTexGenParameterivSGIS = 0;
pfn_glPixelTexGenParameterfSGIS QGLglPixelTexGenParameterfSGIS = 0;
pfn_glPixelTexGenParameterfvSGIS QGLglPixelTexGenParameterfvSGIS = 0;
pfn_glGetPixelTexGenParameterivSGIS QGLglGetPixelTexGenParameterivSGIS = 0;
pfn_glGetPixelTexGenParameterfvSGIS QGLglGetPixelTexGenParameterfvSGIS = 0;

static void Init_GL_SGIS_pixel_texture ()
{
    QGLNumFunctions_GL_SGIS_pixel_texture = 0;

    QGLglPixelTexGenParameteriSGIS = (pfn_glPixelTexGenParameteriSGIS)GetFunctionPointer("glPixelTexGenParameteriSGIS");
    if ( QGLglPixelTexGenParameteriSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    QGLglPixelTexGenParameterivSGIS = (pfn_glPixelTexGenParameterivSGIS)GetFunctionPointer("glPixelTexGenParameterivSGIS");
    if ( QGLglPixelTexGenParameterivSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    QGLglPixelTexGenParameterfSGIS = (pfn_glPixelTexGenParameterfSGIS)GetFunctionPointer("glPixelTexGenParameterfSGIS");
    if ( QGLglPixelTexGenParameterfSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    QGLglPixelTexGenParameterfvSGIS = (pfn_glPixelTexGenParameterfvSGIS)GetFunctionPointer("glPixelTexGenParameterfvSGIS");
    if ( QGLglPixelTexGenParameterfvSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    QGLglGetPixelTexGenParameterivSGIS = (pfn_glGetPixelTexGenParameterivSGIS)GetFunctionPointer("glGetPixelTexGenParameterivSGIS");
    if ( QGLglGetPixelTexGenParameterivSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    QGLglGetPixelTexGenParameterfvSGIS = (pfn_glGetPixelTexGenParameterfvSGIS)GetFunctionPointer("glGetPixelTexGenParameterfvSGIS");
    if ( QGLglGetPixelTexGenParameterfvSGIS )
        ++QGLNumFunctions_GL_SGIS_pixel_texture;

    if ( QGLNumFunctions_GL_SGIS_pixel_texture == 6 )
        QGLExists_GL_SGIS_pixel_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_pixel_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_pixel_texture ()
{
    QGLNumFunctions_GL_SGIS_pixel_texture = 0;
    QGLExists_GL_SGIS_pixel_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture4D;
int QGLNumFunctions_GL_SGIS_texture4D;
#ifdef QGL_DEFINED_GL_SGIS_texture4D
pfn_glTexImage4DSGIS QGLglTexImage4DSGIS = 0;
pfn_glTexSubImage4DSGIS QGLglTexSubImage4DSGIS = 0;

static void Init_GL_SGIS_texture4D ()
{
    QGLNumFunctions_GL_SGIS_texture4D = 0;

    QGLglTexImage4DSGIS = (pfn_glTexImage4DSGIS)GetFunctionPointer("glTexImage4DSGIS");
    if ( QGLglTexImage4DSGIS )
        ++QGLNumFunctions_GL_SGIS_texture4D;

    QGLglTexSubImage4DSGIS = (pfn_glTexSubImage4DSGIS)GetFunctionPointer("glTexSubImage4DSGIS");
    if ( QGLglTexSubImage4DSGIS )
        ++QGLNumFunctions_GL_SGIS_texture4D;

    if ( QGLNumFunctions_GL_SGIS_texture4D == 2 )
        QGLExists_GL_SGIS_texture4D = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_texture4D = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture4D ()
{
    QGLNumFunctions_GL_SGIS_texture4D = 0;
    QGLExists_GL_SGIS_texture4D = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGI_texture_color_table;
int QGLNumFunctions_GL_SGI_texture_color_table;
#ifdef QGL_DEFINED_GL_SGI_texture_color_table
static void Init_GL_SGI_texture_color_table ()
{
    QGLNumFunctions_GL_SGI_texture_color_table = 0;
    QGLExists_GL_SGI_texture_color_table = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGI_texture_color_table ()
{
    QGLNumFunctions_GL_SGI_texture_color_table = 0;
    QGLExists_GL_SGI_texture_color_table = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_cmyka;
int QGLNumFunctions_GL_EXT_cmyka;
#ifdef QGL_DEFINED_GL_EXT_cmyka
static void Init_GL_EXT_cmyka ()
{
    QGLNumFunctions_GL_EXT_cmyka = 0;
    QGLExists_GL_EXT_cmyka = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_cmyka ()
{
    QGLNumFunctions_GL_EXT_cmyka = 0;
    QGLExists_GL_EXT_cmyka = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_object;
int QGLNumFunctions_GL_EXT_texture_object;
#ifdef QGL_DEFINED_GL_EXT_texture_object
pfn_glAreTexturesResidentEXT QGLglAreTexturesResidentEXT = 0;
pfn_glBindTextureEXT QGLglBindTextureEXT = 0;
pfn_glDeleteTexturesEXT QGLglDeleteTexturesEXT = 0;
pfn_glGenTexturesEXT QGLglGenTexturesEXT = 0;
pfn_glIsTextureEXT QGLglIsTextureEXT = 0;
pfn_glPrioritizeTexturesEXT QGLglPrioritizeTexturesEXT = 0;

static void Init_GL_EXT_texture_object ()
{
    QGLNumFunctions_GL_EXT_texture_object = 0;

    QGLglAreTexturesResidentEXT = (pfn_glAreTexturesResidentEXT)GetFunctionPointer("glAreTexturesResidentEXT");
    if ( QGLglAreTexturesResidentEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    QGLglBindTextureEXT = (pfn_glBindTextureEXT)GetFunctionPointer("glBindTextureEXT");
    if ( QGLglBindTextureEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    QGLglDeleteTexturesEXT = (pfn_glDeleteTexturesEXT)GetFunctionPointer("glDeleteTexturesEXT");
    if ( QGLglDeleteTexturesEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    QGLglGenTexturesEXT = (pfn_glGenTexturesEXT)GetFunctionPointer("glGenTexturesEXT");
    if ( QGLglGenTexturesEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    QGLglIsTextureEXT = (pfn_glIsTextureEXT)GetFunctionPointer("glIsTextureEXT");
    if ( QGLglIsTextureEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    QGLglPrioritizeTexturesEXT = (pfn_glPrioritizeTexturesEXT)GetFunctionPointer("glPrioritizeTexturesEXT");
    if ( QGLglPrioritizeTexturesEXT )
        ++QGLNumFunctions_GL_EXT_texture_object;

    if ( QGLNumFunctions_GL_EXT_texture_object == 6 )
        QGLExists_GL_EXT_texture_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_texture_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_object ()
{
    QGLNumFunctions_GL_EXT_texture_object = 0;
    QGLExists_GL_EXT_texture_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_detail_texture;
int QGLNumFunctions_GL_SGIS_detail_texture;
#ifdef QGL_DEFINED_GL_SGIS_detail_texture
pfn_glDetailTexFuncSGIS QGLglDetailTexFuncSGIS = 0;
pfn_glGetDetailTexFuncSGIS QGLglGetDetailTexFuncSGIS = 0;

static void Init_GL_SGIS_detail_texture ()
{
    QGLNumFunctions_GL_SGIS_detail_texture = 0;

    QGLglDetailTexFuncSGIS = (pfn_glDetailTexFuncSGIS)GetFunctionPointer("glDetailTexFuncSGIS");
    if ( QGLglDetailTexFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_detail_texture;

    QGLglGetDetailTexFuncSGIS = (pfn_glGetDetailTexFuncSGIS)GetFunctionPointer("glGetDetailTexFuncSGIS");
    if ( QGLglGetDetailTexFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_detail_texture;

    if ( QGLNumFunctions_GL_SGIS_detail_texture == 2 )
        QGLExists_GL_SGIS_detail_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_detail_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_detail_texture ()
{
    QGLNumFunctions_GL_SGIS_detail_texture = 0;
    QGLExists_GL_SGIS_detail_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_sharpen_texture;
int QGLNumFunctions_GL_SGIS_sharpen_texture;
#ifdef QGL_DEFINED_GL_SGIS_sharpen_texture
pfn_glSharpenTexFuncSGIS QGLglSharpenTexFuncSGIS = 0;
pfn_glGetSharpenTexFuncSGIS QGLglGetSharpenTexFuncSGIS = 0;

static void Init_GL_SGIS_sharpen_texture ()
{
    QGLNumFunctions_GL_SGIS_sharpen_texture = 0;

    QGLglSharpenTexFuncSGIS = (pfn_glSharpenTexFuncSGIS)GetFunctionPointer("glSharpenTexFuncSGIS");
    if ( QGLglSharpenTexFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_sharpen_texture;

    QGLglGetSharpenTexFuncSGIS = (pfn_glGetSharpenTexFuncSGIS)GetFunctionPointer("glGetSharpenTexFuncSGIS");
    if ( QGLglGetSharpenTexFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_sharpen_texture;

    if ( QGLNumFunctions_GL_SGIS_sharpen_texture == 2 )
        QGLExists_GL_SGIS_sharpen_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_sharpen_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_sharpen_texture ()
{
    QGLNumFunctions_GL_SGIS_sharpen_texture = 0;
    QGLExists_GL_SGIS_sharpen_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_packed_pixels;
int QGLNumFunctions_GL_EXT_packed_pixels;
#ifdef QGL_DEFINED_GL_EXT_packed_pixels
static void Init_GL_EXT_packed_pixels ()
{
    QGLNumFunctions_GL_EXT_packed_pixels = 0;
    QGLExists_GL_EXT_packed_pixels = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_packed_pixels ()
{
    QGLNumFunctions_GL_EXT_packed_pixels = 0;
    QGLExists_GL_EXT_packed_pixels = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture_lod;
int QGLNumFunctions_GL_SGIS_texture_lod;
#ifdef QGL_DEFINED_GL_SGIS_texture_lod
static void Init_GL_SGIS_texture_lod ()
{
    QGLNumFunctions_GL_SGIS_texture_lod = 0;
    QGLExists_GL_SGIS_texture_lod = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture_lod ()
{
    QGLNumFunctions_GL_SGIS_texture_lod = 0;
    QGLExists_GL_SGIS_texture_lod = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_multisample;
int QGLNumFunctions_GL_SGIS_multisample;
#ifdef QGL_DEFINED_GL_SGIS_multisample
pfn_glSampleMaskSGIS QGLglSampleMaskSGIS = 0;
pfn_glSamplePatternSGIS QGLglSamplePatternSGIS = 0;

static void Init_GL_SGIS_multisample ()
{
    QGLNumFunctions_GL_SGIS_multisample = 0;

    QGLglSampleMaskSGIS = (pfn_glSampleMaskSGIS)GetFunctionPointer("glSampleMaskSGIS");
    if ( QGLglSampleMaskSGIS )
        ++QGLNumFunctions_GL_SGIS_multisample;

    QGLglSamplePatternSGIS = (pfn_glSamplePatternSGIS)GetFunctionPointer("glSamplePatternSGIS");
    if ( QGLglSamplePatternSGIS )
        ++QGLNumFunctions_GL_SGIS_multisample;

    if ( QGLNumFunctions_GL_SGIS_multisample == 2 )
        QGLExists_GL_SGIS_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_multisample ()
{
    QGLNumFunctions_GL_SGIS_multisample = 0;
    QGLExists_GL_SGIS_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_rescale_normal;
int QGLNumFunctions_GL_EXT_rescale_normal;
#ifdef QGL_DEFINED_GL_EXT_rescale_normal
static void Init_GL_EXT_rescale_normal ()
{
    QGLNumFunctions_GL_EXT_rescale_normal = 0;
    QGLExists_GL_EXT_rescale_normal = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_rescale_normal ()
{
    QGLNumFunctions_GL_EXT_rescale_normal = 0;
    QGLExists_GL_EXT_rescale_normal = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_vertex_array;
int QGLNumFunctions_GL_EXT_vertex_array;
#ifdef QGL_DEFINED_GL_EXT_vertex_array
pfn_glArrayElementEXT QGLglArrayElementEXT = 0;
pfn_glColorPointerEXT QGLglColorPointerEXT = 0;
pfn_glDrawArraysEXT QGLglDrawArraysEXT = 0;
pfn_glEdgeFlagPointerEXT QGLglEdgeFlagPointerEXT = 0;
pfn_glGetPointervEXT QGLglGetPointervEXT = 0;
pfn_glIndexPointerEXT QGLglIndexPointerEXT = 0;
pfn_glNormalPointerEXT QGLglNormalPointerEXT = 0;
pfn_glTexCoordPointerEXT QGLglTexCoordPointerEXT = 0;
pfn_glVertexPointerEXT QGLglVertexPointerEXT = 0;

static void Init_GL_EXT_vertex_array ()
{
    QGLNumFunctions_GL_EXT_vertex_array = 0;

    QGLglArrayElementEXT = (pfn_glArrayElementEXT)GetFunctionPointer("glArrayElementEXT");
    if ( QGLglArrayElementEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglColorPointerEXT = (pfn_glColorPointerEXT)GetFunctionPointer("glColorPointerEXT");
    if ( QGLglColorPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglDrawArraysEXT = (pfn_glDrawArraysEXT)GetFunctionPointer("glDrawArraysEXT");
    if ( QGLglDrawArraysEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglEdgeFlagPointerEXT = (pfn_glEdgeFlagPointerEXT)GetFunctionPointer("glEdgeFlagPointerEXT");
    if ( QGLglEdgeFlagPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglGetPointervEXT = (pfn_glGetPointervEXT)GetFunctionPointer("glGetPointervEXT");
    if ( QGLglGetPointervEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglIndexPointerEXT = (pfn_glIndexPointerEXT)GetFunctionPointer("glIndexPointerEXT");
    if ( QGLglIndexPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglNormalPointerEXT = (pfn_glNormalPointerEXT)GetFunctionPointer("glNormalPointerEXT");
    if ( QGLglNormalPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglTexCoordPointerEXT = (pfn_glTexCoordPointerEXT)GetFunctionPointer("glTexCoordPointerEXT");
    if ( QGLglTexCoordPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    QGLglVertexPointerEXT = (pfn_glVertexPointerEXT)GetFunctionPointer("glVertexPointerEXT");
    if ( QGLglVertexPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_array;

    if ( QGLNumFunctions_GL_EXT_vertex_array == 9 )
        QGLExists_GL_EXT_vertex_array = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_vertex_array = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_vertex_array ()
{
    QGLNumFunctions_GL_EXT_vertex_array = 0;
    QGLExists_GL_EXT_vertex_array = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_misc_attribute;
int QGLNumFunctions_GL_EXT_misc_attribute;
#ifdef QGL_DEFINED_GL_EXT_misc_attribute
static void Init_GL_EXT_misc_attribute ()
{
    QGLNumFunctions_GL_EXT_misc_attribute = 0;
    QGLExists_GL_EXT_misc_attribute = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_misc_attribute ()
{
    QGLNumFunctions_GL_EXT_misc_attribute = 0;
    QGLExists_GL_EXT_misc_attribute = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_generate_mipmap;
int QGLNumFunctions_GL_SGIS_generate_mipmap;
#ifdef QGL_DEFINED_GL_SGIS_generate_mipmap
static void Init_GL_SGIS_generate_mipmap ()
{
    QGLNumFunctions_GL_SGIS_generate_mipmap = 0;
    QGLExists_GL_SGIS_generate_mipmap = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIS_generate_mipmap ()
{
    QGLNumFunctions_GL_SGIS_generate_mipmap = 0;
    QGLExists_GL_SGIS_generate_mipmap = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_clipmap;
int QGLNumFunctions_GL_SGIX_clipmap;
#ifdef QGL_DEFINED_GL_SGIX_clipmap
static void Init_GL_SGIX_clipmap ()
{
    QGLNumFunctions_GL_SGIX_clipmap = 0;
    QGLExists_GL_SGIX_clipmap = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_clipmap ()
{
    QGLNumFunctions_GL_SGIX_clipmap = 0;
    QGLExists_GL_SGIX_clipmap = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_shadow;
int QGLNumFunctions_GL_SGIX_shadow;
#ifdef QGL_DEFINED_GL_SGIX_shadow
static void Init_GL_SGIX_shadow ()
{
    QGLNumFunctions_GL_SGIX_shadow = 0;
    QGLExists_GL_SGIX_shadow = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_shadow ()
{
    QGLNumFunctions_GL_SGIX_shadow = 0;
    QGLExists_GL_SGIX_shadow = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture_edge_clamp;
int QGLNumFunctions_GL_SGIS_texture_edge_clamp;
#ifdef QGL_DEFINED_GL_SGIS_texture_edge_clamp
static void Init_GL_SGIS_texture_edge_clamp ()
{
    QGLNumFunctions_GL_SGIS_texture_edge_clamp = 0;
    QGLExists_GL_SGIS_texture_edge_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture_edge_clamp ()
{
    QGLNumFunctions_GL_SGIS_texture_edge_clamp = 0;
    QGLExists_GL_SGIS_texture_edge_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture_border_clamp;
int QGLNumFunctions_GL_SGIS_texture_border_clamp;
#ifdef QGL_DEFINED_GL_SGIS_texture_border_clamp
static void Init_GL_SGIS_texture_border_clamp ()
{
    QGLNumFunctions_GL_SGIS_texture_border_clamp = 0;
    QGLExists_GL_SGIS_texture_border_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture_border_clamp ()
{
    QGLNumFunctions_GL_SGIS_texture_border_clamp = 0;
    QGLExists_GL_SGIS_texture_border_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_minmax;
int QGLNumFunctions_GL_EXT_blend_minmax;
#ifdef QGL_DEFINED_GL_EXT_blend_minmax
pfn_glBlendEquationEXT QGLglBlendEquationEXT = 0;

static void Init_GL_EXT_blend_minmax ()
{
    QGLNumFunctions_GL_EXT_blend_minmax = 0;

    QGLglBlendEquationEXT = (pfn_glBlendEquationEXT)GetFunctionPointer("glBlendEquationEXT");
    if ( QGLglBlendEquationEXT )
        ++QGLNumFunctions_GL_EXT_blend_minmax;

    if ( QGLNumFunctions_GL_EXT_blend_minmax == 1 )
        QGLExists_GL_EXT_blend_minmax = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_blend_minmax = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_minmax ()
{
    QGLNumFunctions_GL_EXT_blend_minmax = 0;
    QGLExists_GL_EXT_blend_minmax = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_subtract;
int QGLNumFunctions_GL_EXT_blend_subtract;
#ifdef QGL_DEFINED_GL_EXT_blend_subtract
static void Init_GL_EXT_blend_subtract ()
{
    QGLNumFunctions_GL_EXT_blend_subtract = 0;
    QGLExists_GL_EXT_blend_subtract = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_subtract ()
{
    QGLNumFunctions_GL_EXT_blend_subtract = 0;
    QGLExists_GL_EXT_blend_subtract = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_logic_op;
int QGLNumFunctions_GL_EXT_blend_logic_op;
#ifdef QGL_DEFINED_GL_EXT_blend_logic_op
static void Init_GL_EXT_blend_logic_op ()
{
    QGLNumFunctions_GL_EXT_blend_logic_op = 0;
    QGLExists_GL_EXT_blend_logic_op = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_logic_op ()
{
    QGLNumFunctions_GL_EXT_blend_logic_op = 0;
    QGLExists_GL_EXT_blend_logic_op = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_interlace;
int QGLNumFunctions_GL_SGIX_interlace;
#ifdef QGL_DEFINED_GL_SGIX_interlace
static void Init_GL_SGIX_interlace ()
{
    QGLNumFunctions_GL_SGIX_interlace = 0;
    QGLExists_GL_SGIX_interlace = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_interlace ()
{
    QGLNumFunctions_GL_SGIX_interlace = 0;
    QGLExists_GL_SGIX_interlace = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_pixel_tiles;
int QGLNumFunctions_GL_SGIX_pixel_tiles;
#ifdef QGL_DEFINED_GL_SGIX_pixel_tiles
static void Init_GL_SGIX_pixel_tiles ()
{
    QGLNumFunctions_GL_SGIX_pixel_tiles = 0;
    QGLExists_GL_SGIX_pixel_tiles = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_pixel_tiles ()
{
    QGLNumFunctions_GL_SGIX_pixel_tiles = 0;
    QGLExists_GL_SGIX_pixel_tiles = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_select;
int QGLNumFunctions_GL_SGIX_texture_select;
#ifdef QGL_DEFINED_GL_SGIX_texture_select
static void Init_GL_SGIX_texture_select ()
{
    QGLNumFunctions_GL_SGIX_texture_select = 0;
    QGLExists_GL_SGIX_texture_select = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_select ()
{
    QGLNumFunctions_GL_SGIX_texture_select = 0;
    QGLExists_GL_SGIX_texture_select = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_sprite;
int QGLNumFunctions_GL_SGIX_sprite;
#ifdef QGL_DEFINED_GL_SGIX_sprite
pfn_glSpriteParameterfSGIX QGLglSpriteParameterfSGIX = 0;
pfn_glSpriteParameterfvSGIX QGLglSpriteParameterfvSGIX = 0;
pfn_glSpriteParameteriSGIX QGLglSpriteParameteriSGIX = 0;
pfn_glSpriteParameterivSGIX QGLglSpriteParameterivSGIX = 0;

static void Init_GL_SGIX_sprite ()
{
    QGLNumFunctions_GL_SGIX_sprite = 0;

    QGLglSpriteParameterfSGIX = (pfn_glSpriteParameterfSGIX)GetFunctionPointer("glSpriteParameterfSGIX");
    if ( QGLglSpriteParameterfSGIX )
        ++QGLNumFunctions_GL_SGIX_sprite;

    QGLglSpriteParameterfvSGIX = (pfn_glSpriteParameterfvSGIX)GetFunctionPointer("glSpriteParameterfvSGIX");
    if ( QGLglSpriteParameterfvSGIX )
        ++QGLNumFunctions_GL_SGIX_sprite;

    QGLglSpriteParameteriSGIX = (pfn_glSpriteParameteriSGIX)GetFunctionPointer("glSpriteParameteriSGIX");
    if ( QGLglSpriteParameteriSGIX )
        ++QGLNumFunctions_GL_SGIX_sprite;

    QGLglSpriteParameterivSGIX = (pfn_glSpriteParameterivSGIX)GetFunctionPointer("glSpriteParameterivSGIX");
    if ( QGLglSpriteParameterivSGIX )
        ++QGLNumFunctions_GL_SGIX_sprite;

    if ( QGLNumFunctions_GL_SGIX_sprite == 4 )
        QGLExists_GL_SGIX_sprite = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_sprite = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_sprite ()
{
    QGLNumFunctions_GL_SGIX_sprite = 0;
    QGLExists_GL_SGIX_sprite = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_multi_buffer;
int QGLNumFunctions_GL_SGIX_texture_multi_buffer;
#ifdef QGL_DEFINED_GL_SGIX_texture_multi_buffer
static void Init_GL_SGIX_texture_multi_buffer ()
{
    QGLNumFunctions_GL_SGIX_texture_multi_buffer = 0;
    QGLExists_GL_SGIX_texture_multi_buffer = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_multi_buffer ()
{
    QGLNumFunctions_GL_SGIX_texture_multi_buffer = 0;
    QGLExists_GL_SGIX_texture_multi_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_point_parameters;
int QGLNumFunctions_GL_EXT_point_parameters;
#ifdef QGL_DEFINED_GL_EXT_point_parameters
pfn_glPointParameterfEXT QGLglPointParameterfEXT = 0;
pfn_glPointParameterfvEXT QGLglPointParameterfvEXT = 0;

static void Init_GL_EXT_point_parameters ()
{
    QGLNumFunctions_GL_EXT_point_parameters = 0;

    QGLglPointParameterfEXT = (pfn_glPointParameterfEXT)GetFunctionPointer("glPointParameterfEXT");
    if ( QGLglPointParameterfEXT )
        ++QGLNumFunctions_GL_EXT_point_parameters;

    QGLglPointParameterfvEXT = (pfn_glPointParameterfvEXT)GetFunctionPointer("glPointParameterfvEXT");
    if ( QGLglPointParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_point_parameters;

    if ( QGLNumFunctions_GL_EXT_point_parameters == 2 )
        QGLExists_GL_EXT_point_parameters = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_point_parameters = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_point_parameters ()
{
    QGLNumFunctions_GL_EXT_point_parameters = 0;
    QGLExists_GL_EXT_point_parameters = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_point_parameters;
int QGLNumFunctions_GL_SGIS_point_parameters;
#ifdef QGL_DEFINED_GL_SGIS_point_parameters
pfn_glPointParameterfSGIS QGLglPointParameterfSGIS = 0;
pfn_glPointParameterfvSGIS QGLglPointParameterfvSGIS = 0;

static void Init_GL_SGIS_point_parameters ()
{
    QGLNumFunctions_GL_SGIS_point_parameters = 0;

    QGLglPointParameterfSGIS = (pfn_glPointParameterfSGIS)GetFunctionPointer("glPointParameterfSGIS");
    if ( QGLglPointParameterfSGIS )
        ++QGLNumFunctions_GL_SGIS_point_parameters;

    QGLglPointParameterfvSGIS = (pfn_glPointParameterfvSGIS)GetFunctionPointer("glPointParameterfvSGIS");
    if ( QGLglPointParameterfvSGIS )
        ++QGLNumFunctions_GL_SGIS_point_parameters;

    if ( QGLNumFunctions_GL_SGIS_point_parameters == 2 )
        QGLExists_GL_SGIS_point_parameters = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_point_parameters = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_point_parameters ()
{
    QGLNumFunctions_GL_SGIS_point_parameters = 0;
    QGLExists_GL_SGIS_point_parameters = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_instruments;
int QGLNumFunctions_GL_SGIX_instruments;
#ifdef QGL_DEFINED_GL_SGIX_instruments
pfn_glGetInstrumentsSGIX QGLglGetInstrumentsSGIX = 0;
pfn_glInstrumentsBufferSGIX QGLglInstrumentsBufferSGIX = 0;
pfn_glPollInstrumentsSGIX QGLglPollInstrumentsSGIX = 0;
pfn_glReadInstrumentsSGIX QGLglReadInstrumentsSGIX = 0;
pfn_glStartInstrumentsSGIX QGLglStartInstrumentsSGIX = 0;
pfn_glStopInstrumentsSGIX QGLglStopInstrumentsSGIX = 0;

static void Init_GL_SGIX_instruments ()
{
    QGLNumFunctions_GL_SGIX_instruments = 0;

    QGLglGetInstrumentsSGIX = (pfn_glGetInstrumentsSGIX)GetFunctionPointer("glGetInstrumentsSGIX");
    if ( QGLglGetInstrumentsSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    QGLglInstrumentsBufferSGIX = (pfn_glInstrumentsBufferSGIX)GetFunctionPointer("glInstrumentsBufferSGIX");
    if ( QGLglInstrumentsBufferSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    QGLglPollInstrumentsSGIX = (pfn_glPollInstrumentsSGIX)GetFunctionPointer("glPollInstrumentsSGIX");
    if ( QGLglPollInstrumentsSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    QGLglReadInstrumentsSGIX = (pfn_glReadInstrumentsSGIX)GetFunctionPointer("glReadInstrumentsSGIX");
    if ( QGLglReadInstrumentsSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    QGLglStartInstrumentsSGIX = (pfn_glStartInstrumentsSGIX)GetFunctionPointer("glStartInstrumentsSGIX");
    if ( QGLglStartInstrumentsSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    QGLglStopInstrumentsSGIX = (pfn_glStopInstrumentsSGIX)GetFunctionPointer("glStopInstrumentsSGIX");
    if ( QGLglStopInstrumentsSGIX )
        ++QGLNumFunctions_GL_SGIX_instruments;

    if ( QGLNumFunctions_GL_SGIX_instruments == 6 )
        QGLExists_GL_SGIX_instruments = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_instruments = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_instruments ()
{
    QGLNumFunctions_GL_SGIX_instruments = 0;
    QGLExists_GL_SGIX_instruments = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_scale_bias;
int QGLNumFunctions_GL_SGIX_texture_scale_bias;
#ifdef QGL_DEFINED_GL_SGIX_texture_scale_bias
static void Init_GL_SGIX_texture_scale_bias ()
{
    QGLNumFunctions_GL_SGIX_texture_scale_bias = 0;
    QGLExists_GL_SGIX_texture_scale_bias = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_scale_bias ()
{
    QGLNumFunctions_GL_SGIX_texture_scale_bias = 0;
    QGLExists_GL_SGIX_texture_scale_bias = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_framezoom;
int QGLNumFunctions_GL_SGIX_framezoom;
#ifdef QGL_DEFINED_GL_SGIX_framezoom
pfn_glFrameZoomSGIX QGLglFrameZoomSGIX = 0;

static void Init_GL_SGIX_framezoom ()
{
    QGLNumFunctions_GL_SGIX_framezoom = 0;

    QGLglFrameZoomSGIX = (pfn_glFrameZoomSGIX)GetFunctionPointer("glFrameZoomSGIX");
    if ( QGLglFrameZoomSGIX )
        ++QGLNumFunctions_GL_SGIX_framezoom;

    if ( QGLNumFunctions_GL_SGIX_framezoom == 1 )
        QGLExists_GL_SGIX_framezoom = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_framezoom = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_framezoom ()
{
    QGLNumFunctions_GL_SGIX_framezoom = 0;
    QGLExists_GL_SGIX_framezoom = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_tag_sample_buffer;
int QGLNumFunctions_GL_SGIX_tag_sample_buffer;
#ifdef QGL_DEFINED_GL_SGIX_tag_sample_buffer
pfn_glTagSampleBufferSGIX QGLglTagSampleBufferSGIX = 0;

static void Init_GL_SGIX_tag_sample_buffer ()
{
    QGLNumFunctions_GL_SGIX_tag_sample_buffer = 0;

    QGLglTagSampleBufferSGIX = (pfn_glTagSampleBufferSGIX)GetFunctionPointer("glTagSampleBufferSGIX");
    if ( QGLglTagSampleBufferSGIX )
        ++QGLNumFunctions_GL_SGIX_tag_sample_buffer;

    if ( QGLNumFunctions_GL_SGIX_tag_sample_buffer == 1 )
        QGLExists_GL_SGIX_tag_sample_buffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_tag_sample_buffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_tag_sample_buffer ()
{
    QGLNumFunctions_GL_SGIX_tag_sample_buffer = 0;
    QGLExists_GL_SGIX_tag_sample_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_FfdMaskSGIX;
int QGLNumFunctions_GL_FfdMaskSGIX;
#ifdef QGL_DEFINED_GL_FfdMaskSGIX
static void Init_GL_FfdMaskSGIX ()
{
    QGLNumFunctions_GL_FfdMaskSGIX = 0;
    QGLExists_GL_FfdMaskSGIX = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_FfdMaskSGIX ()
{
    QGLNumFunctions_GL_FfdMaskSGIX = 0;
    QGLExists_GL_FfdMaskSGIX = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_polynomial_ffd;
int QGLNumFunctions_GL_SGIX_polynomial_ffd;
#ifdef QGL_DEFINED_GL_SGIX_polynomial_ffd
pfn_glDeformationMap3dSGIX QGLglDeformationMap3dSGIX = 0;
pfn_glDeformationMap3fSGIX QGLglDeformationMap3fSGIX = 0;
pfn_glDeformSGIX QGLglDeformSGIX = 0;
pfn_glLoadIdentityDeformationMapSGIX QGLglLoadIdentityDeformationMapSGIX = 0;

static void Init_GL_SGIX_polynomial_ffd ()
{
    QGLNumFunctions_GL_SGIX_polynomial_ffd = 0;

    QGLglDeformationMap3dSGIX = (pfn_glDeformationMap3dSGIX)GetFunctionPointer("glDeformationMap3dSGIX");
    if ( QGLglDeformationMap3dSGIX )
        ++QGLNumFunctions_GL_SGIX_polynomial_ffd;

    QGLglDeformationMap3fSGIX = (pfn_glDeformationMap3fSGIX)GetFunctionPointer("glDeformationMap3fSGIX");
    if ( QGLglDeformationMap3fSGIX )
        ++QGLNumFunctions_GL_SGIX_polynomial_ffd;

    QGLglDeformSGIX = (pfn_glDeformSGIX)GetFunctionPointer("glDeformSGIX");
    if ( QGLglDeformSGIX )
        ++QGLNumFunctions_GL_SGIX_polynomial_ffd;

    QGLglLoadIdentityDeformationMapSGIX = (pfn_glLoadIdentityDeformationMapSGIX)GetFunctionPointer("glLoadIdentityDeformationMapSGIX");
    if ( QGLglLoadIdentityDeformationMapSGIX )
        ++QGLNumFunctions_GL_SGIX_polynomial_ffd;

    if ( QGLNumFunctions_GL_SGIX_polynomial_ffd == 4 )
        QGLExists_GL_SGIX_polynomial_ffd = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_polynomial_ffd = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_polynomial_ffd ()
{
    QGLNumFunctions_GL_SGIX_polynomial_ffd = 0;
    QGLExists_GL_SGIX_polynomial_ffd = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_reference_plane;
int QGLNumFunctions_GL_SGIX_reference_plane;
#ifdef QGL_DEFINED_GL_SGIX_reference_plane
pfn_glReferencePlaneSGIX QGLglReferencePlaneSGIX = 0;

static void Init_GL_SGIX_reference_plane ()
{
    QGLNumFunctions_GL_SGIX_reference_plane = 0;

    QGLglReferencePlaneSGIX = (pfn_glReferencePlaneSGIX)GetFunctionPointer("glReferencePlaneSGIX");
    if ( QGLglReferencePlaneSGIX )
        ++QGLNumFunctions_GL_SGIX_reference_plane;

    if ( QGLNumFunctions_GL_SGIX_reference_plane == 1 )
        QGLExists_GL_SGIX_reference_plane = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_reference_plane = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_reference_plane ()
{
    QGLNumFunctions_GL_SGIX_reference_plane = 0;
    QGLExists_GL_SGIX_reference_plane = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_flush_raster;
int QGLNumFunctions_GL_SGIX_flush_raster;
#ifdef QGL_DEFINED_GL_SGIX_flush_raster
pfn_glFlushRasterSGIX QGLglFlushRasterSGIX = 0;

static void Init_GL_SGIX_flush_raster ()
{
    QGLNumFunctions_GL_SGIX_flush_raster = 0;

    QGLglFlushRasterSGIX = (pfn_glFlushRasterSGIX)GetFunctionPointer("glFlushRasterSGIX");
    if ( QGLglFlushRasterSGIX )
        ++QGLNumFunctions_GL_SGIX_flush_raster;

    if ( QGLNumFunctions_GL_SGIX_flush_raster == 1 )
        QGLExists_GL_SGIX_flush_raster = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_flush_raster = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_flush_raster ()
{
    QGLNumFunctions_GL_SGIX_flush_raster = 0;
    QGLExists_GL_SGIX_flush_raster = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_depth_texture;
int QGLNumFunctions_GL_SGIX_depth_texture;
#ifdef QGL_DEFINED_GL_SGIX_depth_texture
static void Init_GL_SGIX_depth_texture ()
{
    QGLNumFunctions_GL_SGIX_depth_texture = 0;
    QGLExists_GL_SGIX_depth_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_depth_texture ()
{
    QGLNumFunctions_GL_SGIX_depth_texture = 0;
    QGLExists_GL_SGIX_depth_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_fog_function;
int QGLNumFunctions_GL_SGIS_fog_function;
#ifdef QGL_DEFINED_GL_SGIS_fog_function
pfn_glFogFuncSGIS QGLglFogFuncSGIS = 0;
pfn_glGetFogFuncSGIS QGLglGetFogFuncSGIS = 0;

static void Init_GL_SGIS_fog_function ()
{
    QGLNumFunctions_GL_SGIS_fog_function = 0;

    QGLglFogFuncSGIS = (pfn_glFogFuncSGIS)GetFunctionPointer("glFogFuncSGIS");
    if ( QGLglFogFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_fog_function;

    QGLglGetFogFuncSGIS = (pfn_glGetFogFuncSGIS)GetFunctionPointer("glGetFogFuncSGIS");
    if ( QGLglGetFogFuncSGIS )
        ++QGLNumFunctions_GL_SGIS_fog_function;

    if ( QGLNumFunctions_GL_SGIS_fog_function == 2 )
        QGLExists_GL_SGIS_fog_function = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_fog_function = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_fog_function ()
{
    QGLNumFunctions_GL_SGIS_fog_function = 0;
    QGLExists_GL_SGIS_fog_function = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_fog_offset;
int QGLNumFunctions_GL_SGIX_fog_offset;
#ifdef QGL_DEFINED_GL_SGIX_fog_offset
static void Init_GL_SGIX_fog_offset ()
{
    QGLNumFunctions_GL_SGIX_fog_offset = 0;
    QGLExists_GL_SGIX_fog_offset = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_fog_offset ()
{
    QGLNumFunctions_GL_SGIX_fog_offset = 0;
    QGLExists_GL_SGIX_fog_offset = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_HP_image_transform;
int QGLNumFunctions_GL_HP_image_transform;
#ifdef QGL_DEFINED_GL_HP_image_transform
pfn_glImageTransformParameteriHP QGLglImageTransformParameteriHP = 0;
pfn_glImageTransformParameterfHP QGLglImageTransformParameterfHP = 0;
pfn_glImageTransformParameterivHP QGLglImageTransformParameterivHP = 0;
pfn_glImageTransformParameterfvHP QGLglImageTransformParameterfvHP = 0;
pfn_glGetImageTransformParameterivHP QGLglGetImageTransformParameterivHP = 0;
pfn_glGetImageTransformParameterfvHP QGLglGetImageTransformParameterfvHP = 0;

static void Init_GL_HP_image_transform ()
{
    QGLNumFunctions_GL_HP_image_transform = 0;

    QGLglImageTransformParameteriHP = (pfn_glImageTransformParameteriHP)GetFunctionPointer("glImageTransformParameteriHP");
    if ( QGLglImageTransformParameteriHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    QGLglImageTransformParameterfHP = (pfn_glImageTransformParameterfHP)GetFunctionPointer("glImageTransformParameterfHP");
    if ( QGLglImageTransformParameterfHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    QGLglImageTransformParameterivHP = (pfn_glImageTransformParameterivHP)GetFunctionPointer("glImageTransformParameterivHP");
    if ( QGLglImageTransformParameterivHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    QGLglImageTransformParameterfvHP = (pfn_glImageTransformParameterfvHP)GetFunctionPointer("glImageTransformParameterfvHP");
    if ( QGLglImageTransformParameterfvHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    QGLglGetImageTransformParameterivHP = (pfn_glGetImageTransformParameterivHP)GetFunctionPointer("glGetImageTransformParameterivHP");
    if ( QGLglGetImageTransformParameterivHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    QGLglGetImageTransformParameterfvHP = (pfn_glGetImageTransformParameterfvHP)GetFunctionPointer("glGetImageTransformParameterfvHP");
    if ( QGLglGetImageTransformParameterfvHP )
        ++QGLNumFunctions_GL_HP_image_transform;

    if ( QGLNumFunctions_GL_HP_image_transform == 6 )
        QGLExists_GL_HP_image_transform = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_HP_image_transform = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_HP_image_transform ()
{
    QGLNumFunctions_GL_HP_image_transform = 0;
    QGLExists_GL_HP_image_transform = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_HP_convolution_border_modes;
int QGLNumFunctions_GL_HP_convolution_border_modes;
#ifdef QGL_DEFINED_GL_HP_convolution_border_modes
static void Init_GL_HP_convolution_border_modes ()
{
    QGLNumFunctions_GL_HP_convolution_border_modes = 0;
    QGLExists_GL_HP_convolution_border_modes = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_HP_convolution_border_modes ()
{
    QGLNumFunctions_GL_HP_convolution_border_modes = 0;
    QGLExists_GL_HP_convolution_border_modes = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INGR_palette_buffer;
int QGLNumFunctions_GL_INGR_palette_buffer;
#ifdef QGL_DEFINED_GL_INGR_palette_buffer
static void Init_GL_INGR_palette_buffer ()
{
    QGLNumFunctions_GL_INGR_palette_buffer = 0;
    QGLExists_GL_INGR_palette_buffer = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_INGR_palette_buffer ()
{
    QGLNumFunctions_GL_INGR_palette_buffer = 0;
    QGLExists_GL_INGR_palette_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_add_env;
int QGLNumFunctions_GL_SGIX_texture_add_env;
#ifdef QGL_DEFINED_GL_SGIX_texture_add_env
static void Init_GL_SGIX_texture_add_env ()
{
    QGLNumFunctions_GL_SGIX_texture_add_env = 0;
    QGLExists_GL_SGIX_texture_add_env = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_add_env ()
{
    QGLNumFunctions_GL_SGIX_texture_add_env = 0;
    QGLExists_GL_SGIX_texture_add_env = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_color_subtable;
int QGLNumFunctions_GL_EXT_color_subtable;
#ifdef QGL_DEFINED_GL_EXT_color_subtable
pfn_glColorSubTableEXT QGLglColorSubTableEXT = 0;
pfn_glCopyColorSubTableEXT QGLglCopyColorSubTableEXT = 0;

static void Init_GL_EXT_color_subtable ()
{
    QGLNumFunctions_GL_EXT_color_subtable = 0;

    QGLglColorSubTableEXT = (pfn_glColorSubTableEXT)GetFunctionPointer("glColorSubTableEXT");
    if ( QGLglColorSubTableEXT )
        ++QGLNumFunctions_GL_EXT_color_subtable;

    QGLglCopyColorSubTableEXT = (pfn_glCopyColorSubTableEXT)GetFunctionPointer("glCopyColorSubTableEXT");
    if ( QGLglCopyColorSubTableEXT )
        ++QGLNumFunctions_GL_EXT_color_subtable;

    if ( QGLNumFunctions_GL_EXT_color_subtable == 2 )
        QGLExists_GL_EXT_color_subtable = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_color_subtable = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_color_subtable ()
{
    QGLNumFunctions_GL_EXT_color_subtable = 0;
    QGLExists_GL_EXT_color_subtable = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_PGI_vertex_hints;
int QGLNumFunctions_GL_PGI_vertex_hints;
#ifdef QGL_DEFINED_GL_PGI_vertex_hints
static void Init_GL_PGI_vertex_hints ()
{
    QGLNumFunctions_GL_PGI_vertex_hints = 0;
    QGLExists_GL_PGI_vertex_hints = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_PGI_vertex_hints ()
{
    QGLNumFunctions_GL_PGI_vertex_hints = 0;
    QGLExists_GL_PGI_vertex_hints = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_PGI_misc_hints;
int QGLNumFunctions_GL_PGI_misc_hints;
#ifdef QGL_DEFINED_GL_PGI_misc_hints
pfn_glHintPGI QGLglHintPGI = 0;

static void Init_GL_PGI_misc_hints ()
{
    QGLNumFunctions_GL_PGI_misc_hints = 0;

    QGLglHintPGI = (pfn_glHintPGI)GetFunctionPointer("glHintPGI");
    if ( QGLglHintPGI )
        ++QGLNumFunctions_GL_PGI_misc_hints;

    if ( QGLNumFunctions_GL_PGI_misc_hints == 1 )
        QGLExists_GL_PGI_misc_hints = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_PGI_misc_hints = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_PGI_misc_hints ()
{
    QGLNumFunctions_GL_PGI_misc_hints = 0;
    QGLExists_GL_PGI_misc_hints = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_paletted_texture;
int QGLNumFunctions_GL_EXT_paletted_texture;
#ifdef QGL_DEFINED_GL_EXT_paletted_texture
pfn_glColorTableEXT QGLglColorTableEXT = 0;
pfn_glGetColorTableEXT QGLglGetColorTableEXT = 0;
pfn_glGetColorTableParameterivEXT QGLglGetColorTableParameterivEXT = 0;
pfn_glGetColorTableParameterfvEXT QGLglGetColorTableParameterfvEXT = 0;

static void Init_GL_EXT_paletted_texture ()
{
    QGLNumFunctions_GL_EXT_paletted_texture = 0;

    QGLglColorTableEXT = (pfn_glColorTableEXT)GetFunctionPointer("glColorTableEXT");
    if ( QGLglColorTableEXT )
        ++QGLNumFunctions_GL_EXT_paletted_texture;

    QGLglGetColorTableEXT = (pfn_glGetColorTableEXT)GetFunctionPointer("glGetColorTableEXT");
    if ( QGLglGetColorTableEXT )
        ++QGLNumFunctions_GL_EXT_paletted_texture;

    QGLglGetColorTableParameterivEXT = (pfn_glGetColorTableParameterivEXT)GetFunctionPointer("glGetColorTableParameterivEXT");
    if ( QGLglGetColorTableParameterivEXT )
        ++QGLNumFunctions_GL_EXT_paletted_texture;

    QGLglGetColorTableParameterfvEXT = (pfn_glGetColorTableParameterfvEXT)GetFunctionPointer("glGetColorTableParameterfvEXT");
    if ( QGLglGetColorTableParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_paletted_texture;

    if ( QGLNumFunctions_GL_EXT_paletted_texture == 4 )
        QGLExists_GL_EXT_paletted_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_paletted_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_paletted_texture ()
{
    QGLNumFunctions_GL_EXT_paletted_texture = 0;
    QGLExists_GL_EXT_paletted_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_clip_volume_hint;
int QGLNumFunctions_GL_EXT_clip_volume_hint;
#ifdef QGL_DEFINED_GL_EXT_clip_volume_hint
static void Init_GL_EXT_clip_volume_hint ()
{
    QGLNumFunctions_GL_EXT_clip_volume_hint = 0;
    QGLExists_GL_EXT_clip_volume_hint = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_clip_volume_hint ()
{
    QGLNumFunctions_GL_EXT_clip_volume_hint = 0;
    QGLExists_GL_EXT_clip_volume_hint = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_list_priority;
int QGLNumFunctions_GL_SGIX_list_priority;
#ifdef QGL_DEFINED_GL_SGIX_list_priority
pfn_glGetListParameterfvSGIX QGLglGetListParameterfvSGIX = 0;
pfn_glGetListParameterivSGIX QGLglGetListParameterivSGIX = 0;
pfn_glListParameterfSGIX QGLglListParameterfSGIX = 0;
pfn_glListParameterfvSGIX QGLglListParameterfvSGIX = 0;
pfn_glListParameteriSGIX QGLglListParameteriSGIX = 0;
pfn_glListParameterivSGIX QGLglListParameterivSGIX = 0;

static void Init_GL_SGIX_list_priority ()
{
    QGLNumFunctions_GL_SGIX_list_priority = 0;

    QGLglGetListParameterfvSGIX = (pfn_glGetListParameterfvSGIX)GetFunctionPointer("glGetListParameterfvSGIX");
    if ( QGLglGetListParameterfvSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    QGLglGetListParameterivSGIX = (pfn_glGetListParameterivSGIX)GetFunctionPointer("glGetListParameterivSGIX");
    if ( QGLglGetListParameterivSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    QGLglListParameterfSGIX = (pfn_glListParameterfSGIX)GetFunctionPointer("glListParameterfSGIX");
    if ( QGLglListParameterfSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    QGLglListParameterfvSGIX = (pfn_glListParameterfvSGIX)GetFunctionPointer("glListParameterfvSGIX");
    if ( QGLglListParameterfvSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    QGLglListParameteriSGIX = (pfn_glListParameteriSGIX)GetFunctionPointer("glListParameteriSGIX");
    if ( QGLglListParameteriSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    QGLglListParameterivSGIX = (pfn_glListParameterivSGIX)GetFunctionPointer("glListParameterivSGIX");
    if ( QGLglListParameterivSGIX )
        ++QGLNumFunctions_GL_SGIX_list_priority;

    if ( QGLNumFunctions_GL_SGIX_list_priority == 6 )
        QGLExists_GL_SGIX_list_priority = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_list_priority = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_list_priority ()
{
    QGLNumFunctions_GL_SGIX_list_priority = 0;
    QGLExists_GL_SGIX_list_priority = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_ir_instrument1;
int QGLNumFunctions_GL_SGIX_ir_instrument1;
#ifdef QGL_DEFINED_GL_SGIX_ir_instrument1
static void Init_GL_SGIX_ir_instrument1 ()
{
    QGLNumFunctions_GL_SGIX_ir_instrument1 = 0;
    QGLExists_GL_SGIX_ir_instrument1 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_ir_instrument1 ()
{
    QGLNumFunctions_GL_SGIX_ir_instrument1 = 0;
    QGLExists_GL_SGIX_ir_instrument1 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_calligraphic_fragment;
int QGLNumFunctions_GL_SGIX_calligraphic_fragment;
#ifdef QGL_DEFINED_GL_SGIX_calligraphic_fragment
static void Init_GL_SGIX_calligraphic_fragment ()
{
    QGLNumFunctions_GL_SGIX_calligraphic_fragment = 0;
    QGLExists_GL_SGIX_calligraphic_fragment = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_calligraphic_fragment ()
{
    QGLNumFunctions_GL_SGIX_calligraphic_fragment = 0;
    QGLExists_GL_SGIX_calligraphic_fragment = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_lod_bias;
int QGLNumFunctions_GL_SGIX_texture_lod_bias;
#ifdef QGL_DEFINED_GL_SGIX_texture_lod_bias
static void Init_GL_SGIX_texture_lod_bias ()
{
    QGLNumFunctions_GL_SGIX_texture_lod_bias = 0;
    QGLExists_GL_SGIX_texture_lod_bias = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_lod_bias ()
{
    QGLNumFunctions_GL_SGIX_texture_lod_bias = 0;
    QGLExists_GL_SGIX_texture_lod_bias = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_shadow_ambient;
int QGLNumFunctions_GL_SGIX_shadow_ambient;
#ifdef QGL_DEFINED_GL_SGIX_shadow_ambient
static void Init_GL_SGIX_shadow_ambient ()
{
    QGLNumFunctions_GL_SGIX_shadow_ambient = 0;
    QGLExists_GL_SGIX_shadow_ambient = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_shadow_ambient ()
{
    QGLNumFunctions_GL_SGIX_shadow_ambient = 0;
    QGLExists_GL_SGIX_shadow_ambient = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_index_texture;
int QGLNumFunctions_GL_EXT_index_texture;
#ifdef QGL_DEFINED_GL_EXT_index_texture
static void Init_GL_EXT_index_texture ()
{
    QGLNumFunctions_GL_EXT_index_texture = 0;
    QGLExists_GL_EXT_index_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_index_texture ()
{
    QGLNumFunctions_GL_EXT_index_texture = 0;
    QGLExists_GL_EXT_index_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_index_material;
int QGLNumFunctions_GL_EXT_index_material;
#ifdef QGL_DEFINED_GL_EXT_index_material
pfn_glIndexMaterialEXT QGLglIndexMaterialEXT = 0;

static void Init_GL_EXT_index_material ()
{
    QGLNumFunctions_GL_EXT_index_material = 0;

    QGLglIndexMaterialEXT = (pfn_glIndexMaterialEXT)GetFunctionPointer("glIndexMaterialEXT");
    if ( QGLglIndexMaterialEXT )
        ++QGLNumFunctions_GL_EXT_index_material;

    if ( QGLNumFunctions_GL_EXT_index_material == 1 )
        QGLExists_GL_EXT_index_material = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_index_material = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_index_material ()
{
    QGLNumFunctions_GL_EXT_index_material = 0;
    QGLExists_GL_EXT_index_material = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_index_func;
int QGLNumFunctions_GL_EXT_index_func;
#ifdef QGL_DEFINED_GL_EXT_index_func
pfn_glIndexFuncEXT QGLglIndexFuncEXT = 0;

static void Init_GL_EXT_index_func ()
{
    QGLNumFunctions_GL_EXT_index_func = 0;

    QGLglIndexFuncEXT = (pfn_glIndexFuncEXT)GetFunctionPointer("glIndexFuncEXT");
    if ( QGLglIndexFuncEXT )
        ++QGLNumFunctions_GL_EXT_index_func;

    if ( QGLNumFunctions_GL_EXT_index_func == 1 )
        QGLExists_GL_EXT_index_func = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_index_func = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_index_func ()
{
    QGLNumFunctions_GL_EXT_index_func = 0;
    QGLExists_GL_EXT_index_func = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_index_array_formats;
int QGLNumFunctions_GL_EXT_index_array_formats;
#ifdef QGL_DEFINED_GL_EXT_index_array_formats
static void Init_GL_EXT_index_array_formats ()
{
    QGLNumFunctions_GL_EXT_index_array_formats = 0;
    QGLExists_GL_EXT_index_array_formats = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_index_array_formats ()
{
    QGLNumFunctions_GL_EXT_index_array_formats = 0;
    QGLExists_GL_EXT_index_array_formats = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_compiled_vertex_array;
int QGLNumFunctions_GL_EXT_compiled_vertex_array;
#ifdef QGL_DEFINED_GL_EXT_compiled_vertex_array
pfn_glLockArraysEXT QGLglLockArraysEXT = 0;
pfn_glUnlockArraysEXT QGLglUnlockArraysEXT = 0;

static void Init_GL_EXT_compiled_vertex_array ()
{
    QGLNumFunctions_GL_EXT_compiled_vertex_array = 0;

    QGLglLockArraysEXT = (pfn_glLockArraysEXT)GetFunctionPointer("glLockArraysEXT");
    if ( QGLglLockArraysEXT )
        ++QGLNumFunctions_GL_EXT_compiled_vertex_array;

    QGLglUnlockArraysEXT = (pfn_glUnlockArraysEXT)GetFunctionPointer("glUnlockArraysEXT");
    if ( QGLglUnlockArraysEXT )
        ++QGLNumFunctions_GL_EXT_compiled_vertex_array;

    if ( QGLNumFunctions_GL_EXT_compiled_vertex_array == 2 )
        QGLExists_GL_EXT_compiled_vertex_array = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_compiled_vertex_array = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_compiled_vertex_array ()
{
    QGLNumFunctions_GL_EXT_compiled_vertex_array = 0;
    QGLExists_GL_EXT_compiled_vertex_array = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_cull_vertex;
int QGLNumFunctions_GL_EXT_cull_vertex;
#ifdef QGL_DEFINED_GL_EXT_cull_vertex
pfn_glCullParameterdvEXT QGLglCullParameterdvEXT = 0;
pfn_glCullParameterfvEXT QGLglCullParameterfvEXT = 0;

static void Init_GL_EXT_cull_vertex ()
{
    QGLNumFunctions_GL_EXT_cull_vertex = 0;

    QGLglCullParameterdvEXT = (pfn_glCullParameterdvEXT)GetFunctionPointer("glCullParameterdvEXT");
    if ( QGLglCullParameterdvEXT )
        ++QGLNumFunctions_GL_EXT_cull_vertex;

    QGLglCullParameterfvEXT = (pfn_glCullParameterfvEXT)GetFunctionPointer("glCullParameterfvEXT");
    if ( QGLglCullParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_cull_vertex;

    if ( QGLNumFunctions_GL_EXT_cull_vertex == 2 )
        QGLExists_GL_EXT_cull_vertex = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_cull_vertex = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_cull_vertex ()
{
    QGLNumFunctions_GL_EXT_cull_vertex = 0;
    QGLExists_GL_EXT_cull_vertex = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_ycrcb;
int QGLNumFunctions_GL_SGIX_ycrcb;
#ifdef QGL_DEFINED_GL_SGIX_ycrcb
static void Init_GL_SGIX_ycrcb ()
{
    QGLNumFunctions_GL_SGIX_ycrcb = 0;
    QGLExists_GL_SGIX_ycrcb = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_ycrcb ()
{
    QGLNumFunctions_GL_SGIX_ycrcb = 0;
    QGLExists_GL_SGIX_ycrcb = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_fragment_lighting;
int QGLNumFunctions_GL_SGIX_fragment_lighting;
#ifdef QGL_DEFINED_GL_SGIX_fragment_lighting
pfn_glFragmentColorMaterialSGIX QGLglFragmentColorMaterialSGIX = 0;
pfn_glFragmentLightfSGIX QGLglFragmentLightfSGIX = 0;
pfn_glFragmentLightfvSGIX QGLglFragmentLightfvSGIX = 0;
pfn_glFragmentLightiSGIX QGLglFragmentLightiSGIX = 0;
pfn_glFragmentLightivSGIX QGLglFragmentLightivSGIX = 0;
pfn_glFragmentLightModelfSGIX QGLglFragmentLightModelfSGIX = 0;
pfn_glFragmentLightModelfvSGIX QGLglFragmentLightModelfvSGIX = 0;
pfn_glFragmentLightModeliSGIX QGLglFragmentLightModeliSGIX = 0;
pfn_glFragmentLightModelivSGIX QGLglFragmentLightModelivSGIX = 0;
pfn_glFragmentMaterialfSGIX QGLglFragmentMaterialfSGIX = 0;
pfn_glFragmentMaterialfvSGIX QGLglFragmentMaterialfvSGIX = 0;
pfn_glFragmentMaterialiSGIX QGLglFragmentMaterialiSGIX = 0;
pfn_glFragmentMaterialivSGIX QGLglFragmentMaterialivSGIX = 0;
pfn_glGetFragmentLightfvSGIX QGLglGetFragmentLightfvSGIX = 0;
pfn_glGetFragmentLightivSGIX QGLglGetFragmentLightivSGIX = 0;
pfn_glGetFragmentMaterialfvSGIX QGLglGetFragmentMaterialfvSGIX = 0;
pfn_glGetFragmentMaterialivSGIX QGLglGetFragmentMaterialivSGIX = 0;
pfn_glLightEnviSGIX QGLglLightEnviSGIX = 0;

static void Init_GL_SGIX_fragment_lighting ()
{
    QGLNumFunctions_GL_SGIX_fragment_lighting = 0;

    QGLglFragmentColorMaterialSGIX = (pfn_glFragmentColorMaterialSGIX)GetFunctionPointer("glFragmentColorMaterialSGIX");
    if ( QGLglFragmentColorMaterialSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightfSGIX = (pfn_glFragmentLightfSGIX)GetFunctionPointer("glFragmentLightfSGIX");
    if ( QGLglFragmentLightfSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightfvSGIX = (pfn_glFragmentLightfvSGIX)GetFunctionPointer("glFragmentLightfvSGIX");
    if ( QGLglFragmentLightfvSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightiSGIX = (pfn_glFragmentLightiSGIX)GetFunctionPointer("glFragmentLightiSGIX");
    if ( QGLglFragmentLightiSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightivSGIX = (pfn_glFragmentLightivSGIX)GetFunctionPointer("glFragmentLightivSGIX");
    if ( QGLglFragmentLightivSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightModelfSGIX = (pfn_glFragmentLightModelfSGIX)GetFunctionPointer("glFragmentLightModelfSGIX");
    if ( QGLglFragmentLightModelfSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightModelfvSGIX = (pfn_glFragmentLightModelfvSGIX)GetFunctionPointer("glFragmentLightModelfvSGIX");
    if ( QGLglFragmentLightModelfvSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightModeliSGIX = (pfn_glFragmentLightModeliSGIX)GetFunctionPointer("glFragmentLightModeliSGIX");
    if ( QGLglFragmentLightModeliSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentLightModelivSGIX = (pfn_glFragmentLightModelivSGIX)GetFunctionPointer("glFragmentLightModelivSGIX");
    if ( QGLglFragmentLightModelivSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentMaterialfSGIX = (pfn_glFragmentMaterialfSGIX)GetFunctionPointer("glFragmentMaterialfSGIX");
    if ( QGLglFragmentMaterialfSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentMaterialfvSGIX = (pfn_glFragmentMaterialfvSGIX)GetFunctionPointer("glFragmentMaterialfvSGIX");
    if ( QGLglFragmentMaterialfvSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentMaterialiSGIX = (pfn_glFragmentMaterialiSGIX)GetFunctionPointer("glFragmentMaterialiSGIX");
    if ( QGLglFragmentMaterialiSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglFragmentMaterialivSGIX = (pfn_glFragmentMaterialivSGIX)GetFunctionPointer("glFragmentMaterialivSGIX");
    if ( QGLglFragmentMaterialivSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglGetFragmentLightfvSGIX = (pfn_glGetFragmentLightfvSGIX)GetFunctionPointer("glGetFragmentLightfvSGIX");
    if ( QGLglGetFragmentLightfvSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglGetFragmentLightivSGIX = (pfn_glGetFragmentLightivSGIX)GetFunctionPointer("glGetFragmentLightivSGIX");
    if ( QGLglGetFragmentLightivSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglGetFragmentMaterialfvSGIX = (pfn_glGetFragmentMaterialfvSGIX)GetFunctionPointer("glGetFragmentMaterialfvSGIX");
    if ( QGLglGetFragmentMaterialfvSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglGetFragmentMaterialivSGIX = (pfn_glGetFragmentMaterialivSGIX)GetFunctionPointer("glGetFragmentMaterialivSGIX");
    if ( QGLglGetFragmentMaterialivSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    QGLglLightEnviSGIX = (pfn_glLightEnviSGIX)GetFunctionPointer("glLightEnviSGIX");
    if ( QGLglLightEnviSGIX )
        ++QGLNumFunctions_GL_SGIX_fragment_lighting;

    if ( QGLNumFunctions_GL_SGIX_fragment_lighting == 18 )
        QGLExists_GL_SGIX_fragment_lighting = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_fragment_lighting = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_fragment_lighting ()
{
    QGLNumFunctions_GL_SGIX_fragment_lighting = 0;
    QGLExists_GL_SGIX_fragment_lighting = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_IBM_rasterpos_clip;
int QGLNumFunctions_GL_IBM_rasterpos_clip;
#ifdef QGL_DEFINED_GL_IBM_rasterpos_clip
static void Init_GL_IBM_rasterpos_clip ()
{
    QGLNumFunctions_GL_IBM_rasterpos_clip = 0;
    QGLExists_GL_IBM_rasterpos_clip = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_IBM_rasterpos_clip ()
{
    QGLNumFunctions_GL_IBM_rasterpos_clip = 0;
    QGLExists_GL_IBM_rasterpos_clip = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_HP_texture_lighting;
int QGLNumFunctions_GL_HP_texture_lighting;
#ifdef QGL_DEFINED_GL_HP_texture_lighting
static void Init_GL_HP_texture_lighting ()
{
    QGLNumFunctions_GL_HP_texture_lighting = 0;
    QGLExists_GL_HP_texture_lighting = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_HP_texture_lighting ()
{
    QGLNumFunctions_GL_HP_texture_lighting = 0;
    QGLExists_GL_HP_texture_lighting = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_draw_range_elements;
int QGLNumFunctions_GL_EXT_draw_range_elements;
#ifdef QGL_DEFINED_GL_EXT_draw_range_elements
pfn_glDrawRangeElementsEXT QGLglDrawRangeElementsEXT = 0;

static void Init_GL_EXT_draw_range_elements ()
{
    QGLNumFunctions_GL_EXT_draw_range_elements = 0;

    QGLglDrawRangeElementsEXT = (pfn_glDrawRangeElementsEXT)GetFunctionPointer("glDrawRangeElementsEXT");
    if ( QGLglDrawRangeElementsEXT )
        ++QGLNumFunctions_GL_EXT_draw_range_elements;

    if ( QGLNumFunctions_GL_EXT_draw_range_elements == 1 )
        QGLExists_GL_EXT_draw_range_elements = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_draw_range_elements = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_draw_range_elements ()
{
    QGLNumFunctions_GL_EXT_draw_range_elements = 0;
    QGLExists_GL_EXT_draw_range_elements = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_WIN_phong_shading;
int QGLNumFunctions_GL_WIN_phong_shading;
#ifdef QGL_DEFINED_GL_WIN_phong_shading
static void Init_GL_WIN_phong_shading ()
{
    QGLNumFunctions_GL_WIN_phong_shading = 0;
    QGLExists_GL_WIN_phong_shading = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_WIN_phong_shading ()
{
    QGLNumFunctions_GL_WIN_phong_shading = 0;
    QGLExists_GL_WIN_phong_shading = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_WIN_specular_fog;
int QGLNumFunctions_GL_WIN_specular_fog;
#ifdef QGL_DEFINED_GL_WIN_specular_fog
static void Init_GL_WIN_specular_fog ()
{
    QGLNumFunctions_GL_WIN_specular_fog = 0;
    QGLExists_GL_WIN_specular_fog = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_WIN_specular_fog ()
{
    QGLNumFunctions_GL_WIN_specular_fog = 0;
    QGLExists_GL_WIN_specular_fog = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_light_texture;
int QGLNumFunctions_GL_EXT_light_texture;
#ifdef QGL_DEFINED_GL_EXT_light_texture
pfn_glApplyTextureEXT QGLglApplyTextureEXT = 0;
pfn_glTextureLightEXT QGLglTextureLightEXT = 0;
pfn_glTextureMaterialEXT QGLglTextureMaterialEXT = 0;

static void Init_GL_EXT_light_texture ()
{
    QGLNumFunctions_GL_EXT_light_texture = 0;

    QGLglApplyTextureEXT = (pfn_glApplyTextureEXT)GetFunctionPointer("glApplyTextureEXT");
    if ( QGLglApplyTextureEXT )
        ++QGLNumFunctions_GL_EXT_light_texture;

    QGLglTextureLightEXT = (pfn_glTextureLightEXT)GetFunctionPointer("glTextureLightEXT");
    if ( QGLglTextureLightEXT )
        ++QGLNumFunctions_GL_EXT_light_texture;

    QGLglTextureMaterialEXT = (pfn_glTextureMaterialEXT)GetFunctionPointer("glTextureMaterialEXT");
    if ( QGLglTextureMaterialEXT )
        ++QGLNumFunctions_GL_EXT_light_texture;

    if ( QGLNumFunctions_GL_EXT_light_texture == 3 )
        QGLExists_GL_EXT_light_texture = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_light_texture = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_light_texture ()
{
    QGLNumFunctions_GL_EXT_light_texture = 0;
    QGLExists_GL_EXT_light_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_blend_alpha_minmax;
int QGLNumFunctions_GL_SGIX_blend_alpha_minmax;
#ifdef QGL_DEFINED_GL_SGIX_blend_alpha_minmax
static void Init_GL_SGIX_blend_alpha_minmax ()
{
    QGLNumFunctions_GL_SGIX_blend_alpha_minmax = 0;
    QGLExists_GL_SGIX_blend_alpha_minmax = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_blend_alpha_minmax ()
{
    QGLNumFunctions_GL_SGIX_blend_alpha_minmax = 0;
    QGLExists_GL_SGIX_blend_alpha_minmax = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_impact_pixel_texture;
int QGLNumFunctions_GL_SGIX_impact_pixel_texture;
#ifdef QGL_DEFINED_GL_SGIX_impact_pixel_texture
static void Init_GL_SGIX_impact_pixel_texture ()
{
    QGLNumFunctions_GL_SGIX_impact_pixel_texture = 0;
    QGLExists_GL_SGIX_impact_pixel_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_impact_pixel_texture ()
{
    QGLNumFunctions_GL_SGIX_impact_pixel_texture = 0;
    QGLExists_GL_SGIX_impact_pixel_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_bgra;
int QGLNumFunctions_GL_EXT_bgra;
#ifdef QGL_DEFINED_GL_EXT_bgra
static void Init_GL_EXT_bgra ()
{
    QGLNumFunctions_GL_EXT_bgra = 0;
    QGLExists_GL_EXT_bgra = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_bgra ()
{
    QGLNumFunctions_GL_EXT_bgra = 0;
    QGLExists_GL_EXT_bgra = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_async;
int QGLNumFunctions_GL_SGIX_async;
#ifdef QGL_DEFINED_GL_SGIX_async
pfn_glAsyncMarkerSGIX QGLglAsyncMarkerSGIX = 0;
pfn_glFinishAsyncSGIX QGLglFinishAsyncSGIX = 0;
pfn_glPollAsyncSGIX QGLglPollAsyncSGIX = 0;
pfn_glGenAsyncMarkersSGIX QGLglGenAsyncMarkersSGIX = 0;
pfn_glDeleteAsyncMarkersSGIX QGLglDeleteAsyncMarkersSGIX = 0;
pfn_glIsAsyncMarkerSGIX QGLglIsAsyncMarkerSGIX = 0;

static void Init_GL_SGIX_async ()
{
    QGLNumFunctions_GL_SGIX_async = 0;

    QGLglAsyncMarkerSGIX = (pfn_glAsyncMarkerSGIX)GetFunctionPointer("glAsyncMarkerSGIX");
    if ( QGLglAsyncMarkerSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    QGLglFinishAsyncSGIX = (pfn_glFinishAsyncSGIX)GetFunctionPointer("glFinishAsyncSGIX");
    if ( QGLglFinishAsyncSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    QGLglPollAsyncSGIX = (pfn_glPollAsyncSGIX)GetFunctionPointer("glPollAsyncSGIX");
    if ( QGLglPollAsyncSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    QGLglGenAsyncMarkersSGIX = (pfn_glGenAsyncMarkersSGIX)GetFunctionPointer("glGenAsyncMarkersSGIX");
    if ( QGLglGenAsyncMarkersSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    QGLglDeleteAsyncMarkersSGIX = (pfn_glDeleteAsyncMarkersSGIX)GetFunctionPointer("glDeleteAsyncMarkersSGIX");
    if ( QGLglDeleteAsyncMarkersSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    QGLglIsAsyncMarkerSGIX = (pfn_glIsAsyncMarkerSGIX)GetFunctionPointer("glIsAsyncMarkerSGIX");
    if ( QGLglIsAsyncMarkerSGIX )
        ++QGLNumFunctions_GL_SGIX_async;

    if ( QGLNumFunctions_GL_SGIX_async == 6 )
        QGLExists_GL_SGIX_async = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_async = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_async ()
{
    QGLNumFunctions_GL_SGIX_async = 0;
    QGLExists_GL_SGIX_async = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_async_pixel;
int QGLNumFunctions_GL_SGIX_async_pixel;
#ifdef QGL_DEFINED_GL_SGIX_async_pixel
static void Init_GL_SGIX_async_pixel ()
{
    QGLNumFunctions_GL_SGIX_async_pixel = 0;
    QGLExists_GL_SGIX_async_pixel = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_async_pixel ()
{
    QGLNumFunctions_GL_SGIX_async_pixel = 0;
    QGLExists_GL_SGIX_async_pixel = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_async_histogram;
int QGLNumFunctions_GL_SGIX_async_histogram;
#ifdef QGL_DEFINED_GL_SGIX_async_histogram
static void Init_GL_SGIX_async_histogram ()
{
    QGLNumFunctions_GL_SGIX_async_histogram = 0;
    QGLExists_GL_SGIX_async_histogram = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_async_histogram ()
{
    QGLNumFunctions_GL_SGIX_async_histogram = 0;
    QGLExists_GL_SGIX_async_histogram = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INTEL_texture_scissor;
int QGLNumFunctions_GL_INTEL_texture_scissor;
#ifdef QGL_DEFINED_GL_INTEL_texture_scissor
static void Init_GL_INTEL_texture_scissor ()
{
    QGLNumFunctions_GL_INTEL_texture_scissor = 0;
    QGLExists_GL_INTEL_texture_scissor = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_INTEL_texture_scissor ()
{
    QGLNumFunctions_GL_INTEL_texture_scissor = 0;
    QGLExists_GL_INTEL_texture_scissor = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INTEL_parallel_arrays;
int QGLNumFunctions_GL_INTEL_parallel_arrays;
#ifdef QGL_DEFINED_GL_INTEL_parallel_arrays
pfn_glVertexPointervINTEL QGLglVertexPointervINTEL = 0;
pfn_glNormalPointervINTEL QGLglNormalPointervINTEL = 0;
pfn_glColorPointervINTEL QGLglColorPointervINTEL = 0;
pfn_glTexCoordPointervINTEL QGLglTexCoordPointervINTEL = 0;

static void Init_GL_INTEL_parallel_arrays ()
{
    QGLNumFunctions_GL_INTEL_parallel_arrays = 0;

    QGLglVertexPointervINTEL = (pfn_glVertexPointervINTEL)GetFunctionPointer("glVertexPointervINTEL");
    if ( QGLglVertexPointervINTEL )
        ++QGLNumFunctions_GL_INTEL_parallel_arrays;

    QGLglNormalPointervINTEL = (pfn_glNormalPointervINTEL)GetFunctionPointer("glNormalPointervINTEL");
    if ( QGLglNormalPointervINTEL )
        ++QGLNumFunctions_GL_INTEL_parallel_arrays;

    QGLglColorPointervINTEL = (pfn_glColorPointervINTEL)GetFunctionPointer("glColorPointervINTEL");
    if ( QGLglColorPointervINTEL )
        ++QGLNumFunctions_GL_INTEL_parallel_arrays;

    QGLglTexCoordPointervINTEL = (pfn_glTexCoordPointervINTEL)GetFunctionPointer("glTexCoordPointervINTEL");
    if ( QGLglTexCoordPointervINTEL )
        ++QGLNumFunctions_GL_INTEL_parallel_arrays;

    if ( QGLNumFunctions_GL_INTEL_parallel_arrays == 4 )
        QGLExists_GL_INTEL_parallel_arrays = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_INTEL_parallel_arrays = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_INTEL_parallel_arrays ()
{
    QGLNumFunctions_GL_INTEL_parallel_arrays = 0;
    QGLExists_GL_INTEL_parallel_arrays = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_HP_occlusion_test;
int QGLNumFunctions_GL_HP_occlusion_test;
#ifdef QGL_DEFINED_GL_HP_occlusion_test
static void Init_GL_HP_occlusion_test ()
{
    QGLNumFunctions_GL_HP_occlusion_test = 0;
    QGLExists_GL_HP_occlusion_test = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_HP_occlusion_test ()
{
    QGLNumFunctions_GL_HP_occlusion_test = 0;
    QGLExists_GL_HP_occlusion_test = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_pixel_transform;
int QGLNumFunctions_GL_EXT_pixel_transform;
#ifdef QGL_DEFINED_GL_EXT_pixel_transform
pfn_glPixelTransformParameteriEXT QGLglPixelTransformParameteriEXT = 0;
pfn_glPixelTransformParameterfEXT QGLglPixelTransformParameterfEXT = 0;
pfn_glPixelTransformParameterivEXT QGLglPixelTransformParameterivEXT = 0;
pfn_glPixelTransformParameterfvEXT QGLglPixelTransformParameterfvEXT = 0;

static void Init_GL_EXT_pixel_transform ()
{
    QGLNumFunctions_GL_EXT_pixel_transform = 0;

    QGLglPixelTransformParameteriEXT = (pfn_glPixelTransformParameteriEXT)GetFunctionPointer("glPixelTransformParameteriEXT");
    if ( QGLglPixelTransformParameteriEXT )
        ++QGLNumFunctions_GL_EXT_pixel_transform;

    QGLglPixelTransformParameterfEXT = (pfn_glPixelTransformParameterfEXT)GetFunctionPointer("glPixelTransformParameterfEXT");
    if ( QGLglPixelTransformParameterfEXT )
        ++QGLNumFunctions_GL_EXT_pixel_transform;

    QGLglPixelTransformParameterivEXT = (pfn_glPixelTransformParameterivEXT)GetFunctionPointer("glPixelTransformParameterivEXT");
    if ( QGLglPixelTransformParameterivEXT )
        ++QGLNumFunctions_GL_EXT_pixel_transform;

    QGLglPixelTransformParameterfvEXT = (pfn_glPixelTransformParameterfvEXT)GetFunctionPointer("glPixelTransformParameterfvEXT");
    if ( QGLglPixelTransformParameterfvEXT )
        ++QGLNumFunctions_GL_EXT_pixel_transform;

    if ( QGLNumFunctions_GL_EXT_pixel_transform == 4 )
        QGLExists_GL_EXT_pixel_transform = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_pixel_transform = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_pixel_transform ()
{
    QGLNumFunctions_GL_EXT_pixel_transform = 0;
    QGLExists_GL_EXT_pixel_transform = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_pixel_transform_color_table;
int QGLNumFunctions_GL_EXT_pixel_transform_color_table;
#ifdef QGL_DEFINED_GL_EXT_pixel_transform_color_table
static void Init_GL_EXT_pixel_transform_color_table ()
{
    QGLNumFunctions_GL_EXT_pixel_transform_color_table = 0;
    QGLExists_GL_EXT_pixel_transform_color_table = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_pixel_transform_color_table ()
{
    QGLNumFunctions_GL_EXT_pixel_transform_color_table = 0;
    QGLExists_GL_EXT_pixel_transform_color_table = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_shared_texture_palette;
int QGLNumFunctions_GL_EXT_shared_texture_palette;
#ifdef QGL_DEFINED_GL_EXT_shared_texture_palette
static void Init_GL_EXT_shared_texture_palette ()
{
    QGLNumFunctions_GL_EXT_shared_texture_palette = 0;
    QGLExists_GL_EXT_shared_texture_palette = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_shared_texture_palette ()
{
    QGLNumFunctions_GL_EXT_shared_texture_palette = 0;
    QGLExists_GL_EXT_shared_texture_palette = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_separate_specular_color;
int QGLNumFunctions_GL_EXT_separate_specular_color;
#ifdef QGL_DEFINED_GL_EXT_separate_specular_color
static void Init_GL_EXT_separate_specular_color ()
{
    QGLNumFunctions_GL_EXT_separate_specular_color = 0;
    QGLExists_GL_EXT_separate_specular_color = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_separate_specular_color ()
{
    QGLNumFunctions_GL_EXT_separate_specular_color = 0;
    QGLExists_GL_EXT_separate_specular_color = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_secondary_color;
int QGLNumFunctions_GL_EXT_secondary_color;
#ifdef QGL_DEFINED_GL_EXT_secondary_color
pfn_glSecondaryColor3bEXT QGLglSecondaryColor3bEXT = 0;
pfn_glSecondaryColor3bvEXT QGLglSecondaryColor3bvEXT = 0;
pfn_glSecondaryColor3dEXT QGLglSecondaryColor3dEXT = 0;
pfn_glSecondaryColor3dvEXT QGLglSecondaryColor3dvEXT = 0;
pfn_glSecondaryColor3fEXT QGLglSecondaryColor3fEXT = 0;
pfn_glSecondaryColor3fvEXT QGLglSecondaryColor3fvEXT = 0;
pfn_glSecondaryColor3iEXT QGLglSecondaryColor3iEXT = 0;
pfn_glSecondaryColor3ivEXT QGLglSecondaryColor3ivEXT = 0;
pfn_glSecondaryColor3sEXT QGLglSecondaryColor3sEXT = 0;
pfn_glSecondaryColor3svEXT QGLglSecondaryColor3svEXT = 0;
pfn_glSecondaryColor3ubEXT QGLglSecondaryColor3ubEXT = 0;
pfn_glSecondaryColor3ubvEXT QGLglSecondaryColor3ubvEXT = 0;
pfn_glSecondaryColor3uiEXT QGLglSecondaryColor3uiEXT = 0;
pfn_glSecondaryColor3uivEXT QGLglSecondaryColor3uivEXT = 0;
pfn_glSecondaryColor3usEXT QGLglSecondaryColor3usEXT = 0;
pfn_glSecondaryColor3usvEXT QGLglSecondaryColor3usvEXT = 0;
pfn_glSecondaryColorPointerEXT QGLglSecondaryColorPointerEXT = 0;

static void Init_GL_EXT_secondary_color ()
{
    QGLNumFunctions_GL_EXT_secondary_color = 0;

    QGLglSecondaryColor3bEXT = (pfn_glSecondaryColor3bEXT)GetFunctionPointer("glSecondaryColor3bEXT");
    if ( QGLglSecondaryColor3bEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3bvEXT = (pfn_glSecondaryColor3bvEXT)GetFunctionPointer("glSecondaryColor3bvEXT");
    if ( QGLglSecondaryColor3bvEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3dEXT = (pfn_glSecondaryColor3dEXT)GetFunctionPointer("glSecondaryColor3dEXT");
    if ( QGLglSecondaryColor3dEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3dvEXT = (pfn_glSecondaryColor3dvEXT)GetFunctionPointer("glSecondaryColor3dvEXT");
    if ( QGLglSecondaryColor3dvEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3fEXT = (pfn_glSecondaryColor3fEXT)GetFunctionPointer("glSecondaryColor3fEXT");
    if ( QGLglSecondaryColor3fEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3fvEXT = (pfn_glSecondaryColor3fvEXT)GetFunctionPointer("glSecondaryColor3fvEXT");
    if ( QGLglSecondaryColor3fvEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3iEXT = (pfn_glSecondaryColor3iEXT)GetFunctionPointer("glSecondaryColor3iEXT");
    if ( QGLglSecondaryColor3iEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3ivEXT = (pfn_glSecondaryColor3ivEXT)GetFunctionPointer("glSecondaryColor3ivEXT");
    if ( QGLglSecondaryColor3ivEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3sEXT = (pfn_glSecondaryColor3sEXT)GetFunctionPointer("glSecondaryColor3sEXT");
    if ( QGLglSecondaryColor3sEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3svEXT = (pfn_glSecondaryColor3svEXT)GetFunctionPointer("glSecondaryColor3svEXT");
    if ( QGLglSecondaryColor3svEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3ubEXT = (pfn_glSecondaryColor3ubEXT)GetFunctionPointer("glSecondaryColor3ubEXT");
    if ( QGLglSecondaryColor3ubEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3ubvEXT = (pfn_glSecondaryColor3ubvEXT)GetFunctionPointer("glSecondaryColor3ubvEXT");
    if ( QGLglSecondaryColor3ubvEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3uiEXT = (pfn_glSecondaryColor3uiEXT)GetFunctionPointer("glSecondaryColor3uiEXT");
    if ( QGLglSecondaryColor3uiEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3uivEXT = (pfn_glSecondaryColor3uivEXT)GetFunctionPointer("glSecondaryColor3uivEXT");
    if ( QGLglSecondaryColor3uivEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3usEXT = (pfn_glSecondaryColor3usEXT)GetFunctionPointer("glSecondaryColor3usEXT");
    if ( QGLglSecondaryColor3usEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColor3usvEXT = (pfn_glSecondaryColor3usvEXT)GetFunctionPointer("glSecondaryColor3usvEXT");
    if ( QGLglSecondaryColor3usvEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    QGLglSecondaryColorPointerEXT = (pfn_glSecondaryColorPointerEXT)GetFunctionPointer("glSecondaryColorPointerEXT");
    if ( QGLglSecondaryColorPointerEXT )
        ++QGLNumFunctions_GL_EXT_secondary_color;

    if ( QGLNumFunctions_GL_EXT_secondary_color == 17 )
        QGLExists_GL_EXT_secondary_color = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_secondary_color = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_secondary_color ()
{
    QGLNumFunctions_GL_EXT_secondary_color = 0;
    QGLExists_GL_EXT_secondary_color = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_perturb_normal;
int QGLNumFunctions_GL_EXT_texture_perturb_normal;
#ifdef QGL_DEFINED_GL_EXT_texture_perturb_normal
pfn_glTextureNormalEXT QGLglTextureNormalEXT = 0;

static void Init_GL_EXT_texture_perturb_normal ()
{
    QGLNumFunctions_GL_EXT_texture_perturb_normal = 0;

    QGLglTextureNormalEXT = (pfn_glTextureNormalEXT)GetFunctionPointer("glTextureNormalEXT");
    if ( QGLglTextureNormalEXT )
        ++QGLNumFunctions_GL_EXT_texture_perturb_normal;

    if ( QGLNumFunctions_GL_EXT_texture_perturb_normal == 1 )
        QGLExists_GL_EXT_texture_perturb_normal = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_texture_perturb_normal = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_perturb_normal ()
{
    QGLNumFunctions_GL_EXT_texture_perturb_normal = 0;
    QGLExists_GL_EXT_texture_perturb_normal = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_multi_draw_arrays;
int QGLNumFunctions_GL_EXT_multi_draw_arrays;
#ifdef QGL_DEFINED_GL_EXT_multi_draw_arrays
pfn_glMultiDrawArraysEXT QGLglMultiDrawArraysEXT = 0;
pfn_glMultiDrawElementsEXT QGLglMultiDrawElementsEXT = 0;

static void Init_GL_EXT_multi_draw_arrays ()
{
    QGLNumFunctions_GL_EXT_multi_draw_arrays = 0;

    QGLglMultiDrawArraysEXT = (pfn_glMultiDrawArraysEXT)GetFunctionPointer("glMultiDrawArraysEXT");
    if ( QGLglMultiDrawArraysEXT )
        ++QGLNumFunctions_GL_EXT_multi_draw_arrays;

    QGLglMultiDrawElementsEXT = (pfn_glMultiDrawElementsEXT)GetFunctionPointer("glMultiDrawElementsEXT");
    if ( QGLglMultiDrawElementsEXT )
        ++QGLNumFunctions_GL_EXT_multi_draw_arrays;

    if ( QGLNumFunctions_GL_EXT_multi_draw_arrays == 2 )
        QGLExists_GL_EXT_multi_draw_arrays = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_multi_draw_arrays = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_multi_draw_arrays ()
{
    QGLNumFunctions_GL_EXT_multi_draw_arrays = 0;
    QGLExists_GL_EXT_multi_draw_arrays = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_fog_coord;
int QGLNumFunctions_GL_EXT_fog_coord;
#ifdef QGL_DEFINED_GL_EXT_fog_coord
pfn_glFogCoordfEXT QGLglFogCoordfEXT = 0;
pfn_glFogCoordfvEXT QGLglFogCoordfvEXT = 0;
pfn_glFogCoorddEXT QGLglFogCoorddEXT = 0;
pfn_glFogCoorddvEXT QGLglFogCoorddvEXT = 0;
pfn_glFogCoordPointerEXT QGLglFogCoordPointerEXT = 0;

static void Init_GL_EXT_fog_coord ()
{
    QGLNumFunctions_GL_EXT_fog_coord = 0;

    QGLglFogCoordfEXT = (pfn_glFogCoordfEXT)GetFunctionPointer("glFogCoordfEXT");
    if ( QGLglFogCoordfEXT )
        ++QGLNumFunctions_GL_EXT_fog_coord;

    QGLglFogCoordfvEXT = (pfn_glFogCoordfvEXT)GetFunctionPointer("glFogCoordfvEXT");
    if ( QGLglFogCoordfvEXT )
        ++QGLNumFunctions_GL_EXT_fog_coord;

    QGLglFogCoorddEXT = (pfn_glFogCoorddEXT)GetFunctionPointer("glFogCoorddEXT");
    if ( QGLglFogCoorddEXT )
        ++QGLNumFunctions_GL_EXT_fog_coord;

    QGLglFogCoorddvEXT = (pfn_glFogCoorddvEXT)GetFunctionPointer("glFogCoorddvEXT");
    if ( QGLglFogCoorddvEXT )
        ++QGLNumFunctions_GL_EXT_fog_coord;

    QGLglFogCoordPointerEXT = (pfn_glFogCoordPointerEXT)GetFunctionPointer("glFogCoordPointerEXT");
    if ( QGLglFogCoordPointerEXT )
        ++QGLNumFunctions_GL_EXT_fog_coord;

    if ( QGLNumFunctions_GL_EXT_fog_coord == 5 )
        QGLExists_GL_EXT_fog_coord = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_fog_coord = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_fog_coord ()
{
    QGLNumFunctions_GL_EXT_fog_coord = 0;
    QGLExists_GL_EXT_fog_coord = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_REND_screen_coordinates;
int QGLNumFunctions_GL_REND_screen_coordinates;
#ifdef QGL_DEFINED_GL_REND_screen_coordinates
static void Init_GL_REND_screen_coordinates ()
{
    QGLNumFunctions_GL_REND_screen_coordinates = 0;
    QGLExists_GL_REND_screen_coordinates = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_REND_screen_coordinates ()
{
    QGLNumFunctions_GL_REND_screen_coordinates = 0;
    QGLExists_GL_REND_screen_coordinates = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_coordinate_frame;
int QGLNumFunctions_GL_EXT_coordinate_frame;
#ifdef QGL_DEFINED_GL_EXT_coordinate_frame
pfn_glTangent3bEXT QGLglTangent3bEXT = 0;
pfn_glTangent3bvEXT QGLglTangent3bvEXT = 0;
pfn_glTangent3dEXT QGLglTangent3dEXT = 0;
pfn_glTangent3dvEXT QGLglTangent3dvEXT = 0;
pfn_glTangent3fEXT QGLglTangent3fEXT = 0;
pfn_glTangent3fvEXT QGLglTangent3fvEXT = 0;
pfn_glTangent3iEXT QGLglTangent3iEXT = 0;
pfn_glTangent3ivEXT QGLglTangent3ivEXT = 0;
pfn_glTangent3sEXT QGLglTangent3sEXT = 0;
pfn_glTangent3svEXT QGLglTangent3svEXT = 0;
pfn_glBinormal3bEXT QGLglBinormal3bEXT = 0;
pfn_glBinormal3bvEXT QGLglBinormal3bvEXT = 0;
pfn_glBinormal3dEXT QGLglBinormal3dEXT = 0;
pfn_glBinormal3dvEXT QGLglBinormal3dvEXT = 0;
pfn_glBinormal3fEXT QGLglBinormal3fEXT = 0;
pfn_glBinormal3fvEXT QGLglBinormal3fvEXT = 0;
pfn_glBinormal3iEXT QGLglBinormal3iEXT = 0;
pfn_glBinormal3ivEXT QGLglBinormal3ivEXT = 0;
pfn_glBinormal3sEXT QGLglBinormal3sEXT = 0;
pfn_glBinormal3svEXT QGLglBinormal3svEXT = 0;
pfn_glTangentPointerEXT QGLglTangentPointerEXT = 0;
pfn_glBinormalPointerEXT QGLglBinormalPointerEXT = 0;

static void Init_GL_EXT_coordinate_frame ()
{
    QGLNumFunctions_GL_EXT_coordinate_frame = 0;

    QGLglTangent3bEXT = (pfn_glTangent3bEXT)GetFunctionPointer("glTangent3bEXT");
    if ( QGLglTangent3bEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3bvEXT = (pfn_glTangent3bvEXT)GetFunctionPointer("glTangent3bvEXT");
    if ( QGLglTangent3bvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3dEXT = (pfn_glTangent3dEXT)GetFunctionPointer("glTangent3dEXT");
    if ( QGLglTangent3dEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3dvEXT = (pfn_glTangent3dvEXT)GetFunctionPointer("glTangent3dvEXT");
    if ( QGLglTangent3dvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3fEXT = (pfn_glTangent3fEXT)GetFunctionPointer("glTangent3fEXT");
    if ( QGLglTangent3fEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3fvEXT = (pfn_glTangent3fvEXT)GetFunctionPointer("glTangent3fvEXT");
    if ( QGLglTangent3fvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3iEXT = (pfn_glTangent3iEXT)GetFunctionPointer("glTangent3iEXT");
    if ( QGLglTangent3iEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3ivEXT = (pfn_glTangent3ivEXT)GetFunctionPointer("glTangent3ivEXT");
    if ( QGLglTangent3ivEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3sEXT = (pfn_glTangent3sEXT)GetFunctionPointer("glTangent3sEXT");
    if ( QGLglTangent3sEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangent3svEXT = (pfn_glTangent3svEXT)GetFunctionPointer("glTangent3svEXT");
    if ( QGLglTangent3svEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3bEXT = (pfn_glBinormal3bEXT)GetFunctionPointer("glBinormal3bEXT");
    if ( QGLglBinormal3bEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3bvEXT = (pfn_glBinormal3bvEXT)GetFunctionPointer("glBinormal3bvEXT");
    if ( QGLglBinormal3bvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3dEXT = (pfn_glBinormal3dEXT)GetFunctionPointer("glBinormal3dEXT");
    if ( QGLglBinormal3dEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3dvEXT = (pfn_glBinormal3dvEXT)GetFunctionPointer("glBinormal3dvEXT");
    if ( QGLglBinormal3dvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3fEXT = (pfn_glBinormal3fEXT)GetFunctionPointer("glBinormal3fEXT");
    if ( QGLglBinormal3fEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3fvEXT = (pfn_glBinormal3fvEXT)GetFunctionPointer("glBinormal3fvEXT");
    if ( QGLglBinormal3fvEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3iEXT = (pfn_glBinormal3iEXT)GetFunctionPointer("glBinormal3iEXT");
    if ( QGLglBinormal3iEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3ivEXT = (pfn_glBinormal3ivEXT)GetFunctionPointer("glBinormal3ivEXT");
    if ( QGLglBinormal3ivEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3sEXT = (pfn_glBinormal3sEXT)GetFunctionPointer("glBinormal3sEXT");
    if ( QGLglBinormal3sEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormal3svEXT = (pfn_glBinormal3svEXT)GetFunctionPointer("glBinormal3svEXT");
    if ( QGLglBinormal3svEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglTangentPointerEXT = (pfn_glTangentPointerEXT)GetFunctionPointer("glTangentPointerEXT");
    if ( QGLglTangentPointerEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    QGLglBinormalPointerEXT = (pfn_glBinormalPointerEXT)GetFunctionPointer("glBinormalPointerEXT");
    if ( QGLglBinormalPointerEXT )
        ++QGLNumFunctions_GL_EXT_coordinate_frame;

    if ( QGLNumFunctions_GL_EXT_coordinate_frame == 22 )
        QGLExists_GL_EXT_coordinate_frame = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_coordinate_frame = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_coordinate_frame ()
{
    QGLNumFunctions_GL_EXT_coordinate_frame = 0;
    QGLExists_GL_EXT_coordinate_frame = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_env_combine;
int QGLNumFunctions_GL_EXT_texture_env_combine;
#ifdef QGL_DEFINED_GL_EXT_texture_env_combine
static void Init_GL_EXT_texture_env_combine ()
{
    QGLNumFunctions_GL_EXT_texture_env_combine = 0;
    QGLExists_GL_EXT_texture_env_combine = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_env_combine ()
{
    QGLNumFunctions_GL_EXT_texture_env_combine = 0;
    QGLExists_GL_EXT_texture_env_combine = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_specular_vector;
int QGLNumFunctions_GL_APPLE_specular_vector;
#ifdef QGL_DEFINED_GL_APPLE_specular_vector
static void Init_GL_APPLE_specular_vector ()
{
    QGLNumFunctions_GL_APPLE_specular_vector = 0;
    QGLExists_GL_APPLE_specular_vector = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_APPLE_specular_vector ()
{
    QGLNumFunctions_GL_APPLE_specular_vector = 0;
    QGLExists_GL_APPLE_specular_vector = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_transform_hint;
int QGLNumFunctions_GL_APPLE_transform_hint;
#ifdef QGL_DEFINED_GL_APPLE_transform_hint
static void Init_GL_APPLE_transform_hint ()
{
    QGLNumFunctions_GL_APPLE_transform_hint = 0;
    QGLExists_GL_APPLE_transform_hint = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_APPLE_transform_hint ()
{
    QGLNumFunctions_GL_APPLE_transform_hint = 0;
    QGLExists_GL_APPLE_transform_hint = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_fog_scale;
int QGLNumFunctions_GL_SGIX_fog_scale;
#ifdef QGL_DEFINED_GL_SGIX_fog_scale
static void Init_GL_SGIX_fog_scale ()
{
    QGLNumFunctions_GL_SGIX_fog_scale = 0;
    QGLExists_GL_SGIX_fog_scale = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_fog_scale ()
{
    QGLNumFunctions_GL_SGIX_fog_scale = 0;
    QGLExists_GL_SGIX_fog_scale = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUNX_constant_data;
int QGLNumFunctions_GL_SUNX_constant_data;
#ifdef QGL_DEFINED_GL_SUNX_constant_data
pfn_glFinishTextureSUNX QGLglFinishTextureSUNX = 0;

static void Init_GL_SUNX_constant_data ()
{
    QGLNumFunctions_GL_SUNX_constant_data = 0;

    QGLglFinishTextureSUNX = (pfn_glFinishTextureSUNX)GetFunctionPointer("glFinishTextureSUNX");
    if ( QGLglFinishTextureSUNX )
        ++QGLNumFunctions_GL_SUNX_constant_data;

    if ( QGLNumFunctions_GL_SUNX_constant_data == 1 )
        QGLExists_GL_SUNX_constant_data = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SUNX_constant_data = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SUNX_constant_data ()
{
    QGLNumFunctions_GL_SUNX_constant_data = 0;
    QGLExists_GL_SUNX_constant_data = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_global_alpha;
int QGLNumFunctions_GL_SUN_global_alpha;
#ifdef QGL_DEFINED_GL_SUN_global_alpha
pfn_glGlobalAlphaFactorbSUN QGLglGlobalAlphaFactorbSUN = 0;
pfn_glGlobalAlphaFactorsSUN QGLglGlobalAlphaFactorsSUN = 0;
pfn_glGlobalAlphaFactoriSUN QGLglGlobalAlphaFactoriSUN = 0;
pfn_glGlobalAlphaFactorfSUN QGLglGlobalAlphaFactorfSUN = 0;
pfn_glGlobalAlphaFactordSUN QGLglGlobalAlphaFactordSUN = 0;
pfn_glGlobalAlphaFactorubSUN QGLglGlobalAlphaFactorubSUN = 0;
pfn_glGlobalAlphaFactorusSUN QGLglGlobalAlphaFactorusSUN = 0;
pfn_glGlobalAlphaFactoruiSUN QGLglGlobalAlphaFactoruiSUN = 0;

static void Init_GL_SUN_global_alpha ()
{
    QGLNumFunctions_GL_SUN_global_alpha = 0;

    QGLglGlobalAlphaFactorbSUN = (pfn_glGlobalAlphaFactorbSUN)GetFunctionPointer("glGlobalAlphaFactorbSUN");
    if ( QGLglGlobalAlphaFactorbSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactorsSUN = (pfn_glGlobalAlphaFactorsSUN)GetFunctionPointer("glGlobalAlphaFactorsSUN");
    if ( QGLglGlobalAlphaFactorsSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactoriSUN = (pfn_glGlobalAlphaFactoriSUN)GetFunctionPointer("glGlobalAlphaFactoriSUN");
    if ( QGLglGlobalAlphaFactoriSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactorfSUN = (pfn_glGlobalAlphaFactorfSUN)GetFunctionPointer("glGlobalAlphaFactorfSUN");
    if ( QGLglGlobalAlphaFactorfSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactordSUN = (pfn_glGlobalAlphaFactordSUN)GetFunctionPointer("glGlobalAlphaFactordSUN");
    if ( QGLglGlobalAlphaFactordSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactorubSUN = (pfn_glGlobalAlphaFactorubSUN)GetFunctionPointer("glGlobalAlphaFactorubSUN");
    if ( QGLglGlobalAlphaFactorubSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactorusSUN = (pfn_glGlobalAlphaFactorusSUN)GetFunctionPointer("glGlobalAlphaFactorusSUN");
    if ( QGLglGlobalAlphaFactorusSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    QGLglGlobalAlphaFactoruiSUN = (pfn_glGlobalAlphaFactoruiSUN)GetFunctionPointer("glGlobalAlphaFactoruiSUN");
    if ( QGLglGlobalAlphaFactoruiSUN )
        ++QGLNumFunctions_GL_SUN_global_alpha;

    if ( QGLNumFunctions_GL_SUN_global_alpha == 8 )
        QGLExists_GL_SUN_global_alpha = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SUN_global_alpha = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SUN_global_alpha ()
{
    QGLNumFunctions_GL_SUN_global_alpha = 0;
    QGLExists_GL_SUN_global_alpha = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_triangle_list;
int QGLNumFunctions_GL_SUN_triangle_list;
#ifdef QGL_DEFINED_GL_SUN_triangle_list
pfn_glReplacementCodeuiSUN QGLglReplacementCodeuiSUN = 0;
pfn_glReplacementCodeusSUN QGLglReplacementCodeusSUN = 0;
pfn_glReplacementCodeubSUN QGLglReplacementCodeubSUN = 0;
pfn_glReplacementCodeuivSUN QGLglReplacementCodeuivSUN = 0;
pfn_glReplacementCodeusvSUN QGLglReplacementCodeusvSUN = 0;
pfn_glReplacementCodeubvSUN QGLglReplacementCodeubvSUN = 0;
pfn_glReplacementCodePointerSUN QGLglReplacementCodePointerSUN = 0;

static void Init_GL_SUN_triangle_list ()
{
    QGLNumFunctions_GL_SUN_triangle_list = 0;

    QGLglReplacementCodeuiSUN = (pfn_glReplacementCodeuiSUN)GetFunctionPointer("glReplacementCodeuiSUN");
    if ( QGLglReplacementCodeuiSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodeusSUN = (pfn_glReplacementCodeusSUN)GetFunctionPointer("glReplacementCodeusSUN");
    if ( QGLglReplacementCodeusSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodeubSUN = (pfn_glReplacementCodeubSUN)GetFunctionPointer("glReplacementCodeubSUN");
    if ( QGLglReplacementCodeubSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodeuivSUN = (pfn_glReplacementCodeuivSUN)GetFunctionPointer("glReplacementCodeuivSUN");
    if ( QGLglReplacementCodeuivSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodeusvSUN = (pfn_glReplacementCodeusvSUN)GetFunctionPointer("glReplacementCodeusvSUN");
    if ( QGLglReplacementCodeusvSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodeubvSUN = (pfn_glReplacementCodeubvSUN)GetFunctionPointer("glReplacementCodeubvSUN");
    if ( QGLglReplacementCodeubvSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    QGLglReplacementCodePointerSUN = (pfn_glReplacementCodePointerSUN)GetFunctionPointer("glReplacementCodePointerSUN");
    if ( QGLglReplacementCodePointerSUN )
        ++QGLNumFunctions_GL_SUN_triangle_list;

    if ( QGLNumFunctions_GL_SUN_triangle_list == 7 )
        QGLExists_GL_SUN_triangle_list = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SUN_triangle_list = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SUN_triangle_list ()
{
    QGLNumFunctions_GL_SUN_triangle_list = 0;
    QGLExists_GL_SUN_triangle_list = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_vertex;
int QGLNumFunctions_GL_SUN_vertex;
#ifdef QGL_DEFINED_GL_SUN_vertex
pfn_glColor4ubVertex2fSUN QGLglColor4ubVertex2fSUN = 0;
pfn_glColor4ubVertex2fvSUN QGLglColor4ubVertex2fvSUN = 0;
pfn_glColor4ubVertex3fSUN QGLglColor4ubVertex3fSUN = 0;
pfn_glColor4ubVertex3fvSUN QGLglColor4ubVertex3fvSUN = 0;
pfn_glColor3fVertex3fSUN QGLglColor3fVertex3fSUN = 0;
pfn_glColor3fVertex3fvSUN QGLglColor3fVertex3fvSUN = 0;
pfn_glNormal3fVertex3fSUN QGLglNormal3fVertex3fSUN = 0;
pfn_glNormal3fVertex3fvSUN QGLglNormal3fVertex3fvSUN = 0;
pfn_glColor4fNormal3fVertex3fSUN QGLglColor4fNormal3fVertex3fSUN = 0;
pfn_glColor4fNormal3fVertex3fvSUN QGLglColor4fNormal3fVertex3fvSUN = 0;
pfn_glTexCoord2fVertex3fSUN QGLglTexCoord2fVertex3fSUN = 0;
pfn_glTexCoord2fVertex3fvSUN QGLglTexCoord2fVertex3fvSUN = 0;
pfn_glTexCoord4fVertex4fSUN QGLglTexCoord4fVertex4fSUN = 0;
pfn_glTexCoord4fVertex4fvSUN QGLglTexCoord4fVertex4fvSUN = 0;
pfn_glTexCoord2fColor4ubVertex3fSUN QGLglTexCoord2fColor4ubVertex3fSUN = 0;
pfn_glTexCoord2fColor4ubVertex3fvSUN QGLglTexCoord2fColor4ubVertex3fvSUN = 0;
pfn_glTexCoord2fColor3fVertex3fSUN QGLglTexCoord2fColor3fVertex3fSUN = 0;
pfn_glTexCoord2fColor3fVertex3fvSUN QGLglTexCoord2fColor3fVertex3fvSUN = 0;
pfn_glTexCoord2fNormal3fVertex3fSUN QGLglTexCoord2fNormal3fVertex3fSUN = 0;
pfn_glTexCoord2fNormal3fVertex3fvSUN QGLglTexCoord2fNormal3fVertex3fvSUN = 0;
pfn_glTexCoord2fColor4fNormal3fVertex3fSUN QGLglTexCoord2fColor4fNormal3fVertex3fSUN = 0;
pfn_glTexCoord2fColor4fNormal3fVertex3fvSUN QGLglTexCoord2fColor4fNormal3fVertex3fvSUN = 0;
pfn_glTexCoord4fColor4fNormal3fVertex4fSUN QGLglTexCoord4fColor4fNormal3fVertex4fSUN = 0;
pfn_glTexCoord4fColor4fNormal3fVertex4fvSUN QGLglTexCoord4fColor4fNormal3fVertex4fvSUN = 0;
pfn_glReplacementCodeuiVertex3fSUN QGLglReplacementCodeuiVertex3fSUN = 0;
pfn_glReplacementCodeuiVertex3fvSUN QGLglReplacementCodeuiVertex3fvSUN = 0;
pfn_glReplacementCodeuiColor4ubVertex3fSUN QGLglReplacementCodeuiColor4ubVertex3fSUN = 0;
pfn_glReplacementCodeuiColor4ubVertex3fvSUN QGLglReplacementCodeuiColor4ubVertex3fvSUN = 0;
pfn_glReplacementCodeuiColor3fVertex3fSUN QGLglReplacementCodeuiColor3fVertex3fSUN = 0;
pfn_glReplacementCodeuiColor3fVertex3fvSUN QGLglReplacementCodeuiColor3fVertex3fvSUN = 0;
pfn_glReplacementCodeuiNormal3fVertex3fSUN QGLglReplacementCodeuiNormal3fVertex3fSUN = 0;
pfn_glReplacementCodeuiNormal3fVertex3fvSUN QGLglReplacementCodeuiNormal3fVertex3fvSUN = 0;
pfn_glReplacementCodeuiColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fSUN = 0;
pfn_glReplacementCodeuiColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiColor4fNormal3fVertex3fvSUN = 0;
pfn_glReplacementCodeuiTexCoord2fVertex3fSUN QGLglReplacementCodeuiTexCoord2fVertex3fSUN = 0;
pfn_glReplacementCodeuiTexCoord2fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fVertex3fvSUN = 0;
pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = 0;
pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = 0;
pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = 0;
pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = 0;

static void Init_GL_SUN_vertex ()
{
    QGLNumFunctions_GL_SUN_vertex = 0;

    QGLglColor4ubVertex2fSUN = (pfn_glColor4ubVertex2fSUN)GetFunctionPointer("glColor4ubVertex2fSUN");
    if ( QGLglColor4ubVertex2fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor4ubVertex2fvSUN = (pfn_glColor4ubVertex2fvSUN)GetFunctionPointer("glColor4ubVertex2fvSUN");
    if ( QGLglColor4ubVertex2fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor4ubVertex3fSUN = (pfn_glColor4ubVertex3fSUN)GetFunctionPointer("glColor4ubVertex3fSUN");
    if ( QGLglColor4ubVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor4ubVertex3fvSUN = (pfn_glColor4ubVertex3fvSUN)GetFunctionPointer("glColor4ubVertex3fvSUN");
    if ( QGLglColor4ubVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor3fVertex3fSUN = (pfn_glColor3fVertex3fSUN)GetFunctionPointer("glColor3fVertex3fSUN");
    if ( QGLglColor3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor3fVertex3fvSUN = (pfn_glColor3fVertex3fvSUN)GetFunctionPointer("glColor3fVertex3fvSUN");
    if ( QGLglColor3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglNormal3fVertex3fSUN = (pfn_glNormal3fVertex3fSUN)GetFunctionPointer("glNormal3fVertex3fSUN");
    if ( QGLglNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglNormal3fVertex3fvSUN = (pfn_glNormal3fVertex3fvSUN)GetFunctionPointer("glNormal3fVertex3fvSUN");
    if ( QGLglNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor4fNormal3fVertex3fSUN = (pfn_glColor4fNormal3fVertex3fSUN)GetFunctionPointer("glColor4fNormal3fVertex3fSUN");
    if ( QGLglColor4fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglColor4fNormal3fVertex3fvSUN = (pfn_glColor4fNormal3fVertex3fvSUN)GetFunctionPointer("glColor4fNormal3fVertex3fvSUN");
    if ( QGLglColor4fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fVertex3fSUN = (pfn_glTexCoord2fVertex3fSUN)GetFunctionPointer("glTexCoord2fVertex3fSUN");
    if ( QGLglTexCoord2fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fVertex3fvSUN = (pfn_glTexCoord2fVertex3fvSUN)GetFunctionPointer("glTexCoord2fVertex3fvSUN");
    if ( QGLglTexCoord2fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord4fVertex4fSUN = (pfn_glTexCoord4fVertex4fSUN)GetFunctionPointer("glTexCoord4fVertex4fSUN");
    if ( QGLglTexCoord4fVertex4fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord4fVertex4fvSUN = (pfn_glTexCoord4fVertex4fvSUN)GetFunctionPointer("glTexCoord4fVertex4fvSUN");
    if ( QGLglTexCoord4fVertex4fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor4ubVertex3fSUN = (pfn_glTexCoord2fColor4ubVertex3fSUN)GetFunctionPointer("glTexCoord2fColor4ubVertex3fSUN");
    if ( QGLglTexCoord2fColor4ubVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor4ubVertex3fvSUN = (pfn_glTexCoord2fColor4ubVertex3fvSUN)GetFunctionPointer("glTexCoord2fColor4ubVertex3fvSUN");
    if ( QGLglTexCoord2fColor4ubVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor3fVertex3fSUN = (pfn_glTexCoord2fColor3fVertex3fSUN)GetFunctionPointer("glTexCoord2fColor3fVertex3fSUN");
    if ( QGLglTexCoord2fColor3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor3fVertex3fvSUN = (pfn_glTexCoord2fColor3fVertex3fvSUN)GetFunctionPointer("glTexCoord2fColor3fVertex3fvSUN");
    if ( QGLglTexCoord2fColor3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fNormal3fVertex3fSUN = (pfn_glTexCoord2fNormal3fVertex3fSUN)GetFunctionPointer("glTexCoord2fNormal3fVertex3fSUN");
    if ( QGLglTexCoord2fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fNormal3fVertex3fvSUN = (pfn_glTexCoord2fNormal3fVertex3fvSUN)GetFunctionPointer("glTexCoord2fNormal3fVertex3fvSUN");
    if ( QGLglTexCoord2fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor4fNormal3fVertex3fSUN = (pfn_glTexCoord2fColor4fNormal3fVertex3fSUN)GetFunctionPointer("glTexCoord2fColor4fNormal3fVertex3fSUN");
    if ( QGLglTexCoord2fColor4fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord2fColor4fNormal3fVertex3fvSUN = (pfn_glTexCoord2fColor4fNormal3fVertex3fvSUN)GetFunctionPointer("glTexCoord2fColor4fNormal3fVertex3fvSUN");
    if ( QGLglTexCoord2fColor4fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord4fColor4fNormal3fVertex4fSUN = (pfn_glTexCoord4fColor4fNormal3fVertex4fSUN)GetFunctionPointer("glTexCoord4fColor4fNormal3fVertex4fSUN");
    if ( QGLglTexCoord4fColor4fNormal3fVertex4fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglTexCoord4fColor4fNormal3fVertex4fvSUN = (pfn_glTexCoord4fColor4fNormal3fVertex4fvSUN)GetFunctionPointer("glTexCoord4fColor4fNormal3fVertex4fvSUN");
    if ( QGLglTexCoord4fColor4fNormal3fVertex4fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiVertex3fSUN = (pfn_glReplacementCodeuiVertex3fSUN)GetFunctionPointer("glReplacementCodeuiVertex3fSUN");
    if ( QGLglReplacementCodeuiVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiVertex3fvSUN = (pfn_glReplacementCodeuiVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiVertex3fvSUN");
    if ( QGLglReplacementCodeuiVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor4ubVertex3fSUN = (pfn_glReplacementCodeuiColor4ubVertex3fSUN)GetFunctionPointer("glReplacementCodeuiColor4ubVertex3fSUN");
    if ( QGLglReplacementCodeuiColor4ubVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor4ubVertex3fvSUN = (pfn_glReplacementCodeuiColor4ubVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiColor4ubVertex3fvSUN");
    if ( QGLglReplacementCodeuiColor4ubVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor3fVertex3fSUN = (pfn_glReplacementCodeuiColor3fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiColor3fVertex3fSUN");
    if ( QGLglReplacementCodeuiColor3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor3fVertex3fvSUN = (pfn_glReplacementCodeuiColor3fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiColor3fVertex3fvSUN");
    if ( QGLglReplacementCodeuiColor3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiNormal3fVertex3fSUN = (pfn_glReplacementCodeuiNormal3fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiNormal3fVertex3fSUN");
    if ( QGLglReplacementCodeuiNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiNormal3fVertex3fvSUN = (pfn_glReplacementCodeuiNormal3fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiNormal3fVertex3fvSUN");
    if ( QGLglReplacementCodeuiNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor4fNormal3fVertex3fSUN = (pfn_glReplacementCodeuiColor4fNormal3fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiColor4fNormal3fVertex3fSUN");
    if ( QGLglReplacementCodeuiColor4fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiColor4fNormal3fVertex3fvSUN = (pfn_glReplacementCodeuiColor4fNormal3fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiColor4fNormal3fVertex3fvSUN");
    if ( QGLglReplacementCodeuiColor4fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fVertex3fSUN = (pfn_glReplacementCodeuiTexCoord2fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fVertex3fSUN");
    if ( QGLglReplacementCodeuiTexCoord2fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fVertex3fvSUN = (pfn_glReplacementCodeuiTexCoord2fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fVertex3fvSUN");
    if ( QGLglReplacementCodeuiTexCoord2fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = (pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN");
    if ( QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = (pfn_glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN");
    if ( QGLglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = (pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN");
    if ( QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = (pfn_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)GetFunctionPointer("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN");
    if ( QGLglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN )
        ++QGLNumFunctions_GL_SUN_vertex;

    if ( QGLNumFunctions_GL_SUN_vertex == 40 )
        QGLExists_GL_SUN_vertex = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SUN_vertex = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SUN_vertex ()
{
    QGLNumFunctions_GL_SUN_vertex = 0;
    QGLExists_GL_SUN_vertex = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_func_separate;
int QGLNumFunctions_GL_EXT_blend_func_separate;
#ifdef QGL_DEFINED_GL_EXT_blend_func_separate
pfn_glBlendFuncSeparateEXT QGLglBlendFuncSeparateEXT = 0;

static void Init_GL_EXT_blend_func_separate ()
{
    QGLNumFunctions_GL_EXT_blend_func_separate = 0;

    QGLglBlendFuncSeparateEXT = (pfn_glBlendFuncSeparateEXT)GetFunctionPointer("glBlendFuncSeparateEXT");
    if ( QGLglBlendFuncSeparateEXT )
        ++QGLNumFunctions_GL_EXT_blend_func_separate;

    if ( QGLNumFunctions_GL_EXT_blend_func_separate == 1 )
        QGLExists_GL_EXT_blend_func_separate = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_blend_func_separate = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_func_separate ()
{
    QGLNumFunctions_GL_EXT_blend_func_separate = 0;
    QGLExists_GL_EXT_blend_func_separate = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INGR_blend_func_separate;
int QGLNumFunctions_GL_INGR_blend_func_separate;
#ifdef QGL_DEFINED_GL_INGR_blend_func_separate
pfn_glBlendFuncSeparateINGR QGLglBlendFuncSeparateINGR = 0;

static void Init_GL_INGR_blend_func_separate ()
{
    QGLNumFunctions_GL_INGR_blend_func_separate = 0;

    QGLglBlendFuncSeparateINGR = (pfn_glBlendFuncSeparateINGR)GetFunctionPointer("glBlendFuncSeparateINGR");
    if ( QGLglBlendFuncSeparateINGR )
        ++QGLNumFunctions_GL_INGR_blend_func_separate;

    if ( QGLNumFunctions_GL_INGR_blend_func_separate == 1 )
        QGLExists_GL_INGR_blend_func_separate = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_INGR_blend_func_separate = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_INGR_blend_func_separate ()
{
    QGLNumFunctions_GL_INGR_blend_func_separate = 0;
    QGLExists_GL_INGR_blend_func_separate = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INGR_color_clamp;
int QGLNumFunctions_GL_INGR_color_clamp;
#ifdef QGL_DEFINED_GL_INGR_color_clamp
static void Init_GL_INGR_color_clamp ()
{
    QGLNumFunctions_GL_INGR_color_clamp = 0;
    QGLExists_GL_INGR_color_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_INGR_color_clamp ()
{
    QGLNumFunctions_GL_INGR_color_clamp = 0;
    QGLExists_GL_INGR_color_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_INGR_interlace_read;
int QGLNumFunctions_GL_INGR_interlace_read;
#ifdef QGL_DEFINED_GL_INGR_interlace_read
static void Init_GL_INGR_interlace_read ()
{
    QGLNumFunctions_GL_INGR_interlace_read = 0;
    QGLExists_GL_INGR_interlace_read = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_INGR_interlace_read ()
{
    QGLNumFunctions_GL_INGR_interlace_read = 0;
    QGLExists_GL_INGR_interlace_read = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_stencil_wrap;
int QGLNumFunctions_GL_EXT_stencil_wrap;
#ifdef QGL_DEFINED_GL_EXT_stencil_wrap
static void Init_GL_EXT_stencil_wrap ()
{
    QGLNumFunctions_GL_EXT_stencil_wrap = 0;
    QGLExists_GL_EXT_stencil_wrap = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_stencil_wrap ()
{
    QGLNumFunctions_GL_EXT_stencil_wrap = 0;
    QGLExists_GL_EXT_stencil_wrap = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_422_pixels;
int QGLNumFunctions_GL_EXT_422_pixels;
#ifdef QGL_DEFINED_GL_EXT_422_pixels
static void Init_GL_EXT_422_pixels ()
{
    QGLNumFunctions_GL_EXT_422_pixels = 0;
    QGLExists_GL_EXT_422_pixels = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_422_pixels ()
{
    QGLNumFunctions_GL_EXT_422_pixels = 0;
    QGLExists_GL_EXT_422_pixels = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texgen_reflection;
int QGLNumFunctions_GL_NV_texgen_reflection;
#ifdef QGL_DEFINED_GL_NV_texgen_reflection
static void Init_GL_NV_texgen_reflection ()
{
    QGLNumFunctions_GL_NV_texgen_reflection = 0;
    QGLExists_GL_NV_texgen_reflection = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texgen_reflection ()
{
    QGLNumFunctions_GL_NV_texgen_reflection = 0;
    QGLExists_GL_NV_texgen_reflection = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_cube_map;
int QGLNumFunctions_GL_EXT_texture_cube_map;
#ifdef QGL_DEFINED_GL_EXT_texture_cube_map
static void Init_GL_EXT_texture_cube_map ()
{
    QGLNumFunctions_GL_EXT_texture_cube_map = 0;
    QGLExists_GL_EXT_texture_cube_map = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_cube_map ()
{
    QGLNumFunctions_GL_EXT_texture_cube_map = 0;
    QGLExists_GL_EXT_texture_cube_map = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_convolution_border_modes;
int QGLNumFunctions_GL_SUN_convolution_border_modes;
#ifdef QGL_DEFINED_GL_SUN_convolution_border_modes
static void Init_GL_SUN_convolution_border_modes ()
{
    QGLNumFunctions_GL_SUN_convolution_border_modes = 0;
    QGLExists_GL_SUN_convolution_border_modes = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SUN_convolution_border_modes ()
{
    QGLNumFunctions_GL_SUN_convolution_border_modes = 0;
    QGLExists_GL_SUN_convolution_border_modes = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_env_add;
int QGLNumFunctions_GL_EXT_texture_env_add;
#ifdef QGL_DEFINED_GL_EXT_texture_env_add
static void Init_GL_EXT_texture_env_add ()
{
    QGLNumFunctions_GL_EXT_texture_env_add = 0;
    QGLExists_GL_EXT_texture_env_add = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_env_add ()
{
    QGLNumFunctions_GL_EXT_texture_env_add = 0;
    QGLExists_GL_EXT_texture_env_add = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_lod_bias;
int QGLNumFunctions_GL_EXT_texture_lod_bias;
#ifdef QGL_DEFINED_GL_EXT_texture_lod_bias
static void Init_GL_EXT_texture_lod_bias ()
{
    QGLNumFunctions_GL_EXT_texture_lod_bias = 0;
    QGLExists_GL_EXT_texture_lod_bias = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_lod_bias ()
{
    QGLNumFunctions_GL_EXT_texture_lod_bias = 0;
    QGLExists_GL_EXT_texture_lod_bias = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_filter_anisotropic;
int QGLNumFunctions_GL_EXT_texture_filter_anisotropic;
#ifdef QGL_DEFINED_GL_EXT_texture_filter_anisotropic
static void Init_GL_EXT_texture_filter_anisotropic ()
{
    QGLNumFunctions_GL_EXT_texture_filter_anisotropic = 0;
    QGLExists_GL_EXT_texture_filter_anisotropic = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_filter_anisotropic ()
{
    QGLNumFunctions_GL_EXT_texture_filter_anisotropic = 0;
    QGLExists_GL_EXT_texture_filter_anisotropic = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_vertex_weighting;
int QGLNumFunctions_GL_EXT_vertex_weighting;
#ifdef QGL_DEFINED_GL_EXT_vertex_weighting
pfn_glVertexWeightfEXT QGLglVertexWeightfEXT = 0;
pfn_glVertexWeightfvEXT QGLglVertexWeightfvEXT = 0;
pfn_glVertexWeightPointerEXT QGLglVertexWeightPointerEXT = 0;

static void Init_GL_EXT_vertex_weighting ()
{
    QGLNumFunctions_GL_EXT_vertex_weighting = 0;

    QGLglVertexWeightfEXT = (pfn_glVertexWeightfEXT)GetFunctionPointer("glVertexWeightfEXT");
    if ( QGLglVertexWeightfEXT )
        ++QGLNumFunctions_GL_EXT_vertex_weighting;

    QGLglVertexWeightfvEXT = (pfn_glVertexWeightfvEXT)GetFunctionPointer("glVertexWeightfvEXT");
    if ( QGLglVertexWeightfvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_weighting;

    QGLglVertexWeightPointerEXT = (pfn_glVertexWeightPointerEXT)GetFunctionPointer("glVertexWeightPointerEXT");
    if ( QGLglVertexWeightPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_weighting;

    if ( QGLNumFunctions_GL_EXT_vertex_weighting == 3 )
        QGLExists_GL_EXT_vertex_weighting = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_vertex_weighting = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_vertex_weighting ()
{
    QGLNumFunctions_GL_EXT_vertex_weighting = 0;
    QGLExists_GL_EXT_vertex_weighting = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_light_max_exponent;
int QGLNumFunctions_GL_NV_light_max_exponent;
#ifdef QGL_DEFINED_GL_NV_light_max_exponent
static void Init_GL_NV_light_max_exponent ()
{
    QGLNumFunctions_GL_NV_light_max_exponent = 0;
    QGLExists_GL_NV_light_max_exponent = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_light_max_exponent ()
{
    QGLNumFunctions_GL_NV_light_max_exponent = 0;
    QGLExists_GL_NV_light_max_exponent = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_array_range;
int QGLNumFunctions_GL_NV_vertex_array_range;
#ifdef QGL_DEFINED_GL_NV_vertex_array_range
pfn_glFlushVertexArrayRangeNV QGLglFlushVertexArrayRangeNV = 0;
pfn_glVertexArrayRangeNV QGLglVertexArrayRangeNV = 0;

static void Init_GL_NV_vertex_array_range ()
{
    QGLNumFunctions_GL_NV_vertex_array_range = 0;

    QGLglFlushVertexArrayRangeNV = (pfn_glFlushVertexArrayRangeNV)GetFunctionPointer("glFlushVertexArrayRangeNV");
    if ( QGLglFlushVertexArrayRangeNV )
        ++QGLNumFunctions_GL_NV_vertex_array_range;

    QGLglVertexArrayRangeNV = (pfn_glVertexArrayRangeNV)GetFunctionPointer("glVertexArrayRangeNV");
    if ( QGLglVertexArrayRangeNV )
        ++QGLNumFunctions_GL_NV_vertex_array_range;

    if ( QGLNumFunctions_GL_NV_vertex_array_range == 2 )
        QGLExists_GL_NV_vertex_array_range = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_vertex_array_range = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_array_range ()
{
    QGLNumFunctions_GL_NV_vertex_array_range = 0;
    QGLExists_GL_NV_vertex_array_range = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_register_combiners;
int QGLNumFunctions_GL_NV_register_combiners;
#ifdef QGL_DEFINED_GL_NV_register_combiners
pfn_glCombinerParameterfvNV QGLglCombinerParameterfvNV = 0;
pfn_glCombinerParameterfNV QGLglCombinerParameterfNV = 0;
pfn_glCombinerParameterivNV QGLglCombinerParameterivNV = 0;
pfn_glCombinerParameteriNV QGLglCombinerParameteriNV = 0;
pfn_glCombinerInputNV QGLglCombinerInputNV = 0;
pfn_glCombinerOutputNV QGLglCombinerOutputNV = 0;
pfn_glFinalCombinerInputNV QGLglFinalCombinerInputNV = 0;
pfn_glGetCombinerInputParameterfvNV QGLglGetCombinerInputParameterfvNV = 0;
pfn_glGetCombinerInputParameterivNV QGLglGetCombinerInputParameterivNV = 0;
pfn_glGetCombinerOutputParameterfvNV QGLglGetCombinerOutputParameterfvNV = 0;
pfn_glGetCombinerOutputParameterivNV QGLglGetCombinerOutputParameterivNV = 0;
pfn_glGetFinalCombinerInputParameterfvNV QGLglGetFinalCombinerInputParameterfvNV = 0;
pfn_glGetFinalCombinerInputParameterivNV QGLglGetFinalCombinerInputParameterivNV = 0;

static void Init_GL_NV_register_combiners ()
{
    QGLNumFunctions_GL_NV_register_combiners = 0;

    QGLglCombinerParameterfvNV = (pfn_glCombinerParameterfvNV)GetFunctionPointer("glCombinerParameterfvNV");
    if ( QGLglCombinerParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglCombinerParameterfNV = (pfn_glCombinerParameterfNV)GetFunctionPointer("glCombinerParameterfNV");
    if ( QGLglCombinerParameterfNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglCombinerParameterivNV = (pfn_glCombinerParameterivNV)GetFunctionPointer("glCombinerParameterivNV");
    if ( QGLglCombinerParameterivNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglCombinerParameteriNV = (pfn_glCombinerParameteriNV)GetFunctionPointer("glCombinerParameteriNV");
    if ( QGLglCombinerParameteriNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglCombinerInputNV = (pfn_glCombinerInputNV)GetFunctionPointer("glCombinerInputNV");
    if ( QGLglCombinerInputNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglCombinerOutputNV = (pfn_glCombinerOutputNV)GetFunctionPointer("glCombinerOutputNV");
    if ( QGLglCombinerOutputNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglFinalCombinerInputNV = (pfn_glFinalCombinerInputNV)GetFunctionPointer("glFinalCombinerInputNV");
    if ( QGLglFinalCombinerInputNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetCombinerInputParameterfvNV = (pfn_glGetCombinerInputParameterfvNV)GetFunctionPointer("glGetCombinerInputParameterfvNV");
    if ( QGLglGetCombinerInputParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetCombinerInputParameterivNV = (pfn_glGetCombinerInputParameterivNV)GetFunctionPointer("glGetCombinerInputParameterivNV");
    if ( QGLglGetCombinerInputParameterivNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetCombinerOutputParameterfvNV = (pfn_glGetCombinerOutputParameterfvNV)GetFunctionPointer("glGetCombinerOutputParameterfvNV");
    if ( QGLglGetCombinerOutputParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetCombinerOutputParameterivNV = (pfn_glGetCombinerOutputParameterivNV)GetFunctionPointer("glGetCombinerOutputParameterivNV");
    if ( QGLglGetCombinerOutputParameterivNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetFinalCombinerInputParameterfvNV = (pfn_glGetFinalCombinerInputParameterfvNV)GetFunctionPointer("glGetFinalCombinerInputParameterfvNV");
    if ( QGLglGetFinalCombinerInputParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    QGLglGetFinalCombinerInputParameterivNV = (pfn_glGetFinalCombinerInputParameterivNV)GetFunctionPointer("glGetFinalCombinerInputParameterivNV");
    if ( QGLglGetFinalCombinerInputParameterivNV )
        ++QGLNumFunctions_GL_NV_register_combiners;

    if ( QGLNumFunctions_GL_NV_register_combiners == 13 )
        QGLExists_GL_NV_register_combiners = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_register_combiners = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_register_combiners ()
{
    QGLNumFunctions_GL_NV_register_combiners = 0;
    QGLExists_GL_NV_register_combiners = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_fog_distance;
int QGLNumFunctions_GL_NV_fog_distance;
#ifdef QGL_DEFINED_GL_NV_fog_distance
static void Init_GL_NV_fog_distance ()
{
    QGLNumFunctions_GL_NV_fog_distance = 0;
    QGLExists_GL_NV_fog_distance = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_fog_distance ()
{
    QGLNumFunctions_GL_NV_fog_distance = 0;
    QGLExists_GL_NV_fog_distance = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texgen_emboss;
int QGLNumFunctions_GL_NV_texgen_emboss;
#ifdef QGL_DEFINED_GL_NV_texgen_emboss
static void Init_GL_NV_texgen_emboss ()
{
    QGLNumFunctions_GL_NV_texgen_emboss = 0;
    QGLExists_GL_NV_texgen_emboss = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texgen_emboss ()
{
    QGLNumFunctions_GL_NV_texgen_emboss = 0;
    QGLExists_GL_NV_texgen_emboss = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_blend_square;
int QGLNumFunctions_GL_NV_blend_square;
#ifdef QGL_DEFINED_GL_NV_blend_square
static void Init_GL_NV_blend_square ()
{
    QGLNumFunctions_GL_NV_blend_square = 0;
    QGLExists_GL_NV_blend_square = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_blend_square ()
{
    QGLNumFunctions_GL_NV_blend_square = 0;
    QGLExists_GL_NV_blend_square = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_env_combine4;
int QGLNumFunctions_GL_NV_texture_env_combine4;
#ifdef QGL_DEFINED_GL_NV_texture_env_combine4
static void Init_GL_NV_texture_env_combine4 ()
{
    QGLNumFunctions_GL_NV_texture_env_combine4 = 0;
    QGLExists_GL_NV_texture_env_combine4 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_env_combine4 ()
{
    QGLNumFunctions_GL_NV_texture_env_combine4 = 0;
    QGLExists_GL_NV_texture_env_combine4 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_MESA_resize_buffers;
int QGLNumFunctions_GL_MESA_resize_buffers;
#ifdef QGL_DEFINED_GL_MESA_resize_buffers
pfn_glResizeBuffersMESA QGLglResizeBuffersMESA = 0;

static void Init_GL_MESA_resize_buffers ()
{
    QGLNumFunctions_GL_MESA_resize_buffers = 0;

    QGLglResizeBuffersMESA = (pfn_glResizeBuffersMESA)GetFunctionPointer("glResizeBuffersMESA");
    if ( QGLglResizeBuffersMESA )
        ++QGLNumFunctions_GL_MESA_resize_buffers;

    if ( QGLNumFunctions_GL_MESA_resize_buffers == 1 )
        QGLExists_GL_MESA_resize_buffers = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_MESA_resize_buffers = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_MESA_resize_buffers ()
{
    QGLNumFunctions_GL_MESA_resize_buffers = 0;
    QGLExists_GL_MESA_resize_buffers = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_MESA_window_pos;
int QGLNumFunctions_GL_MESA_window_pos;
#ifdef QGL_DEFINED_GL_MESA_window_pos
pfn_glWindowPos2dMESA QGLglWindowPos2dMESA = 0;
pfn_glWindowPos2dvMESA QGLglWindowPos2dvMESA = 0;
pfn_glWindowPos2fMESA QGLglWindowPos2fMESA = 0;
pfn_glWindowPos2fvMESA QGLglWindowPos2fvMESA = 0;
pfn_glWindowPos2iMESA QGLglWindowPos2iMESA = 0;
pfn_glWindowPos2ivMESA QGLglWindowPos2ivMESA = 0;
pfn_glWindowPos2sMESA QGLglWindowPos2sMESA = 0;
pfn_glWindowPos2svMESA QGLglWindowPos2svMESA = 0;
pfn_glWindowPos3dMESA QGLglWindowPos3dMESA = 0;
pfn_glWindowPos3dvMESA QGLglWindowPos3dvMESA = 0;
pfn_glWindowPos3fMESA QGLglWindowPos3fMESA = 0;
pfn_glWindowPos3fvMESA QGLglWindowPos3fvMESA = 0;
pfn_glWindowPos3iMESA QGLglWindowPos3iMESA = 0;
pfn_glWindowPos3ivMESA QGLglWindowPos3ivMESA = 0;
pfn_glWindowPos3sMESA QGLglWindowPos3sMESA = 0;
pfn_glWindowPos3svMESA QGLglWindowPos3svMESA = 0;
pfn_glWindowPos4dMESA QGLglWindowPos4dMESA = 0;
pfn_glWindowPos4dvMESA QGLglWindowPos4dvMESA = 0;
pfn_glWindowPos4fMESA QGLglWindowPos4fMESA = 0;
pfn_glWindowPos4fvMESA QGLglWindowPos4fvMESA = 0;
pfn_glWindowPos4iMESA QGLglWindowPos4iMESA = 0;
pfn_glWindowPos4ivMESA QGLglWindowPos4ivMESA = 0;
pfn_glWindowPos4sMESA QGLglWindowPos4sMESA = 0;
pfn_glWindowPos4svMESA QGLglWindowPos4svMESA = 0;

static void Init_GL_MESA_window_pos ()
{
    QGLNumFunctions_GL_MESA_window_pos = 0;

    QGLglWindowPos2dMESA = (pfn_glWindowPos2dMESA)GetFunctionPointer("glWindowPos2dMESA");
    if ( QGLglWindowPos2dMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2dvMESA = (pfn_glWindowPos2dvMESA)GetFunctionPointer("glWindowPos2dvMESA");
    if ( QGLglWindowPos2dvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2fMESA = (pfn_glWindowPos2fMESA)GetFunctionPointer("glWindowPos2fMESA");
    if ( QGLglWindowPos2fMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2fvMESA = (pfn_glWindowPos2fvMESA)GetFunctionPointer("glWindowPos2fvMESA");
    if ( QGLglWindowPos2fvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2iMESA = (pfn_glWindowPos2iMESA)GetFunctionPointer("glWindowPos2iMESA");
    if ( QGLglWindowPos2iMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2ivMESA = (pfn_glWindowPos2ivMESA)GetFunctionPointer("glWindowPos2ivMESA");
    if ( QGLglWindowPos2ivMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2sMESA = (pfn_glWindowPos2sMESA)GetFunctionPointer("glWindowPos2sMESA");
    if ( QGLglWindowPos2sMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos2svMESA = (pfn_glWindowPos2svMESA)GetFunctionPointer("glWindowPos2svMESA");
    if ( QGLglWindowPos2svMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3dMESA = (pfn_glWindowPos3dMESA)GetFunctionPointer("glWindowPos3dMESA");
    if ( QGLglWindowPos3dMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3dvMESA = (pfn_glWindowPos3dvMESA)GetFunctionPointer("glWindowPos3dvMESA");
    if ( QGLglWindowPos3dvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3fMESA = (pfn_glWindowPos3fMESA)GetFunctionPointer("glWindowPos3fMESA");
    if ( QGLglWindowPos3fMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3fvMESA = (pfn_glWindowPos3fvMESA)GetFunctionPointer("glWindowPos3fvMESA");
    if ( QGLglWindowPos3fvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3iMESA = (pfn_glWindowPos3iMESA)GetFunctionPointer("glWindowPos3iMESA");
    if ( QGLglWindowPos3iMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3ivMESA = (pfn_glWindowPos3ivMESA)GetFunctionPointer("glWindowPos3ivMESA");
    if ( QGLglWindowPos3ivMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3sMESA = (pfn_glWindowPos3sMESA)GetFunctionPointer("glWindowPos3sMESA");
    if ( QGLglWindowPos3sMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos3svMESA = (pfn_glWindowPos3svMESA)GetFunctionPointer("glWindowPos3svMESA");
    if ( QGLglWindowPos3svMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4dMESA = (pfn_glWindowPos4dMESA)GetFunctionPointer("glWindowPos4dMESA");
    if ( QGLglWindowPos4dMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4dvMESA = (pfn_glWindowPos4dvMESA)GetFunctionPointer("glWindowPos4dvMESA");
    if ( QGLglWindowPos4dvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4fMESA = (pfn_glWindowPos4fMESA)GetFunctionPointer("glWindowPos4fMESA");
    if ( QGLglWindowPos4fMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4fvMESA = (pfn_glWindowPos4fvMESA)GetFunctionPointer("glWindowPos4fvMESA");
    if ( QGLglWindowPos4fvMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4iMESA = (pfn_glWindowPos4iMESA)GetFunctionPointer("glWindowPos4iMESA");
    if ( QGLglWindowPos4iMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4ivMESA = (pfn_glWindowPos4ivMESA)GetFunctionPointer("glWindowPos4ivMESA");
    if ( QGLglWindowPos4ivMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4sMESA = (pfn_glWindowPos4sMESA)GetFunctionPointer("glWindowPos4sMESA");
    if ( QGLglWindowPos4sMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    QGLglWindowPos4svMESA = (pfn_glWindowPos4svMESA)GetFunctionPointer("glWindowPos4svMESA");
    if ( QGLglWindowPos4svMESA )
        ++QGLNumFunctions_GL_MESA_window_pos;

    if ( QGLNumFunctions_GL_MESA_window_pos == 24 )
        QGLExists_GL_MESA_window_pos = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_MESA_window_pos = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_MESA_window_pos ()
{
    QGLNumFunctions_GL_MESA_window_pos = 0;
    QGLExists_GL_MESA_window_pos = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_compression_s3tc;
int QGLNumFunctions_GL_EXT_texture_compression_s3tc;
#ifdef QGL_DEFINED_GL_EXT_texture_compression_s3tc
static void Init_GL_EXT_texture_compression_s3tc ()
{
    QGLNumFunctions_GL_EXT_texture_compression_s3tc = 0;
    QGLExists_GL_EXT_texture_compression_s3tc = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_compression_s3tc ()
{
    QGLNumFunctions_GL_EXT_texture_compression_s3tc = 0;
    QGLExists_GL_EXT_texture_compression_s3tc = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_IBM_cull_vertex;
int QGLNumFunctions_GL_IBM_cull_vertex;
#ifdef QGL_DEFINED_GL_IBM_cull_vertex
static void Init_GL_IBM_cull_vertex ()
{
    QGLNumFunctions_GL_IBM_cull_vertex = 0;
    QGLExists_GL_IBM_cull_vertex = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_IBM_cull_vertex ()
{
    QGLNumFunctions_GL_IBM_cull_vertex = 0;
    QGLExists_GL_IBM_cull_vertex = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_IBM_multimode_draw_arrays;
int QGLNumFunctions_GL_IBM_multimode_draw_arrays;
#ifdef QGL_DEFINED_GL_IBM_multimode_draw_arrays
pfn_glMultiModeDrawArraysIBM QGLglMultiModeDrawArraysIBM = 0;
pfn_glMultiModeDrawElementsIBM QGLglMultiModeDrawElementsIBM = 0;

static void Init_GL_IBM_multimode_draw_arrays ()
{
    QGLNumFunctions_GL_IBM_multimode_draw_arrays = 0;

    QGLglMultiModeDrawArraysIBM = (pfn_glMultiModeDrawArraysIBM)GetFunctionPointer("glMultiModeDrawArraysIBM");
    if ( QGLglMultiModeDrawArraysIBM )
        ++QGLNumFunctions_GL_IBM_multimode_draw_arrays;

    QGLglMultiModeDrawElementsIBM = (pfn_glMultiModeDrawElementsIBM)GetFunctionPointer("glMultiModeDrawElementsIBM");
    if ( QGLglMultiModeDrawElementsIBM )
        ++QGLNumFunctions_GL_IBM_multimode_draw_arrays;

    if ( QGLNumFunctions_GL_IBM_multimode_draw_arrays == 2 )
        QGLExists_GL_IBM_multimode_draw_arrays = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_IBM_multimode_draw_arrays = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_IBM_multimode_draw_arrays ()
{
    QGLNumFunctions_GL_IBM_multimode_draw_arrays = 0;
    QGLExists_GL_IBM_multimode_draw_arrays = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_IBM_vertex_array_lists;
int QGLNumFunctions_GL_IBM_vertex_array_lists;
#ifdef QGL_DEFINED_GL_IBM_vertex_array_lists
pfn_glColorPointerListIBM QGLglColorPointerListIBM = 0;
pfn_glSecondaryColorPointerListIBM QGLglSecondaryColorPointerListIBM = 0;
pfn_glEdgeFlagPointerListIBM QGLglEdgeFlagPointerListIBM = 0;
pfn_glFogCoordPointerListIBM QGLglFogCoordPointerListIBM = 0;
pfn_glIndexPointerListIBM QGLglIndexPointerListIBM = 0;
pfn_glNormalPointerListIBM QGLglNormalPointerListIBM = 0;
pfn_glTexCoordPointerListIBM QGLglTexCoordPointerListIBM = 0;
pfn_glVertexPointerListIBM QGLglVertexPointerListIBM = 0;

static void Init_GL_IBM_vertex_array_lists ()
{
    QGLNumFunctions_GL_IBM_vertex_array_lists = 0;

    QGLglColorPointerListIBM = (pfn_glColorPointerListIBM)GetFunctionPointer("glColorPointerListIBM");
    if ( QGLglColorPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglSecondaryColorPointerListIBM = (pfn_glSecondaryColorPointerListIBM)GetFunctionPointer("glSecondaryColorPointerListIBM");
    if ( QGLglSecondaryColorPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglEdgeFlagPointerListIBM = (pfn_glEdgeFlagPointerListIBM)GetFunctionPointer("glEdgeFlagPointerListIBM");
    if ( QGLglEdgeFlagPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglFogCoordPointerListIBM = (pfn_glFogCoordPointerListIBM)GetFunctionPointer("glFogCoordPointerListIBM");
    if ( QGLglFogCoordPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglIndexPointerListIBM = (pfn_glIndexPointerListIBM)GetFunctionPointer("glIndexPointerListIBM");
    if ( QGLglIndexPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglNormalPointerListIBM = (pfn_glNormalPointerListIBM)GetFunctionPointer("glNormalPointerListIBM");
    if ( QGLglNormalPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglTexCoordPointerListIBM = (pfn_glTexCoordPointerListIBM)GetFunctionPointer("glTexCoordPointerListIBM");
    if ( QGLglTexCoordPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    QGLglVertexPointerListIBM = (pfn_glVertexPointerListIBM)GetFunctionPointer("glVertexPointerListIBM");
    if ( QGLglVertexPointerListIBM )
        ++QGLNumFunctions_GL_IBM_vertex_array_lists;

    if ( QGLNumFunctions_GL_IBM_vertex_array_lists == 8 )
        QGLExists_GL_IBM_vertex_array_lists = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_IBM_vertex_array_lists = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_IBM_vertex_array_lists ()
{
    QGLNumFunctions_GL_IBM_vertex_array_lists = 0;
    QGLExists_GL_IBM_vertex_array_lists = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_subsample;
int QGLNumFunctions_GL_SGIX_subsample;
#ifdef QGL_DEFINED_GL_SGIX_subsample
static void Init_GL_SGIX_subsample ()
{
    QGLNumFunctions_GL_SGIX_subsample = 0;
    QGLExists_GL_SGIX_subsample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_subsample ()
{
    QGLNumFunctions_GL_SGIX_subsample = 0;
    QGLExists_GL_SGIX_subsample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_ycrcb_subsample;
int QGLNumFunctions_GL_SGIX_ycrcb_subsample;
#ifdef QGL_DEFINED_GL_SGIX_ycrcb_subsample
static void Init_GL_SGIX_ycrcb_subsample ()
{
    QGLNumFunctions_GL_SGIX_ycrcb_subsample = 0;
    QGLExists_GL_SGIX_ycrcb_subsample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_ycrcb_subsample ()
{
    QGLNumFunctions_GL_SGIX_ycrcb_subsample = 0;
    QGLExists_GL_SGIX_ycrcb_subsample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_ycrcba;
int QGLNumFunctions_GL_SGIX_ycrcba;
#ifdef QGL_DEFINED_GL_SGIX_ycrcba
static void Init_GL_SGIX_ycrcba ()
{
    QGLNumFunctions_GL_SGIX_ycrcba = 0;
    QGLExists_GL_SGIX_ycrcba = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_ycrcba ()
{
    QGLNumFunctions_GL_SGIX_ycrcba = 0;
    QGLExists_GL_SGIX_ycrcba = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_depth_pass_instrument;
int QGLNumFunctions_GL_SGIX_depth_pass_instrument;
#ifdef QGL_DEFINED_GL_SGIX_depth_pass_instrument
static void Init_GL_SGIX_depth_pass_instrument ()
{
    QGLNumFunctions_GL_SGIX_depth_pass_instrument = 0;
    QGLExists_GL_SGIX_depth_pass_instrument = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_depth_pass_instrument ()
{
    QGLNumFunctions_GL_SGIX_depth_pass_instrument = 0;
    QGLExists_GL_SGIX_depth_pass_instrument = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_3DFX_texture_compression_FXT1;
int QGLNumFunctions_GL_3DFX_texture_compression_FXT1;
#ifdef QGL_DEFINED_GL_3DFX_texture_compression_FXT1
static void Init_GL_3DFX_texture_compression_FXT1 ()
{
    QGLNumFunctions_GL_3DFX_texture_compression_FXT1 = 0;
    QGLExists_GL_3DFX_texture_compression_FXT1 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_3DFX_texture_compression_FXT1 ()
{
    QGLNumFunctions_GL_3DFX_texture_compression_FXT1 = 0;
    QGLExists_GL_3DFX_texture_compression_FXT1 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_3DFX_multisample;
int QGLNumFunctions_GL_3DFX_multisample;
#ifdef QGL_DEFINED_GL_3DFX_multisample
static void Init_GL_3DFX_multisample ()
{
    QGLNumFunctions_GL_3DFX_multisample = 0;
    QGLExists_GL_3DFX_multisample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_3DFX_multisample ()
{
    QGLNumFunctions_GL_3DFX_multisample = 0;
    QGLExists_GL_3DFX_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_3DFX_tbuffer;
int QGLNumFunctions_GL_3DFX_tbuffer;
#ifdef QGL_DEFINED_GL_3DFX_tbuffer
pfn_glTbufferMask3DFX QGLglTbufferMask3DFX = 0;

static void Init_GL_3DFX_tbuffer ()
{
    QGLNumFunctions_GL_3DFX_tbuffer = 0;

    QGLglTbufferMask3DFX = (pfn_glTbufferMask3DFX)GetFunctionPointer("glTbufferMask3DFX");
    if ( QGLglTbufferMask3DFX )
        ++QGLNumFunctions_GL_3DFX_tbuffer;

    if ( QGLNumFunctions_GL_3DFX_tbuffer == 1 )
        QGLExists_GL_3DFX_tbuffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_3DFX_tbuffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_3DFX_tbuffer ()
{
    QGLNumFunctions_GL_3DFX_tbuffer = 0;
    QGLExists_GL_3DFX_tbuffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_multisample;
int QGLNumFunctions_GL_EXT_multisample;
#ifdef QGL_DEFINED_GL_EXT_multisample
pfn_glSampleMaskEXT QGLglSampleMaskEXT = 0;
pfn_glSamplePatternEXT QGLglSamplePatternEXT = 0;

static void Init_GL_EXT_multisample ()
{
    QGLNumFunctions_GL_EXT_multisample = 0;

    QGLglSampleMaskEXT = (pfn_glSampleMaskEXT)GetFunctionPointer("glSampleMaskEXT");
    if ( QGLglSampleMaskEXT )
        ++QGLNumFunctions_GL_EXT_multisample;

    QGLglSamplePatternEXT = (pfn_glSamplePatternEXT)GetFunctionPointer("glSamplePatternEXT");
    if ( QGLglSamplePatternEXT )
        ++QGLNumFunctions_GL_EXT_multisample;

    if ( QGLNumFunctions_GL_EXT_multisample == 2 )
        QGLExists_GL_EXT_multisample = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_multisample = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_multisample ()
{
    QGLNumFunctions_GL_EXT_multisample = 0;
    QGLExists_GL_EXT_multisample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_vertex_preclip;
int QGLNumFunctions_GL_SGIX_vertex_preclip;
#ifdef QGL_DEFINED_GL_SGIX_vertex_preclip
static void Init_GL_SGIX_vertex_preclip ()
{
    QGLNumFunctions_GL_SGIX_vertex_preclip = 0;
    QGLExists_GL_SGIX_vertex_preclip = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_vertex_preclip ()
{
    QGLNumFunctions_GL_SGIX_vertex_preclip = 0;
    QGLExists_GL_SGIX_vertex_preclip = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_convolution_accuracy;
int QGLNumFunctions_GL_SGIX_convolution_accuracy;
#ifdef QGL_DEFINED_GL_SGIX_convolution_accuracy
static void Init_GL_SGIX_convolution_accuracy ()
{
    QGLNumFunctions_GL_SGIX_convolution_accuracy = 0;
    QGLExists_GL_SGIX_convolution_accuracy = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_convolution_accuracy ()
{
    QGLNumFunctions_GL_SGIX_convolution_accuracy = 0;
    QGLExists_GL_SGIX_convolution_accuracy = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_resample;
int QGLNumFunctions_GL_SGIX_resample;
#ifdef QGL_DEFINED_GL_SGIX_resample
static void Init_GL_SGIX_resample ()
{
    QGLNumFunctions_GL_SGIX_resample = 0;
    QGLExists_GL_SGIX_resample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_resample ()
{
    QGLNumFunctions_GL_SGIX_resample = 0;
    QGLExists_GL_SGIX_resample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_point_line_texgen;
int QGLNumFunctions_GL_SGIS_point_line_texgen;
#ifdef QGL_DEFINED_GL_SGIS_point_line_texgen
static void Init_GL_SGIS_point_line_texgen ()
{
    QGLNumFunctions_GL_SGIS_point_line_texgen = 0;
    QGLExists_GL_SGIS_point_line_texgen = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIS_point_line_texgen ()
{
    QGLNumFunctions_GL_SGIS_point_line_texgen = 0;
    QGLExists_GL_SGIS_point_line_texgen = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIS_texture_color_mask;
int QGLNumFunctions_GL_SGIS_texture_color_mask;
#ifdef QGL_DEFINED_GL_SGIS_texture_color_mask
pfn_glTextureColorMaskSGIS QGLglTextureColorMaskSGIS = 0;

static void Init_GL_SGIS_texture_color_mask ()
{
    QGLNumFunctions_GL_SGIS_texture_color_mask = 0;

    QGLglTextureColorMaskSGIS = (pfn_glTextureColorMaskSGIS)GetFunctionPointer("glTextureColorMaskSGIS");
    if ( QGLglTextureColorMaskSGIS )
        ++QGLNumFunctions_GL_SGIS_texture_color_mask;

    if ( QGLNumFunctions_GL_SGIS_texture_color_mask == 1 )
        QGLExists_GL_SGIS_texture_color_mask = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIS_texture_color_mask = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIS_texture_color_mask ()
{
    QGLNumFunctions_GL_SGIS_texture_color_mask = 0;
    QGLExists_GL_SGIS_texture_color_mask = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_igloo_interface;
int QGLNumFunctions_GL_SGIX_igloo_interface;
#ifdef QGL_DEFINED_GL_SGIX_igloo_interface
pfn_glIglooInterfaceSGIX QGLglIglooInterfaceSGIX = 0;

static void Init_GL_SGIX_igloo_interface ()
{
    QGLNumFunctions_GL_SGIX_igloo_interface = 0;

    QGLglIglooInterfaceSGIX = (pfn_glIglooInterfaceSGIX)GetFunctionPointer("glIglooInterfaceSGIX");
    if ( QGLglIglooInterfaceSGIX )
        ++QGLNumFunctions_GL_SGIX_igloo_interface;

    if ( QGLNumFunctions_GL_SGIX_igloo_interface == 1 )
        QGLExists_GL_SGIX_igloo_interface = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SGIX_igloo_interface = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SGIX_igloo_interface ()
{
    QGLNumFunctions_GL_SGIX_igloo_interface = 0;
    QGLExists_GL_SGIX_igloo_interface = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_env_dot3;
int QGLNumFunctions_GL_EXT_texture_env_dot3;
#ifdef QGL_DEFINED_GL_EXT_texture_env_dot3
static void Init_GL_EXT_texture_env_dot3 ()
{
    QGLNumFunctions_GL_EXT_texture_env_dot3 = 0;
    QGLExists_GL_EXT_texture_env_dot3 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_env_dot3 ()
{
    QGLNumFunctions_GL_EXT_texture_env_dot3 = 0;
    QGLExists_GL_EXT_texture_env_dot3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_texture_mirror_once;
int QGLNumFunctions_GL_ATI_texture_mirror_once;
#ifdef QGL_DEFINED_GL_ATI_texture_mirror_once
static void Init_GL_ATI_texture_mirror_once ()
{
    QGLNumFunctions_GL_ATI_texture_mirror_once = 0;
    QGLExists_GL_ATI_texture_mirror_once = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ATI_texture_mirror_once ()
{
    QGLNumFunctions_GL_ATI_texture_mirror_once = 0;
    QGLExists_GL_ATI_texture_mirror_once = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_fence;
int QGLNumFunctions_GL_NV_fence;
#ifdef QGL_DEFINED_GL_NV_fence
pfn_glDeleteFencesNV QGLglDeleteFencesNV = 0;
pfn_glGenFencesNV QGLglGenFencesNV = 0;
pfn_glIsFenceNV QGLglIsFenceNV = 0;
pfn_glTestFenceNV QGLglTestFenceNV = 0;
pfn_glGetFenceivNV QGLglGetFenceivNV = 0;
pfn_glFinishFenceNV QGLglFinishFenceNV = 0;
pfn_glSetFenceNV QGLglSetFenceNV = 0;

static void Init_GL_NV_fence ()
{
    QGLNumFunctions_GL_NV_fence = 0;

    QGLglDeleteFencesNV = (pfn_glDeleteFencesNV)GetFunctionPointer("glDeleteFencesNV");
    if ( QGLglDeleteFencesNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglGenFencesNV = (pfn_glGenFencesNV)GetFunctionPointer("glGenFencesNV");
    if ( QGLglGenFencesNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglIsFenceNV = (pfn_glIsFenceNV)GetFunctionPointer("glIsFenceNV");
    if ( QGLglIsFenceNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglTestFenceNV = (pfn_glTestFenceNV)GetFunctionPointer("glTestFenceNV");
    if ( QGLglTestFenceNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglGetFenceivNV = (pfn_glGetFenceivNV)GetFunctionPointer("glGetFenceivNV");
    if ( QGLglGetFenceivNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglFinishFenceNV = (pfn_glFinishFenceNV)GetFunctionPointer("glFinishFenceNV");
    if ( QGLglFinishFenceNV )
        ++QGLNumFunctions_GL_NV_fence;

    QGLglSetFenceNV = (pfn_glSetFenceNV)GetFunctionPointer("glSetFenceNV");
    if ( QGLglSetFenceNV )
        ++QGLNumFunctions_GL_NV_fence;

    if ( QGLNumFunctions_GL_NV_fence == 7 )
        QGLExists_GL_NV_fence = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_fence = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_fence ()
{
    QGLNumFunctions_GL_NV_fence = 0;
    QGLExists_GL_NV_fence = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_IBM_texture_mirrored_repeat;
int QGLNumFunctions_GL_IBM_texture_mirrored_repeat;
#ifdef QGL_DEFINED_GL_IBM_texture_mirrored_repeat
static void Init_GL_IBM_texture_mirrored_repeat ()
{
    QGLNumFunctions_GL_IBM_texture_mirrored_repeat = 0;
    QGLExists_GL_IBM_texture_mirrored_repeat = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_IBM_texture_mirrored_repeat ()
{
    QGLNumFunctions_GL_IBM_texture_mirrored_repeat = 0;
    QGLExists_GL_IBM_texture_mirrored_repeat = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_evaluators;
int QGLNumFunctions_GL_NV_evaluators;
#ifdef QGL_DEFINED_GL_NV_evaluators
pfn_glMapControlPointsNV QGLglMapControlPointsNV = 0;
pfn_glMapParameterivNV QGLglMapParameterivNV = 0;
pfn_glMapParameterfvNV QGLglMapParameterfvNV = 0;
pfn_glGetMapControlPointsNV QGLglGetMapControlPointsNV = 0;
pfn_glGetMapParameterivNV QGLglGetMapParameterivNV = 0;
pfn_glGetMapParameterfvNV QGLglGetMapParameterfvNV = 0;
pfn_glGetMapAttribParameterivNV QGLglGetMapAttribParameterivNV = 0;
pfn_glGetMapAttribParameterfvNV QGLglGetMapAttribParameterfvNV = 0;
pfn_glEvalMapsNV QGLglEvalMapsNV = 0;

static void Init_GL_NV_evaluators ()
{
    QGLNumFunctions_GL_NV_evaluators = 0;

    QGLglMapControlPointsNV = (pfn_glMapControlPointsNV)GetFunctionPointer("glMapControlPointsNV");
    if ( QGLglMapControlPointsNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglMapParameterivNV = (pfn_glMapParameterivNV)GetFunctionPointer("glMapParameterivNV");
    if ( QGLglMapParameterivNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglMapParameterfvNV = (pfn_glMapParameterfvNV)GetFunctionPointer("glMapParameterfvNV");
    if ( QGLglMapParameterfvNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglGetMapControlPointsNV = (pfn_glGetMapControlPointsNV)GetFunctionPointer("glGetMapControlPointsNV");
    if ( QGLglGetMapControlPointsNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglGetMapParameterivNV = (pfn_glGetMapParameterivNV)GetFunctionPointer("glGetMapParameterivNV");
    if ( QGLglGetMapParameterivNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglGetMapParameterfvNV = (pfn_glGetMapParameterfvNV)GetFunctionPointer("glGetMapParameterfvNV");
    if ( QGLglGetMapParameterfvNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglGetMapAttribParameterivNV = (pfn_glGetMapAttribParameterivNV)GetFunctionPointer("glGetMapAttribParameterivNV");
    if ( QGLglGetMapAttribParameterivNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglGetMapAttribParameterfvNV = (pfn_glGetMapAttribParameterfvNV)GetFunctionPointer("glGetMapAttribParameterfvNV");
    if ( QGLglGetMapAttribParameterfvNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    QGLglEvalMapsNV = (pfn_glEvalMapsNV)GetFunctionPointer("glEvalMapsNV");
    if ( QGLglEvalMapsNV )
        ++QGLNumFunctions_GL_NV_evaluators;

    if ( QGLNumFunctions_GL_NV_evaluators == 9 )
        QGLExists_GL_NV_evaluators = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_evaluators = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_evaluators ()
{
    QGLNumFunctions_GL_NV_evaluators = 0;
    QGLExists_GL_NV_evaluators = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_packed_depth_stencil;
int QGLNumFunctions_GL_NV_packed_depth_stencil;
#ifdef QGL_DEFINED_GL_NV_packed_depth_stencil
static void Init_GL_NV_packed_depth_stencil ()
{
    QGLNumFunctions_GL_NV_packed_depth_stencil = 0;
    QGLExists_GL_NV_packed_depth_stencil = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_packed_depth_stencil ()
{
    QGLNumFunctions_GL_NV_packed_depth_stencil = 0;
    QGLExists_GL_NV_packed_depth_stencil = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_register_combiners2;
int QGLNumFunctions_GL_NV_register_combiners2;
#ifdef QGL_DEFINED_GL_NV_register_combiners2
pfn_glCombinerStageParameterfvNV QGLglCombinerStageParameterfvNV = 0;
pfn_glGetCombinerStageParameterfvNV QGLglGetCombinerStageParameterfvNV = 0;

static void Init_GL_NV_register_combiners2 ()
{
    QGLNumFunctions_GL_NV_register_combiners2 = 0;

    QGLglCombinerStageParameterfvNV = (pfn_glCombinerStageParameterfvNV)GetFunctionPointer("glCombinerStageParameterfvNV");
    if ( QGLglCombinerStageParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners2;

    QGLglGetCombinerStageParameterfvNV = (pfn_glGetCombinerStageParameterfvNV)GetFunctionPointer("glGetCombinerStageParameterfvNV");
    if ( QGLglGetCombinerStageParameterfvNV )
        ++QGLNumFunctions_GL_NV_register_combiners2;

    if ( QGLNumFunctions_GL_NV_register_combiners2 == 2 )
        QGLExists_GL_NV_register_combiners2 = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_register_combiners2 = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_register_combiners2 ()
{
    QGLNumFunctions_GL_NV_register_combiners2 = 0;
    QGLExists_GL_NV_register_combiners2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_compression_vtc;
int QGLNumFunctions_GL_NV_texture_compression_vtc;
#ifdef QGL_DEFINED_GL_NV_texture_compression_vtc
static void Init_GL_NV_texture_compression_vtc ()
{
    QGLNumFunctions_GL_NV_texture_compression_vtc = 0;
    QGLExists_GL_NV_texture_compression_vtc = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_compression_vtc ()
{
    QGLNumFunctions_GL_NV_texture_compression_vtc = 0;
    QGLExists_GL_NV_texture_compression_vtc = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_rectangle;
int QGLNumFunctions_GL_NV_texture_rectangle;
#ifdef QGL_DEFINED_GL_NV_texture_rectangle
static void Init_GL_NV_texture_rectangle ()
{
    QGLNumFunctions_GL_NV_texture_rectangle = 0;
    QGLExists_GL_NV_texture_rectangle = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_rectangle ()
{
    QGLNumFunctions_GL_NV_texture_rectangle = 0;
    QGLExists_GL_NV_texture_rectangle = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_shader;
int QGLNumFunctions_GL_NV_texture_shader;
#ifdef QGL_DEFINED_GL_NV_texture_shader
static void Init_GL_NV_texture_shader ()
{
    QGLNumFunctions_GL_NV_texture_shader = 0;
    QGLExists_GL_NV_texture_shader = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_shader ()
{
    QGLNumFunctions_GL_NV_texture_shader = 0;
    QGLExists_GL_NV_texture_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_shader2;
int QGLNumFunctions_GL_NV_texture_shader2;
#ifdef QGL_DEFINED_GL_NV_texture_shader2
static void Init_GL_NV_texture_shader2 ()
{
    QGLNumFunctions_GL_NV_texture_shader2 = 0;
    QGLExists_GL_NV_texture_shader2 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_shader2 ()
{
    QGLNumFunctions_GL_NV_texture_shader2 = 0;
    QGLExists_GL_NV_texture_shader2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_array_range2;
int QGLNumFunctions_GL_NV_vertex_array_range2;
#ifdef QGL_DEFINED_GL_NV_vertex_array_range2
static void Init_GL_NV_vertex_array_range2 ()
{
    QGLNumFunctions_GL_NV_vertex_array_range2 = 0;
    QGLExists_GL_NV_vertex_array_range2 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_array_range2 ()
{
    QGLNumFunctions_GL_NV_vertex_array_range2 = 0;
    QGLExists_GL_NV_vertex_array_range2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_program;
int QGLNumFunctions_GL_NV_vertex_program;
#ifdef QGL_DEFINED_GL_NV_vertex_program
pfn_glAreProgramsResidentNV QGLglAreProgramsResidentNV = 0;
pfn_glBindProgramNV QGLglBindProgramNV = 0;
pfn_glDeleteProgramsNV QGLglDeleteProgramsNV = 0;
pfn_glExecuteProgramNV QGLglExecuteProgramNV = 0;
pfn_glGenProgramsNV QGLglGenProgramsNV = 0;
pfn_glGetProgramParameterdvNV QGLglGetProgramParameterdvNV = 0;
pfn_glGetProgramParameterfvNV QGLglGetProgramParameterfvNV = 0;
pfn_glGetProgramivNV QGLglGetProgramivNV = 0;
pfn_glGetProgramStringNV QGLglGetProgramStringNV = 0;
pfn_glGetTrackMatrixivNV QGLglGetTrackMatrixivNV = 0;
pfn_glGetVertexAttribdvNV QGLglGetVertexAttribdvNV = 0;
pfn_glGetVertexAttribfvNV QGLglGetVertexAttribfvNV = 0;
pfn_glGetVertexAttribivNV QGLglGetVertexAttribivNV = 0;
pfn_glGetVertexAttribPointervNV QGLglGetVertexAttribPointervNV = 0;
pfn_glIsProgramNV QGLglIsProgramNV = 0;
pfn_glLoadProgramNV QGLglLoadProgramNV = 0;
pfn_glProgramParameter4dNV QGLglProgramParameter4dNV = 0;
pfn_glProgramParameter4dvNV QGLglProgramParameter4dvNV = 0;
pfn_glProgramParameter4fNV QGLglProgramParameter4fNV = 0;
pfn_glProgramParameter4fvNV QGLglProgramParameter4fvNV = 0;
pfn_glProgramParameters4dvNV QGLglProgramParameters4dvNV = 0;
pfn_glProgramParameters4fvNV QGLglProgramParameters4fvNV = 0;
pfn_glRequestResidentProgramsNV QGLglRequestResidentProgramsNV = 0;
pfn_glTrackMatrixNV QGLglTrackMatrixNV = 0;
pfn_glVertexAttribPointerNV QGLglVertexAttribPointerNV = 0;
pfn_glVertexAttrib1dNV QGLglVertexAttrib1dNV = 0;
pfn_glVertexAttrib1dvNV QGLglVertexAttrib1dvNV = 0;
pfn_glVertexAttrib1fNV QGLglVertexAttrib1fNV = 0;
pfn_glVertexAttrib1fvNV QGLglVertexAttrib1fvNV = 0;
pfn_glVertexAttrib1sNV QGLglVertexAttrib1sNV = 0;
pfn_glVertexAttrib1svNV QGLglVertexAttrib1svNV = 0;
pfn_glVertexAttrib2dNV QGLglVertexAttrib2dNV = 0;
pfn_glVertexAttrib2dvNV QGLglVertexAttrib2dvNV = 0;
pfn_glVertexAttrib2fNV QGLglVertexAttrib2fNV = 0;
pfn_glVertexAttrib2fvNV QGLglVertexAttrib2fvNV = 0;
pfn_glVertexAttrib2sNV QGLglVertexAttrib2sNV = 0;
pfn_glVertexAttrib2svNV QGLglVertexAttrib2svNV = 0;
pfn_glVertexAttrib3dNV QGLglVertexAttrib3dNV = 0;
pfn_glVertexAttrib3dvNV QGLglVertexAttrib3dvNV = 0;
pfn_glVertexAttrib3fNV QGLglVertexAttrib3fNV = 0;
pfn_glVertexAttrib3fvNV QGLglVertexAttrib3fvNV = 0;
pfn_glVertexAttrib3sNV QGLglVertexAttrib3sNV = 0;
pfn_glVertexAttrib3svNV QGLglVertexAttrib3svNV = 0;
pfn_glVertexAttrib4dNV QGLglVertexAttrib4dNV = 0;
pfn_glVertexAttrib4dvNV QGLglVertexAttrib4dvNV = 0;
pfn_glVertexAttrib4fNV QGLglVertexAttrib4fNV = 0;
pfn_glVertexAttrib4fvNV QGLglVertexAttrib4fvNV = 0;
pfn_glVertexAttrib4sNV QGLglVertexAttrib4sNV = 0;
pfn_glVertexAttrib4svNV QGLglVertexAttrib4svNV = 0;
pfn_glVertexAttrib4ubNV QGLglVertexAttrib4ubNV = 0;
pfn_glVertexAttrib4ubvNV QGLglVertexAttrib4ubvNV = 0;
pfn_glVertexAttribs1dvNV QGLglVertexAttribs1dvNV = 0;
pfn_glVertexAttribs1fvNV QGLglVertexAttribs1fvNV = 0;
pfn_glVertexAttribs1svNV QGLglVertexAttribs1svNV = 0;
pfn_glVertexAttribs2dvNV QGLglVertexAttribs2dvNV = 0;
pfn_glVertexAttribs2fvNV QGLglVertexAttribs2fvNV = 0;
pfn_glVertexAttribs2svNV QGLglVertexAttribs2svNV = 0;
pfn_glVertexAttribs3dvNV QGLglVertexAttribs3dvNV = 0;
pfn_glVertexAttribs3fvNV QGLglVertexAttribs3fvNV = 0;
pfn_glVertexAttribs3svNV QGLglVertexAttribs3svNV = 0;
pfn_glVertexAttribs4dvNV QGLglVertexAttribs4dvNV = 0;
pfn_glVertexAttribs4fvNV QGLglVertexAttribs4fvNV = 0;
pfn_glVertexAttribs4svNV QGLglVertexAttribs4svNV = 0;
pfn_glVertexAttribs4ubvNV QGLglVertexAttribs4ubvNV = 0;

static void Init_GL_NV_vertex_program ()
{
    QGLNumFunctions_GL_NV_vertex_program = 0;

    QGLglAreProgramsResidentNV = (pfn_glAreProgramsResidentNV)GetFunctionPointer("glAreProgramsResidentNV");
    if ( QGLglAreProgramsResidentNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglBindProgramNV = (pfn_glBindProgramNV)GetFunctionPointer("glBindProgramNV");
    if ( QGLglBindProgramNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglDeleteProgramsNV = (pfn_glDeleteProgramsNV)GetFunctionPointer("glDeleteProgramsNV");
    if ( QGLglDeleteProgramsNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglExecuteProgramNV = (pfn_glExecuteProgramNV)GetFunctionPointer("glExecuteProgramNV");
    if ( QGLglExecuteProgramNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGenProgramsNV = (pfn_glGenProgramsNV)GetFunctionPointer("glGenProgramsNV");
    if ( QGLglGenProgramsNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetProgramParameterdvNV = (pfn_glGetProgramParameterdvNV)GetFunctionPointer("glGetProgramParameterdvNV");
    if ( QGLglGetProgramParameterdvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetProgramParameterfvNV = (pfn_glGetProgramParameterfvNV)GetFunctionPointer("glGetProgramParameterfvNV");
    if ( QGLglGetProgramParameterfvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetProgramivNV = (pfn_glGetProgramivNV)GetFunctionPointer("glGetProgramivNV");
    if ( QGLglGetProgramivNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetProgramStringNV = (pfn_glGetProgramStringNV)GetFunctionPointer("glGetProgramStringNV");
    if ( QGLglGetProgramStringNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetTrackMatrixivNV = (pfn_glGetTrackMatrixivNV)GetFunctionPointer("glGetTrackMatrixivNV");
    if ( QGLglGetTrackMatrixivNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetVertexAttribdvNV = (pfn_glGetVertexAttribdvNV)GetFunctionPointer("glGetVertexAttribdvNV");
    if ( QGLglGetVertexAttribdvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetVertexAttribfvNV = (pfn_glGetVertexAttribfvNV)GetFunctionPointer("glGetVertexAttribfvNV");
    if ( QGLglGetVertexAttribfvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetVertexAttribivNV = (pfn_glGetVertexAttribivNV)GetFunctionPointer("glGetVertexAttribivNV");
    if ( QGLglGetVertexAttribivNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglGetVertexAttribPointervNV = (pfn_glGetVertexAttribPointervNV)GetFunctionPointer("glGetVertexAttribPointervNV");
    if ( QGLglGetVertexAttribPointervNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglIsProgramNV = (pfn_glIsProgramNV)GetFunctionPointer("glIsProgramNV");
    if ( QGLglIsProgramNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglLoadProgramNV = (pfn_glLoadProgramNV)GetFunctionPointer("glLoadProgramNV");
    if ( QGLglLoadProgramNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameter4dNV = (pfn_glProgramParameter4dNV)GetFunctionPointer("glProgramParameter4dNV");
    if ( QGLglProgramParameter4dNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameter4dvNV = (pfn_glProgramParameter4dvNV)GetFunctionPointer("glProgramParameter4dvNV");
    if ( QGLglProgramParameter4dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameter4fNV = (pfn_glProgramParameter4fNV)GetFunctionPointer("glProgramParameter4fNV");
    if ( QGLglProgramParameter4fNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameter4fvNV = (pfn_glProgramParameter4fvNV)GetFunctionPointer("glProgramParameter4fvNV");
    if ( QGLglProgramParameter4fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameters4dvNV = (pfn_glProgramParameters4dvNV)GetFunctionPointer("glProgramParameters4dvNV");
    if ( QGLglProgramParameters4dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglProgramParameters4fvNV = (pfn_glProgramParameters4fvNV)GetFunctionPointer("glProgramParameters4fvNV");
    if ( QGLglProgramParameters4fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglRequestResidentProgramsNV = (pfn_glRequestResidentProgramsNV)GetFunctionPointer("glRequestResidentProgramsNV");
    if ( QGLglRequestResidentProgramsNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglTrackMatrixNV = (pfn_glTrackMatrixNV)GetFunctionPointer("glTrackMatrixNV");
    if ( QGLglTrackMatrixNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribPointerNV = (pfn_glVertexAttribPointerNV)GetFunctionPointer("glVertexAttribPointerNV");
    if ( QGLglVertexAttribPointerNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1dNV = (pfn_glVertexAttrib1dNV)GetFunctionPointer("glVertexAttrib1dNV");
    if ( QGLglVertexAttrib1dNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1dvNV = (pfn_glVertexAttrib1dvNV)GetFunctionPointer("glVertexAttrib1dvNV");
    if ( QGLglVertexAttrib1dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1fNV = (pfn_glVertexAttrib1fNV)GetFunctionPointer("glVertexAttrib1fNV");
    if ( QGLglVertexAttrib1fNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1fvNV = (pfn_glVertexAttrib1fvNV)GetFunctionPointer("glVertexAttrib1fvNV");
    if ( QGLglVertexAttrib1fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1sNV = (pfn_glVertexAttrib1sNV)GetFunctionPointer("glVertexAttrib1sNV");
    if ( QGLglVertexAttrib1sNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib1svNV = (pfn_glVertexAttrib1svNV)GetFunctionPointer("glVertexAttrib1svNV");
    if ( QGLglVertexAttrib1svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2dNV = (pfn_glVertexAttrib2dNV)GetFunctionPointer("glVertexAttrib2dNV");
    if ( QGLglVertexAttrib2dNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2dvNV = (pfn_glVertexAttrib2dvNV)GetFunctionPointer("glVertexAttrib2dvNV");
    if ( QGLglVertexAttrib2dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2fNV = (pfn_glVertexAttrib2fNV)GetFunctionPointer("glVertexAttrib2fNV");
    if ( QGLglVertexAttrib2fNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2fvNV = (pfn_glVertexAttrib2fvNV)GetFunctionPointer("glVertexAttrib2fvNV");
    if ( QGLglVertexAttrib2fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2sNV = (pfn_glVertexAttrib2sNV)GetFunctionPointer("glVertexAttrib2sNV");
    if ( QGLglVertexAttrib2sNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib2svNV = (pfn_glVertexAttrib2svNV)GetFunctionPointer("glVertexAttrib2svNV");
    if ( QGLglVertexAttrib2svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3dNV = (pfn_glVertexAttrib3dNV)GetFunctionPointer("glVertexAttrib3dNV");
    if ( QGLglVertexAttrib3dNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3dvNV = (pfn_glVertexAttrib3dvNV)GetFunctionPointer("glVertexAttrib3dvNV");
    if ( QGLglVertexAttrib3dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3fNV = (pfn_glVertexAttrib3fNV)GetFunctionPointer("glVertexAttrib3fNV");
    if ( QGLglVertexAttrib3fNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3fvNV = (pfn_glVertexAttrib3fvNV)GetFunctionPointer("glVertexAttrib3fvNV");
    if ( QGLglVertexAttrib3fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3sNV = (pfn_glVertexAttrib3sNV)GetFunctionPointer("glVertexAttrib3sNV");
    if ( QGLglVertexAttrib3sNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib3svNV = (pfn_glVertexAttrib3svNV)GetFunctionPointer("glVertexAttrib3svNV");
    if ( QGLglVertexAttrib3svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4dNV = (pfn_glVertexAttrib4dNV)GetFunctionPointer("glVertexAttrib4dNV");
    if ( QGLglVertexAttrib4dNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4dvNV = (pfn_glVertexAttrib4dvNV)GetFunctionPointer("glVertexAttrib4dvNV");
    if ( QGLglVertexAttrib4dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4fNV = (pfn_glVertexAttrib4fNV)GetFunctionPointer("glVertexAttrib4fNV");
    if ( QGLglVertexAttrib4fNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4fvNV = (pfn_glVertexAttrib4fvNV)GetFunctionPointer("glVertexAttrib4fvNV");
    if ( QGLglVertexAttrib4fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4sNV = (pfn_glVertexAttrib4sNV)GetFunctionPointer("glVertexAttrib4sNV");
    if ( QGLglVertexAttrib4sNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4svNV = (pfn_glVertexAttrib4svNV)GetFunctionPointer("glVertexAttrib4svNV");
    if ( QGLglVertexAttrib4svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4ubNV = (pfn_glVertexAttrib4ubNV)GetFunctionPointer("glVertexAttrib4ubNV");
    if ( QGLglVertexAttrib4ubNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttrib4ubvNV = (pfn_glVertexAttrib4ubvNV)GetFunctionPointer("glVertexAttrib4ubvNV");
    if ( QGLglVertexAttrib4ubvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs1dvNV = (pfn_glVertexAttribs1dvNV)GetFunctionPointer("glVertexAttribs1dvNV");
    if ( QGLglVertexAttribs1dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs1fvNV = (pfn_glVertexAttribs1fvNV)GetFunctionPointer("glVertexAttribs1fvNV");
    if ( QGLglVertexAttribs1fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs1svNV = (pfn_glVertexAttribs1svNV)GetFunctionPointer("glVertexAttribs1svNV");
    if ( QGLglVertexAttribs1svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs2dvNV = (pfn_glVertexAttribs2dvNV)GetFunctionPointer("glVertexAttribs2dvNV");
    if ( QGLglVertexAttribs2dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs2fvNV = (pfn_glVertexAttribs2fvNV)GetFunctionPointer("glVertexAttribs2fvNV");
    if ( QGLglVertexAttribs2fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs2svNV = (pfn_glVertexAttribs2svNV)GetFunctionPointer("glVertexAttribs2svNV");
    if ( QGLglVertexAttribs2svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs3dvNV = (pfn_glVertexAttribs3dvNV)GetFunctionPointer("glVertexAttribs3dvNV");
    if ( QGLglVertexAttribs3dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs3fvNV = (pfn_glVertexAttribs3fvNV)GetFunctionPointer("glVertexAttribs3fvNV");
    if ( QGLglVertexAttribs3fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs3svNV = (pfn_glVertexAttribs3svNV)GetFunctionPointer("glVertexAttribs3svNV");
    if ( QGLglVertexAttribs3svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs4dvNV = (pfn_glVertexAttribs4dvNV)GetFunctionPointer("glVertexAttribs4dvNV");
    if ( QGLglVertexAttribs4dvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs4fvNV = (pfn_glVertexAttribs4fvNV)GetFunctionPointer("glVertexAttribs4fvNV");
    if ( QGLglVertexAttribs4fvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs4svNV = (pfn_glVertexAttribs4svNV)GetFunctionPointer("glVertexAttribs4svNV");
    if ( QGLglVertexAttribs4svNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    QGLglVertexAttribs4ubvNV = (pfn_glVertexAttribs4ubvNV)GetFunctionPointer("glVertexAttribs4ubvNV");
    if ( QGLglVertexAttribs4ubvNV )
        ++QGLNumFunctions_GL_NV_vertex_program;

    if ( QGLNumFunctions_GL_NV_vertex_program == 64 )
        QGLExists_GL_NV_vertex_program = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_vertex_program = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_program ()
{
    QGLNumFunctions_GL_NV_vertex_program = 0;
    QGLExists_GL_NV_vertex_program = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_texture_coordinate_clamp;
int QGLNumFunctions_GL_SGIX_texture_coordinate_clamp;
#ifdef QGL_DEFINED_GL_SGIX_texture_coordinate_clamp
static void Init_GL_SGIX_texture_coordinate_clamp ()
{
    QGLNumFunctions_GL_SGIX_texture_coordinate_clamp = 0;
    QGLExists_GL_SGIX_texture_coordinate_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_texture_coordinate_clamp ()
{
    QGLNumFunctions_GL_SGIX_texture_coordinate_clamp = 0;
    QGLExists_GL_SGIX_texture_coordinate_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SGIX_scalebias_hint;
int QGLNumFunctions_GL_SGIX_scalebias_hint;
#ifdef QGL_DEFINED_GL_SGIX_scalebias_hint
static void Init_GL_SGIX_scalebias_hint ()
{
    QGLNumFunctions_GL_SGIX_scalebias_hint = 0;
    QGLExists_GL_SGIX_scalebias_hint = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SGIX_scalebias_hint ()
{
    QGLNumFunctions_GL_SGIX_scalebias_hint = 0;
    QGLExists_GL_SGIX_scalebias_hint = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_OML_interlace;
int QGLNumFunctions_GL_OML_interlace;
#ifdef QGL_DEFINED_GL_OML_interlace
static void Init_GL_OML_interlace ()
{
    QGLNumFunctions_GL_OML_interlace = 0;
    QGLExists_GL_OML_interlace = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_OML_interlace ()
{
    QGLNumFunctions_GL_OML_interlace = 0;
    QGLExists_GL_OML_interlace = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_OML_subsample;
int QGLNumFunctions_GL_OML_subsample;
#ifdef QGL_DEFINED_GL_OML_subsample
static void Init_GL_OML_subsample ()
{
    QGLNumFunctions_GL_OML_subsample = 0;
    QGLExists_GL_OML_subsample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_OML_subsample ()
{
    QGLNumFunctions_GL_OML_subsample = 0;
    QGLExists_GL_OML_subsample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_OML_resample;
int QGLNumFunctions_GL_OML_resample;
#ifdef QGL_DEFINED_GL_OML_resample
static void Init_GL_OML_resample ()
{
    QGLNumFunctions_GL_OML_resample = 0;
    QGLExists_GL_OML_resample = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_OML_resample ()
{
    QGLNumFunctions_GL_OML_resample = 0;
    QGLExists_GL_OML_resample = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_copy_depth_to_color;
int QGLNumFunctions_GL_NV_copy_depth_to_color;
#ifdef QGL_DEFINED_GL_NV_copy_depth_to_color
static void Init_GL_NV_copy_depth_to_color ()
{
    QGLNumFunctions_GL_NV_copy_depth_to_color = 0;
    QGLExists_GL_NV_copy_depth_to_color = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_copy_depth_to_color ()
{
    QGLNumFunctions_GL_NV_copy_depth_to_color = 0;
    QGLExists_GL_NV_copy_depth_to_color = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_envmap_bumpmap;
int QGLNumFunctions_GL_ATI_envmap_bumpmap;
#ifdef QGL_DEFINED_GL_ATI_envmap_bumpmap
pfn_glTexBumpParameterivATI QGLglTexBumpParameterivATI = 0;
pfn_glTexBumpParameterfvATI QGLglTexBumpParameterfvATI = 0;
pfn_glGetTexBumpParameterivATI QGLglGetTexBumpParameterivATI = 0;
pfn_glGetTexBumpParameterfvATI QGLglGetTexBumpParameterfvATI = 0;

static void Init_GL_ATI_envmap_bumpmap ()
{
    QGLNumFunctions_GL_ATI_envmap_bumpmap = 0;

    QGLglTexBumpParameterivATI = (pfn_glTexBumpParameterivATI)GetFunctionPointer("glTexBumpParameterivATI");
    if ( QGLglTexBumpParameterivATI )
        ++QGLNumFunctions_GL_ATI_envmap_bumpmap;

    QGLglTexBumpParameterfvATI = (pfn_glTexBumpParameterfvATI)GetFunctionPointer("glTexBumpParameterfvATI");
    if ( QGLglTexBumpParameterfvATI )
        ++QGLNumFunctions_GL_ATI_envmap_bumpmap;

    QGLglGetTexBumpParameterivATI = (pfn_glGetTexBumpParameterivATI)GetFunctionPointer("glGetTexBumpParameterivATI");
    if ( QGLglGetTexBumpParameterivATI )
        ++QGLNumFunctions_GL_ATI_envmap_bumpmap;

    QGLglGetTexBumpParameterfvATI = (pfn_glGetTexBumpParameterfvATI)GetFunctionPointer("glGetTexBumpParameterfvATI");
    if ( QGLglGetTexBumpParameterfvATI )
        ++QGLNumFunctions_GL_ATI_envmap_bumpmap;

    if ( QGLNumFunctions_GL_ATI_envmap_bumpmap == 4 )
        QGLExists_GL_ATI_envmap_bumpmap = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_envmap_bumpmap = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_envmap_bumpmap ()
{
    QGLNumFunctions_GL_ATI_envmap_bumpmap = 0;
    QGLExists_GL_ATI_envmap_bumpmap = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_fragment_shader;
int QGLNumFunctions_GL_ATI_fragment_shader;
#ifdef QGL_DEFINED_GL_ATI_fragment_shader
pfn_glGenFragmentShadersATI QGLglGenFragmentShadersATI = 0;
pfn_glBindFragmentShaderATI QGLglBindFragmentShaderATI = 0;
pfn_glDeleteFragmentShaderATI QGLglDeleteFragmentShaderATI = 0;
pfn_glBeginFragmentShaderATI QGLglBeginFragmentShaderATI = 0;
pfn_glEndFragmentShaderATI QGLglEndFragmentShaderATI = 0;
pfn_glPassTexCoordATI QGLglPassTexCoordATI = 0;
pfn_glSampleMapATI QGLglSampleMapATI = 0;
pfn_glColorFragmentOp1ATI QGLglColorFragmentOp1ATI = 0;
pfn_glColorFragmentOp2ATI QGLglColorFragmentOp2ATI = 0;
pfn_glColorFragmentOp3ATI QGLglColorFragmentOp3ATI = 0;
pfn_glAlphaFragmentOp1ATI QGLglAlphaFragmentOp1ATI = 0;
pfn_glAlphaFragmentOp2ATI QGLglAlphaFragmentOp2ATI = 0;
pfn_glAlphaFragmentOp3ATI QGLglAlphaFragmentOp3ATI = 0;
pfn_glSetFragmentShaderConstantATI QGLglSetFragmentShaderConstantATI = 0;

static void Init_GL_ATI_fragment_shader ()
{
    QGLNumFunctions_GL_ATI_fragment_shader = 0;

    QGLglGenFragmentShadersATI = (pfn_glGenFragmentShadersATI)GetFunctionPointer("glGenFragmentShadersATI");
    if ( QGLglGenFragmentShadersATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglBindFragmentShaderATI = (pfn_glBindFragmentShaderATI)GetFunctionPointer("glBindFragmentShaderATI");
    if ( QGLglBindFragmentShaderATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglDeleteFragmentShaderATI = (pfn_glDeleteFragmentShaderATI)GetFunctionPointer("glDeleteFragmentShaderATI");
    if ( QGLglDeleteFragmentShaderATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglBeginFragmentShaderATI = (pfn_glBeginFragmentShaderATI)GetFunctionPointer("glBeginFragmentShaderATI");
    if ( QGLglBeginFragmentShaderATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglEndFragmentShaderATI = (pfn_glEndFragmentShaderATI)GetFunctionPointer("glEndFragmentShaderATI");
    if ( QGLglEndFragmentShaderATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglPassTexCoordATI = (pfn_glPassTexCoordATI)GetFunctionPointer("glPassTexCoordATI");
    if ( QGLglPassTexCoordATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglSampleMapATI = (pfn_glSampleMapATI)GetFunctionPointer("glSampleMapATI");
    if ( QGLglSampleMapATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglColorFragmentOp1ATI = (pfn_glColorFragmentOp1ATI)GetFunctionPointer("glColorFragmentOp1ATI");
    if ( QGLglColorFragmentOp1ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglColorFragmentOp2ATI = (pfn_glColorFragmentOp2ATI)GetFunctionPointer("glColorFragmentOp2ATI");
    if ( QGLglColorFragmentOp2ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglColorFragmentOp3ATI = (pfn_glColorFragmentOp3ATI)GetFunctionPointer("glColorFragmentOp3ATI");
    if ( QGLglColorFragmentOp3ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglAlphaFragmentOp1ATI = (pfn_glAlphaFragmentOp1ATI)GetFunctionPointer("glAlphaFragmentOp1ATI");
    if ( QGLglAlphaFragmentOp1ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglAlphaFragmentOp2ATI = (pfn_glAlphaFragmentOp2ATI)GetFunctionPointer("glAlphaFragmentOp2ATI");
    if ( QGLglAlphaFragmentOp2ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglAlphaFragmentOp3ATI = (pfn_glAlphaFragmentOp3ATI)GetFunctionPointer("glAlphaFragmentOp3ATI");
    if ( QGLglAlphaFragmentOp3ATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    QGLglSetFragmentShaderConstantATI = (pfn_glSetFragmentShaderConstantATI)GetFunctionPointer("glSetFragmentShaderConstantATI");
    if ( QGLglSetFragmentShaderConstantATI )
        ++QGLNumFunctions_GL_ATI_fragment_shader;

    if ( QGLNumFunctions_GL_ATI_fragment_shader == 14 )
        QGLExists_GL_ATI_fragment_shader = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_fragment_shader = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_fragment_shader ()
{
    QGLNumFunctions_GL_ATI_fragment_shader = 0;
    QGLExists_GL_ATI_fragment_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_pn_triangles;
int QGLNumFunctions_GL_ATI_pn_triangles;
#ifdef QGL_DEFINED_GL_ATI_pn_triangles
pfn_glPNTrianglesiATI QGLglPNTrianglesiATI = 0;
pfn_glPNTrianglesfATI QGLglPNTrianglesfATI = 0;

static void Init_GL_ATI_pn_triangles ()
{
    QGLNumFunctions_GL_ATI_pn_triangles = 0;

    QGLglPNTrianglesiATI = (pfn_glPNTrianglesiATI)GetFunctionPointer("glPNTrianglesiATI");
    if ( QGLglPNTrianglesiATI )
        ++QGLNumFunctions_GL_ATI_pn_triangles;

    QGLglPNTrianglesfATI = (pfn_glPNTrianglesfATI)GetFunctionPointer("glPNTrianglesfATI");
    if ( QGLglPNTrianglesfATI )
        ++QGLNumFunctions_GL_ATI_pn_triangles;

    if ( QGLNumFunctions_GL_ATI_pn_triangles == 2 )
        QGLExists_GL_ATI_pn_triangles = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_pn_triangles = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_pn_triangles ()
{
    QGLNumFunctions_GL_ATI_pn_triangles = 0;
    QGLExists_GL_ATI_pn_triangles = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_vertex_array_object;
int QGLNumFunctions_GL_ATI_vertex_array_object;
#ifdef QGL_DEFINED_GL_ATI_vertex_array_object
pfn_glNewObjectBufferATI QGLglNewObjectBufferATI = 0;
pfn_glIsObjectBufferATI QGLglIsObjectBufferATI = 0;
pfn_glUpdateObjectBufferATI QGLglUpdateObjectBufferATI = 0;
pfn_glGetObjectBufferfvATI QGLglGetObjectBufferfvATI = 0;
pfn_glGetObjectBufferivATI QGLglGetObjectBufferivATI = 0;
pfn_glFreeObjectBufferATI QGLglFreeObjectBufferATI = 0;
pfn_glArrayObjectATI QGLglArrayObjectATI = 0;
pfn_glGetArrayObjectfvATI QGLglGetArrayObjectfvATI = 0;
pfn_glGetArrayObjectivATI QGLglGetArrayObjectivATI = 0;
pfn_glVariantArrayObjectATI QGLglVariantArrayObjectATI = 0;
pfn_glGetVariantArrayObjectfvATI QGLglGetVariantArrayObjectfvATI = 0;
pfn_glGetVariantArrayObjectivATI QGLglGetVariantArrayObjectivATI = 0;

static void Init_GL_ATI_vertex_array_object ()
{
    QGLNumFunctions_GL_ATI_vertex_array_object = 0;

    QGLglNewObjectBufferATI = (pfn_glNewObjectBufferATI)GetFunctionPointer("glNewObjectBufferATI");
    if ( QGLglNewObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglIsObjectBufferATI = (pfn_glIsObjectBufferATI)GetFunctionPointer("glIsObjectBufferATI");
    if ( QGLglIsObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglUpdateObjectBufferATI = (pfn_glUpdateObjectBufferATI)GetFunctionPointer("glUpdateObjectBufferATI");
    if ( QGLglUpdateObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetObjectBufferfvATI = (pfn_glGetObjectBufferfvATI)GetFunctionPointer("glGetObjectBufferfvATI");
    if ( QGLglGetObjectBufferfvATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetObjectBufferivATI = (pfn_glGetObjectBufferivATI)GetFunctionPointer("glGetObjectBufferivATI");
    if ( QGLglGetObjectBufferivATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglFreeObjectBufferATI = (pfn_glFreeObjectBufferATI)GetFunctionPointer("glFreeObjectBufferATI");
    if ( QGLglFreeObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglArrayObjectATI = (pfn_glArrayObjectATI)GetFunctionPointer("glArrayObjectATI");
    if ( QGLglArrayObjectATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetArrayObjectfvATI = (pfn_glGetArrayObjectfvATI)GetFunctionPointer("glGetArrayObjectfvATI");
    if ( QGLglGetArrayObjectfvATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetArrayObjectivATI = (pfn_glGetArrayObjectivATI)GetFunctionPointer("glGetArrayObjectivATI");
    if ( QGLglGetArrayObjectivATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglVariantArrayObjectATI = (pfn_glVariantArrayObjectATI)GetFunctionPointer("glVariantArrayObjectATI");
    if ( QGLglVariantArrayObjectATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetVariantArrayObjectfvATI = (pfn_glGetVariantArrayObjectfvATI)GetFunctionPointer("glGetVariantArrayObjectfvATI");
    if ( QGLglGetVariantArrayObjectfvATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    QGLglGetVariantArrayObjectivATI = (pfn_glGetVariantArrayObjectivATI)GetFunctionPointer("glGetVariantArrayObjectivATI");
    if ( QGLglGetVariantArrayObjectivATI )
        ++QGLNumFunctions_GL_ATI_vertex_array_object;

    if ( QGLNumFunctions_GL_ATI_vertex_array_object == 12 )
        QGLExists_GL_ATI_vertex_array_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_vertex_array_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_vertex_array_object ()
{
    QGLNumFunctions_GL_ATI_vertex_array_object = 0;
    QGLExists_GL_ATI_vertex_array_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_vertex_shader;
int QGLNumFunctions_GL_EXT_vertex_shader;
#ifdef QGL_DEFINED_GL_EXT_vertex_shader
pfn_glBeginVertexShaderEXT QGLglBeginVertexShaderEXT = 0;
pfn_glEndVertexShaderEXT QGLglEndVertexShaderEXT = 0;
pfn_glBindVertexShaderEXT QGLglBindVertexShaderEXT = 0;
pfn_glGenVertexShadersEXT QGLglGenVertexShadersEXT = 0;
pfn_glDeleteVertexShaderEXT QGLglDeleteVertexShaderEXT = 0;
pfn_glShaderOp1EXT QGLglShaderOp1EXT = 0;
pfn_glShaderOp2EXT QGLglShaderOp2EXT = 0;
pfn_glShaderOp3EXT QGLglShaderOp3EXT = 0;
pfn_glSwizzleEXT QGLglSwizzleEXT = 0;
pfn_glWriteMaskEXT QGLglWriteMaskEXT = 0;
pfn_glInsertComponentEXT QGLglInsertComponentEXT = 0;
pfn_glExtractComponentEXT QGLglExtractComponentEXT = 0;
pfn_glGenSymbolsEXT QGLglGenSymbolsEXT = 0;
pfn_glSetInvariantEXT QGLglSetInvariantEXT = 0;
pfn_glSetLocalConstantEXT QGLglSetLocalConstantEXT = 0;
pfn_glVariantbvEXT QGLglVariantbvEXT = 0;
pfn_glVariantsvEXT QGLglVariantsvEXT = 0;
pfn_glVariantivEXT QGLglVariantivEXT = 0;
pfn_glVariantfvEXT QGLglVariantfvEXT = 0;
pfn_glVariantdvEXT QGLglVariantdvEXT = 0;
pfn_glVariantubvEXT QGLglVariantubvEXT = 0;
pfn_glVariantusvEXT QGLglVariantusvEXT = 0;
pfn_glVariantuivEXT QGLglVariantuivEXT = 0;
pfn_glVariantPointerEXT QGLglVariantPointerEXT = 0;
pfn_glEnableVariantClientStateEXT QGLglEnableVariantClientStateEXT = 0;
pfn_glDisableVariantClientStateEXT QGLglDisableVariantClientStateEXT = 0;
pfn_glBindLightParameterEXT QGLglBindLightParameterEXT = 0;
pfn_glBindMaterialParameterEXT QGLglBindMaterialParameterEXT = 0;
pfn_glBindTexGenParameterEXT QGLglBindTexGenParameterEXT = 0;
pfn_glBindTextureUnitParameterEXT QGLglBindTextureUnitParameterEXT = 0;
pfn_glBindParameterEXT QGLglBindParameterEXT = 0;
pfn_glIsVariantEnabledEXT QGLglIsVariantEnabledEXT = 0;
pfn_glGetVariantBooleanvEXT QGLglGetVariantBooleanvEXT = 0;
pfn_glGetVariantIntegervEXT QGLglGetVariantIntegervEXT = 0;
pfn_glGetVariantFloatvEXT QGLglGetVariantFloatvEXT = 0;
pfn_glGetVariantPointervEXT QGLglGetVariantPointervEXT = 0;
pfn_glGetInvariantBooleanvEXT QGLglGetInvariantBooleanvEXT = 0;
pfn_glGetInvariantIntegervEXT QGLglGetInvariantIntegervEXT = 0;
pfn_glGetInvariantFloatvEXT QGLglGetInvariantFloatvEXT = 0;
pfn_glGetLocalConstantBooleanvEXT QGLglGetLocalConstantBooleanvEXT = 0;
pfn_glGetLocalConstantIntegervEXT QGLglGetLocalConstantIntegervEXT = 0;
pfn_glGetLocalConstantFloatvEXT QGLglGetLocalConstantFloatvEXT = 0;

static void Init_GL_EXT_vertex_shader ()
{
    QGLNumFunctions_GL_EXT_vertex_shader = 0;

    QGLglBeginVertexShaderEXT = (pfn_glBeginVertexShaderEXT)GetFunctionPointer("glBeginVertexShaderEXT");
    if ( QGLglBeginVertexShaderEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglEndVertexShaderEXT = (pfn_glEndVertexShaderEXT)GetFunctionPointer("glEndVertexShaderEXT");
    if ( QGLglEndVertexShaderEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindVertexShaderEXT = (pfn_glBindVertexShaderEXT)GetFunctionPointer("glBindVertexShaderEXT");
    if ( QGLglBindVertexShaderEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGenVertexShadersEXT = (pfn_glGenVertexShadersEXT)GetFunctionPointer("glGenVertexShadersEXT");
    if ( QGLglGenVertexShadersEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglDeleteVertexShaderEXT = (pfn_glDeleteVertexShaderEXT)GetFunctionPointer("glDeleteVertexShaderEXT");
    if ( QGLglDeleteVertexShaderEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglShaderOp1EXT = (pfn_glShaderOp1EXT)GetFunctionPointer("glShaderOp1EXT");
    if ( QGLglShaderOp1EXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglShaderOp2EXT = (pfn_glShaderOp2EXT)GetFunctionPointer("glShaderOp2EXT");
    if ( QGLglShaderOp2EXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglShaderOp3EXT = (pfn_glShaderOp3EXT)GetFunctionPointer("glShaderOp3EXT");
    if ( QGLglShaderOp3EXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglSwizzleEXT = (pfn_glSwizzleEXT)GetFunctionPointer("glSwizzleEXT");
    if ( QGLglSwizzleEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglWriteMaskEXT = (pfn_glWriteMaskEXT)GetFunctionPointer("glWriteMaskEXT");
    if ( QGLglWriteMaskEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglInsertComponentEXT = (pfn_glInsertComponentEXT)GetFunctionPointer("glInsertComponentEXT");
    if ( QGLglInsertComponentEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglExtractComponentEXT = (pfn_glExtractComponentEXT)GetFunctionPointer("glExtractComponentEXT");
    if ( QGLglExtractComponentEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGenSymbolsEXT = (pfn_glGenSymbolsEXT)GetFunctionPointer("glGenSymbolsEXT");
    if ( QGLglGenSymbolsEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglSetInvariantEXT = (pfn_glSetInvariantEXT)GetFunctionPointer("glSetInvariantEXT");
    if ( QGLglSetInvariantEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglSetLocalConstantEXT = (pfn_glSetLocalConstantEXT)GetFunctionPointer("glSetLocalConstantEXT");
    if ( QGLglSetLocalConstantEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantbvEXT = (pfn_glVariantbvEXT)GetFunctionPointer("glVariantbvEXT");
    if ( QGLglVariantbvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantsvEXT = (pfn_glVariantsvEXT)GetFunctionPointer("glVariantsvEXT");
    if ( QGLglVariantsvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantivEXT = (pfn_glVariantivEXT)GetFunctionPointer("glVariantivEXT");
    if ( QGLglVariantivEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantfvEXT = (pfn_glVariantfvEXT)GetFunctionPointer("glVariantfvEXT");
    if ( QGLglVariantfvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantdvEXT = (pfn_glVariantdvEXT)GetFunctionPointer("glVariantdvEXT");
    if ( QGLglVariantdvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantubvEXT = (pfn_glVariantubvEXT)GetFunctionPointer("glVariantubvEXT");
    if ( QGLglVariantubvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantusvEXT = (pfn_glVariantusvEXT)GetFunctionPointer("glVariantusvEXT");
    if ( QGLglVariantusvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantuivEXT = (pfn_glVariantuivEXT)GetFunctionPointer("glVariantuivEXT");
    if ( QGLglVariantuivEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglVariantPointerEXT = (pfn_glVariantPointerEXT)GetFunctionPointer("glVariantPointerEXT");
    if ( QGLglVariantPointerEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglEnableVariantClientStateEXT = (pfn_glEnableVariantClientStateEXT)GetFunctionPointer("glEnableVariantClientStateEXT");
    if ( QGLglEnableVariantClientStateEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglDisableVariantClientStateEXT = (pfn_glDisableVariantClientStateEXT)GetFunctionPointer("glDisableVariantClientStateEXT");
    if ( QGLglDisableVariantClientStateEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindLightParameterEXT = (pfn_glBindLightParameterEXT)GetFunctionPointer("glBindLightParameterEXT");
    if ( QGLglBindLightParameterEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindMaterialParameterEXT = (pfn_glBindMaterialParameterEXT)GetFunctionPointer("glBindMaterialParameterEXT");
    if ( QGLglBindMaterialParameterEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindTexGenParameterEXT = (pfn_glBindTexGenParameterEXT)GetFunctionPointer("glBindTexGenParameterEXT");
    if ( QGLglBindTexGenParameterEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindTextureUnitParameterEXT = (pfn_glBindTextureUnitParameterEXT)GetFunctionPointer("glBindTextureUnitParameterEXT");
    if ( QGLglBindTextureUnitParameterEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglBindParameterEXT = (pfn_glBindParameterEXT)GetFunctionPointer("glBindParameterEXT");
    if ( QGLglBindParameterEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglIsVariantEnabledEXT = (pfn_glIsVariantEnabledEXT)GetFunctionPointer("glIsVariantEnabledEXT");
    if ( QGLglIsVariantEnabledEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetVariantBooleanvEXT = (pfn_glGetVariantBooleanvEXT)GetFunctionPointer("glGetVariantBooleanvEXT");
    if ( QGLglGetVariantBooleanvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetVariantIntegervEXT = (pfn_glGetVariantIntegervEXT)GetFunctionPointer("glGetVariantIntegervEXT");
    if ( QGLglGetVariantIntegervEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetVariantFloatvEXT = (pfn_glGetVariantFloatvEXT)GetFunctionPointer("glGetVariantFloatvEXT");
    if ( QGLglGetVariantFloatvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetVariantPointervEXT = (pfn_glGetVariantPointervEXT)GetFunctionPointer("glGetVariantPointervEXT");
    if ( QGLglGetVariantPointervEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetInvariantBooleanvEXT = (pfn_glGetInvariantBooleanvEXT)GetFunctionPointer("glGetInvariantBooleanvEXT");
    if ( QGLglGetInvariantBooleanvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetInvariantIntegervEXT = (pfn_glGetInvariantIntegervEXT)GetFunctionPointer("glGetInvariantIntegervEXT");
    if ( QGLglGetInvariantIntegervEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetInvariantFloatvEXT = (pfn_glGetInvariantFloatvEXT)GetFunctionPointer("glGetInvariantFloatvEXT");
    if ( QGLglGetInvariantFloatvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetLocalConstantBooleanvEXT = (pfn_glGetLocalConstantBooleanvEXT)GetFunctionPointer("glGetLocalConstantBooleanvEXT");
    if ( QGLglGetLocalConstantBooleanvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetLocalConstantIntegervEXT = (pfn_glGetLocalConstantIntegervEXT)GetFunctionPointer("glGetLocalConstantIntegervEXT");
    if ( QGLglGetLocalConstantIntegervEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    QGLglGetLocalConstantFloatvEXT = (pfn_glGetLocalConstantFloatvEXT)GetFunctionPointer("glGetLocalConstantFloatvEXT");
    if ( QGLglGetLocalConstantFloatvEXT )
        ++QGLNumFunctions_GL_EXT_vertex_shader;

    if ( QGLNumFunctions_GL_EXT_vertex_shader == 42 )
        QGLExists_GL_EXT_vertex_shader = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_vertex_shader = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_vertex_shader ()
{
    QGLNumFunctions_GL_EXT_vertex_shader = 0;
    QGLExists_GL_EXT_vertex_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_vertex_streams;
int QGLNumFunctions_GL_ATI_vertex_streams;
#ifdef QGL_DEFINED_GL_ATI_vertex_streams
pfn_glVertexStream1sATI QGLglVertexStream1sATI = 0;
pfn_glVertexStream1svATI QGLglVertexStream1svATI = 0;
pfn_glVertexStream1iATI QGLglVertexStream1iATI = 0;
pfn_glVertexStream1ivATI QGLglVertexStream1ivATI = 0;
pfn_glVertexStream1fATI QGLglVertexStream1fATI = 0;
pfn_glVertexStream1fvATI QGLglVertexStream1fvATI = 0;
pfn_glVertexStream1dATI QGLglVertexStream1dATI = 0;
pfn_glVertexStream1dvATI QGLglVertexStream1dvATI = 0;
pfn_glVertexStream2sATI QGLglVertexStream2sATI = 0;
pfn_glVertexStream2svATI QGLglVertexStream2svATI = 0;
pfn_glVertexStream2iATI QGLglVertexStream2iATI = 0;
pfn_glVertexStream2ivATI QGLglVertexStream2ivATI = 0;
pfn_glVertexStream2fATI QGLglVertexStream2fATI = 0;
pfn_glVertexStream2fvATI QGLglVertexStream2fvATI = 0;
pfn_glVertexStream2dATI QGLglVertexStream2dATI = 0;
pfn_glVertexStream2dvATI QGLglVertexStream2dvATI = 0;
pfn_glVertexStream3sATI QGLglVertexStream3sATI = 0;
pfn_glVertexStream3svATI QGLglVertexStream3svATI = 0;
pfn_glVertexStream3iATI QGLglVertexStream3iATI = 0;
pfn_glVertexStream3ivATI QGLglVertexStream3ivATI = 0;
pfn_glVertexStream3fATI QGLglVertexStream3fATI = 0;
pfn_glVertexStream3fvATI QGLglVertexStream3fvATI = 0;
pfn_glVertexStream3dATI QGLglVertexStream3dATI = 0;
pfn_glVertexStream3dvATI QGLglVertexStream3dvATI = 0;
pfn_glVertexStream4sATI QGLglVertexStream4sATI = 0;
pfn_glVertexStream4svATI QGLglVertexStream4svATI = 0;
pfn_glVertexStream4iATI QGLglVertexStream4iATI = 0;
pfn_glVertexStream4ivATI QGLglVertexStream4ivATI = 0;
pfn_glVertexStream4fATI QGLglVertexStream4fATI = 0;
pfn_glVertexStream4fvATI QGLglVertexStream4fvATI = 0;
pfn_glVertexStream4dATI QGLglVertexStream4dATI = 0;
pfn_glVertexStream4dvATI QGLglVertexStream4dvATI = 0;
pfn_glNormalStream3bATI QGLglNormalStream3bATI = 0;
pfn_glNormalStream3bvATI QGLglNormalStream3bvATI = 0;
pfn_glNormalStream3sATI QGLglNormalStream3sATI = 0;
pfn_glNormalStream3svATI QGLglNormalStream3svATI = 0;
pfn_glNormalStream3iATI QGLglNormalStream3iATI = 0;
pfn_glNormalStream3ivATI QGLglNormalStream3ivATI = 0;
pfn_glNormalStream3fATI QGLglNormalStream3fATI = 0;
pfn_glNormalStream3fvATI QGLglNormalStream3fvATI = 0;
pfn_glNormalStream3dATI QGLglNormalStream3dATI = 0;
pfn_glNormalStream3dvATI QGLglNormalStream3dvATI = 0;
pfn_glClientActiveVertexStreamATI QGLglClientActiveVertexStreamATI = 0;
pfn_glVertexBlendEnviATI QGLglVertexBlendEnviATI = 0;
pfn_glVertexBlendEnvfATI QGLglVertexBlendEnvfATI = 0;

static void Init_GL_ATI_vertex_streams ()
{
    QGLNumFunctions_GL_ATI_vertex_streams = 0;

    QGLglVertexStream1sATI = (pfn_glVertexStream1sATI)GetFunctionPointer("glVertexStream1sATI");
    if ( QGLglVertexStream1sATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1svATI = (pfn_glVertexStream1svATI)GetFunctionPointer("glVertexStream1svATI");
    if ( QGLglVertexStream1svATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1iATI = (pfn_glVertexStream1iATI)GetFunctionPointer("glVertexStream1iATI");
    if ( QGLglVertexStream1iATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1ivATI = (pfn_glVertexStream1ivATI)GetFunctionPointer("glVertexStream1ivATI");
    if ( QGLglVertexStream1ivATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1fATI = (pfn_glVertexStream1fATI)GetFunctionPointer("glVertexStream1fATI");
    if ( QGLglVertexStream1fATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1fvATI = (pfn_glVertexStream1fvATI)GetFunctionPointer("glVertexStream1fvATI");
    if ( QGLglVertexStream1fvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1dATI = (pfn_glVertexStream1dATI)GetFunctionPointer("glVertexStream1dATI");
    if ( QGLglVertexStream1dATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream1dvATI = (pfn_glVertexStream1dvATI)GetFunctionPointer("glVertexStream1dvATI");
    if ( QGLglVertexStream1dvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2sATI = (pfn_glVertexStream2sATI)GetFunctionPointer("glVertexStream2sATI");
    if ( QGLglVertexStream2sATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2svATI = (pfn_glVertexStream2svATI)GetFunctionPointer("glVertexStream2svATI");
    if ( QGLglVertexStream2svATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2iATI = (pfn_glVertexStream2iATI)GetFunctionPointer("glVertexStream2iATI");
    if ( QGLglVertexStream2iATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2ivATI = (pfn_glVertexStream2ivATI)GetFunctionPointer("glVertexStream2ivATI");
    if ( QGLglVertexStream2ivATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2fATI = (pfn_glVertexStream2fATI)GetFunctionPointer("glVertexStream2fATI");
    if ( QGLglVertexStream2fATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2fvATI = (pfn_glVertexStream2fvATI)GetFunctionPointer("glVertexStream2fvATI");
    if ( QGLglVertexStream2fvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2dATI = (pfn_glVertexStream2dATI)GetFunctionPointer("glVertexStream2dATI");
    if ( QGLglVertexStream2dATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream2dvATI = (pfn_glVertexStream2dvATI)GetFunctionPointer("glVertexStream2dvATI");
    if ( QGLglVertexStream2dvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3sATI = (pfn_glVertexStream3sATI)GetFunctionPointer("glVertexStream3sATI");
    if ( QGLglVertexStream3sATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3svATI = (pfn_glVertexStream3svATI)GetFunctionPointer("glVertexStream3svATI");
    if ( QGLglVertexStream3svATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3iATI = (pfn_glVertexStream3iATI)GetFunctionPointer("glVertexStream3iATI");
    if ( QGLglVertexStream3iATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3ivATI = (pfn_glVertexStream3ivATI)GetFunctionPointer("glVertexStream3ivATI");
    if ( QGLglVertexStream3ivATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3fATI = (pfn_glVertexStream3fATI)GetFunctionPointer("glVertexStream3fATI");
    if ( QGLglVertexStream3fATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3fvATI = (pfn_glVertexStream3fvATI)GetFunctionPointer("glVertexStream3fvATI");
    if ( QGLglVertexStream3fvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3dATI = (pfn_glVertexStream3dATI)GetFunctionPointer("glVertexStream3dATI");
    if ( QGLglVertexStream3dATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream3dvATI = (pfn_glVertexStream3dvATI)GetFunctionPointer("glVertexStream3dvATI");
    if ( QGLglVertexStream3dvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4sATI = (pfn_glVertexStream4sATI)GetFunctionPointer("glVertexStream4sATI");
    if ( QGLglVertexStream4sATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4svATI = (pfn_glVertexStream4svATI)GetFunctionPointer("glVertexStream4svATI");
    if ( QGLglVertexStream4svATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4iATI = (pfn_glVertexStream4iATI)GetFunctionPointer("glVertexStream4iATI");
    if ( QGLglVertexStream4iATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4ivATI = (pfn_glVertexStream4ivATI)GetFunctionPointer("glVertexStream4ivATI");
    if ( QGLglVertexStream4ivATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4fATI = (pfn_glVertexStream4fATI)GetFunctionPointer("glVertexStream4fATI");
    if ( QGLglVertexStream4fATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4fvATI = (pfn_glVertexStream4fvATI)GetFunctionPointer("glVertexStream4fvATI");
    if ( QGLglVertexStream4fvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4dATI = (pfn_glVertexStream4dATI)GetFunctionPointer("glVertexStream4dATI");
    if ( QGLglVertexStream4dATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexStream4dvATI = (pfn_glVertexStream4dvATI)GetFunctionPointer("glVertexStream4dvATI");
    if ( QGLglVertexStream4dvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3bATI = (pfn_glNormalStream3bATI)GetFunctionPointer("glNormalStream3bATI");
    if ( QGLglNormalStream3bATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3bvATI = (pfn_glNormalStream3bvATI)GetFunctionPointer("glNormalStream3bvATI");
    if ( QGLglNormalStream3bvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3sATI = (pfn_glNormalStream3sATI)GetFunctionPointer("glNormalStream3sATI");
    if ( QGLglNormalStream3sATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3svATI = (pfn_glNormalStream3svATI)GetFunctionPointer("glNormalStream3svATI");
    if ( QGLglNormalStream3svATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3iATI = (pfn_glNormalStream3iATI)GetFunctionPointer("glNormalStream3iATI");
    if ( QGLglNormalStream3iATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3ivATI = (pfn_glNormalStream3ivATI)GetFunctionPointer("glNormalStream3ivATI");
    if ( QGLglNormalStream3ivATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3fATI = (pfn_glNormalStream3fATI)GetFunctionPointer("glNormalStream3fATI");
    if ( QGLglNormalStream3fATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3fvATI = (pfn_glNormalStream3fvATI)GetFunctionPointer("glNormalStream3fvATI");
    if ( QGLglNormalStream3fvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3dATI = (pfn_glNormalStream3dATI)GetFunctionPointer("glNormalStream3dATI");
    if ( QGLglNormalStream3dATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglNormalStream3dvATI = (pfn_glNormalStream3dvATI)GetFunctionPointer("glNormalStream3dvATI");
    if ( QGLglNormalStream3dvATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglClientActiveVertexStreamATI = (pfn_glClientActiveVertexStreamATI)GetFunctionPointer("glClientActiveVertexStreamATI");
    if ( QGLglClientActiveVertexStreamATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexBlendEnviATI = (pfn_glVertexBlendEnviATI)GetFunctionPointer("glVertexBlendEnviATI");
    if ( QGLglVertexBlendEnviATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    QGLglVertexBlendEnvfATI = (pfn_glVertexBlendEnvfATI)GetFunctionPointer("glVertexBlendEnvfATI");
    if ( QGLglVertexBlendEnvfATI )
        ++QGLNumFunctions_GL_ATI_vertex_streams;

    if ( QGLNumFunctions_GL_ATI_vertex_streams == 45 )
        QGLExists_GL_ATI_vertex_streams = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_vertex_streams = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_vertex_streams ()
{
    QGLNumFunctions_GL_ATI_vertex_streams = 0;
    QGLExists_GL_ATI_vertex_streams = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_element_array;
int QGLNumFunctions_GL_ATI_element_array;
#ifdef QGL_DEFINED_GL_ATI_element_array
pfn_glElementPointerATI QGLglElementPointerATI = 0;
pfn_glDrawElementArrayATI QGLglDrawElementArrayATI = 0;
pfn_glDrawRangeElementArrayATI QGLglDrawRangeElementArrayATI = 0;

static void Init_GL_ATI_element_array ()
{
    QGLNumFunctions_GL_ATI_element_array = 0;

    QGLglElementPointerATI = (pfn_glElementPointerATI)GetFunctionPointer("glElementPointerATI");
    if ( QGLglElementPointerATI )
        ++QGLNumFunctions_GL_ATI_element_array;

    QGLglDrawElementArrayATI = (pfn_glDrawElementArrayATI)GetFunctionPointer("glDrawElementArrayATI");
    if ( QGLglDrawElementArrayATI )
        ++QGLNumFunctions_GL_ATI_element_array;

    QGLglDrawRangeElementArrayATI = (pfn_glDrawRangeElementArrayATI)GetFunctionPointer("glDrawRangeElementArrayATI");
    if ( QGLglDrawRangeElementArrayATI )
        ++QGLNumFunctions_GL_ATI_element_array;

    if ( QGLNumFunctions_GL_ATI_element_array == 3 )
        QGLExists_GL_ATI_element_array = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_element_array = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_element_array ()
{
    QGLNumFunctions_GL_ATI_element_array = 0;
    QGLExists_GL_ATI_element_array = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_mesh_array;
int QGLNumFunctions_GL_SUN_mesh_array;
#ifdef QGL_DEFINED_GL_SUN_mesh_array
pfn_glDrawMeshArraysSUN QGLglDrawMeshArraysSUN = 0;

static void Init_GL_SUN_mesh_array ()
{
    QGLNumFunctions_GL_SUN_mesh_array = 0;

    QGLglDrawMeshArraysSUN = (pfn_glDrawMeshArraysSUN)GetFunctionPointer("glDrawMeshArraysSUN");
    if ( QGLglDrawMeshArraysSUN )
        ++QGLNumFunctions_GL_SUN_mesh_array;

    if ( QGLNumFunctions_GL_SUN_mesh_array == 1 )
        QGLExists_GL_SUN_mesh_array = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_SUN_mesh_array = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_SUN_mesh_array ()
{
    QGLNumFunctions_GL_SUN_mesh_array = 0;
    QGLExists_GL_SUN_mesh_array = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_SUN_slice_accum;
int QGLNumFunctions_GL_SUN_slice_accum;
#ifdef QGL_DEFINED_GL_SUN_slice_accum
static void Init_GL_SUN_slice_accum ()
{
    QGLNumFunctions_GL_SUN_slice_accum = 0;
    QGLExists_GL_SUN_slice_accum = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_SUN_slice_accum ()
{
    QGLNumFunctions_GL_SUN_slice_accum = 0;
    QGLExists_GL_SUN_slice_accum = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_multisample_filter_hint;
int QGLNumFunctions_GL_NV_multisample_filter_hint;
#ifdef QGL_DEFINED_GL_NV_multisample_filter_hint
static void Init_GL_NV_multisample_filter_hint ()
{
    QGLNumFunctions_GL_NV_multisample_filter_hint = 0;
    QGLExists_GL_NV_multisample_filter_hint = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_multisample_filter_hint ()
{
    QGLNumFunctions_GL_NV_multisample_filter_hint = 0;
    QGLExists_GL_NV_multisample_filter_hint = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_depth_clamp;
int QGLNumFunctions_GL_NV_depth_clamp;
#ifdef QGL_DEFINED_GL_NV_depth_clamp
static void Init_GL_NV_depth_clamp ()
{
    QGLNumFunctions_GL_NV_depth_clamp = 0;
    QGLExists_GL_NV_depth_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_depth_clamp ()
{
    QGLNumFunctions_GL_NV_depth_clamp = 0;
    QGLExists_GL_NV_depth_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_occlusion_query;
int QGLNumFunctions_GL_NV_occlusion_query;
#ifdef QGL_DEFINED_GL_NV_occlusion_query
pfn_glGenOcclusionQueriesNV QGLglGenOcclusionQueriesNV = 0;
pfn_glDeleteOcclusionQueriesNV QGLglDeleteOcclusionQueriesNV = 0;
pfn_glIsOcclusionQueryNV QGLglIsOcclusionQueryNV = 0;
pfn_glBeginOcclusionQueryNV QGLglBeginOcclusionQueryNV = 0;
pfn_glEndOcclusionQueryNV QGLglEndOcclusionQueryNV = 0;
pfn_glGetOcclusionQueryivNV QGLglGetOcclusionQueryivNV = 0;
pfn_glGetOcclusionQueryuivNV QGLglGetOcclusionQueryuivNV = 0;

static void Init_GL_NV_occlusion_query ()
{
    QGLNumFunctions_GL_NV_occlusion_query = 0;

    QGLglGenOcclusionQueriesNV = (pfn_glGenOcclusionQueriesNV)GetFunctionPointer("glGenOcclusionQueriesNV");
    if ( QGLglGenOcclusionQueriesNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglDeleteOcclusionQueriesNV = (pfn_glDeleteOcclusionQueriesNV)GetFunctionPointer("glDeleteOcclusionQueriesNV");
    if ( QGLglDeleteOcclusionQueriesNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglIsOcclusionQueryNV = (pfn_glIsOcclusionQueryNV)GetFunctionPointer("glIsOcclusionQueryNV");
    if ( QGLglIsOcclusionQueryNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglBeginOcclusionQueryNV = (pfn_glBeginOcclusionQueryNV)GetFunctionPointer("glBeginOcclusionQueryNV");
    if ( QGLglBeginOcclusionQueryNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglEndOcclusionQueryNV = (pfn_glEndOcclusionQueryNV)GetFunctionPointer("glEndOcclusionQueryNV");
    if ( QGLglEndOcclusionQueryNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglGetOcclusionQueryivNV = (pfn_glGetOcclusionQueryivNV)GetFunctionPointer("glGetOcclusionQueryivNV");
    if ( QGLglGetOcclusionQueryivNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    QGLglGetOcclusionQueryuivNV = (pfn_glGetOcclusionQueryuivNV)GetFunctionPointer("glGetOcclusionQueryuivNV");
    if ( QGLglGetOcclusionQueryuivNV )
        ++QGLNumFunctions_GL_NV_occlusion_query;

    if ( QGLNumFunctions_GL_NV_occlusion_query == 7 )
        QGLExists_GL_NV_occlusion_query = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_occlusion_query = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_occlusion_query ()
{
    QGLNumFunctions_GL_NV_occlusion_query = 0;
    QGLExists_GL_NV_occlusion_query = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_point_sprite;
int QGLNumFunctions_GL_NV_point_sprite;
#ifdef QGL_DEFINED_GL_NV_point_sprite
pfn_glPointParameteriNV QGLglPointParameteriNV = 0;
pfn_glPointParameterivNV QGLglPointParameterivNV = 0;

static void Init_GL_NV_point_sprite ()
{
    QGLNumFunctions_GL_NV_point_sprite = 0;

    QGLglPointParameteriNV = (pfn_glPointParameteriNV)GetFunctionPointer("glPointParameteriNV");
    if ( QGLglPointParameteriNV )
        ++QGLNumFunctions_GL_NV_point_sprite;

    QGLglPointParameterivNV = (pfn_glPointParameterivNV)GetFunctionPointer("glPointParameterivNV");
    if ( QGLglPointParameterivNV )
        ++QGLNumFunctions_GL_NV_point_sprite;

    if ( QGLNumFunctions_GL_NV_point_sprite == 2 )
        QGLExists_GL_NV_point_sprite = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_point_sprite = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_point_sprite ()
{
    QGLNumFunctions_GL_NV_point_sprite = 0;
    QGLExists_GL_NV_point_sprite = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_shader3;
int QGLNumFunctions_GL_NV_texture_shader3;
#ifdef QGL_DEFINED_GL_NV_texture_shader3
static void Init_GL_NV_texture_shader3 ()
{
    QGLNumFunctions_GL_NV_texture_shader3 = 0;
    QGLExists_GL_NV_texture_shader3 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_shader3 ()
{
    QGLNumFunctions_GL_NV_texture_shader3 = 0;
    QGLExists_GL_NV_texture_shader3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_program1_1;
int QGLNumFunctions_GL_NV_vertex_program1_1;
#ifdef QGL_DEFINED_GL_NV_vertex_program1_1
static void Init_GL_NV_vertex_program1_1 ()
{
    QGLNumFunctions_GL_NV_vertex_program1_1 = 0;
    QGLExists_GL_NV_vertex_program1_1 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_program1_1 ()
{
    QGLNumFunctions_GL_NV_vertex_program1_1 = 0;
    QGLExists_GL_NV_vertex_program1_1 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_shadow_funcs;
int QGLNumFunctions_GL_EXT_shadow_funcs;
#ifdef QGL_DEFINED_GL_EXT_shadow_funcs
static void Init_GL_EXT_shadow_funcs ()
{
    QGLNumFunctions_GL_EXT_shadow_funcs = 0;
    QGLExists_GL_EXT_shadow_funcs = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_shadow_funcs ()
{
    QGLNumFunctions_GL_EXT_shadow_funcs = 0;
    QGLExists_GL_EXT_shadow_funcs = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_stencil_two_side;
int QGLNumFunctions_GL_EXT_stencil_two_side;
#ifdef QGL_DEFINED_GL_EXT_stencil_two_side
pfn_glActiveStencilFaceEXT QGLglActiveStencilFaceEXT = 0;

static void Init_GL_EXT_stencil_two_side ()
{
    QGLNumFunctions_GL_EXT_stencil_two_side = 0;

    QGLglActiveStencilFaceEXT = (pfn_glActiveStencilFaceEXT)GetFunctionPointer("glActiveStencilFaceEXT");
    if ( QGLglActiveStencilFaceEXT )
        ++QGLNumFunctions_GL_EXT_stencil_two_side;

    if ( QGLNumFunctions_GL_EXT_stencil_two_side == 1 )
        QGLExists_GL_EXT_stencil_two_side = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_stencil_two_side = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_stencil_two_side ()
{
    QGLNumFunctions_GL_EXT_stencil_two_side = 0;
    QGLExists_GL_EXT_stencil_two_side = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_text_fragment_shader;
int QGLNumFunctions_GL_ATI_text_fragment_shader;
#ifdef QGL_DEFINED_GL_ATI_text_fragment_shader
static void Init_GL_ATI_text_fragment_shader ()
{
    QGLNumFunctions_GL_ATI_text_fragment_shader = 0;
    QGLExists_GL_ATI_text_fragment_shader = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ATI_text_fragment_shader ()
{
    QGLNumFunctions_GL_ATI_text_fragment_shader = 0;
    QGLExists_GL_ATI_text_fragment_shader = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_client_storage;
int QGLNumFunctions_GL_APPLE_client_storage;
#ifdef QGL_DEFINED_GL_APPLE_client_storage
static void Init_GL_APPLE_client_storage ()
{
    QGLNumFunctions_GL_APPLE_client_storage = 0;
    QGLExists_GL_APPLE_client_storage = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_APPLE_client_storage ()
{
    QGLNumFunctions_GL_APPLE_client_storage = 0;
    QGLExists_GL_APPLE_client_storage = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_element_array;
int QGLNumFunctions_GL_APPLE_element_array;
#ifdef QGL_DEFINED_GL_APPLE_element_array
pfn_glElementPointerAPPLE QGLglElementPointerAPPLE = 0;
pfn_glDrawElementArrayAPPLE QGLglDrawElementArrayAPPLE = 0;
pfn_glDrawRangeElementArrayAPPLE QGLglDrawRangeElementArrayAPPLE = 0;
pfn_glMultiDrawElementArrayAPPLE QGLglMultiDrawElementArrayAPPLE = 0;
pfn_glMultiDrawRangeElementArrayAPPLE QGLglMultiDrawRangeElementArrayAPPLE = 0;

static void Init_GL_APPLE_element_array ()
{
    QGLNumFunctions_GL_APPLE_element_array = 0;

    QGLglElementPointerAPPLE = (pfn_glElementPointerAPPLE)GetFunctionPointer("glElementPointerAPPLE");
    if ( QGLglElementPointerAPPLE )
        ++QGLNumFunctions_GL_APPLE_element_array;

    QGLglDrawElementArrayAPPLE = (pfn_glDrawElementArrayAPPLE)GetFunctionPointer("glDrawElementArrayAPPLE");
    if ( QGLglDrawElementArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_element_array;

    QGLglDrawRangeElementArrayAPPLE = (pfn_glDrawRangeElementArrayAPPLE)GetFunctionPointer("glDrawRangeElementArrayAPPLE");
    if ( QGLglDrawRangeElementArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_element_array;

    QGLglMultiDrawElementArrayAPPLE = (pfn_glMultiDrawElementArrayAPPLE)GetFunctionPointer("glMultiDrawElementArrayAPPLE");
    if ( QGLglMultiDrawElementArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_element_array;

    QGLglMultiDrawRangeElementArrayAPPLE = (pfn_glMultiDrawRangeElementArrayAPPLE)GetFunctionPointer("glMultiDrawRangeElementArrayAPPLE");
    if ( QGLglMultiDrawRangeElementArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_element_array;

    if ( QGLNumFunctions_GL_APPLE_element_array == 5 )
        QGLExists_GL_APPLE_element_array = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_APPLE_element_array = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_APPLE_element_array ()
{
    QGLNumFunctions_GL_APPLE_element_array = 0;
    QGLExists_GL_APPLE_element_array = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_fence;
int QGLNumFunctions_GL_APPLE_fence;
#ifdef QGL_DEFINED_GL_APPLE_fence
pfn_glGenFencesAPPLE QGLglGenFencesAPPLE = 0;
pfn_glDeleteFencesAPPLE QGLglDeleteFencesAPPLE = 0;
pfn_glSetFenceAPPLE QGLglSetFenceAPPLE = 0;
pfn_glIsFenceAPPLE QGLglIsFenceAPPLE = 0;
pfn_glTestFenceAPPLE QGLglTestFenceAPPLE = 0;
pfn_glFinishFenceAPPLE QGLglFinishFenceAPPLE = 0;
pfn_glTestObjectAPPLE QGLglTestObjectAPPLE = 0;
pfn_glFinishObjectAPPLE QGLglFinishObjectAPPLE = 0;

static void Init_GL_APPLE_fence ()
{
    QGLNumFunctions_GL_APPLE_fence = 0;

    QGLglGenFencesAPPLE = (pfn_glGenFencesAPPLE)GetFunctionPointer("glGenFencesAPPLE");
    if ( QGLglGenFencesAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglDeleteFencesAPPLE = (pfn_glDeleteFencesAPPLE)GetFunctionPointer("glDeleteFencesAPPLE");
    if ( QGLglDeleteFencesAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglSetFenceAPPLE = (pfn_glSetFenceAPPLE)GetFunctionPointer("glSetFenceAPPLE");
    if ( QGLglSetFenceAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglIsFenceAPPLE = (pfn_glIsFenceAPPLE)GetFunctionPointer("glIsFenceAPPLE");
    if ( QGLglIsFenceAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglTestFenceAPPLE = (pfn_glTestFenceAPPLE)GetFunctionPointer("glTestFenceAPPLE");
    if ( QGLglTestFenceAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglFinishFenceAPPLE = (pfn_glFinishFenceAPPLE)GetFunctionPointer("glFinishFenceAPPLE");
    if ( QGLglFinishFenceAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglTestObjectAPPLE = (pfn_glTestObjectAPPLE)GetFunctionPointer("glTestObjectAPPLE");
    if ( QGLglTestObjectAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    QGLglFinishObjectAPPLE = (pfn_glFinishObjectAPPLE)GetFunctionPointer("glFinishObjectAPPLE");
    if ( QGLglFinishObjectAPPLE )
        ++QGLNumFunctions_GL_APPLE_fence;

    if ( QGLNumFunctions_GL_APPLE_fence == 8 )
        QGLExists_GL_APPLE_fence = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_APPLE_fence = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_APPLE_fence ()
{
    QGLNumFunctions_GL_APPLE_fence = 0;
    QGLExists_GL_APPLE_fence = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_vertex_array_object;
int QGLNumFunctions_GL_APPLE_vertex_array_object;
#ifdef QGL_DEFINED_GL_APPLE_vertex_array_object
pfn_glBindVertexArrayAPPLE QGLglBindVertexArrayAPPLE = 0;
pfn_glDeleteVertexArraysAPPLE QGLglDeleteVertexArraysAPPLE = 0;
pfn_glGenVertexArraysAPPLE QGLglGenVertexArraysAPPLE = 0;
pfn_glIsVertexArrayAPPLE QGLglIsVertexArrayAPPLE = 0;

static void Init_GL_APPLE_vertex_array_object ()
{
    QGLNumFunctions_GL_APPLE_vertex_array_object = 0;

    QGLglBindVertexArrayAPPLE = (pfn_glBindVertexArrayAPPLE)GetFunctionPointer("glBindVertexArrayAPPLE");
    if ( QGLglBindVertexArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_object;

    QGLglDeleteVertexArraysAPPLE = (pfn_glDeleteVertexArraysAPPLE)GetFunctionPointer("glDeleteVertexArraysAPPLE");
    if ( QGLglDeleteVertexArraysAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_object;

    QGLglGenVertexArraysAPPLE = (pfn_glGenVertexArraysAPPLE)GetFunctionPointer("glGenVertexArraysAPPLE");
    if ( QGLglGenVertexArraysAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_object;

    QGLglIsVertexArrayAPPLE = (pfn_glIsVertexArrayAPPLE)GetFunctionPointer("glIsVertexArrayAPPLE");
    if ( QGLglIsVertexArrayAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_object;

    if ( QGLNumFunctions_GL_APPLE_vertex_array_object == 4 )
        QGLExists_GL_APPLE_vertex_array_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_APPLE_vertex_array_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_APPLE_vertex_array_object ()
{
    QGLNumFunctions_GL_APPLE_vertex_array_object = 0;
    QGLExists_GL_APPLE_vertex_array_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_vertex_array_range;
int QGLNumFunctions_GL_APPLE_vertex_array_range;
#ifdef QGL_DEFINED_GL_APPLE_vertex_array_range
pfn_glVertexArrayRangeAPPLE QGLglVertexArrayRangeAPPLE = 0;
pfn_glFlushVertexArrayRangeAPPLE QGLglFlushVertexArrayRangeAPPLE = 0;
pfn_glVertexArrayParameteriAPPLE QGLglVertexArrayParameteriAPPLE = 0;

static void Init_GL_APPLE_vertex_array_range ()
{
    QGLNumFunctions_GL_APPLE_vertex_array_range = 0;

    QGLglVertexArrayRangeAPPLE = (pfn_glVertexArrayRangeAPPLE)GetFunctionPointer("glVertexArrayRangeAPPLE");
    if ( QGLglVertexArrayRangeAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_range;

    QGLglFlushVertexArrayRangeAPPLE = (pfn_glFlushVertexArrayRangeAPPLE)GetFunctionPointer("glFlushVertexArrayRangeAPPLE");
    if ( QGLglFlushVertexArrayRangeAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_range;

    QGLglVertexArrayParameteriAPPLE = (pfn_glVertexArrayParameteriAPPLE)GetFunctionPointer("glVertexArrayParameteriAPPLE");
    if ( QGLglVertexArrayParameteriAPPLE )
        ++QGLNumFunctions_GL_APPLE_vertex_array_range;

    if ( QGLNumFunctions_GL_APPLE_vertex_array_range == 3 )
        QGLExists_GL_APPLE_vertex_array_range = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_APPLE_vertex_array_range = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_APPLE_vertex_array_range ()
{
    QGLNumFunctions_GL_APPLE_vertex_array_range = 0;
    QGLExists_GL_APPLE_vertex_array_range = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_APPLE_ycbcr_422;
int QGLNumFunctions_GL_APPLE_ycbcr_422;
#ifdef QGL_DEFINED_GL_APPLE_ycbcr_422
static void Init_GL_APPLE_ycbcr_422 ()
{
    QGLNumFunctions_GL_APPLE_ycbcr_422 = 0;
    QGLExists_GL_APPLE_ycbcr_422 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_APPLE_ycbcr_422 ()
{
    QGLNumFunctions_GL_APPLE_ycbcr_422 = 0;
    QGLExists_GL_APPLE_ycbcr_422 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_S3_s3tc;
int QGLNumFunctions_GL_S3_s3tc;
#ifdef QGL_DEFINED_GL_S3_s3tc
static void Init_GL_S3_s3tc ()
{
    QGLNumFunctions_GL_S3_s3tc = 0;
    QGLExists_GL_S3_s3tc = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_S3_s3tc ()
{
    QGLNumFunctions_GL_S3_s3tc = 0;
    QGLExists_GL_S3_s3tc = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_draw_buffers;
int QGLNumFunctions_GL_ATI_draw_buffers;
#ifdef QGL_DEFINED_GL_ATI_draw_buffers
pfn_glDrawBuffersATI QGLglDrawBuffersATI = 0;

static void Init_GL_ATI_draw_buffers ()
{
    QGLNumFunctions_GL_ATI_draw_buffers = 0;

    QGLglDrawBuffersATI = (pfn_glDrawBuffersATI)GetFunctionPointer("glDrawBuffersATI");
    if ( QGLglDrawBuffersATI )
        ++QGLNumFunctions_GL_ATI_draw_buffers;

    if ( QGLNumFunctions_GL_ATI_draw_buffers == 1 )
        QGLExists_GL_ATI_draw_buffers = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_draw_buffers = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_draw_buffers ()
{
    QGLNumFunctions_GL_ATI_draw_buffers = 0;
    QGLExists_GL_ATI_draw_buffers = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_pixel_format_float;
int QGLNumFunctions_GL_ATI_pixel_format_float;
#ifdef QGL_DEFINED_GL_ATI_pixel_format_float
static void Init_GL_ATI_pixel_format_float ()
{
    QGLNumFunctions_GL_ATI_pixel_format_float = 0;
    QGLExists_GL_ATI_pixel_format_float = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ATI_pixel_format_float ()
{
    QGLNumFunctions_GL_ATI_pixel_format_float = 0;
    QGLExists_GL_ATI_pixel_format_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_texture_env_combine3;
int QGLNumFunctions_GL_ATI_texture_env_combine3;
#ifdef QGL_DEFINED_GL_ATI_texture_env_combine3
static void Init_GL_ATI_texture_env_combine3 ()
{
    QGLNumFunctions_GL_ATI_texture_env_combine3 = 0;
    QGLExists_GL_ATI_texture_env_combine3 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ATI_texture_env_combine3 ()
{
    QGLNumFunctions_GL_ATI_texture_env_combine3 = 0;
    QGLExists_GL_ATI_texture_env_combine3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_texture_float;
int QGLNumFunctions_GL_ATI_texture_float;
#ifdef QGL_DEFINED_GL_ATI_texture_float
static void Init_GL_ATI_texture_float ()
{
    QGLNumFunctions_GL_ATI_texture_float = 0;
    QGLExists_GL_ATI_texture_float = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_ATI_texture_float ()
{
    QGLNumFunctions_GL_ATI_texture_float = 0;
    QGLExists_GL_ATI_texture_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_float_buffer;
int QGLNumFunctions_GL_NV_float_buffer;
#ifdef QGL_DEFINED_GL_NV_float_buffer
static void Init_GL_NV_float_buffer ()
{
    QGLNumFunctions_GL_NV_float_buffer = 0;
    QGLExists_GL_NV_float_buffer = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_float_buffer ()
{
    QGLNumFunctions_GL_NV_float_buffer = 0;
    QGLExists_GL_NV_float_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_fragment_program;
int QGLNumFunctions_GL_NV_fragment_program;
#ifdef QGL_DEFINED_GL_NV_fragment_program
pfn_glProgramNamedParameter4fNV QGLglProgramNamedParameter4fNV = 0;
pfn_glProgramNamedParameter4dNV QGLglProgramNamedParameter4dNV = 0;
pfn_glProgramNamedParameter4fvNV QGLglProgramNamedParameter4fvNV = 0;
pfn_glProgramNamedParameter4dvNV QGLglProgramNamedParameter4dvNV = 0;
pfn_glGetProgramNamedParameterfvNV QGLglGetProgramNamedParameterfvNV = 0;
pfn_glGetProgramNamedParameterdvNV QGLglGetProgramNamedParameterdvNV = 0;

static void Init_GL_NV_fragment_program ()
{
    QGLNumFunctions_GL_NV_fragment_program = 0;

    QGLglProgramNamedParameter4fNV = (pfn_glProgramNamedParameter4fNV)GetFunctionPointer("glProgramNamedParameter4fNV");
    if ( QGLglProgramNamedParameter4fNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    QGLglProgramNamedParameter4dNV = (pfn_glProgramNamedParameter4dNV)GetFunctionPointer("glProgramNamedParameter4dNV");
    if ( QGLglProgramNamedParameter4dNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    QGLglProgramNamedParameter4fvNV = (pfn_glProgramNamedParameter4fvNV)GetFunctionPointer("glProgramNamedParameter4fvNV");
    if ( QGLglProgramNamedParameter4fvNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    QGLglProgramNamedParameter4dvNV = (pfn_glProgramNamedParameter4dvNV)GetFunctionPointer("glProgramNamedParameter4dvNV");
    if ( QGLglProgramNamedParameter4dvNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    QGLglGetProgramNamedParameterfvNV = (pfn_glGetProgramNamedParameterfvNV)GetFunctionPointer("glGetProgramNamedParameterfvNV");
    if ( QGLglGetProgramNamedParameterfvNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    QGLglGetProgramNamedParameterdvNV = (pfn_glGetProgramNamedParameterdvNV)GetFunctionPointer("glGetProgramNamedParameterdvNV");
    if ( QGLglGetProgramNamedParameterdvNV )
        ++QGLNumFunctions_GL_NV_fragment_program;

    if ( QGLNumFunctions_GL_NV_fragment_program == 6 )
        QGLExists_GL_NV_fragment_program = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_fragment_program = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_fragment_program ()
{
    QGLNumFunctions_GL_NV_fragment_program = 0;
    QGLExists_GL_NV_fragment_program = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_half_float;
int QGLNumFunctions_GL_NV_half_float;
#ifdef QGL_DEFINED_GL_NV_half_float
pfn_glVertex2hNV QGLglVertex2hNV = 0;
pfn_glVertex2hvNV QGLglVertex2hvNV = 0;
pfn_glVertex3hNV QGLglVertex3hNV = 0;
pfn_glVertex3hvNV QGLglVertex3hvNV = 0;
pfn_glVertex4hNV QGLglVertex4hNV = 0;
pfn_glVertex4hvNV QGLglVertex4hvNV = 0;
pfn_glNormal3hNV QGLglNormal3hNV = 0;
pfn_glNormal3hvNV QGLglNormal3hvNV = 0;
pfn_glColor3hNV QGLglColor3hNV = 0;
pfn_glColor3hvNV QGLglColor3hvNV = 0;
pfn_glColor4hNV QGLglColor4hNV = 0;
pfn_glColor4hvNV QGLglColor4hvNV = 0;
pfn_glTexCoord1hNV QGLglTexCoord1hNV = 0;
pfn_glTexCoord1hvNV QGLglTexCoord1hvNV = 0;
pfn_glTexCoord2hNV QGLglTexCoord2hNV = 0;
pfn_glTexCoord2hvNV QGLglTexCoord2hvNV = 0;
pfn_glTexCoord3hNV QGLglTexCoord3hNV = 0;
pfn_glTexCoord3hvNV QGLglTexCoord3hvNV = 0;
pfn_glTexCoord4hNV QGLglTexCoord4hNV = 0;
pfn_glTexCoord4hvNV QGLglTexCoord4hvNV = 0;
pfn_glMultiTexCoord1hNV QGLglMultiTexCoord1hNV = 0;
pfn_glMultiTexCoord1hvNV QGLglMultiTexCoord1hvNV = 0;
pfn_glMultiTexCoord2hNV QGLglMultiTexCoord2hNV = 0;
pfn_glMultiTexCoord2hvNV QGLglMultiTexCoord2hvNV = 0;
pfn_glMultiTexCoord3hNV QGLglMultiTexCoord3hNV = 0;
pfn_glMultiTexCoord3hvNV QGLglMultiTexCoord3hvNV = 0;
pfn_glMultiTexCoord4hNV QGLglMultiTexCoord4hNV = 0;
pfn_glMultiTexCoord4hvNV QGLglMultiTexCoord4hvNV = 0;
pfn_glFogCoordhNV QGLglFogCoordhNV = 0;
pfn_glFogCoordhvNV QGLglFogCoordhvNV = 0;
pfn_glSecondaryColor3hNV QGLglSecondaryColor3hNV = 0;
pfn_glSecondaryColor3hvNV QGLglSecondaryColor3hvNV = 0;
pfn_glVertexWeighthNV QGLglVertexWeighthNV = 0;
pfn_glVertexWeighthvNV QGLglVertexWeighthvNV = 0;
pfn_glVertexAttrib1hNV QGLglVertexAttrib1hNV = 0;
pfn_glVertexAttrib1hvNV QGLglVertexAttrib1hvNV = 0;
pfn_glVertexAttrib2hNV QGLglVertexAttrib2hNV = 0;
pfn_glVertexAttrib2hvNV QGLglVertexAttrib2hvNV = 0;
pfn_glVertexAttrib3hNV QGLglVertexAttrib3hNV = 0;
pfn_glVertexAttrib3hvNV QGLglVertexAttrib3hvNV = 0;
pfn_glVertexAttrib4hNV QGLglVertexAttrib4hNV = 0;
pfn_glVertexAttrib4hvNV QGLglVertexAttrib4hvNV = 0;
pfn_glVertexAttribs1hvNV QGLglVertexAttribs1hvNV = 0;
pfn_glVertexAttribs2hvNV QGLglVertexAttribs2hvNV = 0;
pfn_glVertexAttribs3hvNV QGLglVertexAttribs3hvNV = 0;
pfn_glVertexAttribs4hvNV QGLglVertexAttribs4hvNV = 0;

static void Init_GL_NV_half_float ()
{
    QGLNumFunctions_GL_NV_half_float = 0;

    QGLglVertex2hNV = (pfn_glVertex2hNV)GetFunctionPointer("glVertex2hNV");
    if ( QGLglVertex2hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertex2hvNV = (pfn_glVertex2hvNV)GetFunctionPointer("glVertex2hvNV");
    if ( QGLglVertex2hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertex3hNV = (pfn_glVertex3hNV)GetFunctionPointer("glVertex3hNV");
    if ( QGLglVertex3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertex3hvNV = (pfn_glVertex3hvNV)GetFunctionPointer("glVertex3hvNV");
    if ( QGLglVertex3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertex4hNV = (pfn_glVertex4hNV)GetFunctionPointer("glVertex4hNV");
    if ( QGLglVertex4hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertex4hvNV = (pfn_glVertex4hvNV)GetFunctionPointer("glVertex4hvNV");
    if ( QGLglVertex4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglNormal3hNV = (pfn_glNormal3hNV)GetFunctionPointer("glNormal3hNV");
    if ( QGLglNormal3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglNormal3hvNV = (pfn_glNormal3hvNV)GetFunctionPointer("glNormal3hvNV");
    if ( QGLglNormal3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglColor3hNV = (pfn_glColor3hNV)GetFunctionPointer("glColor3hNV");
    if ( QGLglColor3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglColor3hvNV = (pfn_glColor3hvNV)GetFunctionPointer("glColor3hvNV");
    if ( QGLglColor3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglColor4hNV = (pfn_glColor4hNV)GetFunctionPointer("glColor4hNV");
    if ( QGLglColor4hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglColor4hvNV = (pfn_glColor4hvNV)GetFunctionPointer("glColor4hvNV");
    if ( QGLglColor4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord1hNV = (pfn_glTexCoord1hNV)GetFunctionPointer("glTexCoord1hNV");
    if ( QGLglTexCoord1hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord1hvNV = (pfn_glTexCoord1hvNV)GetFunctionPointer("glTexCoord1hvNV");
    if ( QGLglTexCoord1hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord2hNV = (pfn_glTexCoord2hNV)GetFunctionPointer("glTexCoord2hNV");
    if ( QGLglTexCoord2hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord2hvNV = (pfn_glTexCoord2hvNV)GetFunctionPointer("glTexCoord2hvNV");
    if ( QGLglTexCoord2hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord3hNV = (pfn_glTexCoord3hNV)GetFunctionPointer("glTexCoord3hNV");
    if ( QGLglTexCoord3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord3hvNV = (pfn_glTexCoord3hvNV)GetFunctionPointer("glTexCoord3hvNV");
    if ( QGLglTexCoord3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord4hNV = (pfn_glTexCoord4hNV)GetFunctionPointer("glTexCoord4hNV");
    if ( QGLglTexCoord4hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglTexCoord4hvNV = (pfn_glTexCoord4hvNV)GetFunctionPointer("glTexCoord4hvNV");
    if ( QGLglTexCoord4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord1hNV = (pfn_glMultiTexCoord1hNV)GetFunctionPointer("glMultiTexCoord1hNV");
    if ( QGLglMultiTexCoord1hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord1hvNV = (pfn_glMultiTexCoord1hvNV)GetFunctionPointer("glMultiTexCoord1hvNV");
    if ( QGLglMultiTexCoord1hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord2hNV = (pfn_glMultiTexCoord2hNV)GetFunctionPointer("glMultiTexCoord2hNV");
    if ( QGLglMultiTexCoord2hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord2hvNV = (pfn_glMultiTexCoord2hvNV)GetFunctionPointer("glMultiTexCoord2hvNV");
    if ( QGLglMultiTexCoord2hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord3hNV = (pfn_glMultiTexCoord3hNV)GetFunctionPointer("glMultiTexCoord3hNV");
    if ( QGLglMultiTexCoord3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord3hvNV = (pfn_glMultiTexCoord3hvNV)GetFunctionPointer("glMultiTexCoord3hvNV");
    if ( QGLglMultiTexCoord3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord4hNV = (pfn_glMultiTexCoord4hNV)GetFunctionPointer("glMultiTexCoord4hNV");
    if ( QGLglMultiTexCoord4hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglMultiTexCoord4hvNV = (pfn_glMultiTexCoord4hvNV)GetFunctionPointer("glMultiTexCoord4hvNV");
    if ( QGLglMultiTexCoord4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglFogCoordhNV = (pfn_glFogCoordhNV)GetFunctionPointer("glFogCoordhNV");
    if ( QGLglFogCoordhNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglFogCoordhvNV = (pfn_glFogCoordhvNV)GetFunctionPointer("glFogCoordhvNV");
    if ( QGLglFogCoordhvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglSecondaryColor3hNV = (pfn_glSecondaryColor3hNV)GetFunctionPointer("glSecondaryColor3hNV");
    if ( QGLglSecondaryColor3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglSecondaryColor3hvNV = (pfn_glSecondaryColor3hvNV)GetFunctionPointer("glSecondaryColor3hvNV");
    if ( QGLglSecondaryColor3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexWeighthNV = (pfn_glVertexWeighthNV)GetFunctionPointer("glVertexWeighthNV");
    if ( QGLglVertexWeighthNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexWeighthvNV = (pfn_glVertexWeighthvNV)GetFunctionPointer("glVertexWeighthvNV");
    if ( QGLglVertexWeighthvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib1hNV = (pfn_glVertexAttrib1hNV)GetFunctionPointer("glVertexAttrib1hNV");
    if ( QGLglVertexAttrib1hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib1hvNV = (pfn_glVertexAttrib1hvNV)GetFunctionPointer("glVertexAttrib1hvNV");
    if ( QGLglVertexAttrib1hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib2hNV = (pfn_glVertexAttrib2hNV)GetFunctionPointer("glVertexAttrib2hNV");
    if ( QGLglVertexAttrib2hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib2hvNV = (pfn_glVertexAttrib2hvNV)GetFunctionPointer("glVertexAttrib2hvNV");
    if ( QGLglVertexAttrib2hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib3hNV = (pfn_glVertexAttrib3hNV)GetFunctionPointer("glVertexAttrib3hNV");
    if ( QGLglVertexAttrib3hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib3hvNV = (pfn_glVertexAttrib3hvNV)GetFunctionPointer("glVertexAttrib3hvNV");
    if ( QGLglVertexAttrib3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib4hNV = (pfn_glVertexAttrib4hNV)GetFunctionPointer("glVertexAttrib4hNV");
    if ( QGLglVertexAttrib4hNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttrib4hvNV = (pfn_glVertexAttrib4hvNV)GetFunctionPointer("glVertexAttrib4hvNV");
    if ( QGLglVertexAttrib4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttribs1hvNV = (pfn_glVertexAttribs1hvNV)GetFunctionPointer("glVertexAttribs1hvNV");
    if ( QGLglVertexAttribs1hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttribs2hvNV = (pfn_glVertexAttribs2hvNV)GetFunctionPointer("glVertexAttribs2hvNV");
    if ( QGLglVertexAttribs2hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttribs3hvNV = (pfn_glVertexAttribs3hvNV)GetFunctionPointer("glVertexAttribs3hvNV");
    if ( QGLglVertexAttribs3hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    QGLglVertexAttribs4hvNV = (pfn_glVertexAttribs4hvNV)GetFunctionPointer("glVertexAttribs4hvNV");
    if ( QGLglVertexAttribs4hvNV )
        ++QGLNumFunctions_GL_NV_half_float;

    if ( QGLNumFunctions_GL_NV_half_float == 46 )
        QGLExists_GL_NV_half_float = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_half_float = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_half_float ()
{
    QGLNumFunctions_GL_NV_half_float = 0;
    QGLExists_GL_NV_half_float = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_pixel_data_range;
int QGLNumFunctions_GL_NV_pixel_data_range;
#ifdef QGL_DEFINED_GL_NV_pixel_data_range
pfn_glPixelDataRangeNV QGLglPixelDataRangeNV = 0;
pfn_glFlushPixelDataRangeNV QGLglFlushPixelDataRangeNV = 0;

static void Init_GL_NV_pixel_data_range ()
{
    QGLNumFunctions_GL_NV_pixel_data_range = 0;

    QGLglPixelDataRangeNV = (pfn_glPixelDataRangeNV)GetFunctionPointer("glPixelDataRangeNV");
    if ( QGLglPixelDataRangeNV )
        ++QGLNumFunctions_GL_NV_pixel_data_range;

    QGLglFlushPixelDataRangeNV = (pfn_glFlushPixelDataRangeNV)GetFunctionPointer("glFlushPixelDataRangeNV");
    if ( QGLglFlushPixelDataRangeNV )
        ++QGLNumFunctions_GL_NV_pixel_data_range;

    if ( QGLNumFunctions_GL_NV_pixel_data_range == 2 )
        QGLExists_GL_NV_pixel_data_range = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_pixel_data_range = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_pixel_data_range ()
{
    QGLNumFunctions_GL_NV_pixel_data_range = 0;
    QGLExists_GL_NV_pixel_data_range = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_primitive_restart;
int QGLNumFunctions_GL_NV_primitive_restart;
#ifdef QGL_DEFINED_GL_NV_primitive_restart
pfn_glPrimitiveRestartNV QGLglPrimitiveRestartNV = 0;
pfn_glPrimitiveRestartIndexNV QGLglPrimitiveRestartIndexNV = 0;

static void Init_GL_NV_primitive_restart ()
{
    QGLNumFunctions_GL_NV_primitive_restart = 0;

    QGLglPrimitiveRestartNV = (pfn_glPrimitiveRestartNV)GetFunctionPointer("glPrimitiveRestartNV");
    if ( QGLglPrimitiveRestartNV )
        ++QGLNumFunctions_GL_NV_primitive_restart;

    QGLglPrimitiveRestartIndexNV = (pfn_glPrimitiveRestartIndexNV)GetFunctionPointer("glPrimitiveRestartIndexNV");
    if ( QGLglPrimitiveRestartIndexNV )
        ++QGLNumFunctions_GL_NV_primitive_restart;

    if ( QGLNumFunctions_GL_NV_primitive_restart == 2 )
        QGLExists_GL_NV_primitive_restart = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_NV_primitive_restart = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_NV_primitive_restart ()
{
    QGLNumFunctions_GL_NV_primitive_restart = 0;
    QGLExists_GL_NV_primitive_restart = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_texture_expand_normal;
int QGLNumFunctions_GL_NV_texture_expand_normal;
#ifdef QGL_DEFINED_GL_NV_texture_expand_normal
static void Init_GL_NV_texture_expand_normal ()
{
    QGLNumFunctions_GL_NV_texture_expand_normal = 0;
    QGLExists_GL_NV_texture_expand_normal = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_texture_expand_normal ()
{
    QGLNumFunctions_GL_NV_texture_expand_normal = 0;
    QGLExists_GL_NV_texture_expand_normal = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_program2;
int QGLNumFunctions_GL_NV_vertex_program2;
#ifdef QGL_DEFINED_GL_NV_vertex_program2
static void Init_GL_NV_vertex_program2 ()
{
    QGLNumFunctions_GL_NV_vertex_program2 = 0;
    QGLExists_GL_NV_vertex_program2 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_program2 ()
{
    QGLNumFunctions_GL_NV_vertex_program2 = 0;
    QGLExists_GL_NV_vertex_program2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_map_object_buffer;
int QGLNumFunctions_GL_ATI_map_object_buffer;
#ifdef QGL_DEFINED_GL_ATI_map_object_buffer
pfn_glMapObjectBufferATI QGLglMapObjectBufferATI = 0;
pfn_glUnmapObjectBufferATI QGLglUnmapObjectBufferATI = 0;

static void Init_GL_ATI_map_object_buffer ()
{
    QGLNumFunctions_GL_ATI_map_object_buffer = 0;

    QGLglMapObjectBufferATI = (pfn_glMapObjectBufferATI)GetFunctionPointer("glMapObjectBufferATI");
    if ( QGLglMapObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_map_object_buffer;

    QGLglUnmapObjectBufferATI = (pfn_glUnmapObjectBufferATI)GetFunctionPointer("glUnmapObjectBufferATI");
    if ( QGLglUnmapObjectBufferATI )
        ++QGLNumFunctions_GL_ATI_map_object_buffer;

    if ( QGLNumFunctions_GL_ATI_map_object_buffer == 2 )
        QGLExists_GL_ATI_map_object_buffer = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_map_object_buffer = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_map_object_buffer ()
{
    QGLNumFunctions_GL_ATI_map_object_buffer = 0;
    QGLExists_GL_ATI_map_object_buffer = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_separate_stencil;
int QGLNumFunctions_GL_ATI_separate_stencil;
#ifdef QGL_DEFINED_GL_ATI_separate_stencil
pfn_glStencilOpSeparateATI QGLglStencilOpSeparateATI = 0;
pfn_glStencilFuncSeparateATI QGLglStencilFuncSeparateATI = 0;

static void Init_GL_ATI_separate_stencil ()
{
    QGLNumFunctions_GL_ATI_separate_stencil = 0;

    QGLglStencilOpSeparateATI = (pfn_glStencilOpSeparateATI)GetFunctionPointer("glStencilOpSeparateATI");
    if ( QGLglStencilOpSeparateATI )
        ++QGLNumFunctions_GL_ATI_separate_stencil;

    QGLglStencilFuncSeparateATI = (pfn_glStencilFuncSeparateATI)GetFunctionPointer("glStencilFuncSeparateATI");
    if ( QGLglStencilFuncSeparateATI )
        ++QGLNumFunctions_GL_ATI_separate_stencil;

    if ( QGLNumFunctions_GL_ATI_separate_stencil == 2 )
        QGLExists_GL_ATI_separate_stencil = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_separate_stencil = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_separate_stencil ()
{
    QGLNumFunctions_GL_ATI_separate_stencil = 0;
    QGLExists_GL_ATI_separate_stencil = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_ATI_vertex_attrib_array_object;
int QGLNumFunctions_GL_ATI_vertex_attrib_array_object;
#ifdef QGL_DEFINED_GL_ATI_vertex_attrib_array_object
pfn_glVertexAttribArrayObjectATI QGLglVertexAttribArrayObjectATI = 0;
pfn_glGetVertexAttribArrayObjectfvATI QGLglGetVertexAttribArrayObjectfvATI = 0;
pfn_glGetVertexAttribArrayObjectivATI QGLglGetVertexAttribArrayObjectivATI = 0;

static void Init_GL_ATI_vertex_attrib_array_object ()
{
    QGLNumFunctions_GL_ATI_vertex_attrib_array_object = 0;

    QGLglVertexAttribArrayObjectATI = (pfn_glVertexAttribArrayObjectATI)GetFunctionPointer("glVertexAttribArrayObjectATI");
    if ( QGLglVertexAttribArrayObjectATI )
        ++QGLNumFunctions_GL_ATI_vertex_attrib_array_object;

    QGLglGetVertexAttribArrayObjectfvATI = (pfn_glGetVertexAttribArrayObjectfvATI)GetFunctionPointer("glGetVertexAttribArrayObjectfvATI");
    if ( QGLglGetVertexAttribArrayObjectfvATI )
        ++QGLNumFunctions_GL_ATI_vertex_attrib_array_object;

    QGLglGetVertexAttribArrayObjectivATI = (pfn_glGetVertexAttribArrayObjectivATI)GetFunctionPointer("glGetVertexAttribArrayObjectivATI");
    if ( QGLglGetVertexAttribArrayObjectivATI )
        ++QGLNumFunctions_GL_ATI_vertex_attrib_array_object;

    if ( QGLNumFunctions_GL_ATI_vertex_attrib_array_object == 3 )
        QGLExists_GL_ATI_vertex_attrib_array_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_ATI_vertex_attrib_array_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_ATI_vertex_attrib_array_object ()
{
    QGLNumFunctions_GL_ATI_vertex_attrib_array_object = 0;
    QGLExists_GL_ATI_vertex_attrib_array_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_OES_read_format;
int QGLNumFunctions_GL_OES_read_format;
#ifdef QGL_DEFINED_GL_OES_read_format
static void Init_GL_OES_read_format ()
{
    QGLNumFunctions_GL_OES_read_format = 0;
    QGLExists_GL_OES_read_format = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_OES_read_format ()
{
    QGLNumFunctions_GL_OES_read_format = 0;
    QGLExists_GL_OES_read_format = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_depth_bounds_test;
int QGLNumFunctions_GL_EXT_depth_bounds_test;
#ifdef QGL_DEFINED_GL_EXT_depth_bounds_test
pfn_glDepthBoundsEXT QGLglDepthBoundsEXT = 0;

static void Init_GL_EXT_depth_bounds_test ()
{
    QGLNumFunctions_GL_EXT_depth_bounds_test = 0;

    QGLglDepthBoundsEXT = (pfn_glDepthBoundsEXT)GetFunctionPointer("glDepthBoundsEXT");
    if ( QGLglDepthBoundsEXT )
        ++QGLNumFunctions_GL_EXT_depth_bounds_test;

    if ( QGLNumFunctions_GL_EXT_depth_bounds_test == 1 )
        QGLExists_GL_EXT_depth_bounds_test = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_depth_bounds_test = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_depth_bounds_test ()
{
    QGLNumFunctions_GL_EXT_depth_bounds_test = 0;
    QGLExists_GL_EXT_depth_bounds_test = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_texture_mirror_clamp;
int QGLNumFunctions_GL_EXT_texture_mirror_clamp;
#ifdef QGL_DEFINED_GL_EXT_texture_mirror_clamp
static void Init_GL_EXT_texture_mirror_clamp ()
{
    QGLNumFunctions_GL_EXT_texture_mirror_clamp = 0;
    QGLExists_GL_EXT_texture_mirror_clamp = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_texture_mirror_clamp ()
{
    QGLNumFunctions_GL_EXT_texture_mirror_clamp = 0;
    QGLExists_GL_EXT_texture_mirror_clamp = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_blend_equation_separate;
int QGLNumFunctions_GL_EXT_blend_equation_separate;
#ifdef QGL_DEFINED_GL_EXT_blend_equation_separate
pfn_glBlendEquationSeparateEXT QGLglBlendEquationSeparateEXT = 0;

static void Init_GL_EXT_blend_equation_separate ()
{
    QGLNumFunctions_GL_EXT_blend_equation_separate = 0;

    QGLglBlendEquationSeparateEXT = (pfn_glBlendEquationSeparateEXT)GetFunctionPointer("glBlendEquationSeparateEXT");
    if ( QGLglBlendEquationSeparateEXT )
        ++QGLNumFunctions_GL_EXT_blend_equation_separate;

    if ( QGLNumFunctions_GL_EXT_blend_equation_separate == 1 )
        QGLExists_GL_EXT_blend_equation_separate = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_blend_equation_separate = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_blend_equation_separate ()
{
    QGLNumFunctions_GL_EXT_blend_equation_separate = 0;
    QGLExists_GL_EXT_blend_equation_separate = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_MESA_pack_invert;
int QGLNumFunctions_GL_MESA_pack_invert;
#ifdef QGL_DEFINED_GL_MESA_pack_invert
static void Init_GL_MESA_pack_invert ()
{
    QGLNumFunctions_GL_MESA_pack_invert = 0;
    QGLExists_GL_MESA_pack_invert = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_MESA_pack_invert ()
{
    QGLNumFunctions_GL_MESA_pack_invert = 0;
    QGLExists_GL_MESA_pack_invert = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_MESA_ycbcr_texture;
int QGLNumFunctions_GL_MESA_ycbcr_texture;
#ifdef QGL_DEFINED_GL_MESA_ycbcr_texture
static void Init_GL_MESA_ycbcr_texture ()
{
    QGLNumFunctions_GL_MESA_ycbcr_texture = 0;
    QGLExists_GL_MESA_ycbcr_texture = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_MESA_ycbcr_texture ()
{
    QGLNumFunctions_GL_MESA_ycbcr_texture = 0;
    QGLExists_GL_MESA_ycbcr_texture = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_pixel_buffer_object;
int QGLNumFunctions_GL_EXT_pixel_buffer_object;
#ifdef QGL_DEFINED_GL_EXT_pixel_buffer_object
static void Init_GL_EXT_pixel_buffer_object ()
{
    QGLNumFunctions_GL_EXT_pixel_buffer_object = 0;
    QGLExists_GL_EXT_pixel_buffer_object = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_EXT_pixel_buffer_object ()
{
    QGLNumFunctions_GL_EXT_pixel_buffer_object = 0;
    QGLExists_GL_EXT_pixel_buffer_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_fragment_program_option;
int QGLNumFunctions_GL_NV_fragment_program_option;
#ifdef QGL_DEFINED_GL_NV_fragment_program_option
static void Init_GL_NV_fragment_program_option ()
{
    QGLNumFunctions_GL_NV_fragment_program_option = 0;
    QGLExists_GL_NV_fragment_program_option = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_fragment_program_option ()
{
    QGLNumFunctions_GL_NV_fragment_program_option = 0;
    QGLExists_GL_NV_fragment_program_option = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_fragment_program2;
int QGLNumFunctions_GL_NV_fragment_program2;
#ifdef QGL_DEFINED_GL_NV_fragment_program2
static void Init_GL_NV_fragment_program2 ()
{
    QGLNumFunctions_GL_NV_fragment_program2 = 0;
    QGLExists_GL_NV_fragment_program2 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_fragment_program2 ()
{
    QGLNumFunctions_GL_NV_fragment_program2 = 0;
    QGLExists_GL_NV_fragment_program2 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_program2_option;
int QGLNumFunctions_GL_NV_vertex_program2_option;
#ifdef QGL_DEFINED_GL_NV_vertex_program2_option
static void Init_GL_NV_vertex_program2_option ()
{
    QGLNumFunctions_GL_NV_vertex_program2_option = 0;
    QGLExists_GL_NV_vertex_program2_option = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_program2_option ()
{
    QGLNumFunctions_GL_NV_vertex_program2_option = 0;
    QGLExists_GL_NV_vertex_program2_option = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_NV_vertex_program3;
int QGLNumFunctions_GL_NV_vertex_program3;
#ifdef QGL_DEFINED_GL_NV_vertex_program3
static void Init_GL_NV_vertex_program3 ()
{
    QGLNumFunctions_GL_NV_vertex_program3 = 0;
    QGLExists_GL_NV_vertex_program3 = QGL_EXT_FULLY_EXISTS;
}
#else
static void Init_GL_NV_vertex_program3 ()
{
    QGLNumFunctions_GL_NV_vertex_program3 = 0;
    QGLExists_GL_NV_vertex_program3 = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_EXT_framebuffer_object;
int QGLNumFunctions_GL_EXT_framebuffer_object;
#ifdef QGL_DEFINED_GL_EXT_framebuffer_object
pfn_glIsRenderbufferEXT QGLglIsRenderbufferEXT = 0;
pfn_glBindRenderbufferEXT QGLglBindRenderbufferEXT = 0;
pfn_glDeleteRenderbuffersEXT QGLglDeleteRenderbuffersEXT = 0;
pfn_glGenRenderbuffersEXT QGLglGenRenderbuffersEXT = 0;
pfn_glRenderbufferStorageEXT QGLglRenderbufferStorageEXT = 0;
pfn_glGetRenderbufferParameterivEXT QGLglGetRenderbufferParameterivEXT = 0;
pfn_glIsFramebufferEXT QGLglIsFramebufferEXT = 0;
pfn_glBindFramebufferEXT QGLglBindFramebufferEXT = 0;
pfn_glDeleteFramebuffersEXT QGLglDeleteFramebuffersEXT = 0;
pfn_glGenFramebuffersEXT QGLglGenFramebuffersEXT = 0;
pfn_glCheckFramebufferStatusEXT QGLglCheckFramebufferStatusEXT = 0;
pfn_glFramebufferTexture1DEXT QGLglFramebufferTexture1DEXT = 0;
pfn_glFramebufferTexture2DEXT QGLglFramebufferTexture2DEXT = 0;
pfn_glFramebufferTexture3DEXT QGLglFramebufferTexture3DEXT = 0;
pfn_glFramebufferRenderbufferEXT QGLglFramebufferRenderbufferEXT = 0;
pfn_glGetFramebufferAttachmentParameterivEXT QGLglGetFramebufferAttachmentParameterivEXT = 0;
pfn_glGenerateMipmapEXT QGLglGenerateMipmapEXT = 0;

static void Init_GL_EXT_framebuffer_object ()
{
    QGLNumFunctions_GL_EXT_framebuffer_object = 0;

    QGLglIsRenderbufferEXT = (pfn_glIsRenderbufferEXT)GetFunctionPointer("glIsRenderbufferEXT");
    if ( QGLglIsRenderbufferEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglBindRenderbufferEXT = (pfn_glBindRenderbufferEXT)GetFunctionPointer("glBindRenderbufferEXT");
    if ( QGLglBindRenderbufferEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglDeleteRenderbuffersEXT = (pfn_glDeleteRenderbuffersEXT)GetFunctionPointer("glDeleteRenderbuffersEXT");
    if ( QGLglDeleteRenderbuffersEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglGenRenderbuffersEXT = (pfn_glGenRenderbuffersEXT)GetFunctionPointer("glGenRenderbuffersEXT");
    if ( QGLglGenRenderbuffersEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglRenderbufferStorageEXT = (pfn_glRenderbufferStorageEXT)GetFunctionPointer("glRenderbufferStorageEXT");
    if ( QGLglRenderbufferStorageEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglGetRenderbufferParameterivEXT = (pfn_glGetRenderbufferParameterivEXT)GetFunctionPointer("glGetRenderbufferParameterivEXT");
    if ( QGLglGetRenderbufferParameterivEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglIsFramebufferEXT = (pfn_glIsFramebufferEXT)GetFunctionPointer("glIsFramebufferEXT");
    if ( QGLglIsFramebufferEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglBindFramebufferEXT = (pfn_glBindFramebufferEXT)GetFunctionPointer("glBindFramebufferEXT");
    if ( QGLglBindFramebufferEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglDeleteFramebuffersEXT = (pfn_glDeleteFramebuffersEXT)GetFunctionPointer("glDeleteFramebuffersEXT");
    if ( QGLglDeleteFramebuffersEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglGenFramebuffersEXT = (pfn_glGenFramebuffersEXT)GetFunctionPointer("glGenFramebuffersEXT");
    if ( QGLglGenFramebuffersEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglCheckFramebufferStatusEXT = (pfn_glCheckFramebufferStatusEXT)GetFunctionPointer("glCheckFramebufferStatusEXT");
    if ( QGLglCheckFramebufferStatusEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglFramebufferTexture1DEXT = (pfn_glFramebufferTexture1DEXT)GetFunctionPointer("glFramebufferTexture1DEXT");
    if ( QGLglFramebufferTexture1DEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglFramebufferTexture2DEXT = (pfn_glFramebufferTexture2DEXT)GetFunctionPointer("glFramebufferTexture2DEXT");
    if ( QGLglFramebufferTexture2DEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglFramebufferTexture3DEXT = (pfn_glFramebufferTexture3DEXT)GetFunctionPointer("glFramebufferTexture3DEXT");
    if ( QGLglFramebufferTexture3DEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglFramebufferRenderbufferEXT = (pfn_glFramebufferRenderbufferEXT)GetFunctionPointer("glFramebufferRenderbufferEXT");
    if ( QGLglFramebufferRenderbufferEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglGetFramebufferAttachmentParameterivEXT = (pfn_glGetFramebufferAttachmentParameterivEXT)GetFunctionPointer("glGetFramebufferAttachmentParameterivEXT");
    if ( QGLglGetFramebufferAttachmentParameterivEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    QGLglGenerateMipmapEXT = (pfn_glGenerateMipmapEXT)GetFunctionPointer("glGenerateMipmapEXT");
    if ( QGLglGenerateMipmapEXT )
        ++QGLNumFunctions_GL_EXT_framebuffer_object;

    if ( QGLNumFunctions_GL_EXT_framebuffer_object == 17 )
        QGLExists_GL_EXT_framebuffer_object = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_EXT_framebuffer_object = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_EXT_framebuffer_object ()
{
    QGLNumFunctions_GL_EXT_framebuffer_object = 0;
    QGLExists_GL_EXT_framebuffer_object = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
int QGLExists_GL_GREMEDY_string_marker;
int QGLNumFunctions_GL_GREMEDY_string_marker;
#ifdef QGL_DEFINED_GL_GREMEDY_string_marker
pfn_glStringMarkerGREMEDY QGLglStringMarkerGREMEDY = 0;

static void Init_GL_GREMEDY_string_marker ()
{
    QGLNumFunctions_GL_GREMEDY_string_marker = 0;

    QGLglStringMarkerGREMEDY = (pfn_glStringMarkerGREMEDY)GetFunctionPointer("glStringMarkerGREMEDY");
    if ( QGLglStringMarkerGREMEDY )
        ++QGLNumFunctions_GL_GREMEDY_string_marker;

    if ( QGLNumFunctions_GL_GREMEDY_string_marker == 1 )
        QGLExists_GL_GREMEDY_string_marker = QGL_EXT_FULLY_EXISTS;
    else
        QGLExists_GL_GREMEDY_string_marker = QGL_EXT_PARTIALLY_EXISTS;
}
#else
static void Init_GL_GREMEDY_string_marker ()
{
    QGLNumFunctions_GL_GREMEDY_string_marker = 0;
    QGLExists_GL_GREMEDY_string_marker = QGL_EXT_DOES_NOT_EXIST;
}
#endif

//------------------------------------------------------------------------------------------------------------------
void InitializeOpenGLExtensions ()
{
    Init_GL_VERSION_1_1();
    Init_GL_VERSION_1_2();
    Init_GL_VERSION_1_3();
    Init_GL_VERSION_1_4();
    Init_GL_VERSION_1_5();
    Init_GL_VERSION_2_0();
    Init_GL_ARB_imaging();
    Init_GL_ARB_multitexture();
    Init_GL_ARB_transpose_matrix();
    Init_GL_ARB_multisample();
    Init_GL_ARB_texture_env_add();
    Init_GL_ARB_texture_cube_map();
    Init_GL_ARB_texture_compression();
    Init_GL_ARB_texture_border_clamp();
    Init_GL_ARB_point_parameters();
    Init_GL_ARB_vertex_blend();
    Init_GL_ARB_matrix_palette();
    Init_GL_ARB_texture_env_combine();
    Init_GL_ARB_texture_env_crossbar();
    Init_GL_ARB_texture_env_dot3();
    Init_GL_ARB_texture_mirrored_repeat();
    Init_GL_ARB_depth_texture();
    Init_GL_ARB_shadow();
    Init_GL_ARB_shadow_ambient();
    Init_GL_ARB_window_pos();
    Init_GL_ARB_vertex_program();
    Init_GL_ARB_fragment_program();
    Init_GL_ARB_vertex_buffer_object();
    Init_GL_ARB_occlusion_query();
    Init_GL_ARB_shader_objects();
    Init_GL_ARB_vertex_shader();
    Init_GL_ARB_fragment_shader();
    Init_GL_ARB_shading_language_100();
    Init_GL_ARB_texture_non_power_of_two();
    Init_GL_ARB_point_sprite();
    Init_GL_ARB_fragment_program_shadow();
    Init_GL_ARB_draw_buffers();
    Init_GL_ARB_texture_rectangle();
    Init_GL_ARB_color_buffer_float();
    Init_GL_ARB_half_float_pixel();
    Init_GL_ARB_texture_float();
    Init_GL_ARB_pixel_buffer_object();
    Init_GL_EXT_abgr();
    Init_GL_EXT_blend_color();
    Init_GL_EXT_polygon_offset();
    Init_GL_EXT_texture();
    Init_GL_EXT_texture3D();
    Init_GL_SGIS_texture_filter4();
    Init_GL_EXT_subtexture();
    Init_GL_EXT_copy_texture();
    Init_GL_EXT_histogram();
    Init_GL_EXT_convolution();
    Init_GL_EXT_color_matrix();
    Init_GL_SGI_color_table();
    Init_GL_SGIX_pixel_texture();
    Init_GL_SGIS_pixel_texture();
    Init_GL_SGIS_texture4D();
    Init_GL_SGI_texture_color_table();
    Init_GL_EXT_cmyka();
    Init_GL_EXT_texture_object();
    Init_GL_SGIS_detail_texture();
    Init_GL_SGIS_sharpen_texture();
    Init_GL_EXT_packed_pixels();
    Init_GL_SGIS_texture_lod();
    Init_GL_SGIS_multisample();
    Init_GL_EXT_rescale_normal();
    Init_GL_EXT_vertex_array();
    Init_GL_EXT_misc_attribute();
    Init_GL_SGIS_generate_mipmap();
    Init_GL_SGIX_clipmap();
    Init_GL_SGIX_shadow();
    Init_GL_SGIS_texture_edge_clamp();
    Init_GL_SGIS_texture_border_clamp();
    Init_GL_EXT_blend_minmax();
    Init_GL_EXT_blend_subtract();
    Init_GL_EXT_blend_logic_op();
    Init_GL_SGIX_interlace();
    Init_GL_SGIX_pixel_tiles();
    Init_GL_SGIX_texture_select();
    Init_GL_SGIX_sprite();
    Init_GL_SGIX_texture_multi_buffer();
    Init_GL_EXT_point_parameters();
    Init_GL_SGIS_point_parameters();
    Init_GL_SGIX_instruments();
    Init_GL_SGIX_texture_scale_bias();
    Init_GL_SGIX_framezoom();
    Init_GL_SGIX_tag_sample_buffer();
    Init_GL_FfdMaskSGIX();
    Init_GL_SGIX_polynomial_ffd();
    Init_GL_SGIX_reference_plane();
    Init_GL_SGIX_flush_raster();
    Init_GL_SGIX_depth_texture();
    Init_GL_SGIS_fog_function();
    Init_GL_SGIX_fog_offset();
    Init_GL_HP_image_transform();
    Init_GL_HP_convolution_border_modes();
    Init_GL_INGR_palette_buffer();
    Init_GL_SGIX_texture_add_env();
    Init_GL_EXT_color_subtable();
    Init_GL_PGI_vertex_hints();
    Init_GL_PGI_misc_hints();
    Init_GL_EXT_paletted_texture();
    Init_GL_EXT_clip_volume_hint();
    Init_GL_SGIX_list_priority();
    Init_GL_SGIX_ir_instrument1();
    Init_GL_SGIX_calligraphic_fragment();
    Init_GL_SGIX_texture_lod_bias();
    Init_GL_SGIX_shadow_ambient();
    Init_GL_EXT_index_texture();
    Init_GL_EXT_index_material();
    Init_GL_EXT_index_func();
    Init_GL_EXT_index_array_formats();
    Init_GL_EXT_compiled_vertex_array();
    Init_GL_EXT_cull_vertex();
    Init_GL_SGIX_ycrcb();
    Init_GL_SGIX_fragment_lighting();
    Init_GL_IBM_rasterpos_clip();
    Init_GL_HP_texture_lighting();
    Init_GL_EXT_draw_range_elements();
    Init_GL_WIN_phong_shading();
    Init_GL_WIN_specular_fog();
    Init_GL_EXT_light_texture();
    Init_GL_SGIX_blend_alpha_minmax();
    Init_GL_SGIX_impact_pixel_texture();
    Init_GL_EXT_bgra();
    Init_GL_SGIX_async();
    Init_GL_SGIX_async_pixel();
    Init_GL_SGIX_async_histogram();
    Init_GL_INTEL_texture_scissor();
    Init_GL_INTEL_parallel_arrays();
    Init_GL_HP_occlusion_test();
    Init_GL_EXT_pixel_transform();
    Init_GL_EXT_pixel_transform_color_table();
    Init_GL_EXT_shared_texture_palette();
    Init_GL_EXT_separate_specular_color();
    Init_GL_EXT_secondary_color();
    Init_GL_EXT_texture_perturb_normal();
    Init_GL_EXT_multi_draw_arrays();
    Init_GL_EXT_fog_coord();
    Init_GL_REND_screen_coordinates();
    Init_GL_EXT_coordinate_frame();
    Init_GL_EXT_texture_env_combine();
    Init_GL_APPLE_specular_vector();
    Init_GL_APPLE_transform_hint();
    Init_GL_SGIX_fog_scale();
    Init_GL_SUNX_constant_data();
    Init_GL_SUN_global_alpha();
    Init_GL_SUN_triangle_list();
    Init_GL_SUN_vertex();
    Init_GL_EXT_blend_func_separate();
    Init_GL_INGR_blend_func_separate();
    Init_GL_INGR_color_clamp();
    Init_GL_INGR_interlace_read();
    Init_GL_EXT_stencil_wrap();
    Init_GL_EXT_422_pixels();
    Init_GL_NV_texgen_reflection();
    Init_GL_EXT_texture_cube_map();
    Init_GL_SUN_convolution_border_modes();
    Init_GL_EXT_texture_env_add();
    Init_GL_EXT_texture_lod_bias();
    Init_GL_EXT_texture_filter_anisotropic();
    Init_GL_EXT_vertex_weighting();
    Init_GL_NV_light_max_exponent();
    Init_GL_NV_vertex_array_range();
    Init_GL_NV_register_combiners();
    Init_GL_NV_fog_distance();
    Init_GL_NV_texgen_emboss();
    Init_GL_NV_blend_square();
    Init_GL_NV_texture_env_combine4();
    Init_GL_MESA_resize_buffers();
    Init_GL_MESA_window_pos();
    Init_GL_EXT_texture_compression_s3tc();
    Init_GL_IBM_cull_vertex();
    Init_GL_IBM_multimode_draw_arrays();
    Init_GL_IBM_vertex_array_lists();
    Init_GL_SGIX_subsample();
    Init_GL_SGIX_ycrcb_subsample();
    Init_GL_SGIX_ycrcba();
    Init_GL_SGIX_depth_pass_instrument();
    Init_GL_3DFX_texture_compression_FXT1();
    Init_GL_3DFX_multisample();
    Init_GL_3DFX_tbuffer();
    Init_GL_EXT_multisample();
    Init_GL_SGIX_vertex_preclip();
    Init_GL_SGIX_convolution_accuracy();
    Init_GL_SGIX_resample();
    Init_GL_SGIS_point_line_texgen();
    Init_GL_SGIS_texture_color_mask();
    Init_GL_SGIX_igloo_interface();
    Init_GL_EXT_texture_env_dot3();
    Init_GL_ATI_texture_mirror_once();
    Init_GL_NV_fence();
    Init_GL_IBM_texture_mirrored_repeat();
    Init_GL_NV_evaluators();
    Init_GL_NV_packed_depth_stencil();
    Init_GL_NV_register_combiners2();
    Init_GL_NV_texture_compression_vtc();
    Init_GL_NV_texture_rectangle();
    Init_GL_NV_texture_shader();
    Init_GL_NV_texture_shader2();
    Init_GL_NV_vertex_array_range2();
    Init_GL_NV_vertex_program();
    Init_GL_SGIX_texture_coordinate_clamp();
    Init_GL_SGIX_scalebias_hint();
    Init_GL_OML_interlace();
    Init_GL_OML_subsample();
    Init_GL_OML_resample();
    Init_GL_NV_copy_depth_to_color();
    Init_GL_ATI_envmap_bumpmap();
    Init_GL_ATI_fragment_shader();
    Init_GL_ATI_pn_triangles();
    Init_GL_ATI_vertex_array_object();
    Init_GL_EXT_vertex_shader();
    Init_GL_ATI_vertex_streams();
    Init_GL_ATI_element_array();
    Init_GL_SUN_mesh_array();
    Init_GL_SUN_slice_accum();
    Init_GL_NV_multisample_filter_hint();
    Init_GL_NV_depth_clamp();
    Init_GL_NV_occlusion_query();
    Init_GL_NV_point_sprite();
    Init_GL_NV_texture_shader3();
    Init_GL_NV_vertex_program1_1();
    Init_GL_EXT_shadow_funcs();
    Init_GL_EXT_stencil_two_side();
    Init_GL_ATI_text_fragment_shader();
    Init_GL_APPLE_client_storage();
    Init_GL_APPLE_element_array();
    Init_GL_APPLE_fence();
    Init_GL_APPLE_vertex_array_object();
    Init_GL_APPLE_vertex_array_range();
    Init_GL_APPLE_ycbcr_422();
    Init_GL_S3_s3tc();
    Init_GL_ATI_draw_buffers();
    Init_GL_ATI_pixel_format_float();
    Init_GL_ATI_texture_env_combine3();
    Init_GL_ATI_texture_float();
    Init_GL_NV_float_buffer();
    Init_GL_NV_fragment_program();
    Init_GL_NV_half_float();
    Init_GL_NV_pixel_data_range();
    Init_GL_NV_primitive_restart();
    Init_GL_NV_texture_expand_normal();
    Init_GL_NV_vertex_program2();
    Init_GL_ATI_map_object_buffer();
    Init_GL_ATI_separate_stencil();
    Init_GL_ATI_vertex_attrib_array_object();
    Init_GL_OES_read_format();
    Init_GL_EXT_depth_bounds_test();
    Init_GL_EXT_texture_mirror_clamp();
    Init_GL_EXT_blend_equation_separate();
    Init_GL_MESA_pack_invert();
    Init_GL_MESA_ycbcr_texture();
    Init_GL_EXT_pixel_buffer_object();
    Init_GL_NV_fragment_program_option();
    Init_GL_NV_fragment_program2();
    Init_GL_NV_vertex_program2_option();
    Init_GL_NV_vertex_program3();
    Init_GL_EXT_framebuffer_object();
    Init_GL_GREMEDY_string_marker();
}
//------------------------------------------------------------------------------------------------------------------



