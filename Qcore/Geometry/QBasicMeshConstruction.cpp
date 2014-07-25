#include "QBasicMeshConstruction.h"
#include "QGrphUtils3.h"
#include "QModestArray.h"
#include "QSortUtils.h"
#include "QSet.h"
#include "QMap.h"
using namespace Q;

// support for teapot modeling
template <class Real>
const Vector3<Real> BasicMeshConstruction<Real>::ms_aqTeapotVertices[306] = {
    Vector3<Real>((Real)1.40000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)1.40000,(Real)-0.7840,(Real)2.40000),
    Vector3<Real>((Real)0.78400,(Real)-1.4000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)-1.4000,(Real)2.40000),
    Vector3<Real>((Real)1.33750,(Real)0.00000,(Real)2.53125),
    Vector3<Real>((Real)1.33750,(Real)-0.7490,(Real)2.53125),
    Vector3<Real>((Real)0.74900,(Real)-1.3375,(Real)2.53125),
    Vector3<Real>((Real)0.00000,(Real)-1.3375,(Real)2.53125),
    Vector3<Real>((Real)1.43750,(Real)0.00000,(Real)2.53125),
    Vector3<Real>((Real)1.43750,(Real)-0.8050,(Real)2.53125),
    Vector3<Real>((Real)0.80500,(Real)-1.4375,(Real)2.53125),
    Vector3<Real>((Real)0.00000,(Real)-1.4375,(Real)2.53125),
    Vector3<Real>((Real)1.50000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)1.50000,(Real)-0.8400,(Real)2.40000),
    Vector3<Real>((Real)0.84000,(Real)-1.5000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)-1.5000,(Real)2.40000),
    Vector3<Real>((Real)-0.7840,(Real)-1.4000,(Real)2.40000),
    Vector3<Real>((Real)-1.4000,(Real)-0.7840,(Real)2.40000),
    Vector3<Real>((Real)-1.4000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)-0.7490,(Real)-1.3375,(Real)2.53125),
    Vector3<Real>((Real)-1.3375,(Real)-0.7490,(Real)2.53125),
    Vector3<Real>((Real)-1.3375,(Real)0.00000,(Real)2.53125),
    Vector3<Real>((Real)-0.8050,(Real)-1.4375,(Real)2.53125),
    Vector3<Real>((Real)-1.4375,(Real)-0.8050,(Real)2.53125),
    Vector3<Real>((Real)-1.4375,(Real)0.00000,(Real)2.53125),
    Vector3<Real>((Real)-0.8400,(Real)-1.5000,(Real)2.40000),
    Vector3<Real>((Real)-1.5000,(Real)-0.8400,(Real)2.40000),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)-1.4000,(Real)0.78400,(Real)2.40000),
    Vector3<Real>((Real)-0.7840,(Real)1.40000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)1.40000,(Real)2.40000),
    Vector3<Real>((Real)-1.3375,(Real)0.74900,(Real)2.53125),
    Vector3<Real>((Real)-0.7490,(Real)1.33750,(Real)2.53125),
    Vector3<Real>((Real)0.00000,(Real)1.33750,(Real)2.53125),
    Vector3<Real>((Real)-1.4375,(Real)0.80500,(Real)2.53125),
    Vector3<Real>((Real)-0.8050,(Real)1.43750,(Real)2.53125),
    Vector3<Real>((Real)0.00000,(Real)1.43750,(Real)2.53125),
    Vector3<Real>((Real)-1.5000,(Real)0.84000,(Real)2.40000),
    Vector3<Real>((Real)-0.8400,(Real)1.50000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)1.50000,(Real)2.40000),
    Vector3<Real>((Real)0.78400,(Real)1.40000,(Real)2.40000),
    Vector3<Real>((Real)1.40000,(Real)0.78400,(Real)2.40000),
    Vector3<Real>((Real)0.74900,(Real)1.33750,(Real)2.53125),
    Vector3<Real>((Real)1.33750,(Real)0.74900,(Real)2.53125),
    Vector3<Real>((Real)0.80500,(Real)1.43750,(Real)2.53125),
    Vector3<Real>((Real)1.43750,(Real)0.80500,(Real)2.53125),
    Vector3<Real>((Real)0.84000,(Real)1.50000,(Real)2.40000),
    Vector3<Real>((Real)1.50000,(Real)0.84000,(Real)2.40000),
    Vector3<Real>((Real)1.75000,(Real)0.00000,(Real)1.87500),
    Vector3<Real>((Real)1.75000,(Real)-0.9800,(Real)1.87500),
    Vector3<Real>((Real)0.98000,(Real)-1.7500,(Real)1.87500),
    Vector3<Real>((Real)0.00000,(Real)-1.7500,(Real)1.87500),
    Vector3<Real>((Real)2.00000,(Real)0.00000,(Real)1.35000),
    Vector3<Real>((Real)2.00000,(Real)-1.1200,(Real)1.35000),
    Vector3<Real>((Real)1.12000,(Real)-2.0000,(Real)1.35000),
    Vector3<Real>((Real)0.00000,(Real)-2.0000,(Real)1.35000),
    Vector3<Real>((Real)2.00000,(Real)0.00000,(Real)0.90000),
    Vector3<Real>((Real)2.00000,(Real)-1.1200,(Real)0.90000),
    Vector3<Real>((Real)1.12000,(Real)-2.0000,(Real)0.90000),
    Vector3<Real>((Real)0.00000,(Real)-2.0000,(Real)0.90000),
    Vector3<Real>((Real)-0.9800,(Real)-1.7500,(Real)1.87500),
    Vector3<Real>((Real)-1.7500,(Real)-0.9800,(Real)1.87500),
    Vector3<Real>((Real)-1.7500,(Real)0.00000,(Real)1.87500),
    Vector3<Real>((Real)-1.1200,(Real)-2.0000,(Real)1.35000),
    Vector3<Real>((Real)-2.0000,(Real)-1.1200,(Real)1.35000),
    Vector3<Real>((Real)-2.0000,(Real)0.00000,(Real)1.35000),
    Vector3<Real>((Real)-1.1200,(Real)-2.0000,(Real)0.90000),
    Vector3<Real>((Real)-2.0000,(Real)-1.1200,(Real)0.90000),
    Vector3<Real>((Real)-2.0000,(Real)0.00000,(Real)0.90000),
    Vector3<Real>((Real)-1.7500,(Real)0.98000,(Real)1.87500),
    Vector3<Real>((Real)-0.9800,(Real)1.75000,(Real)1.87500),
    Vector3<Real>((Real)0.00000,(Real)1.75000,(Real)1.87500),
    Vector3<Real>((Real)-2.0000,(Real)1.12000,(Real)1.35000),
    Vector3<Real>((Real)-1.1200,(Real)2.00000,(Real)1.35000),
    Vector3<Real>((Real)0.00000,(Real)2.00000,(Real)1.35000),
    Vector3<Real>((Real)-2.0000,(Real)1.12000,(Real)0.90000),
    Vector3<Real>((Real)-1.1200,(Real)2.00000,(Real)0.90000),
    Vector3<Real>((Real)0.00000,(Real)2.00000,(Real)0.90000),
    Vector3<Real>((Real)0.98000,(Real)1.75000,(Real)1.87500),
    Vector3<Real>((Real)1.75000,(Real)0.98000,(Real)1.87500),
    Vector3<Real>((Real)1.12000,(Real)2.00000,(Real)1.35000),
    Vector3<Real>((Real)2.00000,(Real)1.12000,(Real)1.35000),
    Vector3<Real>((Real)1.12000,(Real)2.00000,(Real)0.90000),
    Vector3<Real>((Real)2.00000,(Real)1.12000,(Real)0.90000),
    Vector3<Real>((Real)2.00000,(Real)0.00000,(Real)0.45000),
    Vector3<Real>((Real)2.00000,(Real)-1.1200,(Real)0.45000),
    Vector3<Real>((Real)1.12000,(Real)-2.0000,(Real)0.45000),
    Vector3<Real>((Real)0.00000,(Real)-2.0000,(Real)0.45000),
    Vector3<Real>((Real)1.50000,(Real)0.00000,(Real)0.22500),
    Vector3<Real>((Real)1.50000,(Real)-0.8400,(Real)0.22500),
    Vector3<Real>((Real)0.84000,(Real)-1.5000,(Real)0.22500),
    Vector3<Real>((Real)0.00000,(Real)-1.5000,(Real)0.22500),
    Vector3<Real>((Real)1.50000,(Real)0.00000,(Real)0.15000),
    Vector3<Real>((Real)1.50000,(Real)-0.8400,(Real)0.15000),
    Vector3<Real>((Real)0.84000,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)0.00000,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)-1.1200,(Real)-2.0000,(Real)0.45000),
    Vector3<Real>((Real)-2.0000,(Real)-1.1200,(Real)0.45000),
    Vector3<Real>((Real)-2.0000,(Real)0.00000,(Real)0.45000),
    Vector3<Real>((Real)-0.8400,(Real)-1.5000,(Real)0.22500),
    Vector3<Real>((Real)-1.5000,(Real)-0.8400,(Real)0.22500),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)0.22500),
    Vector3<Real>((Real)-0.8400,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)-1.5000,(Real)-0.8400,(Real)0.15000),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)0.15000),
    Vector3<Real>((Real)-2.0000,(Real)1.12000,(Real)0.45000),
    Vector3<Real>((Real)-1.1200,(Real)2.00000,(Real)0.45000),
    Vector3<Real>((Real)0.00000,(Real)2.00000,(Real)0.45000),
    Vector3<Real>((Real)-1.5000,(Real)0.84000,(Real)0.22500),
    Vector3<Real>((Real)-0.8400,(Real)1.50000,(Real)0.22500),
    Vector3<Real>((Real)0.00000,(Real)1.50000,(Real)0.22500),
    Vector3<Real>((Real)-1.5000,(Real)0.84000,(Real)0.15000),
    Vector3<Real>((Real)-0.8400,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)0.00000,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)1.12000,(Real)2.00000,(Real)0.45000),
    Vector3<Real>((Real)2.00000,(Real)1.12000,(Real)0.45000),
    Vector3<Real>((Real)0.84000,(Real)1.50000,(Real)0.22500),
    Vector3<Real>((Real)1.50000,(Real)0.84000,(Real)0.22500),
    Vector3<Real>((Real)0.84000,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)1.50000,(Real)0.84000,(Real)0.15000),
    Vector3<Real>((Real)-1.6000,(Real)0.00000,(Real)2.02500),
    Vector3<Real>((Real)-1.6000,(Real)-0.3000,(Real)2.02500),
    Vector3<Real>((Real)-1.5000,(Real)-0.3000,(Real)2.25000),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)2.25000),
    Vector3<Real>((Real)-2.3000,(Real)0.00000,(Real)2.02500),
    Vector3<Real>((Real)-2.3000,(Real)-0.3000,(Real)2.02500),
    Vector3<Real>((Real)-2.5000,(Real)-0.3000,(Real)2.25000),
    Vector3<Real>((Real)-2.5000,(Real)0.00000,(Real)2.25000),
    Vector3<Real>((Real)-2.7000,(Real)0.00000,(Real)2.02500),
    Vector3<Real>((Real)-2.7000,(Real)-0.3000,(Real)2.02500),
    Vector3<Real>((Real)-3.0000,(Real)-0.3000,(Real)2.25000),
    Vector3<Real>((Real)-3.0000,(Real)0.00000,(Real)2.25000),
    Vector3<Real>((Real)-2.7000,(Real)0.00000,(Real)1.80000),
    Vector3<Real>((Real)-2.7000,(Real)-0.3000,(Real)1.80000),
    Vector3<Real>((Real)-3.0000,(Real)-0.3000,(Real)1.80000),
    Vector3<Real>((Real)-3.0000,(Real)0.00000,(Real)1.80000),
    Vector3<Real>((Real)-1.5000,(Real)0.30000,(Real)2.25000),
    Vector3<Real>((Real)-1.6000,(Real)0.30000,(Real)2.02500),
    Vector3<Real>((Real)-2.5000,(Real)0.30000,(Real)2.25000),
    Vector3<Real>((Real)-2.3000,(Real)0.30000,(Real)2.02500),
    Vector3<Real>((Real)-3.0000,(Real)0.30000,(Real)2.25000),
    Vector3<Real>((Real)-2.7000,(Real)0.30000,(Real)2.02500),
    Vector3<Real>((Real)-3.0000,(Real)0.30000,(Real)1.80000),
    Vector3<Real>((Real)-2.7000,(Real)0.30000,(Real)1.80000),
    Vector3<Real>((Real)-2.7000,(Real)0.00000,(Real)1.57500),
    Vector3<Real>((Real)-2.7000,(Real)-0.3000,(Real)1.57500),
    Vector3<Real>((Real)-3.0000,(Real)-0.3000,(Real)1.35000),
    Vector3<Real>((Real)-3.0000,(Real)0.00000,(Real)1.35000),
    Vector3<Real>((Real)-2.5000,(Real)0.00000,(Real)1.12500),
    Vector3<Real>((Real)-2.5000,(Real)-0.3000,(Real)1.12500),
    Vector3<Real>((Real)-2.6500,(Real)-0.3000,(Real)0.93750),
    Vector3<Real>((Real)-2.6500,(Real)0.00000,(Real)0.93750),
    Vector3<Real>((Real)-2.0000,(Real)-0.3000,(Real)0.90000),
    Vector3<Real>((Real)-1.9000,(Real)-0.3000,(Real)0.60000),
    Vector3<Real>((Real)-1.9000,(Real)0.00000,(Real)0.60000),
    Vector3<Real>((Real)-3.0000,(Real)0.30000,(Real)1.35000),
    Vector3<Real>((Real)-2.7000,(Real)0.30000,(Real)1.57500),
    Vector3<Real>((Real)-2.6500,(Real)0.30000,(Real)0.93750),
    Vector3<Real>((Real)-2.5000,(Real)0.30000,(Real)1.12500),
    Vector3<Real>((Real)-1.9000,(Real)0.30000,(Real)0.60000),
    Vector3<Real>((Real)-2.0000,(Real)0.30000,(Real)0.90000),
    Vector3<Real>((Real)1.70000,(Real)0.00000,(Real)1.42500),
    Vector3<Real>((Real)1.70000,(Real)-0.6600,(Real)1.42500),
    Vector3<Real>((Real)1.70000,(Real)-0.6600,(Real)0.60000),
    Vector3<Real>((Real)1.70000,(Real)0.00000,(Real)0.60000),
    Vector3<Real>((Real)2.60000,(Real)0.00000,(Real)1.42500),
    Vector3<Real>((Real)2.60000,(Real)-0.6600,(Real)1.42500),
    Vector3<Real>((Real)3.10000,(Real)-0.6600,(Real)0.82500),
    Vector3<Real>((Real)3.10000,(Real)0.00000,(Real)0.82500),
    Vector3<Real>((Real)2.30000,(Real)0.00000,(Real)2.10000),
    Vector3<Real>((Real)2.30000,(Real)-0.2500,(Real)2.10000),
    Vector3<Real>((Real)2.40000,(Real)-0.2500,(Real)2.02500),
    Vector3<Real>((Real)2.40000,(Real)0.00000,(Real)2.02500),
    Vector3<Real>((Real)2.70000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)2.70000,(Real)-0.2500,(Real)2.40000),
    Vector3<Real>((Real)3.30000,(Real)-0.2500,(Real)2.40000),
    Vector3<Real>((Real)3.30000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)1.70000,(Real)0.66000,(Real)0.60000),
    Vector3<Real>((Real)1.70000,(Real)0.66000,(Real)1.42500),
    Vector3<Real>((Real)3.10000,(Real)0.66000,(Real)0.82500),
    Vector3<Real>((Real)2.60000,(Real)0.66000,(Real)1.42500),
    Vector3<Real>((Real)2.40000,(Real)0.25000,(Real)2.02500),
    Vector3<Real>((Real)2.30000,(Real)0.25000,(Real)2.10000),
    Vector3<Real>((Real)3.30000,(Real)0.25000,(Real)2.40000),
    Vector3<Real>((Real)2.70000,(Real)0.25000,(Real)2.40000),
    Vector3<Real>((Real)2.80000,(Real)0.00000,(Real)2.47500),
    Vector3<Real>((Real)2.80000,(Real)-0.2500,(Real)2.47500),
    Vector3<Real>((Real)3.52500,(Real)-0.2500,(Real)2.49375),
    Vector3<Real>((Real)3.52500,(Real)0.00000,(Real)2.49375),
    Vector3<Real>((Real)2.90000,(Real)0.00000,(Real)2.47500),
    Vector3<Real>((Real)2.90000,(Real)-0.1500,(Real)2.47500),
    Vector3<Real>((Real)3.45000,(Real)-0.1500,(Real)2.51250),
    Vector3<Real>((Real)3.45000,(Real)0.00000,(Real)2.51250),
    Vector3<Real>((Real)2.80000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)2.80000,(Real)-0.1500,(Real)2.40000),
    Vector3<Real>((Real)3.20000,(Real)-0.1500,(Real)2.40000),
    Vector3<Real>((Real)3.20000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)3.52500,(Real)0.25000,(Real)2.49375),
    Vector3<Real>((Real)2.80000,(Real)0.25000,(Real)2.47500),
    Vector3<Real>((Real)3.45000,(Real)0.15000,(Real)2.51250),
    Vector3<Real>((Real)2.90000,(Real)0.15000,(Real)2.47500),
    Vector3<Real>((Real)3.20000,(Real)0.15000,(Real)2.40000),
    Vector3<Real>((Real)2.80000,(Real)0.15000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)0.00000,(Real)3.15000),
    Vector3<Real>((Real)0.00000,(Real)-0.0020,(Real)3.15000),
    Vector3<Real>((Real)0.00200,(Real)0.00000,(Real)3.15000),
    Vector3<Real>((Real)0.80000,(Real)0.00000,(Real)3.15000),
    Vector3<Real>((Real)0.80000,(Real)-0.4500,(Real)3.15000),
    Vector3<Real>((Real)0.45000,(Real)-0.8000,(Real)3.15000),
    Vector3<Real>((Real)0.00000,(Real)-0.8000,(Real)3.15000),
    Vector3<Real>((Real)0.00000,(Real)0.00000,(Real)2.85000),
    Vector3<Real>((Real)0.20000,(Real)0.00000,(Real)2.70000),
    Vector3<Real>((Real)0.20000,(Real)-0.1120,(Real)2.70000),
    Vector3<Real>((Real)0.11200,(Real)-0.2000,(Real)2.70000),
    Vector3<Real>((Real)0.00000,(Real)-0.2000,(Real)2.70000),
    Vector3<Real>((Real)-0.0020,(Real)0.00000,(Real)3.15000),
    Vector3<Real>((Real)-0.4500,(Real)-0.8000,(Real)3.15000),
    Vector3<Real>((Real)-0.8000,(Real)-0.4500,(Real)3.15000),
    Vector3<Real>((Real)-0.8000,(Real)0.00000,(Real)3.15000),
    Vector3<Real>((Real)-0.1120,(Real)-0.2000,(Real)2.70000),
    Vector3<Real>((Real)-0.2000,(Real)-0.1120,(Real)2.70000),
    Vector3<Real>((Real)-0.2000,(Real)0.00000,(Real)2.70000),
    Vector3<Real>((Real)0.00000,(Real)0.00200,(Real)3.15000),
    Vector3<Real>((Real)-0.8000,(Real)0.45000,(Real)3.15000),
    Vector3<Real>((Real)-0.4500,(Real)0.80000,(Real)3.15000),
    Vector3<Real>((Real)0.00000,(Real)0.80000,(Real)3.15000),
    Vector3<Real>((Real)-0.2000,(Real)0.11200,(Real)2.70000),
    Vector3<Real>((Real)-0.1120,(Real)0.20000,(Real)2.70000),
    Vector3<Real>((Real)0.00000,(Real)0.20000,(Real)2.70000),
    Vector3<Real>((Real)0.45000,(Real)0.80000,(Real)3.15000),
    Vector3<Real>((Real)0.80000,(Real)0.45000,(Real)3.15000),
    Vector3<Real>((Real)0.11200,(Real)0.20000,(Real)2.70000),
    Vector3<Real>((Real)0.20000,(Real)0.11200,(Real)2.70000),
    Vector3<Real>((Real)0.40000,(Real)0.00000,(Real)2.55000),
    Vector3<Real>((Real)0.40000,(Real)-0.2240,(Real)2.55000),
    Vector3<Real>((Real)0.22400,(Real)-0.4000,(Real)2.55000),
    Vector3<Real>((Real)0.00000,(Real)-0.4000,(Real)2.55000),
    Vector3<Real>((Real)1.30000,(Real)0.00000,(Real)2.55000),
    Vector3<Real>((Real)1.30000,(Real)-0.7280,(Real)2.55000),
    Vector3<Real>((Real)0.72800,(Real)-1.3000,(Real)2.55000),
    Vector3<Real>((Real)0.00000,(Real)-1.3000,(Real)2.55000),
    Vector3<Real>((Real)1.30000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)1.30000,(Real)-0.7280,(Real)2.40000),
    Vector3<Real>((Real)0.72800,(Real)-1.3000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)-1.3000,(Real)2.40000),
    Vector3<Real>((Real)-0.2240,(Real)-0.4000,(Real)2.55000),
    Vector3<Real>((Real)-0.4000,(Real)-0.2240,(Real)2.55000),
    Vector3<Real>((Real)-0.4000,(Real)0.00000,(Real)2.55000),
    Vector3<Real>((Real)-0.7280,(Real)-1.3000,(Real)2.55000),
    Vector3<Real>((Real)-1.3000,(Real)-0.7280,(Real)2.55000),
    Vector3<Real>((Real)-1.3000,(Real)0.00000,(Real)2.55000),
    Vector3<Real>((Real)-0.7280,(Real)-1.3000,(Real)2.40000),
    Vector3<Real>((Real)-1.3000,(Real)-0.7280,(Real)2.40000),
    Vector3<Real>((Real)-1.3000,(Real)0.00000,(Real)2.40000),
    Vector3<Real>((Real)-0.4000,(Real)0.22400,(Real)2.55000),
    Vector3<Real>((Real)-0.2240,(Real)0.40000,(Real)2.55000),
    Vector3<Real>((Real)0.00000,(Real)0.40000,(Real)2.55000),
    Vector3<Real>((Real)-1.3000,(Real)0.72800,(Real)2.55000),
    Vector3<Real>((Real)-0.7280,(Real)1.30000,(Real)2.55000),
    Vector3<Real>((Real)0.00000,(Real)1.30000,(Real)2.55000),
    Vector3<Real>((Real)-1.3000,(Real)0.72800,(Real)2.40000),
    Vector3<Real>((Real)-0.7280,(Real)1.30000,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)1.30000,(Real)2.40000),
    Vector3<Real>((Real)0.22400,(Real)0.40000,(Real)2.55000),
    Vector3<Real>((Real)0.40000,(Real)0.22400,(Real)2.55000),
    Vector3<Real>((Real)0.72800,(Real)1.30000,(Real)2.55000),
    Vector3<Real>((Real)1.30000,(Real)0.72800,(Real)2.55000),
    Vector3<Real>((Real)0.72800,(Real)1.30000,(Real)2.40000),
    Vector3<Real>((Real)1.30000,(Real)0.72800,(Real)2.40000),
    Vector3<Real>((Real)0.00000,(Real)0.00000,(Real)0.00000),
    Vector3<Real>((Real)1.50000,(Real)0.00000,(Real)0.15000),
    Vector3<Real>((Real)1.50000,(Real)0.84000,(Real)0.15000),
    Vector3<Real>((Real)0.84000,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)0.00000,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)1.50000,(Real)0.00000,(Real)0.07500),
    Vector3<Real>((Real)1.50000,(Real)0.84000,(Real)0.07500),
    Vector3<Real>((Real)0.84000,(Real)1.50000,(Real)0.07500),
    Vector3<Real>((Real)0.00000,(Real)1.50000,(Real)0.07500),
    Vector3<Real>((Real)1.42500,(Real)0.00000,(Real)0.00000),
    Vector3<Real>((Real)1.42500,(Real)0.79800,(Real)0.00000),
    Vector3<Real>((Real)0.79800,(Real)1.42500,(Real)0.00000),
    Vector3<Real>((Real)0.00000,(Real)1.42500,(Real)0.00000),
    Vector3<Real>((Real)-0.8400,(Real)1.50000,(Real)0.15000),
    Vector3<Real>((Real)-1.5000,(Real)0.84000,(Real)0.15000),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)0.15000),
    Vector3<Real>((Real)-0.8400,(Real)1.50000,(Real)0.07500),
    Vector3<Real>((Real)-1.5000,(Real)0.84000,(Real)0.07500),
    Vector3<Real>((Real)-1.5000,(Real)0.00000,(Real)0.07500),
    Vector3<Real>((Real)-0.7980,(Real)1.42500,(Real)0.00000),
    Vector3<Real>((Real)-1.4250,(Real)0.79800,(Real)0.00000),
    Vector3<Real>((Real)-1.4250,(Real)0.00000,(Real)0.00000),
    Vector3<Real>((Real)-1.5000,(Real)-0.8400,(Real)0.15000),
    Vector3<Real>((Real)-0.8400,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)0.00000,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)-1.5000,(Real)-0.8400,(Real)0.07500),
    Vector3<Real>((Real)-0.8400,(Real)-1.5000,(Real)0.07500),
    Vector3<Real>((Real)0.00000,(Real)-1.5000,(Real)0.07500),
    Vector3<Real>((Real)-1.4250,(Real)-0.7980,(Real)0.00000),
    Vector3<Real>((Real)-0.7980,(Real)-1.4250,(Real)0.00000),
    Vector3<Real>((Real)0.00000,(Real)-1.4250,(Real)0.00000),
    Vector3<Real>((Real)0.84000,(Real)-1.5000,(Real)0.15000),
    Vector3<Real>((Real)1.50000,(Real)-0.8400,(Real)0.15000),
    Vector3<Real>((Real)0.84000,(Real)-1.5000,(Real)0.07500),
    Vector3<Real>((Real)1.50000,(Real)-0.8400,(Real)0.07500),
    Vector3<Real>((Real)0.79800,(Real)-1.4250,(Real)0.00000),
    Vector3<Real>((Real)1.42500,(Real)-0.7980,(Real)0.00000)};
