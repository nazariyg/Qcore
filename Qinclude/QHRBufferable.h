#ifndef QHRBUFFERABLE_H
#define QHRBUFFERABLE_H

#include "QRtti.h"

namespace Q
{

class Q_ITEM HRBufferable : public Rtti
{
public:
    enum BufferProfileType
    {
        PT_IMMEDIATE,
        PT_BUFFER
    };

protected:
    // construction and destruction
    HRBufferable ();

#ifdef _DEBUG
    ~HRBufferable ();
#endif

public:
    // when switching from buffered to immediate mode this function notifies the Hardware Renderer to make it
    // considering about withdrawing buffered data from server's memory
    void SetBufferProfile (BufferProfileType eBufferProfile);
    BufferProfileType GetBufferProfile () const;

    // notify the Hardware Renderer "manually"
    void Unbuffer ();

    bool NeedRebuffer;                      // default: true
    unsigned int BufferId;                  // default: 0 (invalid)

private:
    BufferProfileType m_eBufferProfile;     // default: PT_IMMEDIATE
};

#include "QHRBufferable.inl"

}

#endif



