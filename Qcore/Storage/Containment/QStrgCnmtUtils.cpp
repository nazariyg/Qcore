#include "QStrgCnmtUtils.h"

namespace Q
{
//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// Native.
template
void SwapObjects<int>(int&,int&);

template
Link<int,int> MakeLink<int,int>(const int&,const int&);

template
struct DefaultEquator<int>;

template
struct DefaultComparator<int>;

template
struct DefaultEqComparator<int>;

template
struct DefaultHasher<int>;

// Aggregate.
template
void SwapObjects<Link<int,int> >(Link<int,int>&,Link<int,int>&);

template
Link<Link<int,int>,Link<int,int> > MakeLink<Link<int,int>,Link<int,int> >(const Link<int,int>&,
    const Link<int,int>&);

template
struct DefaultEquator<Link<int,int> >;

template
struct DefaultComparator<Link<int,int> >;

template
struct DefaultEqComparator<Link<int,int> >;

template
struct DefaultHasher<Link<int,int> >;
//------------------------------------------------------------------------------------------------------------------
}



