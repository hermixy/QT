#include "Modbus.h"

#include "../../Math/Math.h"

#include <QThread>
#include <QDateTime>
#include <QDebug>

TGong::TGong(const TModebusRtuNode *Node, int NodeId)
{
    mControlNode = new TModebusRtuNode(Node);

    mRun = false;
    mNodeId = NodeId;
    mDataValue = 0;
}

void TGong::fSetRun(bool Run)
{
    mRun = Run;
}

bool TGong::fGetRun()
{
    return mRun;
}

void TGong::fSetControlNode(const TModebusRtuNode *Node)
{
    if(mControlNode)
    {
        delete mControlNode;
    }

    mControlNode = new TModebusRtuNode(Node);
}

TModebusRtuNode* TGong::fGetControlNode()
{
    return mControlNode;
}

void TGong::fSetDataNodeId(int NodeId)
{
    mNodeId = NodeId;
}

int TGong::fGetDataNodeId()
{
    return mNodeId;
}

void TGong::fSetDataValue(double Value)
{
    mDataValue = Value;
}

double TGong::fGetDataValue()
{
    return mDataValue;
}

TModbus::TModbus(TModebusRtuData *ModebusRtuData) : TProtocol()
{
    mIndexScan = -1;
    mLastScanTime = 0;

    mRtuData = ModebusRtuData;

    if(mRtuData)
    {
        foreach(TModebusRtuNode *Node, mRtuData->mList)
        {
            switch(Node->mType)
            {
            case TModebusRtuNode::TYPE_Data:
                break;

            case TModebusRtuNode::TYPE_ControlData:
                mQianGong.fSetDataNodeId(Node->mStartAddr);
                mHouGong.fSetDataNodeId(Node->mStartAddr + 2);

                mControlData = new TModebusRtuNode(Node);

                mRtuData->fDelHost(Node);
                break;

            case TModebusRtuNode::TYPE_Control:
                mQianGong.fSetControlNode(Node);
                Node->mStartAddr += 2;
                mHouGong.fSetControlNode(Node);

                mRtuData->fDelHost(Node);
                break;

            default:
                break;
            }
        }
    }
}

TModbus::~TModbus()
{
    mIndexScan = -1;
    mLastScanTime = 0;    

    delete mRtuData;
}

int TModbus::fSendRead(TConnection *Conn, const TModebusRtuNode *Node)
{
    unsigned char pcSend[16];

    //qDebug() << "TModbus::fSend" << QThread::currentThread();

    pcSend[0] = Node->mHost;
    pcSend[1] = Node->mFun;
    pcSend[2] = (Node->mStartAddr >> 8) & 0xFF;
    pcSend[3] = Node->mStartAddr & 0xFF;
    pcSend[4] = (Node->mRegCount >> 8) & 0xFF;
    pcSend[5] = Node->mRegCount & 0xFF;

    int Crc = TMath::fCrc16(pcSend, 6);
    pcSend[6] = (Crc >> 8) & 0xFF;
    pcSend[7] = Crc & 0xFF;

    if(Conn)
    {
        //int iRet = Conn->fSend(pcSend, 8);
        Conn->fSend(pcSend, 8);

        //qDebug() << "Conn Send" << iRet;
    }

    return 8;
}

int TModbus::fSendWrite(TConnection *Conn, const TModebusRtuNode *Node, int Value)
{
    unsigned char pcSend[16];

    //qDebug() << "TModbus::fSend" << QThread::currentThread() << Node->mHost;

    pcSend[0] = Node->mHost & 0xFF;
    pcSend[1] = Node->mFun & 0xFF;
    pcSend[2] = (Node->mStartAddr >> 8) & 0xFF;
    pcSend[3] = Node->mStartAddr & 0xFF;

    int Count = 4;

    if(Node->mRegCount > 0)
    {
        pcSend[Count++] = (Node->mRegCount >> 8) & 0xFF;
        pcSend[Count++] = Node->mRegCount & 0xFF;
    }

    int Len = Node->mDataLen / 8;

    for(int i = 0; i < Len; i++)
    {
        pcSend[Count++] = (Value >> (Len - i - 1) * 8) & 0xFF;
    }

    int Crc = TMath::fCrc16(pcSend, Count);
    pcSend[Count++] = (Crc >> 8) & 0xFF;
    pcSend[Count++] = Crc & 0xFF;

    if(Conn)
    {
        Conn->fSend(pcSend, Count);
    }

    return Count;
}

