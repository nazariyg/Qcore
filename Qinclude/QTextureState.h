#ifndef QTEXTURESTATE_H
#define QTEXTURESTATE_H

#include "QStateBase.h"
#include "QHRTexture.h"
#include "QModestArray.h"

namespace Q
{

template <class Real>
class TextureState : public StateBase
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    enum PointSpriteCoordOriginType
    {
        PSCOT_LOWER_LEFT,
        PSCOT_UPPER_LEFT
    };

    // construction
    TextureState ();  // empty

    // point sprite
    void SetPointSprite (PointSpriteCoordOriginType ePointSpriteCoordOrigin);
    void UnsetPointSprite ();

    // member access
    int GetTextureQuantity () const;
    int AddTexture (const SmartPointer<HRTexture<Real> >& rspqTexture);
    const SmartPointer<HRTexture<Real> >& GetTexture (int i) const;
    void RemoveTexture (int i);

    bool PointSpriteSet;  // default: false
    PointSpriteCoordOriginType PointSpriteCoordOrigin;

private:
    ModestArray<SmartPointer<HRTexture<Real> > > m_qTextures;
};

#include "QTextureState.inl"

typedef TextureState<float> TextureStatef;
typedef TextureState<double> TextureStated;

typedef SmartPointer<TextureStatef> TextureStatefPtr;
typedef SmartPointer<TextureStated> TextureStatedPtr;

}

#endif



