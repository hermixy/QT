#ifndef THREAD_CONNECTION_H
#define THREAD_CONNECTION_H

#include "Thread.h"

/*
    用于串口 网络客户端的接收和解析；
    或者创建一个单独的接收线程，本线程仅用于解析
*/

#include "../Service/Service.h"
#include "../Protocol/Protocol.h"
#include "../Connection/Connection.h"
#include "../RingBuf/RingBuf.h"

class TThreadConnection : public TThread
{
    Q_OBJECT

public:
    TThreadConnection(TService *Service, QObject *parent = 0);
    ~TThreadConnection();

    enum
    {
        THREAD_RUN_MODE_NORMAL,
        THREAD_RUN_MODE_AUTO
    };

    void fSetRunMode(int RunMode);

protected:
    void run();

private:
    TService *mService;

    int mRunMode;
};

#endif // THREAD_CONNECTION_H
