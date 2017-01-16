#include "Iec104.h"

#include "../../Math/Math.h"

#include <QDateTime>
#include <QDebug>

TIec104::TIec104(int _iReason, int _iAppAddr, int _iMsgAddr, int _iTime,
                 int _iUnAnsSumMax, int _iUnAnsTimeMax, int _iAppAddrValue)
{
    m_iReason = 0;
    m_iAppAddr = 0;
    m_iMsgAddr = 0;
    m_iTime = 0;
    m_iAppAddrValue = 0;
    m_iVS = 0;
    m_iVR = 0;
    m_iACKSend = 0;
    m_iACKRecv = 0;
    m_lUnAnsSendLast = 0;
    m_lUnAnsRecvLast = 0;
    m_iUnAnsSumMax = 0;
    m_iUnAnsTimeMaxT1 = 0;
    m_iUnAnsTimeMaxT2 = 0;
    m_iUnAnsTimeMaxT3 = 0;
    m_lUTestLast = 0;
    m_iBaseReason = 0;
    m_iBaseAppAddr = 0;
    m_iBaseMsgAddr = 0;
    m_iMsgStart = 0;

    // 对各字段进行仔细判断
    if (_iReason < 1 || _iReason > 2 || _iAppAddr < 1 || _iAppAddr > 2
        || _iMsgAddr < 1 || _iMsgAddr > 3 || (2 != _iTime && 3 != _iTime && 7 != _iTime)
        || _iUnAnsSumMax <= 0 || _iUnAnsTimeMax <= 0 || _iAppAddrValue <= 0)
    {
        return;
    }

    m_iReason = _iReason;
    m_iAppAddr = _iAppAddr;
    m_iMsgAddr = _iMsgAddr;
    m_iTime = _iTime;
    m_iAppAddrValue = _iAppAddrValue;

    m_iVS = 0;
    m_iVR = 0;
    m_iACKSend = 0;
    m_iACKRecv = 0;
    m_lUnAnsSendLast = 0;
    m_lUnAnsRecvLast = 0;
    m_iUnAnsSumMax = _iUnAnsSumMax;
    m_iUnAnsTimeMaxT1 = _iUnAnsTimeMax;		// 发送后20s无回复
    m_iUnAnsTimeMaxT2 = _iUnAnsTimeMax / 2;	// 收到10s内回复
    m_iUnAnsTimeMaxT3 = _iUnAnsTimeMax * 3;	// 60s 内，无发送，无接收
    m_lUTestLast = 0;

    m_iBaseReason = 8;
    m_iBaseAppAddr = m_iBaseReason + m_iReason;
    m_iBaseMsgAddr = m_iBaseAppAddr + m_iAppAddr;

    m_iMsgStart = m_iBaseMsgAddr + m_iMsgAddr;
}

TIec104::~TIec104()
{
}

void TIec104::Init()
{
    m_iVS = 0;
    m_iVR = 0;
    m_iACKSend = 0;
    m_iACKRecv = 0;
    m_lUnAnsSendLast = 0;
    m_lUnAnsRecvLast = 0;
}

int TIec104::GetVS(const void *_Src)
{
    char *pcSrc = (char *)_Src;
    return ((pcSrc[2] >> 1) & 0x7F) | ((pcSrc[3] << 7) & 0x7F80);
}

int TIec104::GetVR(const void *_Src)
{
    char *pcSrc = (char *)_Src;
    return ((pcSrc[4] >> 1) & 0x7F) | ((pcSrc[5] << 7) & 0x7F80);
}

int TIec104::GetReason(const void *_Src)
{
    unsigned char *pcSrc = (unsigned char *)_Src;
    return TMath::fGetBytes(pcSrc, m_iBaseReason, m_iReason);
}

int TIec104::GetType(const void *_Src)
{
    char *pcSrc = (char *)_Src;
    return pcSrc[6] & 0xFF;
}

int TIec104::GetChangeAble(const void *_Src)
{
    char *pcSrc = (char *)_Src;
    return pcSrc[7] & 0xFF;
}

int TIec104::GetMsgAddr(const void *_Src)
{
    unsigned char *pcSrc = (unsigned char *)_Src;
    return TMath::fGetBytes(pcSrc, m_iBaseMsgAddr, m_iMsgAddr);
}

int TIec104::GetFixLen()
{
    return 6;
}

int TIec104::GetMsgAddrLen()
{
    return m_iMsgAddr;
}

int TIec104::GetMsgStart()
{
    return m_iMsgStart;
}

int TIec104::GetMsgBefore()
{
    return m_iBaseMsgAddr;
}

void TIec104::SetVsValue(int _iVsValue)
{
    m_iVS = _iVsValue;
}

void TIec104::SetVrValue(int _iVrValue)
{
    m_iVR = _iVrValue;
}

