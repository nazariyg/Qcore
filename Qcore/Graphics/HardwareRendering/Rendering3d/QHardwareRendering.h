#ifndef QHARDWARERENDERING_H
#define QHARDWARERENDERING_H

// Rendering frustum is orthogonal.  The class is dedicated to keeping of one or more rendering contexts with
// the same framebuffer parameters for a single scene.  Only one instance of the class' derivative can exist at
// an instant.

#include "QFramebufferParams.h"
#include "QLogable.h"
#include "QHRCamera3.h"
#include "QHRImage.h"
#include "QHRNode.h"

namespace Q
{

class Q_ITEM HardwareRendering : public Logable
{
// construction and destruction
protected:
    HardwareRendering (const FramebufferParams& rqRequestedFramebufferParams);
    ~HardwareRendering ();

public:
    // framebuffer queries
    const FramebufferParams& GetRequestedFramebufferParams () const;
    const FramebufferParams& GetPresentFramebufferParams () const;

    // non-context-specific options
    void SetDoVerticalSync (bool bDo);
    bool GetDoVerticalSync () const;

    // Viewport.  Initially is the origin-rectangle (0,0,0,0).
    virtual void SetViewport (int iX, int iY, int iWidth, int iHeight) = 0;
    virtual void SetViewport (const Recti& rqViewport) = 0;
    virtual const Recti& GetViewport () const = 0;

    // Framebuffer clearance.  Initially color and depth buffers are set to be cleared at the start of each
    // frame.
    virtual void SetDoColorBufferClear (bool bDo) = 0;
    virtual bool GetDoColorBufferClear () const = 0;
    virtual void SetDoDepthBufferClear (bool bDo) = 0;
    virtual bool GetDoDepthBufferClear () const = 0;

    // Background color.  Initially black (0,0,0,1).
    virtual void SetBackgroundColor (const ColorRgba& rqColor) = 0;
    virtual const ColorRgba& GetBackgroundColor () const = 0;

    // Depth buffer clear value.  Initially 1.0.
    virtual void SetDepthBufferClearValue (float fValue) = 0;
    virtual float GetDepthBufferClearValue () const = 0;

    // Camera.  Setting of camera of one type causes equating of another-type camera's smart pointer to 0.
    virtual void SetCameraf (const HRCamera3fPtr& rspqCamera) = 0;
    virtual const HRCamera3fPtr& GetCameraf () const = 0;
    virtual void SetCamerad (const HRCamera3dPtr& rspqCamera) = 0;
    virtual const HRCamera3dPtr& GetCamerad () const = 0;

    // global states
    virtual int GetGlobalStateQuantity () const = 0;
    virtual int BindGlobalState (const SmartRttiPointer& rspqState) = 0;
    virtual const SmartRttiPointer& GetGlobalState (int i) const = 0;
    virtual void UnbindGlobalState (int i) = 0;

    // pre- and post-draw calls
    virtual void BeginFrame () = 0;
    virtual void EndFrame () = 0;

    // drawing a geometry
    virtual void DrawGeometry (const HRGeometryfPtr& rspqGeometry, bool bBufferOnly = false) = 0;
    virtual void DrawGeometry (const HRGeometrydPtr& rspqGeometry, bool bBufferOnly = false) = 0;

    // drawing a raster image
    virtual void DrawImage (const HRImagefPtr& rspqImage, bool bBufferOnly = false) = 0;
    virtual void DrawImage (const HRImagedPtr& rspqImage, bool bBufferOnly = false) = 0;

    // access root nodes
    const HRNodefPtr& GetRootNodef () const;
    const HRNodedPtr& GetRootNoded () const;

    // drawing root nodes
    virtual void DrawScene (bool bBufferOnly = false) = 0;

