#include "thread.h"

#include <QDebug>

TThreadV6::TThreadV6(TTcpServer *TcpServer, QObject *parent) : QThread(parent)
{
    stopped = false;

    mTcpServer = TcpServer;
}

TThreadV6::~TThreadV6()
{
    stopped = true;
}

void TThreadV6::stop()
{
    stopped = true;
}

int TThreadV6::fAnalyRingbuf(TTcpSocket *TcpSocket, TRingbuf *Ringbuf)
{
    if(NULL == Ringbuf)
    {
        return ReturnErr;
    }

    char pcRecv[256];
    int iRead = 0;

    if(Ringbuf->fDataSize() < 1)
    {
        return ReturnIdle;
    }

    iRead = Ringbuf->fRead(255, pcRecv);

    if(iRead < 1)
    {
        qDebug() << "read data err";
        return ReturnErr;
    }

    pcRecv[iRead] = 0;

    emit mDataCome(TcpSocket->peerName(), TcpSocket->peerAddress().toString(), QString::number(TcpSocket->peerPort()), QString(pcRecv));

    return ReturnOK;
}

void TThreadV6::run()
{
    while(!stopped)
    {
        if(0 == mTcpServer || mTcpServer->mSocketList.length() <= 0)
        {
            //qDebug() << "TThreadV6::run() no Socket";
            sleep(1);
            continue;
        }

        mTcpServer->mListLock.lock();
        foreach(TTcpSocket *mSocket, mTcpServer->mSocketList)
        {
            //qDebug() << "run mSocket:" << mSocket;
            if(ReturnOK != fAnalyRingbuf(mSocket, mSocket->mRingbuf))
            {
                sleep(1);
            }

            if(TTcpSocket::Close == mSocket->mState && 0 == mSocket->mRingbuf->fDataSize())
            {
                mTcpServer->fDelSocket(mSocket);
            }
        }
        mTcpServer->mListLock.unlock();
    }

    stopped = false;
}
