#include "ThreadProcess.h"

#include <QDebug>

TThreadProcess::TThreadProcess(TProtocol *Protocol, QObject *parent) : TThreadList(parent)
{
    mProtocol = Protocol;
}

TThreadProcess::~TThreadProcess()
{
}

void TThreadProcess::run()
{
    bool Idle;

    while(!mStopped)
    {
        Idle = true;

        mLock.lock();

        foreach(TConnection *Conn, mConnectList)
        {
            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                TRingBuf *Ringbuf = Conn->fGetReadRingbuf();

                if(Ringbuf->fDataSize() > 0)
                {
                    Idle = false;
                    qDebug() << "TThreadProcess::run() analy at" << QThread::currentThread();

                    mProtocol->fAnaly(Conn);

                    Ringbuf->fClear();
                }
            }
        }

        mLock.unlock();

        if(Idle)
        {
            msleep(200);
        }
    }

    mStopped = false;
}