int TModbus::fRecv(TConnection *Conn, int LenMax, int BlockTimeMaxMS)
{
    if(NULL == Conn)
    {
        return -1;
    }

    int WaitTime = 0; // ms
    int BaundRate = (int)Conn->fGetUserData();

    if(BaundRate)
    {
        WaitTime = 3.5 * 11 * 1000 * 1000 / BaundRate;
    }
    else
    {
        WaitTime = 2000;
    }

    WaitTime /= 100;

    //qDebug() << "TModbus::fRecv WaitTime" << WaitTime;

    int Delay = 0;      // 单条信息的间隔时间
    int BlockTime = 0;  // 接收的等待时间 ms
    int BufIndex = 0;
    const int BufLen = LenMax;
    char pcRecv[BufLen];

    while(Delay < WaitTime && BufIndex < BufLen)
    {
        BlockTime += 20;

        // 达到等待时间，跳出
        if(BlockTime > BlockTimeMaxMS)
        {
            //qDebug() << "TModbus::fRecv no data, break;";
            break;
        }

        int iRead = Conn->fRecv(pcRecv + BufIndex, BufLen);

        if(iRead > 0)
        {
            // 读到数据，延时清零
            Delay = 0;
            BufIndex += iRead;

            /*qDebug() << "Com Read:";
            for(int i = 0; i < iRead; i++)
            {
                qDebug() << QString::number(pcRecv[i], 16);
            }*/
        }
        else if(iRead <= 0 && BufIndex > 0)
        {
            // 读到数据后，BufIndex > 0 , 然后 没读到数据，累加延时
            Delay += 20;
            QThread::msleep(10);
            continue;
        }
        else
        {
            // 本次没有读到数据， 之前也没收到数据
            QThread::msleep(10);
            continue;
        }
    }

    if(BufIndex > 0)
    {
        //qDebug() << "Modebus Rtu Read" << BufIndex;

        TRingBuf *RingBuf = Conn->fGetReadRingbuf();

        if(RingBuf)
        {
            RingBuf->fClear(); // 写入之前清空
            return RingBuf->fWrite(BufIndex, pcRecv);
        }
    }

    return -1;
}

int TModbus::fSendOneRequest(TConnection *Conn, TModebusRtuNode *Node)
{
    if(NULL == Conn || NULL == Node)
    {
        return ERR;
    }

    fSendRead(Conn, Node);

    // 接收
    if(fRecv(Conn, 64, 200) > 0)
    {
        fAnalyFrame(Conn, Node);
    }
    else
    {
        //qDebug() << "未接收到数据";
    }

    return OK;
}

int TModbus::fSendRequest(TConnection *Conn)
{
    //qDebug() << mIndexScan << mHostList.length();
    if(NULL == mRtuData)
    {
        return ERR;
    }

    TModebusRtuNode *ScanData = NULL;

    if((STATE_CONTROL_LEFT == fGetState() || STATE_CONTROL_RIGHT == fGetState())
            && mControl)
    {
        mControl = false;

        // 控制模式下，获取一次位置信息，再获取一次其他信息
        //qDebug() << "TModbus::fSendRequest ControlData 1";
        ScanData = mControlData;
    }
    else
    {
        mControl = true;

        //qDebug() << "mIndexScan" << mIndexScan << mRtuData->mList.length();

        if(mIndexScan < 0 || mIndexScan >= mRtuData->mList.length())
        {
            mIndexScan = 0;

            if(STATE_CONTROL_LEFT == fGetState()
                || STATE_CONTROL_RIGHT == fGetState())
            {
                // 控制模式下，已经获取过位置信息，在这直接获取控制信息
                //qDebug() << "TModbus::fSendRequest" << mIndexScan;
                ScanData = mRtuData->mList.at(mIndexScan);

                mIndexScan++;
            }
            else
            {
                // 非控制模式下，先请求位置信息，再请求其他信息
                //qDebug() << "TModbus::fSendRequest ControlData 0";
                ScanData = mControlData;
            }
        }
        else
        {
            //qDebug() << "TModbus::fSendRequest" << mIndexScan;
            ScanData = mRtuData->mList.at(mIndexScan);

            mIndexScan++;
        }
    }

    //qDebug() << "发送一个响应" << mIndexScan;

    return fSendOneRequest(Conn, ScanData);
}

