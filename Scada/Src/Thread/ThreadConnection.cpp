#include "ThreadConnection.h"
#include "../Thread/ThreadRecv.h"
#include <QDebug>

TThreadConnection::TThreadConnection(TService *Service, QObject *parent)
    : TThread(parent)
{
    fSetStartMode(START_MODE_INIT_FIRST);

    mService = Service;

    if(mService)
    {
        // Protocol若在run中创建，则不能正确接收SendTo
        mService->fCreateProtocol();
    }
}

void TThreadConnection::fSetRunMode(int RunMode)
{
    mRunMode = RunMode;
}

TThreadConnection::~TThreadConnection()
{
}

void TThreadConnection::run()
{
    //qDebug() << "TThreadConnection::run stopped:" << stopped;
    qDebug() << "TThreadConnection::run()" << QThread::currentThread();

    TProtocol *Protocol = NULL;
    TConnection *Connection = NULL;

    if(mService)
    {
        Protocol = mService->fGetProtocol();
        //Protocol = mService->fCreateProtocol();
        Connection = mService->fCreateConnection();

        //qDebug() << "mService->fGetConnectMode()" << mService->fGetConnectMode();
        fSetRunMode(mService->fGetConnectMode());
    }
    else
    {
        return;
    }

    mInited = true;

    while(mStopped)
    {
        qDebug() << "wait for start";
        sleep(1);
        continue;
    }

    TThreadRecv *ThreadRecv = NULL;
    //
    if(TService::RECV_MODE_AUTO == mService->fGetRecvMode())
    {
        ThreadRecv = new TThreadRecv();
        ThreadRecv->fSetConnection(Connection);
        ThreadRecv->fStart();
    }

    while(!mStopped)
    {
        if(!Connection)
        {
            goto WAIT;
        }

        //qDebug() << "TThreadConnection::run mode:" << mRunMode << "state:" << Connection->fGetState();

        // 自动关闭
        if(TConnection::STATE_CLOSEING == Connection->fGetState())
        {
            Connection->fStop();
        }

        // 自动连接：状态为初始化或者已关闭
        if(TConnection::STATE_RUNING == Connection->fGetState() // 准备连接
                || (THREAD_RUN_MODE_AUTO == mRunMode // 或者处于自动模式下的初始化和关闭状态
                    && (TConnection::STATE_INIT == Connection->fGetState()
                        || TConnection::STATE_CLOSEED  == Connection->fGetState()
                        )
                    )
          )
        {
            Connection->fStart();

            if(Protocol)
            {
                Protocol->fSetState(TProtocol::STATE_START);
            }
        }

        //qDebug() << "Connection->fGetState()" << Connection->fGetState();

        if(TConnection::STATE_RUNED != Connection->fGetState())
        {
            goto WAIT;
        }

        if(NULL == Protocol)
        {
            goto WAIT;
        }

        //qDebug() << "Protocol->fAnaly(Connection)";

        switch(Protocol->fAnaly(Connection))
        {
        case TProtocol::PROTOCOL_IDLE:
            goto WAIT;
            break;

        case TProtocol::PROTOCOL_ERR:
            goto RUN_ERR;
            break;

        case TProtocol::PROTOCOL_OK:
            goto NEXT;
            break;

        default:
            goto RUN_ERR;
            break;
        }

WAIT:
        sleep(1);
        continue;

RUN_ERR:
        continue;

NEXT:
        continue;
    }

    //exec();

    mStopped = false;

    if(ThreadRecv)
    {
        ThreadRecv->fStop();
        ThreadRecv->deleteLater();
    }

    if(mService)
    {
        mService->fDestroyProtocol();
        mService->fDestroyConnection();
    }
}
