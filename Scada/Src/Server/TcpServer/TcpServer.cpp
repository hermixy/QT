#include "TcpServer.h"

#include <QDebug>

TTcpServer::TTcpServer(QObject *parent) : QTcpServer(parent)
{}

TTcpServer::~TTcpServer()
{
    close();
}

bool TTcpServer::fGetNextNewSocket(qintptr &handle)
{
    if(mNewSocketList.length() <= 0)
    {
        return false;
    }
    else
    {
        handle = mNewSocketList.first();
        mNewSocketList.removeFirst();
    }

    return true;
}

void TTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "TTcpServer::incomingConnection emit mNewSocket";

    mNewSocketList.push_back(handle);
}



