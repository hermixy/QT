#include "DialogService.h"
#include "DialogServiceEdit.h"
#include <QDebug>
#include <QHostAddress>

void DialogService::fInit()
{
    mSave = false;
    mServiceId = 0;

    // 服务列表
    mGroupBoxServiceList = NULL;
    mLabelServiceListShow = NULL;
    mTableServiceList = NULL;

    mGroupBoxServiceMsg = NULL;
    mTextEditMsg = NULL;

    mGroupBoxControl = NULL;
    mButtonAdd = NULL;
    mButtonEdit = NULL;
    mButtonDel = NULL;

    mButtonOk = NULL;
    mButtonClose = NULL;
}

DialogService::DialogService(QList<TService *> &ServiceList, QWidget *parent) : QDialog(parent)
{
    fInit();

    foreach(TService *Service, ServiceList)
    {
        mServiceList.push_back(new TService(Service));
    }

    ////////////////////////////////////////////////////////
    mGroupBoxServiceList = new QGroupBox(tr(""));
    mLabelServiceListShow = TBase::fLabel(tr("服务列表"), tr("mLabelServiceListShow"));
    mTableServiceList = new TTableView(tr("mTableServiceList"));
    mTableServiceList->fAddCol(tr("名称"));
    mTableServiceList->fAddCol(tr("端口"));
    mTableServiceList->fAddCol(tr("协议"));
    mTableServiceList->fSetHeadLength(0, 100);
    mTableServiceList->fSetHeadLength(1, 100);
    mTableServiceList->fSetHeadLength(2, 100);
    mTableServiceList->setMinimumWidth(300);                //设定最小宽度

    foreach(TService *Service, mServiceList)
    {
        fAddServiceToTable(Service);

        if(Service->fGetId() > mServiceId)
        {
            mServiceId = Service->fGetId();
        }
    }

    connect(mTableServiceList, SIGNAL(mClick(int,int)), this, SLOT(fServiceListChanged(int,int)));

    QVBoxLayout *LayoutTable = new QVBoxLayout(mGroupBoxServiceList);
    LayoutTable->addWidget(mLabelServiceListShow);
    LayoutTable->addWidget(mTableServiceList);

    ///////

    mGroupBoxControl = new QGroupBox(tr(""));
    mButtonAdd = TBase::fButton(tr("添加"), tr("mButtonAdd"));
    mButtonAdd->setFixedWidth(mButtonAdd->sizeHint().height() * 2);
    connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(fAdd()));
    mButtonEdit = TBase::fButton(tr("修改"), tr("mButtonEdit"));
    mButtonEdit->setFixedWidth(mButtonEdit->sizeHint().height() * 2);
    connect(mButtonEdit, SIGNAL(clicked()), this, SLOT(fEdit()));
    mButtonDel = TBase::fButton(tr("删除"), tr("mButtonDel"));
    mButtonDel->setFixedWidth(mButtonDel->sizeHint().height() * 2);
    connect(mButtonDel, SIGNAL(clicked()), this, SLOT(fDel()));

    QVBoxLayout *LayoutControl = new QVBoxLayout(mGroupBoxControl);
    LayoutControl->addWidget(mButtonAdd);
    LayoutControl->addWidget(mButtonEdit);
    LayoutControl->addWidget(mButtonDel);

    ////
    mGroupBoxServiceMsg = new QGroupBox(tr(""));
    mTextEditMsg = TBase::fTextEdit(tr("mTextEditMsg"));
    mTextEditMsg->setReadOnly(true); // 只读
    mTextEditMsg->setWordWrapMode(QTextOption::NoWrap); // 自动出现横向滚动条
    QVBoxLayout *LayoutRight = new QVBoxLayout(mGroupBoxServiceMsg);
    LayoutRight->addWidget(mTextEditMsg);

    QHBoxLayout *LayoutTop = new QHBoxLayout();
    LayoutTop->addWidget(mGroupBoxServiceList);
    LayoutTop->addWidget(mGroupBoxControl);
    LayoutTop->addWidget(mGroupBoxServiceMsg);

    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOk()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonClose);
    mLayoutButton->addStretch();

    QVBoxLayout *LayoutMain = new QVBoxLayout(this);
    LayoutMain->addLayout(LayoutTop);
    LayoutMain->addStretch();
    LayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(LayoutMain);
    setWindowTitle(tr("管理服务"));
    //setFixedSize(sizeHint());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void DialogService::fAddServiceToTable(TService *Service)
{
    if(NULL == Service || NULL == mTableServiceList)
    {
        return;
    }

    QList<QString>RowData;
    RowData.push_back(Service->fGetName());
    RowData.push_back(fGetPortName(Service->fGetPort()));
    RowData.push_back(fGetAgreeName(Service->fGetAgreeMent()));

    mTableServiceList->fAddRow(RowData);
}

