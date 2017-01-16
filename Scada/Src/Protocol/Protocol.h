#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "../Connection/Connection.h"
#include "../RingBuf/RingBuf.h"
#include "../SendTo/SendTo.h"
#include "../Graphics/Graphics.h"

class TProtocol : public QObject
{
    Q_OBJECT

public:
    TProtocol();
    virtual ~TProtocol();

    virtual int fAnaly(TConnection *Conn) = 0;

    enum TReturnData
    {
        PROTOCOL_IDLE,
        PROTOCOL_ERR,
        PROTOCOL_OK
    };

    enum
    {
        STATE_NORMAL,           // 平常态
        STATE_START,            // 刚刚连接，发送Start激活命令
        STATE_PARENT
    };

    virtual void fSetServiceId(int ServiceId);
    virtual int fGetServiceId();
    virtual void fSetState(int State);
    virtual int fGetState();
    //virtual void fSetConnection(TConnection *Connection);
    //virtual TConnection* fGetConnection();

public slots:
    virtual void fSendTo(QObject *Sender, int Type, double ValueFloat,
                         int ValueInt, QString Text);
    virtual int fGetOneSendTo(TSendTo &SendTo);
    //virtual void fValueCome(int ServiceId, int NodeId, double Value, quint64 Time, double ValueChange);

Q_SIGNALS:
    void mValueCome(int ServiceId, int NodeId, double Value, quint64 Time);

protected:
    int mServiceId;
    int mState;

    QList<TSendTo *>mSendToList;

    virtual void fSendSignal(TConnection *Conn, int NodeId, double Data, quint64 Time);
};

#endif // PROTOCOL_H
