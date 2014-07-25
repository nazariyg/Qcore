#ifndef QOCTREE3_H
#define QOCTREE3_H

#include "QAxisAlignedBox3.h"
#include "QArray.h"

namespace Q
{

template <class Real, class ElementT>
class Octree3
{
public:
    // construction
    Octree3 (const AxisAlignedBox3<Real>& rqOverallBound, int iSplitDepth);

    // member access
    const AxisAlignedBox3<Real>& GetOverallBound () const;
    int GetSplitDepth () const;

    // insertion of an element by a guide-point
    void InsertElement (const Vector3<Real>& rqGuidePoint, const ElementT& rtElement);

private:
    class SplitRecord
    {
    public:
        SplitRecord (const AxisAlignedBox3<Real>& rqBound)
            :
            Bound(rqBound)
        {
            for (int i = 0; i < 8; i++)
                ChildSplits[i] = 0;
        };
        ~SplitRecord ()
        {
            for (int i = 0; i < 8; i++)
                delete ChildSplits[i];
        };

        AxisAlignedBox3<Real> Bound;
        Array<ElementT> Content;
        SplitRecord* ChildSplits[8];
    };

    AxisAlignedBox3<Real> m_qOverallBound;
    int m_iSplitDepth;
    SplitRecord m_qOverallSplitRecord;
};

#include "QOctree3.inl"

}

#endif



