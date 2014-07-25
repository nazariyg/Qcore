#ifndef QOPENGLRENDERING_H
#define QOPENGLRENDERING_H

#ifdef _WIN32
#include "QWglExtensions.h"
#endif

#include "QHardwareRendering.h"
#include "QMap.h"

// states to operate with
#include "QAlphaTestState.h"
#include "QBlendingState.h"
#include "QClippingPlanesState.h"
#include "QColorMaskState.h"
#include "QColorState.h"
#include "QCullFaceState.h"
#include "QDepthMaskState.h"
#include "QDepthRangeState.h"
#include "QDepthTestState.h"
#include "QFogState.h"
#include "QFrontFaceState.h"
#include "QLightState.h"
#include "QLinesAntialiasingState.h"
#include "QLineStippleState.h"
#include "QLineWidthState.h"
#include "QLogicOperationState.h"
#include "QMaterialState.h"
#include "QMultisampleAntialiasingState.h"
#include "QNormalState.h"
#include "QPixelTransferState.h"
#include "QPointParameterState.h"
#include "QPointsAntialiasingState.h"
#include "QPointSizeState.h"
#include "QPolygonModeState.h"
#include "QPolygonOffsetState.h"
#include "QPolygonStippleState.h"
#include "QProjectionState.h"
#include "QScissorState.h"
#include "QSecondaryColorState.h"
#include "QStencilMaskState.h"
#include "QStencilTestState.h"

namespace Q
{

class Q_ITEM OpenGLRendering : public HardwareRendering, public Heap
{
public:
    // construction
    OpenGLRendering (const FramebufferParams& rqRequestedFramebufferParams);

#ifdef _WIN32
    // Initialization.  To be called after an object is constructed and needed variables are set.  The initial
    // window gets 1 as rendering context identifier and is set as current.
    bool Initialize (HWND hWnd, bool (*oRecreateWindowFunction)(HWND& rhWnd) = 0);

    // Adding of a new context.  A new context automatically begins to share display lists with already
    // existing contexts.  Parameter riId on return designates one-based context identifier given.
    bool AddContext (HWND hWnd, int& riId);

    // Setting of a current context and, alas, reinitializing of extensions.  The function also may initialize
    // multisampling accompanied by window recreation (another sadness).
    bool SetCurrentContext (int iId, bool (*oRecreateWindowFunction)(HWND& rhWnd) = 0);
#endif

    // destruction
    ~OpenGLRendering ();

    // Contexts.
    int GetContextQuantity () const;

    // for use with one or more contexts stored
    int GetCurrentContext () const;

    // remove the context and update index of current context if the removed was current
    bool RemoveContext (int iId);

    // OpenGL's vendor, versions, and extension information
    const String& GetGLVendorInfo () const;
    const String& GetGLRendererInfo () const;
    const String& GetGLVersionInfo () const;
    const String& GetGLExtensionInfo () const;

    // Viewport.  Initially is the origin-rectangle (0,0,0,0).
    virtual void SetViewport (int iX, int iY, int iWidth, int iHeight);
    virtual void SetViewport (const Recti& rqViewport);
    virtual const Recti& GetViewport () const;

    // Framebuffer clearance.  Initially color and depth buffers are set to be cleared at the start of each
    // frame.
    virtual void SetDoColorBufferClear (bool bDo);
    virtual bool GetDoColorBufferClear () const;
    virtual void SetDoDepthBufferClear (bool bDo);
    virtual bool GetDoDepthBufferClear () const;

    // Background color.  Initially black (0,0,0,1).
    virtual void SetBackgroundColor (const ColorRgba& rqColor);
    virtual const ColorRgba& GetBackgroundColor () const;

    // Depth buffer clear value.  Initially 1.0.
    virtual void SetDepthBufferClearValue (float fValue);
    virtual float GetDepthBufferClearValue () const;

    // Camera.  Setting of camera of one type causes equating of another-type camera's smart pointer to 0.
    virtual void SetCameraf (const HRCamera3fPtr& rspqCamera);
    virtual const HRCamera3fPtr& GetCameraf () const;
    virtual void SetCamerad (const HRCamera3dPtr& rspqCamera);
    virtual const HRCamera3dPtr& GetCamerad () const;