template <class Real>
const int BasicMeshConstruction<Real>::ms_aaaiTeapotIndices[32][4][4] = {
    {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15},
    {  3,  16,  17,  18,   7,  19,  20,  21,  11,  22,  23,  24,  15,  25,  26,  27},
    { 18,  28,  29,  30,  21,  31,  32,  33,  24,  34,  35,  36,  27,  37,  38,  39},
    { 30,  40,  41,   0,  33,  42,  43,   4,  36,  44,  45,   8,  39,  46,  47,  12},
    { 12,  13,  14,  15,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59},
    { 15,  25,  26,  27,  51,  60,  61,  62,  55,  63,  64,  65,  59,  66,  67,  68},
    { 27,  37,  38,  39,  62,  69,  70,  71,  65,  72,  73,  74,  68,  75,  76,  77},
    { 39,  46,  47,  12,  71,  78,  79,  48,  74,  80,  81,  52,  77,  82,  83,  56},
    { 56,  57,  58,  59,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95},
    { 59,  66,  67,  68,  87,  96,  97,  98,  91,  99, 100, 101,  95, 102, 103, 104},
    { 68,  75,  76,  77,  98, 105, 106, 107, 101, 108, 109, 110, 104, 111, 112, 113},
    { 77,  82,  83,  56, 107, 114, 115,  84, 110, 116, 117,  88, 113, 118, 119,  92},
    {120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135},
    {123, 136, 137, 120, 127, 138, 139, 124, 131, 140, 141, 128, 135, 142, 143, 132},
    {132, 133, 134, 135, 144, 145, 146, 147, 148, 149, 150, 151,  68, 152, 153, 154},
    {135, 142, 143, 132, 147, 155, 156, 144, 151, 157, 158, 148, 154, 159, 160,  68},
    {161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176},
    {164, 177, 178, 161, 168, 179, 180, 165, 172, 181, 182, 169, 176, 183, 184, 173},
    {173, 174, 175, 176, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196},
    {176, 183, 184, 173, 188, 197, 198, 185, 192, 199, 200, 189, 196, 201, 202, 193},
    {203, 203, 203, 203, 206, 207, 208, 209, 210, 210, 210, 210, 211, 212, 213, 214},
    {203, 203, 203, 203, 209, 216, 217, 218, 210, 210, 210, 210, 214, 219, 220, 221},
    {203, 203, 203, 203, 218, 223, 224, 225, 210, 210, 210, 210, 221, 226, 227, 228},
    {203, 203, 203, 203, 225, 229, 230, 206, 210, 210, 210, 210, 228, 231, 232, 211},
    {211, 212, 213, 214, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244},
    {214, 219, 220, 221, 236, 245, 246, 247, 240, 248, 249, 250, 244, 251, 252, 253},
    {221, 226, 227, 228, 247, 254, 255, 256, 250, 257, 258, 259, 253, 260, 261, 262},
    {228, 231, 232, 211, 256, 263, 264, 233, 259, 265, 266, 237, 262, 267, 268, 241},
    {269, 269, 269, 269, 278, 279, 280, 281, 274, 275, 276, 277, 270, 271, 272, 273},
    {269, 269, 269, 269, 281, 288, 289, 290, 277, 285, 286, 287, 273, 282, 283, 284},
    {269, 269, 269, 269, 290, 297, 298, 299, 287, 294, 295, 296, 284, 291, 292, 293},
    {269, 269, 269, 269, 299, 304, 305, 278, 296, 302, 303, 274, 293, 300, 301, 270}};

