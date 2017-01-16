#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "../Connection.h"
#include "../../RingBuf/RingBuf.h"

//#include <QApplication>
#include <QThread>
#include <QTcpSocket>

class TTcpSocket : public TConnection
{
    //Q_OBJECT
    //Q_DECLARE_TR_FUNCTIONS(TTcpSocket)

public:
    TTcpSocket(QObject *parent = 0);
    ~TTcpSocket();

    int fStart();
    int fStop();
    bool fSetHost(QString Ip, int Port);
    int fRecv(void *Dst, int Len);
    int fSend(const void *Data, int Len);

    bool fSetSocketDescriptor(qintptr handle);
    QString fGetPeerName();
    QString fGetPeerAddr();
    QString fGetPeerPort();

private:
    QTcpSocket *mSocket;
    QString mIp;
    int mPort;

    void fSocketConnected();
    void fSocketDisconnect();
    void fSocketConnectErr(QAbstractSocket::SocketError err);
};

#endif // TCPSOCKET_H