    // global states
    virtual int GetGlobalStateQuantity () const;
    virtual int BindGlobalState (const SmartRttiPointer& rspqState);
    virtual const SmartRttiPointer& GetGlobalState (int i) const;
    virtual void UnbindGlobalState (int i);

    // pre- and post-draw calls
    virtual void BeginFrame ();
    virtual void EndFrame ();

    // drawing a geometry
    virtual void DrawGeometry (const HRGeometryfPtr& rspqGeometry, bool bBufferOnly = false);
    virtual void DrawGeometry (const HRGeometrydPtr& rspqGeometry, bool bBufferOnly = false);

    // drawing a raster image
    virtual void DrawImage (const HRImagefPtr& rspqImage, bool bBufferOnly = false);
    virtual void DrawImage (const HRImagedPtr& rspqImage, bool bBufferOnly = false);

    // drawing root nodes
    virtual void DrawScene (bool bBufferOnly = false);

    // availability of extensions
    virtual bool IsBlendColorPresent () const;
    virtual bool IsBlendEquationSeparatePresent () const;
    virtual bool IsBlendFuncSeparatePresent () const;
    virtual bool IsBlendLogicOpPresent () const;
    virtual bool IsBlendMinmaxPresent () const;
    virtual bool IsBlendSquarePresent () const;
    virtual bool IsBlendSubtractPresent () const;
    virtual bool IsColorMatrixPresent () const;
    virtual bool IsColorTablePresent () const;
    virtual bool IsConvolutionPresent () const;
    virtual bool IsDepthTexturePresent () const;
    virtual bool IsFogCoordPresent () const;
    virtual bool IsGenerateMipmapPresent () const;
    virtual bool IsMultisamplePresent () const;
    virtual bool IsMultitexturePresent () const;
    virtual bool IsPackedPixelsPresent () const;
    virtual bool IsPointParametersPresent () const;
    virtual bool IsPointSpritePresent () const;
    virtual bool IsSecondaryColorPresent () const;
    virtual bool IsSeparateSpecularColorPresent () const;
    virtual bool IsSeparateStencilPresent () const;
    virtual bool IsShadowFuncsPresent () const;
    virtual bool IsShadowPresent () const;
    virtual bool IsStencilMaskSeparatePresent () const;
    virtual bool IsTexgenReflectionPresent () const;
    virtual bool IsTexParameterLodBiasPresent () const;
    virtual bool IsTexture3dPresent () const;
    virtual bool IsTextureBorderClampPresent () const;
    virtual bool IsTextureCompressionFxt1Present () const;
    virtual bool IsTextureCompressionPresent () const;
    virtual bool IsTextureCompressionS3tcPresent () const;
    virtual bool IsTextureCubeMapPresent () const;
    virtual bool IsTextureEdgeClampPresent () const;
    virtual bool IsTextureEnvAddPresent () const;
    virtual bool IsTextureEnvCombinePresent () const;
    virtual bool IsTextureEnvCrossbarPresent () const;
    virtual bool IsTextureEnvDot3Present () const;
    virtual bool IsTextureLodPresent () const;
    virtual bool IsTextureMirroredRepeatPresent () const;
    virtual bool IsTextureNonPowerOfTwoPresent () const;
    virtual bool IsTypeUnsignedShort565Present () const;
    virtual bool IsWindowPosPresent () const;

protected:
    virtual void CheckForErrors ();

private:
    class PushedStatesRecord
    {
    public:
        PushedStatesRecord (bool bGlobal)
            :
            Global(bGlobal),
            Quantity(0),
            ColorBufferAttribGroup(false),
            TransformAttribGroup(false),
            CurrentAttribGroup(false),
            PolygonAttribGroup(false),
            DepthBufferAttribGroup(false),
            ViewportAttribGroup(false),
            FogAttribGroup(false),
            HintAttribGroup(false),
            LightningAttribGroup(false),
            LineAttribGroup(false),
            MultisampleAttribGroup(false),
            PixelAttribGroup(false),
            PointAttribGroup(false),
            ProjectionMatrix(false),
            ScissorAttribGroup(false),
            StencilBufferAttribGroup(false)
        {
        };