bool TModbus::fSendControl(TConnection *Conn, const TModebusRtuNode *Node, int Value)
{
    //qDebug() << "TModbus::fSendControl" << Node->mHost;

    fSendWrite(Conn, Node, Value);

    bool Ret = false;

    if(fRecv(Conn, 64, 100) > 0)
    {
        int RetHost = 0;
        int RetFun = 0;

        if(TProtocol::PROTOCOL_OK == fAnalyFrame(Conn, &RetHost, &RetFun, NULL))
        {
            if(RetHost == Node->mHost && RetFun == Node->mFun)
            {
                Ret = true;
            }
        }
    }

    //qDebug() << "TModbus::fSendControl" << Ret;

    return Ret;
}

int TModbus::fAnalyFrame(TConnection *Conn, int *Host, int *Fun, int *StartAddr)
{
    if(NULL == Conn)
    {
        return PROTOCOL_ERR;
    }

    TRingBuf *Ringbuf = Conn->fGetReadRingbuf();

    if(NULL == Ringbuf)
    {
        return PROTOCOL_ERR;
    }

    int Crc = 0;
    unsigned char pcRecv[256];
    int iRead = Ringbuf->fDataSize();

    //qDebug() << "data size:" << Ringbuf->fDataSize();
    if(iRead <= 0)
    {
        return PROTOCOL_IDLE;
    }

    iRead = Ringbuf->fRead(256, pcRecv);

    if(iRead < 5)
    {
        goto ANALY_ERR;
    }

    /*qDebug() << "fAnalyFrame:";
    for(int i = 0; i < iRead; i++)
    {
        qDebug() << QString::number(pcRecv[i], 16);
    }*/

    // 校验和
    Crc = (pcRecv[iRead - 2] << 8) | pcRecv[iRead - 1];

    if(Crc != TMath::fCrc16(pcRecv, iRead - 2))
    {
        qDebug() << "check now" << QString::number(Crc, 16);
        qDebug() << "check right" << QString::number(TMath::fCrc16(pcRecv, iRead - 2), 16);

        goto ANALY_ERR;
    }

    if(Host)
    {
        *Host = pcRecv[0];
    }

    if(Fun)
    {
        *Fun = pcRecv[1];
    }

    if(StartAddr)
    {
        *StartAddr = (pcRecv[2] << 8) | pcRecv[3];
    }

    return PROTOCOL_OK;

ANALY_ERR:

    if(Ringbuf)
    {
        Ringbuf->fClear();
    }

    return PROTOCOL_ERR;
}

