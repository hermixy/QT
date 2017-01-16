#include "Thread.h"

#include <QDebug>

TThread::TThread(QObject *parent) : QThread(parent)
{
    mInited = false;
    mStopped = false;

    mStartMode = START_MODE_NORMAL;
}

TThread::~TThread()
{
    fStop();
}

void TThread::fInit()
{
    if(START_MODE_INIT_FIRST == mStartMode)
    {
        if(mInited)
        {
            return;
        }

        mStopped = true;

        fStartRun();

        // 启动后，等待run函数中改变mInited状态

        int Count = 5;

        while(Count-- > 0)
        {
            if(mInited)
            {
                break;
            }
            else
            {
                sleep(1);
            }
        }
    }
    else
    {
        mInited = true;
    }
}

void TThread::fStart()
{
    if(START_MODE_INIT_FIRST == mStartMode)
    {
        if(!mInited)
        {
            fInit();
        }

        mStopped = false;
    }
    else
    {
        mStopped = false;

        fStartRun();
    }
}

void TThread::fStop()
{
    if(isRunning() || !mStopped)
    {
        mStopped = true;
        wait();
    }
}

void TThread::fSetStartMode(int Mode)
{
    mStartMode = Mode;
}

void TThread::fStartRun()
{
    start();
}

void TThread::run()
{
}
