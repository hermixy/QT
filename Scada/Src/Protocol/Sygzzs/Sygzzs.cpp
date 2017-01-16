#include "Sygzzs.h"

#include "../../Service/Service.h"
#include "../../Math/Math.h"

#include <QDateTime>
#include <QDebug>

TSygzzs::TSygzzs() : TProtocol()
{
}

int TSygzzs::fAnaly(TConnection *Conn)
{
    if(NULL == Conn)
    {
        return PROTOCOL_IDLE;
    }

    unsigned char pcRecv[256];
    unsigned char pcSend[256];
    int iRead = 0;
    int iLen = 0;
    TRingBuf *mRingbuf = Conn->fGetReadRingbuf();

    if(!mRingbuf)
    {
        qDebug() << "mRingbuf is null";
        goto ANALY_ERR;
    }

    qDebug() << "mRingbuf->fDataSize()" << mRingbuf->fDataSize();

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

    qDebug() << "TSygzzs::fAnaly Head:" << QString::number(pcRecv[0], 16)
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

    if(0xDF != iLen)
    {
        qDebug() << "len value err";
        goto ANALY_ERR;
    }

    iLen += 6;

    if(mRingbuf->fDataSize() < iLen)
    {
        return PROTOCOL_IDLE;
    }

    iRead = mRingbuf->fRead(iLen, pcRecv);

    if(iRead != iLen)
    {
        qDebug() << "analy" << iRead << iLen;
        goto ANALY_ERR;
    }

    // 结尾
    if(0x16 != pcRecv[iRead - 1])
    {
        qDebug() << "end err";
        goto ANALY_ERR;
    }

    // 校验和
    if(pcRecv[iRead - 2] != TMath::fCheckSum(pcRecv + 4, iRead - 6))
    {
        qDebug() << "TSygzzs::fAnaly check right" << QString::number(TMath::fCheckSum(pcRecv + 4, iRead - 6), 16);

        goto ANALY_ERR;
    }        

    for(int i = 32; i < iRead - 2; i += 13)
    {
        qDebug() << "analy data :" << i;
        int NodeId = ((pcRecv[i] << 8) & 0xFF00) | ((pcRecv[i + 1]) & 0xFF);
        int Year = pcRecv[i + 2] & 0xFF;
        int Mon = pcRecv[i + 3] & 0xFF;
        int Day = pcRecv[i + 4] & 0xFF;
        int Hour = pcRecv[i + 5] & 0xFF;
        int Min = pcRecv[i + 6] & 0xFF;
        int Sec = pcRecv[i + 7] & 0xFF;
        int MSec = ((pcRecv[i + 8]  << 8) & 0xFF00) | (pcRecv[i + 9] & 0xFF);
        Year = Year < 0 ? 0 : Year;
        Year += 2000;
        Mon = (Mon < 1 || Mon > 12) ? 1 : Mon;
        Day = (Day < 1 || Day > 31) ? 1 : Day;
        Hour = (Hour < 0 || Hour > 23) ? 0 : Hour;
        Min = (Min < 0 || Min > 59) ? 0 : Min;
        Sec = (Sec < 0 || Sec > 59) ? 0 : Sec;
        MSec = (MSec < 0 || MSec > 999) ? 0 : MSec;

        int Value = pcRecv[i + 10] & 0x3;

        qDebug() << "NodeId:" <<NodeId << " Value:" << Value;

        if(0 == NodeId)
        {
            break;
        }

        QString Time = QString("%1-%2-%3 %4:%5:%6.%7").arg(Year, 4, 10, QChar('0'))
                                                        .arg(Mon, 2, 10, QChar('0'))
                                                        .arg(Day, 2, 10, QChar('0'))
                                                        .arg(Hour, 2, 10, QChar('0'))
                                                        .arg(Min, 2, 10, QChar('0'))
                                                        .arg(Sec, 2, 10, QChar('0'))
                                                        .arg(MSec, 3, 10, QChar('0'))
                                                        ;

        QDateTime DateTime = QDateTime::fromString(Time, tr("yyyy-MM-dd HH:mm:ss.zzz"));

        qDebug() << Time << DateTime.toMSecsSinceEpoch();

        qDebug() << "emit:" << NodeId << Value;
        fSendSignal(Conn, NodeId, Value, DateTime.toMSecsSinceEpoch());
    }

    // 回复
    pcSend[0] = 0x68;
    pcSend[1] = 0x77;
    pcSend[2] = 0x77;
    pcSend[3] = 0x68;
    pcSend[4] = pcRecv[5];// 通讯终端地址 4字节
    pcSend[5] = pcRecv[6];
    pcSend[6] = pcRecv[7];
    pcSend[7] = pcRecv[8];
    pcSend[8] = 0x0;
    pcSend[9] = 0x0;
    pcSend[10] = 0x0;
    pcSend[11] = 0x0;
    pcSend[12] = 0x0;
    pcSend[13] = 0x0;

    //emit mSendData(Conn, pcSend, 14);
    Conn->fSend(pcSend, 14);

    return PROTOCOL_OK;

ANALY_ERR:
    if(mRingbuf)
    {
        mRingbuf->fRemove(1);
    }

    return PROTOCOL_ERR;
}