int TModbus::fAnalyFrame(TConnection *Conn, const TModebusRtuNode *RtuNode)
{
    //qDebug() << "TModbus::fAnalyFrame";

    if(NULL == Conn || NULL == RtuNode)
    {
        return PROTOCOL_ERR;
    }

    TRingBuf *Ringbuf = Conn->fGetReadRingbuf();

    if(NULL == Ringbuf)
    {
        return PROTOCOL_ERR;
    }

    int Len = 0;
    int Sum = 0;
    int Crc = 0;
    unsigned char pcRecv[256];
    int iRead = Ringbuf->fDataSize();

    //qDebug() << "data size:" << Ringbuf->fDataSize();
    if(iRead <= 0)
    {
        return PROTOCOL_IDLE;
    }

    iRead = Ringbuf->fRead(256, pcRecv);

    if(iRead < 5)
    {
        goto ANALY_ERR;
    }

    /*qDebug() << "fAnalyFrame:";
    for(int i = 0; i < iRead; i++)
    {
        qDebug() << QString::number(pcRecv[i], 16);
    }*/

    // 校验和
    Crc = (pcRecv[iRead - 2] << 8) | pcRecv[iRead - 1];

    if(Crc != TMath::fCrc16(pcRecv, iRead - 2))
    {
        qDebug() << "check now" << QString::number(Crc, 16);
        qDebug() << "check right" << QString::number(TMath::fCrc16(pcRecv, iRead - 2), 16);

        goto ANALY_ERR;
    }

    Sum = pcRecv[2];

    if(Sum <= 0 || Sum > 128)
    {
        goto ANALY_ERR;
    }

    Len = RtuNode->mDataLen / 8;
    Sum /= Len;

    for(int i = 0; i < Sum; i++)
    {
        int NodeId = RtuNode->mStartAddr + 2 * i;

        int Value = TMath::fGetBytesHighFront(pcRecv, 3 + i * Len, Len);

        //qDebug() << "NodeId:" << QString::number(NodeId, 16) << " Value:" << Value;

        fSendSignal(Conn, NodeId, Value, 0);
    }

    // 发送的请求得到响应，可以开始新的请求
    //mLastScanTime = 0;

    return PROTOCOL_OK;

ANALY_ERR:

    if(Ringbuf)
    {
        Ringbuf->fRemove(1);
    }

    return PROTOCOL_ERR;
}

bool TModbus::fMoveControl(TConnection *Conn, const TModebusRtuNode *Node, bool Move, int Dir)
{
    TModebusRtuNode ControlNode(Node);

    if(fSendControl(Conn, &ControlNode, Move ? 0xFF00 : 0x0000))
    {
        ControlNode.mStartAddr += 1;
        if(fSendControl(Conn, &ControlNode, MOVE_LEFT == Dir ? 0xFF00 : 0x0000))
        {
            return true;
        }
    }

    return false;
}

bool TModbus::fIsInControl(QMap<int, double> &Map, int NodeId)
{
    return Map.count(NodeId) > 0;
}

void TModbus::fSetControlData(QMap<int, double> &Map, int NodeId, double Value)
{
    if(Map.count(NodeId) > 0)
    {
        Map[NodeId] = Value;
    }
}

double TModbus::fGetControlData(QMap<int, double> &Map, int NodeId)
{
    if(Map.count(NodeId) > 0)
    {
        return Map[NodeId];
    }

    return 0;
}