//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructBox (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fExtentX, Real fExtentY,
    Real fExtentZ, int iSegmentsX, int iSegmentsY, int iSegmentsZ, Real fRadius, int iRadiusSegments)
{
    assert( fExtentX >= (Real)0.0 && fExtentY >= (Real)0.0 && fExtentZ >= (Real)0.0
        &&  iSegmentsX >= 1 && iSegmentsY >= 1 && iSegmentsZ >= 1
        &&  (Real)0.0 <= fRadius && fRadius <= Math<Real>::Min3(
            ( fExtentX != (Real)0.0 ? fExtentX : Math<Real>::MAX_REAL),
            ( fExtentY != (Real)0.0 ? fExtentY : Math<Real>::MAX_REAL),
            ( fExtentZ != (Real)0.0 ? fExtentZ : Math<Real>::MAX_REAL))/2
        &&  !(fRadius != (Real)0.0 && iRadiusSegments < 1) );
    assert( ( fRadius == (Real)0.0 ) == ( iRadiusSegments == 0 ) );

    int iSegmentsWkX = ( fExtentX != (Real)0.0 ? iSegmentsX : 0 );
    int iSegmentsWkY = ( fExtentY != (Real)0.0 ? iSegmentsY : 0 );
    int iSegmentsWkZ = ( fExtentZ != (Real)0.0 ? iSegmentsZ : 0 );
    ModestArray<Real> qExtents;
    if ( fExtentX != (Real)0.0 )
        qExtents.Push(fExtentX);
    if ( fExtentY != (Real)0.0 )
        qExtents.Push(fExtentY);
    if ( fExtentZ != (Real)0.0 )
        qExtents.Push(fExtentZ);

    Real fRadiusCorr = (Real)0.0;
    Real fExtentCorrX = fExtentX;
    Real fExtentCorrY = fExtentY;
    Real fExtentCorrZ = fExtentZ;
    if ( fRadius != (Real)0.0 && qExtents.GetQuantity() > 0 )
    {
        SortUtils<Real>::Sort(qExtents);
        Real fHalfMinExtent = qExtents[0]/2;
        fRadiusCorr = ( fRadius > fHalfMinExtent ? fHalfMinExtent : fRadius );
        Real fVar0 = 2*fRadiusCorr;
        fExtentCorrX -= fVar0;
        fExtentCorrY -= fVar0;
        fExtentCorrZ -= fVar0;
    }

    Real fFractionX = ( iSegmentsWkX != 0 ? fExtentCorrX/iSegmentsWkX : 0 );
    Real fFractionY = ( iSegmentsWkY != 0 ? fExtentCorrY/iSegmentsWkY : 0 );
    Real fFractionZ = ( iSegmentsWkZ != 0 ? fExtentCorrZ/iSegmentsWkZ : 0 );
    Real fHalfExtentX = fExtentX/2;
    Real fHalfExtentY = fExtentY/2;
    Real fHalfExtentZ = fExtentZ/2;
    Real fHalfXExtentCorr = fExtentCorrX/2;
    Real fHalfYExtentCorr = fExtentCorrY/2;
    Real fHalfZExtentCorr = fExtentCorrZ/2;
    Real fTrigRadiusFraction = Math<Real>::HALF_PI/iRadiusSegments;

    int iVertexQuantity, iTriQuantity;
    if ( fRadius == (Real)0.0 )
    {
        iVertexQuantity =
            (iSegmentsWkX + 1)*(iSegmentsWkZ + 1)*( fExtentY != (Real)0.0 ? 2 : 1 ) +
            (iSegmentsWkX + 1)*(iSegmentsWkY + 1)*( fExtentZ != (Real)0.0 ? 2 : 1 ) +
            (iSegmentsWkY + 1)*(iSegmentsWkZ + 1)*( fExtentX != (Real)0.0 ? 2 : 1 );
        iTriQuantity = (
            iSegmentsWkX*iSegmentsWkZ*( fExtentY != (Real)0.0 ? 2 : 1 ) +
            iSegmentsWkX*iSegmentsWkY*( fExtentZ != (Real)0.0 ? 2 : 1 ) +
            iSegmentsWkY*iSegmentsWkZ*( fExtentX != (Real)0.0 ? 2 : 1 ))*2;
    }
    else
    {
        if ( qExtents.GetQuantity() == 3 )
        {
            iVertexQuantity = (
                (iSegmentsWkX + 1)*(iSegmentsWkZ + 1) +
                (iSegmentsWkX + 1)*(iSegmentsWkY + 1) +
                (iSegmentsWkY + 1)*(iSegmentsWkZ + 1))*2 + (
                (iSegmentsWkZ + 1)*(iRadiusSegments - 1) +
                (iSegmentsWkX + 1)*(iRadiusSegments - 1) +
                (iSegmentsWkY + 1)*(iRadiusSegments - 1))*4 +
                Mathi::Sqr(iRadiusSegments-1)*8;
            iTriQuantity = (
                iSegmentsWkX*iSegmentsWkZ +
                iSegmentsWkX*iSegmentsWkY +
                iSegmentsWkY*iSegmentsWkZ)*4 + (
                iSegmentsWkZ*iRadiusSegments +
                iSegmentsWkX*iRadiusSegments +
                iSegmentsWkY*iRadiusSegments)*8 +
                (2*Mathi::Sqr(iRadiusSegments-2) +
                7*iRadiusSegments - 8)*8;
        }
        else
        {
            iVertexQuantity = 0;
            iTriQuantity = 0;
            if ( fExtentY == (Real)0.0 )
            {
                iVertexQuantity += (iSegmentsWkX + 1)*(iSegmentsWkZ + 1) + (iSegmentsWkX + iSegmentsWkZ + 2)*2 +
                    (iRadiusSegments - 1)*4;
                iTriQuantity += (iSegmentsWkX*iSegmentsWkZ + iSegmentsWkX*2 + iSegmentsWkZ*2)*2 +
                    iRadiusSegments*4;
            }
            if ( fExtentZ == (Real)0.0 )
            {
                iVertexQuantity += (iSegmentsWkX + 1)*(iSegmentsWkY + 1) + (iSegmentsWkX + iSegmentsWkY + 2)*2 +
                    (iRadiusSegments - 1)*4;
                iTriQuantity += (iSegmentsWkX*iSegmentsWkY + iSegmentsWkX*2 + iSegmentsWkY*2)*2 +
                    iRadiusSegments*4;
            }
            if ( fExtentX == (Real)0.0 )
            {
                iVertexQuantity += (iSegmentsWkY + 1)*(iSegmentsWkZ + 1) + (iSegmentsWkY + iSegmentsWkZ + 2)*2 +
                    (iRadiusSegments - 1)*4;
                iTriQuantity += (iSegmentsWkY*iSegmentsWkZ + iSegmentsWkY*2 + iSegmentsWkZ*2)*2 +
                    iRadiusSegments*4;
            }
        }
    }

    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];
    int iIndexQuantity = 3*iTriQuantity;
    int* aiIndex = new int[iIndexQuantity];

    int iIQuantity = 0;
    int iVQuantity = 0;
    int iV0, iV1, iV2, iV3;
    int iVar0, iVar1;
    Real fCurrentX, fCurrentY, fVar0;
    Real* afRoundCos;
    Real* afRoundSin;
    if ( fRadius != (Real)0.0 )
    {
        afRoundCos = new Real[iRadiusSegments];
        afRoundSin = new Real[iRadiusSegments];
        for (int i = 1; i < iRadiusSegments; i++)
        {
            fVar0 = i*fTrigRadiusFraction;
            afRoundCos[i] = Math<Real>::Cos(fVar0);
            afRoundSin[i] = Math<Real>::Sin(fVar0);
        }
    }
    if ( !(qExtents.GetQuantity() < 3 && fRadius != (Real)0.0) )
    {
        // side 1
        int iSide1Begin = iVQuantity;
        if ( fExtentY != (Real)0.0 )
        {
            for (int i0 = 0; i0 < iSegmentsWkX; i0++)
            {
                iVar0 = iSide1Begin + i0*(iSegmentsWkZ + 1);
                for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                {
                    iV0 = iVar0 + i1;
                    iV1 = iV0 + iSegmentsWkZ + 1;
                    iV2 = iV1 + 1;
                    iV3 = iV0 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
            {
                fCurrentX = i0*fFractionX - fHalfXExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        fCurrentX,
                        -fHalfExtentY,
                        i1*fFractionZ-fHalfZExtentCorr);
                }
            }
        }

        // side 2
        int iSide2Begin = iVQuantity;
        for (int i0 = 0; i0 < iSegmentsWkX; i0++)
        {
            iVar0 = iSide2Begin + i0*(iSegmentsWkZ + 1);
            for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
            {
                iV0 = iVar0 + i1 + iSegmentsWkZ + 1;
                iV1 = iVar0 + i1;
                iV2 = iV1 + 1;
                iV3 = iV0 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
        }
        for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
        {
            fCurrentX = i0*fFractionX - fHalfXExtentCorr;
            for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fCurrentX,
                    fHalfExtentY,
                    i1*fFractionZ-fHalfZExtentCorr);
            }
        }

        // side 3
        int iSide3Begin = iVQuantity;
        if ( fExtentZ != (Real)0.0 )
        {
            for (int i0 = 0; i0 < iSegmentsWkX; i0++)
            {
                iVar0 = iSide3Begin + i0*(iSegmentsWkY + 1);
                for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                {
                    iV0 = iVar0 + i1 + iSegmentsWkY + 1;
                    iV1 = iVar0 + i1;
                    iV2 = iV1 + 1;
                    iV3 = iV0 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
            {
                fCurrentX = i0*fFractionX - fHalfXExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        fCurrentX,
                        i1*fFractionY-fHalfYExtentCorr,
                        -fHalfExtentZ);
                }
            }
        }

        // side 4
        int iSide4Begin = iVQuantity;
        for (int i0 = 0; i0 < iSegmentsWkX; i0++)
        {
            iVar0 = iSide4Begin + i0*(iSegmentsWkY + 1);
            for (int i1 = 0; i1 < iSegmentsWkY; i1++)
            {
                iV0 = iVar0 + i1;
                iV1 = iV0 + iSegmentsWkY + 1;
                iV2 = iV1 + 1;
                iV3 = iV0 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
        }
        for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
        {
            fCurrentX = i0*fFractionX - fHalfXExtentCorr;
            for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fCurrentX,
                    i1*fFractionY-fHalfYExtentCorr,
                    fHalfExtentZ);
            }
        }

        // side 5
        int iSide5Begin = iVQuantity;
        if ( fExtentX != (Real)0.0 )
        {
            for (int i0 = 0; i0 < iSegmentsWkY; i0++)
            {
                iVar0 = iSide5Begin + i0*(iSegmentsWkZ + 1);
                for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                {
                    iV0 = iVar0 + i1;
                    iV1 = iV0 + 1;
                    iV2 = iV1 + iSegmentsWkZ + 1;
                    iV3 = iV2 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkY; i0++)
            {
                fCurrentY = i0*fFractionY - fHalfYExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        -fHalfExtentX,
                        fCurrentY,
                        i1*fFractionZ-fHalfZExtentCorr);
                }
            }
        }

        // side 6
        int iSide6Begin = iVQuantity;
        for (int i0 = 0; i0 < iSegmentsWkY; i0++)
        {
            iVar0 = iSide6Begin + i0*(iSegmentsWkZ + 1);
            for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
            {
                iV0 = iVar0 + i1 + 1;
                iV1 = iV0 - 1;
                iV2 = iV0 + iSegmentsWkZ;
                iV3 = iV2 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
        }
        for (int i0 = 0; i0 <= iSegmentsWkY; i0++)
        {
            fCurrentY = i0*fFractionY - fHalfYExtentCorr;
            for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfExtentX,
                    fCurrentY,
                    i1*fFractionZ-fHalfZExtentCorr);
            }
        }
        int iBoxSize = iVQuantity;
        if ( fRadius != (Real)0.0 && qExtents.GetQuantity() == 3 )
        {
            StillArray<Array<int> > qRoundThings(24);

            // round edge 1
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide5Begin + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iVQuantity + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkZ + 1);
                    iVar1 = iVar0 + iSegmentsWkZ + 1;
                    for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkZ + 1) + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide1Begin + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide5Begin + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide1Begin + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr+i1*fFractionZ);
                    if ( i1 == 0 )
                        qRoundThings[0].Push(iVQuantity);
                    if ( i1 == iSegmentsWkZ )
                        qRoundThings[1].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 2
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide4Begin + (i + 1)*iSegmentsWkY + i + 1;
                    iV1 = iV0 - iSegmentsWkY - 1;
                    iV2 = iVQuantity + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkX + 1);
                    iVar1 = iVar0 + iSegmentsWkX + 1;
                    for (int i1 = 0; i1 < iSegmentsWkX; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkX + 1) + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide1Begin + (i + 1)*iSegmentsWkZ + i;
                    iV3 = iV2 + iSegmentsWkZ + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide4Begin + (i + 1)*iSegmentsWkY + i + 1;
                    iV1 = iV0 - iSegmentsWkY - 1;
                    iV2 = iSide1Begin + (i + 1)*iSegmentsWkZ + i;
                    iV3 = iV2 + iSegmentsWkZ + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkX; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr+i1*fFractionX,
                        -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                        fHalfZExtentCorr+fRadiusCorr*afRoundCos[i0]);
                    if ( i1 == 0 )
                        qRoundThings[2].Push(iVQuantity);
                    if ( i1 == iSegmentsWkX )
                        qRoundThings[3].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 3
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide6Begin + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkZ + 1);
                    iVar1 = iVar0 - iSegmentsWkZ - 1;
                    for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide2Begin - iSegmentsWkZ + i;
                    iV1 = iV0 - 1;
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkZ + 1) + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide2Begin - iSegmentsWkZ + i;
                    iV1 = iV0 - 1;
                    iV2 = iSide6Begin + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfXExtentCorr+fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr+i1*fFractionZ);
                    if ( i1 == 0 )
                        qRoundThings[4].Push(iVQuantity);
                    if ( i1 == iSegmentsWkZ )
                        qRoundThings[5].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 4
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide3Begin + i*(iSegmentsWkY + 1);
                    iV3 = iV2 + iSegmentsWkY + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkX + 1);
                    iVar1 = iVar0 - iSegmentsWkX - 1;
                    for (int i1 = 0; i1 < iSegmentsWkX; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide1Begin + (i + 1)*(iSegmentsWkZ + 1);
                    iV1 = iV0 - iSegmentsWkZ - 1;
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkX + 1) + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide1Begin + (i + 1)*(iSegmentsWkZ + 1);
                    iV1 = iV0 - iSegmentsWkZ - 1;
                    iV2 = iSide3Begin + i*(iSegmentsWkY + 1);
                    iV3 = iV2 + iSegmentsWkY + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkX; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr+i1*fFractionX,
                        -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i0]);
                    if ( i1 == 0 )
                        qRoundThings[6].Push(iVQuantity);
                    if ( i1 == iSegmentsWkX )
                        qRoundThings[7].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 5
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide6Begin - iSegmentsWkZ + i - 1;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkZ + 1);
                    iVar1 = iVar0 - iSegmentsWkZ - 1;
                    for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide2Begin + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkZ + 1) + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iSide2Begin + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide6Begin - iSegmentsWkZ + i - 1;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i0],
                        fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr+i1*fFractionZ);
                    if ( i1 == 0 )
                        qRoundThings[8].Push(iVQuantity);
                    if ( i1 == iSegmentsWkZ )
                        qRoundThings[9].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 6
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide4Begin + (i + 1)*iSegmentsWkY + i;
                    iV3 = iV2 + iSegmentsWkY + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkX + 1);
                    iVar1 = iVar0 - iSegmentsWkX - 1;
                    for (int i1 = 0; i1 < iSegmentsWkX; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide2Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV1 = iV0 - iSegmentsWkZ - 1;
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkX + 1) + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide2Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV1 = iV0 - iSegmentsWkZ - 1;
                    iV2 = iSide4Begin + (i + 1)*iSegmentsWkY + i;
                    iV3 = iV2 + iSegmentsWkY + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkX; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr+i1*fFractionX,
                        fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                        fHalfZExtentCorr+fRadiusCorr*afRoundCos[i0]);
                    if ( i1 == 0 )
                        qRoundThings[10].Push(iVQuantity);
                    if ( i1 == iSegmentsWkX )
                        qRoundThings[11].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 7
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iBoxSize - iSegmentsWkZ + i;
                    iV1 = iV0 - 1;
                    iV2 = iVQuantity + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkZ + 1);
                    iVar1 = iVar0 + iSegmentsWkZ + 1;
                    for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkZ + 1) + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide3Begin - 1 - iSegmentsWkZ + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkZ; i++)
                {
                    iV0 = iBoxSize - iSegmentsWkZ + i;
                    iV1 = iV0 - 1;
                    iV2 = iSide3Begin - 1 - iSegmentsWkZ + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfXExtentCorr+fRadiusCorr*afRoundCos[i0],
                        fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr+i1*fFractionZ);
                    if ( i1 == 0 )
                        qRoundThings[12].Push(iVQuantity);
                    if ( i1 == iSegmentsWkZ )
                        qRoundThings[13].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 8
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide3Begin + (i + 2)*iSegmentsWkY + i + 1;
                    iV1 = iV0 - iSegmentsWkY - 1;
                    iV2 = iVQuantity + i;
                    iV3 = iV2 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkX + 1);
                    iVar1 = iVQuantity + (i0 + 1)*(iSegmentsWkX + 1);
                    for (int i1 = 0; i1 < iSegmentsWkX; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iV0 - 1;
                        iV2 = iVar1 + i1;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkX + 1) + i + 1;
                    iV1 = iV0 - 1;
                    iV2 = iSide2Begin + i*iSegmentsWkZ + i;
                    iV3 = iV2 + iSegmentsWkZ + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkX; i++)
                {
                    iV0 = iSide3Begin + (i + 2)*iSegmentsWkY + i + 1;
                    iV1 = iV0 - iSegmentsWkY - 1;
                    iV2 = iSide2Begin + i*iSegmentsWkZ + i;
                    iV3 = iV2 + iSegmentsWkZ + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkX; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr+i1*fFractionX,
                        fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                        -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i0]);
                    if ( i1 == 0 )
                        qRoundThings[14].Push(iVQuantity);
                    if ( i1 == iSegmentsWkX )
                        qRoundThings[15].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 9
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide5Begin + (i + 1)*iSegmentsWkZ + i + 1;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - iSegmentsWkZ - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkY + 1);
                    iVar1 = iVar0 + iSegmentsWkY + 1;
                    for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iVar1 + i1 + 1;
                        iV2 = iV1 - 1;
                        iV3 = iV0 - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkY + 1) + i + 1;
                    iV1 = iSide3Begin + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide5Begin + (i + 1)*iSegmentsWkZ + i + 1;
                    iV1 = iSide3Begin + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - iSegmentsWkZ - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr+i1*fFractionY,
                        -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                    if ( i1 == 0 )
                        qRoundThings[16].Push(iVQuantity);
                    if ( i1 == iSegmentsWkY )
                        qRoundThings[17].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 10
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iSide5Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV2 = iV1 - iSegmentsWkZ - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkY + 1);
                    iVar1 = iVar0 - iSegmentsWkY - 1;
                    for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iVar1 + i1 + 1;
                        iV2 = iV1 - 1;
                        iV3 = iV0 - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide4Begin + i + 1;
                    iV1 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkY + 1) + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide4Begin + i + 1;
                    iV1 = iSide5Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV2 = iV1 - iSegmentsWkZ - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr+i1*fFractionY,
                        fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                    if ( i1 == 0 )
                        qRoundThings[18].Push(iVQuantity);
                    if ( i1 == iSegmentsWkY )
                        qRoundThings[19].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 11
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide6Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - iSegmentsWkZ - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + i0*(iSegmentsWkY + 1);
                    iVar1 = iVar0 + iSegmentsWkY + 1;
                    for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iVar1 + i1 + 1;
                        iV2 = iV1 - 1;
                        iV3 = iV0 - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkY + 1) + i + 1;
                    iV1 = iSide5Begin - iSegmentsWkY + i;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide6Begin + (i + 2)*iSegmentsWkZ + i + 1;
                    iV1 = iSide5Begin - iSegmentsWkY + i;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - iSegmentsWkZ - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfXExtentCorr+fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr+i1*fFractionY,
                        fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                    if ( i1 == 0 )
                        qRoundThings[20].Push(iVQuantity);
                    if ( i1 == iSegmentsWkY )
                        qRoundThings[21].Push(iVQuantity);

                    iVQuantity++;
                }
            }

            // round edge 12
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iVQuantity + i + 1;
                    iV1 = iSide6Begin + (i + 1)*iSegmentsWkZ + i + 1;
                    iV2 = iV1 - iSegmentsWkZ - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
                for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                {
                    iVar0 = iVQuantity + (i0 + 1)*(iSegmentsWkY + 1);
                    iVar1 = iVar0 - iSegmentsWkY - 1;
                    for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                    {
                        iV0 = iVar0 + i1 + 1;
                        iV1 = iVar1 + i1 + 1;
                        iV2 = iV1 - 1;
                        iV3 = iV0 - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                }
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide4Begin - iSegmentsWkY + i;
                    iV1 = iVQuantity + (iRadiusSegments - 2)*(iSegmentsWkY + 1) + i + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            else
            {
                for (int i = 0; i < iSegmentsWkY; i++)
                {
                    iV0 = iSide4Begin - iSegmentsWkY + i;
                    iV1 = iSide6Begin + (i + 1)*iSegmentsWkZ + i + 1;
                    iV2 = iV1 - iSegmentsWkZ - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 1; i0 < iRadiusSegments; i0++)
            {
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfXExtentCorr+fRadiusCorr*afRoundCos[i0],
                        -fHalfYExtentCorr+i1*fFractionY,
                        -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                    if ( i1 == 0 )
                        qRoundThings[22].Push(iVQuantity);
                    if ( i1 == iSegmentsWkY )
                        qRoundThings[23].Push(iVQuantity);

                    iVQuantity++;
                }
            }
            if ( eAxis == AT_AXIS_X )
            {
                // round corner 1
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[6][iRadiusSegments-2];
                    iV1 = iSide1Begin;
                    iV2 = qRoundThings[0][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide3Begin;
                    iV1 = qRoundThings[6][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[16][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[6][iRadiusSegments-3-i];
                        iV1 = qRoundThings[6][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[0][iRadiusSegments-2-i];
                        iV2 = qRoundThings[0][iRadiusSegments-3-i];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[16][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[16][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[0][0];
                    iV2 = iSide5Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[16][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide5Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide5Begin;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide5Begin;
                    iV1 = iSide3Begin;
                    iV2 = iSide1Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 2
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[2][0];
                    iV1 = iSide4Begin;
                    iV2 = qRoundThings[18][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide1Begin + iSegmentsWkZ;
                    iV1 = qRoundThings[2][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[1][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[2][i+1];
                        iV1 = qRoundThings[2][i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[18][iRadiusSegments-2-i];
                        iV2 = qRoundThings[18][iRadiusSegments-3-i];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[1][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[1][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[18][0];
                    iV2 = iSide5Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[1][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide5Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide5Begin + iSegmentsWkZ;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin;
                    iV1 = iSide5Begin + iSegmentsWkZ;
                    iV2 = iSide1Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 3
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[3][iRadiusSegments-2];
                    iV1 = iSide2Begin - 1;
                    iV2 = qRoundThings[5][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide5Begin - 1 - iSegmentsWkY;
                    iV1 = qRoundThings[3][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[20][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[3][iRadiusSegments-3-i];
                        iV1 = qRoundThings[3][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[5][iRadiusSegments-2-i];
                        iV2 = qRoundThings[5][iRadiusSegments-3-i];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[20][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[20][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[5][0];
                    iV2 = iSide6Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[20][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide6Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide6Begin + iSegmentsWkZ;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide6Begin + iSegmentsWkZ;
                    iV1 = iSide5Begin - 1 - iSegmentsWkY;
                    iV2 = iSide2Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 4
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[7][0];
                    iV1 = iSide4Begin - 1 - iSegmentsWkY;
                    iV2 = qRoundThings[22][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide2Begin - 1 - iSegmentsWkZ;
                    iV1 = qRoundThings[7][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[4][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[7][i+1];
                        iV1 = qRoundThings[7][i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[22][iRadiusSegments-2-i];
                        iV2 = qRoundThings[22][iRadiusSegments-3-i];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[4][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[4][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[22][0];
                    iV2 = iSide6Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[4][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide6Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide6Begin;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin - 1 - iSegmentsWkY;
                    iV1 = iSide6Begin;
                    iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 5
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[8][iRadiusSegments-2];
                    iV2 = iSide2Begin;
                    iV3 = qRoundThings[14][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[17][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[14][0];
                    iV3 = iSide3Begin + iSegmentsWkY;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[14][iRadiusSegments-2-i];
                        iV3 = qRoundThings[14][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[8][iRadiusSegments-3-i];
                        iV2 = qRoundThings[8][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[17][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[17][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide6Begin - 1 - iSegmentsWkZ;
                    iV1 = qRoundThings[8][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide6Begin - 1 - iSegmentsWkZ;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[17][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide6Begin - 1 - iSegmentsWkZ;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin;
                    iV1 = iSide3Begin + iSegmentsWkY;
                    iV2 = iSide6Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 6
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[19][iRadiusSegments-2];
                    iV2 = iSide4Begin + iSegmentsWkY;
                    iV3 = qRoundThings[10][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[9][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[10][iRadiusSegments-2];
                    iV3 = iSide2Begin + iSegmentsWkZ;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[10][i];
                        iV3 = qRoundThings[10][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[19][iRadiusSegments-3-i];
                        iV2 = qRoundThings[19][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[9][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[9][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide6Begin - 1;
                    iV1 = qRoundThings[19][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide6Begin - 1;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[9][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide6Begin - 1;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin + iSegmentsWkZ;
                    iV1 = iSide6Begin - 1;
                    iV2 = iSide4Begin + iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 7
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[13][iRadiusSegments-2];
                    iV2 = iSide3Begin - 1;
                    iV3 = qRoundThings[11][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[21][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[11][0];
                    iV3 = iSide5Begin - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[11][iRadiusSegments-2-i];
                        iV3 = qRoundThings[11][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[13][iRadiusSegments-3-i];
                        iV2 = qRoundThings[13][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[21][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[21][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iBoxSize - 1;
                    iV1 = qRoundThings[13][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iBoxSize - 1;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[21][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iBoxSize - 1;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1;
                    iV1 = iSide5Begin - 1;
                    iV2 = iBoxSize - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 8
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[23][iRadiusSegments-2];
                    iV2 = iSide4Begin - 1;
                    iV3 = qRoundThings[15][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[12][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[15][iRadiusSegments-2];
                    iV3 = iSide3Begin - 1 - iSegmentsWkZ;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[15][i];
                        iV3 = qRoundThings[15][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[23][iRadiusSegments-3-i];
                        iV2 = qRoundThings[23][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[12][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[12][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iBoxSize - 1 - iSegmentsWkZ;
                    iV1 = qRoundThings[23][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iBoxSize - 1 - iSegmentsWkZ;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[12][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iBoxSize - 1 - iSegmentsWkZ;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV1 = iBoxSize - 1 - iSegmentsWkZ;
                    iV2 = iSide4Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }
            }
            else if ( eAxis == AT_AXIS_Y )
            {
                // round corner 1
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[16][iRadiusSegments-2];
                    iV1 = iSide3Begin;
                    iV2 = qRoundThings[6][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide5Begin;
                    iV1 = qRoundThings[16][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[0][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[16][iRadiusSegments-3-i];
                        iV1 = qRoundThings[16][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[6][i];
                        iV2 = qRoundThings[6][i+1];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[0][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[0][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[6][iRadiusSegments-2];
                    iV2 = iSide1Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[0][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide1Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide1Begin;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide5Begin;
                    iV1 = iSide3Begin;
                    iV2 = iSide1Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 2
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[18][0];
                    iV1 = iSide5Begin + iSegmentsWkZ;
                    iV2 = qRoundThings[1][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide4Begin;
                    iV1 = qRoundThings[18][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[2][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[18][i+1];
                        iV1 = qRoundThings[18][i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[1][i];
                        iV2 = qRoundThings[1][i+1];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[2][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[2][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[1][iRadiusSegments-2];
                    iV2 = iSide1Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[2][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide1Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide1Begin + iSegmentsWkZ;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin;
                    iV1 = iSide5Begin + iSegmentsWkZ;
                    iV2 = iSide1Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 3
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[20][iRadiusSegments-2];
                    iV1 = iSide5Begin - 1 - iSegmentsWkY;
                    iV2 = qRoundThings[3][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide6Begin + iSegmentsWkZ;
                    iV1 = qRoundThings[20][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[5][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[20][iRadiusSegments-3-i];
                        iV1 = qRoundThings[20][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[3][i];
                        iV2 = qRoundThings[3][i+1];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[5][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[5][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[3][iRadiusSegments-2];
                    iV2 = iSide2Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[5][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide2Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide2Begin - 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide6Begin + iSegmentsWkZ;
                    iV1 = iSide5Begin - 1 - iSegmentsWkY;
                    iV2 = iSide2Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 4
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[22][0];
                    iV1 = iSide6Begin;
                    iV2 = qRoundThings[4][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide4Begin - 1 - iSegmentsWkY;
                    iV1 = qRoundThings[22][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[7][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[22][i+1];
                        iV1 = qRoundThings[22][i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[4][i];
                        iV2 = qRoundThings[4][i+1];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[7][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[7][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[4][iRadiusSegments-2];
                    iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[7][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin - 1 - iSegmentsWkY;
                    iV1 = iSide6Begin;
                    iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 5
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[14][0];
                    iV2 = iSide3Begin + iSegmentsWkY;
                    iV3 = qRoundThings[17][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[8][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[17][0];
                    iV3 = iSide6Begin - 1 - iSegmentsWkZ;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[17][iRadiusSegments-2-i];
                        iV3 = qRoundThings[17][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[14][i+1];
                        iV2 = qRoundThings[14][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[8][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[8][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide2Begin;
                    iV1 = qRoundThings[14][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide2Begin;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[8][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide2Begin;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin;
                    iV1 = iSide3Begin + iSegmentsWkY;
                    iV2 = iSide6Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 6
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[9][0];
                    iV2 = iSide6Begin - 1;
                    iV3 = qRoundThings[19][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[10][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[19][iRadiusSegments-2];
                    iV3 = iSide4Begin + iSegmentsWkY;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[19][i];
                        iV3 = qRoundThings[19][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[9][i+1];
                        iV2 = qRoundThings[9][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[10][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[10][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide2Begin + iSegmentsWkZ;
                    iV1 = qRoundThings[9][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide2Begin + iSegmentsWkZ;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[10][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide2Begin + iSegmentsWkZ;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin + iSegmentsWkZ;
                    iV1 = iSide6Begin - 1;
                    iV2 = iSide4Begin + iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 7
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[11][0];
                    iV2 = iSide5Begin - 1;
                    iV3 = qRoundThings[21][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[13][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[21][0];
                    iV3 = iBoxSize - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[21][iRadiusSegments-2-i];
                        iV3 = qRoundThings[21][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[11][i+1];
                        iV2 = qRoundThings[11][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[13][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[13][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide3Begin - 1;
                    iV1 = qRoundThings[11][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide3Begin - 1;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[13][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide3Begin - 1;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1;
                    iV1 = iSide5Begin - 1;
                    iV2 = iBoxSize - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 8
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[12][0];
                    iV2 = iBoxSize - 1 - iSegmentsWkZ;
                    iV3 = qRoundThings[23][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[15][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[23][iRadiusSegments-2];
                    iV3 = iSide4Begin - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[23][i];
                        iV3 = qRoundThings[23][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[12][i+1];
                        iV2 = qRoundThings[12][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[15][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[15][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV1 = qRoundThings[12][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[15][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV1 = iBoxSize - 1 - iSegmentsWkZ;
                    iV2 = iSide4Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0]);
                        iVQuantity++;
                    }
                }
            }
            else  // eAxis = AT_AXIS_Z
            {
                // round corner 1
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[0][0];
                    iV1 = iSide5Begin;
                    iV2 = qRoundThings[16][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide1Begin;
                    iV1 = qRoundThings[0][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[6][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[0][i+1];
                        iV1 = qRoundThings[0][i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[16][i];
                        iV2 = qRoundThings[16][i+1];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[6][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[6][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[16][iRadiusSegments-2];
                    iV2 = iSide3Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[6][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide3Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide3Begin;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide5Begin;
                    iV1 = iSide3Begin;
                    iV2 = iSide1Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 2
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[1][iRadiusSegments-2];
                    iV1 = iSide1Begin + iSegmentsWkZ;
                    iV2 = qRoundThings[2][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide5Begin + iSegmentsWkZ;
                    iV1 = qRoundThings[1][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[18][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[1][iRadiusSegments-3-i];
                        iV1 = qRoundThings[1][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iV2 + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[2][iRadiusSegments-2-i];
                        iV2 = qRoundThings[2][iRadiusSegments-3-i];
                        iV3 = iV0 + iRadiusSegments - 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[18][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iV1 + iRadiusSegments - 1;
                        iV3 = qRoundThings[18][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[2][0];
                    iV2 = iSide4Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[18][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide4Begin;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide4Begin;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin;
                    iV1 = iSide5Begin + iSegmentsWkZ;
                    iV2 = iSide1Begin + iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 3
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[5][0];
                    iV1 = iSide6Begin + iSegmentsWkZ;
                    iV2 = qRoundThings[20][0];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide2Begin - 1;
                    iV1 = qRoundThings[5][iRadiusSegments-2];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[3][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[5][i+1];
                        iV1 = qRoundThings[5][i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[20][i];
                        iV2 = qRoundThings[20][i+1];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[3][iRadiusSegments-2-i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[3][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[20][iRadiusSegments-2];
                    iV2 = iSide5Begin - 1 - iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[3][0];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide5Begin - 1 - iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide5Begin - 1 - iSegmentsWkY;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide6Begin + iSegmentsWkZ;
                    iV1 = iSide5Begin - 1 - iSegmentsWkY;
                    iV2 = iSide2Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 4
                if ( iRadiusSegments > 1 )
                {
                    iV0 = qRoundThings[4][iRadiusSegments-2];
                    iV1 = iSide2Begin - 1 - iSegmentsWkZ;
                    iV2 = qRoundThings[7][iRadiusSegments-2];
                    iV3 = iVQuantity;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = iSide6Begin;
                    iV1 = qRoundThings[4][0];
                    iV2 = iVQuantity + iRadiusSegments - 2;
                    iV3 = qRoundThings[22][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[4][iRadiusSegments-3-i];
                        iV1 = qRoundThings[4][iRadiusSegments-2-i];
                        iV2 = iVQuantity + i;
                        iV3 = iVQuantity + i + 1;
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i*(iRadiusSegments - 1);
                        iV1 = qRoundThings[7][iRadiusSegments-2-i];
                        iV2 = qRoundThings[7][iRadiusSegments-3-i];
                        iV3 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[22][i];
                        iV1 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[22][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    iV1 = qRoundThings[7][0];
                    iV2 = iSide4Begin - 1 - iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = qRoundThings[22][iRadiusSegments-2];
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = iSide4Begin - 1 - iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = iSide4Begin - 1 - iSegmentsWkY;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + i0*(iRadiusSegments - 1);
                        iVar1 = iVar0 + iRadiusSegments - 1;
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide4Begin - 1 - iSegmentsWkY;
                    iV1 = iSide6Begin;
                    iV2 = iSide2Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfYExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 5
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[17][0];
                    iV2 = iSide6Begin - 1 - iSegmentsWkZ;
                    iV3 = qRoundThings[8][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[14][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[8][iRadiusSegments-2];
                    iV3 = iSide2Begin;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[8][i];
                        iV3 = qRoundThings[8][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[17][i+1];
                        iV2 = qRoundThings[17][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[14][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[14][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide3Begin + iSegmentsWkY;
                    iV1 = qRoundThings[17][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide3Begin + iSegmentsWkY;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[14][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide3Begin + iSegmentsWkY;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin;
                    iV1 = iSide3Begin + iSegmentsWkY;
                    iV2 = iSide6Begin - 1 - iSegmentsWkZ;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 6
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[10][iRadiusSegments-2];
                    iV2 = iSide2Begin + iSegmentsWkZ;
                    iV3 = qRoundThings[9][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[19][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[9][0];
                    iV3 = iSide6Begin - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iV0 - 1;
                        iV2 = qRoundThings[9][iRadiusSegments-2-i];
                        iV3 = qRoundThings[9][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[10][iRadiusSegments-3-i];
                        iV2 = qRoundThings[10][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[19][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[19][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide4Begin + iSegmentsWkY;
                    iV1 = qRoundThings[10][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide4Begin + iSegmentsWkY;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[19][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide4Begin + iSegmentsWkY;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide2Begin + iSegmentsWkZ;
                    iV1 = iSide6Begin - 1;
                    iV2 = iSide4Begin + iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            -fHalfXExtentCorr-fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 7
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[21][0];
                    iV2 = iBoxSize - 1;
                    iV3 = qRoundThings[13][0];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[11][iRadiusSegments-2];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[13][iRadiusSegments-2];
                    iV3 = iSide3Begin - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[13][i];
                        iV3 = qRoundThings[13][i+1];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[21][i+1];
                        iV2 = qRoundThings[21][i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[11][iRadiusSegments-3-i];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[11][iRadiusSegments-2-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide5Begin - 1;
                    iV1 = qRoundThings[21][iRadiusSegments-2];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide5Begin - 1;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[11][0];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide5Begin - 1;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1;
                    iV1 = iSide5Begin - 1;
                    iV2 = iBoxSize - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfZExtentCorr+fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }

                // round corner 8
                if ( iRadiusSegments > 1 )
                {
                    iV0 = iVQuantity;
                    iV1 = qRoundThings[15][iRadiusSegments-2];
                    iV2 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV3 = qRoundThings[12][iRadiusSegments-2];
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    iV0 = qRoundThings[23][0];
                    iV1 = iVQuantity + iRadiusSegments - 2;
                    iV2 = qRoundThings[12][0];
                    iV3 = iBoxSize - 1 - iSegmentsWkZ;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + i + 1;
                        iV1 = iVQuantity + i;
                        iV2 = qRoundThings[12][iRadiusSegments-2-i];
                        iV3 = qRoundThings[12][iRadiusSegments-3-i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iVQuantity + (i + 1)*(iRadiusSegments - 1);
                        iV1 = qRoundThings[15][iRadiusSegments-3-i];
                        iV2 = qRoundThings[15][iRadiusSegments-2-i];
                        iV3 = iVQuantity + i*(iRadiusSegments - 1);
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = qRoundThings[23][i+1];
                        iV1 = iVQuantity + (i + 2)*(iRadiusSegments - 1) - 1;
                        iV2 = iVQuantity + (i + 1)*(iRadiusSegments - 1) - 1;
                        iV3 = qRoundThings[23][i];
                        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                    }
                    iV0 = iSide4Begin - 1;
                    iV1 = qRoundThings[15][0];
                    iV2 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1);
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    iV0 = iSide4Begin - 1;
                    iV1 = iVQuantity + Mathi::Sqr(iRadiusSegments-1) - 1;
                    iV2 = qRoundThings[23][iRadiusSegments-2];
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    for (int i = 0; i < iRadiusSegments-2; i++)
                    {
                        iV0 = iSide4Begin - 1;
                        iV1 = iVQuantity + (iRadiusSegments - 2)*(iRadiusSegments - 1) + i;
                        iV2 = iV1 + 1;
                        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                    }
                    for (int i0 = 0; i0 < iRadiusSegments-2; i0++)
                    {
                        iVar0 = iVQuantity + (i0 + 1)*(iRadiusSegments - 1);
                        iVar1 = iVQuantity + i0*(iRadiusSegments - 1);
                        for (int i1 = 0; i1 < iRadiusSegments-2; i1++)
                        {
                            iV0 = iVar0 + i1 + 1;
                            iV1 = iV0 - 1;
                            iV2 = iVar1 + i1;
                            iV3 = iV2 + 1;
                            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                        }
                    }
                }
                else
                {
                    iV0 = iSide3Begin - 1 - iSegmentsWkZ;
                    iV1 = iBoxSize - 1 - iSegmentsWkZ;
                    iV2 = iSide4Begin - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                for (int i0 = 1; i0 < iRadiusSegments; i0++)
                {
                    for (int i1 = 1; i1 < iRadiusSegments; i1++)
                    {
                        aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                            fHalfXExtentCorr+fRadiusCorr*afRoundSin[i1]*afRoundCos[i0],
                            fHalfYExtentCorr+fRadiusCorr*afRoundCos[i1]*afRoundCos[i0],
                            -fHalfZExtentCorr-fRadiusCorr*afRoundSin[i0]);
                        iVQuantity++;
                    }
                }
            }
        }
    }
    else
    {
        if ( fExtentX == (Real)0.0
        &&   fExtentY != (Real)0.0
        &&   fExtentZ != (Real)0.0 )
        {
            // center thing
            for (int i0 = 0; i0 < iSegmentsWkY; i0++)
            {
                iVar0 = (i0 + 1)*(iSegmentsWkZ + 1);
                iVar1 = i0*(iSegmentsWkZ + 1);
                for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                {
                    iV0 = iVar0 + i1;
                    iV1 = iV0 + 1;
                    iV2 = iVar1 + i1 + 1;
                    iV3 = iV2 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkY; i0++)
            {
                fCurrentY = i0*fFractionY - fHalfYExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        -fHalfExtentX,
                        fCurrentY,
                        i1*fFractionZ-fHalfZExtentCorr);
                }
            }
            int iCenterRectSize = iVQuantity;

            // side things
            for (int i = 0; i < iSegmentsWkY; i++)
            {
                iV0 = i*(iSegmentsWkZ + 1);
                iV1 = iVQuantity + i;
                iV2 = iV1 + 1;
                iV3 = (i + 1)*(iSegmentsWkZ + 1);
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing1Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkY; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    i*fFractionY-fHalfYExtentCorr,
                    -fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkY; i++)
            {
                iV0 = (i + 1)*(iSegmentsWkZ + 1) + iSegmentsWkZ;
                iV1 = iVQuantity + i + 1;
                iV2 = iV1 - 1;
                iV3 = i*(iSegmentsWkZ + 1) + iSegmentsWkZ;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing2Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkY; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    i*fFractionY-fHalfYExtentCorr,
                    fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkZ; i++)
            {
                iV0 = iVQuantity + i + 1;
                iV1 = iV0 - 1;
                iV2 = i;
                iV3 = i + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing3Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkZ; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    -fHalfExtentY,
                    i*fFractionZ-fHalfZExtentCorr);
            }
            for (int i = 0; i < iSegmentsWkZ; i++)
            {
                iV0 = iCenterRectSize - iSegmentsWkZ + i;
                iV1 = iV0 - 1;
                iV2 = iVQuantity + i;
                iV3 = iV2 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing4Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkZ; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    fHalfExtentY,
                    i*fFractionZ-fHalfZExtentCorr);
            }

            // corner 1
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = 0;
                    iV1 = iVQuantity + i;
                    iV2 = iV1 + 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iThing3Begin;
                iV1 = iVQuantity;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iVQuantity + iRadiusSegments - 2;
                iV1 = iThing1Begin;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iThing3Begin;
                iV1 = iThing1Begin;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    -fHalfYExtentCorr-fRadius*afRoundCos[i],
                    -fHalfZExtentCorr-fRadius*afRoundSin[i]);
            }

            // corner 2
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iCenterRectSize - 1 - iSegmentsWkZ;
                    iV1 = iVQuantity + i;
                    iV2 = iV1 + 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iCenterRectSize - 1 - iSegmentsWkZ;
                iV1 = iThing2Begin - 1;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iCenterRectSize - 1 - iSegmentsWkZ;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iThing4Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iCenterRectSize - 1 - iSegmentsWkZ;
                iV1 = iThing2Begin - 1;
                iV2 = iThing4Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    fHalfYExtentCorr+fRadius*afRoundSin[i],
                    -fHalfZExtentCorr-fRadius*afRoundCos[i]);
            }

            // corner 3
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iCenterRectSize - 1;
                    iV1 = iVQuantity + i;
                    iV2 = iV1 + 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iCenterRectSize - 1;
                iV1 = iThing4Begin + iSegmentsWkZ;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iThing2Begin + iSegmentsWkY;
                iV1 = iCenterRectSize - 1;
                iV2 = iVQuantity + iRadiusSegments - 2;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iCenterRectSize - 1;
                iV1 = iThing4Begin + iSegmentsWkZ;
                iV2 = iThing2Begin + iSegmentsWkY;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    fHalfYExtentCorr+fRadius*afRoundCos[i],
                    fHalfZExtentCorr+fRadius*afRoundSin[i]);
            }

            // corner 4
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iSegmentsWkZ;
                    iV1 = iVQuantity + i;
                    iV2 = iV1 + 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iSegmentsWkZ;
                iV1 = iThing2Begin;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iSegmentsWkZ;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iThing4Begin - 1;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iSegmentsWkZ;
                iV1 = iThing2Begin;
                iV2 = iThing4Begin - 1;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    -fHalfYExtentCorr-fRadius*afRoundSin[i],
                    fHalfZExtentCorr+fRadius*afRoundCos[i]);
            }
        }
        else if ( fExtentX != (Real)0.0
             &&   fExtentY == (Real)0.0
             &&   fExtentZ != (Real)0.0 )
        {
            // center thing
            for (int i0 = 0; i0 < iSegmentsWkX; i0++)
            {
                iVar0 = (i0 + 1)*(iSegmentsWkZ + 1);
                iVar1 = i0*(iSegmentsWkZ + 1);
                for (int i1 = 0; i1 < iSegmentsWkZ; i1++)
                {
                    iV0 = iVar0 + i1;
                    iV1 = iVar1 + i1;
                    iV2 = iV1 + 1;
                    iV3 = iV0 + 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
            {
                fCurrentX = i0*fFractionX - fHalfXExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkZ; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        fCurrentX,
                        -fHalfExtentY,
                        i1*fFractionZ-fHalfZExtentCorr);
                }
            }
            int iCenterRectSize = iVQuantity;

            // side things
            for (int i = 0; i < iSegmentsWkX; i++)
            {
                iV0 = i*(iSegmentsWkZ + 1);
                iV1 = (i + 1)*(iSegmentsWkZ + 1);
                iV2 = iVQuantity + i + 1;
                iV3 = iV2 - 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing1Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkX; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    i*fFractionX-fHalfXExtentCorr,
                    -fHalfExtentY,
                    -fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkX; i++)
            {
                iV0 = (i + 1)*(iSegmentsWkZ + 1) + iSegmentsWkZ;
                iV1 = i*(iSegmentsWkZ + 1) + iSegmentsWkZ;
                iV2 = iVQuantity + i;
                iV3 = iV2 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing2Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkX; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    i*fFractionX-fHalfXExtentCorr,
                    -fHalfExtentY,
                    fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkZ; i++)
            {
                iV0 = iVQuantity + i + 1;
                iV1 = i + 1;
                iV2 = i;
                iV3 = iV0 - 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing3Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkZ; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    -fHalfExtentY,
                    i*fFractionZ-fHalfZExtentCorr);
            }
            for (int i = 0; i < iSegmentsWkZ; i++)
            {
                iV0 = iCenterRectSize - iSegmentsWkZ + i;
                iV1 = iVQuantity + i + 1;
                iV2 = iV1 - 1;
                iV3 = iV0 - 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing4Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkZ; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfExtentX,
                    -fHalfExtentY,
                    i*fFractionZ-fHalfZExtentCorr);
            }

            // corner 1
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = 0;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = 0;
                iV1 = iVQuantity;
                iV2 = iThing3Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = 0;
                iV1 = iThing1Begin;
                iV2 = iVQuantity + iRadiusSegments - 2;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = 0;
                iV1 = iThing1Begin;
                iV2 = iThing3Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfXExtentCorr-fRadius*afRoundCos[i],
                    -fHalfExtentY,
                    -fHalfZExtentCorr-fRadius*afRoundSin[i]);
            }

            // corner 2
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iCenterRectSize - 1 - iSegmentsWkZ;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iVQuantity;
                iV1 = iThing2Begin - 1;
                iV2 = iCenterRectSize - 1 - iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iThing4Begin;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iCenterRectSize - 1 - iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iThing4Begin;
                iV1 = iThing2Begin - 1;
                iV2 = iCenterRectSize - 1 - iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfXExtentCorr+fRadius*afRoundSin[i],
                    -fHalfExtentY,
                    -fHalfZExtentCorr-fRadius*afRoundCos[i]);
            }

            // corner 3
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iCenterRectSize - 1;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iVQuantity;
                iV1 = iThing4Begin + iSegmentsWkZ;
                iV2 = iCenterRectSize - 1;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iVQuantity + iRadiusSegments - 2;
                iV1 = iCenterRectSize - 1;
                iV2 = iThing2Begin + iSegmentsWkX;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iThing4Begin + iSegmentsWkZ;
                iV1 = iCenterRectSize - 1;
                iV2 = iThing2Begin + iSegmentsWkX;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfXExtentCorr+fRadius*afRoundCos[i],
                    -fHalfExtentY,
                    fHalfZExtentCorr+fRadius*afRoundSin[i]);
            }

            // corner 4
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iSegmentsWkZ;
                    iV1 = iVQuantity + i + 1;
                    iV2 = iV1 - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iVQuantity;
                iV1 = iThing2Begin;
                iV2 = iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iThing4Begin - 1;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iThing4Begin - 1;
                iV1 = iThing2Begin;
                iV2 = iSegmentsWkZ;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfXExtentCorr-fRadius*afRoundSin[i],
                    -fHalfExtentY,
                    fHalfZExtentCorr+fRadius*afRoundCos[i]);
            }
        }
        else if ( fExtentX != (Real)0.0
             &&   fExtentY != (Real)0.0
             &&   fExtentZ == (Real)0.0 )
        {
            // center thing
            for (int i0 = 0; i0 < iSegmentsWkX; i0++)
            {
                iVar0 = (i0 + 1)*(iSegmentsWkY + 1);
                iVar1 = i0*(iSegmentsWkY + 1);
                for (int i1 = 0; i1 < iSegmentsWkY; i1++)
                {
                    iV0 = iVar0 + i1 + 1;
                    iV1 = iVar1 + i1 + 1;
                    iV2 = iV1 - 1;
                    iV3 = iV0 - 1;
                    AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
                }
            }
            for (int i0 = 0; i0 <= iSegmentsWkX; i0++)
            {
                fCurrentX = i0*fFractionX - fHalfXExtentCorr;
                for (int i1 = 0; i1 <= iSegmentsWkY; i1++)
                {
                    aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                        fCurrentX,
                        i1*fFractionY-fHalfYExtentCorr,
                        -fHalfExtentZ);
                }
            }
            int iCenterRectSize = iVQuantity;

            // side things
            for (int i = 0; i < iSegmentsWkX; i++)
            {
                iV0 = iVQuantity + i;
                iV1 = iV0 + 1;
                iV2 = (i + 1)*(iSegmentsWkY + 1);
                iV3 = i*(iSegmentsWkY + 1);
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing1Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkX; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    i*fFractionX-fHalfXExtentCorr,
                    -fHalfExtentY,
                    -fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkX; i++)
            {
                iV0 = iVQuantity + i + 1;
                iV1 = iV0 - 1;
                iV2 = i*(iSegmentsWkY + 1) + iSegmentsWkY;
                iV3 = (i + 1)*(iSegmentsWkY + 1) + iSegmentsWkY;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing2Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkX; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    i*fFractionX-fHalfXExtentCorr,
                    fHalfExtentY,
                    fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkY; i++)
            {
                iV0 = iVQuantity + i;
                iV1 = i;
                iV2 = i + 1;
                iV3 = iV0 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing3Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkY; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfExtentX,
                    i*fFractionY-fHalfYExtentCorr,
                    -fHalfExtentZ);
            }
            for (int i = 0; i < iSegmentsWkY; i++)
            {
                iV0 = iCenterRectSize - iSegmentsWkY + i - 1;
                iV1 = iVQuantity + i;
                iV2 = iV1 + 1;
                iV3 = iV0 + 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            int iThing4Begin = iVQuantity;
            for (int i = 0; i <= iSegmentsWkY; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfExtentX,
                    i*fFractionY-fHalfYExtentCorr,
                    -fHalfExtentZ);
            }

            // corner 1
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iVQuantity + i;
                    iV1 = iV0 + 1;
                    iV2 = 0;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iThing3Begin;
                iV1 = iVQuantity;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iVQuantity + iRadiusSegments - 2;
                iV1 = iThing1Begin;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iThing3Begin;
                iV1 = iThing1Begin;
                iV2 = 0;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfXExtentCorr-fRadius*afRoundCos[i],
                    -fHalfYExtentCorr-fRadius*afRoundSin[i],
                    -fHalfExtentZ);
            }

            // corner 2
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iVQuantity + i;
                    iV1 = iV0 + 1;
                    iV2 = iCenterRectSize - 1 - iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iCenterRectSize - 1 - iSegmentsWkY;
                iV1 = iThing2Begin - 1;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iCenterRectSize - 1 - iSegmentsWkY;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iThing4Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iCenterRectSize - 1 - iSegmentsWkY;
                iV1 = iThing2Begin - 1;
                iV2 = iThing4Begin;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfXExtentCorr+fRadius*afRoundSin[i],
                    -fHalfYExtentCorr-fRadius*afRoundCos[i],
                    -fHalfExtentZ);
            }

            // corner 3
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iVQuantity + i;
                    iV1 = iV0 + 1;
                    iV2 = iCenterRectSize - 1;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iCenterRectSize - 1;
                iV1 = iThing4Begin + iSegmentsWkY;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iThing2Begin + iSegmentsWkX;
                iV1 = iCenterRectSize - 1;
                iV2 = iVQuantity + iRadiusSegments - 2;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iCenterRectSize - 1;
                iV1 = iThing4Begin + iSegmentsWkY;
                iV2 = iThing2Begin + iSegmentsWkX;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    fHalfXExtentCorr+fRadius*afRoundCos[i],
                    fHalfYExtentCorr+fRadius*afRoundSin[i],
                    -fHalfExtentZ);
            }

            // corner 4
            if ( iRadiusSegments > 1 )
            {
                for (int i = 0; i < iRadiusSegments-2; i++)
                {
                    iV0 = iVQuantity + i;
                    iV1 = iV0 + 1;
                    iV2 = iSegmentsWkY;
                    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                }
                iV0 = iSegmentsWkY;
                iV1 = iThing2Begin;
                iV2 = iVQuantity;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
                iV0 = iSegmentsWkY;
                iV1 = iVQuantity + iRadiusSegments - 2;
                iV2 = iThing4Begin - 1;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            else
            {
                iV0 = iSegmentsWkY;
                iV1 = iThing2Begin;
                iV2 = iThing4Begin - 1;
                AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
            }
            for (int i = 1; i < iRadiusSegments; i++)
            {
                aqVertex[iVQuantity++] = rqCenter + Vector3<Real>(
                    -fHalfXExtentCorr-fRadius*afRoundSin[i],
                    fHalfYExtentCorr+fRadius*afRoundCos[i],
                    -fHalfExtentZ);
            }
        }
    }

    if ( fRadius != (Real)0.0 )
    {
        delete[] afRoundCos;
        delete[] afRoundSin;
    }

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructBall (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fExtentX, Real fExtentY,
    Real fExtentZ, int iSides, int iSegments)
{
    assert( fExtentX >= (Real)0.0
        &&  fExtentY >= (Real)0.0
        &&  fExtentZ >= (Real)0.0
        &&  iSides >= 3 && iSegments >= 2 );

    Real fTrigSideFraction = Math<Real>::TWO_PI/iSides;
    Real fTrigSegmentFraction = Math<Real>::PI/iSegments;
    Real fHalfExtentX = fExtentX/2;
    Real fHalfExtentY = fExtentY/2;
    Real fHalfExtentZ = fExtentZ/2;

    int* aiIndex;
    Vector3<Real>* aqVertex;
    Real fTrigCurrentSegment, fTrigCurrentSide;
    int i, i0, i1, iV0, iV1, iV2, iV3, iVar0, iVertexQuantity, iIndexQuantity;
    int iVQuantity = 0;
    int iIQuantity = 0;
    if ( fExtentX == (Real)0.0
    ||   fExtentY == (Real)0.0
    ||   fExtentZ == (Real)0.0 )
    {
        iVertexQuantity = iSides;
        aqVertex = new Vector3<Real>[iVertexQuantity];

        iIndexQuantity = 3*(iSides - 2);
        aiIndex = new int[iIndexQuantity];

        if ( fExtentX == (Real)0.0
        &&   fExtentY != (Real)0.0
        &&   fExtentZ != (Real)0.0 )
        {
            int* aiRimVIndex = new int[iSides];
            for (i = 0; i < iSides; i++)
            {
                fTrigCurrentSide = i*fTrigSideFraction;
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (Real)0.0,
                    fHalfExtentY*Math<Real>::Sin(fTrigCurrentSide),
                    fHalfExtentZ*Math<Real>::Cos(fTrigCurrentSide));
                iVQuantity++;
                aiRimVIndex[i] = i;
            }
            AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndex,false);
            delete[] aiRimVIndex;
        }
        else if ( fExtentX != (Real)0.0
             &&   fExtentY == (Real)0.0
             &&   fExtentZ != (Real)0.0 )
        {
            int* aiRimVIndex = new int[iSides];
            for (i = 0; i < iSides; i++)
            {
                fTrigCurrentSide = i*fTrigSideFraction;
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fHalfExtentX*Math<Real>::Cos(fTrigCurrentSide),
                    (Real)0.0,
                    fHalfExtentZ*Math<Real>::Sin(fTrigCurrentSide));
                iVQuantity++;
                aiRimVIndex[i] = i;
            }
            AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndex,false);
            delete[] aiRimVIndex;
        }
        else if ( fExtentX != (Real)0.0
             &&   fExtentY != (Real)0.0
             &&   fExtentZ == (Real)0.0 )
        {
            int* aiRimVIndex = new int[iSides];
            for (i = 0; i < iSides; i++)
            {
                fTrigCurrentSide = i*fTrigSideFraction;
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fHalfExtentX*Math<Real>::Cos(fTrigCurrentSide),
                    fHalfExtentY*Math<Real>::Sin(fTrigCurrentSide),
                    (Real)0.0);
                iVQuantity++;
                aiRimVIndex[i] = i;
            }
            AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndex,true);
            delete[] aiRimVIndex;
        }
    }
    else
    {
        iVertexQuantity = (iSegments - 1)*iSides + 2;
        aqVertex = new Vector3<Real>[iVertexQuantity];

        iIndexQuantity = ((iSegments - 2)*iSides + iSides)*6;
        aiIndex = new int[iIndexQuantity];

        if ( eAxis == AT_AXIS_X )
            aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_X*fHalfExtentX;
        else if ( eAxis == AT_AXIS_Y )
            aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_Y*fHalfExtentY;
        else  // eAxis = AT_AXIS_Z
            aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_Z*fHalfExtentZ;

        for (i = 0; i < iSides-1; i++)
        {
            iV0 = i + 2;
            iV1 = 0;
            iV2 = i + 1;
            AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
        }
        iV0 = 1;
        iV1 = 0;
        iV2 = iSides;
        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);

        if ( eAxis == AT_AXIS_X )
        {
            for (i0 = 1; i0 < iSegments; i0++)
            {
                fTrigCurrentSegment = i0*fTrigSegmentFraction;
                for (i1 = 0; i1 < iSides; i1++)
                {
                    fTrigCurrentSide = i1*fTrigSideFraction;
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfExtentX*Math<Real>::Cos(fTrigCurrentSegment),
                        fHalfExtentY*Math<Real>::Cos(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment),
                        fHalfExtentZ*Math<Real>::Sin(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment));
                    iVQuantity++;
                }
            }
        }
        else if ( eAxis == AT_AXIS_Y )
        {
            for (i0 = 1; i0 < iSegments; i0++)
            {
                fTrigCurrentSegment = i0*fTrigSegmentFraction;
                for (i1 = 0; i1 < iSides; i1++)
                {
                    fTrigCurrentSide = i1*fTrigSideFraction;
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfExtentX*Math<Real>::Sin(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment),
                        fHalfExtentY*Math<Real>::Cos(fTrigCurrentSegment),
                        fHalfExtentZ*Math<Real>::Cos(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment));
                    iVQuantity++;
                }
            }
        }
        else  // eAxis = AT_AXIS_Z
        {
            for (i0 = 1; i0 < iSegments; i0++)
            {
                fTrigCurrentSegment = i0*fTrigSegmentFraction;
                for (i1 = 0; i1 < iSides; i1++)
                {
                    fTrigCurrentSide = i1*fTrigSideFraction;
                    aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                        fHalfExtentX*Math<Real>::Cos(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment),
                        fHalfExtentY*Math<Real>::Sin(fTrigCurrentSide)*Math<Real>::Sin(fTrigCurrentSegment),
                        fHalfExtentZ*Math<Real>::Cos(fTrigCurrentSegment));
                    iVQuantity++;
                }
            }
        }
        for (i0 = 1; i0 < iSegments-1; i0++)
        {
            iVar0 = i0*iSides + 1;
            for (i1 = 0; i1 < iSides-1; i1++)
            {
                iV0 = iVar0 + i1;
                iV1 = iV0 + 1;
                iV2 = iV1 - iSides;
                iV3 = iV2 - 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            iV0 = iVar0 + i1;
            iV1 = iV0 - i1;
            iV2 = iV1 - iSides;
            iV3 = iV2 + i1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }

        if ( eAxis == AT_AXIS_X )
            aqVertex[iVQuantity++] = -Vector3<Real>::UNIT_X*fHalfExtentX;
        else if ( eAxis == AT_AXIS_Y )
            aqVertex[iVQuantity++] = -Vector3<Real>::UNIT_Y*fHalfExtentY;
        else  // eAxis = AT_AXIS_Z
            aqVertex[iVQuantity++] = -Vector3<Real>::UNIT_Z*fHalfExtentZ;

        for (i = 0; i < iSides-1; i++)
        {
            iV0 = iVQuantity - iSides + i;
            iV1 = iV0 - 1;
            iV2 = iVQuantity - 1;
            AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
        }
        iV0 = iVQuantity - 1 - iSides;
        iV1 = iV0 + i;
        iV2 = iVQuantity - 1;
        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
    }

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructCylinder (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fExtentX, Real fExtentY,
    Real fExtentZ, int iSides, int iSegments)
{
    assert( fExtentX >= (Real)0.0
        &&  fExtentY >= (Real)0.0
        &&  fExtentZ >= (Real)0.0
        &&  iSides >= 3 && iSegments >= 1 );

    int iVertexQuantity = iSides*2 + (iSegments + 1)*iSides;
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = (iSides - 2 + iSegments*iSides)*6;
    int* aiIndex = new int[iIndexQuantity];

    Real fTrigSideFraction = Math<Real>::TWO_PI/iSides;
    Real fSegmentFraction;
    if ( eAxis == AT_AXIS_X )
        fSegmentFraction = fExtentX/iSegments;
    else if ( eAxis == AT_AXIS_Y )
        fSegmentFraction = fExtentY/iSegments;
    else  // eAxis = AT_AXIS_Z
        fSegmentFraction = fExtentZ/iSegments;

    Real fHalfExtentX = fExtentX/2;
    Real fHalfExtentY = fExtentY/2;
    Real fHalfExtentZ = fExtentZ/2;

    int* aiRimVIndex = new int[iSides];
    Real* afRoundSin = new Real[iSides];
    Real* afRoundCos = new Real[iSides];
    Real fVar0;
    int iVQuantity = 0;
    int iIQuantity = 0;
    int i, i0, i1, iV0, iV1, iV2, iV3, iVar0;
    for (i = 0; i < iSides; i++)
    {
        fVar0 = i*fTrigSideFraction;
        afRoundSin[i] = Math<Real>::Sin(fVar0);
        afRoundCos[i] = Math<Real>::Cos(fVar0);
    }
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                -fHalfExtentX,
                fHalfExtentY*afRoundCos[i],
                fHalfExtentZ*afRoundSin[i]);
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*afRoundSin[i],
                -fHalfExtentY,
                fHalfExtentZ*afRoundCos[i]);
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*afRoundCos[i],
                fHalfExtentY*afRoundSin[i],
                -fHalfExtentZ);
            iVQuantity++;
        }
    }
    for (i = 0; i < iSides; i++)
        aiRimVIndex[i] = i;

    AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndex,false);
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfExtentX+i0*fSegmentFraction,
                    fHalfExtentY*afRoundCos[i1],
                    fHalfExtentZ*afRoundSin[i1]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fHalfExtentX*afRoundSin[i1],
                    -fHalfExtentY+i0*fSegmentFraction,
                    fHalfExtentZ*afRoundCos[i1]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fHalfExtentX*afRoundCos[i1],
                    fHalfExtentY*afRoundSin[i1],
                    -fHalfExtentZ+i0*fSegmentFraction);
                iVQuantity++;
            }
        }
    }
    for (i0 = 0; i0 < iSegments; i0++)
    {
        iVar0 = (i0 + 1)*iSides;
        for (i1 = 0; i1 < iSides-1; i1++)
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 + 1;
            iV2 = iV1 + iSides;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iVar0 + i1;
        iV1 = iV0 - i1;
        iV2 = iVar0 + iSides;
        iV3 = iV2 + i1;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX,
                fHalfExtentY*afRoundCos[i],
                fHalfExtentZ*afRoundSin[i]);
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*afRoundSin[i],
                fHalfExtentY,
                fHalfExtentZ*afRoundCos[i]);
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*afRoundCos[i],
                fHalfExtentY*afRoundSin[i],
                fHalfExtentZ);
            iVQuantity++;
        }
    }
    for (i = 0; i < iSides; i++)
        aiRimVIndex[i] = iVQuantity - iSides + i;

    AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndex,true);

    delete[] aiRimVIndex;
    delete[] afRoundSin;
    delete[] afRoundCos;

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructCone (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fExtentX, Real fExtentY,
    Real fExtentZ, int iSides, int iSegments)
{
    assert( fExtentX >= (Real)0.0
        &&  fExtentY >= (Real)0.0
        &&  fExtentZ >= (Real)0.0
        &&  iSides >= 3 && iSegments >= 1 );

    int iVertexQuantity = iSides + (iSegments + 1)*iSides;
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = 3*(iSides - 2 + (iSegments - 1)*iSides*2 + iSides);
    int* aiIndex = new int[iIndexQuantity];

    Real fAxisExtent;
    if ( eAxis == AT_AXIS_X )
        fAxisExtent = fExtentX;
    else if ( eAxis == AT_AXIS_Y )
        fAxisExtent = fExtentY;
    else  // eAxis = AT_AXIS_Z
        fAxisExtent = fExtentZ;

    Real fTrigSideFraction = Math<Real>::TWO_PI/iSides;
    Real fSegmentFraction = fAxisExtent/iSegments;
    Real fHalfExtentX = fExtentX/2;
    Real fHalfExtentY = fExtentY/2;
    Real fHalfExtentZ = fExtentZ/2;

    int* aiRimVIndices = new int[iSides];

    int i, i0, i1, iV0, iV1, iV2, iV3, iVar0;
    int iVQuantity = 0;
    int iIQuantity = 0;
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                -fHalfExtentX,
                fHalfExtentY*Math<Real>::Cos(i*fTrigSideFraction),
                fHalfExtentZ*Math<Real>::Sin(i*fTrigSideFraction));
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*Math<Real>::Sin(i*fTrigSideFraction),
                -fHalfExtentY,
                fHalfExtentZ*Math<Real>::Cos(i*fTrigSideFraction));
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iSides; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfExtentX*Math<Real>::Cos(i*fTrigSideFraction),
                fHalfExtentY*Math<Real>::Sin(i*fTrigSideFraction),
                -fHalfExtentZ);
            iVQuantity++;
        }
    }
    for (i = 0; i < iSides; i++)
        aiRimVIndices[i] = i;

    AddConvexPolygon(iIQuantity,aiIndex,iSides,aiRimVIndices,false);
    delete[] aiRimVIndices;
    Real fVar0;
    Real* afRoundSin = new Real[iSides];
    Real* afRoundCos = new Real[iSides];
    for (i = 0; i < iSides; i++)
    {
        fVar0 = i*fTrigSideFraction;
        afRoundSin[i] = Math<Real>::Sin(fVar0);
        afRoundCos[i] = Math<Real>::Cos(fVar0);
    }
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            fVar0 = (Real)1.0 - (Real)i0/iSegments;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfExtentX+i0*fSegmentFraction,
                    fVar0*fHalfExtentY*afRoundCos[i1],
                    fVar0*fHalfExtentZ*afRoundSin[i1]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            fVar0 = (Real)1.0 - (Real)i0/iSegments;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fVar0*fHalfExtentX*afRoundSin[i1],
                    -fHalfExtentY+i0*fSegmentFraction,
                    fVar0*fHalfExtentZ*afRoundCos[i1]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            fVar0 = (Real)1.0 - (Real)i0/iSegments;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fVar0*fHalfExtentX*afRoundCos[i1],
                    fVar0*fHalfExtentY*afRoundSin[i1],
                    -fHalfExtentZ+i0*fSegmentFraction);
                iVQuantity++;
            }
        }
    }
    delete[] afRoundSin;
    delete[] afRoundCos;
    for (i0 = 0; i0 < iSegments-1; i0++)
    {
        iVar0 = (i0 + 1)*iSides;
        for (i1 = 0; i1 < iSides-1; i1++)
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 + 1;
            iV2 = iV1 + iSides;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iVar0 + i1;
        iV1 = iV0 - i1;
        iV2 = iVar0 + iSides;
        iV3 = iV2 + i1;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    for (i = 0; i < iSides-1; i++)
    {
        iV0 = (i0 + 1)*iSides + i;
        iV1 = iV0 + 1;
        iV2 = iV1 + iSides;
        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
    }
    iV0 = (i0 + 1)*iSides + i;
    iV1 = iV0 - i;
    iV2 = (i0 + 2)*iSides;
    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructTorus (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fRadius, Real fSectionExtent0,
    Real fSectionExtent1, int iSections, int iSides, Real fStartAngle, Real fEndAngle)
{
    assert( fRadius >= (Real)0.0 && (Real)0.0 <= fSectionExtent0 && fSectionExtent0 <= fRadius
        &&  fSectionExtent1 >= (Real)0.0 && iSections >= 3 && iSides >= 3 );

    Real fSectionExtentX, fSectionExtentY, fSectionExtentZ;
    if ( eAxis == AT_AXIS_X )
    {
        fSectionExtentX = fSectionExtent1;
        fSectionExtentY = fSectionExtent0;
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        fSectionExtentX = fSectionExtent0;
        fSectionExtentY = fSectionExtent1;
    }
    else  // eAxis = AT_AXIS_Y
    {
        fSectionExtentX = fSectionExtent0;
        fSectionExtentZ = fSectionExtent1;
    }

    bool bIntegral = ( fStartAngle == (Real)0.0 && fEndAngle == Math<Real>::TWO_PI );

    int iVertexQuantity = ( bIntegral ? iSides*iSections : iSides*(iSections + 1) + 2*iSides );
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = 6*( bIntegral ? iSides*iSections : (iSides*iSections + iSides - 2) );
    int* aiIndex = new int[iIndexQuantity];

    Real fTrigSectionFraction = ( bIntegral ? Math<Real>::TWO_PI/iSections :
        (fEndAngle - fStartAngle)/iSections );
    Real fTrigSideFraction = Math<Real>::TWO_PI/iSides;

    Real* afSectionCos = new Real[iSections+1];
    Real* afSectionSin = new Real[iSections+1];
    Real* afSideCos = new Real[iSides];
    Real* afSideSin = new Real[iSides];

    Real fVar0;
    int i, i0, i1, iVar0, iV0, iV1, iV2, iV3;
    int iVQuantity = 0;
    int iIQuantity = 0;

    for (i = 0; i <= iSections; i++)
    {
        fVar0 = fStartAngle + i*fTrigSectionFraction;
        afSectionCos[i] = Math<Real>::Cos(fVar0);
        afSectionSin[i] = Math<Real>::Sin(fVar0);
    }
    for (i = 0; i < iSides; i++)
    {
        fVar0 = i*fTrigSideFraction;
        afSideCos[i] = Math<Real>::Cos(fVar0);
        afSideSin[i] = Math<Real>::Sin(fVar0);
    }

    int iSectionDegree = ( bIntegral ? iSections : iSections + 1 );
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 0; i0 < iSectionDegree; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fSectionExtentX*afSideSin[i1],
                    (fRadius+fSectionExtentY*afSideCos[i1])*afSectionSin[i0],
                    (fRadius+fSectionExtentY*afSideCos[i1])*afSectionCos[i0]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 0; i0 < iSectionDegree; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i1])*afSectionCos[i0],
                    fSectionExtentY*afSideSin[i1],
                    (fRadius+fSectionExtentX*afSideCos[i1])*afSectionSin[i0]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 0; i0 < iSectionDegree; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i1])*afSectionSin[i0],
                    (fRadius+fSectionExtentX*afSideCos[i1])*afSectionCos[i0],
                    fSectionExtentZ*afSideSin[i1]);
                iVQuantity++;
            }
        }
    }

    for (i0 = 0; i0 < iSectionDegree-1; i0++)
    {
        iVar0 = i0*iSides;
        for (i1 = 0; i1 < iSides-1; i1++)
        {
            iV0 = iVar0 + i1 + iSides;
            iV1 = iVar0 + i1;
            iV2 = iV1 + 1;
            iV3 = iV2 + iSides;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iVar0 + iSides + i1;
        iV1 = iVar0 + i1;
        iV2 = iV1 - i1;
        iV3 = iV0 - i1;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    if ( bIntegral )
    {
        for (i = 0; i < iSides-1; i++)
        {
            iV0 = i;
            iV1 = (iSections - 1)*iSides + i;
            iV2 = iV1 + 1;
            iV3 = i + 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = i;
        iV1 = (iSections - 1)*iSides + i;
        iV2 = iV1 - i;
        iV3 = 0;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    else
    {
        if ( eAxis == AT_AXIS_X )
        {
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fSectionExtentX*afSideSin[i],
                    (fRadius+fSectionExtentY*afSideCos[i])*afSectionSin[0],
                    (fRadius+fSectionExtentY*afSideCos[i])*afSectionCos[0]);
                iVQuantity++;
            }
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fSectionExtentX*afSideSin[i],
                    (fRadius+fSectionExtentY*afSideCos[i])*afSectionSin[iSections],
                    (fRadius+fSectionExtentY*afSideCos[i])*afSectionCos[iSections]);
                iVQuantity++;
            }
        }
        else if ( eAxis == AT_AXIS_Y )
        {
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionCos[0],
                    fSectionExtentY*afSideSin[i],
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionSin[0]);
                iVQuantity++;
            }
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionCos[iSections],
                    fSectionExtentY*afSideSin[i],
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionSin[iSections]);
                iVQuantity++;
            }
        }
        else  // eAxis = AT_AXIS_Z
        {
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionSin[0],
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionCos[0],
                    fSectionExtentZ*afSideSin[i]);
                iVQuantity++;
            }
            for (i = 0; i < iSides; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionSin[iSections],
                    (fRadius+fSectionExtentX*afSideCos[i])*afSectionCos[iSections],
                    fSectionExtentZ*afSideSin[i]);
                iVQuantity++;
            }
        }

        int* aiSliceVSection = new int[iSides];
        for (i = 0; i < iSides; i++)
        {
            iVar0 = iVQuantity - 2*iSides + i;
            aiSliceVSection[i] = iVar0;
        }
        AddConvexPolygon(iIQuantity,aiIndex,iSides,aiSliceVSection,false);
        for (i = 0; i < iSides; i++)
        {
            iVar0 = iVQuantity - iSides + i;
            aiSliceVSection[i] = iVar0;
        }
        AddConvexPolygon(iIQuantity,aiIndex,iSides,aiSliceVSection,true);
        delete[] aiSliceVSection;
    }

    delete[] afSectionCos;
    delete[] afSectionSin;
    delete[] afSideCos;
    delete[] afSideSin;

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructTube (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fLength, Real fInnerRadius,
    Real fOuterRadius, int iSides, int iSegments, Real fStartAngle, Real fEndAngle)
{
    assert( fLength >= (Real)0.0 && (Real)0.0 <= fInnerRadius && fInnerRadius <= fOuterRadius
        &&  fOuterRadius >= (Real)0.0 && iSides >= 3 && iSegments >= 1 );

    bool bIntegral = ( fStartAngle == (Real)0.0 && fEndAngle == Math<Real>::TWO_PI );

    int iVertexQuantity = ( bIntegral ? iSides*4 + iSides*(iSegments + 1)*2 :
        (iSides + 1)*4 + (iSides + 1)*(iSegments + 1)*2 + (iSegments + 1)*4 );
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = 3*( bIntegral ? iSides*4 + iSides*iSegments*4 :
        (iSides)*4 + (iSides)*iSegments*4 + iSegments*4 );
    int* aiIndex = new int[iIndexQuantity];

    Real fTrigSideFraction = ( bIntegral ? Math<Real>::TWO_PI/iSides :
        (fEndAngle - fStartAngle)/iSides );
    Real fSegmentFraction = fLength/iSegments;
    Real fHalfLength = fLength/2;

    int iSidesDegree = ( bIntegral ? iSides : iSides + 1 );
    Real* afRoundSin = new Real[iSidesDegree];
    Real* afRoundCos = new Real[iSidesDegree];

    Real fVar0;
    int i, i0, i1, iV0, iV1, iV2, iV3, iVar0;
    int iVQuantity = 0;
    int iIQuantity = 0;
    if ( bIntegral )
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            fVar0 = i*fTrigSideFraction;
            afRoundSin[i] = Math<Real>::Sin(fVar0);
            afRoundCos[i] = Math<Real>::Cos(fVar0);
        }
    }
    else
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            fVar0 = fStartAngle + i*fTrigSideFraction;
            afRoundSin[i] = Math<Real>::Sin(fVar0);
            afRoundCos[i] = Math<Real>::Cos(fVar0);
        }
    }

    // lower butt-end vertices
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                -fHalfLength,
                fInnerRadius*afRoundCos[i],
                fInnerRadius*afRoundSin[i]);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                -fHalfLength,
                fOuterRadius*afRoundCos[i],
                fOuterRadius*afRoundSin[i]);
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fInnerRadius*afRoundSin[i],
                -fHalfLength,
                fInnerRadius*afRoundCos[i]);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fOuterRadius*afRoundSin[i],
                -fHalfLength,
                fOuterRadius*afRoundCos[i]);
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fInnerRadius*afRoundCos[i],
                fInnerRadius*afRoundSin[i],
                -fHalfLength);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fOuterRadius*afRoundCos[i],
                fOuterRadius*afRoundSin[i],
                -fHalfLength);
            iVQuantity++;
        }
    }

    // lower butt-end connectivity
    for (i = 0; i < iSidesDegree-1; i++)
    {
        iV0 = i + 1;
        iV1 = i + iSidesDegree + 1;
        iV2 = i + iSidesDegree;
        iV3 = i;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }

    if ( bIntegral )
    {
        iV0 = 0;
        iV1 = iSidesDegree;
        iV2 = i + iSidesDegree;
        iV3 = i;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }

    // length connectivity inner
    for (i0 = 0; i0 < iSegments; i0++)
    {
        iVar0 = iVQuantity + i0*iSidesDegree + iSidesDegree;
        for (i1 = 0; i1 < iSidesDegree-1; i1++)
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 + 1;
            iV2 = iV1 - iSidesDegree;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }

        if ( bIntegral )
        {
            iV0 = iVQuantity + (i0 + 1)*iSidesDegree + i1;
            iV1 = iV0 - i1;
            iV2 = iVQuantity + i0*iSidesDegree;
            iV3 = iV2 + i1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
    }

    // length vertices inner
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i0*fSegmentFraction,
                    fInnerRadius*afRoundCos[i1],
                    fInnerRadius*afRoundSin[i1]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundSin[i1],
                    -fHalfLength+i0*fSegmentFraction,
                    fInnerRadius*afRoundCos[i1]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundCos[i1],
                    fInnerRadius*afRoundSin[i1],
                    -fHalfLength+i0*fSegmentFraction);
                iVQuantity++;
            }
        }
    }

    // length connectivity outer
    for (i0 = 0; i0 < iSegments; i0++)
    {
        iVar0 = iVQuantity + i0*iSidesDegree;
        for (i1 = 0; i1 < iSidesDegree-1; i1++)
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 + 1;
            iV2 = iV1 + iSidesDegree;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }

        if ( bIntegral )
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 - i1;
            iV2 = iVQuantity + (i0 + 1)*iSidesDegree;
            iV3 = iV2 + i1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
    }

    // length vertices outer
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i0*fSegmentFraction,
                    fOuterRadius*afRoundCos[i1],
                    fOuterRadius*afRoundSin[i1]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundSin[i1],
                    -fHalfLength+i0*fSegmentFraction,
                    fOuterRadius*afRoundCos[i1]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 0; i0 <= iSegments; i0++)
        {
            for (i1 = 0; i1 < iSidesDegree; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundCos[i1],
                    fOuterRadius*afRoundSin[i1],
                    -fHalfLength+i0*fSegmentFraction);
                iVQuantity++;
            }
        }
    }

    // upper butt-end connectivity
    for (i = 0; i < iSidesDegree-1; i++)
    {
        iV0 = iVQuantity + i;
        iV1 = iV0 + iSidesDegree;
        iV2 = iV1 + 1;
        iV3 = iV0 + 1;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }

    if ( bIntegral )
    {
        iV0 = iVQuantity + i;
        iV1 = iV0 + iSidesDegree;
        iV2 = iVQuantity + iSidesDegree;
        iV3 = iVQuantity;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }

    // upper butt-end vertices
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfLength,
                fInnerRadius*afRoundCos[i],
                fInnerRadius*afRoundSin[i]);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fHalfLength,
                fOuterRadius*afRoundCos[i],
                fOuterRadius*afRoundSin[i]);
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fInnerRadius*afRoundSin[i],
                fHalfLength,
                fInnerRadius*afRoundCos[i]);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fOuterRadius*afRoundSin[i],
                fHalfLength,
                fOuterRadius*afRoundCos[i]);
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fInnerRadius*afRoundCos[i],
                fInnerRadius*afRoundSin[i],
                fHalfLength);
            iVQuantity++;
        }
        for (i = 0; i < iSidesDegree; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                fOuterRadius*afRoundCos[i],
                fOuterRadius*afRoundSin[i],
                fHalfLength);
            iVQuantity++;
        }
    }

    if ( !bIntegral )
    {
        int iSBegin1, iSBegin2, iSBegin3, iSBegin4;

        // slice at the start
        for (i = 0; i < iSegments; i++)
        {
            iV0 = iVQuantity + i;
            iV1 = iVQuantity + iSegments + 1 + i;
            iV2 = iV1 + 1;
            iV3 = iV0 + 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        if ( eAxis == AT_AXIS_X )
        {
            iSBegin1 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i*fSegmentFraction,
                    fInnerRadius*afRoundCos[0],
                    fInnerRadius*afRoundSin[0]);
                iVQuantity++;
            }
            iSBegin2 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i*fSegmentFraction,
                    fOuterRadius*afRoundCos[0],
                    fOuterRadius*afRoundSin[0]);
                iVQuantity++;
            }
        }
        else if ( eAxis == AT_AXIS_Y )
        {
            iSBegin1 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundSin[0],
                    -fHalfLength+i*fSegmentFraction,
                    fInnerRadius*afRoundCos[0]);
                iVQuantity++;
            }
            iSBegin2 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundSin[0],
                    -fHalfLength+i*fSegmentFraction,
                    fOuterRadius*afRoundCos[0]);
                iVQuantity++;
            }
        }
        else  // eAxis = AT_AXIS_Z
        {
            iSBegin1 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundCos[0],
                    fInnerRadius*afRoundSin[0],
                    -fHalfLength+i*fSegmentFraction);
                iVQuantity++;
            }
            iSBegin2 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundCos[0],
                    fOuterRadius*afRoundSin[0],
                    -fHalfLength+i*fSegmentFraction);
                iVQuantity++;
            }
        }

        // slice at the end
        for (i = 0; i < iSegments; i++)
        {
            iV0 = iVQuantity + i + 1;
            iV1 = iVQuantity + iSegments + 2 + i;
            iV2 = iV1 - 1;
            iV3 = iV0 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        if ( eAxis == AT_AXIS_X )
        {
            iSBegin3 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i*fSegmentFraction,
                    fInnerRadius*afRoundCos[iSidesDegree-1],
                    fInnerRadius*afRoundSin[iSidesDegree-1]);
                iVQuantity++;
            }
            iSBegin4 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+i*fSegmentFraction,
                    fOuterRadius*afRoundCos[iSidesDegree-1],
                    fOuterRadius*afRoundSin[iSidesDegree-1]);
                iVQuantity++;
            }
        }
        else if ( eAxis == AT_AXIS_Y )
        {
            iSBegin3 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundSin[iSidesDegree-1],
                    -fHalfLength+i*fSegmentFraction,
                    fInnerRadius*afRoundCos[iSidesDegree-1]);
                iVQuantity++;
            }
            iSBegin4 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundSin[iSidesDegree-1],
                    -fHalfLength+i*fSegmentFraction,
                    fOuterRadius*afRoundCos[iSidesDegree-1]);
                iVQuantity++;
            }
        }
        else  // eAxis = AT_AXIS_Z
        {
            iSBegin3 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fInnerRadius*afRoundCos[iSidesDegree-1],
                    fInnerRadius*afRoundSin[iSidesDegree-1],
                    -fHalfLength+i*fSegmentFraction);
                iVQuantity++;
            }
            iSBegin4 = iVQuantity;
            for (i = 0; i <= iSegments; i++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fOuterRadius*afRoundCos[iSidesDegree-1],
                    fOuterRadius*afRoundSin[iSidesDegree-1],
                    -fHalfLength+i*fSegmentFraction);
                iVQuantity++;
            }
        }
    }

    delete[] afRoundSin;
    delete[] afRoundCos;

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructCapsule (
    bool bConstructNormals, const Vector3<Real>& rqCenter, AxisType eAxis, Real fRadius, Real fLength, int iSides,
    int iSegments, int iLengthSegments)
{
    assert( (Real)0.0 <= fRadius && fRadius <= fLength/2 && iSides >= 3
        &&  iSegments >= 2 && iLengthSegments >= 1 );

    Real fHalfLength = fLength/2;
    Real fRadiusCorr = fRadius;
    if ( fRadiusCorr > fHalfLength )
        fRadiusCorr = fHalfLength;

    int iVertexQuantity = (1 + iSegments*iSides)*2 + (iLengthSegments - 1)*iSides;
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = (iSides + (iSegments - 1)*iSides*2 + iLengthSegments*iSides)*6;
    int* aiIndex = new int[iIndexQuantity];

    Real fTrigSideFraction = Math<Real>::TWO_PI/iSides;
    Real fTrigSegmentFraction = Math<Real>::HALF_PI/iSegments;
    Real fHSegmentFraction = (fLength - fRadiusCorr*2)/iLengthSegments;

    Real* afRoundSin = new Real[iSides];
    Real* afRoundCos = new Real[iSides];
    Real fVar0;
    int i, i0, i1, iV0, iV1, iV2, iV3, iVar0;
    int iVQuantity = 0;
    int iIQuantity = 0;
    for (i = 0; i < iSides; i++)
    {
        fVar0 = i*fTrigSideFraction;
        afRoundSin[i] = Math<Real>::Sin(fVar0);
        afRoundCos[i] = Math<Real>::Cos(fVar0);
    }
    if ( eAxis == AT_AXIS_X )
        aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_X*fHalfLength;
    else if ( eAxis == AT_AXIS_Y )
        aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_Y*fHalfLength;
    else  // eAxis = AT_AXIS_Z
        aqVertex[iVQuantity++] = rqCenter + Vector3<Real>::UNIT_Z*fHalfLength;

    for (i = 0; i < iSides-1; i++)
    {
        iV0 = i + 2;
        iV1 = 0;
        iV2 = i + 1;
        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
    }
    iV0 = 1;
    iV1 = 0;
    iV2 = iSides;
    AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
    Real fTrigCurrentSegment;
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 1; i0 <= iSegments; i0++)
        {
            fTrigCurrentSegment = i0*fTrigSegmentFraction;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fHalfLength-fRadiusCorr+fRadiusCorr*Math<Real>::Cos(fTrigCurrentSegment),
                    fRadiusCorr*afRoundCos[i1]*Math<Real>::Sin(fTrigCurrentSegment),
                    fRadiusCorr*afRoundSin[i1]*Math<Real>::Sin(fTrigCurrentSegment));
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 1; i0 <= iSegments; i0++)
        {
            fTrigCurrentSegment = i0*fTrigSegmentFraction;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fRadiusCorr*afRoundSin[i1]*Math<Real>::Sin(fTrigCurrentSegment),
                    fHalfLength-fRadiusCorr+fRadiusCorr*Math<Real>::Cos(fTrigCurrentSegment),
                    fRadiusCorr*afRoundCos[i1]*Math<Real>::Sin(fTrigCurrentSegment));
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 1; i0 <= iSegments; i0++)
        {
            fTrigCurrentSegment = i0*fTrigSegmentFraction;
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fRadiusCorr*afRoundCos[i1]*Math<Real>::Sin(fTrigCurrentSegment),
                    fRadiusCorr*afRoundSin[i1]*Math<Real>::Sin(fTrigCurrentSegment),
                    fHalfLength-fRadiusCorr+fRadiusCorr*Math<Real>::Cos(fTrigCurrentSegment));
                iVQuantity++;
            }
        }
    }
    for (i0 = 1; i0 < iSegments; i0++)
    {
        iVar0 = i0*iSides + 1;
        for (i1 = 0; i1 < iSides-1; i1++)
        {
            iV0 = iVar0 + i1;
            iV1 = iV0 + 1;
            iV2 = iV1 - iSides;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iVar0 + i1;
        iV1 = iV0 - i1;
        iV2 = iV1 - iSides;
        iV3 = iV2 + i1;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    int iKnobVQuantity = iVQuantity;
    if ( eAxis == AT_AXIS_X )
    {
        for (i = 0; i < iKnobVQuantity; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                -aqVertex[i].X(),
                aqVertex[i].Y(),
                aqVertex[i].Z());
            iVQuantity++;
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i = 0; i < iKnobVQuantity; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                aqVertex[i].X(),
                -aqVertex[i].Y(),
                aqVertex[i].Z());
            iVQuantity++;
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i = 0; i < iKnobVQuantity; i++)
        {
            aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                aqVertex[i].X(),
                aqVertex[i].Y(),
                -aqVertex[i].Z());
            iVQuantity++;
        }
    }
    int iKnobIQuantity = iIQuantity;
    for (i = 0; i < iSides*3; i += 3)
    {
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+2];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+1];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i];
    }
    for (i = iSides*3; i < iKnobIQuantity; i += 6)
    {
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+3];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+5];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+1];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+5];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+2];
        aiIndex[iIQuantity++] = iKnobVQuantity + aiIndex[i+1];
    }
    int iMediumThingBegin = iVQuantity;
    if ( eAxis == AT_AXIS_X )
    {
        for (i0 = 1; i0 < iLengthSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    -fHalfLength+fRadiusCorr+i0*fHSegmentFraction,
                    fRadiusCorr*afRoundCos[i1],
                    fRadiusCorr*afRoundSin[i1]);
                iVQuantity++;
            }
        }
    }
    else if ( eAxis == AT_AXIS_Y )
    {
        for (i0 = 1; i0 < iLengthSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fRadiusCorr*afRoundSin[i1],
                    -fHalfLength+fRadiusCorr+i0*fHSegmentFraction,
                    fRadiusCorr*afRoundCos[i1]);
                iVQuantity++;
            }
        }
    }
    else  // eAxis = AT_AXIS_Z
    {
        for (i0 = 1; i0 < iLengthSegments; i0++)
        {
            for (i1 = 0; i1 < iSides; i1++)
            {
                aqVertex[iVQuantity] = rqCenter + Vector3<Real>(
                    fRadiusCorr*afRoundCos[i1],
                    fRadiusCorr*afRoundSin[i1],
                    -fHalfLength+fRadiusCorr+i0*fHSegmentFraction);
                iVQuantity++;
            }
        }
    }
    delete[] afRoundSin;
    delete[] afRoundCos;
    if ( iLengthSegments > 1 )
    {
        for (i = 0; i < iSides-1; i++)
        {
            iV0 = iMediumThingBegin - iSides + i;
            iV1 = iV0 + 1;
            iV2 = iMediumThingBegin + i + 1;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iMediumThingBegin - iSides + i;
        iV1 = iMediumThingBegin - iSides;
        iV2 = iMediumThingBegin;
        iV3 = iMediumThingBegin + i;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        for (i0 = 0; i0 < iLengthSegments-2; i0++)
        {
            iVar0 = iMediumThingBegin + i0*iSides;
            for (i1 = 0; i1 < iSides-1; i1++)
            {
                iV0 = iVar0 + i1;
                iV1 = iV0 + 1;
                iV2 = iV1 + iSides;
                iV3 = iV2 - 1;
                AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
            }
            iV0 = iVar0 + i1;
            iV1 = iV0 - i1;
            iV2 = iVar0 + iSides;
            iV3 = iV2 + i1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        for (i = 0; i < iSides-1; i++)
        {
            iV0 = iVQuantity - iSides + i;
            iV1 = iV0 + 1;
            iV2 = iKnobVQuantity - iSides + i + 1;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iVQuantity - iSides + i;
        iV1 = iVQuantity - iSides;
        iV2 = iKnobVQuantity - iSides;
        iV3 = iKnobVQuantity - iSides + i;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }
    else
    {
        for (i = 0; i < iSides-1; i++)
        {
            iV0 = iMediumThingBegin - iSides + i;
            iV1 = iV0 + 1;
            iV2 = iKnobVQuantity - iSides + i + 1;
            iV3 = iV2 - 1;
            AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
        }
        iV0 = iMediumThingBegin - iSides + i;
        iV1 = iMediumThingBegin - iSides;
        iV2 = iKnobVQuantity - iSides;
        iV3 = iKnobVQuantity - iSides + i;
        AddConvexQuadrangle(iIQuantity,aiIndex,iV0,iV1,iV2,iV3);
    }

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructGeosphere (
    bool bConstructNormals, const Vector3<Real>& rqCenter, Real fRadius, int iDegree)
{
    assert( fRadius >= (Real)0.0 && iDegree >= 1 );

    Array<Vector3<Real> > qVertexArray;
    Array<int> qIndexArray;

    static const Real s_fVar0 = Math<Real>::Sqrt(3)/2;
    static const Real s_fVar1 = Math<Real>::PI*2/5;

    Real fVar2;
    qVertexArray.Push(Vector3<Real>::UNIT_Y);
    for (int i = 0; i < 5; i++)
    {
        fVar2 = i*s_fVar1;
        qVertexArray.Push(Vector3<Real>(
            s_fVar0*Math<Real>::Cos(fVar2),
            (Real)0.5,
            s_fVar0*Math<Real>::Sin(fVar2)));
    }
    for (int i = 0; i < 5; i++)
    {
        fVar2 = i*s_fVar1 + s_fVar1/2;
        qVertexArray.Push(Vector3<Real>(
            s_fVar0*Math<Real>::Cos(fVar2),
            -(Real)0.5,
            s_fVar0*Math<Real>::Sin(fVar2)));
    }
    qVertexArray.Push(-Vector3<Real>::UNIT_Y);
    for (int i = 0; i < 4; i++)
    {
        qIndexArray.Push(0);
        qIndexArray.Push(i+2);
        qIndexArray.Push(i+1);
    }
    qIndexArray.Push(0);
    qIndexArray.Push(1);
    qIndexArray.Push(5);
    for (int i = 0; i < 4; i++)
    {
        qIndexArray.Push(i+1);
        qIndexArray.Push(i+2);
        qIndexArray.Push(i+6);
    }
    qIndexArray.Push(5);
    qIndexArray.Push(1);
    qIndexArray.Push(10);
    for (int i = 0; i < 4; i++)
    {
        qIndexArray.Push(i+2);
        qIndexArray.Push(i+7);
        qIndexArray.Push(i+6);
    }
    qIndexArray.Push(1);
    qIndexArray.Push(6);
    qIndexArray.Push(10);
    for (int i = 0; i < 4; i++)
    {
        qIndexArray.Push(i+6);
        qIndexArray.Push(i+7);
        qIndexArray.Push(11);
    }
    qIndexArray.Push(11);
    qIndexArray.Push(10);
    qIndexArray.Push(6);

    int iV0, iV1, iV2, iMV0, iMV1, iMV2, iIQuantity;
    for (int i0 = 0; i0 < iDegree-1; i0++)
    {
        Set<Link<int,int> > qAlreadyDividedEdges;
        Map<Link<int,int>,int> qEdgeToMiddlePoint;
        Array<int> qNewIndices;
        iIQuantity = 0;
        int iQ = qIndexArray.GetQuantity()/3;
        for (int i1 = 0; i1 < iQ; i1++)
        {
            iV0 = qIndexArray[iIQuantity++];
            iV1 = qIndexArray[iIQuantity++];
            iV2 = qIndexArray[iIQuantity++];

            if ( !qAlreadyDividedEdges.IsPresent(MakeLink(iV0,iV1)) )
            {
                iMV0 = qVertexArray.GetQuantity();
                qVertexArray.Push((qVertexArray[iV0]+qVertexArray[iV1])*((Real)0.5));
                qVertexArray[iMV0].Normalize();
                qAlreadyDividedEdges.Insert(MakeLink(iV0,iV1));
                qAlreadyDividedEdges.Insert(MakeLink(iV1,iV0));
                qEdgeToMiddlePoint[MakeLink(iV0,iV1)] = iMV0;
                qEdgeToMiddlePoint[MakeLink(iV1,iV0)] = iMV0;
            }
            else
            {
                iMV0 = qEdgeToMiddlePoint.Find(MakeLink(iV0,iV1))->Second;
            }

            if ( !qAlreadyDividedEdges.IsPresent(MakeLink(iV1,iV2)) )
            {
                iMV1 = qVertexArray.GetQuantity();
                qVertexArray.Push((qVertexArray[iV1]+qVertexArray[iV2])*((Real)0.5));
                qVertexArray[iMV1].Normalize();
                qAlreadyDividedEdges.Insert(MakeLink(iV1,iV2));
                qAlreadyDividedEdges.Insert(MakeLink(iV2,iV1));
                qEdgeToMiddlePoint[MakeLink(iV1,iV2)] = iMV1;
                qEdgeToMiddlePoint[MakeLink(iV2,iV1)] = iMV1;
            }
            else
            {
                iMV1 = qEdgeToMiddlePoint.Find(MakeLink(iV1,iV2))->Second;
            }

            if ( !qAlreadyDividedEdges.IsPresent(MakeLink(iV2,iV0)) )
            {
                iMV2 = qVertexArray.GetQuantity();
                qVertexArray.Push((qVertexArray[iV2]+qVertexArray[iV0])*((Real)0.5));
                qVertexArray[iMV2].Normalize();
                qAlreadyDividedEdges.Insert(MakeLink(iV2,iV0));
                qAlreadyDividedEdges.Insert(MakeLink(iV0,iV2));
                qEdgeToMiddlePoint[MakeLink(iV2,iV0)] = iMV2;
                qEdgeToMiddlePoint[MakeLink(iV0,iV2)] = iMV2;
            }
            else
            {
                iMV2 = qEdgeToMiddlePoint.Find(MakeLink(iV2,iV0))->Second;
            }

            qNewIndices.Push(iMV2);
            qNewIndices.Push(iV0);
            qNewIndices.Push(iMV0);

            qNewIndices.Push(iMV1);
            qNewIndices.Push(iMV0);
            qNewIndices.Push(iV1);

            qNewIndices.Push(iV2);
            qNewIndices.Push(iMV2);
            qNewIndices.Push(iMV1);

            qNewIndices.Push(iMV2);
            qNewIndices.Push(iMV0);
            qNewIndices.Push(iMV1);
        }
        qIndexArray = qNewIndices;
    }

    Vector3<Real>* aqVertex = new Vector3<Real>[qVertexArray.GetQuantity()];
    int* aiIndex = new int[qIndexArray.GetQuantity()];

    for (int i = 0; i < qVertexArray.GetQuantity(); i++)
        aqVertex[i] = fRadius*qVertexArray[i];

    iIQuantity = 0;
    int iTriQuantity = qIndexArray.GetQuantity()/3;
    for (int i = 0; i < iTriQuantity; i++)
    {
        iV0 = qIndexArray[iIQuantity];
        iV1 = qIndexArray[iIQuantity+1];
        iV2 = qIndexArray[iIQuantity+2];
        AddTriangle(iIQuantity,aiIndex,iV0,iV1,iV2);
    }

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(qVertexArray.GetQuantity(),aqVertex,qIndexArray.GetQuantity(),
            aiIndex,aqNormal);
    }

    for (int i = 0; i < qVertexArray.GetQuantity(); i++)
        aqVertex[i] += rqCenter;

    return BasicMeshRecord(qVertexArray.GetQuantity(),aqVertex,qIndexArray.GetQuantity(),aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
typename BasicMeshConstruction<Real>::BasicMeshRecord BasicMeshConstruction<Real>::ConstructTeapot (
    bool bConstructNormals, const Vector3<Real>& rqCenter, Real fSize, int iDegree)
{
    assert( fSize >= (Real)0.0 && iDegree >= 1 );

    int iDPO = iDegree + 1;

    int iVertexQuantity = 32*Mathi::Sqr(iDPO);
    Vector3<Real>* aqVertex = new Vector3<Real>[iVertexQuantity];

    int iIndexQuantity = 32*6*Mathi::Sqr(iDegree);
    int* aiIndex = new int[iIndexQuantity];

    // precompute Bernstein basis functions
    StillArray<StillArray<Real> > qBernBF(iDPO);
    Real fT, fSqrT, fOMT, fSqrOMT;
    for (int i = 0; i < iDPO; i++)
    {
        StillArray<Real>& rqBF = qBernBF[i];
        rqBF.SetQuantity(4);

        fT = ((Real)i)/iDegree;
        fSqrT = fT*fT;
        fOMT = (Real)1.0 - fT;
        fSqrOMT = fOMT*fOMT;
        rqBF[0] = fSqrOMT*fOMT;
        rqBF[1] = ((Real)3.0)*fT*fSqrOMT;
        rqBF[2] = ((Real)3.0)*fSqrT*fOMT;
        rqBF[3] = fSqrT*fT;
    }

    // initial mesh variant with separated patches/submeshes
    StillArray<ModestArray<int> > qVerticesCIs(iVertexQuantity);
    int iVQuantity = 0;
    int iIQuantity = 0;
    int iV0, iV1, iV2, iV3, iRemVQ, iVar;
    for (int i0 = 0; i0 < 32; i0++)
    {
        iRemVQ = iVQuantity;

        for (int iU = 0; iU < iDPO; iU++)
        {
            for (int iV = 0; iV < iDPO; iV++)
            {
                Vector3<Real>& rqPoint = aqVertex[iVQuantity++];

                rqPoint = Vector3<Real>::ZERO;
                for (int i1 = 0; i1 < 4; i1++)
                {
                    for (int i2 = 0; i2 < 4; i2++)
                    {
                        rqPoint += ms_aqTeapotVertices[ms_aaaiTeapotIndices[i0][i1][i2]]*
                            qBernBF[iU][i1]*qBernBF[iV][i2];
                    }
                }
            }
        }

        for (int i1 = 0; i1 < iDegree; i1++)
        {
            iVar = iRemVQ + i1*iDPO;
            for (int i2 = 0; i2 < iDegree; i2++)
            {
                iV0 = iVar + i2;
                iV1 = iV0 + 1;
                iV2 = iV1 + iDPO;
                iV3 = iV2 - 1;

                qVerticesCIs[iV0].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV0;
                qVerticesCIs[iV1].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV1;
                qVerticesCIs[iV2].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV2;
                qVerticesCIs[iV0].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV0;
                qVerticesCIs[iV2].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV2;
                qVerticesCIs[iV3].Push(iIQuantity);
                aiIndex[iIQuantity++] = iV3;
            }
        }
    }
    qBernBF.RemoveAll();

    // construct the new vertex array by joining submeshes where they contact each other, and alter the array
    // of indices
    Array<Vector3<Real> > qNewVertex;
    Map<Vector3<Real>,int> qNewVertexToIndex;
    Map<Vector3<Real>,int>::IteratorT qIt;
    for (int i0 = 0; i0 < iVertexQuantity; i0++)
    {
        const Vector3<Real>& rqPoint = aqVertex[i0];
        const ModestArray<int>& rqCIs = qVerticesCIs[i0];

        qIt = qNewVertexToIndex.Find(rqPoint);
        if ( qIt == qNewVertexToIndex.GetEnd() )
        {
            for (int i1 = 0; i1 < rqCIs.GetQuantity(); i1++)
                aiIndex[rqCIs[i1]] = qNewVertex.GetQuantity();
            qNewVertexToIndex.Insert(rqPoint,qNewVertex.GetQuantity());
            qNewVertex.Push(rqPoint);
        }
        else
        {
            for (int i1 = 0; i1 < rqCIs.GetQuantity(); i1++)
                aiIndex[rqCIs[i1]] = qIt->Second;
        }
    }
    qVerticesCIs.RemoveAll();
    qNewVertexToIndex.RemoveAll();
    delete[] aqVertex;
    iVertexQuantity = qNewVertex.GetQuantity();
    aqVertex = new Vector3<Real>[iVertexQuantity];
    MemCpy(aqVertex,qNewVertex,iVertexQuantity*sizeof(Vector3<Real>));
    qNewVertex.RemoveAll();

    // rotate (to stand on the XZ plane), scale, and move
    Matrix3<Real> qTransform;
    qTransform =
        Matrix3<Real>(fSize,fSize,fSize)*
        qTransform.FromAxisAngle(Vector3<Real>::UNIT_X,-Math<Real>::HALF_PI);
    for (int i = 0; i < iVertexQuantity; i++)
        aqVertex[i] = rqCenter + qTransform*aqVertex[i];

    Vector3<Real>* aqNormal = 0;
    if ( bConstructNormals )
    {
        // construct normals
        GrphUtils3<Real>::ConstructNormals(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
    }

    return BasicMeshRecord(iVertexQuantity,aqVertex,iIndexQuantity,aiIndex,aqNormal);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void BasicMeshConstruction<Real>::AddTriangle (int& riIQuantity, int* aiIndex, int iV0, int iV1, int iV2)
{
    aiIndex[riIQuantity++] = iV0;
    aiIndex[riIQuantity++] = iV1;
    aiIndex[riIQuantity++] = iV2;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void BasicMeshConstruction<Real>::AddConvexQuadrangle (int& riIQuantity, int* aiIndex, int iV0, int iV1, int iV2,
    int iV3)
{
    aiIndex[riIQuantity++] = iV0;
    aiIndex[riIQuantity++] = iV1;
    aiIndex[riIQuantity++] = iV2;
    aiIndex[riIQuantity++] = iV0;
    aiIndex[riIQuantity++] = iV2;
    aiIndex[riIQuantity++] = iV3;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void BasicMeshConstruction<Real>::AddConvexPolygon (int& riIQuantity, int* aiIndex, int iVQuantity,
    const int* aiVIndex, bool bFromFirstToLast)
{
    if ( bFromFirstToLast )
    {
        for (int i = 0; i < iVQuantity-2; i++)
        {
            aiIndex[riIQuantity++] = aiVIndex[0];
            aiIndex[riIQuantity++] = aiVIndex[i+1];
            aiIndex[riIQuantity++] = aiVIndex[i+2];
        }
    }
    else
    {
        for (int i = 0; i < iVQuantity-2; i++)
        {
            aiIndex[riIQuantity++] = aiVIndex[0];
            aiIndex[riIQuantity++] = aiVIndex[iVQuantity-i-1];
            aiIndex[riIQuantity++] = aiVIndex[iVQuantity-i-2];
        }
    }
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class BasicMeshConstruction<float>;

// double
template
class BasicMeshConstruction<double>;
//------------------------------------------------------------------------------------------------------------------



