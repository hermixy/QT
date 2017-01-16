#ifndef LINKAGE_H
#define LINKAGE_H

#include "../Js/Js.h"

#include <QString>

class TLinkage
{
public:
    TLinkage();
    TLinkage(const TLinkage *CopyLinkage);
    ~TLinkage();

    int mSrcServiceId;
    int mSrcNodeId;

    int mDstServiceId;
    int mDstNodeId;
};

#endif // LINKAGE_H
