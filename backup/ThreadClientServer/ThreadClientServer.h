#ifndef THREAD_CLIENT_SERVER_H
#define THREAD_CLIENT_SERVER_H

#include "../Connection/TcpServer/TcpServer.h"

#include <QThread>

class TClientData
{
public:
    TClientData(int ServiceId, int NodeId, int Value)
    {
        mServiceId = ServiceId;
        mNodeId = NodeId;
        mValue = Value;
    }

    ~TClientData(){}

    int mServiceId;
    int mNodeId;
    int mValue;
};

class TThreadClinetServer : public QThread
{
    Q_OBJECT

public:
    TThreadClinetServer(TTcpServer *Server, QObject *parent = 0);
    ~TThreadClinetServer();

    void stop();

    void fAddData(int ServiceId, int NodeId, int Value);

protected:
    void run();

Q_SIGNALS:
    void mSendData(TConnection *Socket, TClientData* Data);

private slots:
    void fNewSocket(TConnection *Con);
    void fSendData(TConnection *Socket, TClientData* Data);

private:
    volatile bool stopped;
    TTcpServer *mTcpServer;

    QList<TClientData *>mDataList;
    QList<TClientData *>mNewData;    
};

#endif // THREAD_CLIENT_SERVER_H
