#ifndef SYREAL_H
#define SYREAL_H

#include "../Protocol.h"

class TSyreal : public TProtocol
{
    Q_OBJECT

public:
    TSyreal();

    int fAnaly(TConnection *Conn);
};

#endif // SYREAL_H
