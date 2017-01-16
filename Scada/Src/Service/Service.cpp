#include "Service.h"

#ifdef DEF_RUN_AS_RUNER
#include "../Connection/Com/Com.h"
#include "../Connection/TcpSocket/TcpSocket.h"
#include "../Protocol/Iec104/Iec104Service.h"
#include "../Protocol/Modbus/Modbus.h"
#include "../Protocol/Syclient/Syclient.h"
#include "../Protocol/Syznkg/Syznkg.h"
#include "../Protocol/Sygzzs/Sygzzs.h"
#include "../Protocol/Protocol.h"
#endif

#include <QDebug>

TNetData::TNetData()
{
    mPort = 0;
}

void TNetData::fCopy(const TNetData *CopyNetData)
{
    if(CopyNetData)
    {
        mIp = CopyNetData->fGetIp();
        mPort = CopyNetData->fGetPort();
    }
}

void TNetData::fSetIp(const QString &Ip)
{
    mIp = Ip;
}

QString TNetData::fGetIp() const
{
    return mIp;
}

void TNetData::fSetPort(int Port)
{
    mPort = Port;
}

int TNetData::fGetPort() const
{
    return mPort;
}

TNetData::~TNetData()
{}

TClientData::TClientData() : TNetData()
{
}

void TClientData::fCopy(const TClientData *CopyClinetData)
{
    if(CopyClinetData)
    {
        TNetData::fCopy(CopyClinetData);
    }
}

TClientData::~TClientData()
{}

TServerData::TServerData() : TNetData()
{
    mIoThreadSum = 1;
    mProcessThreadSum = 1;
}

void TServerData::fCopy(const TServerData *CopyServerData)
{
    if(CopyServerData)
    {
        TNetData::fCopy(CopyServerData);

        mIoThreadSum = CopyServerData->fGetIoThreadSum();
        mProcessThreadSum = CopyServerData->fGetProcessThreadSum();
    }
}

void TServerData::fSetIoThreadSum(int Sum)
{
    mIoThreadSum = Sum;
}

int TServerData::fGetIoThreadSum() const
{
    return mIoThreadSum;
}

void TServerData::fSetProcessThreadSum(int Sum)
{
    mProcessThreadSum = Sum;
}

int TServerData::fGetProcessThreadSum() const
{
    return mProcessThreadSum;
}

TServerData::~TServerData()
{}

TComData::TComData()
{
    //mMode = 0;
    mRate = 0;
    mDataBits = 0;
    mStopBits = 0;
    mCheck = 0;
}

void TComData::fCopy(const TComData *CopyComData)
{
    if(CopyComData)
    {
        fSetName(CopyComData->fGetName());
        fSetRate(CopyComData->fGetRate());
        fSetDataBits(CopyComData->fGetDataBits());
        fSetStopBits(CopyComData->fGetStopBits());
        fSetCheck(CopyComData->fGetCheck());
    }
}

void TComData::fSetName(const QString &Name)
{
    mName = Name;
}

QString TComData::fGetName() const
{
    return mName;
}

void TComData::fSetRate(int Rate)
{
    mRate = Rate;
}

int TComData::fGetRate() const
{
    return mRate;
}

void TComData::fSetDataBits(int Data)
{
    mDataBits = Data;
}

int TComData::fGetDataBits() const
{
    return mDataBits;
}

void TComData::fSetStopBits(int Stop)
{
    mStopBits = Stop;
}

int TComData::fGetStopBits() const
{
    return mStopBits;
}

void TComData::fSetCheck(int Check)
{
    mCheck = Check;
}

int TComData::fGetCheck() const
{
    return mCheck;
}

TComData::~TComData()
{}

TIec104Data::TIec104Data()
{
    mAddr = 0;
    mUnMaxSum = 0;
    mUnMaxTime = 0;
    mSendReason = 0;
    mAppAddr = 0;
    mMsgAddr = 0;
    mTimeLen = 0;
}

void TIec104Data::fCopy(const TIec104Data *Iec104Data)
{
    if(Iec104Data)
    {
        mAddr = Iec104Data->mAddr;
        mUnMaxSum = Iec104Data->mUnMaxSum;
        mUnMaxTime = Iec104Data->mUnMaxTime;
        mSendReason = Iec104Data->mSendReason;
        mAppAddr = Iec104Data->mAppAddr;
        mMsgAddr = Iec104Data->mMsgAddr;
        mTimeLen = Iec104Data->mTimeLen;
    }
}