        bool Global;
        int Quantity;
        bool ColorBufferAttribGroup;
        bool TransformAttribGroup;
        bool CurrentAttribGroup;
        bool PolygonAttribGroup;
        bool DepthBufferAttribGroup;
        bool ViewportAttribGroup;
        bool FogAttribGroup;
        bool HintAttribGroup;
        bool LightningAttribGroup;
        bool LineAttribGroup;
        bool MultisampleAttribGroup;
        bool PixelAttribGroup;
        bool PointAttribGroup;
        bool ProjectionMatrix;
        bool ScissorAttribGroup;
        bool StencilBufferAttribGroup;
    };

    // buffer swapping
    void SwapBuffers ();

    // drawing of nodes
    void DrawNode (const HRNodefPtr& rspqNode, bool bBufferOnly);
    void DrawNode (const HRNodedPtr& rspqNode, bool bBufferOnly);

    // helper functions on a model-view matrix stack
    void PushModelViewMatrixAndMultf (const Matrix4f& rqMatrix);
    void PopModelViewMatrixf ();
    void PushModelViewMatrixAndMultd (const Matrix4d& rqMatrix);
    void PopModelViewMatrixd ();

    // texture and pixel-store attribute groups can be managed only by these two functions
    void ActivateTextureState (const StateSensitive& rqStateSensitive);
    void DeactivateTextureState ();

    // general global/local states
    void ActivateStates (const StateSensitive& rqStateSensitive, PushedStatesRecord& rqPushedStates);
    void DeactivateStates (PushedStatesRecord& rqPushedStates);

    unsigned int AutodetectImageInternalFormat (RasterImage::ImageType eImageType, bool bStoreCompressed) const;
    unsigned int GetImageFormat (RasterImage::ImageType eImageType) const;
    unsigned int GetImageType (RasterImage::ImageType eImageType) const;
    unsigned int GetColorTablesInternalFormatEnum (PixelTransferState::ColorTablesInternalFormatType eType) const;
    unsigned int GetWrappingEnum (HRTextureBase::WrapType eType) const;
    unsigned int GetCombineSourceEnum (HRTextureBase::CombineSourceType eType, int iCrossbarTexture) const;
    unsigned int GetCombineOperandRgbEnum (HRTextureBase::CombineOperandRgbType eType) const;
    unsigned int GetCombineOperandAlphaEnum (HRTextureBase::CombineOperandAlphaType eType) const;
    float GetCombineScaleValue (HRTextureBase::CombineScaleType eType) const;

    void StateAlphaTest (AlphaTestState* pqAlphaTestState) const;
    void StateBlending (BlendingState* pqBlendingState) const;
    void StateClippingPlanes (ClippingPlanesState* pqClippingPlanesState, bool bGlobal) const;
    void StateColorMask (ColorMaskState* pqColorMaskState) const;
    void StateColor (ColorState* pqColorState) const;
    void StateCullFace (CullFaceState* pqCullFaceState) const;
    void StateDepthMask (DepthMaskState* pqDepthMaskState) const;
    void StateDepthRange (DepthRangeState* pqDepthRangeState) const;
    void StateDepthTest (DepthTestState* pqDepthTestState) const;
    void StateFog (FogState* pqFogState) const;
    void StateFrontFace (FrontFaceState* pqFrontFaceState) const;
    void StateLight (LightState* pqLightState, bool bGlobal);
    void StateLinesAntialiasing (LinesAntialiasingState* pqLinesAntialiasingState) const;
    void StateLineStipple (LineStippleState* pqLineStippleState) const;
    void StateLineWidth (LineWidthState* pqLineWidthState) const;
    void StateLogicOperation (LogicOperationState* pqLogicOperationState) const;
    void StateMaterial (MaterialState* pqMaterialState) const;
    void StateMultisampleAntialiasing (MultisampleAntialiasingState* pqMultisampleAntialiasingState) const;
    void StateNormal (NormalStatef* pqNormalStatef) const;
    void StateNormal (NormalStated* pqNormalStated) const;
    void StatePixelTransfer (PixelTransferState* pqPixelTransferState) const;
    void StatePointParameter (PointParameterState* pqPointParameterState) const;
    void StatePointsAntialiasing (PointsAntialiasingState* pqPointsAntialiasingState) const;
    void StatePointSize (PointSizeState* pqPointSizeState) const;
    void StatePolygonMode (PolygonModeState* pqPolygonModeState) const;
    void StatePolygonOffset (PolygonOffsetState* pqPolygonOffsetState) const;
    void StatePolygonStipple (PolygonStippleState* pqPolygonStippleState) const;
    void StateProjection (ProjectionStatef* pqProjectionState) const;
    void StateProjection (ProjectionStated* pqProjectionState) const;
    void StateScissor (ScissorState* pqScissorState) const;
    void StateSecondaryColor (SecondaryColorState* pqSecondaryColorState) const;
    void StateStencilMask (StencilMaskState* pqStencilMaskState) const;
    void StateStencilTest (StencilTestState* pqStencilTestState) const;
    void StateTextureUnit (HRTextureBase& rqTexture);
    void StateTexture (TextureStatef* pqTextureState);
    void StateTexture (TextureStated* pqTextureState);

