#ifndef TTHREAD_RECV_H
#define TTHREAD_RECV_H

/*
 * 串口或者网口 用于 接收的线程
*/

#include "Thread.h"
#include "../Connection/Connection.h"

class TThreadRecv : public TThread
{
    Q_OBJECT

public:
    TThreadRecv(QObject *parent = 0);
    virtual ~TThreadRecv();

    void fSetConnection(TConnection *Conn);

protected:
    virtual void run();

    TConnection *mConn;
};

#endif // TTHREAD_RECV_H