    // availability of extensions
    virtual bool IsBlendColorPresent () const = 0;
    virtual bool IsBlendEquationSeparatePresent () const = 0;
    virtual bool IsBlendFuncSeparatePresent () const = 0;
    virtual bool IsBlendLogicOpPresent () const = 0;
    virtual bool IsBlendMinmaxPresent () const = 0;
    virtual bool IsBlendSquarePresent () const = 0;
    virtual bool IsBlendSubtractPresent () const = 0;
    virtual bool IsColorMatrixPresent () const = 0;
    virtual bool IsColorTablePresent () const = 0;
    virtual bool IsConvolutionPresent () const = 0;
    virtual bool IsDepthTexturePresent () const = 0;
    virtual bool IsFogCoordPresent () const = 0;
    virtual bool IsGenerateMipmapPresent () const = 0;
    virtual bool IsMultisamplePresent () const = 0;
    virtual bool IsMultitexturePresent () const = 0;
    virtual bool IsPackedPixelsPresent () const = 0;
    virtual bool IsPointParametersPresent () const = 0;
    virtual bool IsPointSpritePresent () const = 0;
    virtual bool IsSecondaryColorPresent () const = 0;
    virtual bool IsSeparateSpecularColorPresent () const = 0;
    virtual bool IsSeparateStencilPresent () const = 0;
    virtual bool IsShadowFuncsPresent () const = 0;
    virtual bool IsShadowPresent () const = 0;
    virtual bool IsStencilMaskSeparatePresent () const = 0;
    virtual bool IsTexgenReflectionPresent () const = 0;
    virtual bool IsTexParameterLodBiasPresent () const = 0;
    virtual bool IsTexture3dPresent () const = 0;
    virtual bool IsTextureBorderClampPresent () const = 0;
    virtual bool IsTextureCompressionFxt1Present () const = 0;
    virtual bool IsTextureCompressionPresent () const = 0;
    virtual bool IsTextureCompressionS3tcPresent () const = 0;
    virtual bool IsTextureCubeMapPresent () const = 0;
    virtual bool IsTextureEdgeClampPresent () const = 0;
    virtual bool IsTextureEnvAddPresent () const = 0;
    virtual bool IsTextureEnvCombinePresent () const = 0;
    virtual bool IsTextureEnvCrossbarPresent () const = 0;
    virtual bool IsTextureEnvDot3Present () const = 0;
    virtual bool IsTextureLodPresent () const = 0;
    virtual bool IsTextureMirroredRepeatPresent () const = 0;
    virtual bool IsTextureNonPowerOfTwoPresent () const = 0;
    virtual bool IsTypeUnsignedShort565Present () const = 0;
    virtual bool IsWindowPosPresent () const = 0;

    // main limitations
    int GetMaxLights () const;
    int GetMaxClippingPlanes () const;
    int GetMaxTextureSize () const;
    int GetMaxModelViewStackSize () const;
    int GetMaxAttribStackSize () const;
    int GetMaxPixelMapTableSize () const;
    int GetMaxTextureCoords () const;
    int GetMaxConvolutionFilterWidth () const;
    int GetMaxConvolutionFilterHeight () const;

protected:
    virtual void CheckForErrors () = 0;

    // framebuffer
    FramebufferParams m_qRequestedFramebufferParams;
    FramebufferParams m_qPresentFramebufferParams;

    // non-context-specific options
    bool m_bDoVerticalSync;     // default: false

    bool m_bInitialized;        // default: false

    int m_iMaxLights;
    int m_iMaxClippingPlanes;
    int m_iMaxTextureSize;
    int m_iMaxModelViewStackSize;
    int m_iMaxAttribStackSize;
    int m_iMaxPixelMapTableSize;
    int m_iMaxTextureCoords;
    int m_iMaxConvolutionFilterWidth;
    int m_iMaxConvolutionFilterHeight;

    HRNodefPtr m_qRootNodef;
    HRNodedPtr m_qRootNoded;

private:
    friend class HRBufferable;

    template <class Real>
    friend class HRNode;

    template <class Real>
    friend class TextureState;

    friend class StateSensitive;

    // to be used by unrelated classes
    virtual void OnBufferableLeave (HRBufferable& rqBufferable) = 0;

    static HardwareRendering* ms_pqItself;
};

#include "QHardwareRendering.inl"

}

#endif



