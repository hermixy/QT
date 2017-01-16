#include "ThreadRecv.h"

#include <QDebug>

TThreadRecv::TThreadRecv(QObject *parent) : TThread(parent)
{
    mConn = NULL;
}

TThreadRecv::~TThreadRecv()
{
}

void TThreadRecv::fSetConnection(TConnection *Conn)
{
    mConn = Conn;
}

void TThreadRecv::run()
{
    char pcRecv[256];

    while(!mStopped)
    {
        if(NULL == mConn || TConnection::STATE_RUNED != mConn->fGetState())
        {
            sleep(1);
            continue;
        }

        int iRead = mConn->fRecv(pcRecv, 256);

        //qDebug() << "Socket Read at " << QThread::currentThread() << iRead;

        if(iRead > 0)
        {
            TRingBuf *Ringbuf = mConn->fGetReadRingbuf();

            if(Ringbuf)
            {
                Ringbuf->fWrite(iRead, pcRecv);
            }
        }
        else if(iRead < 0)
        {
            mConn->fSetState(TConnection::STATE_CLOSEING);
        }
    }

    mStopped = false;
}
