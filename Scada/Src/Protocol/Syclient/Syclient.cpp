#include "Syclient.h"

#include "../../Math/Math.h"

#include <QDateTime>
#include <QDebug>

TSyclient::TSyclient() : TProtocol()
{}

int TSyclient::fAnaly(TConnection *Conn)
{
    if(NULL == Conn)
    {
        return PROTOCOL_IDLE;
    }

    unsigned char pcRecv[16];
    int iRead = 0;
    int iLen = 0;
    int ServiceId;
    int NodeId;
    int Value;
    TRingBuf *mRingbuf = Conn->fGetReadRingbuf();

    if(!mRingbuf)
    {
        qDebug() << "mRingbuf is null";
        goto ANALY_ERR;
    }    

    if(mRingbuf->fDataSize() < 4)
    {
        //qDebug() << "fAnalySocketData size < 4";
        return PROTOCOL_IDLE;
    }

    iRead = mRingbuf->fCopy(4, pcRecv);

    if(4 != iRead)
    {
        qDebug() << "read head err";
        goto ANALY_ERR;
    }

    qDebug() << "Head:" << QString::number(pcRecv[0], 16)
                            << QString::number(pcRecv[1], 16)
                            << QString::number(pcRecv[2], 16)
                            << QString::number(pcRecv[3], 16);

    if(0x68 != pcRecv[0] || pcRecv[0] != pcRecv[3])
    {
        qDebug() << "head err";
        goto ANALY_ERR;
    }

    if(pcRecv[1] != pcRecv[2])
    {
        qDebug() << "len err";
        goto ANALY_ERR;
    }

    iLen = pcRecv[1];

    if(3 != iLen)
    {
        qDebug() << "len value err";
        goto ANALY_ERR;
    }

    iRead = mRingbuf->fRead(8, pcRecv);

    if(8 != iRead)
    {
        qDebug() << "len value err";
        goto ANALY_ERR;
    }

    ServiceId = pcRecv[4];
    NodeId = pcRecv[5];
    Value = pcRecv[6];

    qDebug() << "TSyclient::fAnaly emit" << ServiceId << "," << NodeId << "," << Value;

    fSendSignal(Conn, NodeId, Value, 0);

    return PROTOCOL_OK;

ANALY_ERR:
    if(mRingbuf)
    {
        mRingbuf->fRemove(1);
    }

    return PROTOCOL_ERR;
}
