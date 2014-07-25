#include "QOpenGLRendering.h"
#include "QIdenUtils.h"
#include "QSystUtils.h"
#include "QIntrBox3Frustum3.h"
using namespace Q;

unsigned int OpenGLRendering::ms_uiNextContextId(1);

//------------------------------------------------------------------------------------------------------------------
OpenGLRendering::OpenGLRendering (const FramebufferParams& rqRequestedFramebufferParams)
    :
    HardwareRendering(rqRequestedFramebufferParams),
    m_iCurrentContextId(0),
    m_pqCurrentContext(0),
    m_bEnterMultisample(false),
    m_iPushedModelViewMatrixQuantity(2),
    m_qSecondEchelonModelViewStackf(1),
    m_qSecondEchelonModelViewStackd(1),
    m_bTextureStateActivated(false),
    m_bTexture1dEnabled(false),
    m_bTexture2dEnabled(false),
    m_bTexture3dEnabled(false),
    m_bTextureCubeMapEnabled(false),
    m_qPushedStatesGlobal(true),
    m_qPushedStatesLocal(false)
{
    // create root nodes
    m_qRootNodef = new HRNodef(0);
    m_qRootNodef->OnAttach();
    m_qRootNoded = new HRNoded(0);
    m_qRootNoded->OnAttach();
}
//------------------------------------------------------------------------------------------------------------------
#ifdef _WIN32
bool OpenGLRendering::Initialize (HWND hWnd, bool (*oRecreateWindowFunction)(HWND& rhWnd))
{
    LogInfo("Initializing the OpenGL renderer...");

    if ( m_qRequestedFramebufferParams.ColorMode == FramebufferParams::CMT_COLOR_MODE_INDEX )
    {
        LogAndThrowFatalError("The requested index color mode currently not supported");
        assert( false );
        return false;
    }

    int iId;
    if ( AddContext(hWnd,iId) )
    {
        // set the current context and initialize extensions
        if ( !SetCurrentContext(iId,oRecreateWindowFunction) )
            return false;
    }
    else
    {
        return false;
    }

    LogInfo("Requesting versions and available extensions...");
    m_qGLVendorInfo = (const char*)glGetString(GL_VENDOR);
    m_qGLRendererInfo = (const char*)glGetString(GL_RENDERER);
    m_qGLVersionInfo = (const char*)glGetString(GL_VERSION);
    m_qGLExtensionInfo = (const char*)glGetString(GL_EXTENSIONS);
    LogToLastItem("Done.");

    LogInfo("Requesting main limitations...");
    glGetIntegerv(GL_MAX_LIGHTS,&m_iMaxLights);
    glGetIntegerv(GL_MAX_CLIP_PLANES,&m_iMaxClippingPlanes);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&m_iMaxTextureSize);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&m_iMaxModelViewStackSize);
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH,&m_iMaxAttribStackSize);
    glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE,&m_iMaxPixelMapTableSize);
    if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
        glGetIntegerv(GL_MAX_TEXTURE_COORDS,&m_iMaxTextureCoords);
    if ( QGLExists_GL_EXT_convolution == QGL_EXT_FULLY_EXISTS )
    {
        glGetIntegerv(GL_MAX_CONVOLUTION_WIDTH,&m_iMaxConvolutionFilterWidth);
        glGetIntegerv(GL_MAX_CONVOLUTION_HEIGHT,&m_iMaxConvolutionFilterHeight);
    }
    LogToLastItem("Done.");

    LogInfo("...The renderer has been initialized.");

    m_bInitialized = true;

    return true;
}
#endif
//------------------------------------------------------------------------------------------------------------------
OpenGLRendering::~OpenGLRendering ()
{
    LogInfo("Displacing the renderer...");

    LogInfo("Freeing memory from buffered data...");
    m_qRootNodef->OnDetach();
    m_qRootNoded->OnDetach();
    LogToLastItem("Done.");

    Map<int,ContextRecord>::IteratorT qIt = m_qIdToContext.GetBegin();
    for (int i = 0; i < m_qIdToContext.GetQuantity(); i++)
    {
#ifdef _WIN32
        sprintf(m_acLogTmp,"Removing the rendering context with id %i...",qIt->First);
        LogInfo(m_acLogTmp);
        if ( wglDeleteContext(qIt->Second.RC) )
            LogToLastItem("Done.");
        else
            LogAndThrowFatalError("Cannot remove the rendering context");

        LogInfo("Releasing the window drawing context...");
        if ( ReleaseDC(qIt->Second.Wnd,qIt->Second.DC) == 1 )
            LogToLastItem("Done.");
        else
            LogAndThrowFatalError("Cannot release the window drawing context");
#endif
        ++qIt;
    }

    LogInfo("...The OpenGL renderer has been displaced.");
}
//------------------------------------------------------------------------------------------------------------------
#ifdef _WIN32
bool OpenGLRendering::AddContext (HWND hWnd, int& riId)
{
    LogInfo("Adding a new rendering context...");

    PIXELFORMATDESCRIPTOR qPFD;
    MemSet(&qPFD,0,sizeof(PIXELFORMATDESCRIPTOR));

    qPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    qPFD.nVersion = 1;
    qPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;

    // buffering
    switch ( m_qRequestedFramebufferParams.Buffering )
    {
    case FramebufferParams::BT_BUFFERING_SINGLE:
        break;
    case FramebufferParams::BT_BUFFERING_DOUBLE:
        qPFD.dwFlags |= PFD_DOUBLEBUFFER;
        break;
    }

    // color mode
    switch ( m_qRequestedFramebufferParams.ColorMode )
    {
    case FramebufferParams::CMT_COLOR_MODE_RGBA:
        qPFD.iPixelType = PFD_TYPE_RGBA;
        break;
    }

    // color depth
    switch ( m_qRequestedFramebufferParams.ColorDepth )
    {
    case FramebufferParams::CDT_COLOR_DEPTH_16:
        qPFD.cColorBits = 16;
        break;
    case FramebufferParams::CDT_COLOR_DEPTH_24:
        qPFD.cColorBits = 24;
        break;
    case FramebufferParams::CDT_COLOR_DEPTH_32:
        qPFD.cColorBits = 32;
        break;
    }

    // depth buffer
    switch ( m_qRequestedFramebufferParams.Depth )
    {
    case FramebufferParams::DT_DEPTH_NONE:
        break;
    case FramebufferParams::DT_DEPTH_16:
        qPFD.cDepthBits = 16;
        break;
    case FramebufferParams::DT_DEPTH_24:
        qPFD.cDepthBits = 24;
        break;
    case FramebufferParams::DT_DEPTH_32:
        qPFD.cDepthBits = 32;
        break;
    }

    // stencil buffer
    switch ( m_qRequestedFramebufferParams.Stencil )
    {
    case FramebufferParams::ST_STENCIL_NONE:
        break;
    case FramebufferParams::ST_STENCIL_8:
        qPFD.cStencilBits = 8;
        break;
    }

    qPFD.iLayerType = PFD_MAIN_PLANE;

    LogInfo("Getting a window drawing context...");
    HDC hDC = ::GetDC(hWnd);
    if ( hDC != 0 )
    {
        LogToLastItem("Done.");
    }
    else
    {
        LogAndThrowFatalError("Cannot get the window drawing context");
        return false;
    }

    int iPixelFormat;
    if ( !m_bEnterMultisample )
    {
        LogInfo("Choosing a window pixel format...");
        iPixelFormat = ChoosePixelFormat(hDC,&qPFD);
        if ( iPixelFormat != 0 )
        {
            LogToLastItem("Done.");
        }
        else
        {
            LogAndThrowFatalError("Cannot choose the window pixel format");
            return false;
        }
    }
    else
    {
        iPixelFormat = m_iMultisamplePixelFormat;
        m_bEnterMultisample = false;
    }

    LogInfo("Setting the window pixel format...");
    if ( SetPixelFormat(hDC,iPixelFormat,&qPFD) )
    {
        LogToLastItem("Done.");
    }
    else
    {
        LogAndThrowFatalError("Cannot set the window pixel format");
        return false;
    }

    if ( !m_bInitialized )
    {
        LogInfo("Requesting a pixel format description for the window...");
        if ( DescribePixelFormat(hDC,iPixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&qPFD) != 0 )
        {
            LogToLastItem("Done.");
        }
        else
        {
            LogAndThrowFatalError("Cannot obtain the pixel format description for the window");
            return false;
        }

        // Generate present framebuffer parameters.

        // buffering
        if ( !(qPFD.dwFlags & PFD_DOUBLEBUFFER) )
            m_qPresentFramebufferParams.Buffering = FramebufferParams::BT_BUFFERING_SINGLE;
        else
            m_qPresentFramebufferParams.Buffering = FramebufferParams::BT_BUFFERING_DOUBLE;

        // color mode
        if ( qPFD.iPixelType == PFD_TYPE_RGBA )
            m_qPresentFramebufferParams.ColorMode = FramebufferParams::CMT_COLOR_MODE_RGBA;
        else
            m_qPresentFramebufferParams.ColorMode = FramebufferParams::CMT_COLOR_MODE_INDEX;

        // color depth
        switch ( qPFD.cColorBits )
        {
        case 16:
            m_qPresentFramebufferParams.ColorDepth = FramebufferParams::CDT_COLOR_DEPTH_16;
            break;
        case 24:
            m_qPresentFramebufferParams.ColorDepth = FramebufferParams::CDT_COLOR_DEPTH_24;
            break;
        case 32:
            m_qPresentFramebufferParams.ColorDepth = FramebufferParams::CDT_COLOR_DEPTH_32;
            break;
        }

        // depth buffer
        switch ( qPFD.cDepthBits )
        {
        case 0:
            m_qPresentFramebufferParams.Depth = FramebufferParams::DT_DEPTH_NONE;
            break;
        case 16:
            m_qPresentFramebufferParams.Depth = FramebufferParams::DT_DEPTH_16;
            break;
        case 24:
            m_qPresentFramebufferParams.Depth = FramebufferParams::DT_DEPTH_24;
            break;
        case 32:
            m_qPresentFramebufferParams.Depth = FramebufferParams::DT_DEPTH_32;
            break;
        }

        // stencil buffer
        switch ( qPFD.cStencilBits )
        {
        case 0:
            m_qPresentFramebufferParams.Stencil = FramebufferParams::ST_STENCIL_NONE;
            break;
        case 8:
            m_qPresentFramebufferParams.Stencil = FramebufferParams::ST_STENCIL_8;
            break;
        }

        if ( m_qRequestedFramebufferParams.Buffering != m_qPresentFramebufferParams.Buffering )
            LogWarning("The requested and present buffering multiplicity mismatch");
        if ( m_qRequestedFramebufferParams.ColorMode != m_qPresentFramebufferParams.ColorMode )
            LogWarning("The requested and present color modes mismatch");
        if ( m_qRequestedFramebufferParams.ColorDepth != m_qPresentFramebufferParams.ColorDepth )
            LogWarning("The requested and present color depths mismatch");
        if ( m_qRequestedFramebufferParams.Stencil != m_qPresentFramebufferParams.Stencil )
            LogWarning("The requested and present stencil buffering mismatch");
    }

    LogInfo("Creating the rendering context...");
    HGLRC hRC = wglCreateContext(hDC);
    if ( hRC != 0 )
    {
        LogToLastItem("Done.");
    }
    else
    {
        LogAndThrowFatalError("Cannot create the rendering context");
        return false;
    }

    if ( !m_qIdToContext.IsEmpty() )
    {
        LogInfo("Sharing display lists between the rendering contexts...");
        if ( wglShareLists(m_qIdToContext.GetBegin()->Second.RC,hRC) )
        {
            LogToLastItem("Done.");
        }
        else
        {
            LogAndThrowFatalError("Cannot share display lists between the rendering contexts");
            return false;
        }
    }

    riId = ms_uiNextContextId++;
    m_qIdToContext.Insert(riId,ContextRecord(hWnd,hDC,hRC));

    // context-specific options
    if ( QGLExists_WGL_EXT_swap_control == QGL_EXT_FULLY_EXISTS )
    {
        if ( wglGetSwapIntervalEXT() != ( m_bDoVerticalSync ? 1 : 0 ) )
        {
            LogInfo("Setting vertical synchronization...");
            wglSwapIntervalEXT(( m_bDoVerticalSync ? 1 : 0 ));
            LogToLastItem("Done.");
        }
    }

    sprintf(m_acLogTmp,"...The new rendering context has been added with id %i.",riId);
    LogInfo(m_acLogTmp);

    return true;
}
#endif
//------------------------------------------------------------------------------------------------------------------
#ifdef _WIN32
bool OpenGLRendering::SetCurrentContext (int iId, bool (*oRecreateWindowFunction)(HWND& rhWnd))
{
    assert( iId >= 1 );

    Map<int,ContextRecord>::IteratorT qIt = m_qIdToContext.Find(iId);
    if ( qIt == m_qIdToContext.GetEnd() )
    {
        assert( false );
        return false;
    }

    sprintf(m_acLogTmp,"Setting the context with id %i as current...",iId);
    LogInfo(m_acLogTmp);
    if ( wglMakeCurrent(qIt->Second.DC,qIt->Second.RC) )
    {
        m_iCurrentContextId = iId;
        LogToLastItem("Done.");

        // there is an unavoidable inconsistency with wglGetProcAddress which returns context-dependent
        // pointers, thus have to do the following even if the context isn't new
        LogInfo("Initializing the OpenGL extensions...");
        InitializeOpenGLExtensions();
        InitializeWgl();
        LogToLastItem("Done.");

        // multisampling
        if ( !qIt->Second.MultisampleInitialized && m_qRequestedFramebufferParams.Multisample !=
             FramebufferParams::MT_MULTISAMPLE_NONE )
        {
            assert( oRecreateWindowFunction );

            LogInfo("Initializing the multisampling...");

            if ( wglChoosePixelFormatARB )
            {
                int aiAttributes[256], iPos = 0;
                aiAttributes[iPos++] = WGL_SUPPORT_OPENGL_ARB;
                aiAttributes[iPos++] = 1;
                aiAttributes[iPos++] = WGL_DRAW_TO_WINDOW_ARB;
                aiAttributes[iPos++] = 1;
                aiAttributes[iPos++] = WGL_ACCELERATION_ARB;
                aiAttributes[iPos++] = WGL_FULL_ACCELERATION_ARB;
                aiAttributes[iPos++] = WGL_PIXEL_TYPE_ARB;
                aiAttributes[iPos++] = WGL_TYPE_RGBA_ARB;
                aiAttributes[iPos++] = WGL_RED_BITS_ARB;
                aiAttributes[iPos++] = 8;
                aiAttributes[iPos++] = WGL_GREEN_BITS_ARB;
                aiAttributes[iPos++] = 8;
                aiAttributes[iPos++] = WGL_BLUE_BITS_ARB;
                aiAttributes[iPos++] = 8;
                aiAttributes[iPos++] = WGL_ALPHA_BITS_ARB;
                aiAttributes[iPos++] = 8;

                // buffering
                if ( m_qRequestedFramebufferParams.Stencil == FramebufferParams::BT_BUFFERING_DOUBLE )
                {
                    aiAttributes[iPos++] = WGL_DOUBLE_BUFFER_ARB;
                    aiAttributes[iPos++] = 1;
                }

                // depth buffer
                if ( m_qRequestedFramebufferParams.Depth != FramebufferParams::DT_DEPTH_NONE )
                {
                    aiAttributes[iPos++] = WGL_DEPTH_BITS_ARB;
                    switch ( m_qRequestedFramebufferParams.Depth )
                    {
                    case FramebufferParams::DT_DEPTH_16:
                        aiAttributes[iPos++] = 16;
                        break;
                    case FramebufferParams::DT_DEPTH_24:
                        aiAttributes[iPos++] = 24;
                        break;
                    case FramebufferParams::DT_DEPTH_32:
                        aiAttributes[iPos++] = 32;
                        break;
                    }
                }

                // stencil buffer
                if ( m_qRequestedFramebufferParams.Stencil == FramebufferParams::ST_STENCIL_8 )
                {
                    aiAttributes[iPos++] = WGL_STENCIL_BITS_ARB;
                    aiAttributes[iPos++] = 8;
                }

                // multisampling
                aiAttributes[iPos++] = WGL_SAMPLE_BUFFERS_ARB;
                aiAttributes[iPos++] = 1;
                aiAttributes[iPos++] = WGL_SAMPLES_ARB;
                if ( m_qRequestedFramebufferParams.Multisample == FramebufferParams::MT_MULTISAMPLE_2 )
                    aiAttributes[iPos++] = 2;
                else
                    aiAttributes[iPos++] = 4;

                // end of attributes
                aiAttributes[iPos++] = 0;

                unsigned int uiNumFormats;
                LogInfo("Choosing a window pixel format...");
                if ( wglChoosePixelFormatARB(qIt->Second.DC,aiAttributes,0,1,&m_iMultisamplePixelFormat,
                     &uiNumFormats) && uiNumFormats > 0 )
                {
                    LogToLastItem("Done.");

                    // remove this context
                    RemoveContext(iId);

                    // recreate the window
                    LogInfo("Recreating a window...");
                    HWND hWND;
                    if ( oRecreateWindowFunction(hWND) )
                    {
                        LogToLastItem("Done.");

                        // add the context
                        m_bEnterMultisample = true;
                        int iNewId;
                        AddContext(hWND,iNewId);

                        // set it as current
                        qIt = m_qIdToContext.Find(iNewId);
                        sprintf(m_acLogTmp,"Setting the context with id %i as current...",iNewId);
                        LogInfo(m_acLogTmp);
                        if ( wglMakeCurrent(qIt->Second.DC,qIt->Second.RC) )
                        {
                            m_iCurrentContextId = iNewId;
                            LogToLastItem("Done.");
                            qIt->Second.MultisampleInitialized = true;
                            LogInfo("...The multisampling has been initialized.");
                        }
                        else
                        {
                            LogAndThrowFatalError("Cannot set the current rendering context");
                            return false;
                        }
                    }
                    else
                    {
                        LogAndThrowFatalError("Cannot recreate the window");
                        return false;
                    }
                }
                else
                {
                    LogError("Cannot choose the window pixel format");
                    LogError("...Initialization of the multisampling failed.");
                }
            }
            else
            {
                LogError("Bad pointer to an extension function");
                LogError("...Initialization of the multisampling failed.");
            }
        }
    }
    else
    {
        LogAndThrowFatalError("Cannot set the current rendering context");
        return false;
    }
    m_pqCurrentContext = &qIt->Second;
    return true;
}
#endif
//------------------------------------------------------------------------------------------------------------------
bool OpenGLRendering::RemoveContext (int iId)
{
    assert( iId >= 1 );

    Map<int,ContextRecord>::IteratorT qIt = m_qIdToContext.Find(iId);
    if ( qIt == m_qIdToContext.GetEnd() )
    {
        assert( false );
        return false;
    }
#ifdef _WIN32
    sprintf(m_acLogTmp,"Removing the rendering context with id %i...",iId);
    LogInfo(m_acLogTmp);
    if ( wglDeleteContext(qIt->Second.RC) )
    {
        LogToLastItem("Done.");
        LogInfo("Releasing the window drawing context...");
        if ( ReleaseDC(qIt->Second.Wnd,qIt->Second.DC) != 0 )
        {
            LogToLastItem("Done.");

            m_qIdToContext.Remove(qIt);
            m_iCurrentContextId = 0;
            m_pqCurrentContext = 0;
            return true;
        }
        else
        {
            LogAndThrowFatalError("Cannot release the window drawing context");
            return false;
        }
    }
    else
    {
        LogAndThrowFatalError("Cannot remove the rendering context");
        return false;
    }
#endif
}
//------------------------------------------------------------------------------------------------------------------
int OpenGLRendering::BindGlobalState (const SmartRttiPointer& rspqState)
{
    // projection/texture state not allowed as global
    assert( !rspqState->IsExactly(ProjectionStatef::Class) && !rspqState->IsExactly(ProjectionStated::Class)
        &&  !rspqState->IsExactly(TextureStatef::Class) && !rspqState->IsExactly(TextureStated::Class) );

    return m_qGlobalStates.BindState(rspqState);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::UnbindGlobalState (int i)
{
    m_qGlobalStates.UnbindState(i);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::BeginFrame ()
{
    assert( m_bInitialized );

    // viewport
    glViewport(m_pqCurrentContext->Viewport.X,m_pqCurrentContext->Viewport.Y,m_pqCurrentContext->Viewport.Width,
        m_pqCurrentContext->Viewport.Height);

    // prepare the framebuffer
    if ( m_pqCurrentContext->DoColorBufferClear )
    {
        glClearColor(
            m_pqCurrentContext->BackgroundColor.R(),
            m_pqCurrentContext->BackgroundColor.G(),
            m_pqCurrentContext->BackgroundColor.B(),
            m_pqCurrentContext->BackgroundColor.A());
    }
    if ( m_pqCurrentContext->DoDepthBufferClear )
        glClearDepth(m_pqCurrentContext->DepthBufferClearValue);
    if ( m_pqCurrentContext->DoColorBufferClear && m_pqCurrentContext->DoDepthBufferClear )
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else if ( m_pqCurrentContext->DoColorBufferClear )
        glClear(GL_COLOR_BUFFER_BIT);
    else if ( m_pqCurrentContext->DoDepthBufferClear )
        glClear(GL_DEPTH_BUFFER_BIT);

    // camera
    if ( m_pqCurrentContext->Cameraf )
    {
        // transformation
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(m_pqCurrentContext->Cameraf->GetTransformationMatrix().GetInversed());

        // projection
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(m_pqCurrentContext->Cameraf->GetProjectionMatrix());
    }
    else
    {
        // transformation
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(m_pqCurrentContext->Camerad->GetTransformationMatrix().GetInversed());

        // projection
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(m_pqCurrentContext->Camerad->GetProjectionMatrix());
    }

    // embody bound global states
    ActivateStates(m_qGlobalStates,m_qPushedStatesGlobal);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::EndFrame ()
{
    assert( m_bInitialized );

    // draw bound global states back
    DeactivateStates(m_qPushedStatesGlobal);

    // command show drawn
    if ( m_qRequestedFramebufferParams.Buffering == FramebufferParams::BT_BUFFERING_DOUBLE )
    {
        // swap buffers
        SwapBuffers();
    }
    else
    {
        // flush
        glFlush();
    }

    // check for errors
    CheckForErrors();
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawGeometry (const HRGeometryfPtr& rspqGeometry, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRGeometryf& rqGeometry = *rspqGeometry;

    // test bound-frustum intersection
    if ( !(rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER && rqGeometry.NeedRebuffer) )
    {
        // it isn't buffering stage
        assert( m_pqCurrentContext->Cameraf );
        IntrBox3Frustum3f qBoundsIntr(rqGeometry.GetGlobalBoundingBox(),m_pqCurrentContext->Cameraf->GetFrustum());
        qBoundsIntr.ImpartBoundingBox1(m_pqCurrentContext->Cameraf->GetFrustumAABoundingBox());
        if ( !qBoundsIntr.Test() )
            return;
    }

    if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER && !rqGeometry.NeedRebuffer
    &&   rqGeometry.BufferId != 0 )
    {
        // lone texture state
        ActivateTextureState(rqGeometry);

        // call the previously created display list
        glCallList(rqGeometry.BufferId);

        // draw texture state back
        DeactivateTextureState();

        return;
    }

    assert( rqGeometry.Vertices2 || rqGeometry.Vertices3 || rqGeometry.Vertices4 );

    // only one vertex dimension must be present
    assert( (rqGeometry.Vertices2 && !rqGeometry.Vertices3 && !rqGeometry.Vertices4)
        ||  (!rqGeometry.Vertices2 && rqGeometry.Vertices3 && !rqGeometry.Vertices4)
        ||  (!rqGeometry.Vertices2 && !rqGeometry.Vertices3 && rqGeometry.Vertices4) );

    // none or only one color array
    assert( (!rqGeometry.RgbColors && !rqGeometry.RgbaColors)
        ||  (rqGeometry.RgbColors && !rqGeometry.RgbaColors)
        ||  (!rqGeometry.RgbColors && rqGeometry.RgbaColors) );

    // none or only one texture coordinate array of some dimension
    assert( (!rqGeometry.TexCoords1 && !rqGeometry.TexCoords2
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords1 && !rqGeometry.TexCoords2
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords2 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords3 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords2 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords4 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords2 && !rqGeometry.TexCoords3) );

    // edge flags, if specified, are only for separate triangles, quads, and polygons
    assert( !(rqGeometry.EdgeFlags
        &&  (rqGeometry.Geometry != HRGeometryf::GT_TRIANGLES_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryf::GT_QUADS_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryf::GT_POLYGONS
        &&  rqGeometry.Geometry != HRGeometryf::GT_INDEXED_TRIANGLES_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryf::GT_INDEXED_QUADS_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryf::GT_INDEXED_POLYGONS)) );

    // draw
    int iQuantity1;
    if ( rqGeometry.Geometry == HRGeometryf::GT_POINTS
    ||   rqGeometry.Geometry == HRGeometryf::GT_LINES_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryf::GT_LINES_STRIP
    ||   rqGeometry.Geometry == HRGeometryf::GT_LINES_LOOP
    ||   rqGeometry.Geometry == HRGeometryf::GT_TRIANGLES_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryf::GT_TRIANGLES_STRIP
    ||   rqGeometry.Geometry == HRGeometryf::GT_TRIANGLES_FAN
    ||   rqGeometry.Geometry == HRGeometryf::GT_QUADS_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryf::GT_QUADS_STRIP
    ||   rqGeometry.Geometry == HRGeometryf::GT_POLYGONS )
    {
        // not indexed geometry

        // lone texture state
        ActivateTextureState(rqGeometry);

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqGeometry.BufferId != 0 )
                glDeleteLists(rqGeometry.BufferId,1);

            rqGeometry.BufferId = glGenLists(1);
            if ( rqGeometry.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqGeometry.NeedRebuffer = false;
            glNewList(rqGeometry.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // transformation
        bool bIdentity = true;
        if ( rqGeometry.GetTransformationMatrix() != Matrix4f::IDENTITY )
        {
            bIdentity = false;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixf(rqGeometry.GetTransformationMatrix());
        }

        // normalizing
        bool bUnitScale = true;
        if ( rqGeometry.GetScale().X() != 1.0f
        ||   rqGeometry.GetScale().Y() != 1.0f
        ||   rqGeometry.GetScale().Z() != 1.0f )
        {
            bUnitScale = false;
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glEnable(GL_RESCALE_NORMAL);
            else
                glEnable(GL_NORMALIZE);
        }

        // embody bound states
        ActivateStates(rqGeometry,m_qPushedStatesLocal);

        switch ( rqGeometry.Geometry )
        {
        case HRGeometryf::GT_POINTS:
            glBegin(GL_POINTS);
            break;
        case HRGeometryf::GT_LINES_SEPARATE:
            glBegin(GL_LINES);
            break;
        case HRGeometryf::GT_LINES_STRIP:
            glBegin(GL_LINE_STRIP);
            break;
        case HRGeometryf::GT_LINES_LOOP:
            glBegin(GL_LINE_LOOP);
            break;
        case HRGeometryf::GT_TRIANGLES_SEPARATE:
            glBegin(GL_TRIANGLES);
            break;
        case HRGeometryf::GT_TRIANGLES_STRIP:
            glBegin(GL_TRIANGLE_STRIP);
            break;
        case HRGeometryf::GT_TRIANGLES_FAN:
            glBegin(GL_TRIANGLE_FAN);
            break;
        case HRGeometryf::GT_QUADS_SEPARATE:
            glBegin(GL_QUADS);
            break;
        case HRGeometryf::GT_QUADS_STRIP:
            glBegin(GL_QUAD_STRIP);
            break;
        case HRGeometryf::GT_POLYGONS:
            glBegin(GL_POLYGON);
            break;
        }

        HRMaterial* pqMaterial;
        unsigned int uiFaceEnum;

        int iQuantity2;
        if ( rqGeometry.Vertices2 )
            iQuantity2 = rqGeometry.Vertices2->GetQuantity();
        else if ( rqGeometry.Vertices3 )
            iQuantity2 = rqGeometry.Vertices3->GetQuantity();
        else if ( rqGeometry.Vertices4 )
            iQuantity2 = rqGeometry.Vertices4->GetQuantity();
        for (int i0 = 0; i0 < iQuantity2; i0++)
        {
            // color
            if ( rqGeometry.RgbColors )
                glColor3fv((*rqGeometry.RgbColors)[i0]);
            else if ( rqGeometry.RgbaColors )
                glColor4fv((*rqGeometry.RgbaColors)[i0]);

            // secondary color
            if ( rqGeometry.SecondaryColors )
            {
                if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
                    glSecondaryColor3fv((*rqGeometry.SecondaryColors)[i0]);
            }

            // normal
            if ( rqGeometry.Normals )
                glNormal3fv((*rqGeometry.Normals)[i0]);

            // material
            if ( rqGeometry.Materials )
            {
                pqMaterial = &(*(*rqGeometry.Materials)[i0]);
                switch ( pqMaterial->Face )
                {
                case HRMaterial::FT_FRONT:
                    uiFaceEnum = GL_FRONT;
                    break;
                case HRMaterial::FT_BACK:
                    uiFaceEnum = GL_BACK;
                    break;
                case HRMaterial::FT_BOTH:
                    uiFaceEnum = GL_FRONT_AND_BACK;
                    break;
                }
                glMaterialfv(uiFaceEnum,GL_AMBIENT,pqMaterial->Ambient);
                glMaterialfv(uiFaceEnum,GL_DIFFUSE,pqMaterial->Diffuse);
                glMaterialfv(uiFaceEnum,GL_SPECULAR,pqMaterial->Specular);
                glMaterialfv(uiFaceEnum,GL_EMISSION,pqMaterial->Emission);
                glMaterialf(uiFaceEnum,GL_SHININESS,pqMaterial->Shininess);
            }

            // texture coordinates
            if ( rqGeometry.TexCoords1 )
            {
                iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord1f((*(*rqGeometry.TexCoords1)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords1)[i1] )
                            glMultiTexCoord1f(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords1)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords2 )
            {
                iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord2fv((*(*rqGeometry.TexCoords2)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords2)[i1] )
                            glMultiTexCoord2fv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords2)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords3 )
            {
                iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord3fv((*(*rqGeometry.TexCoords3)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords3)[i1] )
                            glMultiTexCoord3fv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords3)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords4 )
            {
                iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord4fv((*(*rqGeometry.TexCoords4)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords4)[i1] )
                            glMultiTexCoord4fv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords4)[i1])[i0]);
                    }
                }
            }

            // edge flag
            if ( rqGeometry.EdgeFlags )
                glEdgeFlag((*rqGeometry.EdgeFlags)[i0]);

            // fog coordinate
            if ( rqGeometry.FogCoords )
            {
                if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
                    glFogCoordf((*rqGeometry.FogCoords)[i0]);
            }

            // vertex
            if ( rqGeometry.Vertices2 )
                glVertex2fv((*rqGeometry.Vertices2)[i0]);
            else if ( rqGeometry.Vertices3 )
                glVertex3fv((*rqGeometry.Vertices3)[i0]);
            else if ( rqGeometry.Vertices4 )
                glVertex4fv((*rqGeometry.Vertices4)[i0]);
        }

        glEnd();

        // draw bound states back
        DeactivateStates(m_qPushedStatesLocal);

        // normalizing
        if ( !bUnitScale )
        {
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glDisable(GL_RESCALE_NORMAL);
            else
                glDisable(GL_NORMALIZE);
        }

        // transformation
        if ( !bIdentity )
        {
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();

        // draw texture state back
        DeactivateTextureState();
    }
    else
    {
        // indexed geometry

        // materials per vertex cannot be specified for indexed geometry
        assert( !rqGeometry.Materials );

        // Enable client states and set pointers.

        // colors
        if ( rqGeometry.RgbColors )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3,GL_FLOAT,0,*rqGeometry.RgbColors);
        }
        else if ( rqGeometry.RgbaColors )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4,GL_FLOAT,0,*rqGeometry.RgbaColors);
        }

        // secondary colors
        if ( rqGeometry.SecondaryColors )
        {
            if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
            {
                glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
                glSecondaryColorPointer(3,GL_FLOAT,0,*rqGeometry.SecondaryColors);
            }
        }

        // normals
        if ( rqGeometry.Normals )
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT,0,*rqGeometry.Normals);
        }

        // texture coordinates
        if ( rqGeometry.TexCoords1 )
        {
            iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(1,GL_FLOAT,0,*((*rqGeometry.TexCoords1)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords1)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(1,GL_FLOAT,0,*((*rqGeometry.TexCoords1)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords2 )
        {
            iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2,GL_FLOAT,0,*((*rqGeometry.TexCoords2)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords2)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(2,GL_FLOAT,0,*((*rqGeometry.TexCoords2)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords3 )
        {
            iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(3,GL_FLOAT,0,*((*rqGeometry.TexCoords3)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords3)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(3,GL_FLOAT,0,*((*rqGeometry.TexCoords3)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords4 )
        {
            iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(4,GL_FLOAT,0,*((*rqGeometry.TexCoords4)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords4)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(4,GL_FLOAT,0,*((*rqGeometry.TexCoords4)[i]));
                    }
                }
            }
        }

        // edge flags
        if ( rqGeometry.EdgeFlags )
        {
            glEnableClientState(GL_EDGE_FLAG_ARRAY);
            glEdgeFlagPointer(0,*rqGeometry.EdgeFlags);
        }

        // fog coordinates
        if ( rqGeometry.FogCoords )
        {
            if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
            {
                glEnableClientState(GL_FOG_COORD_ARRAY);
                glFogCoordPointer(GL_FLOAT,0,*rqGeometry.FogCoords);
            }
        }

        // vertices
        glEnableClientState(GL_VERTEX_ARRAY);
        if ( rqGeometry.Vertices2 )
            glVertexPointer(2,GL_FLOAT,0,*rqGeometry.Vertices2);
        else if ( rqGeometry.Vertices3 )
            glVertexPointer(3,GL_FLOAT,0,*rqGeometry.Vertices3);
        else if ( rqGeometry.Vertices4 )
            glVertexPointer(4,GL_FLOAT,0,*rqGeometry.Vertices4);

        unsigned int uiEnum;
        switch ( rqGeometry.Geometry )
        {
        case HRGeometryf::GT_INDEXED_POINTS:
            uiEnum = GL_POINTS;
            break;
        case HRGeometryf::GT_INDEXED_LINES_SEPARATE:
            uiEnum = GL_LINES;
            break;
        case HRGeometryf::GT_INDEXED_LINES_STRIP:
            uiEnum = GL_LINE_STRIP;
            break;
        case HRGeometryf::GT_INDEXED_LINES_LOOP:
            uiEnum = GL_LINE_LOOP;
            break;
        case HRGeometryf::GT_INDEXED_TRIANGLES_SEPARATE:
            uiEnum = GL_TRIANGLES;
            break;
        case HRGeometryf::GT_INDEXED_TRIANGLES_STRIP:
            uiEnum = GL_TRIANGLE_STRIP;
            break;
        case HRGeometryf::GT_INDEXED_TRIANGLES_FAN:
            uiEnum = GL_TRIANGLE_FAN;
            break;
        case HRGeometryf::GT_INDEXED_QUADS_SEPARATE:
            uiEnum = GL_QUADS;
            break;
        case HRGeometryf::GT_INDEXED_QUADS_STRIP:
            uiEnum = GL_QUAD_STRIP;
            break;
        case HRGeometryf::GT_INDEXED_POLYGONS:
            uiEnum = GL_POLYGON;
            break;
        }

        // lone texture state
        ActivateTextureState(rqGeometry);

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqGeometry.BufferId != 0 )
                glDeleteLists(rqGeometry.BufferId,1);

            rqGeometry.BufferId = glGenLists(1);
            if ( rqGeometry.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqGeometry.NeedRebuffer = false;
            glNewList(rqGeometry.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // transformation
        bool bIdentity = true;
        if ( rqGeometry.GetTransformationMatrix() != Matrix4f::IDENTITY )
        {
            bIdentity = false;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixf(rqGeometry.GetTransformationMatrix());
        }

        // normalizing
        bool bUnitScale = true;
        if ( rqGeometry.GetScale().X() != 1.0f
        ||   rqGeometry.GetScale().Y() != 1.0f
        ||   rqGeometry.GetScale().Z() != 1.0f )
        {
            bUnitScale = false;
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glEnable(GL_RESCALE_NORMAL);
            else
                glEnable(GL_NORMALIZE);
        }

        // embody bound states
        ActivateStates(rqGeometry,m_qPushedStatesLocal);

        glDrawElements(uiEnum,rqGeometry.Index->GetQuantity(),GL_UNSIGNED_INT,*rqGeometry.Index);

        // draw bound states back
        DeactivateStates(m_qPushedStatesLocal);

        // normalizing
        if ( !bUnitScale )
        {
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glDisable(GL_RESCALE_NORMAL);
            else
                glDisable(GL_NORMALIZE);
        }

        // transformation
        if ( !bIdentity )
        {
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();

        // draw texture state back
        DeactivateTextureState();

        // Disable client states.

        // colors
        if ( rqGeometry.RgbColors )
            glDisableClientState(GL_COLOR_ARRAY);
        else if ( rqGeometry.RgbaColors )
            glDisableClientState(GL_COLOR_ARRAY);

        // secondary colors
        if ( rqGeometry.SecondaryColors )
        {
            if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
                glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        }

        // normals
        if ( rqGeometry.Normals )
            glDisableClientState(GL_NORMAL_ARRAY);

        // texture coordinates
        if ( rqGeometry.TexCoords1 )
        {
            iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords1)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords2 )
        {
            iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords2)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords3 )
        {
            iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords3)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords4 )
        {
            iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords4)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }

        // edge flags
        if ( rqGeometry.EdgeFlags )
            glDisableClientState(GL_EDGE_FLAG_ARRAY);

        // fog coordinates
        if ( rqGeometry.FogCoords )
        {
            if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
                glDisableClientState(GL_FOG_COORD_ARRAY);
        }

        // vertices
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawGeometry (const HRGeometrydPtr& rspqGeometry, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRGeometryd& rqGeometry = *rspqGeometry;

    // test bound-frustum intersection
    if ( !(rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER && rqGeometry.NeedRebuffer) )
    {
        // it isn't buffering stage
        assert( m_pqCurrentContext->Camerad );
        IntrBox3Frustum3d qBoundsIntr(rqGeometry.GetGlobalBoundingBox(),m_pqCurrentContext->Camerad->GetFrustum());
        qBoundsIntr.ImpartBoundingBox1(m_pqCurrentContext->Camerad->GetFrustumAABoundingBox());
        if ( !qBoundsIntr.Test() )
            return;
    }

    if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER && !rqGeometry.NeedRebuffer
    &&   rqGeometry.BufferId != 0 )
    {
        // lone texture state
        ActivateTextureState(rqGeometry);

        // call the previously created display list
        glCallList(rqGeometry.BufferId);

        // draw texture state back
        DeactivateTextureState();

        return;
    }

    assert( rqGeometry.Vertices2 || rqGeometry.Vertices3 || rqGeometry.Vertices4 );

    // only one vertex dimension must be present
    assert( (rqGeometry.Vertices2 && !rqGeometry.Vertices3 && !rqGeometry.Vertices4)
        ||  (!rqGeometry.Vertices2 && rqGeometry.Vertices3 && !rqGeometry.Vertices4)
        ||  (!rqGeometry.Vertices2 && !rqGeometry.Vertices3 && rqGeometry.Vertices4) );

    // none or only one color array
    assert( (!rqGeometry.RgbColors && !rqGeometry.RgbaColors)
        ||  (rqGeometry.RgbColors && !rqGeometry.RgbaColors)
        ||  (!rqGeometry.RgbColors && rqGeometry.RgbaColors) );

    // none or only one texture coordinate array of some dimension
    assert( (!rqGeometry.TexCoords1 && !rqGeometry.TexCoords2
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords1 && !rqGeometry.TexCoords2
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords2 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords3 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords3 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords2 && !rqGeometry.TexCoords4)
        ||  (rqGeometry.TexCoords4 && !rqGeometry.TexCoords1
        &&  !rqGeometry.TexCoords2 && !rqGeometry.TexCoords3) );

    // edge flags, if specified, are only for separate triangles, quads, and polygons
    assert( !(rqGeometry.EdgeFlags
        &&  (rqGeometry.Geometry != HRGeometryd::GT_TRIANGLES_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryd::GT_QUADS_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryd::GT_POLYGONS
        &&  rqGeometry.Geometry != HRGeometryd::GT_INDEXED_TRIANGLES_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryd::GT_INDEXED_QUADS_SEPARATE
        &&  rqGeometry.Geometry != HRGeometryd::GT_INDEXED_POLYGONS)) );

    // draw
    int iQuantity1;
    if ( rqGeometry.Geometry == HRGeometryd::GT_POINTS
    ||   rqGeometry.Geometry == HRGeometryd::GT_LINES_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryd::GT_LINES_STRIP
    ||   rqGeometry.Geometry == HRGeometryd::GT_LINES_LOOP
    ||   rqGeometry.Geometry == HRGeometryd::GT_TRIANGLES_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryd::GT_TRIANGLES_STRIP
    ||   rqGeometry.Geometry == HRGeometryd::GT_TRIANGLES_FAN
    ||   rqGeometry.Geometry == HRGeometryd::GT_QUADS_SEPARATE
    ||   rqGeometry.Geometry == HRGeometryd::GT_QUADS_STRIP
    ||   rqGeometry.Geometry == HRGeometryd::GT_POLYGONS )
    {
        // not indexed geometry

        // lone texture state
        ActivateTextureState(rqGeometry);

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqGeometry.BufferId != 0 )
                glDeleteLists(rqGeometry.BufferId,1);

            rqGeometry.BufferId = glGenLists(1);
            if ( rqGeometry.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqGeometry.NeedRebuffer = false;
            glNewList(rqGeometry.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // transformation
        bool bIdentity = true;
        if ( rqGeometry.GetTransformationMatrix() != Matrix4d::IDENTITY )
        {
            bIdentity = false;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixd(rqGeometry.GetTransformationMatrix());
        }

        // normalizing
        bool bUnitScale = true;
        if ( rqGeometry.GetScale().X() != 1.0
        ||   rqGeometry.GetScale().Y() != 1.0
        ||   rqGeometry.GetScale().Z() != 1.0 )
        {
            bUnitScale = false;
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glEnable(GL_RESCALE_NORMAL);
            else
                glEnable(GL_NORMALIZE);
        }

        // embody bound states
        ActivateStates(rqGeometry,m_qPushedStatesLocal);

        switch ( rqGeometry.Geometry )
        {
        case HRGeometryd::GT_POINTS:
            glBegin(GL_POINTS);
            break;
        case HRGeometryd::GT_LINES_SEPARATE:
            glBegin(GL_LINES);
            break;
        case HRGeometryd::GT_LINES_STRIP:
            glBegin(GL_LINE_STRIP);
            break;
        case HRGeometryd::GT_LINES_LOOP:
            glBegin(GL_LINE_LOOP);
            break;
        case HRGeometryd::GT_TRIANGLES_SEPARATE:
            glBegin(GL_TRIANGLES);
            break;
        case HRGeometryd::GT_TRIANGLES_STRIP:
            glBegin(GL_TRIANGLE_STRIP);
            break;
        case HRGeometryd::GT_TRIANGLES_FAN:
            glBegin(GL_TRIANGLE_FAN);
            break;
        case HRGeometryd::GT_QUADS_SEPARATE:
            glBegin(GL_QUADS);
            break;
        case HRGeometryd::GT_QUADS_STRIP:
            glBegin(GL_QUAD_STRIP);
            break;
        case HRGeometryd::GT_POLYGONS:
            glBegin(GL_POLYGON);
            break;
        }

        HRMaterial* pqMaterial;
        unsigned int uiFaceEnum;

        int iQuantity2;
        if ( rqGeometry.Vertices2 )
            iQuantity2 = rqGeometry.Vertices2->GetQuantity();
        else if ( rqGeometry.Vertices3 )
            iQuantity2 = rqGeometry.Vertices3->GetQuantity();
        else if ( rqGeometry.Vertices4 )
            iQuantity2 = rqGeometry.Vertices4->GetQuantity();
        for (int i0 = 0; i0 < iQuantity2; i0++)
        {
            // color
            if ( rqGeometry.RgbColors )
                glColor3fv((*rqGeometry.RgbColors)[i0]);
            else if ( rqGeometry.RgbaColors )
                glColor4fv((*rqGeometry.RgbaColors)[i0]);

            // secondary color
            if ( rqGeometry.SecondaryColors )
            {
                if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
                    glSecondaryColor3fv((*rqGeometry.SecondaryColors)[i0]);
            }

            // normal
            if ( rqGeometry.Normals )
                glNormal3dv((*rqGeometry.Normals)[i0]);

            // material
            if ( rqGeometry.Materials )
            {
                pqMaterial = &(*(*rqGeometry.Materials)[i0]);
                switch ( pqMaterial->Face )
                {
                case HRMaterial::FT_FRONT:
                    uiFaceEnum = GL_FRONT;
                    break;
                case HRMaterial::FT_BACK:
                    uiFaceEnum = GL_BACK;
                    break;
                case HRMaterial::FT_BOTH:
                    uiFaceEnum = GL_FRONT_AND_BACK;
                    break;
                }
                glMaterialfv(uiFaceEnum,GL_AMBIENT,pqMaterial->Ambient);
                glMaterialfv(uiFaceEnum,GL_DIFFUSE,pqMaterial->Diffuse);
                glMaterialfv(uiFaceEnum,GL_SPECULAR,pqMaterial->Specular);
                glMaterialfv(uiFaceEnum,GL_EMISSION,pqMaterial->Emission);
                glMaterialf(uiFaceEnum,GL_SHININESS,pqMaterial->Shininess);
            }

            // texture coordinates
            if ( rqGeometry.TexCoords1 )
            {
                iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord1d((*(*rqGeometry.TexCoords1)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords1)[i1] )
                            glMultiTexCoord1d(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords1)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords2 )
            {
                iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord2dv((*(*rqGeometry.TexCoords2)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords2)[i1] )
                            glMultiTexCoord2dv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords2)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords3 )
            {
                iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord3dv((*(*rqGeometry.TexCoords3)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords3)[i1] )
                            glMultiTexCoord3dv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords3)[i1])[i0]);
                    }
                }
            }
            else if ( rqGeometry.TexCoords4 )
            {
                iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
                if ( iQuantity1 == 1 )
                {
                    glTexCoord4dv((*(*rqGeometry.TexCoords4)[0])[i0]);
                }
                else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
                {
                    for (int i1 = 0; i1 < iQuantity1 && i1 < m_iMaxTextureCoords; i1++)
                    {
                        if ( (*rqGeometry.TexCoords4)[i1] )
                            glMultiTexCoord4dv(GL_TEXTURE0+i1,(*(*rqGeometry.TexCoords4)[i1])[i0]);
                    }
                }
            }

            // edge flag
            if ( rqGeometry.EdgeFlags )
                glEdgeFlag((*rqGeometry.EdgeFlags)[i0]);

            // fog coordinate
            if ( rqGeometry.FogCoords )
            {
                if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
                    glFogCoordd((*rqGeometry.FogCoords)[i0]);
            }

            // vertex
            if ( rqGeometry.Vertices2 )
                glVertex2dv((*rqGeometry.Vertices2)[i0]);
            else if ( rqGeometry.Vertices3 )
                glVertex3dv((*rqGeometry.Vertices3)[i0]);
            else if ( rqGeometry.Vertices4 )
                glVertex4dv((*rqGeometry.Vertices4)[i0]);
        }

        glEnd();

        // draw bound states back
        DeactivateStates(m_qPushedStatesLocal);

        // normalizing
        if ( !bUnitScale )
        {
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glDisable(GL_RESCALE_NORMAL);
            else
                glDisable(GL_NORMALIZE);
        }

        // transformation
        if ( !bIdentity )
        {
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();

        // draw texture state back
        DeactivateTextureState();
    }
    else
    {
        // indexed geometry

        // materials per vertex cannot be specified for indexed geometry
        assert( !rqGeometry.Materials );

        // Enable client states and set pointers.

        // colors
        if ( rqGeometry.RgbColors )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3,GL_FLOAT,0,*rqGeometry.RgbColors);
        }
        else if ( rqGeometry.RgbaColors )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4,GL_FLOAT,0,*rqGeometry.RgbaColors);
        }

        // secondary colors
        if ( rqGeometry.SecondaryColors )
        {
            if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
            {
                glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
                glSecondaryColorPointer(3,GL_FLOAT,0,*rqGeometry.SecondaryColors);
            }
        }

        // normals
        if ( rqGeometry.Normals )
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_DOUBLE,0,*rqGeometry.Normals);
        }

        // texture coordinates
        if ( rqGeometry.TexCoords1 )
        {
            iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(1,GL_DOUBLE,0,*((*rqGeometry.TexCoords1)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords1)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(1,GL_DOUBLE,0,*((*rqGeometry.TexCoords1)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords2 )
        {
            iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2,GL_DOUBLE,0,*((*rqGeometry.TexCoords2)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords2)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(2,GL_DOUBLE,0,*((*rqGeometry.TexCoords2)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords3 )
        {
            iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(3,GL_DOUBLE,0,*((*rqGeometry.TexCoords3)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords3)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(3,GL_DOUBLE,0,*((*rqGeometry.TexCoords3)[i]));
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords4 )
        {
            iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(4,GL_DOUBLE,0,*((*rqGeometry.TexCoords4)[0]));
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords4)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glTexCoordPointer(4,GL_DOUBLE,0,*((*rqGeometry.TexCoords4)[i]));
                    }
                }
            }
        }

        // edge flags
        if ( rqGeometry.EdgeFlags )
        {
            glEnableClientState(GL_EDGE_FLAG_ARRAY);
            glEdgeFlagPointer(0,*rqGeometry.EdgeFlags);
        }

        // fog coordinates
        if ( rqGeometry.FogCoords )
        {
            if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
            {
                glEnableClientState(GL_FOG_COORD_ARRAY);
                glFogCoordPointer(GL_DOUBLE,0,*rqGeometry.FogCoords);
            }
        }

        // vertices
        glEnableClientState(GL_VERTEX_ARRAY);
        if ( rqGeometry.Vertices2 )
            glVertexPointer(2,GL_DOUBLE,0,*rqGeometry.Vertices2);
        else if ( rqGeometry.Vertices3 )
            glVertexPointer(3,GL_DOUBLE,0,*rqGeometry.Vertices3);
        else if ( rqGeometry.Vertices4 )
            glVertexPointer(4,GL_DOUBLE,0,*rqGeometry.Vertices4);

        unsigned int uiEnum;
        switch ( rqGeometry.Geometry )
        {
        case HRGeometryd::GT_INDEXED_POINTS:
            uiEnum = GL_POINTS;
            break;
        case HRGeometryd::GT_INDEXED_LINES_SEPARATE:
            uiEnum = GL_LINES;
            break;
        case HRGeometryd::GT_INDEXED_LINES_STRIP:
            uiEnum = GL_LINE_STRIP;
            break;
        case HRGeometryd::GT_INDEXED_LINES_LOOP:
            uiEnum = GL_LINE_LOOP;
            break;
        case HRGeometryd::GT_INDEXED_TRIANGLES_SEPARATE:
            uiEnum = GL_TRIANGLES;
            break;
        case HRGeometryd::GT_INDEXED_TRIANGLES_STRIP:
            uiEnum = GL_TRIANGLE_STRIP;
            break;
        case HRGeometryd::GT_INDEXED_TRIANGLES_FAN:
            uiEnum = GL_TRIANGLE_FAN;
            break;
        case HRGeometryd::GT_INDEXED_QUADS_SEPARATE:
            uiEnum = GL_QUADS;
            break;
        case HRGeometryd::GT_INDEXED_QUADS_STRIP:
            uiEnum = GL_QUAD_STRIP;
            break;
        case HRGeometryd::GT_INDEXED_POLYGONS:
            uiEnum = GL_POLYGON;
            break;
        }

        // lone texture state
        ActivateTextureState(rqGeometry);

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqGeometry.BufferId != 0 )
                glDeleteLists(rqGeometry.BufferId,1);

            rqGeometry.BufferId = glGenLists(1);
            if ( rqGeometry.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqGeometry.NeedRebuffer = false;
            glNewList(rqGeometry.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // transformation
        bool bIdentity = true;
        if ( rqGeometry.GetTransformationMatrix() != Matrix4d::IDENTITY )
        {
            bIdentity = false;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixd(rqGeometry.GetTransformationMatrix());
        }

        // normalizing
        bool bUnitScale = true;
        if ( rqGeometry.GetScale().X() != 1.0
        ||   rqGeometry.GetScale().Y() != 1.0
        ||   rqGeometry.GetScale().Z() != 1.0 )
        {
            bUnitScale = false;
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glEnable(GL_RESCALE_NORMAL);
            else
                glEnable(GL_NORMALIZE);
        }

        // embody bound states
        ActivateStates(rqGeometry,m_qPushedStatesLocal);

        glDrawElements(uiEnum,rqGeometry.Index->GetQuantity(),GL_UNSIGNED_INT,*rqGeometry.Index);

        // draw bound states back
        DeactivateStates(m_qPushedStatesLocal);

        // normalizing
        if ( !bUnitScale )
        {
            if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
                glDisable(GL_RESCALE_NORMAL);
            else
                glDisable(GL_NORMALIZE);
        }

        // transformation
        if ( !bIdentity )
        {
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        if ( rqGeometry.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();

        // draw texture state back
        DeactivateTextureState();

        // Disable client states.

        // colors
        if ( rqGeometry.RgbColors )
            glDisableClientState(GL_COLOR_ARRAY);
        else if ( rqGeometry.RgbaColors )
            glDisableClientState(GL_COLOR_ARRAY);

        // secondary colors
        if ( rqGeometry.SecondaryColors )
        {
            if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
                glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        }

        // normals
        if ( rqGeometry.Normals )
            glDisableClientState(GL_NORMAL_ARRAY);

        // texture coordinates
        if ( rqGeometry.TexCoords1 )
        {
            iQuantity1 = rqGeometry.TexCoords1->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords1)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords2 )
        {
            iQuantity1 = rqGeometry.TexCoords2->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords2)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords3 )
        {
            iQuantity1 = rqGeometry.TexCoords3->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords3)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }
        else if ( rqGeometry.TexCoords4 )
        {
            iQuantity1 = rqGeometry.TexCoords4->GetQuantity();
            if ( iQuantity1 == 1 )
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS )
            {
                for (int i = 0; i < iQuantity1 && i < m_iMaxTextureCoords; i++)
                {
                    if ( (*rqGeometry.TexCoords4)[i] )
                    {
                        glClientActiveTexture(GL_TEXTURE0+i);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }
                }
            }
        }

        // edge flags
        if ( rqGeometry.EdgeFlags )
            glDisableClientState(GL_EDGE_FLAG_ARRAY);

        // fog coordinates
        if ( rqGeometry.FogCoords )
        {
            if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
                glDisableClientState(GL_FOG_COORD_ARRAY);
        }

        // vertices
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawImage (const HRImagefPtr& rspqImage, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRImagef& rqImage = *rspqImage;

    if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER && !rqImage.NeedRebuffer && rqImage.BufferId != 0 )
    {
        // call the previously created display list
        glCallList(rqImage.BufferId);
    }
    else
    {
        if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqImage.BufferId != 0 )
                glDeleteLists(rqImage.BufferId,1);

            rqImage.BufferId = glGenLists(1);
            if ( rqImage.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqImage.NeedRebuffer = false;
            glNewList(rqImage.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // target buffer
        if ( rqImage.TargetBufferSet )
        {
            unsigned int uiTrgtEnum;
            switch ( rqImage.TargetBuffer )
            {
            case HRImagef::TBT_FRONT:
                uiTrgtEnum = GL_FRONT;
                break;
            case HRImagef::TBT_BACK:
                uiTrgtEnum = GL_BACK;
                break;
            case HRImagef::TBT_LEFT:
                uiTrgtEnum = GL_LEFT;
                break;
            case HRImagef::TBT_RIGHT:
                uiTrgtEnum = GL_RIGHT;
                break;
            case HRImagef::TBT_FRONT_LEFT:
                uiTrgtEnum = GL_FRONT_LEFT;
                break;
            case HRImagef::TBT_FRONT_RIGHT:
                uiTrgtEnum = GL_FRONT_RIGHT;
                break;
            case HRImagef::TBT_BACK_LEFT:
                uiTrgtEnum = GL_BACK_LEFT;
                break;
            case HRImagef::TBT_BACK_RIGHT:
                uiTrgtEnum = GL_BACK_RIGHT;
                break;
            case HRImagef::TBT_FRONT_AND_BACK:
                uiTrgtEnum = GL_FRONT_AND_BACK;
                break;
            }
            glDrawBuffer(uiTrgtEnum);
        }

        // position
        if ( !rqImage.DrawInWindowCoordinates )
        {
            glRasterPos4fv(rqImage.Position);
        }
        else
        {
            if ( QGLExists_GL_ARB_window_pos == QGL_EXT_FULLY_EXISTS )
            {
                glWindowPos3f(
                    rqImage.Position.X(),
                    rqImage.Position.Y(),
                    rqImage.Position.Z());
            }
        }

        // format
        unsigned int uiFrmtEnum;
        if ( !rqImage.FormatSet )
        {
            // autodetect
            uiFrmtEnum = GetImageFormat(rqImage.Image->GetType());
        }
        else
        {
            switch ( rqImage.Format )
            {
            case HRImagef::FT_STENCIL_INDEX:
                uiFrmtEnum = GL_STENCIL_INDEX;
                break;
            case HRImagef::FT_DEPTH_COMPONENT:
                uiFrmtEnum = GL_DEPTH_COMPONENT;
                break;
            case HRImagef::FT_RED:
                uiFrmtEnum = GL_RED;
                break;
            case HRImagef::FT_GREEN:
                uiFrmtEnum = GL_GREEN;
                break;
            case HRImagef::FT_BLUE:
                uiFrmtEnum = GL_BLUE;
                break;
            case HRImagef::FT_ALPHA:
                uiFrmtEnum = GL_ALPHA;
                break;
            case HRImagef::FT_LUMINANCE:
                uiFrmtEnum = GL_LUMINANCE;
                break;
            }
        }

        // type
        unsigned int uiTypeEnum = GetImageType(rqImage.Image->GetType());

        if ( uiFrmtEnum != 0 && uiTypeEnum != 0 )
            glDrawPixels(rqImage.Image->GetWidth(),rqImage.Image->GetHeight(),uiFrmtEnum,uiTypeEnum,*rqImage.Image);

        if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawImage (const HRImagedPtr& rspqImage, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRImaged& rqImage = *rspqImage;

    if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER && !rqImage.NeedRebuffer && rqImage.BufferId != 0 )
    {
        // call the previously created display list
        glCallList(rqImage.BufferId);
    }
    else
    {
        if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the display list needs to be created or recreated
            if ( rqImage.BufferId != 0 )
                glDeleteLists(rqImage.BufferId,1);

            rqImage.BufferId = glGenLists(1);
            if ( rqImage.BufferId == 0 )
            {
                LogError("Cannot create display list");
                return;
            }
            rqImage.NeedRebuffer = false;
            glNewList(rqImage.BufferId,( bBufferOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE ));
        }

        // target buffer
        if ( rqImage.TargetBufferSet )
        {
            unsigned int uiTrgtEnum;
            switch ( rqImage.TargetBuffer )
            {
            case HRImaged::TBT_FRONT:
                uiTrgtEnum = GL_FRONT;
                break;
            case HRImaged::TBT_BACK:
                uiTrgtEnum = GL_BACK;
                break;
            case HRImaged::TBT_LEFT:
                uiTrgtEnum = GL_LEFT;
                break;
            case HRImaged::TBT_RIGHT:
                uiTrgtEnum = GL_RIGHT;
                break;
            case HRImaged::TBT_FRONT_LEFT:
                uiTrgtEnum = GL_FRONT_LEFT;
                break;
            case HRImaged::TBT_FRONT_RIGHT:
                uiTrgtEnum = GL_FRONT_RIGHT;
                break;
            case HRImaged::TBT_BACK_LEFT:
                uiTrgtEnum = GL_BACK_LEFT;
                break;
            case HRImaged::TBT_BACK_RIGHT:
                uiTrgtEnum = GL_BACK_RIGHT;
                break;
            case HRImaged::TBT_FRONT_AND_BACK:
                uiTrgtEnum = GL_FRONT_AND_BACK;
                break;
            }
            glDrawBuffer(uiTrgtEnum);
        }

        // position
        if ( !rqImage.DrawInWindowCoordinates )
        {
            glRasterPos4dv(rqImage.Position);
        }
        else
        {
            if ( QGLExists_GL_ARB_window_pos == QGL_EXT_FULLY_EXISTS )
            {
                glWindowPos3d(
                    rqImage.Position.X(),
                    rqImage.Position.Y(),
                    rqImage.Position.Z());
            }
        }

        // format
        unsigned int uiFrmtEnum;
        if ( !rqImage.FormatSet )
        {
            // autodetect
            uiFrmtEnum = GetImageFormat(rqImage.Image->GetType());
        }
        else
        {
            switch ( rqImage.Format )
            {
            case HRImaged::FT_STENCIL_INDEX:
                uiFrmtEnum = GL_STENCIL_INDEX;
                break;
            case HRImaged::FT_DEPTH_COMPONENT:
                uiFrmtEnum = GL_DEPTH_COMPONENT;
                break;
            case HRImaged::FT_RED:
                uiFrmtEnum = GL_RED;
                break;
            case HRImaged::FT_GREEN:
                uiFrmtEnum = GL_GREEN;
                break;
            case HRImaged::FT_BLUE:
                uiFrmtEnum = GL_BLUE;
                break;
            case HRImaged::FT_ALPHA:
                uiFrmtEnum = GL_ALPHA;
                break;
            case HRImaged::FT_LUMINANCE:
                uiFrmtEnum = GL_LUMINANCE;
                break;
            }
        }

        // type
        unsigned int uiTypeEnum = GetImageType(rqImage.Image->GetType());

        if ( uiFrmtEnum != 0 && uiTypeEnum != 0 )
            glDrawPixels(rqImage.Image->GetWidth(),rqImage.Image->GetHeight(),uiFrmtEnum,uiTypeEnum,*rqImage.Image);

        if ( rqImage.GetBufferProfile() == HRBufferable::PT_BUFFER )
            glEndList();
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawScene (bool bBufferOnly)
{
    DrawNode(m_qRootNodef,bBufferOnly);
    DrawNode(m_qRootNoded,bBufferOnly);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::CheckForErrors ()
{
    switch ( glGetError() )
    {
    case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        assert( false );
        LogError("OpenGL error 'GL_INVALID_ENUM' detected");
        break;
    case GL_INVALID_VALUE:
        assert( false );
        LogError("OpenGL error 'GL_INVALID_VALUE' detected");
        break;
    case GL_INVALID_OPERATION:
        assert( false );
        LogError("OpenGL error 'GL_INVALID_OPERATION' detected");
        break;
    case GL_STACK_OVERFLOW:
        assert( false );
        LogError("OpenGL error 'GL_STACK_OVERFLOW' detected");
        break;
    case GL_STACK_UNDERFLOW:
        assert( false );
        LogError("OpenGL error 'GL_STACK_UNDERFLOW' detected");
        break;
    case GL_OUT_OF_MEMORY:
        assert( false );
        LogError("OpenGL error 'GL_OUT_OF_MEMORY' detected");
        break;
    case GL_TABLE_TOO_LARGE:
        assert( false );
        LogError("OpenGL error 'GL_TABLE_TOO_LARGE' detected");
        break;
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::SwapBuffers ()
{
    if ( m_qPresentFramebufferParams.Buffering != FramebufferParams::BT_BUFFERING_DOUBLE )
    {
        LogAndThrowFatalError("Cannot swap buffers");
        return;
    }

#ifdef _WIN32
    if ( !::SwapBuffers(m_pqCurrentContext->DC) )
        LogAndThrowFatalError("Cannot swap buffers");
#endif
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawNode (const HRNodefPtr& rspqNode, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRNodef& rqNode = *rspqNode;

    // transformation
    bool bIdentity = true;
    if ( rqNode.GetTransformationMatrix() != Matrix4f::IDENTITY )
    {
        bIdentity = false;
        PushModelViewMatrixAndMultf(rqNode.GetTransformationMatrix());
    }

    // normalizing
    bool bUnitScale = true;
    if ( rqNode.GetScale().X() != 1.0f
    ||   rqNode.GetScale().Y() != 1.0f
    ||   rqNode.GetScale().Z() != 1.0f )
    {
        bUnitScale = false;

        // there is no state related to normalizing somehow, so can use this
        if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
            glEnable(GL_RESCALE_NORMAL);
        else
            glEnable(GL_NORMALIZE);
    }

    // draw bound geometry
    for (int i = 0; i < rqNode.GetGeometryQuantity(); i++)
        DrawGeometry(rqNode.GetGeometry(i),bBufferOnly);

    // recur attached nodes
    for (int i = 0; i < rqNode.GetNodeQuantity(); i++)
        DrawNode(rqNode.GetNode(i),bBufferOnly);

    // normalizing
    if ( !bUnitScale )
    {
        // there is no state related to normalizing somehow, so can use this
        if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
            glDisable(GL_RESCALE_NORMAL);
        else
            glDisable(GL_NORMALIZE);
    }

    // transformation
    if ( !bIdentity )
        PopModelViewMatrixf();
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DrawNode (const HRNodedPtr& rspqNode, bool bBufferOnly)
{
    assert( m_bInitialized );

    HRNoded& rqNode = *rspqNode;

    // transformation
    bool bIdentity = true;
    if ( rqNode.GetTransformationMatrix() != Matrix4d::IDENTITY )
    {
        bIdentity = false;
        PushModelViewMatrixAndMultd(rqNode.GetTransformationMatrix());
    }

    // normalizing
    bool bUnitScale = true;
    if ( rqNode.GetScale().X() != 1.0
    ||   rqNode.GetScale().Y() != 1.0
    ||   rqNode.GetScale().Z() != 1.0 )
    {
        bUnitScale = false;

        // there is no state related to normalizing somehow, so can use this
        if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
            glEnable(GL_RESCALE_NORMAL);
        else
            glEnable(GL_NORMALIZE);
    }

    // draw bound geometry
    for (int i = 0; i < rqNode.GetGeometryQuantity(); i++)
        DrawGeometry(rqNode.GetGeometry(i),bBufferOnly);

    // recur attached nodes
    for (int i = 0; i < rqNode.GetNodeQuantity(); i++)
        DrawNode(rqNode.GetNode(i),bBufferOnly);

    // normalizing
    if ( !bUnitScale )
    {
        // there is no state related to normalizing somehow, so can use this
        if ( QGLExists_GL_EXT_rescale_normal == QGL_EXT_FULLY_EXISTS )
            glDisable(GL_RESCALE_NORMAL);
        else
            glDisable(GL_NORMALIZE);
    }

    // transformation
    if ( !bIdentity )
        PopModelViewMatrixd();
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::PushModelViewMatrixAndMultf (const Matrix4f& rqMatrix)
{
    glMatrixMode(GL_MODELVIEW);
    if ( m_iPushedModelViewMatrixQuantity < m_iMaxModelViewStackSize )
    {
        // using the first echelon
        glPushMatrix();
        glMultMatrixf(rqMatrix);
    }
    else
    {
        // using the second echelon
        if ( m_iPushedModelViewMatrixQuantity == m_iMaxModelViewStackSize )
        {
            // cutting edge
            glGetFloatv(GL_MODELVIEW_MATRIX,m_qSecondEchelonModelViewStackf.GetLast());
        }
        glMultMatrixf(rqMatrix);
        m_qSecondEchelonModelViewStackf.AdvanceQuantity(1);
        glGetFloatv(GL_MODELVIEW_MATRIX,m_qSecondEchelonModelViewStackf.GetLast());
    }
    m_iPushedModelViewMatrixQuantity++;
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::PopModelViewMatrixf ()
{
    glMatrixMode(GL_MODELVIEW);
    if ( m_iPushedModelViewMatrixQuantity <= m_iMaxModelViewStackSize )
    {
        // using the first echelon
        glPopMatrix();
    }
    else
    {
        // using the second echelon
        m_qSecondEchelonModelViewStackf.Pop();
        glLoadMatrixf(m_qSecondEchelonModelViewStackf.GetLast());
    }
    m_iPushedModelViewMatrixQuantity--;
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::PushModelViewMatrixAndMultd (const Matrix4d& rqMatrix)
{
    glMatrixMode(GL_MODELVIEW);
    if ( m_iPushedModelViewMatrixQuantity < m_iMaxModelViewStackSize )
    {
        // using the first echelon
        glPushMatrix();
        glMultMatrixd(rqMatrix);
    }
    else
    {
        // using the second echelon
        if ( m_iPushedModelViewMatrixQuantity == m_iMaxModelViewStackSize )
        {
            // cutting edge
            glGetDoublev(GL_MODELVIEW_MATRIX,m_qSecondEchelonModelViewStackd.GetLast());
        }
        glMultMatrixd(rqMatrix);
        m_qSecondEchelonModelViewStackd.AdvanceQuantity(1);
        glGetDoublev(GL_MODELVIEW_MATRIX,m_qSecondEchelonModelViewStackd.GetLast());
    }
    m_iPushedModelViewMatrixQuantity++;
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::PopModelViewMatrixd ()
{
    glMatrixMode(GL_MODELVIEW);
    if ( m_iPushedModelViewMatrixQuantity <= m_iMaxModelViewStackSize )
    {
        // using the first echelon
        glPopMatrix();
    }
    else
    {
        // using the second echelon
        m_qSecondEchelonModelViewStackd.Pop();
        glLoadMatrixd(m_qSecondEchelonModelViewStackd.GetLast());
    }
    m_iPushedModelViewMatrixQuantity--;
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::ActivateTextureState (const StateSensitive& rqStateSensitive)
{
    if ( rqStateSensitive.GetTextureStatef() )
    {
        if ( rqStateSensitive.GetTextureStatef()->On )
        {
            assert( m_qPushedStatesGlobal.Quantity != m_iMaxAttribStackSize );

            glPushAttrib(GL_TEXTURE_BIT);
            m_qPushedStatesLocal.Quantity++;

            glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
            glGetIntegerv(GL_PERSPECTIVE_CORRECTION_HINT,&m_iPrevTexCorrHint);
            if ( QGLExists_GL_SGIS_generate_mipmap == QGL_EXT_FULLY_EXISTS )
                glGetIntegerv(GL_GENERATE_MIPMAP_HINT,&m_iPrevMMGenHint);
            if ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS )
            {
                m_bPrevPointSpriteEnabled = ( glIsEnabled(GL_POINT_SPRITE) ? true : false );
                glGetIntegerv(GL_POINT_SPRITE_COORD_ORIGIN,&m_iPrevPointSpriteCoordOrigin);
            }

            m_bTextureStateActivated = true;
            StateTexture(rqStateSensitive.GetTextureStatef());
        }
    }
    else if ( rqStateSensitive.GetTextureStated() )
    {
        if ( rqStateSensitive.GetTextureStated()->On )
        {
            assert( m_qPushedStatesGlobal.Quantity != m_iMaxAttribStackSize );

            glPushAttrib(GL_TEXTURE_BIT);
            m_qPushedStatesLocal.Quantity++;

            glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
            glGetIntegerv(GL_PERSPECTIVE_CORRECTION_HINT,&m_iPrevTexCorrHint);
            if ( QGLExists_GL_SGIS_generate_mipmap == QGL_EXT_FULLY_EXISTS )
                glGetIntegerv(GL_GENERATE_MIPMAP_HINT,&m_iPrevMMGenHint);
            if ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS )
            {
                m_bPrevPointSpriteEnabled = ( glIsEnabled(GL_POINT_SPRITE) ? true : false );
                glGetIntegerv(GL_POINT_SPRITE_COORD_ORIGIN,&m_iPrevPointSpriteCoordOrigin);
            }

            m_bTextureStateActivated = true;
            StateTexture(rqStateSensitive.GetTextureStated());
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DeactivateTextureState ()
{
    if ( m_bTextureStateActivated )
    {
        // popping of a texture attribute group will be done by general global/local state deactivating function
        glPopClientAttrib();  // pixel-store
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,m_iPrevTexCorrHint);
        if ( QGLExists_GL_SGIS_generate_mipmap == QGL_EXT_FULLY_EXISTS )
            glHint(GL_GENERATE_MIPMAP_HINT,m_iPrevMMGenHint);
        if ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS )
        {
            if ( m_bPrevPointSpriteEnabled )
                glEnable(GL_POINT_SPRITE);
            else
                glDisable(GL_POINT_SPRITE);
            glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN,m_iPrevPointSpriteCoordOrigin);
        }

        m_bTexture1dEnabled = false;
        m_bTexture2dEnabled = false;
        m_bTexture3dEnabled = false;
        m_bTextureCubeMapEnabled = false;
        m_bTextureStateActivated = false;
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::ActivateStates (const StateSensitive& rqStateSensitive, PushedStatesRecord& rqPushedStates)
{
    for (int i = 0; i < rqStateSensitive.GetStateQuantity(); i++)
    {
        const SmartRttiPointer& rspqState = rqStateSensitive.GetState(i);

        if ( rspqState->IsExactly(AlphaTestState::Class) )
        {
            AlphaTestState* pqAlphaTestState = StaticCast<AlphaTestState>(rspqState);
            if ( pqAlphaTestState->On )
            {
                if ( !rqPushedStates.ColorBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ColorBufferAttribGroup = true;
                }
                StateAlphaTest(pqAlphaTestState);
            }
        }
        else if ( rspqState->IsExactly(BlendingState::Class) )
        {
            BlendingState* pqBlendingState = StaticCast<BlendingState>(rspqState);
            if ( pqBlendingState->On )
            {
                if ( !rqPushedStates.ColorBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ColorBufferAttribGroup = true;
                }
                StateBlending(pqBlendingState);
            }
        }
        else if ( rspqState->IsExactly(ClippingPlanesState::Class) )
        {
            ClippingPlanesState* pqClippingPlanesState = StaticCast<ClippingPlanesState>(rspqState);
            if ( pqClippingPlanesState->On )
            {
                if ( !rqPushedStates.TransformAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_TRANSFORM_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.TransformAttribGroup = true;
                }
                StateClippingPlanes(pqClippingPlanesState,rqPushedStates.Global);
            }
        }
        else if ( rspqState->IsExactly(ColorMaskState::Class) )
        {
            ColorMaskState* pqColorMaskState = StaticCast<ColorMaskState>(rspqState);
            if ( pqColorMaskState->On )
            {
                if ( !rqPushedStates.ColorBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ColorBufferAttribGroup = true;
                }
                StateColorMask(pqColorMaskState);
            }
        }
        else if ( rspqState->IsExactly(ColorState::Class) )
        {
            ColorState* pqColorState = StaticCast<ColorState>(rspqState);
            if ( pqColorState->On )
            {
                if ( !rqPushedStates.CurrentAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_CURRENT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.CurrentAttribGroup = true;
                }
                StateColor(pqColorState);
            }
        }
        else if ( rspqState->IsExactly(CullFaceState::Class) )
        {
            CullFaceState* pqCullFaceState = StaticCast<CullFaceState>(rspqState);
            if ( pqCullFaceState->On )
            {
                if ( !rqPushedStates.PolygonAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POLYGON_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PolygonAttribGroup = true;
                }
                StateCullFace(pqCullFaceState);
            }
        }
        else if ( rspqState->IsExactly(DepthMaskState::Class) )
        {
            DepthMaskState* pqDepthMaskState = StaticCast<DepthMaskState>(rspqState);
            if ( pqDepthMaskState->On )
            {
                if ( !rqPushedStates.DepthBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_DEPTH_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.DepthBufferAttribGroup = true;
                }
                StateDepthMask(pqDepthMaskState);
            }
        }
        else if ( rspqState->IsExactly(DepthRangeState::Class) )
        {
            DepthRangeState* pqDepthRangeState = StaticCast<DepthRangeState>(rspqState);
            if ( pqDepthRangeState->On )
            {
                if ( !rqPushedStates.ViewportAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_VIEWPORT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ViewportAttribGroup = true;
                }
                StateDepthRange(pqDepthRangeState);
            }
        }
        else if ( rspqState->IsExactly(DepthTestState::Class) )
        {
            DepthTestState* pqDepthTestState = StaticCast<DepthTestState>(rspqState);
            if ( pqDepthTestState->On )
            {
                if ( !rqPushedStates.DepthBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_DEPTH_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.DepthBufferAttribGroup = true;
                }
                StateDepthTest(pqDepthTestState);
            }
        }
        else if ( rspqState->IsExactly(FogState::Class) )
        {
            FogState* pqFogState = StaticCast<FogState>(rspqState);
            if ( pqFogState->On )
            {
                if ( !rqPushedStates.FogAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_FOG_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.FogAttribGroup = true;
                }
                if ( !rqPushedStates.HintAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_HINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.HintAttribGroup = true;
                }
                StateFog(pqFogState);
            }
        }
        else if ( rspqState->IsExactly(FrontFaceState::Class) )
        {
            FrontFaceState* pqFrontFaceState = StaticCast<FrontFaceState>(rspqState);
            if ( pqFrontFaceState->On )
            {
                if ( !rqPushedStates.PolygonAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POLYGON_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PolygonAttribGroup = true;
                }
                StateFrontFace(pqFrontFaceState);
            }
        }
        else if ( rspqState->IsExactly(LightState::Class) )
        {
            LightState* pqLightState = StaticCast<LightState>(rspqState);
            if ( pqLightState->On )
            {
                if ( !rqPushedStates.LightningAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_LIGHTING_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.LightningAttribGroup = true;
                }
                StateLight(pqLightState,rqPushedStates.Global);
            }
        }
        else if ( rspqState->IsExactly(LinesAntialiasingState::Class) )
        {
            LinesAntialiasingState* pqLinesAntialiasingState = StaticCast<LinesAntialiasingState>(rspqState);
            if ( pqLinesAntialiasingState->On )
            {
                if ( !rqPushedStates.LineAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_LINE_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.LineAttribGroup = true;
                }
                if ( !rqPushedStates.HintAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_HINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.HintAttribGroup = true;
                }
                StateLinesAntialiasing(pqLinesAntialiasingState);
            }
        }
        else if ( rspqState->IsExactly(LineStippleState::Class) )
        {
            LineStippleState* pqLineStippleState = StaticCast<LineStippleState>(rspqState);
            if ( pqLineStippleState->On )
            {
                if ( !rqPushedStates.LineAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_LINE_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.LineAttribGroup = true;
                }
                StateLineStipple(pqLineStippleState);
            }
        }
        else if ( rspqState->IsExactly(LineWidthState::Class) )
        {
            LineWidthState* pqLineWidthState = StaticCast<LineWidthState>(rspqState);
            if ( pqLineWidthState->On )
            {
                if ( !rqPushedStates.LineAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_LINE_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.LineAttribGroup = true;
                }
                StateLineWidth(pqLineWidthState);
            }
        }
        else if ( rspqState->IsExactly(LogicOperationState::Class) )
        {
            LogicOperationState* pqLogicOperationState = StaticCast<LogicOperationState>(rspqState);
            if ( pqLogicOperationState->On )
            {
                if ( !rqPushedStates.ColorBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ColorBufferAttribGroup = true;
                }
                StateLogicOperation(pqLogicOperationState);
            }
        }
        else if ( rspqState->IsExactly(MaterialState::Class) )
        {
            MaterialState* pqMaterialState = StaticCast<MaterialState>(rspqState);
            if ( pqMaterialState->On )
            {
                if ( !rqPushedStates.LightningAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_LIGHTING_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.LightningAttribGroup = true;
                }
                StateMaterial(pqMaterialState);
            }
        }
        else if ( rspqState->IsExactly(MultisampleAntialiasingState::Class) )
        {
            if ( QGLExists_GL_ARB_multisample == QGL_EXT_FULLY_EXISTS )
            {
                MultisampleAntialiasingState* pqMultisampleAntialiasingState =
                    StaticCast<MultisampleAntialiasingState>(rspqState);
                if ( pqMultisampleAntialiasingState->On )
                {
                    if ( !rqPushedStates.MultisampleAttribGroup )
                    {
                        assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                            &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity +
                                rqPushedStates.Quantity == m_iMaxAttribStackSize) );
                        glPushAttrib(GL_MULTISAMPLE_BIT);
                        rqPushedStates.Quantity++;
                        rqPushedStates.MultisampleAttribGroup = true;
                    }
                    StateMultisampleAntialiasing(pqMultisampleAntialiasingState);
                }
            }
        }
        else if ( rspqState->IsExactly(NormalStatef::Class) )
        {
            NormalStatef* pqNormalStatef = StaticCast<NormalStatef>(rspqState);
            if ( pqNormalStatef->On )
            {
                if ( !rqPushedStates.CurrentAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_CURRENT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.CurrentAttribGroup = true;
                }
                StateNormal(pqNormalStatef);
            }
        }
        else if ( rspqState->IsExactly(NormalStated::Class) )
        {
            NormalStated* pqNormalStated = StaticCast<NormalStated>(rspqState);
            if ( pqNormalStated->On )
            {
                if ( !rqPushedStates.CurrentAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_CURRENT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.CurrentAttribGroup = true;
                }
                StateNormal(pqNormalStated);
            }
        }
        else if ( rspqState->IsExactly(PixelTransferState::Class) )
        {
            PixelTransferState* pqPixelTransferState = StaticCast<PixelTransferState>(rspqState);
            if ( pqPixelTransferState->On )
            {
                if ( !rqPushedStates.PixelAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_PIXEL_MODE_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PixelAttribGroup = true;
                }
                StatePixelTransfer(pqPixelTransferState);
            }
        }
        else if ( rspqState->IsExactly(PointParameterState::Class) )
        {
            PointParameterState* pqPointParameterState = StaticCast<PointParameterState>(rspqState);
            if ( pqPointParameterState->On )
            {
                if ( !rqPushedStates.PointAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PointAttribGroup = true;
                }
                StatePointParameter(pqPointParameterState);
            }
        }
        else if ( rspqState->IsExactly(PointsAntialiasingState::Class) )
        {
            PointsAntialiasingState* pqPointsAntialiasingState = StaticCast<PointsAntialiasingState>(rspqState);
            if ( pqPointsAntialiasingState->On )
            {
                if ( !rqPushedStates.PointAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PointAttribGroup = true;
                }
                if ( !rqPushedStates.HintAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_HINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.HintAttribGroup = true;
                }
                StatePointsAntialiasing(pqPointsAntialiasingState);
            }
        }
        else if ( rspqState->IsExactly(PointSizeState::Class) )
        {
            PointSizeState* pqPointSizeState = StaticCast<PointSizeState>(rspqState);
            if ( pqPointSizeState->On )
            {
                if ( !rqPushedStates.PointAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POINT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PointAttribGroup = true;
                }
                StatePointSize(pqPointSizeState);
            }
        }
        else if ( rspqState->IsExactly(PolygonModeState::Class) )
        {
            PolygonModeState* pqPolygonModeState = StaticCast<PolygonModeState>(rspqState);
            if ( pqPolygonModeState->On )
            {
                if ( !rqPushedStates.PolygonAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POLYGON_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PolygonAttribGroup = true;
                }
                StatePolygonMode(pqPolygonModeState);
            }
        }
        else if ( rspqState->IsExactly(PolygonOffsetState::Class) )
        {
            PolygonOffsetState* pqPolygonOffsetState = StaticCast<PolygonOffsetState>(rspqState);
            if ( pqPolygonOffsetState->On )
            {
                if ( !rqPushedStates.PolygonAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POLYGON_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PolygonAttribGroup = true;
                }
                StatePolygonOffset(pqPolygonOffsetState);
            }
        }
        else if ( rspqState->IsExactly(PolygonStippleState::Class) )
        {
            PolygonStippleState* pqPolygonStippleState = StaticCast<PolygonStippleState>(rspqState);
            if ( pqPolygonStippleState->On )
            {
                if ( !rqPushedStates.PolygonAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_POLYGON_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.PolygonAttribGroup = true;
                }
                StatePolygonStipple(pqPolygonStippleState);
            }
        }
        else if ( rspqState->IsExactly(ProjectionStatef::Class) )
        {
            ProjectionStatef* pqProjectionStatef = StaticCast<ProjectionStatef>(rspqState);
            if ( pqProjectionStatef->On )
            {
                if ( !rqPushedStates.ProjectionMatrix )
                {
                    glMatrixMode(GL_PROJECTION);
                    glPushMatrix();
                    rqPushedStates.ProjectionMatrix = true;
                }
                StateProjection(pqProjectionStatef);
            }
        }
        else if ( rspqState->IsExactly(ProjectionStated::Class) )
        {
            ProjectionStated* pqProjectionStated = StaticCast<ProjectionStated>(rspqState);
            if ( pqProjectionStated->On )
            {
                if ( !rqPushedStates.ProjectionMatrix )
                {
                    glMatrixMode(GL_PROJECTION);
                    glPushMatrix();
                    rqPushedStates.ProjectionMatrix = true;
                }
                StateProjection(pqProjectionStated);
            }
        }
        else if ( rspqState->IsExactly(ScissorState::Class) )
        {
            ScissorState* pqScissorState = StaticCast<ScissorState>(rspqState);
            if ( pqScissorState->On )
            {
                if ( !rqPushedStates.ScissorAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_SCISSOR_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.ScissorAttribGroup = true;
                }
                StateScissor(pqScissorState);
            }
        }
        else if ( rspqState->IsExactly(SecondaryColorState::Class) )
        {
            SecondaryColorState* pqSecondaryColorState = StaticCast<SecondaryColorState>(rspqState);
            if ( pqSecondaryColorState->On )
            {
                if ( !rqPushedStates.CurrentAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_CURRENT_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.CurrentAttribGroup = true;
                }
                StateSecondaryColor(pqSecondaryColorState);
            }
        }
        else if ( rspqState->IsExactly(StencilMaskState::Class) )
        {
            StencilMaskState* pqStencilMaskState = StaticCast<StencilMaskState>(rspqState);
            if ( pqStencilMaskState->On )
            {
                if ( !rqPushedStates.StencilBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_STENCIL_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.StencilBufferAttribGroup = true;
                }
                StateStencilMask(pqStencilMaskState);
            }
        }
        else if ( rspqState->IsExactly(StencilTestState::Class) )
        {
            StencilTestState* pqStencilTestState = StaticCast<StencilTestState>(rspqState);
            if ( pqStencilTestState->On )
            {
                if ( !rqPushedStates.StencilBufferAttribGroup )
                {
                    assert( !(rqPushedStates.Global && rqPushedStates.Quantity == m_iMaxAttribStackSize)
                        &&  !(!rqPushedStates.Global && m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity ==
                            m_iMaxAttribStackSize) );
                    glPushAttrib(GL_STENCIL_BUFFER_BIT);
                    rqPushedStates.Quantity++;
                    rqPushedStates.StencilBufferAttribGroup = true;
                }
                StateStencilTest(pqStencilTestState);
            }
        }
    }

    // receiving of light
    if ( !rqStateSensitive.GetReceiveAllLights() )
    {
        if ( !rqPushedStates.LightningAttribGroup )
        {
            assert( m_qPushedStatesGlobal.Quantity + rqPushedStates.Quantity != m_iMaxAttribStackSize );
            glPushAttrib(GL_LIGHTING_BIT);
            rqPushedStates.Quantity++;
            rqPushedStates.LightningAttribGroup = true;
        }
        glDisable(GL_LIGHTING);
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::DeactivateStates (PushedStatesRecord& rqPushedStates)
{
    for (int i = 0; i < rqPushedStates.Quantity; i++)
        glPopAttrib();

    rqPushedStates.Quantity = 0;
    rqPushedStates.ColorBufferAttribGroup = false;
    rqPushedStates.TransformAttribGroup = false;
    rqPushedStates.CurrentAttribGroup = false;
    rqPushedStates.PolygonAttribGroup = false;
    rqPushedStates.DepthBufferAttribGroup = false;
    rqPushedStates.ViewportAttribGroup = false;
    rqPushedStates.FogAttribGroup = false;
    rqPushedStates.HintAttribGroup = false;
    rqPushedStates.LightningAttribGroup = false;
    rqPushedStates.LineAttribGroup = false;
    rqPushedStates.MultisampleAttribGroup = false;
    rqPushedStates.PixelAttribGroup = false;
    rqPushedStates.PointAttribGroup = false;
    if ( rqPushedStates.ProjectionMatrix )
    {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        rqPushedStates.ProjectionMatrix = false;
    }
    rqPushedStates.ScissorAttribGroup = false;
    rqPushedStates.StencilBufferAttribGroup = false;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::AutodetectImageInternalFormat (RasterImage::ImageType eImageType,
    bool bStoreCompressed) const
{
    if ( !bStoreCompressed )
    {
        // detect internal format without data loss for some special types
        switch ( eImageType )
        {
        case RasterImage::IT_GRAYSCALE_8:
            return GL_LUMINANCE8;
        case RasterImage::IT_GRAYSCALE_16:
            return GL_LUMINANCE16;
        case RasterImage::IT_GRAYSCALE_32:
            return GL_LUMINANCE16;
        case RasterImage::IT_RGB_3_3_2:
            return GL_RGB4;
        case RasterImage::IT_RGB_5_6_5:
            return GL_RGB8;
        case RasterImage::IT_RGBA_4_4_4_4:
            return GL_RGBA4;
        case RasterImage::IT_RGBA_8_8_8_8:
            return GL_RGBA8;
        case RasterImage::IT_RGBA_16_16_16_16:
            return GL_RGBA16;
        case RasterImage::IT_RGBA_5_5_5_1:
            return GL_RGB5_A1;
        case RasterImage::IT_RGBA_10_10_10_2:
            return GL_RGB10_A2;
        }
    }
    else
    {
        if ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS )
        {
            if ( eImageType == RasterImage::IT_GRAYSCALE_8
            ||   eImageType == RasterImage::IT_GRAYSCALE_16
            ||   eImageType == RasterImage::IT_GRAYSCALE_32 )
            {
                return GL_COMPRESSED_LUMINANCE;
            }
            else if ( eImageType == RasterImage::IT_RGB_3_3_2
                 ||   eImageType == RasterImage::IT_RGB_5_6_5 )
            {
                return GL_COMPRESSED_RGB;
            }
            else if ( eImageType == RasterImage::IT_RGBA_4_4_4_4
                 ||   eImageType == RasterImage::IT_RGBA_8_8_8_8
                 ||   eImageType == RasterImage::IT_RGBA_16_16_16_16
                 ||   eImageType == RasterImage::IT_RGBA_5_5_5_1
                 ||   eImageType == RasterImage::IT_RGBA_10_10_10_2 )
            {
                return GL_COMPRESSED_RGBA;
            }
        }
    }

    assert( false );
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetImageFormat (RasterImage::ImageType eImageType) const
{
    switch ( eImageType )
    {
    case RasterImage::IT_GRAYSCALE_8:
        return GL_LUMINANCE;
    case RasterImage::IT_GRAYSCALE_16:
        return GL_LUMINANCE;
    case RasterImage::IT_GRAYSCALE_32:
        return GL_LUMINANCE;
    case RasterImage::IT_RGB_3_3_2:
        return GL_RGB;
    case RasterImage::IT_RGB_5_6_5:
        return GL_RGB;
    case RasterImage::IT_RGBA_4_4_4_4:
        return GL_RGBA;
    case RasterImage::IT_RGBA_8_8_8_8:
        return GL_RGBA;
    case RasterImage::IT_RGBA_16_16_16_16:
        return GL_RGBA;
    case RasterImage::IT_RGBA_5_5_5_1:
        return GL_RGBA;
    case RasterImage::IT_RGBA_10_10_10_2:
        return GL_RGBA;
    }

    assert( false );
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetImageType (RasterImage::ImageType eImageType) const
{
    switch ( eImageType )
    {
    case RasterImage::IT_GRAYSCALE_8:
        return GL_UNSIGNED_BYTE;
    case RasterImage::IT_GRAYSCALE_16:
        return GL_UNSIGNED_SHORT;
    case RasterImage::IT_GRAYSCALE_32:
        return GL_UNSIGNED_INT;
    case RasterImage::IT_RGB_3_3_2:
        return ( QGLExists_GL_EXT_packed_pixels == QGL_EXT_FULLY_EXISTS ? GL_UNSIGNED_BYTE_3_3_2 : 0 );
    case RasterImage::IT_RGB_5_6_5:
        return ( QGLExists_GL_VERSION_1_2 == QGL_EXT_FULLY_EXISTS ? GL_UNSIGNED_SHORT_5_6_5 : 0 );
    case RasterImage::IT_RGBA_4_4_4_4:
        return ( QGLExists_GL_EXT_packed_pixels == QGL_EXT_FULLY_EXISTS ? GL_UNSIGNED_SHORT_4_4_4_4 : 0 );
    case RasterImage::IT_RGBA_8_8_8_8:
        return GL_UNSIGNED_BYTE;
    case RasterImage::IT_RGBA_16_16_16_16:
        return GL_UNSIGNED_SHORT;
    case RasterImage::IT_RGBA_5_5_5_1:
        return ( QGLExists_GL_EXT_packed_pixels == QGL_EXT_FULLY_EXISTS ? GL_UNSIGNED_SHORT_5_5_5_1 : 0 );
    case RasterImage::IT_RGBA_10_10_10_2:
        return ( QGLExists_GL_EXT_packed_pixels == QGL_EXT_FULLY_EXISTS ? GL_UNSIGNED_INT_10_10_10_2 : 0 );
    }

    assert( false );
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetColorTablesInternalFormatEnum (
    PixelTransferState::ColorTablesInternalFormatType eType) const
{
    switch ( eType )
    {
    case PixelTransferState::CFIFT_ALPHA:
        return GL_ALPHA;
    case PixelTransferState::CFIFT_LUMINANCE:
        return GL_LUMINANCE;
    case PixelTransferState::CFIFT_LUMINANCE_ALPHA:
        return GL_LUMINANCE_ALPHA;
    case PixelTransferState::CFIFT_INTENSITY:
        return GL_INTENSITY;
    case PixelTransferState::CFIFT_RGB:
        return GL_RGB;
    case PixelTransferState::CFIFT_RGBA:
        return GL_RGBA;
    case PixelTransferState::CFIFT_ALPHA4:
        return GL_ALPHA4;
    case PixelTransferState::CFIFT_ALPHA8:
        return GL_ALPHA8;
    case PixelTransferState::CFIFT_ALPHA12:
        return GL_ALPHA12;
    case PixelTransferState::CFIFT_ALPHA16:
        return GL_ALPHA16;
    case PixelTransferState::CFIFT_LUMINANCE4:
        return GL_LUMINANCE4;
    case PixelTransferState::CFIFT_LUMINANCE8:
        return GL_LUMINANCE8;
    case PixelTransferState::CFIFT_LUMINANCE12:
        return GL_LUMINANCE12;
    case PixelTransferState::CFIFT_LUMINANCE16:
        return GL_LUMINANCE16;
    case PixelTransferState::CFIFT_LUMINANCE4_ALPHA4:
        return GL_LUMINANCE4_ALPHA4;
    case PixelTransferState::CFIFT_LUMINANCE6_ALPHA2:
        return GL_LUMINANCE6_ALPHA2;
    case PixelTransferState::CFIFT_LUMINANCE8_ALPHA8:
        return GL_LUMINANCE8_ALPHA8;
    case PixelTransferState::CFIFT_LUMINANCE12_ALPHA4:
        return GL_LUMINANCE12_ALPHA4;
    case PixelTransferState::CFIFT_LUMINANCE12_ALPHA12:
        return GL_LUMINANCE12_ALPHA12;
    case PixelTransferState::CFIFT_LUMINANCE16_ALPHA16:
        return GL_LUMINANCE16_ALPHA16;
    case PixelTransferState::CFIFT_INTENSITY4:
        return GL_INTENSITY4;
    case PixelTransferState::CFIFT_INTENSITY8:
        return GL_INTENSITY8;
    case PixelTransferState::CFIFT_INTENSITY12:
        return GL_INTENSITY12;
    case PixelTransferState::CFIFT_INTENSITY16:
        return GL_INTENSITY16;
    case PixelTransferState::CFIFT_R3_G3_B2:
        return GL_R3_G3_B2;
    case PixelTransferState::CFIFT_RGB4:
        return GL_RGB4;
    case PixelTransferState::CFIFT_RGB5:
        return GL_RGB5;
    case PixelTransferState::CFIFT_RGB8:
        return GL_RGB8;
    case PixelTransferState::CFIFT_RGB10:
        return GL_RGB10;
    case PixelTransferState::CFIFT_RGB12:
        return GL_RGB12;
    case PixelTransferState::CFIFT_RGB16:
        return GL_RGB16;
    case PixelTransferState::CFIFT_RGBA2:
        return GL_RGBA2;
    case PixelTransferState::CFIFT_RGBA4:
        return GL_RGBA4;
    case PixelTransferState::CFIFT_RGB5_A1:
        return GL_RGB5_A1;
    case PixelTransferState::CFIFT_RGBA8:
        return GL_RGBA8;
    case PixelTransferState::CFIFT_RGB10_A2:
        return GL_RGB10_A2;
    case PixelTransferState::CFIFT_RGBA12:
        return GL_RGBA12;
    case PixelTransferState::CFIFT_RGBA16:
        return GL_RGBA16;
    case PixelTransferState::CFIFT_COMPRESSED_ALPHA:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ? GL_COMPRESSED_ALPHA : 0 );
    case PixelTransferState::CFIFT_COMPRESSED_LUMINANCE:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ? GL_COMPRESSED_LUMINANCE : 0 );
    case PixelTransferState::CFIFT_COMPRESSED_LUMINANCE_ALPHA:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ? GL_COMPRESSED_LUMINANCE_ALPHA : 0 );
    case PixelTransferState::CFIFT_COMPRESSED_INTENSITY:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ? GL_COMPRESSED_INTENSITY : 0 );
    case PixelTransferState::CFIFT_COMPRESSED_RGB:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ? GL_COMPRESSED_RGB : 0 );
    case PixelTransferState::CFIFT_COMPRESSED_RGBA:
        return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?  GL_COMPRESSED_RGBA : 0 );
    }

    // no assertion
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetWrappingEnum (HRTextureBase::WrapType eType) const
{
    switch ( eType )
    {
    case HRTexturef::WT_CLAMP:
        return GL_CLAMP;
        break;
    case HRTexturef::WT_CLAMP_TO_EDGE:
        return ( QGLExists_GL_SGIS_texture_edge_clamp == QGL_EXT_FULLY_EXISTS ? GL_CLAMP_TO_EDGE : 0 );
        break;
    case HRTexturef::WT_CLAMP_TO_BORDER:
        return ( QGLExists_GL_ARB_texture_border_clamp == QGL_EXT_FULLY_EXISTS ? GL_CLAMP_TO_BORDER : 0 );
        break;
    case HRTexturef::WT_REPEAT:
        return GL_REPEAT;
        break;
    case HRTexturef::WT_MIRRORED_REPEAT:
        return ( QGLExists_GL_IBM_texture_mirrored_repeat == QGL_EXT_FULLY_EXISTS ? GL_MIRRORED_REPEAT : 0 );
        break;
    }

    // no assertion
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetCombineSourceEnum (HRTextureBase::CombineSourceType eType,
    int iCrossbarTexture) const
{
    assert( iCrossbarTexture >= 0 );

    switch ( eType )
    {
    case HRTextureBase::CST_TEXTURE:
        return GL_TEXTURE;
    case HRTextureBase::CST_TEXTURE_N:
        return ( QGLExists_GL_ARB_texture_env_crossbar == QGL_EXT_FULLY_EXISTS ? GL_TEXTURE0 + iCrossbarTexture :
            0 );
    case HRTextureBase::CST_CONSTANT:
        return GL_CONSTANT;
    case HRTextureBase::CST_PRIMARY_COLOR:
        return GL_PRIMARY_COLOR;
    case HRTextureBase::CST_PREVIOUS:
        return GL_PREVIOUS;
    }

    // no assertion
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetCombineOperandRgbEnum (HRTextureBase::CombineOperandRgbType eType) const
{
    switch ( eType )
    {
    case HRTextureBase::CORGBT_SRC_COLOR:
        return GL_SRC_COLOR;
    case HRTextureBase::CORGBT_ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    case HRTextureBase::CORGBT_SRC_ALPHA:
        return GL_SRC_ALPHA;
    case HRTextureBase::CORGBT_ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    }

    // no assertion
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int OpenGLRendering::GetCombineOperandAlphaEnum (HRTextureBase::CombineOperandAlphaType eType) const
{
    switch ( eType )
    {
    case HRTextureBase::COAT_SRC_ALPHA:
        return GL_SRC_ALPHA;
    case HRTextureBase::COAT_ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    }

    // no assertion
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
float OpenGLRendering::GetCombineScaleValue (HRTextureBase::CombineScaleType eType) const
{
    switch ( eType )
    {
    case HRTextureBase::CST_ONE:
        return 1.0f;
    case HRTextureBase::CST_TWO:
        return 2.0f;
    case HRTextureBase::CST_FOUR:
        return 4.0f;
    }

    // no assertion
    return 0.0f;
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateAlphaTest (AlphaTestState* pqAlphaTestState) const
{
    glEnable(GL_ALPHA_TEST);

    unsigned int uiEnum;
    switch ( pqAlphaTestState->Function )
    {
    case AlphaTestState::FT_NEVER:
        uiEnum = GL_NEVER;
        break;
    case AlphaTestState::FT_ALWAYS:
        uiEnum = GL_ALWAYS;
        break;
    case AlphaTestState::FT_LESS:
        uiEnum = GL_LESS;
        break;
    case AlphaTestState::FT_LESS_OR_EQUAL:
        uiEnum = GL_LEQUAL;
        break;
    case AlphaTestState::FT_EQUAL:
        uiEnum = GL_EQUAL;
        break;
    case AlphaTestState::FT_GREATER_OR_EQUAL:
        uiEnum = GL_GEQUAL;
        break;
    case AlphaTestState::FT_GREATER:
        uiEnum = GL_GREATER;
        break;
    case AlphaTestState::FT_NOT_EQUAL:
        uiEnum = GL_NOTEQUAL;
        break;
    }
    glAlphaFunc(uiEnum,pqAlphaTestState->Reference);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateBlending (BlendingState* pqBlendingState) const
{
    glEnable(GL_BLEND);

    // blending equations
    if ( QGLExists_GL_EXT_blend_minmax == QGL_EXT_FULLY_EXISTS )
    {
        unsigned int uiEqRgbEnum;
        switch ( pqBlendingState->EquationRgb )
        {
        case BlendingState::ET_ADD:
            uiEqRgbEnum = GL_FUNC_ADD;
            break;
        case BlendingState::ET_SUBTRACT:
            uiEqRgbEnum = ( QGLExists_GL_EXT_blend_subtract == QGL_EXT_FULLY_EXISTS ? GL_FUNC_SUBTRACT : 0 );
            break;
        case BlendingState::ET_REVERSE_SUBTRACT:
            uiEqRgbEnum = ( QGLExists_GL_EXT_blend_subtract == QGL_EXT_FULLY_EXISTS ? GL_FUNC_REVERSE_SUBTRACT :
                0 );
            break;
        case BlendingState::ET_MIN:
            uiEqRgbEnum = GL_MIN;
            break;
        case BlendingState::ET_MAX:
            uiEqRgbEnum = GL_MAX;
            break;
        case BlendingState::ET_LOGIC_OP:
            uiEqRgbEnum = ( QGLExists_GL_EXT_blend_logic_op == QGL_EXT_FULLY_EXISTS ? GL_LOGIC_OP : 0 );
            break;
        }
        if ( pqBlendingState->EquationRgb != pqBlendingState->EquationAlpha )
        {
            if ( QGLExists_GL_EXT_blend_equation_separate == QGL_EXT_FULLY_EXISTS )
            {
                unsigned int uiEqAlphaEnum;
                switch ( pqBlendingState->EquationAlpha )
                {
                case BlendingState::ET_ADD:
                    uiEqAlphaEnum = GL_FUNC_ADD;
                    break;
                case BlendingState::ET_SUBTRACT:
                    uiEqAlphaEnum = ( QGLExists_GL_EXT_blend_subtract == QGL_EXT_FULLY_EXISTS ? GL_FUNC_SUBTRACT :
                        0 );
                    break;
                case BlendingState::ET_REVERSE_SUBTRACT:
                    uiEqAlphaEnum = ( QGLExists_GL_EXT_blend_subtract == QGL_EXT_FULLY_EXISTS ?
                        GL_FUNC_REVERSE_SUBTRACT : 0 );
                    break;
                case BlendingState::ET_MIN:
                    uiEqAlphaEnum = GL_MIN;
                    break;
                case BlendingState::ET_MAX:
                    uiEqAlphaEnum = GL_MAX;
                    break;
                case BlendingState::ET_LOGIC_OP:
                    uiEqAlphaEnum = ( QGLExists_GL_EXT_blend_logic_op == QGL_EXT_FULLY_EXISTS ? GL_LOGIC_OP : 0 );
                    break;
                }
                if ( uiEqRgbEnum != 0 && uiEqAlphaEnum != 0 )
                    glBlendEquationSeparate(uiEqRgbEnum,uiEqAlphaEnum);
            }
        }
        else
        {
            if ( uiEqRgbEnum != 0 )
                glBlendEquation(uiEqRgbEnum);
        }
    }

    // blending factors
    unsigned int uiSrcFactorRgbEnum, uiDstFactorRgbEnum;
    switch ( pqBlendingState->SrcFactorRgb )
    {
    case BlendingState::FT_ZERO:
        uiSrcFactorRgbEnum = GL_ZERO;
        break;
    case BlendingState::FT_ONE:
        uiSrcFactorRgbEnum = GL_ONE;
        break;
    case BlendingState::FT_SRC_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_SRC_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_SRC_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_SRC_COLOR : 0 );
        break;
    case BlendingState::FT_DST_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_DST_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_DST_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_DST_COLOR : 0 );
        break;
    case BlendingState::FT_SRC_ALPHA:
        uiSrcFactorRgbEnum = GL_SRC_ALPHA;
        break;
    case BlendingState::FT_ONE_MINUS_SRC_ALPHA:
        uiSrcFactorRgbEnum = GL_ONE_MINUS_SRC_ALPHA;
        break;
    case BlendingState::FT_DST_ALPHA:
        uiSrcFactorRgbEnum = GL_DST_ALPHA;
        break;
    case BlendingState::FT_ONE_MINUS_DST_ALPHA:
        uiSrcFactorRgbEnum = GL_ONE_MINUS_DST_ALPHA;
        break;
    case BlendingState::FT_CONST_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_CONST_COLOR:
        uiSrcFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_CONSTANT_COLOR :
            0 );
        break;
    case BlendingState::FT_CONST_ALPHA:
        uiSrcFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_ALPHA : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_CONST_ALPHA:
        uiSrcFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_CONSTANT_ALPHA :
            0 );
        break;
    case BlendingState::FT_SRC_ALPHA_SATURATE:
        uiSrcFactorRgbEnum = GL_SRC_ALPHA_SATURATE;
        break;
    }
    switch ( pqBlendingState->DstFactorRgb )
    {
    case BlendingState::FT_ZERO:
        uiDstFactorRgbEnum = GL_ZERO;
        break;
    case BlendingState::FT_ONE:
        uiDstFactorRgbEnum = GL_ONE;
        break;
    case BlendingState::FT_SRC_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_SRC_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_SRC_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_SRC_COLOR : 0 );
        break;
    case BlendingState::FT_DST_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_DST_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_DST_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_DST_COLOR : 0 );
        break;
    case BlendingState::FT_SRC_ALPHA:
        uiDstFactorRgbEnum = GL_SRC_ALPHA;
        break;
    case BlendingState::FT_ONE_MINUS_SRC_ALPHA:
        uiDstFactorRgbEnum = GL_ONE_MINUS_SRC_ALPHA;
        break;
    case BlendingState::FT_DST_ALPHA:
        uiDstFactorRgbEnum = GL_DST_ALPHA;
        break;
    case BlendingState::FT_ONE_MINUS_DST_ALPHA:
        uiDstFactorRgbEnum = GL_ONE_MINUS_DST_ALPHA;
        break;
    case BlendingState::FT_CONST_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_COLOR : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_CONST_COLOR:
        uiDstFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_CONSTANT_COLOR :
            0 );
        break;
    case BlendingState::FT_CONST_ALPHA:
        uiDstFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_ALPHA : 0 );
        break;
    case BlendingState::FT_ONE_MINUS_CONST_ALPHA:
        uiDstFactorRgbEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_ONE_MINUS_CONSTANT_ALPHA :
            0 );
        break;
    case BlendingState::FT_SRC_ALPHA_SATURATE:
        uiDstFactorRgbEnum = GL_SRC_ALPHA_SATURATE;
        break;
    }
    if ( pqBlendingState->SrcFactorRgb != pqBlendingState->SrcFactorAlpha
    ||   pqBlendingState->DstFactorRgb != pqBlendingState->DstFactorAlpha )
    {
        if ( QGLExists_GL_EXT_blend_func_separate == QGL_EXT_FULLY_EXISTS )
        {
            unsigned int uiSrcFactorAlphaEnum, uiDstFactorAlphaEnum;
            switch ( pqBlendingState->SrcFactorAlpha )
            {
            case BlendingState::FT_ZERO:
                uiSrcFactorAlphaEnum = GL_ZERO;
                break;
            case BlendingState::FT_ONE:
                uiSrcFactorAlphaEnum = GL_ONE;
                break;
            case BlendingState::FT_SRC_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_SRC_COLOR : 0 );
                break;
            case BlendingState::FT_ONE_MINUS_SRC_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_SRC_COLOR : 0 );
                break;
            case BlendingState::FT_DST_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_DST_COLOR : 0 );
                break;
            case BlendingState::FT_ONE_MINUS_DST_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_DST_COLOR : 0 );
                break;
            case BlendingState::FT_SRC_ALPHA:
                uiSrcFactorAlphaEnum = GL_SRC_ALPHA;
                break;
            case BlendingState::FT_ONE_MINUS_SRC_ALPHA:
                uiSrcFactorAlphaEnum = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case BlendingState::FT_DST_ALPHA:
                uiSrcFactorAlphaEnum = GL_DST_ALPHA;
                break;
            case BlendingState::FT_ONE_MINUS_DST_ALPHA:
                uiSrcFactorAlphaEnum = GL_ONE_MINUS_DST_ALPHA;
                break;
            case BlendingState::FT_CONST_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_COLOR :
                    0 );
                break;
            case BlendingState::FT_ONE_MINUS_CONST_COLOR:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_CONSTANT_COLOR : 0 );
                break;
            case BlendingState::FT_CONST_ALPHA:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_ALPHA :
                    0 );
                break;
            case BlendingState::FT_ONE_MINUS_CONST_ALPHA:
                uiSrcFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_CONSTANT_ALPHA : 0 );
                break;
            case BlendingState::FT_SRC_ALPHA_SATURATE:
                uiSrcFactorAlphaEnum = GL_SRC_ALPHA_SATURATE;
                break;
            }
            switch ( pqBlendingState->DstFactorAlpha )
            {
            case BlendingState::FT_ZERO:
                uiDstFactorAlphaEnum = GL_ZERO;
                break;
            case BlendingState::FT_ONE:
                uiDstFactorAlphaEnum = GL_ONE;
                break;
            case BlendingState::FT_SRC_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_SRC_COLOR : 0 );
                break;
            case BlendingState::FT_ONE_MINUS_SRC_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_SRC_COLOR : 0 );
                break;
            case BlendingState::FT_DST_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ? GL_DST_COLOR : 0 );
                break;
            case BlendingState::FT_ONE_MINUS_DST_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_DST_COLOR : 0 );
                break;
            case BlendingState::FT_SRC_ALPHA:
                uiDstFactorAlphaEnum = GL_SRC_ALPHA;
                break;
            case BlendingState::FT_ONE_MINUS_SRC_ALPHA:
                uiDstFactorAlphaEnum = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case BlendingState::FT_DST_ALPHA:
                uiDstFactorAlphaEnum = GL_DST_ALPHA;
                break;
            case BlendingState::FT_ONE_MINUS_DST_ALPHA:
                uiDstFactorAlphaEnum = GL_ONE_MINUS_DST_ALPHA;
                break;
            case BlendingState::FT_CONST_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_COLOR :
                    0 );
                break;
            case BlendingState::FT_ONE_MINUS_CONST_COLOR:
                uiDstFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_CONSTANT_COLOR : 0 );
                break;
            case BlendingState::FT_CONST_ALPHA:
                uiDstFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ? GL_CONSTANT_ALPHA :
                    0 );
                break;
            case BlendingState::FT_ONE_MINUS_CONST_ALPHA:
                uiDstFactorAlphaEnum = ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS ?
                    GL_ONE_MINUS_CONSTANT_ALPHA : 0 );
                break;
            case BlendingState::FT_SRC_ALPHA_SATURATE:
                uiDstFactorAlphaEnum = GL_SRC_ALPHA_SATURATE;
                break;
            }
            if ( uiSrcFactorRgbEnum != 0 && uiDstFactorRgbEnum != 0 && uiSrcFactorAlphaEnum != 0
            &&   uiDstFactorAlphaEnum != 0 )
            {
                glBlendFuncSeparate(uiSrcFactorRgbEnum,uiDstFactorRgbEnum,uiSrcFactorAlphaEnum,
                    uiDstFactorAlphaEnum);
            }
        }
    }
    else
    {
        if ( uiSrcFactorRgbEnum != 0 && uiDstFactorRgbEnum != 0 )
            glBlendFunc(uiSrcFactorRgbEnum,uiDstFactorRgbEnum);
    }

    // blending color
    if ( pqBlendingState->BlendingColorSet )
    {
        if ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS )
        {
            glBlendColor(
                pqBlendingState->BlendingColor.R(),
                pqBlendingState->BlendingColor.G(),
                pqBlendingState->BlendingColor.B(),
                pqBlendingState->BlendingColor.A());
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateClippingPlanes (ClippingPlanesState* pqClippingPlanesState, bool bGlobal) const
{
    unsigned int uiEnum;
    if ( bGlobal )
    {
        // occupy the left part of enumerator range
        for (int i = 0; i < pqClippingPlanesState->GetPlaneQuantity(); i++)
        {
            if ( i == m_iMaxClippingPlanes )
                break;

            uiEnum = GL_CLIP_PLANE0 + i;
            glEnable(uiEnum);
            glClipPlane(uiEnum,pqClippingPlanesState->GetPlane(i));
        }
    }
    else
    {
        // occupy the right part of enumerator range
        for (int i = 0; i < pqClippingPlanesState->GetPlaneQuantity(); i++)
        {
            if ( i == m_iMaxClippingPlanes )
                break;

            uiEnum = GL_CLIP_PLANE0 + m_iMaxClippingPlanes - i;
            glEnable(uiEnum);
            glClipPlane(uiEnum,pqClippingPlanesState->GetPlane(i));
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateColorMask (ColorMaskState* pqColorMaskState) const
{
    glColorMask(
        pqColorMaskState->Red,
        pqColorMaskState->Green,
        pqColorMaskState->Blue,
        pqColorMaskState->Alpha);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateColor (ColorState* pqColorState) const
{
    glColor4fv(pqColorState->Color);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateCullFace (CullFaceState* pqCullFaceState) const
{
    glEnable(GL_CULL_FACE);

    unsigned int uiEnum;
    switch ( pqCullFaceState->Face )
    {
    case CullFaceState::FT_FRONT:
        uiEnum = GL_FRONT;
        break;
    case CullFaceState::FT_BACK:
        uiEnum = GL_BACK;
        break;
    case CullFaceState::FT_BOTH:
        uiEnum = GL_FRONT_AND_BACK;
        break;
    }
    glCullFace(uiEnum);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateDepthMask (DepthMaskState* pqDepthMaskState) const
{
    glDepthMask(pqDepthMaskState->EnableWrite);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateDepthRange (DepthRangeState* pqDepthRangeState) const
{
    glDepthRange(pqDepthRangeState->Near,pqDepthRangeState->Far);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateDepthTest (DepthTestState* pqDepthTestState) const
{
    glEnable(GL_DEPTH_TEST);

    unsigned int uiEnum;
    switch ( pqDepthTestState->Function )
    {
    case DepthTestState::FT_NEVER:
        uiEnum = GL_NEVER;
        break;
    case DepthTestState::FT_ALWAYS:
        uiEnum = GL_ALWAYS;
        break;
    case DepthTestState::FT_LESS:
        uiEnum = GL_LESS;
        break;
    case DepthTestState::FT_LESS_OR_EQUAL:
        uiEnum = GL_LEQUAL;
        break;
    case DepthTestState::FT_EQUAL:
        uiEnum = GL_EQUAL;
        break;
    case DepthTestState::FT_GREATER_OR_EQUAL:
        uiEnum = GL_GEQUAL;
        break;
    case DepthTestState::FT_GREATER:
        uiEnum = GL_GREATER;
        break;
    case DepthTestState::FT_NOT_EQUAL:
        uiEnum = GL_NOTEQUAL;
        break;
    }
    glDepthFunc(uiEnum);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateFog (FogState* pqFogState) const
{
    assert( pqFogState->Density >= 0.0f );

    glEnable(GL_FOG);

    // fog equation
    unsigned int uiEqEnum;
    switch ( pqFogState->Equation )
    {
    case FogState::ET_LINEAR:
        uiEqEnum = GL_LINEAR;
        break;
    case FogState::ET_EXP:
        uiEqEnum = GL_EXP;
        break;
    case FogState::ET_EXP2:
        uiEqEnum = GL_EXP2;
        break;
    }
    glFogi(GL_FOG_MODE,uiEqEnum);

    // coordinate source
    if ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS )
    {
        unsigned int uiFogCoordSrcEnum;
        switch ( pqFogState->CoordinateSource )
        {
        case FogState::CST_FRAGMENT_DEPTH:
            uiFogCoordSrcEnum = GL_FRAGMENT_DEPTH;
            break;
        case FogState::CST_FOG_COORD:
            uiFogCoordSrcEnum = GL_FOG_COORD;
            break;
        }
        glFogi(GL_FOG_COORD_SRC,uiFogCoordSrcEnum);
    }

    // fog color
    glFogfv(GL_FOG_COLOR,pqFogState->Color);

    // fog parameters
    glFogf(GL_FOG_DENSITY,pqFogState->Density);
    glFogf(GL_FOG_START,pqFogState->Start);
    glFogf(GL_FOG_END,pqFogState->End);

    // hint
    glHint(GL_FOG_HINT,( pqFogState->Hint == FogState::HT_FASTEST ? GL_FASTEST : GL_NICEST ));
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateFrontFace (FrontFaceState* pqFrontFaceState) const
{
    unsigned int uiEnum;
    switch ( pqFrontFaceState->Orientation )
    {
    case FrontFaceState::OT_CCW:
        uiEnum = GL_CCW;
        break;
    case FrontFaceState::OT_CW:
        uiEnum = GL_CW;
        break;
    }
    glFrontFace(uiEnum);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateLight (LightState* pqLightState, bool bGlobal)
{
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);

    unsigned int uiEnum;
    float afHGVector[4];
    for (int i = 0; i < pqLightState->GetLightQuantity(); i++)
    {
        if ( i == m_iMaxLights )
            break;

        HRLight3& rqLight = *pqLightState->GetLight(i);

        if ( rqLight.On )
        {
            // Transformation.  No check for identity matrix.
            glPushMatrix();
            glMultMatrixf(rqLight.GetTransformationMatrix());

            // a global state occupies the left part of enumerator range and a local state occupies the right
            // part
            uiEnum = ( bGlobal ? GL_LIGHT0 + i : GL_LIGHT0 + m_iMaxLights - i );

            glEnable(uiEnum);

            glLightfv(uiEnum,GL_AMBIENT,rqLight.Ambient);
            glLightfv(uiEnum,GL_DIFFUSE,rqLight.Diffuse);
            glLightfv(uiEnum,GL_SPECULAR,rqLight.Specular);

            if ( rqLight.Source == HRLight3::ST_DIRECTIONAL )
            {
                afHGVector[0] = -rqLight.Direction.X();
                afHGVector[1] = -rqLight.Direction.Y();
                afHGVector[2] = -rqLight.Direction.Z();
                afHGVector[3] = 0.0f;
                glLightfv(uiEnum,GL_POSITION,afHGVector);
            }
            else
            {
                assert( rqLight.AttenConstant >= 0.0f );
                assert( rqLight.AttenLinear >= 0.0f );
                assert( rqLight.AttenQuadratic >= 0.0f );

                afHGVector[0] = rqLight.GetTranslate().X();
                afHGVector[1] = rqLight.GetTranslate().Y();
                afHGVector[2] = rqLight.GetTranslate().Z();
                afHGVector[3] = 1.0f;
                glLightfv(uiEnum,GL_POSITION,afHGVector);

                glLightf(uiEnum,GL_CONSTANT_ATTENUATION,rqLight.AttenConstant);
                glLightf(uiEnum,GL_LINEAR_ATTENUATION,rqLight.AttenLinear);
                glLightf(uiEnum,GL_QUADRATIC_ATTENUATION,rqLight.AttenQuadratic);

                if ( rqLight.Source == HRLight3::ST_SPOT )
                {
                    assert( 0.0f <= rqLight.SpotExponent && rqLight.SpotExponent <= 128.0f );
                    assert( (0.0f <= rqLight.SpotCutoffDeg && rqLight.SpotCutoffDeg <= 90.0f)
                        ||  rqLight.SpotCutoffDeg == 180.0f );

                    glLightfv(uiEnum,GL_SPOT_DIRECTION,-rqLight.Direction);
                    glLightf(uiEnum,GL_SPOT_EXPONENT,rqLight.SpotExponent);
                    glLightf(uiEnum,GL_SPOT_CUTOFF,rqLight.SpotCutoffDeg);
                }
            }

            // transformation
            glPopMatrix();
        }
    }

    if ( pqLightState->ShadeModelSet )
        glShadeModel(( pqLightState->ShadeModel == LightState::SMT_FLAT ? GL_FLAT : GL_SMOOTH ));
    if ( pqLightState->GlobalAmbientSet )
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,pqLightState->GlobalAmbient);
    if ( pqLightState->TwoSidedSet )
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,pqLightState->TwoSided);

    if ( pqLightState->ActualViewpointForSpecularSet )
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,pqLightState->ActualViewpointForSpecular);

    if ( pqLightState->SeparateSpecularSet )
    {
        if ( QGLExists_GL_EXT_separate_specular_color == QGL_EXT_FULLY_EXISTS )
        {
            glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,( pqLightState->SeparateSpecular ?
                GL_SEPARATE_SPECULAR_COLOR : GL_SINGLE_COLOR ));
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateLinesAntialiasing (LinesAntialiasingState* pqLinesAntialiasingState) const
{
    glEnable(GL_LINE_SMOOTH);

    glHint(GL_LINE_SMOOTH_HINT,( pqLinesAntialiasingState->Hint == LinesAntialiasingState::HT_FASTEST ?
        GL_FASTEST : GL_NICEST ));
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateLineStipple (LineStippleState* pqLineStippleState) const
{
    glEnable(GL_LINE_STIPPLE);

    glLineStipple(pqLineStippleState->Factor,pqLineStippleState->Pattern);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateLineWidth (LineWidthState* pqLineWidthState) const
{
    assert( pqLineWidthState->Width > 0.0f );

    glLineWidth(pqLineWidthState->Width);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateLogicOperation (LogicOperationState* pqLogicOperationState) const
{
    glEnable(GL_COLOR_LOGIC_OP);

    unsigned int uiEnum;
    switch ( pqLogicOperationState->Operation )
    {
    case LogicOperationState::OT_CLEAR:
        uiEnum = GL_CLEAR;
        break;
    case LogicOperationState::OT_AND:
        uiEnum = GL_AND;
        break;
    case LogicOperationState::OT_AND_REVERSE:
        uiEnum = GL_AND_REVERSE;
        break;
    case LogicOperationState::OT_COPY:
        uiEnum = GL_COPY;
        break;
    case LogicOperationState::OT_AND_INVERTED:
        uiEnum = GL_AND_INVERTED;
        break;
    case LogicOperationState::OT_NOOP:
        uiEnum = GL_NOOP;
        break;
    case LogicOperationState::OT_XOR:
        uiEnum = GL_XOR;
        break;
    case LogicOperationState::OT_OR:
        uiEnum = GL_OR;
        break;
    case LogicOperationState::OT_NOR:
        uiEnum = GL_NOR;
        break;
    case LogicOperationState::OT_EQUIV:
        uiEnum = GL_EQUIV;
        break;
    case LogicOperationState::OT_INVERT:
        uiEnum = GL_INVERT;
        break;
    case LogicOperationState::OT_OR_REVERSE:
        uiEnum = GL_OR_REVERSE;
        break;
    case LogicOperationState::OT_COPY_INVERTED:
        uiEnum = GL_COPY_INVERTED;
        break;
    case LogicOperationState::OT_OR_INVERTED:
        uiEnum = GL_OR_INVERTED;
        break;
    case LogicOperationState::OT_NAND:
        uiEnum = GL_NAND;
        break;
    case LogicOperationState::OT_SET:
        uiEnum = GL_SET;
        break;
    }
    glLogicOp(uiEnum);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateMaterial (MaterialState* pqMaterialState) const
{
    // lightning must be enabled by a light state
    const HRMaterial& rqMaterial = *pqMaterialState->Material;

    assert( 0.0f <= rqMaterial.Shininess && rqMaterial.Shininess <= 128.0f );

    unsigned int uiEnum;
    switch ( rqMaterial.Face )
    {
    case HRMaterial::FT_FRONT:
        uiEnum = GL_FRONT;
        break;
    case HRMaterial::FT_BACK:
        uiEnum = GL_BACK;
        break;
    case HRMaterial::FT_BOTH:
        uiEnum = GL_FRONT_AND_BACK;
        break;
    }
    glMaterialfv(uiEnum,GL_AMBIENT,rqMaterial.Ambient);
    glMaterialfv(uiEnum,GL_DIFFUSE,rqMaterial.Diffuse);
    glMaterialfv(uiEnum,GL_SPECULAR,rqMaterial.Specular);
    glMaterialf(uiEnum,GL_SHININESS,rqMaterial.Shininess);
    glMaterialfv(uiEnum,GL_EMISSION,rqMaterial.Emission);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateMultisampleAntialiasing (
    MultisampleAntialiasingState* pqMultisampleAntialiasingState) const
{
    glEnable(GL_MULTISAMPLE);

    if ( pqMultisampleAntialiasingState->SampleAlphaToCoverageSet )
    {
        if ( pqMultisampleAntialiasingState->SampleAlphaToCoverage )
            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        else
            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    }
    if ( pqMultisampleAntialiasingState->SampleAlphaToOneSet )
    {
        if ( pqMultisampleAntialiasingState->SampleAlphaToOne )
            glEnable(GL_SAMPLE_ALPHA_TO_ONE);
        else
            glDisable(GL_SAMPLE_ALPHA_TO_ONE);
    }

    if ( pqMultisampleAntialiasingState->SampleCoverageSet )
    {
        glSampleCoverage(pqMultisampleAntialiasingState->SampleCoverageValue,
            pqMultisampleAntialiasingState->SampleCoverageInvert);
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateNormal (NormalStatef* pqNormalStatef) const
{
    glNormal3fv(pqNormalStatef->Normal);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateNormal (NormalStated* pqNormalStated) const
{
    glNormal3dv(pqNormalStated->Normal);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePixelTransfer (PixelTransferState* pqPixelTransferState) const
{
    // regular
    if ( pqPixelTransferState->ScaleSet )
    {
        glPixelTransferf(GL_RED_SCALE,pqPixelTransferState->Scale.R());
        glPixelTransferf(GL_GREEN_SCALE,pqPixelTransferState->Scale.G());
        glPixelTransferf(GL_BLUE_SCALE,pqPixelTransferState->Scale.B());
        glPixelTransferf(GL_ALPHA_SCALE,pqPixelTransferState->Scale.A());
    }
    if ( pqPixelTransferState->BiasSet )
    {
        glPixelTransferf(GL_RED_BIAS,pqPixelTransferState->Bias.R());
        glPixelTransferf(GL_GREEN_BIAS,pqPixelTransferState->Bias.G());
        glPixelTransferf(GL_BLUE_BIAS,pqPixelTransferState->Bias.B());
        glPixelTransferf(GL_ALPHA_BIAS,pqPixelTransferState->Bias.A());
    }
    if ( pqPixelTransferState->DepthScaleSet )
        glPixelTransferf(GL_DEPTH_SCALE,pqPixelTransferState->DepthScale);
    if ( pqPixelTransferState->DepthBiasSet )
        glPixelTransferf(GL_DEPTH_BIAS,pqPixelTransferState->DepthBias);

    // mapping
    if ( pqPixelTransferState->ColorToColorMappingSet )
    {
        assert( 32 <= pqPixelTransferState->ColorToColorMapSize
            &&  pqPixelTransferState->ColorToColorMapSize <= m_iMaxPixelMapTableSize
            &&  IsPowerOfTwo(pqPixelTransferState->ColorToColorMapSize) );

        unsigned int uiEnum;
        switch ( pqPixelTransferState->PixelColorMapping )
        {
        case PixelTransferState::PCMT_R_TO_R:
            uiEnum = GL_PIXEL_MAP_R_TO_R;
            break;
        case PixelTransferState::PCMT_G_TO_G:
            uiEnum = GL_PIXEL_MAP_G_TO_G;
            break;
        case PixelTransferState::PCMT_B_TO_B:
            uiEnum = GL_PIXEL_MAP_B_TO_B;
            break;
        case PixelTransferState::PCMT_A_TO_A:
            uiEnum = GL_PIXEL_MAP_A_TO_A;
            break;
        }
        glPixelMapfv(uiEnum,pqPixelTransferState->ColorToColorMapSize,*pqPixelTransferState->ColorMapEntry);
    }
    if ( pqPixelTransferState->StencilToStencilMappingSet )
    {
        assert( 32 <= pqPixelTransferState->StencilToStencilMapSize
            &&  pqPixelTransferState->StencilToStencilMapSize <= m_iMaxPixelMapTableSize
            &&  IsPowerOfTwo(pqPixelTransferState->StencilToStencilMapSize) );

        glPixelMapuiv(GL_PIXEL_MAP_S_TO_S,pqPixelTransferState->StencilToStencilMapSize,
            *pqPixelTransferState->StencilMapEntry);
    }

    // convolution
    if ( pqPixelTransferState->AnyConvolutionFilterSet )
    {
        if ( QGLExists_GL_EXT_convolution == QGL_EXT_FULLY_EXISTS )
        {
            assert( !(!pqPixelTransferState->ConvolutionFilterSeparableSet
                &&  (pqPixelTransferState->ConvolutionFilterImage->GetWidth() > m_iMaxConvolutionFilterWidth
                ||  pqPixelTransferState->ConvolutionFilterImage->GetHeight() > m_iMaxConvolutionFilterHeight
                ||  (pqPixelTransferState->ConvolutionFilter == PixelTransferState::CFT_1D
                &&  pqPixelTransferState->ConvolutionFilterImage->GetHeight() != 1))) );
            assert( !(pqPixelTransferState->ConvolutionFilterSeparableSet
                &&  pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetType() !=
                    pqPixelTransferState->ConvolutionFilterSeparableColumnImage->GetType()) );
            assert( !(pqPixelTransferState->ConvolutionFilterSeparableSet
                &&  (pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetHeight() != 1
                ||  pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetWidth() >
                    m_iMaxConvolutionFilterWidth
                ||  pqPixelTransferState->ConvolutionFilterSeparableColumnImage->GetWidth() != 1
                ||  pqPixelTransferState->ConvolutionFilterSeparableColumnImage->GetHeight() >
                    m_iMaxConvolutionFilterHeight)) );

            unsigned int uiInrlFrmtEnum;
            if ( pqPixelTransferState->ConvolutionFilterInternalFormatSet )
            {
                switch ( pqPixelTransferState->ConvolutionFilterInternalFormat )
                {
                case PixelTransferState::CFIFT_ALPHA:
                    uiInrlFrmtEnum = GL_ALPHA;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE:
                    uiInrlFrmtEnum = GL_LUMINANCE;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE_ALPHA:
                    uiInrlFrmtEnum = GL_LUMINANCE_ALPHA;
                    break;
                case PixelTransferState::CFIFT_INTENSITY:
                    uiInrlFrmtEnum = GL_INTENSITY;
                    break;
                case PixelTransferState::CFIFT_RGB:
                    uiInrlFrmtEnum = GL_RGB;
                    break;
                case PixelTransferState::CFIFT_RGBA:
                    uiInrlFrmtEnum = GL_RGBA;
                    break;
                case PixelTransferState::CFIFT_ALPHA4:
                    uiInrlFrmtEnum = GL_ALPHA4;
                    break;
                case PixelTransferState::CFIFT_ALPHA8:
                    uiInrlFrmtEnum = GL_ALPHA8;
                    break;
                case PixelTransferState::CFIFT_ALPHA12:
                    uiInrlFrmtEnum = GL_ALPHA12;
                    break;
                case PixelTransferState::CFIFT_ALPHA16:
                    uiInrlFrmtEnum = GL_ALPHA16;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE4:
                    uiInrlFrmtEnum = GL_LUMINANCE4;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE8:
                    uiInrlFrmtEnum = GL_LUMINANCE8;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE12:
                    uiInrlFrmtEnum = GL_LUMINANCE12;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE16:
                    uiInrlFrmtEnum = GL_LUMINANCE16;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE4_ALPHA4:
                    uiInrlFrmtEnum = GL_LUMINANCE4_ALPHA4;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE6_ALPHA2:
                    uiInrlFrmtEnum = GL_LUMINANCE6_ALPHA2;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE8_ALPHA8:
                    uiInrlFrmtEnum = GL_LUMINANCE8_ALPHA8;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE12_ALPHA4:
                    uiInrlFrmtEnum = GL_LUMINANCE12_ALPHA4;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE12_ALPHA12:
                    uiInrlFrmtEnum = GL_LUMINANCE12_ALPHA12;
                    break;
                case PixelTransferState::CFIFT_LUMINANCE16_ALPHA16:
                    uiInrlFrmtEnum = GL_LUMINANCE16_ALPHA16;
                    break;
                case PixelTransferState::CFIFT_INTENSITY4:
                    uiInrlFrmtEnum = GL_INTENSITY4;
                    break;
                case PixelTransferState::CFIFT_INTENSITY8:
                    uiInrlFrmtEnum = GL_INTENSITY8;
                    break;
                case PixelTransferState::CFIFT_INTENSITY12:
                    uiInrlFrmtEnum = GL_INTENSITY12;
                    break;
                case PixelTransferState::CFIFT_INTENSITY16:
                    uiInrlFrmtEnum = GL_INTENSITY16;
                    break;
                case PixelTransferState::CFIFT_R3_G3_B2:
                    uiInrlFrmtEnum = GL_R3_G3_B2;
                    break;
                case PixelTransferState::CFIFT_RGB4:
                    uiInrlFrmtEnum = GL_RGB4;
                    break;
                case PixelTransferState::CFIFT_RGB5:
                    uiInrlFrmtEnum = GL_RGB5;
                    break;
                case PixelTransferState::CFIFT_RGB8:
                    uiInrlFrmtEnum = GL_RGB8;
                    break;
                case PixelTransferState::CFIFT_RGB10:
                    uiInrlFrmtEnum = GL_RGB10;
                    break;
                case PixelTransferState::CFIFT_RGB12:
                    uiInrlFrmtEnum = GL_RGB12;
                    break;
                case PixelTransferState::CFIFT_RGB16:
                    uiInrlFrmtEnum = GL_RGB16;
                    break;
                case PixelTransferState::CFIFT_RGBA2:
                    uiInrlFrmtEnum = GL_RGBA2;
                    break;
                case PixelTransferState::CFIFT_RGBA4:
                    uiInrlFrmtEnum = GL_RGBA4;
                    break;
                case PixelTransferState::CFIFT_RGB5_A1:
                    uiInrlFrmtEnum = GL_RGB5_A1;
                    break;
                case PixelTransferState::CFIFT_RGBA8:
                    uiInrlFrmtEnum = GL_RGBA8;
                    break;
                case PixelTransferState::CFIFT_RGB10_A2:
                    uiInrlFrmtEnum = GL_RGB10_A2;
                    break;
                case PixelTransferState::CFIFT_RGBA12:
                    uiInrlFrmtEnum = GL_RGBA12;
                    break;
                case PixelTransferState::CFIFT_RGBA16:
                    uiInrlFrmtEnum = GL_RGBA16;
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_ALPHA:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_ALPHA : 0 );
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_LUMINANCE:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_LUMINANCE : 0 );
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_LUMINANCE_ALPHA:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_LUMINANCE_ALPHA : 0 );
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_INTENSITY:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_INTENSITY : 0 );
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_RGB:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_RGB : 0 );
                    break;
                case PixelTransferState::CFIFT_COMPRESSED_RGBA:
                    uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                        GL_COMPRESSED_RGBA : 0 );
                    break;
                }
            }
            else
            {
                // autodetect
                if ( !pqPixelTransferState->ConvolutionFilterSeparableSet )
                {
                    uiInrlFrmtEnum = AutodetectImageInternalFormat(
                        pqPixelTransferState->ConvolutionFilterImage->GetType(),
                        pqPixelTransferState->ConvolutionFilterStoreCompressedSet);
                }
                else
                {
                    uiInrlFrmtEnum = AutodetectImageInternalFormat(
                        pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetType(),
                        pqPixelTransferState->ConvolutionFilterStoreCompressedSet);
                }
            }

            unsigned int uiFrmtEnum, uiTypeEnum;
            if ( !pqPixelTransferState->ConvolutionFilterSeparableSet )
            {
                // usual
                uiFrmtEnum = GetImageFormat(pqPixelTransferState->ConvolutionFilterImage->GetType());
                uiTypeEnum = GetImageType(pqPixelTransferState->ConvolutionFilterImage->GetType());

                if ( uiInrlFrmtEnum != 0 && uiFrmtEnum != 0 && uiTypeEnum != 0 )
                {
                    if ( pqPixelTransferState->ConvolutionFilter == PixelTransferState::CFT_1D )
                    {
                        // one-dimensional
                        glEnable(GL_CONVOLUTION_1D);

                        glConvolutionFilter1D(GL_CONVOLUTION_1D,uiInrlFrmtEnum,
                            pqPixelTransferState->ConvolutionFilterImage->GetWidth(),
                            uiFrmtEnum,uiTypeEnum,*pqPixelTransferState->ConvolutionFilterImage);

                        // scale and bias
                        if ( pqPixelTransferState->ConvolutionFilterScaleSet )
                        {
                            glConvolutionParameterfv(GL_CONVOLUTION_1D,GL_CONVOLUTION_FILTER_SCALE,
                                pqPixelTransferState->ConvolutionFilterScale);
                        }
                        if ( pqPixelTransferState->ConvolutionFilterBiasSet )
                        {
                            glConvolutionParameterfv(GL_CONVOLUTION_1D,GL_CONVOLUTION_FILTER_BIAS,
                                pqPixelTransferState->ConvolutionFilterBias);
                        }

                        // convolution border
                        if ( pqPixelTransferState->ConvolutionFilterBorderSet )
                        {
                            unsigned int uiEnum;
                            switch ( pqPixelTransferState->ConvolutionFilterBorder )
                            {
                            case PixelTransferState::CFBT_REDUCE:
                                uiEnum = GL_REDUCE;
                                break;
                            case PixelTransferState::CFBT_CONSTANT_BORDER:
                                uiEnum = GL_CONSTANT_BORDER;
                                break;
                            case PixelTransferState::CFBT_REPLICATE_BORDER:
                                uiEnum = GL_REPLICATE_BORDER;
                                break;
                            }
                            glConvolutionParameteri(GL_CONVOLUTION_1D,GL_CONVOLUTION_BORDER_MODE,uiEnum);
                        }
                    }
                    else
                    {
                        // two-dimensional
                        glEnable(GL_CONVOLUTION_2D);

                        glConvolutionFilter2D(GL_CONVOLUTION_2D,uiInrlFrmtEnum,
                            pqPixelTransferState->ConvolutionFilterImage->GetWidth(),
                            pqPixelTransferState->ConvolutionFilterImage->GetHeight(),
                            uiFrmtEnum,uiTypeEnum,*pqPixelTransferState->ConvolutionFilterImage);

                        // scale and bias
                        if ( pqPixelTransferState->ConvolutionFilterScaleSet )
                        {
                            glConvolutionParameterfv(GL_CONVOLUTION_2D,GL_CONVOLUTION_FILTER_SCALE,
                                pqPixelTransferState->ConvolutionFilterScale);
                        }
                        if ( pqPixelTransferState->ConvolutionFilterBiasSet )
                        {
                            glConvolutionParameterfv(GL_CONVOLUTION_2D,GL_CONVOLUTION_FILTER_BIAS,
                                pqPixelTransferState->ConvolutionFilterBias);
                        }

                        // convolution border
                        if ( pqPixelTransferState->ConvolutionFilterBorderSet )
                        {
                            unsigned int uiEnum;
                            switch ( pqPixelTransferState->ConvolutionFilterBorder )
                            {
                            case PixelTransferState::CFBT_REDUCE:
                                uiEnum = GL_REDUCE;
                                break;
                            case PixelTransferState::CFBT_CONSTANT_BORDER:
                                uiEnum = GL_CONSTANT_BORDER;
                                break;
                            case PixelTransferState::CFBT_REPLICATE_BORDER:
                                uiEnum = GL_REPLICATE_BORDER;
                                break;
                            }
                            glConvolutionParameteri(GL_CONVOLUTION_2D,GL_CONVOLUTION_BORDER_MODE,uiEnum);
                        }
                    }
                }
            }
            else
            {
                // separable
                uiFrmtEnum = GetImageFormat(pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetType());
                uiTypeEnum = GetImageType(pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetType());

                if ( uiInrlFrmtEnum != 0 && uiFrmtEnum != 0 && uiTypeEnum != 0 )
                {
                    glEnable(GL_SEPARABLE_2D);

                    glSeparableFilter2D(GL_SEPARABLE_2D,uiInrlFrmtEnum,
                        pqPixelTransferState->ConvolutionFilterSeparableRowImage->GetWidth(),
                        pqPixelTransferState->ConvolutionFilterSeparableColumnImage->GetHeight(),uiFrmtEnum,
                        uiTypeEnum,*pqPixelTransferState->ConvolutionFilterSeparableRowImage,
                        *pqPixelTransferState->ConvolutionFilterSeparableColumnImage);

                    // scale and bias
                    if ( pqPixelTransferState->ConvolutionFilterScaleSet )
                    {
                        glConvolutionParameterfv(GL_SEPARABLE_2D,GL_CONVOLUTION_FILTER_SCALE,
                            pqPixelTransferState->ConvolutionFilterScale);
                    }
                    if ( pqPixelTransferState->ConvolutionFilterBiasSet )
                    {
                        glConvolutionParameterfv(GL_SEPARABLE_2D,GL_CONVOLUTION_FILTER_BIAS,
                            pqPixelTransferState->ConvolutionFilterBias);
                    }

                    // convolution border
                    if ( pqPixelTransferState->ConvolutionFilterBorderSet )
                    {
                        unsigned int uiEnum;
                        switch ( pqPixelTransferState->ConvolutionFilterBorder )
                        {
                        case PixelTransferState::CFBT_REDUCE:
                            uiEnum = GL_REDUCE;
                            break;
                        case PixelTransferState::CFBT_CONSTANT_BORDER:
                            uiEnum = GL_CONSTANT_BORDER;
                            break;
                        case PixelTransferState::CFBT_REPLICATE_BORDER:
                            uiEnum = GL_REPLICATE_BORDER;
                            break;
                        }
                        glConvolutionParameteri(GL_SEPARABLE_2D,GL_CONVOLUTION_BORDER_MODE,uiEnum);
                    }
                }
            }

            // post-convolution scale and bias
            if ( pqPixelTransferState->PostConvolutionRedScaleSet )
                glPixelTransferf(GL_POST_CONVOLUTION_RED_SCALE,pqPixelTransferState->PostConvolutionRedScale);
            if ( pqPixelTransferState->PostConvolutionGreenScaleSet )
                glPixelTransferf(GL_POST_CONVOLUTION_GREEN_SCALE,pqPixelTransferState->PostConvolutionGreenScale);
            if ( pqPixelTransferState->PostConvolutionBlueScaleSet )
                glPixelTransferf(GL_POST_CONVOLUTION_BLUE_SCALE,pqPixelTransferState->PostConvolutionBlueScale);
            if ( pqPixelTransferState->PostConvolutionAlphaScaleSet )
                glPixelTransferf(GL_POST_CONVOLUTION_ALPHA_SCALE,pqPixelTransferState->PostConvolutionAlphaScale);
            if ( pqPixelTransferState->PostConvolutionRedBiasSet )
                glPixelTransferf(GL_POST_CONVOLUTION_RED_BIAS,pqPixelTransferState->PostConvolutionRedBias);
            if ( pqPixelTransferState->PostConvolutionGreenBiasSet )
                glPixelTransferf(GL_POST_CONVOLUTION_GREEN_BIAS,pqPixelTransferState->PostConvolutionGreenBias);
            if ( pqPixelTransferState->PostConvolutionBlueBiasSet )
                glPixelTransferf(GL_POST_CONVOLUTION_BLUE_BIAS,pqPixelTransferState->PostConvolutionBlueBias);
            if ( pqPixelTransferState->PostConvolutionAlphaBiasSet )
                glPixelTransferf(GL_POST_CONVOLUTION_ALPHA_BIAS,pqPixelTransferState->PostConvolutionAlphaBias);
        }
    }

    // color matrix
    if ( pqPixelTransferState->ColorMatrixSet )
    {
        if ( QGLExists_GL_EXT_color_matrix == QGL_EXT_FULLY_EXISTS )
        {
            glMatrixMode(GL_COLOR);
            glMultMatrixf(pqPixelTransferState->ColorMatrix);

            // post-color-matrix scale and bias
            if ( pqPixelTransferState->PostColorMatrixRedScaleSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_RED_SCALE,pqPixelTransferState->PostColorMatrixRedScale);
            if ( pqPixelTransferState->PostColorMatrixGreenScaleSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_GREEN_SCALE,pqPixelTransferState->PostColorMatrixGreenScale);
            if ( pqPixelTransferState->PostColorMatrixBlueScaleSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_BLUE_SCALE,pqPixelTransferState->PostColorMatrixBlueScale);
            if ( pqPixelTransferState->PostColorMatrixAlphaScaleSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_ALPHA_SCALE,pqPixelTransferState->PostColorMatrixAlphaScale);
            if ( pqPixelTransferState->PostColorMatrixRedBiasSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_RED_BIAS,pqPixelTransferState->PostColorMatrixRedBias);
            if ( pqPixelTransferState->PostColorMatrixGreenBiasSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_GREEN_BIAS,pqPixelTransferState->PostColorMatrixGreenBias);
            if ( pqPixelTransferState->PostColorMatrixBlueBiasSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_BLUE_BIAS,pqPixelTransferState->PostColorMatrixBlueBias);
            if ( pqPixelTransferState->PostColorMatrixAlphaBiasSet )
                glPixelTransferf(GL_POST_COLOR_MATRIX_ALPHA_BIAS,pqPixelTransferState->PostColorMatrixAlphaBias);
        }
    }

    // color tables
    if ( pqPixelTransferState->ColorTableSet )
    {
        if ( QGLExists_GL_SGI_color_table == QGL_EXT_FULLY_EXISTS )
        {
            assert( pqPixelTransferState->ColorTableImage->GetHeight() == 1
                &&  pqPixelTransferState->ColorTableImage->GetWidth() >= 32
                &&  IsPowerOfTwo(pqPixelTransferState->ColorTableImage->GetWidth()) );

            unsigned int uiInrlFrmtEnum =
                GetColorTablesInternalFormatEnum(pqPixelTransferState->ColorTableInternalFormat);
            unsigned int uiFrmtEnum = GetImageFormat(pqPixelTransferState->ColorTableImage->GetType());
            unsigned int uiTypeEnum = GetImageType(pqPixelTransferState->ColorTableImage->GetType());

            if ( uiInrlFrmtEnum != 0 && uiFrmtEnum != 0 && uiTypeEnum != 0 )
            {
                glEnable(GL_COLOR_TABLE);

                glColorTable(GL_COLOR_TABLE,uiInrlFrmtEnum,pqPixelTransferState->ColorTableImage->GetWidth(),
                    uiFrmtEnum,uiTypeEnum,*pqPixelTransferState->ColorTableImage);

                // scale and bias
                if ( pqPixelTransferState->ColorTableScaleSet )
                {
                    glColorTableParameterfv(GL_COLOR_TABLE,GL_COLOR_TABLE_SCALE,
                        pqPixelTransferState->ColorTableScale);
                }
                if ( pqPixelTransferState->ColorTableBiasSet )
                {
                    glColorTableParameterfv(GL_COLOR_TABLE,GL_COLOR_TABLE_BIAS,
                        pqPixelTransferState->ColorTableBias);
                }
            }
        }
    }
    if ( pqPixelTransferState->PostConvColorTableSet )
    {
        if ( QGLExists_GL_SGI_color_table == QGL_EXT_FULLY_EXISTS )
        {
            assert( pqPixelTransferState->PostConvColorTableImage->GetHeight() == 1
                &&  pqPixelTransferState->PostConvColorTableImage->GetWidth() >= 32
                &&  IsPowerOfTwo(pqPixelTransferState->PostConvColorTableImage->GetWidth()) );

            unsigned int uiInrlFrmtEnum =
                GetColorTablesInternalFormatEnum(pqPixelTransferState->PostConvColorTableInternalFormat);
            unsigned int uiFrmtEnum = GetImageFormat(pqPixelTransferState->PostConvColorTableImage->GetType());
            unsigned int uiTypeEnum = GetImageType(pqPixelTransferState->PostConvColorTableImage->GetType());

            if ( uiInrlFrmtEnum != 0 && uiFrmtEnum != 0 && uiTypeEnum != 0 )
            {
                glEnable(GL_POST_CONVOLUTION_COLOR_TABLE);

                glColorTable(GL_POST_CONVOLUTION_COLOR_TABLE,uiInrlFrmtEnum,
                    pqPixelTransferState->PostConvColorTableImage->GetWidth(),uiFrmtEnum,uiTypeEnum,
                    *pqPixelTransferState->PostConvColorTableImage);

                // scale and bias
                if ( pqPixelTransferState->PostConvColorTableScaleSet )
                {
                    glColorTableParameterfv(GL_POST_CONVOLUTION_COLOR_TABLE,GL_COLOR_TABLE_SCALE,
                        pqPixelTransferState->PostConvColorTableScale);
                }
                if ( pqPixelTransferState->PostConvColorTableBiasSet )
                {
                    glColorTableParameterfv(GL_POST_CONVOLUTION_COLOR_TABLE,GL_COLOR_TABLE_BIAS,
                        pqPixelTransferState->PostConvColorTableBias);
                }
            }
        }
    }
    if ( pqPixelTransferState->ColorMatrixColorTableSet )
    {
        if ( QGLExists_GL_SGI_color_table == QGL_EXT_FULLY_EXISTS )
        {
            assert( pqPixelTransferState->ColorMatrixColorTableImage->GetHeight() == 1
                &&  pqPixelTransferState->ColorMatrixColorTableImage->GetWidth() >= 32
                &&  IsPowerOfTwo(pqPixelTransferState->ColorMatrixColorTableImage->GetWidth()) );

            unsigned int uiInrlFrmtEnum =
                GetColorTablesInternalFormatEnum(pqPixelTransferState->ColorMatrixColorTableInternalFormat);
            unsigned int uiFrmtEnum = GetImageFormat(pqPixelTransferState->ColorMatrixColorTableImage->GetType());
            unsigned int uiTypeEnum = GetImageType(pqPixelTransferState->ColorMatrixColorTableImage->GetType());

            if ( uiInrlFrmtEnum != 0 && uiFrmtEnum != 0 && uiTypeEnum != 0 )
            {
                glEnable(GL_POST_COLOR_MATRIX_COLOR_TABLE);

                glColorTable(GL_POST_COLOR_MATRIX_COLOR_TABLE,uiInrlFrmtEnum,
                    pqPixelTransferState->ColorMatrixColorTableImage->GetWidth(),uiFrmtEnum,uiTypeEnum,
                    *pqPixelTransferState->ColorMatrixColorTableImage);

                // scale and bias
                if ( pqPixelTransferState->ColorMatrixColorTableScaleSet )
                {
                    glColorTableParameterfv(GL_POST_COLOR_MATRIX_COLOR_TABLE,GL_COLOR_TABLE_SCALE,
                        pqPixelTransferState->ColorMatrixColorTableScale);
                }
                if ( pqPixelTransferState->ColorMatrixColorTableBiasSet )
                {
                    glColorTableParameterfv(GL_POST_COLOR_MATRIX_COLOR_TABLE,GL_COLOR_TABLE_BIAS,
                        pqPixelTransferState->ColorMatrixColorTableBias);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePointParameter (PointParameterState* pqPointParameterState) const
{
    assert( pqPointParameterState->MinSize >= 0.0f && pqPointParameterState->MaxSize >= 0.0f
        &&  pqPointParameterState->FadeThresholdSize >= 0.0f );

    if ( QGLExists_GL_EXT_point_parameters == QGL_EXT_FULLY_EXISTS )
    {
        glPointParameterf(GL_POINT_SIZE_MIN,pqPointParameterState->MinSize);
        glPointParameterf(GL_POINT_SIZE_MAX,pqPointParameterState->MaxSize);
        glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION,pqPointParameterState->DistAttenCoeffs);
        glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE,pqPointParameterState->FadeThresholdSize);
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePointsAntialiasing (PointsAntialiasingState* pqPointsAntialiasingState) const
{
    glEnable(GL_POINT_SMOOTH);

    glHint(GL_POINT_SMOOTH_HINT,( pqPointsAntialiasingState->Hint == PointsAntialiasingState::HT_FASTEST ?
        GL_FASTEST : GL_NICEST ));
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePointSize (PointSizeState* pqPointSizeState) const
{
    assert( pqPointSizeState->Size > 0.0f );
    glPointSize(pqPointSizeState->Size);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePolygonMode (PolygonModeState* pqPolygonModeState) const
{
    unsigned int uiFaceEnum, uiModeEnum;
    switch ( pqPolygonModeState->Face )
    {
    case PolygonModeState::FT_FRONT:
        uiFaceEnum = GL_FRONT;
        break;
    case PolygonModeState::FT_BACK:
        uiFaceEnum = GL_BACK;
        break;
    case PolygonModeState::FT_BOTH:
        uiFaceEnum = GL_FRONT_AND_BACK;
        break;
    }
    switch ( pqPolygonModeState->Mode )
    {
    case PolygonModeState::MT_POINT:
        uiModeEnum = GL_POINT;
        break;
    case PolygonModeState::MT_LINE:
        uiModeEnum = GL_LINE;
        break;
    case PolygonModeState::MT_FILL:
        uiModeEnum = GL_FILL;
        break;
    }
    glPolygonMode(uiFaceEnum,uiModeEnum);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePolygonOffset (PolygonOffsetState* pqPolygonOffsetState) const
{
    unsigned int uiModeEnum;
    switch ( pqPolygonOffsetState->Mode )
    {
    case PolygonOffsetState::MT_POINT:
        uiModeEnum = GL_POLYGON_OFFSET_POINT;
        break;
    case PolygonOffsetState::MT_LINE:
        uiModeEnum = GL_POLYGON_OFFSET_LINE;
        break;
    case PolygonOffsetState::MT_FILL:
        uiModeEnum = GL_POLYGON_OFFSET_FILL;
        break;
    }
    glEnable(uiModeEnum);

    glPolygonOffset(pqPolygonOffsetState->Factor,pqPolygonOffsetState->Units);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StatePolygonStipple (PolygonStippleState* pqPolygonStippleState) const
{
    assert( pqPolygonStippleState->Pattern->GetQuantity() == 1024 );

    glEnable(GL_POLYGON_STIPPLE);

    glPolygonStipple(*pqPolygonStippleState->Pattern);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateProjection (ProjectionStatef* pqProjectionState) const
{
    // must be already in the projection matrix mode
    glLoadMatrixf(pqProjectionState->ProjectionMatrix);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateProjection (ProjectionStated* pqProjectionState) const
{
    // must be already in the projection matrix mode
    glLoadMatrixd(pqProjectionState->ProjectionMatrix);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateScissor (ScissorState* pqScissorState) const
{
    assert( pqScissorState->Width >= 0 && pqScissorState->Height >= 0 );

    glEnable(GL_SCISSOR_TEST);

    glScissor(pqScissorState->X,pqScissorState->Y,pqScissorState->Width,pqScissorState->Height);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateSecondaryColor (SecondaryColorState* pqSecondaryColorState) const
{
    if ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS )
        glSecondaryColor3fv(pqSecondaryColorState->Color);
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateStencilMask (StencilMaskState* pqStencilMaskState) const
{
    if ( !pqStencilMaskState->Separate )
    {
        glStencilMask(pqStencilMaskState->Mask);
    }
    else
    {
        // lone extension function
        if ( glStencilMaskSeparate != 0 )
        {
            unsigned int uiEnum;
            switch ( pqStencilMaskState->Face )
            {
            case StencilMaskState::FT_FRONT:
                uiEnum = GL_FRONT;
                break;
            case StencilMaskState::FT_BACK:
                uiEnum = GL_BACK;
                break;
            case StencilMaskState::FT_BOTH:
                uiEnum = GL_FRONT_AND_BACK;
                break;
            }
            glStencilMaskSeparate(uiEnum,pqStencilMaskState->Mask);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateStencilTest (StencilTestState* pqStencilTestState) const
{
    glEnable(GL_STENCIL_TEST);

    unsigned int uiFuncEnum, uiStnlFailEnum, uiStnlPassDpthFailEnum, uiStnlPassDpthPassEnum;
    switch ( pqStencilTestState->Function )
    {
    case StencilTestState::FT_NEVER:
        uiFuncEnum = GL_NEVER;
        break;
    case StencilTestState::FT_ALWAYS:
        uiFuncEnum = GL_ALWAYS;
        break;
    case StencilTestState::FT_LESS:
        uiFuncEnum = GL_LESS;
        break;
    case StencilTestState::FT_LESS_OR_EQUAL:
        uiFuncEnum = GL_LEQUAL;
        break;
    case StencilTestState::FT_EQUAL:
        uiFuncEnum = GL_EQUAL;
        break;
    case StencilTestState::FT_GREATER_OR_EQUAL:
        uiFuncEnum = GL_GEQUAL;
        break;
    case StencilTestState::FT_GREATER:
        uiFuncEnum = GL_GREATER;
        break;
    case StencilTestState::FT_NOT_EQUAL:
        uiFuncEnum = GL_NOTEQUAL;
        break;
    }
    switch ( pqStencilTestState->StencilFail )
    {
    case StencilTestState::OT_KEEP:
        uiStnlFailEnum = GL_KEEP;
        break;
    case StencilTestState::OT_ZERO:
        uiStnlFailEnum = GL_ZERO;
        break;
    case StencilTestState::OT_REPLACE:
        uiStnlFailEnum = GL_REPLACE;
        break;
    case StencilTestState::OT_INCR:
        uiStnlFailEnum = GL_INCR;
        break;
    case StencilTestState::OT_DECR:
        uiStnlFailEnum = GL_DECR;
        break;
    case StencilTestState::OT_INVERT:
        uiStnlFailEnum = GL_INVERT;
        break;
    case StencilTestState::OT_INCR_WRAP:
        uiStnlFailEnum = GL_INCR_WRAP;
        break;
    case StencilTestState::OT_DECR_WRAP:
        uiStnlFailEnum = GL_DECR_WRAP;
        break;
    }
    switch ( pqStencilTestState->StencilPassDepthFail )
    {
    case StencilTestState::OT_KEEP:
        uiStnlPassDpthFailEnum = GL_KEEP;
        break;
    case StencilTestState::OT_ZERO:
        uiStnlPassDpthFailEnum = GL_ZERO;
        break;
    case StencilTestState::OT_REPLACE:
        uiStnlPassDpthFailEnum = GL_REPLACE;
        break;
    case StencilTestState::OT_INCR:
        uiStnlPassDpthFailEnum = GL_INCR;
        break;
    case StencilTestState::OT_DECR:
        uiStnlPassDpthFailEnum = GL_DECR;
        break;
    case StencilTestState::OT_INVERT:
        uiStnlPassDpthFailEnum = GL_INVERT;
        break;
    case StencilTestState::OT_INCR_WRAP:
        uiStnlPassDpthFailEnum = GL_INCR_WRAP;
        break;
    case StencilTestState::OT_DECR_WRAP:
        uiStnlPassDpthFailEnum = GL_DECR_WRAP;
        break;
    }
    switch ( pqStencilTestState->StencilPassDepthPass )
    {
    case StencilTestState::OT_KEEP:
        uiStnlPassDpthPassEnum = GL_KEEP;
        break;
    case StencilTestState::OT_ZERO:
        uiStnlPassDpthPassEnum = GL_ZERO;
        break;
    case StencilTestState::OT_REPLACE:
        uiStnlPassDpthPassEnum = GL_REPLACE;
        break;
    case StencilTestState::OT_INCR:
        uiStnlPassDpthPassEnum = GL_INCR;
        break;
    case StencilTestState::OT_DECR:
        uiStnlPassDpthPassEnum = GL_DECR;
        break;
    case StencilTestState::OT_INVERT:
        uiStnlPassDpthPassEnum = GL_INVERT;
        break;
    case StencilTestState::OT_INCR_WRAP:
        uiStnlPassDpthPassEnum = GL_INCR_WRAP;
        break;
    case StencilTestState::OT_DECR_WRAP:
        uiStnlPassDpthPassEnum = GL_DECR_WRAP;
        break;
    }
    if ( !pqStencilTestState->Separate )
    {
        glStencilFunc(uiFuncEnum,pqStencilTestState->Reference,pqStencilTestState->Mask);
        glStencilOp(uiStnlFailEnum,uiStnlPassDpthFailEnum,uiStnlPassDpthPassEnum);
    }
    else
    {
        if ( QGLExists_GL_ATI_separate_stencil == QGL_EXT_FULLY_EXISTS )
        {
            unsigned int uiFaceEnum;
            switch ( pqStencilTestState->Face )
            {
            case StencilTestState::FT_FRONT:
                uiFaceEnum = GL_FRONT;
                break;
            case StencilTestState::FT_BACK:
                uiFaceEnum = GL_BACK;
                break;
            case StencilTestState::FT_BOTH:
                uiFaceEnum = GL_FRONT_AND_BACK;
                break;
            }
            glStencilFuncSeparate(uiFaceEnum,uiFuncEnum,pqStencilTestState->Reference,pqStencilTestState->Mask);
            glStencilOpSeparate(uiFaceEnum,uiStnlFailEnum,uiStnlPassDpthFailEnum,uiStnlPassDpthPassEnum);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateTextureUnit (HRTextureBase& rqTexture)
{
    // target enumerator
    unsigned int uiTrgtEnum;
    switch ( rqTexture.GeneralType )
    {
    case HRTextureBase::GTT_1D:
        uiTrgtEnum = GL_TEXTURE_1D;
        if ( !m_bTexture1dEnabled )
        {
            glEnable(uiTrgtEnum);
            m_bTexture1dEnabled = true;
        }
        break;
    case HRTextureBase::GTT_2D:
        uiTrgtEnum = GL_TEXTURE_2D;
        if ( !m_bTexture2dEnabled )
        {
            glEnable(uiTrgtEnum);
            m_bTexture2dEnabled = true;
        }
        break;
    case HRTextureBase::GTT_3D:
        uiTrgtEnum = ( QGLExists_GL_EXT_texture3D == QGL_EXT_FULLY_EXISTS ? GL_TEXTURE_3D : 0 );
        if ( !m_bTexture3dEnabled && uiTrgtEnum != 0 )
        {
            glEnable(uiTrgtEnum);
            m_bTexture3dEnabled = true;
        }
        break;
    case HRTextureBase::GTT_CUBE_MAP:
        uiTrgtEnum = ( QGLExists_GL_EXT_texture_cube_map == QGL_EXT_FULLY_EXISTS ? GL_TEXTURE_CUBE_MAP : 0 );
        if ( !m_bTextureCubeMapEnabled && uiTrgtEnum != 0 )
        {
            glEnable(uiTrgtEnum);
            m_bTextureCubeMapEnabled = true;
        }
        break;
    }

    if ( uiTrgtEnum == 0 )
        return;

    if ( rqTexture.GetBufferProfile() == HRBufferable::PT_BUFFER && !rqTexture.NeedRebuffer
    &&   rqTexture.BufferId != 0 )
    {
        // bind the previously created texture object
        glBindTexture(uiTrgtEnum,rqTexture.BufferId);
    }
    else
    {
        if ( rqTexture.GetBufferProfile() == HRBufferable::PT_BUFFER )
        {
            // the texture object needs to be created or recreated
            if ( rqTexture.BufferId != 0 )
                glDeleteTextures(1,&rqTexture.BufferId);

            glGenTextures(1,&rqTexture.BufferId);
            rqTexture.NeedRebuffer = false;
            glBindTexture(uiTrgtEnum,rqTexture.BufferId);
        }

        // internal format
        unsigned int uiInrlFrmtEnum;
        if ( rqTexture.InternalFormatSet )
        {
            switch ( rqTexture.InternalFormat )
            {
            case HRTextureBase::IFT_ALPHA:
                uiInrlFrmtEnum = GL_ALPHA;
                break;
            case HRTextureBase::IFT_LUMINANCE:
                uiInrlFrmtEnum = GL_LUMINANCE;
                break;
            case HRTextureBase::IFT_LUMINANCE_ALPHA:
                uiInrlFrmtEnum = GL_LUMINANCE_ALPHA;
                break;
            case HRTextureBase::IFT_INTENSITY:
                uiInrlFrmtEnum = GL_INTENSITY;
                break;
            case HRTextureBase::IFT_RGB:
                uiInrlFrmtEnum = GL_RGB;
                break;
            case HRTextureBase::IFT_RGBA:
                uiInrlFrmtEnum = GL_RGBA;
                break;
            case HRTextureBase::IFT_ALPHA4:
                uiInrlFrmtEnum = GL_ALPHA4;
                break;
            case HRTextureBase::IFT_ALPHA8:
                uiInrlFrmtEnum = GL_ALPHA8;
                break;
            case HRTextureBase::IFT_ALPHA12:
                uiInrlFrmtEnum = GL_ALPHA12;
                break;
            case HRTextureBase::IFT_ALPHA16:
                uiInrlFrmtEnum = GL_ALPHA16;
                break;
            case HRTextureBase::IFT_LUMINANCE4:
                uiInrlFrmtEnum = GL_LUMINANCE4;
                break;
            case HRTextureBase::IFT_LUMINANCE8:
                uiInrlFrmtEnum = GL_LUMINANCE8;
                break;
            case HRTextureBase::IFT_LUMINANCE12:
                uiInrlFrmtEnum = GL_LUMINANCE12;
                break;
            case HRTextureBase::IFT_LUMINANCE16:
                uiInrlFrmtEnum = GL_LUMINANCE16;
                break;
            case HRTextureBase::IFT_LUMINANCE4_ALPHA4:
                uiInrlFrmtEnum = GL_LUMINANCE4_ALPHA4;
                break;
            case HRTextureBase::IFT_LUMINANCE6_ALPHA2:
                uiInrlFrmtEnum = GL_LUMINANCE6_ALPHA2;
                break;
            case HRTextureBase::IFT_LUMINANCE8_ALPHA8:
                uiInrlFrmtEnum = GL_LUMINANCE8_ALPHA8;
                break;
            case HRTextureBase::IFT_LUMINANCE12_ALPHA4:
                uiInrlFrmtEnum = GL_LUMINANCE12_ALPHA4;
                break;
            case HRTextureBase::IFT_LUMINANCE12_ALPHA12:
                uiInrlFrmtEnum = GL_LUMINANCE12_ALPHA12;
                break;
            case HRTextureBase::IFT_LUMINANCE16_ALPHA16:
                uiInrlFrmtEnum = GL_LUMINANCE16_ALPHA16;
                break;
            case HRTextureBase::IFT_INTENSITY4:
                uiInrlFrmtEnum = GL_INTENSITY4;
                break;
            case HRTextureBase::IFT_INTENSITY8:
                uiInrlFrmtEnum = GL_INTENSITY8;
                break;
            case HRTextureBase::IFT_INTENSITY12:
                uiInrlFrmtEnum = GL_INTENSITY12;
                break;
            case HRTextureBase::IFT_INTENSITY16:
                uiInrlFrmtEnum = GL_INTENSITY16;
                break;
            case HRTextureBase::IFT_R3_G3_B2:
                uiInrlFrmtEnum = GL_R3_G3_B2;
                break;
            case HRTextureBase::IFT_RGB4:
                uiInrlFrmtEnum = GL_RGB4;
                break;
            case HRTextureBase::IFT_RGB5:
                uiInrlFrmtEnum = GL_RGB5;
                break;
            case HRTextureBase::IFT_RGB8:
                uiInrlFrmtEnum = GL_RGB8;
                break;
            case HRTextureBase::IFT_RGB10:
                uiInrlFrmtEnum = GL_RGB10;
                break;
            case HRTextureBase::IFT_RGB12:
                uiInrlFrmtEnum = GL_RGB12;
                break;
            case HRTextureBase::IFT_RGB16:
                uiInrlFrmtEnum = GL_RGB16;
                break;
            case HRTextureBase::IFT_RGBA2:
                uiInrlFrmtEnum = GL_RGBA2;
                break;
            case HRTextureBase::IFT_RGBA4:
                uiInrlFrmtEnum = GL_RGBA4;
                break;
            case HRTextureBase::IFT_RGB5_A1:
                uiInrlFrmtEnum = GL_RGB5_A1;
                break;
            case HRTextureBase::IFT_RGBA8:
                uiInrlFrmtEnum = GL_RGBA8;
                break;
            case HRTextureBase::IFT_RGB10_A2:
                uiInrlFrmtEnum = GL_RGB10_A2;
                break;
            case HRTextureBase::IFT_RGBA12:
                uiInrlFrmtEnum = GL_RGBA12;
                break;
            case HRTextureBase::IFT_RGBA16:
                uiInrlFrmtEnum = GL_RGBA16;
                break;
            case HRTextureBase::IFT_COMPRESSED_ALPHA:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_ALPHA : 0 );
                break;
            case HRTextureBase::IFT_COMPRESSED_LUMINANCE:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_LUMINANCE : 0 );
                break;
            case HRTextureBase::IFT_COMPRESSED_LUMINANCE_ALPHA:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_LUMINANCE_ALPHA : 0 );
                break;
            case HRTextureBase::IFT_COMPRESSED_INTENSITY:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_INTENSITY : 0 );
                break;
            case HRTextureBase::IFT_COMPRESSED_RGB:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_RGB : 0 );
                break;
            case HRTextureBase::IFT_COMPRESSED_RGBA:
                uiInrlFrmtEnum = ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS ?
                    GL_COMPRESSED_RGBA : 0 );
                break;
            }
        }
        else
        {
            // autodetect
            if ( rqTexture.Type == HRTextureBase::TT_DEPTH_1D
            ||   rqTexture.Type == HRTextureBase::TT_DEPTH_2D )
            {
                // depth texture
                if ( QGLExists_GL_ARB_depth_texture == QGL_EXT_FULLY_EXISTS )
                {
                    switch ( rqTexture.Image->GetType() )
                    {
                    case RasterImage::IT_GRAYSCALE_16:
                        uiInrlFrmtEnum = GL_DEPTH_COMPONENT16;
                        break;
                    case RasterImage::IT_GRAYSCALE_32:
                        uiInrlFrmtEnum = GL_DEPTH_COMPONENT32;
                        break;
                    default:
                        assert( false );
                        break;
                    }
                }
                else
                {
                    uiInrlFrmtEnum = 0;
                }
            }
            else
            {
                // not a depth texture
                if ( rqTexture.Image )
                {
                    uiInrlFrmtEnum = AutodetectImageInternalFormat(rqTexture.Image->GetType(),
                        rqTexture.StoreCompressedSet);
                }
                else if ( rqTexture.ImageArray )
                {
                    uiInrlFrmtEnum = AutodetectImageInternalFormat((*rqTexture.ImageArray)[0]->GetType(),
                        rqTexture.StoreCompressedSet);
                }
                else
                {
                    // precompressed texture
                    if ( rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_DXT1_1D
                    ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_DXT1_2D
                    ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_DXT1_3D
                    ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_DXT1_CUBE_MAP )
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_EXT_texture_compression_s3tc == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGB_S3TC_DXT1_EXT : 0 );
                    }
                    else if ( rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT1_1D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT1_2D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT1_3D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT1_CUBE_MAP )
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_EXT_texture_compression_s3tc == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : 0 );
                    }
                    else if ( rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT3_1D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT3_2D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT3_3D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT3_CUBE_MAP )
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_EXT_texture_compression_s3tc == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGBA_S3TC_DXT3_EXT : 0 );
                    }
                    else if ( rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT5_1D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT5_2D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT5_3D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGBA_DXT5_CUBE_MAP )
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_EXT_texture_compression_s3tc == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : 0 );
                    }
                    else if ( rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_FXT1_2D
                         ||   rqTexture.Type == HRTextureBase::TT_COMPRESSED_RGB_FXT1_3D )
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_3DFX_texture_compression_FXT1 == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGB_FXT1_3DFX : 0 );
                    }
                    else  // RGBA_FXT1
                    {
                        uiInrlFrmtEnum = ( QGLExists_GL_3DFX_texture_compression_FXT1 == QGL_EXT_FULLY_EXISTS ?
                            GL_COMPRESSED_RGBA_FXT1_3DFX : 0 );
                    }
                }
            }
        }

        if ( uiInrlFrmtEnum == 0 )
            return;

        // border
        int iBorder = ( rqTexture.BorderSet ? 1 : 0 );

        // mipmap building and subtexturing are incompatible
        assert( !(rqTexture.BuildMipmaps && rqTexture.SubTextureMode) );

        // TexImage
        if ( rqTexture.GeneralType == HRTextureBase::GTT_1D )
        {
            // one-dimensional
            if ( !rqTexture.CompressedTextureMipmapArray )
            {
                // not precompressed
                int iImageWidth, iImageHeight;

                if ( !rqTexture.SubRectSet )
                {
                    // no subrectangle
                    iImageWidth = rqTexture.Image->GetWidth();
                    iImageHeight = rqTexture.Image->GetHeight();
                }
                else
                {
                    // subrectangle
                    assert( rqTexture.SubRect.X + rqTexture.SubRect.Width <= rqTexture.Image->GetWidth() );

                    glPixelStorei(GL_UNPACK_ROW_LENGTH,rqTexture.Image->GetWidth());
                    glPixelStorei(GL_UNPACK_SKIP_PIXELS,rqTexture.SubRect.X);

                    iImageWidth = rqTexture.SubRect.Width;
                    iImageHeight = rqTexture.SubRect.Height;
                }

                assert( iImageWidth <= m_iMaxTextureSize && iImageHeight == 1 );

                // format and type
                unsigned int uiFrmtEnum = GetImageFormat(rqTexture.Image->GetType());
                unsigned int uiTypeEnum = GetImageType(rqTexture.Image->GetType());

                if ( uiFrmtEnum == 0 || uiTypeEnum == 0 )
                    return;

                if ( !rqTexture.BuildMipmaps )
                {
                    // without building of mipmaps
                    assert( !(QGLExists_GL_ARB_texture_non_power_of_two != QGL_EXT_FULLY_EXISTS
                        &&  !IsPowerOfTwo(iImageWidth)) );

                    if ( !rqTexture.SubTextureMode )
                    {
                        // regular
                        glTexImage1D(uiTrgtEnum,0,uiInrlFrmtEnum,iImageWidth,iBorder,uiFrmtEnum,uiTypeEnum,
                            *rqTexture.Image);
                    }
                    else
                    {
                        // subtexture
#ifdef _DEBUG
                        int iWidth;
                        glGetTexLevelParameteriv(uiTrgtEnum,0,GL_TEXTURE_WIDTH,&iWidth);
                        assert( rqTexture.SubTextureOffset.X + iImageWidth <= iWidth );
#endif
                        glTexSubImage1D(uiTrgtEnum,0,rqTexture.SubTextureOffset.X,iImageWidth,uiFrmtEnum,
                            uiTypeEnum,*rqTexture.Image);
                    }
                }
                else
                {
                    // build mipmaps
                    if ( !rqTexture.LimitMipmapsBuildingSet )
                    {
                        // full mipmap set
                        gluBuild1DMipmaps(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,uiFrmtEnum,uiTypeEnum,
                            *rqTexture.Image);
                    }
                    else
                    {
                        // limited mipmap set
                        gluBuild1DMipmapLevels(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,uiFrmtEnum,uiTypeEnum,
                            rqTexture.LMBLevel,rqTexture.LMBBaseLevel,rqTexture.LMBMaxLevel,*rqTexture.Image);
                    }
                }
            }
            else
            {
                // Precompressed.  Check on extension presence is already done by uiInrlFrmtEnum.
                if ( !rqTexture.SubTextureMode )
                {
                    // regular
                    for (int i = 0; i < rqTexture.CompressedTextureMipmapArray->GetQuantity(); i++)
                    {
                        const HRTextureBase::CompressedTextureMipmap& rqComprMM =
                            (*rqTexture.CompressedTextureMipmapArray)[i];

                        assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                            &&  rqComprMM.Height == 1 );

                        glCompressedTexImage1D(uiTrgtEnum,i,uiInrlFrmtEnum,rqComprMM.Width,iBorder,rqComprMM.Size,
                            *rqComprMM.Data);
                    }
                }
                else
                {
                    // subtexture
                    for (int i = 0; i < rqTexture.CompressedTextureMipmapArray->GetQuantity(); i++)
                    {
                        const HRTextureBase::CompressedTextureMipmap& rqComprMM =
                            (*rqTexture.CompressedTextureMipmapArray)[i];
                        const HRTextureBase::SubTextureOffsetRecord& rqSTOR = (*rqTexture.SubTextureOffsetArray)[i];

                        assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                            &&  rqComprMM.Height == 1 );
#ifdef _DEBUG
                        int iWidth;
                        glGetTexLevelParameteriv(uiTrgtEnum,i,GL_TEXTURE_WIDTH,&iWidth);
                        assert( rqSTOR.X + rqComprMM.Width <= iWidth );
#endif
                        glCompressedTexSubImage1D(uiTrgtEnum,i,rqSTOR.X,rqComprMM.Width,uiInrlFrmtEnum,
                            rqComprMM.Size,*rqComprMM.Data);
                    }
                }
            }
        }
        else if ( rqTexture.GeneralType == HRTextureBase::GTT_2D )
        {
            // two-dimensional
            if ( !rqTexture.CompressedTextureMipmapArray )
            {
                // not precompressed
                int iImageWidth, iImageHeight;

                if ( !rqTexture.SubRectSet )
                {
                    // no subrectangle
                    iImageWidth = rqTexture.Image->GetWidth();
                    iImageHeight = rqTexture.Image->GetHeight();
                }
                else
                {
                    // subrectangle
                    assert( rqTexture.SubRect.X + rqTexture.SubRect.Width <= rqTexture.Image->GetWidth()
                        &&  rqTexture.SubRect.Y + rqTexture.SubRect.Height <= rqTexture.Image->GetHeight() );

                    glPixelStorei(GL_UNPACK_ROW_LENGTH,rqTexture.Image->GetWidth());
                    glPixelStorei(GL_UNPACK_SKIP_PIXELS,rqTexture.SubRect.X);
                    glPixelStorei(GL_UNPACK_SKIP_ROWS,rqTexture.SubRect.Y);

                    iImageWidth = rqTexture.SubRect.Width;
                    iImageHeight = rqTexture.SubRect.Height;
                }

                assert( iImageWidth <= m_iMaxTextureSize && iImageHeight <= m_iMaxTextureSize );

                // format and type
                unsigned int uiFrmtEnum = GetImageFormat(rqTexture.Image->GetType());
                unsigned int uiTypeEnum = GetImageType(rqTexture.Image->GetType());

                if ( uiFrmtEnum == 0 || uiTypeEnum == 0 )
                    return;

                if ( !rqTexture.BuildMipmaps )
                {
                    // without building of mipmaps
                    assert( !(QGLExists_GL_ARB_texture_non_power_of_two != QGL_EXT_FULLY_EXISTS
                        &&  (!IsPowerOfTwo(iImageWidth)) || !IsPowerOfTwo(iImageHeight)) );

                    if ( !rqTexture.SubTextureMode )
                    {
                        // regular
                        glTexImage2D(uiTrgtEnum,0,uiInrlFrmtEnum,iImageWidth,iImageHeight,iBorder,uiFrmtEnum,
                            uiTypeEnum,*rqTexture.Image);
                    }
                    else
                    {
                        // subtexture
#ifdef _DEBUG
                        int iWidth, iHeight;
                        glGetTexLevelParameteriv(uiTrgtEnum,0,GL_TEXTURE_WIDTH,&iWidth);
                        glGetTexLevelParameteriv(uiTrgtEnum,0,GL_TEXTURE_HEIGHT,&iHeight);
                        assert( rqTexture.SubTextureOffset.X + iImageWidth <= iWidth
                            &&  rqTexture.SubTextureOffset.Y + iImageHeight <= iHeight );
#endif
                        glTexSubImage2D(uiTrgtEnum,0,rqTexture.SubTextureOffset.X,rqTexture.SubTextureOffset.Y,
                            iImageWidth,iImageHeight,uiFrmtEnum,uiTypeEnum,*rqTexture.Image);
                    }
                }
                else
                {
                    // build mipmaps
                    if ( !rqTexture.LimitMipmapsBuildingSet )
                    {
                        // full mipmap set
                        gluBuild2DMipmaps(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,uiFrmtEnum,uiTypeEnum,
                            *rqTexture.Image);
                    }
                    else
                    {
                        // limited mipmap set
                        gluBuild2DMipmapLevels(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,uiFrmtEnum,
                            uiTypeEnum,rqTexture.LMBLevel,rqTexture.LMBBaseLevel,rqTexture.LMBMaxLevel,
                            *rqTexture.Image);
                    }
                }
            }
            else
            {
                // Precompressed.  Check on extension presence is already done by uiInrlFrmtEnum.
                if ( !rqTexture.SubTextureMode )
                {
                    // regular
                    for (int i = 0; i < rqTexture.CompressedTextureMipmapArray->GetQuantity(); i++)
                    {
                        const HRTextureBase::CompressedTextureMipmap& rqComprMM =
                            (*rqTexture.CompressedTextureMipmapArray)[i];

                        assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                            &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );

                        glCompressedTexImage2D(uiTrgtEnum,i,uiInrlFrmtEnum,rqComprMM.Width,rqComprMM.Height,
                            iBorder,rqComprMM.Size,*rqComprMM.Data);
                    }
                }
                else
                {
                    // subtexture
                    for (int i = 0; i < rqTexture.CompressedTextureMipmapArray->GetQuantity(); i++)
                    {
                        const HRTextureBase::CompressedTextureMipmap& rqComprMM =
                            (*rqTexture.CompressedTextureMipmapArray)[i];
                        const HRTextureBase::SubTextureOffsetRecord& rqSTOR = (*rqTexture.SubTextureOffsetArray)[i];

                        assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                            &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );
#ifdef _DEBUG
                        int iWidth, iHeight;
                        glGetTexLevelParameteriv(uiTrgtEnum,i,GL_TEXTURE_WIDTH,&iWidth);
                        glGetTexLevelParameteriv(uiTrgtEnum,i,GL_TEXTURE_HEIGHT,&iHeight);
                        assert( rqSTOR.X + rqComprMM.Width <= iWidth && rqSTOR.Y + rqComprMM.Height <= iHeight );
#endif
                        glCompressedTexSubImage2D(uiTrgtEnum,i,rqSTOR.X,rqSTOR.Y,rqComprMM.Width,rqComprMM.Height,
                            uiInrlFrmtEnum,rqComprMM.Size,*rqComprMM.Data);
                    }
                }
            }
        }
        else if ( rqTexture.GeneralType == HRTextureBase::GTT_3D )
        {
            // three-dimensional
            if ( QGLExists_GL_EXT_texture3D == QGL_EXT_FULLY_EXISTS )
            {
                if ( !rqTexture.CompressedTextureMipmapArrayArray )
                {
                    // not precompressed
                    int iImageWidth, iImageHeight;
                    unsigned int uiFrmtEnum, uiTypeEnum;
                    for (int i = 0; i < rqTexture.ImageArray->GetQuantity(); i++)
                    {
                        const RasterImage& rqImage = *(*rqTexture.ImageArray)[i];

                        if ( !rqTexture.SubRectSet )
                        {
                            // no subrectangle
                            iImageWidth = rqImage.GetWidth();
                            iImageHeight = rqImage.GetHeight();
                        }
                        else
                        {
                            // subrectangle
                            const Recti& rqSubRect = (*rqTexture.SubRectArray)[i];

                            assert( rqSubRect.X + rqSubRect.Width <= rqImage.GetWidth()
                                &&  rqSubRect.Y + rqSubRect.Height <= rqImage.GetHeight() );

                            glPixelStorei(GL_UNPACK_ROW_LENGTH,rqImage.GetWidth());
                            glPixelStorei(GL_UNPACK_SKIP_PIXELS,rqSubRect.X);
                            glPixelStorei(GL_UNPACK_SKIP_ROWS,rqSubRect.Y);

                            iImageWidth = rqSubRect.Width;
                            iImageHeight = rqSubRect.Height;
                        }

                        assert( iImageWidth <= m_iMaxTextureSize && iImageHeight <= m_iMaxTextureSize );

                        // format and type
                        uiFrmtEnum = GetImageFormat(rqImage.GetType());
                        uiTypeEnum = GetImageType(rqImage.GetType());

                        if ( uiFrmtEnum == 0 || uiTypeEnum == 0 )
                            return;

                        if ( !rqTexture.BuildMipmaps )
                        {
                            // without building of mipmaps
                            assert( !(QGLExists_GL_ARB_texture_non_power_of_two != QGL_EXT_FULLY_EXISTS
                                &&  (!IsPowerOfTwo(iImageWidth)) || !IsPowerOfTwo(iImageHeight)) );

                            if ( !rqTexture.SubTextureMode )
                            {
                                // regular
                                glTexImage3D(uiTrgtEnum,0,uiInrlFrmtEnum,iImageWidth,iImageHeight,i,iBorder,
                                    uiFrmtEnum,uiTypeEnum,rqImage);
                            }
                            else
                            {
                                // subtexture
                                assert( rqTexture.ImageArray->GetQuantity() ==
                                        rqTexture.SubTextureOffsetArray->GetQuantity() );

                                const HRTextureBase::SubTextureOffsetRecord& rqSTOR =
                                    (*rqTexture.SubTextureOffsetArray)[i];
#ifdef _DEBUG
                                int iWidth, iHeight;
                                glGetTexLevelParameteriv(uiTrgtEnum,0,GL_TEXTURE_WIDTH,&iWidth);
                                glGetTexLevelParameteriv(uiTrgtEnum,0,GL_TEXTURE_HEIGHT,&iHeight);
                                assert( rqSTOR.X + iImageWidth <= iWidth && rqSTOR.Y + iImageHeight <= iHeight );
#endif
                                glTexSubImage3D(uiTrgtEnum,0,rqSTOR.X,rqSTOR.Y,rqSTOR.Z,iImageWidth,iImageHeight,
                                    i,uiFrmtEnum,uiTypeEnum,rqImage);
                            }
                        }
                        else
                        {
                            // build mipmaps
                            if ( !rqTexture.LimitMipmapsBuildingSet )
                            {
                                // full mipmap set
                                gluBuild3DMipmaps(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,i,uiFrmtEnum,
                                    uiTypeEnum,rqImage);
                            }
                            else
                            {
                                // limited mipmap set
                                gluBuild3DMipmapLevels(uiTrgtEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,i,
                                    uiFrmtEnum,uiTypeEnum,rqTexture.LMBLevel,rqTexture.LMBBaseLevel,
                                    rqTexture.LMBMaxLevel,rqImage);
                            }
                        }
                    }
                }
                else
                {
                    // Precompressed.  Check on extension presence is already done by uiInrlFrmtEnum.
                    if ( !rqTexture.SubTextureMode )
                    {
                        // regular
                        for (int i0 = 0; i0 < rqTexture.CompressedTextureMipmapArrayArray-> GetQuantity(); i0++)
                        {
                            const HRTextureBase::ComprMipmapArrayPtr& rspqComprMMArray =
                                (*rqTexture.CompressedTextureMipmapArrayArray)[i0];

                            for (int i1 = 0; i1 < rspqComprMMArray->GetQuantity(); i1++)
                            {
                                const HRTextureBase::CompressedTextureMipmap& rqComprMM = (*rspqComprMMArray)[i1];

                                assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                                    &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );

                                glCompressedTexImage3D(uiTrgtEnum,i1,uiInrlFrmtEnum,rqComprMM.Width,
                                    rqComprMM.Height,i0,iBorder,rqComprMM.Size,*rqComprMM.Data);
                            }
                        }
                    }
                    else
                    {
                        // subtexture
                        assert( rqTexture.CompressedTextureMipmapArrayArray->GetQuantity() ==
                                rqTexture.SubTextureOffsetArrayArray->GetQuantity() );

                        for (int i0 = 0; i0 < rqTexture.CompressedTextureMipmapArrayArray->GetQuantity(); i0++)
                        {
                            const HRTextureBase::ComprMipmapArrayPtr& rspqComprMMArray =
                                (*rqTexture.CompressedTextureMipmapArrayArray)[i0];
                            const HRTextureBase::SubTexOffsetRecArrayPtr& rspqSTORArray =
                                (*rqTexture.SubTextureOffsetArrayArray)[i0];

                            assert( rspqComprMMArray->GetQuantity() == rspqSTORArray->GetQuantity() );

                            for (int i1 = 0; i1 < rspqComprMMArray->GetQuantity(); i1++)
                            {
                                const HRTextureBase::CompressedTextureMipmap& rqComprMM = (*rspqComprMMArray)[i1];
                                const HRTextureBase::SubTextureOffsetRecord& rqSTOR = (*rspqSTORArray)[i1];

                                assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                                    &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );
#ifdef _DEBUG
                                int iWidth, iHeight;
                                glGetTexLevelParameteriv(uiTrgtEnum,i1,GL_TEXTURE_WIDTH,&iWidth);
                                glGetTexLevelParameteriv(uiTrgtEnum,i1,GL_TEXTURE_HEIGHT,&iHeight);
                                assert( rqSTOR.X + rqComprMM.Width <= iWidth
                                    &&  rqSTOR.Y + rqComprMM.Height <= iHeight );
#endif
                                glCompressedTexSubImage3D(uiTrgtEnum,i1,rqSTOR.X,rqSTOR.Y,rqSTOR.Z,
                                    rqComprMM.Width,rqComprMM.Height,i0,uiInrlFrmtEnum,rqComprMM.Size,
                                    *rqComprMM.Data);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            // cube map
            if ( QGLExists_GL_EXT_texture_cube_map == QGL_EXT_FULLY_EXISTS )
            {
                if ( !rqTexture.CompressedTextureMipmapArrayArray )
                {
                    // not precompressed
                    assert( rqTexture.ImageArray->GetQuantity() == 6 );

                    unsigned int uiSideEnum;
                    int iImageWidth, iImageHeight;
                    unsigned int uiFrmtEnum, uiTypeEnum;
                    for (int i = 0; i < 6; i++)
                    {
                        uiSideEnum = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

                        const RasterImage& rqImage = *(*rqTexture.ImageArray)[i];

                        if ( !rqTexture.SubRectSet )
                        {
                            // no subrectangle
                            iImageWidth = rqImage.GetWidth();
                            iImageHeight = rqImage.GetHeight();
                        }
                        else
                        {
                            // subrectangle
                            const Recti& rqSubRect = (*rqTexture.SubRectArray)[i];

                            assert( rqSubRect.X + rqSubRect.Width <= rqImage.GetWidth()
                                &&  rqSubRect.Y + rqSubRect.Height <= rqImage.GetHeight() );

                            glPixelStorei(GL_UNPACK_ROW_LENGTH,rqImage.GetWidth());
                            glPixelStorei(GL_UNPACK_SKIP_PIXELS,rqSubRect.X);
                            glPixelStorei(GL_UNPACK_SKIP_ROWS,rqSubRect.Y);

                            iImageWidth = rqSubRect.Width;
                            iImageHeight = rqSubRect.Height;
                        }

                        assert( iImageWidth <= m_iMaxTextureSize && iImageHeight <= m_iMaxTextureSize );

                        // format and type
                        uiFrmtEnum = GetImageFormat(rqImage.GetType());
                        uiTypeEnum = GetImageType(rqImage.GetType());

                        if ( uiFrmtEnum == 0 || uiTypeEnum == 0 )
                            return;

                        if ( !rqTexture.BuildMipmaps )
                        {
                            // without building of mipmaps
                            assert( !(QGLExists_GL_ARB_texture_non_power_of_two != QGL_EXT_FULLY_EXISTS
                                &&  (!IsPowerOfTwo(iImageWidth)) || !IsPowerOfTwo(iImageHeight)) );

                            if ( !rqTexture.SubTextureMode )
                            {
                                // regular
                                glTexImage2D(uiSideEnum,0,uiInrlFrmtEnum,iImageWidth,iImageHeight,iBorder,
                                    uiFrmtEnum,uiTypeEnum,rqImage);
                            }
                            else
                            {
                                // subtexture
                                assert( rqTexture.ImageArray->GetQuantity() ==
                                        rqTexture.SubTextureOffsetArray->GetQuantity() );

                                const HRTextureBase::SubTextureOffsetRecord& rqSTOR =
                                    (*rqTexture.SubTextureOffsetArray)[i];
#ifdef _DEBUG
                                int iWidth, iHeight;
                                glGetTexLevelParameteriv(uiSideEnum,0,GL_TEXTURE_WIDTH,&iWidth);
                                glGetTexLevelParameteriv(uiSideEnum,0,GL_TEXTURE_HEIGHT,&iHeight);
                                assert( rqSTOR.X + iImageWidth <= iWidth && rqSTOR.Y + iImageHeight <= iHeight );
#endif
                                glTexSubImage2D(uiSideEnum,0,rqSTOR.X,rqSTOR.Y,iImageWidth,iImageHeight,
                                    uiFrmtEnum,uiTypeEnum,rqImage);
                            }
                        }
                        else
                        {
                            // build mipmaps
                            if ( !rqTexture.LimitMipmapsBuildingSet )
                            {
                                // full mipmap set
                                gluBuild2DMipmaps(uiSideEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,uiFrmtEnum,
                                    uiTypeEnum,rqImage);
                            }
                            else
                            {
                                // limited mipmap set
                                gluBuild2DMipmapLevels(uiSideEnum,uiInrlFrmtEnum,iImageWidth,iImageHeight,
                                    uiFrmtEnum,uiTypeEnum,rqTexture.LMBLevel,rqTexture.LMBBaseLevel,
                                    rqTexture.LMBMaxLevel,rqImage);
                            }
                        }
                    }
                }
                else
                {
                    // Precompressed.  Check on extension presence is already done by uiInrlFrmtEnum.
                    assert( rqTexture.CompressedTextureMipmapArrayArray->GetQuantity() == 6 );

                    unsigned int uiSideEnum;
                    if ( !rqTexture.SubTextureMode )
                    {
                        // regular
                        for (int i0 = 0; i0 < 6; i0++)
                        {
                            uiSideEnum = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i0;

                            const HRTextureBase::ComprMipmapArrayPtr& rspqComprMMArray =
                                (*rqTexture.CompressedTextureMipmapArrayArray)[i0];

                            for (int i1 = 0; i1 < rspqComprMMArray->GetQuantity(); i1++)
                            {
                                const HRTextureBase::CompressedTextureMipmap& rqComprMM = (*rspqComprMMArray)[i1];

                                assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                                    &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );

                                glCompressedTexImage2D(uiSideEnum,i1,uiInrlFrmtEnum,rqComprMM.Width,
                                    rqComprMM.Height,iBorder,rqComprMM.Size,*rqComprMM.Data);
                            }
                        }
                    }
                    else
                    {
                        // subtexture
                        assert( rqTexture.SubTextureOffsetArrayArray->GetQuantity() == 6 );

                        for (int i0 = 0; i0 < 6; i0++)
                        {
                            uiSideEnum = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i0;

                            const HRTextureBase::ComprMipmapArrayPtr& rspqComprMMArray =
                                (*rqTexture.CompressedTextureMipmapArrayArray)[i0];
                            const HRTextureBase::SubTexOffsetRecArrayPtr& rspqSTORArray =
                                (*rqTexture.SubTextureOffsetArrayArray)[i0];

                            assert( rspqComprMMArray->GetQuantity() == rspqSTORArray->GetQuantity() );

                            for (int i1 = 0; i1 < rspqComprMMArray->GetQuantity(); i1++)
                            {
                                const HRTextureBase::CompressedTextureMipmap& rqComprMM = (*rspqComprMMArray)[i1];
                                const HRTextureBase::SubTextureOffsetRecord& rqSTOR = (*rspqSTORArray)[i1];

                                assert( 0 <= rqComprMM.Width && rqComprMM.Width <= m_iMaxTextureSize
                                    &&  0 <= rqComprMM.Height && rqComprMM.Height <= m_iMaxTextureSize );
#ifdef _DEBUG
                                int iWidth, iHeight;
                                glGetTexLevelParameteriv(uiSideEnum,i1,GL_TEXTURE_WIDTH,&iWidth);
                                glGetTexLevelParameteriv(uiSideEnum,i1,GL_TEXTURE_HEIGHT,&iHeight);
                                assert( rqSTOR.X + rqComprMM.Width <= iWidth
                                    &&  rqSTOR.Y + rqComprMM.Height <= iHeight );
#endif
                                glCompressedTexSubImage2D(uiSideEnum,i1,rqSTOR.X,rqSTOR.Y,rqComprMM.Width,
                                    rqComprMM.Height,uiInrlFrmtEnum,rqComprMM.Size,*rqComprMM.Data);
                            }
                        }
                    }
                }
            }
        }

        // priority
        if ( rqTexture.PrioritySet )
            glTexParameterf(uiTrgtEnum,GL_TEXTURE_PRIORITY,rqTexture.Priority);

        // perspective correction
        if ( rqTexture.CorrectionSet )
        {
            glHint(GL_PERSPECTIVE_CORRECTION_HINT,( rqTexture.Correction == HRTextureBase::CT_AFFINE ?
                GL_FASTEST : GL_NICEST ));
        }

        // wrapping
        if ( rqTexture.WrapSSet )
        {
            unsigned int uiWrapEnum = GetWrappingEnum(rqTexture.WrapS);
            if ( uiWrapEnum != 0 )
                glTexParameteri(uiTrgtEnum,GL_TEXTURE_WRAP_S,uiWrapEnum);
        }
        if ( rqTexture.WrapTSet )
        {
            unsigned int uiWrapEnum = GetWrappingEnum(rqTexture.WrapT);
            if ( uiWrapEnum != 0 )
                glTexParameteri(uiTrgtEnum,GL_TEXTURE_WRAP_T,uiWrapEnum);
        }
        if ( rqTexture.WrapRSet )
        {
            if ( QGLExists_GL_EXT_texture3D == QGL_EXT_FULLY_EXISTS )
            {
                unsigned int uiWrapEnum = GetWrappingEnum(rqTexture.WrapR);
                if ( uiWrapEnum != 0 )
                    glTexParameteri(uiTrgtEnum,GL_TEXTURE_WRAP_R,uiWrapEnum);
            }
        }

        // filtering
        if ( rqTexture.ShrinkFilterSet )
        {
            unsigned int uiFltrEnum;
            switch ( rqTexture.ShrinkFilter )
            {
            case HRTextureBase::SFT_NEAREST:
                uiFltrEnum = GL_NEAREST;
                break;
            case HRTextureBase::SFT_LINEAR:
                uiFltrEnum = GL_LINEAR;
                break;
            case HRTextureBase::SFT_MM_NEAREST_NEAREST:
                uiFltrEnum = GL_NEAREST_MIPMAP_NEAREST;
                break;
            case HRTextureBase::SFT_MM_NEAREST_LINEAR:
                uiFltrEnum = GL_NEAREST_MIPMAP_LINEAR;
                break;
            case HRTextureBase::SFT_MM_LINEAR_NEAREST:
                uiFltrEnum = GL_LINEAR_MIPMAP_NEAREST;
                break;
            case HRTextureBase::SFT_MM_LINEAR_LINEAR:
                uiFltrEnum = GL_LINEAR_MIPMAP_LINEAR;
                break;
            }
            glTexParameteri(uiTrgtEnum,GL_TEXTURE_MIN_FILTER,uiFltrEnum);
        }
        if ( rqTexture.EnlargeFilterSet )
        {
            unsigned int uiFltrEnum;
            switch ( rqTexture.EnlargeFilter )
            {
            case HRTextureBase::EFT_NEAREST:
                uiFltrEnum = GL_NEAREST;
                break;
            case HRTextureBase::EFT_LINEAR:
                uiFltrEnum = GL_LINEAR;
                break;
            }
            glTexParameteri(uiTrgtEnum,GL_TEXTURE_MAG_FILTER,uiFltrEnum);
        }

        // application
        if ( rqTexture.ApplySet )
        {
            unsigned int uiApplEnum;
            switch ( rqTexture.Apply )
            {
            case HRTextureBase::AT_REPLACE:
                uiApplEnum = GL_REPLACE;
                break;
            case HRTextureBase::AT_MODULATE:
                uiApplEnum = GL_MODULATE;
                break;
            case HRTextureBase::AT_DECAL:
                uiApplEnum = GL_DECAL;
                break;
            case HRTextureBase::AT_BLEND:
                uiApplEnum = GL_BLEND;
                break;
            case HRTextureBase::AT_ADD:
                uiApplEnum = ( QGLExists_GL_ARB_texture_env_add == QGL_EXT_FULLY_EXISTS ? GL_ADD : 0 );
                break;
            case HRTextureBase::AT_COMBINE:
                uiApplEnum = ( QGLExists_GL_ARB_texture_env_combine == QGL_EXT_FULLY_EXISTS ? GL_COMBINE : 0 );
                break;
            }
            if ( uiApplEnum != 0 )
                glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,uiApplEnum);
        }

        // border
        if ( rqTexture.BorderSet )
            glTexParameterfv(uiTrgtEnum,GL_TEXTURE_BORDER_COLOR,rqTexture.BorderColor);

        // depth texture related
        if ( QGLExists_GL_ARB_depth_texture == QGL_EXT_FULLY_EXISTS )
        {
            if ( rqTexture.DepthModeSet )
            {
                unsigned int uiDpthModeEnum;
                switch ( rqTexture.DepthMode )
                {
                case HRTextureBase::DMT_LUMINANCE:
                    uiDpthModeEnum = GL_LUMINANCE;
                    break;
                case HRTextureBase::DMT_INTENSITY:
                    uiDpthModeEnum = GL_INTENSITY;
                    break;
                case HRTextureBase::DMT_ALPHA:
                    uiDpthModeEnum = GL_ALPHA;
                    break;
                }
                glTexParameteri(uiTrgtEnum,GL_DEPTH_TEXTURE_MODE,uiDpthModeEnum);
            }

            if ( rqTexture.DepthCompareModeSet )
            {
                if ( QGLExists_GL_ARB_shadow == QGL_EXT_FULLY_EXISTS )
                {
                    unsigned int uiCmprModeEnum;
                    switch ( rqTexture.DepthCompareMode )
                    {
                    case HRTextureBase::DCMT_NONE:
                        uiCmprModeEnum = GL_NONE;
                        break;
                    case HRTextureBase::DCMT_COMPARE_R_TO_TEXTURE:
                        uiCmprModeEnum = GL_COMPARE_R_TO_TEXTURE;
                        break;
                    }
                    glTexParameteri(uiTrgtEnum,GL_TEXTURE_COMPARE_MODE,uiCmprModeEnum);
                }
            }

            if ( rqTexture.DepthCompareFunctionSet )
            {
                unsigned int uiCmprFuncEnum;
                switch ( rqTexture.DepthCompareFunction )
                {
                case HRTextureBase::DCFT_NEVER:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_NEVER : 0 );
                    break;
                case HRTextureBase::DCFT_ALWAYS:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_ALWAYS : 0 );
                    break;
                case HRTextureBase::DCFT_LESS:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_LESS : 0 );
                    break;
                case HRTextureBase::DCFT_LESS_OR_EQUAL:
                    uiCmprFuncEnum = ( QGLExists_GL_ARB_shadow == QGL_EXT_FULLY_EXISTS ? GL_LEQUAL : 0 );
                    break;
                case HRTextureBase::DCFT_EQUAL:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_EQUAL : 0 );
                    break;
                case HRTextureBase::DCFT_GREATER_OR_EQUAL:
                    uiCmprFuncEnum = ( QGLExists_GL_ARB_shadow == QGL_EXT_FULLY_EXISTS ? GL_GEQUAL : 0 );
                    break;
                case HRTextureBase::DCFT_GREATER:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_GREATER : 0 );
                    break;
                case HRTextureBase::DCFT_NOT_EQUAL:
                    uiCmprFuncEnum = ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS ? GL_NOTEQUAL : 0 );
                    break;
                }
                if ( uiCmprFuncEnum != 0 )
                    glTexParameteri(uiTrgtEnum,GL_TEXTURE_COMPARE_FUNC,uiCmprFuncEnum);
            }
        }

        // blending color
        if ( rqTexture.BlendingColorSet )
            glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,rqTexture.BlendingColor);

        // combining
        if ( QGLExists_GL_ARB_texture_env_combine == QGL_EXT_FULLY_EXISTS )
        {
            if ( rqTexture.CombineRgbFunctionSet )
            {
                unsigned int uiEnum;
                switch ( rqTexture.CombineRgbFunction )
                {
                case HRTextureBase::CRGBFT_REPLACE:
                    uiEnum = GL_REPLACE;
                    break;
                case HRTextureBase::CRGBFT_MODULATE:
                    uiEnum = GL_MODULATE;
                    break;
                case HRTextureBase::CRGBFT_ADD:
                    uiEnum = GL_ADD;
                    break;
                case HRTextureBase::CRGBFT_ADD_SIGNED:
                    uiEnum = GL_ADD_SIGNED;
                    break;
                case HRTextureBase::CRGBFT_INTERPOLATE:
                    uiEnum = GL_INTERPOLATE;
                    break;
                case HRTextureBase::CRGBFT_SUBTRACT:
                    uiEnum = GL_SUBTRACT;
                    break;
                case HRTextureBase::CRGBFT_DOT3_RGB:
                    uiEnum = ( QGLExists_GL_ARB_texture_env_dot3 == QGL_EXT_FULLY_EXISTS ? GL_DOT3_RGB : 0 );
                    break;
                case HRTextureBase::CRGBFT_DOT3_RGBA:
                    uiEnum = ( QGLExists_GL_ARB_texture_env_dot3 == QGL_EXT_FULLY_EXISTS ? GL_DOT3_RGBA : 0 );
                    break;
                }
                if ( uiEnum != 0 )
                    glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,uiEnum);
            }

            if ( rqTexture.CombineAlphaFunctionSet )
            {
                unsigned int uiEnum;
                switch ( rqTexture.CombineAlphaFunction )
                {
                case HRTextureBase::CAFT_REPLACE:
                    uiEnum = GL_REPLACE;
                    break;
                case HRTextureBase::CAFT_MODULATE:
                    uiEnum = GL_MODULATE;
                    break;
                case HRTextureBase::CAFT_ADD:
                    uiEnum = GL_ADD;
                    break;
                case HRTextureBase::CAFT_ADD_SIGNED:
                    uiEnum = GL_ADD_SIGNED;
                    break;
                case HRTextureBase::CAFT_INTERPOLATE:
                    uiEnum = GL_INTERPOLATE;
                    break;
                case HRTextureBase::CAFT_SUBTRACT:
                    uiEnum = GL_SUBTRACT;
                    break;
                }
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,uiEnum);
            }

            if ( rqTexture.CombineRgbArgument0Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceRgb0,
                    rqTexture.CrossbarTextureSourceRgb0);
                unsigned int uiOpndEnum = GetCombineOperandRgbEnum(rqTexture.CombineOperandRgb0);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_RGB,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineRgbArgument1Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceRgb1,
                    rqTexture.CrossbarTextureSourceRgb1);
                unsigned int uiOpndEnum = GetCombineOperandRgbEnum(rqTexture.CombineOperandRgb1);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_RGB,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineRgbArgument2Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceRgb2,
                    rqTexture.CrossbarTextureSourceRgb2);
                unsigned int uiOpndEnum = GetCombineOperandRgbEnum(rqTexture.CombineOperandRgb2);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_RGB,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineAlphaArgument0Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceAlpha0,
                    rqTexture.CrossbarTextureSourceAlpha0);
                unsigned int uiOpndEnum = GetCombineOperandAlphaEnum(rqTexture.CombineOperandAlpha0);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineAlphaArgument1Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceAlpha1,
                    rqTexture.CrossbarTextureSourceAlpha1);
                unsigned int uiOpndEnum = GetCombineOperandAlphaEnum(rqTexture.CombineOperandAlpha1);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_ALPHA,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineAlphaArgument2Set )
            {
                unsigned int uiSrcEnum = GetCombineSourceEnum(rqTexture.CombineSourceAlpha2,
                    rqTexture.CrossbarTextureSourceAlpha2);
                unsigned int uiOpndEnum = GetCombineOperandAlphaEnum(rqTexture.CombineOperandAlpha2);
                if ( uiSrcEnum != 0 && uiOpndEnum != 0 )
                {
                    glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,uiSrcEnum);
                    glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_ALPHA,uiOpndEnum);
                }
            }

            if ( rqTexture.CombineScaleRgbSet )
                glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE,GetCombineScaleValue(rqTexture.CombineScaleRgb));

            if ( rqTexture.CombineScaleAlphaSet )
                glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,GetCombineScaleValue(rqTexture.CombineScaleAlpha));
        }

        // mipmap autogeneration
        if ( QGLExists_GL_SGIS_generate_mipmap == QGL_EXT_FULLY_EXISTS )
        {
            if ( rqTexture.MipmapsAutoGenSet )
                glTexParameteri(uiTrgtEnum,GL_GENERATE_MIPMAP,rqTexture.MipmapsAutoGen);

            if ( rqTexture.MipmapsAutoGenHintSet )
            {
                unsigned int uiHintEnum;
                switch ( rqTexture.MipmapsAutoGenHint )
                {
                case HRTextureBase::MAGHT_FASTEST:
                    uiHintEnum = GL_FASTEST;
                    break;
                case HRTextureBase::MAGHT_NICEST:
                    uiHintEnum = GL_NICEST;
                    break;
                }
                glHint(GL_GENERATE_MIPMAP_HINT,uiHintEnum);
            }
        }

        // mipmap level-of-detail
        if ( QGLExists_GL_SGIS_texture_lod == QGL_EXT_FULLY_EXISTS )
        {
            if ( rqTexture.ClampLambdaSet )
            {
                glTexParameterf(uiTrgtEnum,GL_TEXTURE_MIN_LOD,rqTexture.MinLod);
                glTexParameterf(uiTrgtEnum,GL_TEXTURE_MAX_LOD,rqTexture.MaxLod);
            }
            if ( rqTexture.ClampMipmapLevelsSet )
            {
                assert( rqTexture.CMLBaseLevel >= 0 && rqTexture.CMLMaxLevel >= 0 );

                glTexParameteri(uiTrgtEnum,GL_TEXTURE_BASE_LEVEL,rqTexture.CMLBaseLevel);
                glTexParameteri(uiTrgtEnum,GL_TEXTURE_MAX_LEVEL,rqTexture.CMLMaxLevel);
            }
        }
        if ( rqTexture.LodBiasSet )
        {
            if ( QGLExists_GL_VERSION_1_4 == QGL_EXT_FULLY_EXISTS )
                glTexParameterf(uiTrgtEnum,GL_TEXTURE_LOD_BIAS,rqTexture.LodBias);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateTexture (TextureStatef* pqTextureState)
{
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();

    for (int i = 0; i < pqTextureState->GetTextureQuantity(); i++)
    {
        if ( !(QGLExists_GL_ARB_multitexture != QGL_EXT_FULLY_EXISTS && i > 0)
        &&   !(QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS && i == m_iMaxTextureCoords) )
        {
            HRTexturef& rqTexture = *pqTextureState->GetTexture(i);

            // global texture state is not allowed, so privately using all texture enumerator range
            if ( pqTextureState->GetTextureQuantity() > 1 )
                glActiveTexture(GL_TEXTURE0+i);

            StateTextureUnit(rqTexture);

            // Floating-point precision specific (or probability of it).

            // transformation
            glLoadMatrixf(rqTexture.GetTransformationMatrix());

            // texture coordinate generation
            if ( rqTexture.TexCoordGenSSet )
            {
                glEnable(GL_TEXTURE_GEN_S);

                switch ( rqTexture.TexCoordGenS )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGenfv(GL_S,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsS);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGenfv(GL_S,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsS);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenTSet )
            {
                glEnable(GL_TEXTURE_GEN_T);

                switch ( rqTexture.TexCoordGenT )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGenfv(GL_T,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsT);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGenfv(GL_T,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsT);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenRSet )
            {
                glEnable(GL_TEXTURE_GEN_R);

                switch ( rqTexture.TexCoordGenR )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGenfv(GL_R,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsR);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGenfv(GL_R,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsR);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenQSet )
            {
                glEnable(GL_TEXTURE_GEN_Q);

                switch ( rqTexture.TexCoordGenQ )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGenfv(GL_Q,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsQ);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGenfv(GL_Q,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsQ);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    glPopMatrix();

    // point sprite
    if ( pqTextureState->PointSpriteSet )
    {
        if ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS )
        {
            glEnable(GL_POINT_SPRITE);

            unsigned int uiEnum;
            switch ( pqTextureState->PointSpriteCoordOrigin )
            {
            case TextureStatef::PSCOT_LOWER_LEFT:
                uiEnum = GL_LOWER_LEFT;
                break;
            case TextureStatef::PSCOT_UPPER_LEFT:
                uiEnum = GL_UPPER_LEFT;
                break;
            }
            glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN,uiEnum);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::StateTexture (TextureStated* pqTextureState)
{
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();

    for (int i = 0; i < pqTextureState->GetTextureQuantity(); i++)
    {
        if ( !(QGLExists_GL_ARB_multitexture != QGL_EXT_FULLY_EXISTS && i > 0)
        &&   !(QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS && i == m_iMaxTextureCoords) )
        {
            HRTextured& rqTexture = *pqTextureState->GetTexture(i);

            // global texture state is not allowed, so privately using all texture enumerator range
            if ( pqTextureState->GetTextureQuantity() > 1 )
                glActiveTexture(GL_TEXTURE0+i);

            StateTextureUnit(rqTexture);

            // Floating-point precision specific (or probability of it).

            // transformation
            glLoadMatrixd(rqTexture.GetTransformationMatrix());

            // texture coordinate generation
            if ( rqTexture.TexCoordGenSSet )
            {
                glEnable(GL_TEXTURE_GEN_S);

                switch ( rqTexture.TexCoordGenS )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGendv(GL_S,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsS);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGendv(GL_S,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsS);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenTSet )
            {
                glEnable(GL_TEXTURE_GEN_T);

                switch ( rqTexture.TexCoordGenT )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGendv(GL_T,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsT);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGendv(GL_T,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsT);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenRSet )
            {
                glEnable(GL_TEXTURE_GEN_R);

                switch ( rqTexture.TexCoordGenR )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGendv(GL_R,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsR);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGendv(GL_R,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsR);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
            if ( rqTexture.TexCoordGenQSet )
            {
                glEnable(GL_TEXTURE_GEN_Q);

                switch ( rqTexture.TexCoordGenQ )
                {
                case HRTextureBase::TCGT_LOCAL_PLANE:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
                    glTexGendv(GL_Q,GL_OBJECT_PLANE,rqTexture.TexCoordGenPlaneParamsQ);
                    break;
                case HRTextureBase::TCGT_EYE_PLANE:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
                    glTexGendv(GL_Q,GL_EYE_PLANE,rqTexture.TexCoordGenPlaneParamsQ);
                    break;
                case HRTextureBase::TCGT_SPHERE_MAP:
                    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
                    break;
                case HRTextureBase::TCGT_REFLECTION_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
                    break;
                case HRTextureBase::TCGT_NORMAL_MAP:
                    if ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS )
                        glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    glPopMatrix();

    // point sprite
    if ( pqTextureState->PointSpriteSet )
    {
        if ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS )
        {
            glEnable(GL_POINT_SPRITE);

            unsigned int uiEnum;
            switch ( pqTextureState->PointSpriteCoordOrigin )
            {
            case TextureStatef::PSCOT_LOWER_LEFT:
                uiEnum = GL_LOWER_LEFT;
                break;
            case TextureStatef::PSCOT_UPPER_LEFT:
                uiEnum = GL_UPPER_LEFT;
                break;
            }
            glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN,uiEnum);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void OpenGLRendering::OnBufferableLeave (HRBufferable& rqBufferable)
{
    if ( rqBufferable.BufferId != 0 )
    {
        if ( rqBufferable.IsExactly(HRGeometryf::Class)
        ||   rqBufferable.IsExactly(HRGeometryd::Class) )
        {
            // geometry
            glDeleteLists(rqBufferable.BufferId,1);
        }
        else
        {
            // texture
            glDeleteTextures(1,&rqBufferable.BufferId);
        }
        rqBufferable.BufferId = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------



