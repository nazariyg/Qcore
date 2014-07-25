//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRTexture<Real>::HRTexture (TextureType eType, const RasterImagePtr& rspqImage)
    :
    HRTextureBase(eType,rspqImage),
    TexCoordGenSSet(false),
    TexCoordGenTSet(false),
    TexCoordGenRSet(false),
    TexCoordGenQSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRTexture<Real>::HRTexture (TextureType eType, const RasterImagePtrArrayPtr& rspqImageArray)
    :
    HRTextureBase(eType,rspqImageArray),
    TexCoordGenSSet(false),
    TexCoordGenTSet(false),
    TexCoordGenRSet(false),
    TexCoordGenQSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRTexture<Real>::HRTexture (TextureType eType, const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray)
    :
    HRTextureBase(eType,rspqCompressedTextureMipmapArray),
    TexCoordGenSSet(false),
    TexCoordGenTSet(false),
    TexCoordGenRSet(false),
    TexCoordGenQSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRTexture<Real>::HRTexture (TextureType eType,
    const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray)
    :
    HRTextureBase(eType,rspqCompressedTextureMipmapArrayArray),
    TexCoordGenSSet(false),
    TexCoordGenTSet(false),
    TexCoordGenRSet(false),
    TexCoordGenQSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenS (TexCoordGenType eTexCoordGen)
{
    TexCoordGenS = eTexCoordGen;
    TexCoordGenSSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenS (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams)
{
    TexCoordGenS = eTexCoordGen;
    TexCoordGenPlaneParamsS = rqTexCoordGenPlaneParams;
    TexCoordGenSSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::UnsetTexCoordGenS ()
{
    TexCoordGenSSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenT (TexCoordGenType eTexCoordGen)
{
    TexCoordGenT = eTexCoordGen;
    TexCoordGenTSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenT (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams)
{
    TexCoordGenT = eTexCoordGen;
    TexCoordGenPlaneParamsT = rqTexCoordGenPlaneParams;
    TexCoordGenTSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::UnsetTexCoordGenT ()
{
    TexCoordGenTSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenR (TexCoordGenType eTexCoordGen)
{
    TexCoordGenR = eTexCoordGen;
    TexCoordGenRSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenR (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams)
{
    TexCoordGenR = eTexCoordGen;
    TexCoordGenPlaneParamsR = rqTexCoordGenPlaneParams;
    TexCoordGenRSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::UnsetTexCoordGenR ()
{
    TexCoordGenRSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenQ (TexCoordGenType eTexCoordGen)
{
    TexCoordGenQ = eTexCoordGen;
    TexCoordGenQSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::SetTexCoordGenQ (TexCoordGenType eTexCoordGen, const Vector4<Real>& rqTexCoordGenPlaneParams)
{
    TexCoordGenQ = eTexCoordGen;
    TexCoordGenPlaneParamsQ = rqTexCoordGenPlaneParams;
    TexCoordGenQSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRTexture<Real>::UnsetTexCoordGenQ ()
{
    TexCoordGenQSet = false;
}
//------------------------------------------------------------------------------------------------------------------



