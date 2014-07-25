#ifndef QHRTEXTURE_H
#define QHRTEXTURE_H

#include "QHRTextureBase.h"
#include "QTransformable3.h"

namespace Q
{

template <class Real>
class HRTexture : public HRTextureBase, public Heap, public Transformable3<Real,true>
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    // construction
    HRTexture (TextureType eType, const RasterImagePtr& rspqImage);
    HRTexture (TextureType eType, const RasterImagePtrArrayPtr& rspqImageArray);
    HRTexture (TextureType eType, const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray);
    HRTexture (TextureType eType, const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray);

    // texture coordinate generation
    void SetTexCoordGenS (TexCoordGenType eTexCoordGen);
    void SetTexCoordGenS (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams);
    void UnsetTexCoordGenS ();
    void SetTexCoordGenT (TexCoordGenType eTexCoordGen);
    void SetTexCoordGenT (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams);
    void UnsetTexCoordGenT ();
    void SetTexCoordGenR (TexCoordGenType eTexCoordGen);
    void SetTexCoordGenR (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams);
    void UnsetTexCoordGenR ();
    void SetTexCoordGenQ (TexCoordGenType eTexCoordGen);
    void SetTexCoordGenQ (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams);
    void UnsetTexCoordGenQ ();

    // texture coordinate generation
    bool TexCoordGenSSet;                       // default: false
    TexCoordGenType TexCoordGenS;
    Vector4<Real> TexCoordGenPlaneParamsS;
    bool TexCoordGenTSet;                       // default: false
    TexCoordGenType TexCoordGenT;
    Vector4<Real> TexCoordGenPlaneParamsT;
    bool TexCoordGenRSet;                       // default: false
    TexCoordGenType TexCoordGenR;
    Vector4<Real> TexCoordGenPlaneParamsR;
    bool TexCoordGenQSet;                       // default: false
    TexCoordGenType TexCoordGenQ;
    Vector4<Real> TexCoordGenPlaneParamsQ;
};

#include "QHRTexture.inl"

typedef HRTexture<float> HRTexturef;
typedef HRTexture<double> HRTextured;

typedef SmartPointer<HRTexturef> HRTexturefPtr;
typedef SmartPointer<HRTextured> HRTexturedPtr;

}

#endif



