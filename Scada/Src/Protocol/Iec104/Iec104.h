#ifndef IEC104_H
#define IEC104_H

class TIec104
{
public:
    TIec104(int _iReason, int _iAppAddr, int _iMsgAddr, int _iTime,
            int _iUnAnsSumMax, int _iUnAnsTimeMax, int _iAppAddrValue);

    ~TIec104();

    enum
    {
        UN_ANSWER_NORMAL = 0,
        UN_ANSWER_SUM = 104,
        UN_ANSWER_TIME_1 = 101,
        UN_ANSWER_TIME_2 = 102,
        UN_ANSWER_TIME_3 = 103
    };

    enum
    {
        TYPE_CALL   = 0x64,
        TYPE_INIT   = 0x46,
        TYPE_YX1    = 0x1,
        TYPE_YX3    = 0x3,
        TYPE_SOE    = 0x2,
        TYPE_YC9    = 0x9,
        TYPE_YC11   = 0xB,
        TYPE_YC13   = 0xD,
        TYPE_YC21   = 0x15,
        TYPE_YK1    = 0x2D,
        TYPE_YK2    = 0x2E,
        TYPE_TIME   = 0x67,
        TYPE_SET    = 0x70,
        TYPE_APPLY  = 0x71,
    };

    enum
    {
        Reason_Loop                 = 1,		// 周期、循环
        Reason_Scan                 = 2,		// 背景扫描
        Reason_Mutations            = 3,		// 突变信息
        Reason_Init                 = 4,		// 初始化
        Reason_Ask                  = 5,		// 请求、被请求
        Reason_Active               = 6,		// 激活-- 主站
        Reason_ActiveSure           = 7,		// 激活确认-- 子站
        Reason_StopActive           = 8,		// 停止激活
        Reason_StopActiveSure       = 9,		// 停止激活确认
        Reason_EndActive            = 10,		// 激活终止
        Reason_Back                 = 11,		// 远方命令引起的返送信息
        Reason_LocalBack            = 12,		// 当地命令引起的返送信息
        Reason_SendFile             = 13,		// 文件传输
        Reason_ResponseCall         = 20,		// 响应站总召唤
        Reason_ResponseGroupMIN     = 21,		// 响应组召唤第1组
        Reason_ResponseGroupMAX	    = 36,		// 响应组召唤第16组
        Reason_ResponseCountCall    = 37,		// 响应计数量总召唤
        Reason_ResponseCountMIN     = 38,		// 响应第1组计数量
        Reason_ResponseCountMAX     = 41,		// 响应第4组计数量
        Reason_UnknowType           = 44,		// 未知的类型标示
        Reason_UnknowSendReason     = 45,		// 未知的传送原因
        Reason_UnknowAppAddr        = 46,		// 未知的应用服务数据单元公共地址
        Reason_UnkonwMsgAddr        = 47,		// 未知的信息对象地址
    };

    void Init();
    int GetVS(const void *_Src);
    int GetVR(const void *_Src);
    int GetReason(const void *_Src);
    int GetType(const void *_Src);
    int GetChangeAble(const void *_Src);
    int GetMsgAddr(const void *_Src);
    int GetFixLen();
    int GetMsgAddrLen();
    int GetMsgStart();
    int GetMsgBefore();
    void SetVsValue(int _iVsValue);
    void SetVrValue(int _iVrValue);
    int GetVsValue();
    int GetVrValue();
    long GetNowSeconds();
    int GetAckSendState();
    int GetAckRecvState();
    void Add1ToVS();
    void Add1ToVR();
    int GetTimeLen();
    int GetUnAnsSumMax();
    int GetUnAnsTimeMax();
    void SetAckSendValue(int _iAckValue);
    void SetAckRecvValue(int _iAckValue);
    void UpdateUnAnsRecvLast();
    void SetHead(void *_pcDst);
    void SetLen(void *_pcDst, int _iLen);
    void SetVS(void *_pcDst);
    void SetVR(void *_pcDst);
    void SetType(void *_pcDst, int _iType);
    void SetChangeAble(void *_pcDst, int _iSQ, int _iSum);
    void SetReason(void *_pcDst, int _iReason);
    void SetAppAddr(void *_pcDst);
    void SetMsgAddr(void *_pcDst, int _iMsgAddrValue);

private:
    int m_iReason;
    int m_iAppAddr;
    int m_iMsgAddr;
    int m_iTime;
    int m_iVS;
    int m_iVR;
    int m_iACKSend;         // 发送的，已经确认的帧号
    int m_iACKRecv;         // 接受的，已经确认的帧号
    long m_lUnAnsSendLast;	// 最后一次发送时间
    long m_lUnAnsRecvLast;	// 最后一次接收时间
    int m_iUnAnsSumMax;
    int m_iUnAnsTimeMaxT1;		// 发送帧后，t1时间内未被确认，关闭
    int m_iUnAnsTimeMaxT2;		// 接收帧后，t2时间内进行确认
    int m_iUnAnsTimeMaxT3;		// 最后一次接受帧，t3后，发送Utest
    long m_lUTestLast;		// 上一次发送Utest的时间，避免在对方回复前，多次发送
    int m_iBaseReason;
    int m_iBaseAppAddr;
    int m_iBaseMsgAddr;
    int m_iAppAddrValue;
    int m_iMsgStart;
};

#endif // IEC104_H