QString DialogService::fGetPortName(int Index)
{
    QString Name = tr("未知");

    switch(Index)
    {
    case 0: Name = tr("网络服务器TCP");
            break;

    case 1: Name = tr("网络服务器UDP");
            break;

    case 2: Name = tr("网络客户端TCP");
            break;

    case 3: Name = tr("网络客户端UDP");
            break;

    case 4: Name = tr("串口");
            break;

    default:
            break;
    }

    return Name;
}

QString DialogService::fGetAgreeName(int Index)
{
    QString Name = tr("未知");

    switch(Index)
    {
    case 0: Name= tr("自定义协议");
        break;
    case 10: Name = tr("IEC101");
            break;

    case 1: Name = tr("IEC104");
            break;

    case 2: Name = tr("CDT");
            break;

    case 3: Name = tr("376.1");
            break;

    case 4: Name = tr("Modebus-RTU");
            break;

    case 5: Name = tr("私有-故障指示器");
            break;

    case 6: Name = tr("私有-智能开关");
            break;

    case 7: Name = tr("私有-实时数据");
            break;

    default:
        break;
    }

    return Name;
}

DialogService::~DialogService()
{
    foreach(TService *Service, mServiceList)
    {
        mServiceList.removeOne(Service);
        delete Service;
    }
}

void DialogService::fOk()
{
    mSave = true;
    close();
}

void DialogService::fAdd()
{
    DialogServiceEdit *ServiceEdit = new DialogServiceEdit(NULL ,this);

    if(ServiceEdit)
    {
        ServiceEdit->exec();

        if(ServiceEdit->mSave)
        {
            for(int i=0; i<ServiceEdit->ipNum; i++)
            {
                TService *AddServie = new TService(ServiceEdit->mService);

                if(AddServie)
                {
                    TServerData *ServerData = AddServie->fGetServerData();
                    if(ServerData)
                    {
                        quint32 ipv4=QHostAddress(ServerData->fGetIp()).toIPv4Address()+i;
                        ServerData->fSetIp(QHostAddress(ipv4).toString());
                    }
                    if(i)
                        AddServie->fSetName(AddServie->fGetName()+QString::number(i));
                    else
                        AddServie->fSetName(AddServie->fGetName());
                    AddServie->fSetId(++mServiceId);
                    fAddServiceToTable(AddServie);
                    mServiceList.push_back(AddServie);
                }
            }

        }

        delete ServiceEdit;
    }
}

void DialogService::fEdit()
{
    int Index = mTableServiceList->fGetCurrentIndex();

    if(Index < 0 || Index >= mTableServiceList->fGetRowSum()
        || Index >= mServiceList.length())
    {
        return;
    }

    TService *Service = mServiceList.at(Index);

    DialogServiceEdit *ServiceEdit = new DialogServiceEdit(Service ,this);

    if(ServiceEdit)
    {
        ServiceEdit->exec();

        if(ServiceEdit->mSave)
        {
            Service->fCopy(ServiceEdit->mService);
            fServiceListChanged(Index, 0);

            // 更新表格
            mTableServiceList->fSetCell(Index, 0, Service->fGetName());
            mTableServiceList->fSetCell(Index, 1, fGetPortName(Service->fGetPort()));
            mTableServiceList->fSetCell(Index, 2, fGetAgreeName(Service->fGetAgreeMent()));
        }

        delete ServiceEdit;
    }
}

void DialogService::fDel()
{
    int index = mTableServiceList->fGetCurrentIndex();
    TService *Service = mServiceList.at(index);
    mServiceList.removeOne(Service);
    delete Service;

    mTableServiceList->fDelRow(index);

    mTextEditMsg->clear();
}

