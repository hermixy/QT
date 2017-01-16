#ifndef TTHREAD_LIST_H
#define TTHREAD_LIST_H

/*
    管理一个连接列表，作为一个基类使用
*/

#include "Thread.h"
#include "../Connection/Connection.h"

class TThreadList : public TThread
{
    Q_OBJECT

public:
    TThreadList(QObject *parent = 0);
    virtual ~TThreadList();

    virtual int fGetConnectionSum();

public slots:
    virtual int fAddConnection(TConnection *Connection);
    virtual int fDelConnection(TConnection *Connection);

Q_SIGNALS:
    void mAddConnection(TConnection *Connection);
    void mDelConnection(TConnection *Connection);

protected:

    virtual void run();

    QList<TConnection *> mConnectList;
    int AddConn(TConnection *Connection);
    int DelConn(TConnection *Connection);
};

#endif // TTHREAD_LIST_H
