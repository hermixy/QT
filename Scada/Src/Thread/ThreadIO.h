#ifndef TTHREAD_IO_H
#define TTHREAD_IO_H

/*
 * 服务器 用于 收发的线程
*/

#include "ThreadList.h"

class TThreadIO : public TThreadList
{
    Q_OBJECT

public:
    TThreadIO(QObject *parent = 0);
    virtual ~TThreadIO();

public slots:
    void fNewSocket(QObject *DstObj, qintptr handle);

protected:
    QList<qintptr> mNewSocketList;
    virtual void run();
};

#endif // TTHREAD_IO_H
