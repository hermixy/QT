#ifndef TTHREAD_H
#define TTHREAD_H

#include <QMutex>
#include <QThread>

/*
    所有线程的基类，实现启动、停止等方法
*/

class TThread : public QThread
{
    Q_OBJECT

public:
    TThread(QObject *parent = 0);
    virtual ~TThread();

    virtual void fInit();
    virtual void fStart();
    virtual void fStop();

    enum
    {
        START_MODE_NORMAL,      // 正常模式，直接启动，停止
        START_MODE_INIT_FIRST   // 启动前 必须先调用fInit
    };

    virtual void fSetStartMode(int Mode);

protected:
    QMutex mLock;

    volatile bool mInited;      // 是否初始化
    volatile bool mStopped;     // 是否停止
    virtual void run();
    virtual void fStartRun();   // 开始运行Run函数

    volatile int mStartMode;
};

#endif // TTHREAD_H
