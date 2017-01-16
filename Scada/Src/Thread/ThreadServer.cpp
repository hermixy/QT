#include "ThreadServer.h"

#include "../Connection/Connection.h"
#include <QDebug>

TThreadServer::TThreadServer(TService *Service, QObject *parent)
    : TThread(parent)
{
    fSetStartMode(START_MODE_INIT_FIRST);

    mService = Service;

    if(!mService)
    {
        return;
    }

    TProtocol *Protocol = mService->fCreateProtocol();

    mThreadIOSum = 1;
    mThreadProcessSum = 1;

    TServerData *ServerData = mService->fGetServerData();

    if(ServerData)
    {
        mThreadIOSum = ServerData->fGetIoThreadSum();
        mThreadProcessSum = ServerData->fGetProcessThreadSum();
    }

    if(mThreadIOSum < 1)
    {
        mThreadIOSum = 1;
    }

    if(mThreadProcessSum < 1)
    {
        mThreadProcessSum = 1;
    }

    int multiple = mThreadIOSum / mThreadProcessSum;

    if(multiple < 1)
    {
        multiple = 1;
    }

    mThreadIOSum = multiple * mThreadProcessSum;

    this->fInit();

    for(int i = 0; i < mThreadIOSum; i++)
    {
        TThreadIO *ThreadIO = new TThreadIO();

        if(ThreadIO)
        {
            connect(this, SIGNAL(mNewSocket(QObject*,qintptr)), ThreadIO, SLOT(fNewSocket(QObject*,qintptr)));
            mThreadIOList.push_back(ThreadIO);
        }
    }

    for(int i = 0; i < mThreadProcessSum; i++)
    {
        TThreadProcess *ThreadProcess = new TThreadProcess(Protocol);
        mThreadProcessList.push_back(ThreadProcess);

        for(int j = 0; j < multiple; j++)
        {
            int Num = i * multiple + j;

            if(Num >= 0 && Num < mThreadIOList.length())
            {
                TThreadIO *ThreadIO = mThreadIOList.at(Num);

                if(ThreadIO)
                {
                    connect(ThreadIO, SIGNAL(mAddConnection(TConnection*)), ThreadProcess, SLOT(fAddConnection(TConnection*)));
                    connect(ThreadIO, SIGNAL(mDelConnection(TConnection*)), ThreadProcess, SLOT(fDelConnection(TConnection*)));
                }
            }
        }
    }
}

TThreadServer::~TThreadServer()
{
}

void TThreadServer::fStop()
{
    TThread::fStop();

    // 停止收发线程
    foreach(TThreadIO *ThreadIO, mThreadIOList)
    {
        ThreadIO->fStop();
    }

    // 停止处理线程
    foreach(TThreadProcess *ThreadProcess, mThreadProcessList)
    {
        ThreadProcess->fStop();
    }
}

void TThreadServer::fStartRun()
{
    TThread::fStartRun();

    // 启动收发线程
    foreach(TThreadIO *ThreadIO, mThreadIOList)
    {
        ThreadIO->fStart();
    }

    // 启动处理线程
    foreach(TThreadProcess *ThreadProcess, mThreadProcessList)
    {
        ThreadProcess->fStart();
    }
}

void TThreadServer::run()
{
    TTcpServer *TcpServer = NULL;

    if(mService)
    {
        TcpServer = mService->fCreateServer();
    }

    mInited = true;

    while(mStopped)
    {
        qDebug() << "TThreadServer wait for start";
        sleep(1);
        continue;
    }

    if(mThreadIOList.length() <= 0 ||
            mThreadProcessList.length() <= 0)
    {
        mStopped = true;
    }

    while(!mStopped)
    {
        //qDebug() << "waitForNewConnection(1000) at" << QThread::currentThread();

        if(TcpServer->waitForNewConnection(1000))
        {
        }
        else
        {
            msleep(100);
        }

        qintptr handle;

        while(TcpServer->fGetNextNewSocket(handle))
        {
            TThreadIO *ThreadIOFind = mThreadIOList.first();
            int NumFind = ThreadIOFind->fGetConnectionSum();

            foreach(TThreadIO *ThreadIO, mThreadIOList)
            {
                int Num = ThreadIO->fGetConnectionSum();

                if(Num < NumFind)
                {
                    NumFind = Num;
                    ThreadIOFind = ThreadIO;
                }
            }

            emit mNewSocket(ThreadIOFind, handle);
        }
    }

    mStopped = false;

    if(mService)
    {
        mService->fDestroyServer();
    }
}
