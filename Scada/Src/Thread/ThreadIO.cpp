#include "ThreadIO.h"

#include "../Connection/TcpSocket/TcpSocket.h"

#include <QDebug>

TThreadIO::TThreadIO(QObject *parent) : TThreadList(parent)
{
}

TThreadIO::~TThreadIO()
{
}

void TThreadIO::fNewSocket(QObject *DstObj, qintptr handle)
{
    //qDebug() << "TThreadIO::fNewSocket" << DstObj << this;

    if(this == DstObj)
    {
        mNewSocketList.push_back(handle);
    }
}

void TThreadIO::run()
{
    bool Idle;
    char pcRecv[256];

    while(!mStopped)
    {
        // 新建连接
        //qDebug() << "mNewSocketList len" << mNewSocketList.length();
        foreach(qintptr handle, mNewSocketList)
        {
            TTcpSocket *Socket = new TTcpSocket();

            Socket->fCreateReadRingbuf(1024);
            Socket->fCreateWriteRingbuf(1024);
            Socket->fSetSocketDescriptor(handle);
            Socket->fSetState(TConnection::STATE_RUNED);
            Socket->fStart();

            //qDebug() << "fAddConnection(Socket)" << Socket;

            fAddConnection(Socket);
            emit mAddConnection(Socket);

            mNewSocketList.removeOne(handle);
        }

        // 处理所有收发
        mLock.lock();

        Idle = true;

        foreach(TConnection *Conn, mConnectList)
        {
            //qDebug() << "TThreadIO::run() conn state" << Conn->fGetState();

            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                int iRead = Conn->fRecv(pcRecv, 256);

                if(iRead > 0)
                {
                    qDebug() << "Socket Read at " << QThread::currentThread() << iRead;

                    TRingBuf *Ringbuf = Conn->fGetReadRingbuf();

                    if(Ringbuf)
                    {
                        Ringbuf->fWrite(iRead, pcRecv);
                    }

                    Idle = false;
                }
                else if(iRead < 0)
                {
                    Conn->fSetState(TConnection::STATE_CLOSEING);
                    emit mDelConnection(Conn);
                }
            }

            if(TConnection::STATE_CLOSEING == Conn->fGetState() // 析构已关闭连接
                    && Conn->fGetReference() <= 1)
            {
                Conn->fSetState(TConnection::STATE_DESTROY);

                DelConn(Conn);

                Conn->deleteLater();
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
