#include "RingBuf.h"

#include "../../Src/Base/LibBase.h"

#include <string>
#include <QThread>
#include <QDebug>

TRingBuf::TRingBuf(int Size, /*QObject *parent, */int Type) //: QObject(parent)
{
    mReadPos = 0;
    mWritePos = 0;

    if(Size <= 0)
    {
        Size = 0;
        return;
    }

    mBuf = (unsigned char *)malloc(Size);

    if(mBuf)
    {
        mSize = Size;
    }
    else
    {
        mSize = 0;
    }

    mType = Type;
}

TRingBuf::~TRingBuf()
{
    if(mBuf)
    {
        BUF_LOCK
        free(mBuf);
        mBuf = NULL;
        BUF_UNLOCK
    }
}

void TRingBuf::RingBufClear()
{
    mReadPos = 0;
    mWritePos = 0;
}

int TRingBuf::Pos(int _iPos, int _iBufSize)
{
    return _iPos % _iBufSize;
}

// 环形buf中现存可读数据
int TRingBuf::fDataSize()
{
    return mBuf ? mWritePos - mReadPos : 0;
}

// 环形buf现可写数据
int TRingBuf::fCapacity()
{
    return mBuf ? (mSize - (mWritePos - mReadPos)) : 0;
}

void TRingBuf::WriteData(int _iSize, const void *_ptSrc)
{
    int iWpos = mWritePos % mSize;

    // 再写入数据
    if(iWpos + _iSize <= mSize)
    {
        memcpy(mBuf + iWpos, _ptSrc, _iSize);
    }
    else
    {
        int nBytes = mSize - iWpos;

        memcpy(mBuf + iWpos, _ptSrc, nBytes);
        memcpy(mBuf, (char *)_ptSrc + nBytes, _iSize - nBytes);
    }

    mWritePos += _iSize;
}

// 写数据到环形buf
int TRingBuf::fWrite(int _iSize, const void *_ptSrc)
{
    if(NULL == _ptSrc || NULL == mBuf || _iSize < 1)
    {
        return -1;
    }

    // safe check
    if(mSize < 1 || mReadPos < 0 || mWritePos < 0)
    {
        return -1;
    }

    const char *ptSrc = (const char *)_ptSrc;

    BUF_LOCK

    if(mWritePos <= mReadPos)
    {
        RingBufClear();
    }
    else if(mWritePos > 0x1FFFFFFF)
    {
        int iIndx1 = mWritePos / mSize;
        int iIndx2 = mReadPos / mSize;
        int iIndx = iIndx1 < iIndx2 ? iIndx1 : iIndx2;
        int iDec = iIndx * mSize;

        mWritePos -= iDec;
        mReadPos -= iDec;
    }

    int iDataSize = mWritePos - mReadPos;
    int iCap = mSize - iDataSize;

    if(_iSize <= iCap) // 空间够
    {
        WriteData(_iSize, ptSrc);
    }
    else if(TRingBuf::Wait == mType)// 空间不够
    {
        // 等待
        int iLeft = _iSize;

        while (iLeft > 0)
        {
            iCap = mSize - (mWritePos - mReadPos);

            if (iCap > 0)
            {
                iCap = MIN(iLeft, iCap);

                WriteData(iCap, ptSrc);

                ptSrc += iCap;
                iLeft -= iCap;
            }
            else
            {
                BUF_UNLOCK
                QThread::sleep(1);
                BUF_LOCK
            }
        }
    }
    else // 空间不够 覆盖
    {
        if (_iSize < mSize)
        {
            WriteData(_iSize, ptSrc);
            mReadPos = mWritePos - _iSize;
        }
        else
        {
            RingBufClear();
            WriteData(mSize, ptSrc + (_iSize - mSize));
        }

    }

    BUF_UNLOCK

    return _iSize;
}

void TRingBuf::ReadData(int _iSize, void *_ptDst)
{
    int iRpos = Pos(mReadPos, mSize);

    if (iRpos + _iSize <= mSize)
    {
        memcpy(_ptDst, mBuf + iRpos, _iSize);
    }
    else
    {
        int nBytes = mSize - iRpos;

        memcpy(_ptDst, mBuf + iRpos, nBytes);
        memcpy((char *)_ptDst + nBytes, mBuf, _iSize - nBytes);
    }

    mReadPos += _iSize;
}

// 从环形buf读取数据
int TRingBuf::fRead(int _iSize, void *_ptDst)
{
    if(NULL == _ptDst || NULL == mBuf || _iSize < 1)
    {
        qDebug() << "return -1";
        return -1;
    }

    // safe check
    if(mSize < 1 || mReadPos < 0 || mWritePos < 0)
    {
        qDebug() << "return -1";
        return -1;
    }

    int iDataSize = 0;
    int nBytes = _iSize;

    BUF_LOCK

    if(mReadPos >= mWritePos)
    {
        nBytes = 0;
        RingBufClear();
        goto Exit;
    }

    iDataSize = mWritePos - mReadPos;

    //qDebug() << "iDataSize" << iDataSize << "nBytes" << nBytes;
    if (iDataSize < nBytes)
    {
        nBytes = iDataSize;
    }

    //qDebug() << "nBytes" << nBytes;

    ReadData(nBytes, _ptDst);

Exit:
    BUF_UNLOCK

    return nBytes;
}

int TRingBuf::fClear()
{
    if(mBuf)
    {
        BUF_LOCK

        RingBufClear();

        BUF_UNLOCK
    }

    return 0;
}

void TRingBuf::CopyData(int _iSize, void *_ptDst)
{
    int iRpos = Pos(mReadPos, mSize);

    if (iRpos + _iSize <= mSize)
    {
        memcpy(_ptDst, mBuf + iRpos, _iSize);
    }
    else
    {
        int nBytes = mSize - iRpos;

        memcpy(_ptDst, mBuf + iRpos, nBytes);
        memcpy((char *)_ptDst + nBytes, mBuf, _iSize - nBytes);
    }

    //mReadPos += _iSize;
}

//复制环形buf数据,不删除
int TRingBuf::fCopy(int _iSize, void *_ptDst)
{
    if(NULL == _ptDst || NULL == mBuf || _iSize < 1)
    {
        return -1;
    }

    // safe check
    if(mSize < 1 || mReadPos < 0 || mWritePos < 0)
    {
        return -1;
    }

    int iDataSize = 0;
    int nBytes = _iSize;

    BUF_LOCK

    if(mReadPos >= mWritePos)
    {
        RingBufClear();
        goto Exit;
    }

    iDataSize = mWritePos - mReadPos;

    if (iDataSize < nBytes)
    {
        nBytes = iDataSize;
    }

    CopyData(nBytes, _ptDst);

Exit:
    BUF_UNLOCK

    return nBytes;
}

//删除环形buf数据
int TRingBuf::fRemove(int _iSize)
{
    if(NULL == mBuf || _iSize < 1)
    {
        return -1;
    }

    // safe check
    if(mSize < 1 || mReadPos < 0 || mWritePos < 0)
    {
        return -1;
    }

    int nBytes = _iSize;

    BUF_LOCK

    if(mReadPos >= mWritePos)
    {
        RingBufClear();
        goto Exit;
    }

    nBytes = MIN(nBytes, mWritePos - mReadPos);

    mReadPos += nBytes;

Exit:
    BUF_UNLOCK

    return nBytes;
}
