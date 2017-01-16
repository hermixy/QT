#include "Syznkg.h"

#include "../../Math/Math.h"

#include <QDateTime>
#include <QDebug>

TSyznkg::TSyznkg() : TProtocol()
{
}

int TSyznkg::fAnaly(TConnection *Conn)
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

    int SID = 0;
    int Sum = 0;
    QDateTime DateTimeNow;

    int Year = 0;
    int Mon = 0;
    int Day = 0;
    int Hour = 0;
    int Min = 0;
    int Sec = 0;
    int MSec = 0;

    QString Time;
    QDateTime DateTime;    

    if(!mRingbuf)
    {
        qDebug() << "mRingbuf is null";
        goto DATAERR;
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
        goto DATAERR;
    }

    qDebug() << "TSyznkg::fAnaly Head:" << QString::number(pcRecv[0], 16)
                            << QString::number(pcRecv[1], 16)
                            << QString::number(pcRecv[2], 16)
                            << QString::number(pcRecv[3], 16);

    if(0x68 != pcRecv[0] || pcRecv[0] != pcRecv[3])
    {
        qDebug() << "head err";
        goto DATAERR;
    }

    if(pcRecv[1] != pcRecv[2])
    {
        qDebug() << "len err";
        goto DATAERR;
    }

    iLen = pcRecv[1];

    if(iLen < 11 || iLen > 27)
    {
        qDebug() << "len value err";
        goto DATAERR;
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
        goto DATAERR;
    }

    // 结尾
    if(0x16 != pcRecv[iRead - 1])
    {
        qDebug() << "end err";
        goto DATAERR;
    }

    // 校验和
    if(pcRecv[iRead - 2] != TMath::fCheckSum(pcRecv + 4, iRead - 6))
    {
        qDebug() << "TSyznkg::fAnaly check right" << QString::number(TMath::fCheckSum(pcRecv + 4, iRead - 6), 16);

        goto DATAERR;
    }

    SID = pcRecv[12];
    Sum = pcRecv[14];

    if(Sum <= 0 || Sum > 8)
    {
        goto DATAERR;
    }

    Year = pcRecv[4] & 0xFF;
    Mon = pcRecv[5] & 0xFF;
    Day = pcRecv[6] & 0xFF;
    Hour = pcRecv[7] & 0xFF;
    Min = pcRecv[8] & 0xFF;
    Sec = pcRecv[9] & 0xFF;
    MSec = ((pcRecv[10]  << 8) & 0xFF00) | (pcRecv[11] & 0xFF);

    Year = Year < 0 ? 0 : Year;
    Year += 2000;
    Mon = (Mon < 1 || Mon > 12) ? 1 : Mon;
    Day = (Day < 1 || Day > 31) ? 1 : Day;
    Hour = (Hour < 0 || Hour > 23) ? 0 : Hour;
    Min = (Min < 0 || Min > 59) ? 0 : Min;
    Sec = (Sec < 0 || Sec > 59) ? 0 : Sec;
    MSec = (MSec < 0 || MSec > 999) ? 0 : MSec;

    Time = QString("%1-%2-%3 %4:%5:%6.%7").arg(Year, 4, 10, QChar('0'))
                                                    .arg(Mon, 2, 10, QChar('0'))
                                                    .arg(Day, 2, 10, QChar('0'))
                                                    .arg(Hour, 2, 10, QChar('0'))
                                                    .arg(Min, 2, 10, QChar('0'))
                                                    .arg(Sec, 2, 10, QChar('0'))
                                                    .arg(MSec, 3, 10, QChar('0'))
                                                    ;

    DateTime = QDateTime::fromString(Time, tr("yyyy-MM-dd HH:mm:ss.zzz"));

    for(int i = 0; i < Sum; i++)
    {
        int NodeId = ((SID << 8) & 0xFF00) | ((pcRecv[2 * i + 15]) & 0xFF);

        int Value = pcRecv[2 * i + 16] & 0x3;

        qDebug() << "NodeId:" <<NodeId << " Value:" << Value;

        if(0 == NodeId)
        {
            break;
        }        

        qDebug() << Time << DateTime.toMSecsSinceEpoch();

        qDebug() << "emit:" << NodeId << Value;
        fSendSignal(Conn, NodeId, Value, DateTime.toMSecsSinceEpoch());
    }

    // 回复
    DateTimeNow = QDateTime::currentDateTime();
    pcSend[0] = 0x68;
    pcSend[1] = 0x8;
    pcSend[2] = 0x8;
    pcSend[3] = 0x68;
    pcSend[4] = DateTimeNow.date().year() - 2000;
    pcSend[5] = DateTimeNow.date().month();
    pcSend[6] = DateTimeNow.date().day();
    pcSend[7] = DateTimeNow.time().hour();
    pcSend[8] = DateTimeNow.time().minute();
    pcSend[9] = DateTimeNow.time().second();
    pcSend[10] = (DateTimeNow.time().msec() >> 8) & 0xFF;
    pcSend[11] = DateTimeNow.time().msec() & 0xFF;
    pcSend[12] = TMath::fCheckSum(pcSend + 4, 8);
    pcSend[13] = 0x16;

    /*qDebug() << "send:";
    qDebug() << QString::number(pcSend[0], 16);
    qDebug() << QString::number(pcSend[1], 16);
    qDebug() << QString::number(pcSend[2], 16);
    qDebug() << QString::number(pcSend[3], 16);
    */

    //emit mSendData(Conn, pcSend, 14);
    Conn->fSend(pcSend, 14);

    return PROTOCOL_OK;

DATAERR:
    if(mRingbuf)
    {
        mRingbuf->fRemove(1);
    }

    return PROTOCOL_ERR;
}
