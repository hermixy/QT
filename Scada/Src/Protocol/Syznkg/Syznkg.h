#ifndef SYZNKG_H
#define SYZNKG_H

#include "../Protocol.h"

class TSyznkg : public TProtocol
{
    Q_OBJECT

public:
    TSyznkg();
    int fAnaly(TConnection *Conn);
};

#endif // SYZNKG_H
