#include "Iec104Service.h"

#include "../../Math/Math.h"

#include <QThread>
#include <QDateTime>
#include <QDebug>

TIec104Service::TIec104Service(TIec104Data *Iec104Data) : TProtocol()
{
    if(Iec104Data)
    {
        mIec104 = new TIec104(Iec104Data->mSendReason,
                              Iec104Data->mAppAddr,
                              Iec104Data->mMsgAddr,
                              Iec104Data->mTimeLen,
                              Iec104Data->mUnMaxSum,
                              Iec104Data->mUnMaxTime,
                              Iec104Data->mAddr);
    }
    else
    {
        mIec104 = new TIec104(2, 2, 3, 7, 20, 30, 2);
    }
}

void TIec104Service::fSendData(TConnection *Conn, const char *_pcSend, int _iLen)
{
    qDebug() << "TIec104Service::fSendData";

    // 线程中的发送都通过发送信号完成
    //emit mSendData(Conn, _pcSend, _iLen);

    if(Conn)
    {
        Conn->fSend(_pcSend, _iLen);
    }
}

void TIec104Service::fSendYK(TConnection *Conn, int _Addr, int _YKType, int _YKValue)
{
    qDebug() << "TIec104Service::fSendYK";

    char pcSend[64];

    mIec104->SetHead(pcSend);

    mIec104->SetType(pcSend, _YKType);
    mIec104->SetChangeAble(pcSend, 0, 1);

    if (0xFF == _YKValue)
    {
        mIec104->SetReason(pcSend, 0x8);
    }
    else
    {
        mIec104->SetReason(pcSend, 0x6);
    }

    mIec104->SetAppAddr(pcSend);
    mIec104->SetMsgAddr(pcSend, _Addr);

    int Count = mIec104->GetMsgStart();

    pcSend[Count] = _YKValue;

    Count++;

    mIec104->SetLen(pcSend, Count - 2);

    mIec104->SetVS(pcSend);
    mIec104->SetVR(pcSend);
    // 发送序号增长
    mIec104->Add1ToVS();

    /*// 从脚本中调用的发送，直接使用Connection进行发送，不能发送信号由线程中发送，否则会死锁
    if(mConnection)
    {
        mConnection->fSend(pcSend, Count);
    }*/
    fSendData(Conn, pcSend, Count);
}

void TIec104Service::fSendRepleS(TConnection *Conn, int _VrLocal)
{
    char pcSend[6];

    pcSend[0] = 0x68;
    pcSend[1] = 0x4;
    pcSend[2] = 0x1;
    pcSend[3] = 0;
    pcSend[4] =  (_VrLocal & 0xFF) << 1;
    pcSend[5] = (_VrLocal >> 8) & 0xFF;

    fSendData(Conn, pcSend, 6);
}

void TIec104Service::fSendRepleU(TConnection *Conn, int _Control)
{
    char pcSend[6];

    pcSend[0] = 0x68;
    pcSend[1] = 0x4;
    pcSend[2] = _Control;
    pcSend[3] = 0;
    pcSend[4] = 0;
    pcSend[5] = 0;

    fSendData(Conn, pcSend, 6);
}

void TIec104Service::fSendState(TConnection *Conn, int _Type, int _Reason, int _QOI)
{
    char pcSend[256];

    mIec104->SetHead(pcSend);

    mIec104->SetType(pcSend, _Type);
    mIec104->SetChangeAble(pcSend, 0, 1);
    mIec104->SetReason(pcSend, _Reason);
    mIec104->SetAppAddr(pcSend);
    mIec104->SetMsgAddr(pcSend, 0);

    int Count = mIec104->GetMsgStart();

    pcSend[Count] = _QOI;

    Count++;

    mIec104->SetLen(pcSend, Count - 2);

    mIec104->SetVS(pcSend);
    mIec104->SetVR(pcSend);
    // 发送序号增长
    mIec104->Add1ToVS();

    fSendData(Conn, pcSend, Count);
}

