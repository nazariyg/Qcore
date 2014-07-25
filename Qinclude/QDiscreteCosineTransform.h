#ifndef QDISCRETECOSINETRANSFORM_H
#define QDISCRETECOSINETRANSFORM_H

#include "QSystem.h"

namespace Q
{

template <class Real>
class Q_ITEM DiscreteCosineTransform
{
public:
    // DCTT_ORTHOGONAL_2D: two-dimensional DCT-II and DCT-III with orthogonal matrix up to a scale factor (i.e.
    // with 1/sqrt(2) in the formula)
    enum DctType
    {
        DCTT_ORTHOGONAL_2D
    };

    // Construction.  Initially creates helper tables.
    DiscreteCosineTransform (DctType eDct, int iSideExtension);

    // destruction
    ~DiscreteCosineTransform ();

    // transformation
    void ApplyForward2d (Real* afElement) const;
    void ApplyInverse2d (Real* afElement) const;

private:
    DctType m_eDct;
    int m_iSideExtension;
    Real** m_aafTables;
};

typedef DiscreteCosineTransform<float> DiscreteCosineTransformf;
typedef DiscreteCosineTransform<double> DiscreteCosineTransformd;

}

#endif



