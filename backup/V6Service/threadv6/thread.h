#ifndef THREAD_H
#define THREAD_H

#include "../tcp/tcpserver.h"

#include "../tcp/tcpserver.h"

#include <QThread>

class TThreadV6 : public QThread
{
    Q_OBJECT

public:
    TThreadV6(TTcpServer *TcpServer, QObject *parent = 0);
    ~TThreadV6();

    void stop();

Q_SIGNALS:
    void mDataCome(const QString Name, const QString IP, const QString Port, const QString pcRecv);

protected:
    void run();

private:
    volatile bool stopped;
    enum AnalyReturn
    {
        ReturnIdle,
        ReturnErr,
        ReturnOK
    };

    int fAnalyRingbuf(TTcpSocket *TcpSocket, TRingbuf *Ringbuf);
    TTcpServer *mTcpServer;
};

#endif // THREAD_H