    // to be used by unrelated classes
    virtual void OnBufferableLeave (HRBufferable& rqBufferable);

    int m_iPushedModelViewMatrixQuantity;                       // default: 2
    ModestArray<Matrix4f> m_qSecondEchelonModelViewStackf;      // default: of size 1
    ModestArray<Matrix4d> m_qSecondEchelonModelViewStackd;      // default: of size 1

    StateSensitive m_qGlobalStates;

    bool m_bTextureStateActivated;                              // default: false
    int m_iPrevTexCorrHint;
    int m_iPrevMMGenHint;
    bool m_bPrevPointSpriteEnabled;
    int m_iPrevPointSpriteCoordOrigin;
    bool m_bTexture1dEnabled;                                   // default: false
    bool m_bTexture2dEnabled;                                   // default: false
    bool m_bTexture3dEnabled;                                   // default: false
    bool m_bTextureCubeMapEnabled;                              // default: false

    PushedStatesRecord m_qPushedStatesGlobal;
    PushedStatesRecord m_qPushedStatesLocal;

#ifdef _WIN32
    class ContextRecord
    {
    public:
        ContextRecord () {};
        ContextRecord (HWND hWnd, HDC hDC, HGLRC hRC)
            :
            Wnd(hWnd),
            DC(hDC),
            RC(hRC),
            Viewport(Recti(0,0,0,0)),
            DoColorBufferClear(true),
            DoDepthBufferClear(true),
            BackgroundColor(ColorRgba::Black),
            DepthBufferClearValue(1.0f),
            MultisampleInitialized(false),
            Cameraf(new HRCamera3f(Vector3f::ZERO,Matrix3f::IDENTITY,45.0f,1.0f,2.0f,100.0f)),
            Camerad(0)
        {
        };

        HWND Wnd;
        HDC DC;
        HGLRC RC;
        Recti Viewport;
        bool DoColorBufferClear;
        bool DoDepthBufferClear;
        ColorRgba BackgroundColor;
        float DepthBufferClearValue;
        bool MultisampleInitialized;
        HRCamera3fPtr Cameraf;
        HRCamera3dPtr Camerad;
    };

    bool m_bEnterMultisample;
    int m_iMultisamplePixelFormat;
    Map<int,ContextRecord> m_qIdToContext;
#endif

    static unsigned int ms_uiNextContextId;
    int m_iCurrentContextId;
    ContextRecord* m_pqCurrentContext;
    String m_qGLVendorInfo;
    String m_qGLRendererInfo;
    String m_qGLVersionInfo;
    String m_qGLExtensionInfo;
};

#include "QOpenGLRendering.inl"

typedef SmartPointer<OpenGLRendering> OpenGLRenderingPtr;

}

#endif



