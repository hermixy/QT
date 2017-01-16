#ifndef SENDERORDER_H
#define SENDERORDER_H

#include <QObject>
#include <QThread>
#include "../Service/Service.h"
#include <QUdpSocket>
#include <QByteArray>
#include <QMutex>
class SenderOrder : public QObject
{
    Q_OBJECT

public:
    explicit SenderOrder(QObject *parent = 0);
    ~SenderOrder();
public:

    QString getIp();
    static QMutex mutex;
    static QMutex sendTimerByte;
    QHash<QString, QList<QByteArray>> sendList;
    QHash<QString, QList<QByteArray>> sendThreeList;

public slots:
    void sendOrder(TService *serverData,quint16 startaddr, int mainId, int secondId, int value);
    void timerSendBrocast();
    void ScheldTime();
private:
    QThread *sendOrderThread;
    QUdpSocket *udpSocket;
    QTimer *timer;
    QTimer *SchedleTimer;
};

#endif // SENDERORDER_H
