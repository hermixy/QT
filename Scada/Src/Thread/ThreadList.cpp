#include "ThreadList.h"

#include <QDebug>

TThreadList::TThreadList(QObject *parent) : TThread(parent)
{
}

TThreadList::~TThreadList()
{
}

int TThreadList::fGetConnectionSum()
{
    int iRet = 0;

    mLock.lock();
    iRet = mConnectList.length();
    mLock.unlock();

    return iRet;
}

int TThreadList::AddConn(TConnection *Connection)
{
    if(NULL == Connection)
    {
        return ERR;
    }

    Connection->fAddOneToRef();
    mConnectList.push_back(Connection);

    return OK;
}

int TThreadList::DelConn(TConnection *Connection)
{
    if(NULL == Connection)
    {
        return ERR;
    }

    Connection->fDelOneFromRef();
    mConnectList.removeOne(Connection);

    return OK;
}

int TThreadList::fAddConnection(TConnection *Connection)
{
    mLock.lock();

    AddConn(Connection);

    mLock.unlock();

    return OK;
}

int TThreadList::fDelConnection(TConnection *Connection)
{
    mLock.lock();

    DelConn(Connection);

    mLock.unlock();

    return OK;
}

void TThreadList::run()
{
}
