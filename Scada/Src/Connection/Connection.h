#ifndef CONNECTION_H
#define CONNECTION_H

#include "../RingBuf/RingBuf.h"
#include "../Base/LibBase.h"

#include <QObject>

#define CONNECTION_LOCK_ENABLE

#ifdef CONNECTION_LOCK_ENABLE
#include <QMutex>
// 选择加锁
#define CONN_LOCK mLock.lock();
#define CONN_UNLOCK mLock.unlock();
#else
#define CONN_LOCK
#define CONN_UNLOCK
#endif

class TConnection : public QObject
{
    Q_OBJECT

public:
    TConnection(int Type = TCON_UNKNOWN);
    virtual ~TConnection();

    enum
    {
        TCON_UNKNOWN,
        TCON_CLIENT_TCP,
        TCON_SERVER_TCP,
        TCON_SOCKET_TCP,
        TCON_CLIENT_UDP,
        TCON_SERVER_UDP,
        TCON_SOCKET_UDP,
        TCON_COM,
        TCON_MAX
    };

    void fSetType(int Type);
    int fGetType();

    void fSetUserData(void *Data);
    void* fGetUserData();
    void fSetState(int State);
    int fGetState();
    void fSetReference(int Ref);
    void fAddOneToRef();
    void fDelOneFromRef();
    int fGetReference();

    virtual int fStart();
    virtual int fStop();
    virtual int fRecv(void *Dst, int Len);
    virtual int fSend(const void *Data, int Len);

    TRingBuf* fCreateReadRingbuf(int Size);
    TRingBuf* fGetReadRingbuf();
    TRingBuf* fCreateWriteRingbuf(int Size);
    TRingBuf* fGetWriteRingbuf();

    enum
    {
        STATE_UNKNOWN,
        STATE_INIT,
        STATE_RUNING,   // 准备运行
        STATE_RUNED,    // 已经运行
        STATE_CLOSEING, // 准备关闭
        STATE_CLOSEED,  // 已经关闭
        STATE_DESTROY,
        STATE_MAX
    };

protected:
    TRingBuf *mRingbufForRead;
    TRingBuf *mRingbufForWrite;

    QMutex mLock;

private:
    int mType;
    void* mUserData;
    int mState;
    int mReference;
};

#endif // CONNECTION_H
