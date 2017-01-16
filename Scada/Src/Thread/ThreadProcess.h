#ifndef TTHREAD_PROCESS_H
#define TTHREAD_PROCESS_H

/*
 * 服务器 用于 数据处理的线程
*/

#include "ThreadList.h"
#include "../Protocol/Protocol.h"

class TThreadProcess : public TThreadList
{
    Q_OBJECT

public:
    TThreadProcess(TProtocol *Protocol, QObject *parent = 0);
    virtual ~TThreadProcess();

protected:
    TProtocol *mProtocol;
    virtual void run();
};

#endif // TTHREAD_PROCESS_H
