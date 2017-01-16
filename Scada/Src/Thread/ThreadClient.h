#ifndef THREAD_CLIENT_H
#define THREAD_CLIENT_H

#include "Thread.h"

/*
    用于运行器--客户端
    从数据库获取最新数据，更新页面显示
*/

#include "../DataBase/DataBase.h"

class TThreadClient : public TThread
{
    Q_OBJECT

public:
    TThreadClient(TDataBase *DataBase, QObject *parent = 0);
    ~TThreadClient();

Q_SIGNALS:
    void mValueCome(int ServiceId, int NodeId, double Value, quint64 Time);

protected:
    void run();

private:
    TDataBase *mDataBase;
};

#endif // THREAD_CLIENT_H
