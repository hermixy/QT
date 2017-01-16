#ifndef SYGZZS_H
#define SYGZZS_H

#include "../Protocol.h"

class TSygzzs : public TProtocol
{
    Q_OBJECT

public:
    TSygzzs();
    int fAnaly(TConnection *Conn);
};

#endif // SYGZZS_H