void TIec104Service::fAnalyData(TConnection *Conn, const unsigned char *_pcRecv, int _iLen)
{
    Q_UNUSED(Conn);
    Q_UNUSED(_iLen);

    int ValueLen = 0;   // 每个数据的长度
    int ValueType = 0;  // 类型
    int QualityLen = 0; // 品质描述长度

    int SQ = TMath::fGetBits(mIec104->GetChangeAble(_pcRecv), 0, 1);
    int Sum = TMath::fGetBits(mIec104->GetChangeAble(_pcRecv), 1, 7);

    int Addr = mIec104->GetMsgAddr(_pcRecv);
    int MsgStart = mIec104->GetMsgStart();
    int ValueChange = 0;

    switch (mIec104->GetType(_pcRecv))
    {
    case 1:
        qDebug() << "104 单位遥信";
        ValueType = 1;// YX 单点
        ValueLen = 1;
        break;

    case 3:
        qDebug() << "104 双位遥信";
        ValueType = 1;// YX 双点
        ValueLen = 1;
        ValueChange -= 1; // 转化为单点
        break;

    case 0x9:
        qDebug() << "104 品质归一化遥测";
        ValueType = 1;// YC int
        ValueLen = 2;
        QualityLen = 1;
        break;

    case 0xB:
        qDebug() << "104 表度化遥测";
        ValueType = 1;// YC int
        ValueLen = 2;
        QualityLen = 1;
        break;

    case 0xD:
        qDebug() << "104 短浮点遥测";
        ValueType = 2;// YC float
        ValueLen = 4;
        QualityLen = 1;
        break;

    case 0x15:
        qDebug() << "104 归一化遥测";
        ValueType = 2;// YC float
        ValueLen = 2;
        break;

    case 0x31:
    case 0x71:
        ValueType = 2;// YS
        ValueLen = 4;
        break;

    default:
        return;
    }

    if (1 == SQ)
    {
        for (int i = 0; i < Sum; i++)
        {
            int IntValue = TMath::fGetBytes(_pcRecv, (ValueLen + QualityLen) * i + MsgStart, ValueLen) + ValueChange;

            double Value = 0;

            if(1 == ValueType)
            {
                Value = IntValue;
            }
            else
            {
                Value = TMath::fIntToFloat(IntValue);
            }

            //qDebug() << "1fSendSignal" << Addr + i << Value;

            fSendSignal(Conn, Addr + i, Value, 0);
        }
    }
    else
    {
        int MsgAddrLen = mIec104->GetMsgAddrLen();
        MsgStart -= MsgAddrLen;

        for (int i = 0; i < Sum; i++)
        {
            int IntValue = TMath::fGetBytes(_pcRecv, i * (MsgAddrLen + ValueLen + QualityLen) + MsgStart + MsgAddrLen, ValueLen) + ValueChange;

            double Value = 0;
            if(1 == ValueType)
            {
                Value = IntValue;
            }
            else
            {
                Value = TMath::fIntToFloat(IntValue);
            }

            //qDebug() << "0fSendSignal" << TMath::fGetBytes(_pcRecv, i * (MsgAddrLen + ValueLen + QualityLen) + MsgStart, MsgAddrLen) << Value;

            fSendSignal(Conn,
                        TMath::fGetBytes(_pcRecv, i * (MsgAddrLen + ValueLen + QualityLen) + MsgStart, MsgAddrLen),
                        Value,
                        0);
        }
    }
}

int TIec104Service::fRecv(TConnection *Conn, int WaitTimeMS)
{
    if(NULL == Conn)
    {
        return -1;
    }

    char pcRecv[256];

    do
    {
        int iRead = Conn->fRecv(pcRecv, 256);

        if(iRead > 0)
        {
            qDebug() << "Socket read " << iRead;
            TRingBuf *RingBuf = Conn->fGetReadRingbuf();

            if(RingBuf)
            {
                RingBuf->fWrite(iRead, pcRecv);
            }
        }
        else if(iRead < 0)
        {
            qDebug() << "socket disconnect";
            Conn->fSetState(TConnection::STATE_CLOSEING);
        }
        else
        {
            QThread::msleep(100);
        }

    }while((WaitTimeMS -= 100) > 0);

    return 0;
}

