#include "Protocol.h"

#include <QDebug>

TProtocol::TProtocol()
{
    mServiceId = 0;
    mState = STATE_NORMAL;
}

TProtocol::~TProtocol()
{}

void TProtocol::fSetServiceId(int ServiceId)
{
    mServiceId = ServiceId;
}

int TProtocol::fGetServiceId()
{
    return mServiceId;
}

void TProtocol::fSetState(int State)
{
    mState = State;
}

int TProtocol::fGetState()
{
    return mState;
}

/*void TProtocol::fSetConnection(TConnection *Connection)
{
    mConnection = Connection;
}

TConnection* TProtocol::fGetConnection()
{
    return mConnection;
}*/

void TProtocol::fSendTo(QObject *Sender, int Type, double ValueFloat,
                        int ValueInt, QString Text)
{
    qDebug() << "TProtocol::fSendTo" << Sender << Type << ValueFloat << ValueInt << Text;

    mSendToList.push_back(new TSendTo(Sender, Type, ValueFloat, ValueInt, Text));
}

int TProtocol::fGetOneSendTo(TSendTo &SendTo)
{
    TSendTo* Src = NULL;

    if(mSendToList.length() > 0)
    {
        Src = mSendToList.first();
    }

    //qDebug() << "TProtocol::fGetOneSendTo" << SendTo;

    if(Src)
    {
        mSendToList.removeFirst();

        SendTo.fCopy(Src);

        delete Src;

        return OK;
    }
    else
    {
        return ERR;
    }
}

void TProtocol::fSendSignal(TConnection *Conn, int NodeId, double Data, quint64 Time)
{
    Q_UNUSED(Conn);

    //qDebug() << "TProtocol::fSendSignal" << NodeId << mGraphicsMap.count(NodeId);
    emit mValueCome(mServiceId, NodeId, Data, Time);
}
