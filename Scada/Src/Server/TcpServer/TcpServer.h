#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QList>

class TTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TTcpServer(QObject *parent = 0);
    ~TTcpServer();

    bool fGetNextNewSocket(qintptr &handle);

private:
    void incomingConnection(qintptr handle);

    QList<qintptr> mNewSocketList;
};

#endif // TCPSERVER_H
