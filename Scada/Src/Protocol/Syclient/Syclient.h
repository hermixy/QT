#ifndef SYCLIENT_H
#define SYCLIENT_H

#include "../Protocol.h"

class TSyclient : public TProtocol
{
    Q_OBJECT

public:
    TSyclient();
    enum
    {
        TYPE_MIN,
        TYPE_DATA,          // 变位数据
        TYPE_LAST,          // 最新数据
        TYPE_HISTORY,       // 历史数据
        TYPE_ALARM,         // 报警信息
        TYPE_MAX
    };

    int fAnaly(TConnection *Conn);
};

#endif // SYCLIENT_H
