#include "Connection.h"

TConnection::TConnection(int Type)
{
    mType = Type;
    mRingbufForRead = NULL;
    mRingbufForWrite = NULL;
    mUserData = NULL;
    mState = STATE_INIT;
    mReference = 0;
}

TConnection::~TConnection()
{}

int TConnection::fStart()
{
    fSetState(STATE_RUNED);
    //emit mStart();

    return 0;
}

int TConnection::fStop()
{
    fSetState(STATE_CLOSEED);
    //emit mStop();

    return 0;
}

int TConnection::fRecv(void *Dst, int Len)
{
    Q_UNUSED(Dst);
    Q_UNUSED(Len);

    return -1;
}

int TConnection::fSend(const void *Data, int Len)
{
    Q_UNUSED(Data);
    Q_UNUSED(Len);

    return -1;
}

TRingBuf* TConnection::fCreateReadRingbuf(int Size)
{
    if(mRingbufForRead)
    {
        return mRingbufForRead;
    }

    CONN_LOCK;

    if(Size > 0)
    {
        mRingbufForRead = new TRingBuf(Size);
    }

    CONN_UNLOCK;

    return mRingbufForRead;
}

TRingBuf* TConnection::fGetReadRingbuf()
{
    return mRingbufForRead;
}

TRingBuf* TConnection::fCreateWriteRingbuf(int Size)
{
    if(mRingbufForWrite)
    {
        return mRingbufForWrite;
    }

    CONN_LOCK;

    if(Size > 0)
    {
        mRingbufForWrite = new TRingBuf(Size);
    }

    CONN_UNLOCK;

    return mRingbufForWrite;
}

TRingBuf* TConnection::fGetWriteRingbuf()
{
    return mRingbufForWrite;
}

void TConnection::fSetUserData(void *Data)
{
    CONN_LOCK;
    mUserData = Data;
    CONN_UNLOCK;
}

void* TConnection::fGetUserData()
{
    return mUserData;
}

int TConnection::fGetState()
{
    return mState;
}

void TConnection::fSetState(int State)
{
    CONN_LOCK;
    mState = State;
    CONN_UNLOCK;
}

void TConnection::fSetType(int Type)
{
    CONN_LOCK;
    mType = Type;
    CONN_UNLOCK;
}

int TConnection::fGetType()
{
    return mType;
}

void TConnection::fSetReference(int Ref)
{
    CONN_LOCK;
    mReference = Ref;
    CONN_UNLOCK;
}

void TConnection::fAddOneToRef()
{
    CONN_LOCK;
    mReference++;
    CONN_UNLOCK;
}

void TConnection::fDelOneFromRef()
{
    CONN_LOCK;
    mReference--;
    CONN_UNLOCK;
}

int TConnection::fGetReference()
{
    return mReference;
}