int TIec104::GetVsValue()
{
    return m_iVS;
}

int TIec104::GetVrValue()
{
    return m_iVR;
}

long TIec104::GetNowSeconds()
{
    return QDateTime::currentMSecsSinceEpoch() / 1000;
}

int TIec104::GetAckSendState()
{
    if (m_iVS - m_iACKSend > m_iUnAnsSumMax)
    {
        return UN_ANSWER_SUM;
    }

    if (//m_lUnAnsSendLast > 0 && // 发送过帧
            (m_iVS - m_iACKSend > 0) // 存在未确认
            && (GetNowSeconds() - m_lUnAnsSendLast > m_iUnAnsTimeMaxT1)) // > t1
    {
        return UN_ANSWER_TIME_1;
    }

    return UN_ANSWER_NORMAL;
}

int TIec104::GetAckRecvState()
{
    if (m_iVR - m_iACKRecv > m_iUnAnsSumMax)
    {
        return UN_ANSWER_SUM;
    }

    long mNow = GetNowSeconds();

    if ((m_iVR - m_iACKRecv > 0) // 表示存在未确认帧
            && (mNow - m_lUnAnsRecvLast > m_iUnAnsTimeMaxT2)) // > t2
    {
        return UN_ANSWER_TIME_2;
    }

    if (m_iVR == m_iACKRecv // 没有未确认帧
        && m_iVS == m_iACKSend // 所有帧都被确认了
        && m_lUnAnsRecvLast > 0 // 表示已经开始通讯
        && mNow - m_lUnAnsRecvLast > m_iUnAnsTimeMaxT3 // > t3
        && mNow - m_lUTestLast > m_iUnAnsTimeMaxT3 / 2)
    {
        m_lUTestLast = mNow;
        return UN_ANSWER_TIME_3;
    }

    return UN_ANSWER_NORMAL;
}

void TIec104::Add1ToVS()
{
    // 发送时，更新计时器
    m_lUnAnsSendLast = GetNowSeconds();
    m_iVS++;
}

void TIec104::Add1ToVR()
{
    // 每次接收到信息，更新计时器
    m_lUnAnsRecvLast = GetNowSeconds();

    m_iVR++;
}

int TIec104::GetTimeLen()
{
    return m_iTime;
}

int TIec104::GetUnAnsSumMax()
{
    return m_iUnAnsSumMax;
}

int TIec104::GetUnAnsTimeMax()
{
    return m_iUnAnsTimeMaxT1;
}

void TIec104::SetAckSendValue(int _iAckValue)
{
    m_iACKSend = _iAckValue;
}

void TIec104::SetAckRecvValue(int _iAckValue)
{
    m_iACKRecv = _iAckValue;
}

void TIec104::UpdateUnAnsRecvLast()
{
    m_lUnAnsRecvLast = GetNowSeconds();
}

void TIec104::SetHead(void *_pcDst)
{
    char *pcDst = (char *)_pcDst;
    pcDst[0] = 0x68;
}

void TIec104::SetLen(void *_pcDst, int _iLen)
{
    char *pcDst = (char *)_pcDst;
    pcDst[1] = _iLen;
}

void TIec104::SetVS(void *_pcDst)
{
    char *pcDst = (char *)_pcDst;
    pcDst[2] = (m_iVS << 1) & 0xFE;
    pcDst[3] = (m_iVS >> 7) & 0xFF;
}

void TIec104::SetVR(void *_pcDst)
{
    char *pcDst = (char *)_pcDst;
    pcDst[4] = (m_iVR << 1) & 0xFF;
    pcDst[5] = (m_iVR >> 7) & 0xFF;
}

void TIec104::SetType(void *_pcDst, int _iType)
{
    char *pcDst = (char *)_pcDst;
    pcDst[6] = _iType;
}

void TIec104::SetChangeAble(void *_pcDst, int _iSQ, int _iSum)
{
    char *pcDst = (char *)_pcDst;
    pcDst[7] = (((_iSQ) & 0x1) << 7) | ((_iSum) & 0x7F);
}

void TIec104::SetReason(void *_pcDst, int _iReason)
{
    char *pcDst = (char *)_pcDst;
    TMath::fSetBytes(pcDst, m_iBaseReason, m_iReason, _iReason);
}

void TIec104::SetAppAddr(void *_pcDst)
{
    char *pcDst = (char *)_pcDst;
    TMath::fSetBytes(pcDst, m_iBaseAppAddr, m_iAppAddr, m_iAppAddrValue);
}

void TIec104::SetMsgAddr(void *_pcDst, int _iMsgAddrValue)
{
    char *pcDst = (char *)_pcDst;
    TMath::fSetBytes(pcDst, m_iBaseMsgAddr, m_iMsgAddr, _iMsgAddrValue);
}
