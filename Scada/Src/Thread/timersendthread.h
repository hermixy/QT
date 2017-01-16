#ifndef TIMERSENDTHREAD_H
#define TIMERSENDTHREAD_H
#include <QMutex>
#include <QThread>
#include <QUdpSocket>
#include <QObject>
#include <QTimer>

class TimerSendThread : public QThread
{
    Q_OBJECT
public:
    TimerSendThread(QObject *parent=0);
    ~TimerSendThread();
    void run();
    void stop();
    static QHash<QString, QList<QByteArray>> sendList;
    static QHash<QString, QList<QByteArray>> sendThreeList;
    //static QHash<QString, QList<int>> ipToStarr;
    //QTimer *timer;
    bool sTop=false;
public slots:
    //void sendSeckThree();
};

#endif // TIMERSENDTHREAD_H
