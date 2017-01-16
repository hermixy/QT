#ifndef RING_BUF_H
#define RING_BUF_H

//#include <QObject>

#define RING_BUF_LOCK_ENABLE

#ifdef RING_BUF_LOCK_ENABLE
#include <QMutex>
// 选择加锁，则对Buf Pos的操作都要加锁
#define BUF_LOCK mLock.lock();
#define BUF_UNLOCK mLock.unlock();
#else
#define BUF_LOCK
#define BUF_UNLOCK
#endif

class TRingBuf
{
public:    
    TRingBuf(int Size, int Type = TRingBuf::Cover);
    ~TRingBuf();

    // 环形buf中现存可读数据
    int fDataSize();

    // 环形buf现可写数据
    int fCapacity();

    // 写数据到环形buf
    int fWrite(int _iSize, const void *_ptSrc);

    // 从环形buf读取数据
    int fRead(int _iSize, void *_ptDst);

    int fClear();

    //复制环形buf数据,不删除
    int fCopy(int _iSize, void *_ptDst);

    //删除环形buf数据
    int fRemove(int _iSize);

private:
    unsigned char *mBuf;

    int mType;
    enum type
    {
        Cover,
        Wait
    };

    int mSize;
    int mReadPos;
    int mWritePos;
#ifdef RING_BUF_LOCK_ENABLE
    QMutex mLock;
#endif
    void RingBufClear();
    void WriteData(int _iSize, const void *_ptSrc);
    int Pos(int _iPos, int _iBufSize);
    void ReadData(int _iSize, void *_ptDst);
    void CopyData(int _iSize, void *_ptDst);
};

#endif // RING_BUF_H
