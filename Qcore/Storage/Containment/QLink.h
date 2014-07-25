#ifndef QLINK_H
#define QLINK_H

// The class lets link together two objects of arbitrary type so that operate them as one.  If usage of an
// inter-link comparison operator is implied, the FirstT and SecondT template parameters may require to be
// equipped with one of the following member functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// where Class is the same as FirstT or SecondT, the 'const' and '&' are optional.

#include "QStream.h"

namespace Q
{

template <class FirstT, class SecondT>
class Link
{

Q_DECLARE_STREAM;

public:
    // construction
    Link ();  // uninitialized
    Link (const FirstT& rtFirst, const SecondT& rtSecond);
    Link (const Link& rqLink);

    // comparison
    bool operator== (const Link& rqLink) const;
    bool operator!= (const Link& rqLink) const;
    bool operator<  (const Link& rqLink) const;
    bool operator<= (const Link& rqLink) const;
    bool operator>  (const Link& rqLink) const;
    bool operator>= (const Link& rqLink) const;

    FirstT First;
    SecondT Second;
};

#include "QLink.inl"

}

#endif



