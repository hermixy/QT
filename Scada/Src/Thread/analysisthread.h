#ifndef ANALYSISTHREAD_H
#define ANALYSISTHREAD_H
#include <QMutex>
#include <QThread>
#include <QUdpSocket>
struct reciverData{
    QString ip;
    QByteArray data;
};

class AnalysisThread : public QThread
{
    Q_OBJECT
public:
    AnalysisThread(QObject *parent=0);
    ~AnalysisThread();
    void run();
public slots:
    void handleCommand();
public:
    static QList<reciverData> waitHandl;
    static QMutex mutex;
private:

};

#endif // ANALYSISTHREAD_H