int TIec104Service::fAnaly(TConnection *Conn)
{
    if(NULL == Conn)
    {
        return PROTOCOL_IDLE;
    }

    unsigned char pcRecv[256];
    int iRead = 0;
    int iLen = 0;
    int iVr = 0;
    int iVs = 0;
    int iVrLocal = 0;
    int iVsLocal = 0;

    TRingBuf *mRingbuf = Conn->fGetReadRingbuf();

    //qDebug() << "104 mIec104->GetAckSendState()" << mIec104->GetAckSendState()
     //        << "mIec104->GetAckRecvState()" << mIec104->GetAckRecvState();

    switch(mIec104->GetAckSendState())
    {
    case TIec104::UN_ANSWER_TIME_1:
        qDebug() << "发送的帧未被确认，关闭连接";
        mIec104->Init();
        Conn->fStop();
        break;

    default:
        break;
    }

    switch(mIec104->GetAckRecvState())
    {
    case TIec104::UN_ANSWER_NORMAL:
        break;

    case TIec104::UN_ANSWER_SUM:
    case TIec104::UN_ANSWER_TIME_2:
        qDebug() << "104 S帧确认" << mIec104->GetVrValue();
        fSendRepleS(Conn, mIec104->GetVrValue());
        mIec104->SetAckRecvValue(mIec104->GetVrValue());
        break;

    case TIec104::UN_ANSWER_TIME_3:
        qDebug() << "104 发送U帧";
        mIec104->UpdateUnAnsRecvLast();
        fSendRepleU(Conn, 0x7);
        break;

    default:
        qDebug() << "未知的 GetAckRecvState";
        break;
    }

    if(STATE_START == fGetState())
    {
        fSetState(STATE_WAIT_ACTIVE);
        fSendRepleU(Conn, 0x7);
    }
    else if(STATE_ACTIVE == fGetState())
    {
        fSetState(STATE_NORMAL);
        fSendState(Conn, 0x64, 6, 0x14);
    }

    fRecv(Conn, 200);

    if(!mRingbuf)
    {
        qDebug() << "mRingbuf is null";
        goto ANALY_ERR;
    }

    if(mRingbuf->fDataSize() < 6)
    {
        //qDebug() << "fAnalySocketData size < 6";
        return PROTOCOL_IDLE;
    }

    iRead = mRingbuf->fCopy(2, pcRecv);

    if(2 != iRead)
    {
        qDebug() << "read head err";
        goto ANALY_ERR;
    }

    iLen  = pcRecv[1];

    /*qDebug() << "Head:" << QString::number(pcRecv[0], 16)
                            << QString::number(pcRecv[1], 16)
                            << QString::number(pcRecv[2], 16)
                            << QString::number(pcRecv[3], 16);
    */

    if(0x68 != pcRecv[0] || iLen > 253 || iLen < 4)
    {
        qDebug() << "analy head err";
        goto ANALY_ERR;
    }

    iLen += 2;

    if(mRingbuf->fDataSize() < iLen)
    {
        return PROTOCOL_IDLE;
    }

    iRead = mRingbuf->fRead(iLen, pcRecv);

    if(iRead != iLen)
    {
        qDebug() << "read frame err";
        goto ANALY_ERR;
    }

    // 开始解析
    if(6 == iLen) // 定长帧
    {
        switch(TMath::fGetBits(pcRecv[2], 6, 2))
        {
        case 3: // 末2位均为1，判断为U格式
        {
            int Control = 0;

            // 01234567
            switch(pcRecv[2])
            {
                case 0x7:
                    Control = 0xB;
                    qDebug() << "104 U帧 启动生效";
                    break;

                case 0xB:
                    Control = 0;
                    qDebug() << "104 U帧 启动确认";
                    fSetState(STATE_ACTIVE);
                    break;

                case 0x13:
                    Control = 0x23;
                    qDebug() << "104 U帧 停止生效";
                    break;

                case 0x23:
                    Control = 0;
                    qDebug() << "104 U帧 停止确认";
                    break;

                case 0x43:
                    Control = 0x83;
                    qDebug() << "104 U帧 测试生效";
                    break;

                case 0x83:
                    Control = 0;
                    qDebug() << "104 U帧 测试生效";
                    break;

                default:
                    qDebug() << "104 U格式未知的控制域";
                    break;
            }

            // 更新最后一次接收的时间
            mIec104->UpdateUnAnsRecvLast();

            if(Control > 0)
            {
                fSendRepleU(Conn, Control);
            }
        }
        break;

        case 1: // S帧
            iVr = mIec104->GetVR(pcRecv);
            qDebug() << "104 S帧确认个数：" << iVr;
            mIec104->SetAckSendValue(iVr);
            break;

        default:
            qDebug() << "未知的定长帧";
            break;
        }
    }
    else // 变长帧
    {
        iVr = mIec104->GetVR(pcRecv);
        iVs = mIec104->GetVS(pcRecv);
        iVrLocal = mIec104->GetVrValue();
        iVsLocal = mIec104->GetVsValue();

        qDebug() <<"帧序号" << iVrLocal << iVsLocal << iVs << iVr;

        // 判断序号是否正确
        if (
            // 正常
            (iVs == iVrLocal && iVr == iVsLocal)

            // 容错
            || (iVs - 1 == iVrLocal && iVr == iVsLocal - 1)
            || (iVs - 1 == iVrLocal && iVr == iVsLocal)
            || (iVs == iVrLocal && iVr == iVsLocal - 1)
            )
        {
            mIec104->Add1ToVR();
            mIec104->SetAckSendValue(iVr);
        }
        else
        {
            fSendRepleU(Conn, 0x13);
            mIec104->Init();
            qDebug() << "帧序号错误";
            goto RETURN_ERR;
        }

        int iType = mIec104->GetType(pcRecv);

        //qDebug() << "Type :" << iType;

        switch(iType)
        {
        case TIec104::TYPE_CALL:
            switch(mIec104->GetReason(pcRecv))
            {
            case TIec104::Reason_ActiveSure:
                qDebug() << "104 总召确认";
                break;

            case TIec104::Reason_EndActive:
                qDebug() << "104 总召结束";
                break;

            default:
                qDebug() << "104 未知的总召传送原因";
                break;
            }
            break;

        case TIec104::TYPE_INIT:
            qDebug() << "104 初始化结束";
            break;

        case TIec104::TYPE_YX1:
        case TIec104::TYPE_YX3:
        case TIec104::TYPE_YC9:
        case TIec104::TYPE_YC11:
        case TIec104::TYPE_YC13:
        case TIec104::TYPE_YC21:
        case TIec104::TYPE_YK1:
        case TIec104::TYPE_YK2:
            fAnalyData(Conn, pcRecv, iLen);
            break;

        case TIec104::TYPE_SOE:
            qDebug() << "104 SOE";
            break;

        case TIec104::TYPE_TIME:
            qDebug() << "104 校时完成";
            break;

        case TIec104::TYPE_SET:
            qDebug() << "104 设置完成";
            break;

        case TIec104::TYPE_APPLY:
            qDebug() << "104 应用完成";
            break;

        default:
            qDebug() << "104 未知的帧类型：" + QString::number(iType);
            break;
        }
    }

    return PROTOCOL_OK;

ANALY_ERR:
    if(mRingbuf)
    {
        mRingbuf->fRemove(1);
    }

RETURN_ERR:

    return PROTOCOL_ERR;
}
