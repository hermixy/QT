#ifndef SERVICE_H
#define SERVICE_H

#ifdef DEF_RUN_AS_RUNER
#include "../Connection/Connection.h"
#include "../Server/TcpServer/TcpServer.h"
#include "../Protocol/Syreal/Syreal.h"
#include "../Protocol/Protocol.h"
#endif

#include <QString>
#include <QList>

class TNetData
{
public:
    TNetData();
    ~TNetData();

    void fCopy(const TNetData *CopyNetData);

    void fSetIp(const QString &Ip);
    QString fGetIp() const;
    void fSetPort(int Port);
    int fGetPort() const;

private:
    QString mIp;
    int mPort;
};

class TClientData : public TNetData
{
public:
    TClientData();
    ~TClientData();

    void fCopy(const TClientData *CopyClinetData);
};

class TServerData : public TNetData
{
public:
    TServerData();
    ~TServerData();

    void fCopy(const TServerData *CopyServerData);

    void fSetIoThreadSum(int Sum);
    int fGetIoThreadSum() const;
    void fSetProcessThreadSum(int Sum);
    int fGetProcessThreadSum() const;

private:
    int mIoThreadSum;
    int mProcessThreadSum;
};

class TComData
{
public:
    TComData();
    ~TComData();

    void fCopy(const TComData *CopyComData);

    void fSetName(const QString &Name);
    QString fGetName() const;
    void fSetRate(int Rate);
    int fGetRate() const;
    void fSetDataBits(int Data);
    int fGetDataBits() const;
    void fSetStopBits(int Stop);
    int fGetStopBits() const;
    void fSetCheck(int Check);
    int fGetCheck() const;

private:
    QString mName;
    int mRate;
    int mDataBits;
    int mStopBits;
    int mCheck;
};

class TIec104Data
{
public:
    TIec104Data();
    ~TIec104Data();

    void fCopy(const TIec104Data *Iec104Data);

    int mAddr;
    int mUnMaxSum;
    int mUnMaxTime;
    int mSendReason;
    int mAppAddr;
    int mMsgAddr;
    int mTimeLen;
};

class TModebusRtuNode
{
public:
    TModebusRtuNode();
    TModebusRtuNode(const TModebusRtuNode *Copy);
    TModebusRtuNode(int Host, int Fun, int StartAddr,
                    int RegCount, int DataLen, int Type);
    ~TModebusRtuNode();
    int mHost;      // 地址
    int mFun;       // 功能码
    int mStartAddr; // 通道
    int mRegCount;  // 寄存器个数
    int mDataLen;  // 数据长度 位bits
    int mType;      // 类型

    enum
    {
        TYPE_Data = 0,      // 查询数据
        TYPE_ControlData,   // 查询数据，数据用于控制时判断
        TYPE_Control        // 控制
    };
};

class TModebusRtuData
{
public:
    TModebusRtuData();
    ~TModebusRtuData();

    void fCopy(const TModebusRtuData *ModebusRtuData);

    void fAddHost(int Host, int Fun, int StartAddr,
                  int RegCount, int DataLen, int Type);

    void fDelHost(TModebusRtuNode *Node);

    void fDelHost(int Host, int Fun, int StartAddr,
                  int RegCount, int DataLen, int Type);
    void fClearHost();

    QList<TModebusRtuNode *>mList;
};

class TService
{
public:
    TService();
    TService(TService *CopyService);
    ~TService();

    void fCopy(TService *From);

    bool fIsServer();

    void fSetId(int Id);
    int fGetId()  const;
    void fSetName(const QString &Name);
    QString fGetName() const;

    enum
    {

        Port_Net_Server_Tcp=0,
        Port_Net_Server_Udp,
        Port_Net_Client_Tcp ,
        Port_Net_Client_Udp,

        Port_Com
    };

    void fSetPort(int Port);
    int fGetPort() const;

    enum
    {
        CONNECT_MODE_NORMAL,
        CONNECT_MODE_AUTO
    };

    void fSetConnectMode(int Mode);
    int fGetConnectMode() const;

    enum
    {
        RECV_MODE_NORMAL,
        RECV_MODE_AUTO
    };

    void fSetRecvMode(int Mode);
    int fGetRecvMode() const;

    enum
    {
        AgreeMent_UnKnown = 0,
        AgreeMent_Modebus_RTU,
        AgreeMent_SY_ZNKG,
        AgreeMent_Iec101 ,
        AgreeMent_Iec104,
        AgreeMent_CDT,
        AgreeMent_3761,     
        AgreeMent_SY_GZZS, 
        AgreeMent_SY_REAL,

    };

    void fSetAgreeMent(int AgreeMent);
    int fGetAgreeMent() const;

    TClientData *fGetClientData();
    TServerData *fGetServerData();
    TComData *fGetComData();
    TIec104Data *fGetIec104Data();
    TModebusRtuData *fGetModebusRtuData();

#ifdef DEF_RUN_AS_RUNER
    TProtocol* fCreateProtocol();   // 创建
    TProtocol* fGetProtocol();      // 获取
    TConnection* fCreateConnection();
    TConnection* fGetConnection();
    TTcpServer* fCreateServer();
    TTcpServer* fGetServer();
    void fDestroyProtocol();
    void fDestroyConnection();
    void fDestroyServer();
#endif

private:
    int mId;            // 服务ID
    QString mName;      // 服务名称
    int mPort;          // 通讯介质
    int mConnectMode;   // 模式：正常，自动
    int mAgreeMent;     // 协议
    int mRecvMode;      // 接收模式：正常，自动

    TClientData *mClientData;   // 网络
    TServerData *mServerData;   // 服务器
    TComData *mComData;         // 串口

    TIec104Data *mIec104Data;
    TModebusRtuData *mModebusRtuData;

    void fInit();

#ifdef DEF_RUN_AS_RUNER
    TProtocol* mProtocol;
    TConnection* mConnection;
    TTcpServer* mServer;
#endif
};

#endif // SERVICE_H
