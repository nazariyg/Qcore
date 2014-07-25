#ifndef QSTATESENSITIVE_H
#define QSTATESENSITIVE_H

#include "QTextureState.h"

namespace Q
{

class Q_ITEM StateSensitive
{
public:
    // construction
    StateSensitive ();  // empty

    // state binding
    int GetStateQuantity () const;
    int BindState (const SmartRttiPointer& rspqState);
    const SmartRttiPointer& GetState (int i) const;
    void UnbindState (int i);

    // particulars
    void BindTextureStatef (const TextureStatefPtr& rspqTextureState);
    void BindTextureStated (const TextureStatedPtr& rspqTextureState);
    const TextureStatefPtr& GetTextureStatef () const;
    const TextureStatedPtr& GetTextureStated () const;
    void UnbindTextureState ();

    void SetReceiveAllLights (bool bSet);
    bool GetReceiveAllLights () const;

private:
    ModestArray<SmartRttiPointer> m_qStates;
    TextureStatefPtr m_spqTextureStatef;        // default: 0
    TextureStatedPtr m_spqTextureStated;        // default: 0
    bool m_bReceiveAllLights;                   // dafault: true
};

#include "QStateSensitive.inl"

}

#endif



