#include "ThreadClientServer.h"

#include "../Connection/Connection.h"
#include <QDebug>

TThreadClinetServer::TThreadClinetServer(TTcpServer *Server, QObject *parent) : QThread(parent)
{
    if(Server)
    {
        stopped = false;

        mTcpServer = Server;

        connect(mTcpServer, SIGNAL(mNewSocket(TConnection*)), this, SLOT(fNewSocket(TConnection*)));
    }

    //qDebug() << "TThreadClinetServer" << this->currentThreadId();
    connect(this, SIGNAL(mSendData(TConnection*,TClientData*)), this, SLOT(fSendData(TConnection*,TClientData*)));
}

TThreadClinetServer::~TThreadClinetServer()
{
    stopped = true;

    foreach(TClientData *Data, mDataList)
    {
        delete Data;
    }

    mDataList.clear();

    // 等待线程结束
    wait();
}

void TThreadClinetServer::stop()
{
    stopped = true;
}

void TThreadClinetServer::fAddData(int ServiceId, int NodeId, int Value)
{
    TClientData *Find = 0;

    foreach(TClientData *Data, mDataList)
    {
        if(Data->mServiceId == ServiceId && Data->mNodeId == NodeId)
        {
            Find = Data;
        }
    }

    if(Find)
    {
        Find->mValue = Value;
    }
    else
    {
        Find = new TClientData(ServiceId, NodeId, Value);

        if(Find)
        {
            mDataList.push_back(Find);
        }
    }

    if(Find)
    {        
        mNewData.push_back(Find);
    }
}

void TThreadClinetServer::fSendData(TConnection *Socket, TClientData* Data)
{
    if(Socket && Data)
    {
        char pcSend[8];
        pcSend[0]    = 0x68;
        pcSend[1]    = 0x3;
        pcSend[2]    = 0x3;
        pcSend[3]    = 0x68;
        pcSend[4]    = Data->mServiceId;
        pcSend[5]    = Data->mNodeId;
        pcSend[6]    = Data->mValue;
        pcSend[7]    = 0x16;

        Socket->fSend(pcSend, 8);
    }
}

void TThreadClinetServer::fNewSocket(TConnection *Con)
{
    if(0 == Con)
    {
        return;
    }

    foreach(TClientData *Data, mDataList)
    {
        //qDebug() << "fNewSocket send data";
        fSendData(Con, Data);
    }
}

void TThreadClinetServer::run()
{
    TClientData *ClientData = 0;

    //qDebug() << "TThreadClinetServer->run()" << this->currentThreadId();

    while(!stopped)
    {
        if(0 == mTcpServer || mTcpServer->mSocketList.length() <= 0)
        {
            //qDebug() << "TThread Client::run() no Socket";
            sleep(1);
            continue;
        }

        if(mNewData.count() < 1)
        {
            //qDebug() << "list no data";
            sleep(1);
            continue;
        }

        ClientData = 0;
        //ClientData = mNewData.at(0);
        ClientData = mNewData.first();

        if(0 == ClientData)
        {
            //qDebug() << "pop err";
            sleep(1);
            continue;
        }

        mTcpServer->mListLock.lock();
        foreach(TConnection *Socket, mTcpServer->mSocketList)
        {
            if(0 == Socket)
            {
                continue;
            }

            // 发送数据
            //qDebug() << "run send data";
            emit mSendData(Socket, ClientData);

            if(Socket && TConnection::STATE_CLOSEING == Socket->fGetState())
            {
                qDebug() << "thread TConnection::STATE_CLOSEING";
                mTcpServer->fDelSocket(Socket);
            }
        }
        mTcpServer->mListLock.unlock();

        qDebug() << "before mNewData.removeOne" << mNewData.count();
        mNewData.removeOne(ClientData);
    }

    stopped = false;
}