TIec104Data::~TIec104Data()
{
}

TModebusRtuNode::TModebusRtuNode()
{
    mHost = 0;
    mFun = 0;
    mStartAddr = 0;
    mRegCount = 0;
    mDataLen = 0;
    mType = 0;
}

TModebusRtuNode::TModebusRtuNode(const TModebusRtuNode *Copy)
{
    if(Copy)
    {
        mHost = Copy->mHost;
        mFun = Copy->mFun;
        mStartAddr = Copy->mStartAddr;
        mRegCount = Copy->mRegCount;
        mDataLen = Copy->mDataLen;
        mType = Copy->mType;
    }
    else
    {
        mHost = 0;
        mFun = 0;
        mStartAddr = 0;
        mRegCount = 0;
        mDataLen = 0;
        mType = 0;
    }
}

TModebusRtuNode::TModebusRtuNode(int Host, int Fun, int StartAddr,
                                 int RegCount, int DataLen, int Type)
{
    mHost = Host;
    mFun = Fun;
    mStartAddr = StartAddr;
    mRegCount = RegCount;
    mDataLen = DataLen;
    mType = Type;
}

TModebusRtuNode::~TModebusRtuNode()
{}

TModebusRtuData::TModebusRtuData()
{}

TModebusRtuData::~TModebusRtuData()
{
    foreach(TModebusRtuNode *Node, mList)
    {
        mList.removeOne(Node);
        delete Node;
    }
}

void TModebusRtuData::fCopy(const TModebusRtuData *ModebusRtuData)
{
    if(ModebusRtuData)
    {
        fClearHost();

        foreach(TModebusRtuNode *Node, ModebusRtuData->mList)
        {
            mList.push_back(new TModebusRtuNode(Node->mHost, Node->mFun,
                                                Node->mStartAddr, Node->mRegCount,
                                                Node->mDataLen, Node->mType));
        }
    }
}

void TModebusRtuData::fAddHost(int Host, int Fun, int StartAddr,
                               int RegCount, int DataLen, int Type)
{
    //qDebug() << Host << Fun << StartAddr << RegCount << Type;

    mList.push_back(new TModebusRtuNode(Host, Fun, StartAddr,
                                        RegCount, DataLen, Type));
}

void TModebusRtuData::fDelHost(TModebusRtuNode *Node)
{
    if(Node && mList.contains(Node))
    {
        mList.removeOne(Node);
        delete Node;
    }
}

void TModebusRtuData::fDelHost(int Host, int Fun, int StartAddr,
                               int RegCount, int DataLen, int Type)
{
    foreach(TModebusRtuNode *Node, mList)
    {
        if(Host == Node->mHost && Fun == Node->mFun
                && StartAddr == Node->mStartAddr && RegCount == Node->mRegCount
                &&  DataLen == Node->mDataLen && Type == Node->mType)
        {
            fDelHost(Node);
            break;
        }
    }
}

void TModebusRtuData::fClearHost()
{
    foreach(TModebusRtuNode *Node, mList)
    {
        fDelHost(Node);
    }
}
///////////////////////////////////////////////////////////////////

void TService::fInit()
{
    mId = 0;
    mPort = 0;
    mConnectMode = CONNECT_MODE_NORMAL;
    mRecvMode = RECV_MODE_NORMAL,
    mAgreeMent = AgreeMent_UnKnown;

    mClientData = NULL;
    mServerData = NULL;
    mComData = NULL;

    mIec104Data = NULL;
    mModebusRtuData = NULL;


#ifdef DEF_RUN_AS_RUNER
    mProtocol = NULL;
    mConnection = NULL;
    mServer = NULL;
#endif
}

TService::TService()
{
    fInit();
}