void TModbus::fSendSignal(TConnection *Conn, int NodeId, double Data, quint64 Time)
{
    if(NodeId == mQianGong.fGetDataNodeId())
    {
        mQianGong.fSetDataValue(Data);
    }
    else if(NodeId == mHouGong.fGetDataNodeId())
    {
        mHouGong.fSetDataValue(Data);
    }

    double StopRange = 0.4 * mStopCoe;  // 快拱开始等待
    double RunRange = 0.5 * StopRange;  // 慢拱开始追赶
    const double OverRange = 1;         // 完成
    int State = fGetState();

    double MoveDiff = mQianGong.fGetDataValue() - mHouGong.fGetDataValue();

    qDebug() << "qian" << mQianGong.fGetDataValue() << "hou" << mHouGong.fGetDataValue();

    if(STATE_CONTROL_LEFT == State)
    {
        if(MoveDiff < -StopRange) // 前拱快1
        {
            if(!mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_LEFT))
                {
                    qDebug() << "HouGong Run";
                    mHouGong.fSetRun(true);
                }
            }
            else if(mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), false, MOVE_LEFT))
                {
                    qDebug() << "QianGong Stop";
                    mQianGong.fSetRun(false);
                }
            }
        }
        else if(MoveDiff < -RunRange) // 前拱快0.5
        {
            if(!mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_LEFT))
                {
                    qDebug() << "HouGong Run";
                    mHouGong.fSetRun(true);
                }
            }
        }
        else if(MoveDiff > StopRange) // 后拱快1
        {
            if(!mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_LEFT))
                {
                    qDebug() << "QianGong Run";
                    mQianGong.fSetRun(true);
                }
            }
            else if(mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), false, MOVE_LEFT))
                {
                    qDebug() << "HouGong Stop";
                    mHouGong.fSetRun(false);
                }
            }
        }
        else if(MoveDiff > RunRange) // 后拱快0.5
        {
            if(!mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_LEFT))
                {
                    qDebug() << "QianGong Run";
                    mQianGong.fSetRun(true);
                }
            }
        }

        if(mStopPos - mQianGong.fGetDataValue() > -OverRange)
        {
            fSendTo(this, TSendTo::TYPE_StopMove, mQianGong.fGetControlNode()->mHost,
                    mQianGong.fGetControlNode()->mStartAddr, QString::number(Time));
        }
    }
    else if(STATE_CONTROL_RIGHT == fGetState())
    {
        // （右移时，数越大，越快）
        if(MoveDiff > StopRange) // 前拱快1
        {
            if(!mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_RIGHT))
                {
                    qDebug() << "HouGong Run";
                    mHouGong.fSetRun(true);
                }
            }
            else if(mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), false, MOVE_RIGHT))
                {
                    qDebug() << "QianGong Stop";
                    mQianGong.fSetRun(false);
                }
            }
        }
        else if(MoveDiff > RunRange) // 前拱快0.5
        {
            if(!mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_RIGHT))
                {
                    qDebug() << "HouGong Run";
                    mHouGong.fSetRun(true);
                }
            }
        }
        else if(MoveDiff < -StopRange) // 后拱快1
        {
            if(!mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_RIGHT))
                {
                    qDebug() << "QianGong Run";
                    mQianGong.fSetRun(true);
                }
            }
            else if(mHouGong.fGetRun())
            {
                if(fMoveControl(Conn, mHouGong.fGetControlNode(), false, MOVE_RIGHT))
                {
                    qDebug() << "HouGong Stop";
                    mHouGong.fSetRun(false);
                }
            }
        }
        else if(MoveDiff < -RunRange) // 后拱快0.5
        {
            if(!mQianGong.fGetRun())
            {
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_RIGHT))
                {
                    qDebug() << "QianGong Run";
                    mQianGong.fSetRun(true);
                }
            }
        }

        if(mQianGong.fGetDataValue() - mStopPos > -OverRange)
        {
            fSendTo(this, TSendTo::TYPE_StopMove, mQianGong.fGetControlNode()->mHost,
                    mQianGong.fGetControlNode()->mStartAddr, QString::number(Time));
        }
    }

    TProtocol::fSendSignal(Conn, NodeId, Data, Time);
}

