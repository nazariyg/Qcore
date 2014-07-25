#ifndef QDISCRETEFOURIERTRANSFORM_H
#define QDISCRETEFOURIERTRANSFORM_H

#include "QComplex.h"

namespace Q
{

template <class Real>
class Q_ITEM DiscreteFourierTransform
{
public:
    enum DftType
    {
        DFTT_1D,
        DFTT_1D_FAST_FORWARD,
        DFTT_1D_FAST_INVERSE,
        DFTT_1D_FAST_FORWARD_REAL,
        DFTT_1D_FAST_INVERSE_REAL
    };

    // Construction. If the type of the transform is any of the fast ones, iN must be a power of 2.
    DiscreteFourierTransform (DftType eDft, int iN);

    // destruction
    ~DiscreteFourierTransform ();

    // transforms for complex input
    void ApplyForward (Complex<Real>* aqPoint) const;
    void ApplyInverse (Complex<Real>* aqPoint) const;

    // transforms for real input in the form of complex numbers with the imaginary part of each number being 0.0
    void ApplyForwardReal (Complex<Real>* aqPoint) const;
    void ApplyInverseReal (Complex<Real>* aqPoint) const;

private:
    void CtApply (Complex<Real>* aqPoint, int iN) const;

    DftType m_eDft;
    int m_iN, m_iHalfN;
    Complex<Real>** m_aaqCtLuTable;
    int m_iCtLuTPointQuantityL0;
    Complex<Real>* m_aqRpA;
    Complex<Real>* m_aqRpB;
    Complex<Real>* m_aqPackedPoint;
};

typedef DiscreteFourierTransform<float> DiscreteFourierTransformf;
typedef DiscreteFourierTransform<double> DiscreteFourierTransformd;

}

#endif



