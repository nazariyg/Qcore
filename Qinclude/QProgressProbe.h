#ifndef QPROGRESSPROBE_H
#define QPROGRESSPROBE_H

// A ProgressProbe object accompanies a process run by a ProgressBase derivative.  (You may look into the
// ProgressBase class header for the related information.)

#include "QProgressBase.h"

namespace Q
{

class Q_ITEM ProgressProbe
{
public:
    // construction
    ProgressProbe (ProgressBase* pqProgress, float fAlreadyDone = 0.0f);

    // destruction
    ~ProgressProbe ();

    // progress control
    void SetAdvance (float fAdvance);          // adjusts the step amount
    void MakeAdvance ();                       // makes a step
    void MakeOneTimeAdvance (float fAdvance);  // makes a non-adjusted step
    void SetDone (float fDone);                // sets the progress state directly; the range is [0.0, 1.0]

private:
    void OnUpdate () const;

    static const int ms_iDoneDeltaQuantity;    // 100
    static const float ms_fInvDoneDeltaQuantity;
    ProgressBase* m_pqProgress;
    float m_fDone, m_fAdvance;
};

#include "QProgressProbe.inl"

}

#endif



