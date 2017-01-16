#ifndef MODBUS_H
#define MODBUS_H

#include "../../Service/Service.h"
#include "../Protocol.h"

class TGong
{
public:
    TGong(const TModebusRtuNode *Node = NULL, int NodeId = 0);

    void fSetRun(bool Run);
    bool fGetRun();

    void fSetControlNode(const TModebusRtuNode *Node);
    TModebusRtuNode* fGetControlNode();

    void fSetDataNodeId(int NodeId);
    int fGetDataNodeId();

    void fSetDataValue(double Value);
    double fGetDataValue();

private:
    bool mRun;

    TModebusRtuNode *mControlNode;

    int mNodeId;
    double mDataValue;
};

class TModbus : public TProtocol
{
    Q_OBJECT

public:
    TModbus(TModebusRtuData *ModebusRtuData);
    ~TModbus();

    enum
    {
        STATE_QUERY = STATE_PARENT,     // 遍历模式
        STATE_CONTROL_LEFT,             // 控制模式
        STATE_CONTROL_RIGHT             // 控制模式
    };

    int fAnaly(TConnection *Conn);

    int fRecv(TConnection *Conn, int LenMax, int BlockTimeMaxMS);
    int fSendRead(TConnection *Conn, const TModebusRtuNode *Node);

    int fSendWrite(TConnection *Conn, const TModebusRtuNode *Node, int Value);

private:
    TGong mQianGong;    // 前拱
    TGong mHouGong;     // 后拱
    bool mControl;      // 查询控制的位置信息
    double mStopCoe;    // 控制时的系数
    double mStopPos;    // 停止位置
    TModebusRtuNode *mControlData;  // 获取钻机的位置信息

    int mIndexScan;
    qint64 mLastScanTime;

    TModebusRtuData *mRtuData;
    //QMap<int, double>mControlMap;
    //QMap<int, double>mStopMap;
    //TModebusRtuNode *mControlData;  // 获取钻机的位置信息
    //TModebusRtuNode *mControlNode;  // 控制钻机动作

    int fSendRequest(TConnection *Conn);
    int fSendOneRequest(TConnection *Conn, TModebusRtuNode *Node);

    int fAnalyFrame(TConnection *Conn, const TModebusRtuNode *RtuNode);
    int fAnalyFrame(TConnection *Conn, int *Host, int *Fun, int *StartAddr);

    bool fSendControl(TConnection *Conn, const TModebusRtuNode *Node, int Value);

    enum
    {
        MOVE_LEFT,
        MOVE_RIGHT
    };

    bool fMoveControl(TConnection *Conn, const TModebusRtuNode *Node, bool Move, int Dir);

    void fSetControlData(QMap<int, double> &Map, int NodeId, double Value);
    double fGetControlData(QMap<int, double> &Map, int NodeId);
    bool fIsInControl(QMap<int, double> &Map, int NodeId);
    void fSendSignal(TConnection *Conn, int NodeId, double Data, quint64 Time);
};

#endif // MODBUS_H
