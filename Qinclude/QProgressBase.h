#ifndef QPROGRESSBASE_H
#define QPROGRESSBASE_H

// An inheritance from the ProgressBase class allows an object of a derivative class to report the progress
// state of a currently running process.  A progress state is indicated by a 'float' value in the range from
// 0.0 (0 percent complete) to 1.0 (100 percent complete).  A process enabled with progress tracking is
// associated with a member function and commented as "progressed".  The ProgressBase class is accompanied by
// the ProgressProbe class.  Progress tracking works as follows: in the member function of the process and
// before the process actually starts, a ProgressProbe object is constructed and adjusted; with the
// construction of the ProgressProbe object, the object associated with the member function flags the progress
// up; while the member function is executing the process, it updates the progress state of the process via the
// ProgressProbe object; meanwhile, any thread can obtain the progress flag and the progress state via the
// layer of the ProgressBase class; when the member function exits, the ProgressProbe object destructs having
// flagged the progress down.

#include "QSystem.h"

namespace Q
{

class ProgressProbe;

class Q_ITEM ProgressBase
{
protected:
    // construction
    ProgressBase ();

public:
    // progress state
    bool GetProgressFlag () const;
    float GetProgressState () const;
    void SetProgressCallback (void (*oCallback)(float));

    // Redirection.  If a process depends on another process, the progress of the sub-process can be redirected
    // to the main process.  If redirected, the sub-process is updating the progress state of the main process
    // instead of its own.  The update formula is:
    //   main_state = initial_main_state + contribution_factor * sub_state
    // where initial_main_state is the progress state of the main process at the moment of redirection, and
    // contribution_factor is a value in the range [0.0, 1.0] specifying the contribution "window" of the
    // sub-process.  Parameter pqObject specifies the address of the object of the main process, parameter
    // rqProbe specifies the ProgressProbe object accompanying the main process.
    void RedirectProgressTo (ProgressBase* pqObject, ProgressProbe& rqProbe, float fContribFactor);

private:
    friend class ProgressProbe;

    bool m_bDoes;
    float m_fDone;
    void (*m_oCallback)(float);
    bool m_bRedirMode;
    ProgressProbe* m_pqRedirProbe;
    float m_fRedirDoneBefore, m_fRedirContribFactor;
};

#include "QProgressBase.inl"

}

#endif