void TService::fCopy(TService *From)
{
    if(NULL == From)
    {
        return;
    }

    fSetId(From->fGetId());
    fSetName(From->fGetName());

    fSetPort(From->fGetPort());
    fSetConnectMode(From->fGetConnectMode());
    fSetRecvMode(From->fGetRecvMode());

    //qDebug() << "fGetPort()" << fGetPort();

    switch(fGetPort())
    {
    case TService::Port_Net_Client_Tcp:
    case TService::Port_Net_Client_Udp:
    {
        TClientData *DstClient = fGetClientData();
        if(DstClient)
        {
            DstClient->fCopy(From->fGetClientData());
        }
    }
    break;

    case TService::Port_Net_Server_Tcp:
    case TService::Port_Net_Server_Udp:
    {
        TServerData *DstServer = fGetServerData();
        if(DstServer)
        {
            DstServer->fCopy(From->fGetServerData());
        }
    }
    break;

    case Port_Com:
    {
        TComData *DstCom = fGetComData();

        if(DstCom)
        {
            DstCom->fCopy(From->fGetComData());
        }
    }
    break;

    default:
    break;
    }

    fSetAgreeMent(From->fGetAgreeMent());

    //qDebug() << "fGetAgreeMent()" << fGetAgreeMent();

    switch(fGetAgreeMent())
    {
    case AgreeMent_Iec101:
        break;

    case AgreeMent_Iec104:
    {
        TIec104Data *DstData = fGetIec104Data();

        if(DstData)
        {
            DstData->fCopy(From->fGetIec104Data());
        }
    }
    break;

    case AgreeMent_Modebus_RTU:
    {
        TModebusRtuData *ModebusData = fGetModebusRtuData();

        if(ModebusData)
        {
            ModebusData->fCopy(From->fGetModebusRtuData());
        }
    }
    break;

    default:
        break;
    }
}

TService::TService(TService *CopyService)
{
    fInit();

    fCopy(CopyService);
}

void TService::fSetId(int Id)
{
    mId = Id;
}

int TService::fGetId() const
{
    return mId;
}

void TService::fSetName(const QString &Name)
{
    mName = Name;
}

QString TService::fGetName() const
{
    return mName;
}