int TModbus::fAnaly(TConnection *Conn)
{
    //qDebug() << "TModbus::fAnaly" << Conn;

    if(NULL == Conn)
    {
        return PROTOCOL_IDLE;
    }

    TSendTo SendTo;

    // 处理所有SendTo
    while(OK == fGetOneSendTo(SendTo))
    {
        switch(SendTo.fGetType())
        {
        case TSendTo::TYPE_Start:
            if(TConnection::STATE_INIT == Conn->fGetState()
                    || TConnection::STATE_CLOSEED  == Conn->fGetState())
            {
                Conn->fSetState(TConnection::STATE_RUNING);
            }
            break;

        case TSendTo::TYPE_Stop:
            if(TConnection::STATE_RUNING == Conn->fGetState()
                    || TConnection::STATE_RUNED == Conn->fGetState())
            {
                Conn->fSetState(TConnection::STATE_CLOSEING);
            }
            break;

        case TSendTo::TYPE_Left:
            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                qDebug() << "TSendTo::TYPE_Left";

                // 前
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_LEFT))
                {
                    // 后
                    if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_LEFT))
                    {
                        /*mStopMap.clear();

                        QMap<int, double>::iterator it;
                        int i = 0;
                        for(it = mControlMap.begin(); it != mControlMap.end(); ++it)
                        {
                            qDebug() << tr("TSendTo::TYPE_Left%1:").arg(++i) << it.value() << SendTo.fGetText().toDouble() * SendTo.fGetValueFloat();
                            mStopMap[it.key()] = it.value() - SendTo.fGetText().toDouble() * SendTo.fGetValueFloat();
                        }*/

                        mStopCoe = SendTo.fGetValueFloat();

                        mStopPos = mQianGong.fGetDataValue() - SendTo.fGetText().toDouble() * mStopCoe;

                        mQianGong.fSetRun(true);
                        mHouGong.fSetRun(true);

                        mControl = true;
                        fSetState(STATE_CONTROL_LEFT);
                    }
                    else
                    {
                        // 启动 后拱 失败，关闭 前拱
                        fMoveControl(Conn, mQianGong.fGetControlNode(), false, MOVE_LEFT);
                    }
                }
            }
            break;

        case TSendTo::TYPE_Right:
            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                // 前
                if(fMoveControl(Conn, mQianGong.fGetControlNode(), true, MOVE_RIGHT))
                {
                    // 后
                    if(fMoveControl(Conn, mHouGong.fGetControlNode(), true, MOVE_RIGHT))
                    {
                        mStopCoe = SendTo.fGetValueFloat();

                        mStopPos = mQianGong.fGetDataValue() + SendTo.fGetText().toDouble() * mStopCoe;

                        mQianGong.fSetRun(true);
                        mHouGong.fSetRun(true);

                        mControl = true;
                        fSetState(STATE_CONTROL_RIGHT);
                    }
                    else
                    {
                        // 启动 后拱 失败，关闭 前拱
                        fMoveControl(Conn, mQianGong.fGetControlNode(), false, MOVE_RIGHT);
                    }
                }
            }
            break;

        case TSendTo::TYPE_StopMove:
            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                // 广播
                /*TModebusRtuNode Node;

                Node.mHost = 0;
                Node.mFun = 0x12;
                Node.mStartAddr = 0x9110;
                Node.mDataLen = 16;
                fSendControl(Conn, &Node, 0x0000);
                */

                int i = 3;
                // 停前拱
                while(!fMoveControl(Conn, mQianGong.fGetControlNode(), false, MOVE_RIGHT))
                {
                    if(i-- <= 0)
                    {
                        break;
                    }

                    QThread::msleep(10);
                }

                // 停后拱
                i = 3;
                while(!fMoveControl(Conn, mHouGong.fGetControlNode(), false, MOVE_RIGHT))
                {
                    if(i-- <= 0)
                    {
                        break;
                    }

                    QThread::msleep(10);
                }

                fSetState(STATE_QUERY);
            }
            break;

        case TSendTo::TYPE_MESSAGE_INPUT:
            if(TConnection::STATE_RUNED == Conn->fGetState())
            {
                TModebusRtuNode Node;
                Node.mHost = (int)SendTo.fGetValueFloat();
                Node.mFun = 0x10;
                Node.mStartAddr = SendTo.fGetValueInt();
                Node.mRegCount = 2;
                Node.mDataLen = 32;
                fSendControl(Conn, &Node, 0x0);
            }
            break;

        default:
            break;
        }
    }

#if false
    return PROTOCOL_OK;
#endif

    //qDebug() << "mLastScanTime" << mLastScanTime << "TMath::fGetCurrentTime()" << TMath::fGetCurrentTime();
    if(TMath::fGetCurrentTime() - mLastScanTime >= 100 || mLastScanTime <= 0)
    {
        fSendRequest(Conn);

        mLastScanTime = TMath::fGetCurrentTime();
    }
    else
    {
        //qDebug() << "QThread::msleep(10)";

        QThread::msleep(10);
    }

    return PROTOCOL_OK;
}
