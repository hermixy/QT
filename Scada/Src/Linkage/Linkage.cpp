#include "Linkage.h"

TLinkage::TLinkage()
{
    mSrcServiceId = 0;
    mSrcNodeId = 0;

    mDstServiceId = 0;
    mDstNodeId = 0;
}

TLinkage::TLinkage(const TLinkage *CopyLinkage)
{
    mSrcServiceId = CopyLinkage->mSrcServiceId;
    mSrcNodeId = CopyLinkage->mSrcNodeId;

    mDstServiceId = CopyLinkage->mDstServiceId;
    mDstNodeId = CopyLinkage->mDstNodeId;
}

TLinkage::~TLinkage()
{
}