bool TService::fIsServer()
{
    if(Port_Net_Server_Tcp == mPort
            || Port_Net_Server_Udp == mPort)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TService::fSetPort(int Port)
{
    if(Port >= Port_Net_Server_Tcp && Port <= Port_Com)
    {
        mPort = Port;

        switch(mPort)
        {
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Client_Udp:
        {
            if(NULL == mClientData)
            {
                mClientData = new TClientData();
            }
        }
        break;

        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Server_Udp:
        {

            if(NULL == mServerData)
            {
                mServerData = new TServerData();
            }
        }
        break;

        case Port_Com:
        {
            if(NULL == mComData)
            {
                mComData = new TComData();
            }
        }
        break;

        default:
        break;
        }
    }
}

int TService::fGetPort() const
{
    return mPort;
}

void TService::fSetConnectMode(int Mode)
{
    mConnectMode = Mode;
}

int TService::fGetConnectMode() const
{
    return mConnectMode;
}

void TService::fSetRecvMode(int Mode)
{
    mRecvMode = Mode;
}

int TService::fGetRecvMode() const
{
    return mRecvMode;
}

void TService::fSetAgreeMent(int AgreeMent)
{
    if(AgreeMent >= AgreeMent_Iec101 && AgreeMent < AgreeMent_UnKnown)
    {
        mAgreeMent = AgreeMent;

        switch(mAgreeMent)
        {
        case AgreeMent_Iec101:
            break;

        case AgreeMent_Iec104:
            if(NULL == mIec104Data)
            {
                mIec104Data = new TIec104Data();
            }
            break;

        case AgreeMent_Modebus_RTU:
            if(NULL == mModebusRtuData)
            {
                mModebusRtuData = new TModebusRtuData();
            }
            break;

        default:
            break;
        }
    }
}

int TService::fGetAgreeMent() const
{
    return mAgreeMent;
}

TClientData *TService::fGetClientData()
{
    return mClientData;
}

TServerData *TService::fGetServerData()
{
    return mServerData;
}

TComData *TService::fGetComData()
{
    return mComData;
}

TIec104Data *TService::fGetIec104Data()
{
    return mIec104Data;
}

TModebusRtuData *TService::fGetModebusRtuData()
{
    return mModebusRtuData;
}

TService::~TService()
{
    if(mClientData)
    {
        delete mClientData;
    }

    if(mServerData)
    {
        delete mServerData;
    }

    if(mComData)
    {
        delete mComData;
    }

    if(mIec104Data)
    {
        delete mIec104Data;
    }
}

#ifdef DEF_RUN_AS_RUNER
TProtocol* TService::fGetProtocol()
{
    return mProtocol;
}

TProtocol* TService::fCreateProtocol()
{
    if(mProtocol)
    {
        qDebug() << "TService::fCreateProtocol() has Created";
        //mProtocol->deleteLater();
        //mProtocol = NULL;

        return mProtocol;
    }

    //qDebug() << "fGetAgreeMent()" << fGetAgreeMent();

    switch(fGetAgreeMent())
    {
    //case TService::AgreeMent_ServerClient:
    //{}
    //break;

    // IEC101
    case TService::AgreeMent_Iec101:
    {}
    break;

    // IEC104
    case TService::AgreeMent_Iec104:
    {
        mProtocol = new TIec104Service(fGetIec104Data());
    }
    break;

    case TService::AgreeMent_Modebus_RTU:
    {
        mProtocol = new TModbus(fGetModebusRtuData());
    }
    break;

    // CDT
    case TService::AgreeMent_CDT:
    {}
    break;

    // 376.1
    case TService::AgreeMent_3761:
    {}
    break;

    // 故障指示器
    case TService::AgreeMent_SY_GZZS:
    {
        mProtocol = new TSygzzs();
    }
    break;

    // 智能开关
    case TService::AgreeMent_SY_ZNKG:
    {
        mProtocol = new TSyznkg();
    }
    break;

    // 实时数据
    case TService::AgreeMent_SY_REAL:
    {
        mProtocol = new TSyreal();
    }
    break;

    default:
    break;
    }

    if(mProtocol)
    {
        mProtocol->fSetServiceId(this->fGetId());
    }

    return mProtocol;
}

TConnection* TService::fGetConnection()
{
    return mConnection;
}

TConnection* TService::fCreateConnection()
{
    qDebug() << "Service->mPort" << fGetPort();

    if(fIsServer())
    {
        return NULL;
    }

    if(mConnection)
    {
        qDebug() << "TService::fCreateConnection() has Created";
        //mConnection->deleteLater();
        //mConnection = NULL;

        return mConnection;
    }

    switch(fGetPort())
    {
    // 网络客户端TCP
    case TService::Port_Net_Client_Tcp:
        {
            TTcpSocket *TcpSocket = new TTcpSocket();

            TClientData *ClientData = fGetClientData();

            if(ClientData)
            {
                TcpSocket->fSetHost(ClientData->fGetIp(), ClientData->fGetPort());
                TcpSocket->fCreateReadRingbuf(1024);
            }

            mConnection = TcpSocket;
        }
        break;

    // 网络客户端UDP
    case TService::Port_Net_Client_Udp:
        {}
        break;

    // 串口
    case TService::Port_Com:
        {
            TCom *Com = new TCom();

            TComData *ComData = fGetComData();

            if(ComData)
            {
                qDebug() << ComData->fGetName();
                qDebug() << ComData->fGetRate();
                qDebug() << ComData->fGetDataBits();
                qDebug() << ComData->fGetStopBits();
                qDebug() << ComData->fGetCheck();

                Com->fSetPortName(ComData->fGetName());
                Com->fSetBaudRate(ComData->fGetRate());
                Com->fSetDataBits(ComData->fGetDataBits());
                Com->fSetStopBits(ComData->fGetStopBits());
                Com->fSetParity(ComData->fGetCheck());
            }

            mConnection = Com;

            mConnection->fSetUserData((void *)Com->fGetBaudRate());
        }
        break;

    default:
        break;
    }

    return mConnection;
}

TTcpServer* TService::fGetServer()
{
    return mServer;
}

TTcpServer* TService::fCreateServer()
{
    qDebug() << "Service->mPort" << fGetPort();

    if(!fIsServer())
    {
        return NULL;
    }

    if(mServer)
    {
        qDebug() << "TService::fCreateServer() has Created";
        return mServer;
    }

    switch(fGetPort())
    {
    // 网络服务器TCP
    case TService::Port_Net_Server_Tcp:
        {
            mServer = new TTcpServer();

            TServerData *ServerData = fGetServerData();

            if(ServerData)
            {
                if(!mServer->listen(QHostAddress::Any, ServerData->fGetPort()))
                {
                    //return false;
                }
            }
        }
        break;

    // 网络服务器UDP
    case TService::Port_Net_Server_Udp:
        {}
        break;

    default:
        break;
    }

    return mServer;
}

void TService::fDestroyProtocol()
{
    if(mProtocol)
    {
        mProtocol->deleteLater();
    }
}

void TService::fDestroyConnection()
{
    if(mConnection)
    {
        mConnection->fStop();
        mConnection->deleteLater();
    }
}

void TService::fDestroyServer()
{
    if(mServer)
    {
        mServer->close();
        mServer->deleteLater();
    }
}

#endif