void DialogService::fServiceListChanged(int Row, int Col)
{
    Q_UNUSED(Col);

    if(Row < 0 || Row >= mServiceList.length())
    {
        return;
    }

    mTextEditMsg->clear();

    TService *Service = mServiceList.at(Row);

    if(NULL == Service)
    {
        return;
    }

    QFont Font;

    Font.setBold(true);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(tr("服务名称:"));
    Font.setBold(false);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(Service->fGetName() + tr("\n\n"));

    Font.setBold(true);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(tr("通讯方式:"));
    Font.setBold(false);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(fGetPortName(Service->fGetPort()) + tr("\n"));
    mTextEditMsg->insertPlainText(tr("    连接方式:") + (Service->fGetConnectMode() ? tr("自动") : tr("手动")) + tr("\n"));
    mTextEditMsg->insertPlainText(tr("    接收方式:") + (Service->fGetRecvMode() ? tr("自动接收") : tr("协议内接收")) + tr("\n"));

    switch(Service->fGetPort())
    {
    case TService::Port_Net_Client_Tcp:
    case TService::Port_Net_Client_Udp:
    {
        TClientData *ClientData = Service->fGetClientData();

        if(ClientData)
        {
            mTextEditMsg->insertPlainText(tr("    IP地址:") + ClientData->fGetIp() + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    端口号:%1").arg(ClientData->fGetPort()) + tr("\n"));
        }
    }
    break;

    case TService::Port_Net_Server_Tcp:
    case TService::Port_Net_Server_Udp:
    {
        TServerData *ServerData = Service->fGetServerData();

        if(ServerData)
        {
            mTextEditMsg->insertPlainText(tr("    IP地址:") + ServerData->fGetIp() + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    端口号:%1").arg(ServerData->fGetPort()) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    收发线程数:%1").arg(ServerData->fGetIoThreadSum()) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    处理线程数:%1").arg(ServerData->fGetProcessThreadSum()) + tr("\n"));
        }
    }
    break;

    case TService::Port_Com:
    {
        TComData *ComData = Service->fGetComData();

        if(ComData)
        {
            mTextEditMsg->insertPlainText(tr("    端口号:") + ComData->fGetName() + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    波特率:%1").arg(ComData->fGetRate()) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    数据位:%1").arg(ComData->fGetDataBits()) + tr("\n"));

            QString Str = tr("未知");
            switch(ComData->fGetStopBits())
            {
            case 0:
                Str = tr("1");
                break;

            case 1:
                Str = tr("1.5");
                break;

            case 2:
                Str = tr("2");
                break;

            default:
                break;
            }

            mTextEditMsg->insertPlainText(tr("    停止位:") + Str + tr("\n"));

            Str = tr("未知");
            switch(ComData->fGetCheck())
            {
            case 0:
                Str = tr("无校验(None)");
                break;

            case 1:
                Str = tr("奇校验(Odd)");
                break;

            case 2:
                Str = tr("偶校验(Even)");
                break;

            case 3:
                Str = tr("1校验(Mark)");
                break;

            case 4:
                Str = tr("0校验(Space)");
                break;

            default:
                break;
            }

            mTextEditMsg->insertPlainText(tr("    校验位:") + Str + tr("\n"));
        }
    }
    break;

    default:
        break;
    }

    mTextEditMsg->insertPlainText(tr("\n"));
    Font.setBold(true);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(tr("通讯协议:"));
    Font.setBold(false);
    mTextEditMsg->setCurrentFont(Font);
    mTextEditMsg->insertPlainText(fGetAgreeName(Service->fGetAgreeMent()) + tr("\n"));

    switch(Service->fGetAgreeMent())
    {
    case TService::AgreeMent_Iec104:
    {
        TIec104Data *Iec104Data = Service->fGetIec104Data();

        if(Iec104Data)
        {
            mTextEditMsg->insertPlainText(tr("    设备地址:%1").arg(Iec104Data->mAddr) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    最大未确认数:%1").arg(Iec104Data->mUnMaxSum) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    最大未确认时间:%1").arg(Iec104Data->mUnMaxTime) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    应用地址:%1").arg(Iec104Data->mAppAddr) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    信息地址:%1").arg(Iec104Data->mMsgAddr) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    传送原因长度:%1").arg(Iec104Data->mSendReason) + tr("\n"));
            mTextEditMsg->insertPlainText(tr("    时标长度:%1").arg(Iec104Data->mTimeLen) + tr("\n"));
        }
    }
    break;

    case TService::AgreeMent_Modebus_RTU:
    {
        TModebusRtuData *ModebusRtuData = Service->fGetModebusRtuData();

        if(ModebusRtuData)
        {
            foreach(TModebusRtuNode *Node, ModebusRtuData->mList)
            {
                mTextEditMsg->insertPlainText(tr("    设备:%1,功能:%2,起始地址:%3,寄存器个数:%4,数据%5位,类型%6")
                                              .arg(Node->mHost)
                                              .arg(Node->mFun)
                                              .arg(Node->mStartAddr)
                                              .arg(Node->mRegCount)
                                              .arg(Node->mDataLen)
                                              .arg(Node->mType)
                                              + tr("\n"));
            }
        }
    }
    break;

    default:
        break;
    }
}
