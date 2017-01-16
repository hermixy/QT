#ifndef THREAD_SERVER_H
#define THREAD_SERVER_H

/*
 * 服务器 用于 创建收发、数据处理线程，并监听新连接
*/

#include "Thread.h"

#include "../Service/Service.h"
#include "../Protocol/Protocol.h"
#include "../Server/TcpServer/TcpServer.h"
#include "../RingBuf/RingBuf.h"
#include "ThreadIO.h"
#include "ThreadProcess.h"

class TThreadServer : public TThread
{
    Q_OBJECT

public:
    TThreadServer(TService *Service, QObject *parent = 0);
    ~TThreadServer();

    virtual void fStop();

Q_SIGNALS:
    void mNewSocket(QObject *DstObj, qintptr handle);

protected:
    virtual void run();
    virtual void fStartRun();   // 开始运行Run函数

private:
    int mThreadIOSum;
    int mThreadProcessSum;
    QList<TThreadIO *> mThreadIOList;           // 负责收发的线程列表
    QList<TThreadProcess *> mThreadProcessList; // 负责数据处理的线程列表
    TService *mService;
};

#endif // THREAD_SERVER_H
