#include "DialogServiceEdit.h"

#include <QDebug>

void DialogServiceEdit::fInit()
{
    mSave = false;
    mService = NULL;

    mGroupBoxServiceMsg = NULL;
    mLabelServiceNameShow = NULL;
    mLineEditServiceName = NULL;
    mLabelPortShow = NULL;
    mComboBoxPort = NULL;
    mLabelPortMode = NULL;
    mComboBoxPortMode = NULL;
    mLabelAgreeMentShow = NULL;
    mComboBoxAgreeMent = NULL;

    // 网络IP Port
    mGroupBoxNetMsg = NULL;
    mLabelIpShow = NULL;
    mLineEditIp = NULL;
    mLabelNetPortShow = NULL;
    mSpinBoxNetPort = NULL;
    // 客户端
    mGroupBoxClientMsg = NULL;
    mLabelRecvModeShow = NULL;
    mComboBoxRecvMode = NULL;
    // 服务器
    mGroupBoxServerMsg = NULL;
    mLabelIoThreadShow = NULL;
    mSpinBoxIoThread = NULL;
    mLabelProcessThreadShow = NULL;
    mSpinBoxProcessThread = NULL;

    // 串口
    mGroupBoxComMsg = NULL;
    //mLabelComModeShow = NULL;
    //mComboBoxComMode = NULL;
    mLabelSystemShow = NULL;
    mComboBoxSystem = NULL;
    mLabelComNumShow = NULL;
    mSpinBoxComNum = NULL;
    mLabelComNameShow = NULL;
    mLineEditComName = NULL;
    mLabelComRateShow = NULL;
    mComboBoxComRate = NULL;
    mLabelComDataShow = NULL;
    mComboBoxComData = NULL;
    mLabelComStopShow = NULL;
    mComboBoxComStop = NULL;
    mLabelComCheckShow = NULL;
    mComboBoxComCheck = NULL;

    // 104协议
    mGroupBoxIec104Msg = NULL;
    mLabelIec104Addr = NULL;
    mSpinBoxIec104Addr = NULL;
    mLabelIec104UnMaxSumShow = NULL;
    mSpinBoxIec104UnMaxSum = NULL;
    mLabelIec104UnMaxTimeShow = NULL;
    mSpinBoxIec104UnMaxTime = NULL;
    mLabelIec104SendReasonShow = NULL;
    mComboBoxIec104SendReason = NULL;
    mLabelIec104AppAddrShow = NULL;
    mComboBoxIec104AppAddr = NULL;
    mLabelIec104MsgAddrShow = NULL;
    mComboBoxIec104MsgAddr = NULL;
    mLabelIec104TimeLenShow = NULL;
    mComboBoxIec104TimeLen = NULL;

    // RTU
    mGroupBoxRtuMsg = NULL;
    mLableRtuListShow = NULL;
    mTableRtuList = NULL;
    mLabelRtuHostShow = NULL;
    mSpinBoxRtuHost = NULL;
    mLabelRtuFunShow = NULL;
    mSpinBoxRtuFun = NULL;
    mLabelRtuStartAddrShow = NULL;
    mSpinBoxRtuStartAddr = NULL;
    mLabelRtuRegCountShow = NULL;
    mSpinBoxRtuRegCount = NULL;
    mLabelRtuDataLenShow = NULL;
    mSpinBoxRtuDataLen = NULL;
    mLabelRtuTypeShow = NULL;
    mSpinBoxRtuType = NULL;
    mButtonRtuAdd = NULL;
    mButtonRtuDel = NULL;

    mLabelResult = NULL;

    mButtonOk = NULL;
    mButtonClose = NULL;
}

DialogServiceEdit::DialogServiceEdit(TService *Service, QWidget *parent) : QDialog(parent)
{
    fInit();

    mService = new TService(Service);

    if(NULL == mService)
    {
        return;
    }

    ////////////////////////////////////
    mGroupBoxServiceMsg = new QGroupBox(tr(""));
    mLabelServiceNameShow = TBase::fLabel(tr("服务名称"), tr("mLabelServiceNameShow"));
    mLineEditServiceName = TBase::fLineEdit(tr("mLineEditServiceName"));
    mLineEditServiceName->setText(mService->fGetName());

    mLabelAgreeMentShow = TBase::fLabel(tr("通讯协议"), tr("mLabelAgreeMentShow"));
    mComboBoxAgreeMent = TBase::fComboBox(tr("mComboBoxAgreeMent"));
    /*mComboBoxAgreeMent->addItem(tr("IEC101"));
    mComboBoxAgreeMent->addItem(tr("IEC104"));
    mComboBoxAgreeMent->addItem(tr("CDT"));
    mComboBoxAgreeMent->addItem(tr("376.1"));
    mComboBoxAgreeMent->addItem(tr("Modebus-RTU"));
    mComboBoxAgreeMent->addItem(tr("私有-故障指示器"));
    mComboBoxAgreeMent->addItem(tr("私有-智能开关"));
    mComboBoxAgreeMent->addItem(tr("私有-实时数据"));*/
    //mComboBoxAgreeMent->addItem(tr("Modebus-RTU"));
    mComboBoxAgreeMent->addItem(tr("自定义协议"));
    connect(mComboBoxAgreeMent, SIGNAL(currentIndexChanged(int)), this, SLOT(fAgreeMentChanged(int)));

    mLabelPortShow = TBase::fLabel(tr("通讯方式"), tr("mLabelPortShow"));
    mComboBoxPort = TBase::fComboBox(tr("mComboBoxPort"), 120, 22);

    /*mComboBoxPort->addItem(tr("网络客户端TCP"));
    mComboBoxPort->addItem(tr("网络客户端UDP"));
    mComboBoxPort->addItem(tr("串口"));*/

    mComboBoxPort->addItem(tr("网络服务器TCP"));
    mComboBoxPort->addItem(tr("网络服务器UDP"));


    connect(mComboBoxPort, SIGNAL(currentIndexChanged(int)), this, SLOT(fPortChanged(int)));

    mLabelPortMode = TBase::fLabel(tr("连接方式"), tr("mLabelPortMode"));
    mComboBoxPortMode = TBase::fComboBox(tr("mComboBoxPortMode"));
    mComboBoxPortMode->addItem(tr("手动"));
    mComboBoxPortMode->addItem(tr("自动"));
    mComboBoxPortMode->setCurrentIndex(mService->fGetConnectMode());

    mLabelRecvModeShow = TBase::fLabel(tr("接收模式"), tr("mLabelRecvModeShow"));
    mComboBoxRecvMode = TBase::fComboBox(tr("mComboBoxRecvMode"));
    mComboBoxRecvMode->addItem(tr("协议内接收"));
    mComboBoxRecvMode->addItem(tr("自动接收"));
    mComboBoxRecvMode->setCurrentIndex(mService->fGetRecvMode());

    QGridLayout *LayoutServiceMsg = new QGridLayout(mGroupBoxServiceMsg);
    LayoutServiceMsg->addWidget(mLabelServiceNameShow, 0, 0);
    LayoutServiceMsg->addWidget(mLineEditServiceName, 0, 1);
    LayoutServiceMsg->addWidget(mLabelAgreeMentShow, 1, 0);
    LayoutServiceMsg->addWidget(mComboBoxAgreeMent, 1, 1);
    LayoutServiceMsg->addWidget(mLabelPortMode, 2, 0);
    LayoutServiceMsg->addWidget(mComboBoxPortMode, 2, 1);
    LayoutServiceMsg->addWidget(mLabelPortShow, 3, 0);
    LayoutServiceMsg->addWidget(mComboBoxPort, 3, 1);
    LayoutServiceMsg->addWidget(mLabelRecvModeShow, 4, 0);
    LayoutServiceMsg->addWidget(mComboBoxRecvMode, 4, 1);

    /////////////////
    mGroupBoxNetMsg = new QGroupBox(tr(""));
    mGroupBoxClientMsg = new QGroupBox(tr(""));// 网络IP Port
    mGroupBoxServerMsg = new QGroupBox(tr(""));// 网络IP Port
    mGroupBoxComMsg = new QGroupBox(tr(""));// 串口

    /////////////////
    mGroupBoxIec104Msg = new QGroupBox(tr(""));;
    mGroupBoxRtuMsg = new QGroupBox(tr(""));;

    QVBoxLayout *mLayoutNet = new QVBoxLayout();
    mLayoutNet->addWidget(mGroupBoxNetMsg);
    mLayoutNet->addWidget(mGroupBoxClientMsg);
    mLayoutNet->addWidget(mGroupBoxServerMsg);

    QHBoxLayout *LayoutTop = new QHBoxLayout();
    LayoutTop->addWidget(mGroupBoxServiceMsg);
    LayoutTop->addLayout(mLayoutNet);
    LayoutTop->addWidget(mGroupBoxComMsg);
    LayoutTop->addWidget(mGroupBoxIec104Msg);
    LayoutTop->addWidget(mGroupBoxRtuMsg);

    mLabelResult = TBase::fLabel(tr(""), tr("mLabelResult"));
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

    ////////////////////////////////////////
    // 必须手动调用fPortChanged一次，否则可能初始化不完全
    mComboBoxPort->setCurrentIndex(mService->fGetPort());
    fPortChanged(mService->fGetPort());
    mComboBoxAgreeMent->setCurrentIndex(mService->fGetAgreeMent());
    fAgreeMentChanged(mService->fGetAgreeMent());

    QVBoxLayout *LayoutMain = new QVBoxLayout(this);
    LayoutMain->addLayout(LayoutTop);
    LayoutMain->addStretch();
    LayoutMain->addWidget(mLabelResult, 0, Qt::AlignCenter);
    LayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(LayoutMain);
    setWindowTitle(tr("编辑服务"));
    //setFixedSize(sizeHint());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void DialogServiceEdit::fCreateServer()
{
    if(mGroupBoxServerMsg->layout())
    {
        return;
    }

    fCreateNet();

    mLabelIoThreadShow = TBase::fLabel(tr("收发线程数"), tr("mLabelIoThreadShow"));
    mSpinBoxIoThread = TBase::fSpinBox(tr("mSpinBoxIoThread"), 1, 64);
    mLabelProcessThreadShow = TBase::fLabel(tr("处理线程数"), tr("mLabelProcessThreadShow"));
    mSpinBoxProcessThread = TBase::fSpinBox(tr("mSpinBoxProcessThread"), 1, 64);

    QGridLayout *mLayoutServerMsg = new QGridLayout(mGroupBoxServerMsg);
    mLayoutServerMsg->addWidget(mLabelIoThreadShow, 0, 0);
    mLayoutServerMsg->addWidget(mSpinBoxIoThread, 0, 1);
    mLayoutServerMsg->addWidget(mLabelProcessThreadShow, 1, 0);
    mLayoutServerMsg->addWidget(mSpinBoxProcessThread, 1, 1);
}

void DialogServiceEdit::fCreateClient()
{
    if(mGroupBoxClientMsg->layout())
    {
        return;
    }

    fCreateNet();

    //QGridLayout *mLayoutClientMsg = new QGridLayout(mGroupBoxClientMsg);
}

void DialogServiceEdit::fCreateNet()
{
    if(mGroupBoxNetMsg->layout())
    {
        return;
    }

    mLabelIpShow = TBase::fLabel(tr("IP"), tr("mLabelIpShow"));
    mLineEditIp = TBase::fLineEdit(tr("mLineEditIp"), 120, 24);

    mLabelIPNum = TBase::fLabel(tr("连续IP个数"), tr("mLabelIPNum"));
    mLineEditIpNum = TBase::fLineEdit(tr("mLineEditIpNum"), 120, 24);
    mLineEditIpNum->setText(QString::number(ipNum));

    mLabelNetPortShow = TBase::fLabel(tr("端口"), tr("mLabelNetPortShow"));
    mSpinBoxNetPort = TBase::fSpinBox(tr("mSpinBoxNetPort"), 1, 65535);

    QGridLayout *mLayoutNetMsg = new QGridLayout(mGroupBoxNetMsg);
    mLayoutNetMsg->addWidget(mLabelIpShow, 0, 0);
    mLayoutNetMsg->addWidget(mLineEditIp, 0, 1);
    mLayoutNetMsg->addWidget(mLabelIPNum, 1, 0);
    mLayoutNetMsg->addWidget(mLineEditIpNum, 1, 1);

    mLayoutNetMsg->addWidget(mLabelNetPortShow, 2, 0);
    mLayoutNetMsg->addWidget(mSpinBoxNetPort, 2, 1);
}

void DialogServiceEdit::fCreateCom()
{
    if(mGroupBoxComMsg->layout())
    {
        return;
    }

    mLabelSystemShow = TBase::fLabel(tr("操作系统"), tr("mLabelSystemShow"));
    mComboBoxSystem = TBase::fComboBox(tr("mComboBoxSystem"));
    mComboBoxSystem->addItem(tr("Windows"));
    mComboBoxSystem->addItem(tr("Linux默认"));
    mComboBoxSystem->addItem(tr("Linux定制"));
    mComboBoxSystem->addItem(tr("Linux1"));
    mComboBoxSystem->addItem(tr("Linux2"));
    connect(mComboBoxSystem, SIGNAL(currentIndexChanged(int)), this, SLOT(fSystemChanged(int)));

    mLabelComNumShow = TBase::fLabel(tr("串口号"), tr("mLabelComNumShow"));
    mSpinBoxComNum = TBase::fSpinBox(tr("mSpinBoxComNum"), 1, 100);
    connect(mSpinBoxComNum, SIGNAL(valueChanged(int)), this, SLOT(fSystemChanged(int)));

    mLabelComNameShow = TBase::fLabel(tr("串口名称"), tr("mLabelComNameShow"));
    mLineEditComName = TBase::fLineEdit(tr("mLineEditComName"), 120, 24);
    mLineEditComName->setText(tr("COM1"));

    mLabelComRateShow = TBase::fLabel(tr("波特率"), tr("mLabelComRateShow"));
    mComboBoxComRate = TBase::fComboBox(tr("mComboBoxComRate"));
    mComboBoxComRate->addItem(tr("110"));
    mComboBoxComRate->addItem(tr("300"));
    mComboBoxComRate->addItem(tr("600"));
    mComboBoxComRate->addItem(tr("1200"));
    mComboBoxComRate->addItem(tr("2400"));
    mComboBoxComRate->addItem(tr("4800"));
    mComboBoxComRate->addItem(tr("9600"));
    mComboBoxComRate->addItem(tr("14400"));
    mComboBoxComRate->addItem(tr("19200"));
    mComboBoxComRate->addItem(tr("38400"));
    mComboBoxComRate->addItem(tr("56000"));
    mComboBoxComRate->addItem(tr("57600"));
    mComboBoxComRate->addItem(tr("115200"));
    mComboBoxComRate->setCurrentIndex(6);

    mLabelComDataShow = TBase::fLabel(tr("数据位"), tr("mLabelComDataShow"));
    mComboBoxComData = TBase::fComboBox(tr("mComboBoxComData"));
    mComboBoxComData->addItem(tr("4"));
    mComboBoxComData->addItem(tr("5"));
    mComboBoxComData->addItem(tr("6"));
    mComboBoxComData->addItem(tr("7"));
    mComboBoxComData->addItem(tr("8"));
    mComboBoxComData->setCurrentIndex(4);

    mLabelComStopShow = TBase::fLabel(tr("停止位"), tr("mLabelComStopShow"));
    mComboBoxComStop = TBase::fComboBox(tr("mComboBoxComStop"));
    mComboBoxComStop->addItem(tr("1"));
    mComboBoxComStop->addItem(tr("1.5"));
    mComboBoxComStop->addItem(tr("2"));
    mLabelComCheckShow = TBase::fLabel(tr("校验位"), tr("mLabelComCheckShow"));
    mComboBoxComCheck = TBase::fComboBox(tr("mComboBoxComCheck"));
    mComboBoxComCheck->addItem(tr("无校验(None)"));
    mComboBoxComCheck->addItem(tr("奇校验(Odd)"));
    mComboBoxComCheck->addItem(tr("偶校验(Even)"));
    mComboBoxComCheck->addItem(tr("1校验(Mark)"));
    mComboBoxComCheck->addItem(tr("0校验(Space)"));

    QGridLayout *LayoutComMsg = new QGridLayout(mGroupBoxComMsg);
    LayoutComMsg->addWidget(mLabelSystemShow, 1, 0);
    LayoutComMsg->addWidget(mComboBoxSystem, 1, 1);
    LayoutComMsg->addWidget(mLabelComNumShow, 2, 0);
    LayoutComMsg->addWidget(mSpinBoxComNum, 2, 1);
    LayoutComMsg->addWidget(mLabelComNameShow, 3, 0);
    LayoutComMsg->addWidget(mLineEditComName, 3, 1);
    LayoutComMsg->addWidget(mLabelComRateShow, 4, 0);
    LayoutComMsg->addWidget(mComboBoxComRate, 4, 1);
    LayoutComMsg->addWidget(mLabelComDataShow, 5, 0);
    LayoutComMsg->addWidget(mComboBoxComData, 5, 1);
    LayoutComMsg->addWidget(mLabelComStopShow, 6, 0);
    LayoutComMsg->addWidget(mComboBoxComStop, 6, 1);
    LayoutComMsg->addWidget(mLabelComCheckShow, 7, 0);
    LayoutComMsg->addWidget(mComboBoxComCheck, 7, 1);
}

void DialogServiceEdit::fCreateIec104()
{
    if(mGroupBoxIec104Msg->layout())
    {
        return;
    }

    qDebug() << "DialogService::fCreateIec104()";

    mLabelIec104Addr = TBase::fLabel(tr("设备地址"), tr("mLabelIec104Addr"));
    mSpinBoxIec104Addr = TBase::fSpinBox(tr("mSpinBoxIec104Addr"), 0, 255);
    mLabelIec104UnMaxSumShow = TBase::fLabel(tr("最大未确认数"), tr("mLabelIec104UnMaxSumShow"));
    mSpinBoxIec104UnMaxSum = TBase::fSpinBox(tr("mSpinBoxIec104UnMaxSum"), 1, 255);
    mLabelIec104UnMaxTimeShow = TBase::fLabel(tr("最大未确认时间"), tr("mLabelIec104UnMaxTimeShow"));
    mSpinBoxIec104UnMaxTime = TBase::fSpinBox(tr("mSpinBoxIec104UnMaxTime"), 1, 255);
    mLabelIec104SendReasonShow = TBase::fLabel(tr("传送原因长度"), tr("mLabelIec104SendReasonShow"));
    mComboBoxIec104SendReason = TBase::fComboBox(tr("mComboBoxIec104SendReason"));
    mComboBoxIec104SendReason->addItem(tr("1"));
    mComboBoxIec104SendReason->addItem(tr("2"));
    mLabelIec104AppAddrShow = TBase::fLabel(tr("公共地址长度"), tr("mLabelIec104AppAddrShow"));
    mComboBoxIec104AppAddr = TBase::fComboBox(tr("mComboBoxIec104AppAddr"));
    mComboBoxIec104AppAddr->addItem(tr("1"));
    mComboBoxIec104AppAddr->addItem(tr("2"));
    mLabelIec104MsgAddrShow = TBase::fLabel(tr("信息地址长度"), tr("mLabelIec104MsgAddrShow"));
    mComboBoxIec104MsgAddr = TBase::fComboBox(tr("mComboBoxIec104MsgAddr"));
    mComboBoxIec104MsgAddr->addItem(tr("1"));
    mComboBoxIec104MsgAddr->addItem(tr("2"));
    mComboBoxIec104MsgAddr->addItem(tr("3"));
    mLabelIec104TimeLenShow = TBase::fLabel(tr("时标长度"), tr("mLabelIec104TimeLenShow"));
    mComboBoxIec104TimeLen = TBase::fComboBox(tr("mComboBoxTimeLen"));
    mComboBoxIec104TimeLen->addItem(tr("2"));
    mComboBoxIec104TimeLen->addItem(tr("3"));
    mComboBoxIec104TimeLen->addItem(tr("7"));

    QGridLayout *LayoutIec104Msg = new QGridLayout(mGroupBoxIec104Msg);
    LayoutIec104Msg->addWidget(mLabelIec104Addr, 0, 0);
    LayoutIec104Msg->addWidget(mSpinBoxIec104Addr, 0, 1);
    LayoutIec104Msg->addWidget(mLabelIec104UnMaxSumShow, 1, 0);
    LayoutIec104Msg->addWidget(mSpinBoxIec104UnMaxSum, 1, 1);
    LayoutIec104Msg->addWidget(mLabelIec104UnMaxTimeShow, 2, 0);
    LayoutIec104Msg->addWidget(mSpinBoxIec104UnMaxTime, 2, 1);
    LayoutIec104Msg->addWidget(mLabelIec104SendReasonShow, 3, 0);
    LayoutIec104Msg->addWidget(mComboBoxIec104SendReason, 3, 1);
    LayoutIec104Msg->addWidget(mLabelIec104AppAddrShow, 4, 0);
    LayoutIec104Msg->addWidget(mComboBoxIec104AppAddr, 4, 1);
    LayoutIec104Msg->addWidget(mLabelIec104MsgAddrShow, 5, 0);
    LayoutIec104Msg->addWidget(mComboBoxIec104MsgAddr, 5, 1);
    LayoutIec104Msg->addWidget(mLabelIec104TimeLenShow, 6, 0);
    LayoutIec104Msg->addWidget(mComboBoxIec104TimeLen, 6, 1);
}

void DialogServiceEdit::fCreateModebusRtu()
{
    if(mGroupBoxRtuMsg->layout())
    {
        return;
    }

    qDebug() << "DialogService::fCreateModebusRtu()";

    mLableRtuListShow = TBase::fLabel(tr("遍历节点列表"), tr("mLableRtuListShow"));
    mTableRtuList = new TTableView(tr("mTableRtuList"));
    mTableRtuList->fAddCol(tr("主机"));
    mTableRtuList->fAddCol(tr("功能"));
    mTableRtuList->fAddCol(tr("起始地址"));
    mTableRtuList->fAddCol(tr("寄存器个数"));
    mTableRtuList->fAddCol(tr("数据位数"));
    mTableRtuList->fAddCol(tr("类型"));
    mTableRtuList->fSetHeadLength(0, 80);
    mTableRtuList->fSetHeadLength(1, 80);
    mTableRtuList->fSetHeadLength(2, 100);
    mTableRtuList->fSetHeadLength(3, 150);
    mTableRtuList->fSetHeadLength(4, 100);
    mTableRtuList->fSetHeadLength(5, 80);

    mLabelRtuHostShow = TBase::fLabel(tr("主机"), tr("mLabelRtuHostShow"));
    mSpinBoxRtuHost = TBase::fSpinBox(tr("mSpinBoxRtuHost"), 0, 102400);
    mLabelRtuFunShow = TBase::fLabel(tr("功能"), tr("mLabelRtuFunShow"));
    mSpinBoxRtuFun = TBase::fSpinBox(tr("mSpinBoxRtuFun"), 0, 102400);
    mLabelRtuStartAddrShow = TBase::fLabel(tr("起始地址"), tr("mLabelRtuStartAddrShow"));
    mSpinBoxRtuStartAddr = TBase::fSpinBox(tr("mSpinBoxRtuStartAddr"), 0, 102400);
    mLabelRtuRegCountShow = TBase::fLabel(tr("寄存器个数"), tr("mLabelRtuRegCountShow"));
    mSpinBoxRtuRegCount = TBase::fSpinBox(tr("mSpinBoxRtuRegCount"), 0, 256);
    mLabelRtuDataLenShow = TBase::fLabel(tr("数据位数"), tr("mLabelRtuDataLenShow"));
    mSpinBoxRtuDataLen = TBase::fSpinBox(tr("mSpinBoxRtuDataLen"), 0, 256);
    mLabelRtuTypeShow = TBase::fLabel(tr("类型"), tr("mLabelRtuTypeShow"));
    mSpinBoxRtuType = TBase::fSpinBox(tr("mSpinBoxRtuLen"), 0, 102400);

    mButtonRtuAdd = TBase::fButton(tr("添加"), tr("mButtonRtuAdd"));
    connect(mButtonRtuAdd, SIGNAL(clicked()), this, SLOT(fRtuAdd()));
    mButtonRtuDel = TBase::fButton(tr("删除"), tr("mButtonRtuDel"));
    connect(mButtonRtuDel, SIGNAL(clicked()), this, SLOT(fRtuDel()));

    QVBoxLayout *LayoutTop = new QVBoxLayout();
    LayoutTop->addWidget(mLableRtuListShow);
    LayoutTop->addWidget(mTableRtuList);

    QGridLayout *LayoutMsgLeft = new QGridLayout();
    LayoutMsgLeft->addWidget(mLabelRtuHostShow, 0, 0);
    LayoutMsgLeft->addWidget(mSpinBoxRtuHost, 0, 1);
    LayoutMsgLeft->addWidget(mLabelRtuFunShow, 1, 0);
    LayoutMsgLeft->addWidget(mSpinBoxRtuFun, 1, 1);
    LayoutMsgLeft->addWidget(mLabelRtuStartAddrShow, 2, 0);
    LayoutMsgLeft->addWidget(mSpinBoxRtuStartAddr, 2, 1);

    QGridLayout *LayoutMsgRight = new QGridLayout();
    LayoutMsgRight->addWidget(mLabelRtuRegCountShow, 0, 0);
    LayoutMsgRight->addWidget(mSpinBoxRtuRegCount, 0, 1);
    LayoutMsgRight->addWidget(mLabelRtuDataLenShow, 1, 0);
    LayoutMsgRight->addWidget(mSpinBoxRtuDataLen, 1, 1);
    LayoutMsgRight->addWidget(mLabelRtuTypeShow, 2, 0);
    LayoutMsgRight->addWidget(mSpinBoxRtuType, 2, 1);

    QHBoxLayout *LayoutMsg = new QHBoxLayout();
    LayoutMsg->addLayout(LayoutMsgLeft);
    LayoutMsg->addLayout(LayoutMsgRight);

    QHBoxLayout *LayoutButton = new QHBoxLayout();
    LayoutButton->addWidget(mButtonRtuAdd);
    LayoutButton->addWidget(mButtonRtuDel);

    QVBoxLayout *LayoutMid = new QVBoxLayout();
    LayoutMid->addLayout(LayoutButton);
    LayoutMid->addLayout(LayoutMsg);

    QVBoxLayout *Layout = new QVBoxLayout(mGroupBoxRtuMsg);
    Layout->addLayout(LayoutTop);
    Layout->addLayout(LayoutMid);
}

void DialogServiceEdit::fRtuAdd()
{
    QList<QString>RowData;
    RowData.push_back(QString::number(mSpinBoxRtuHost->value()));
    RowData.push_back(QString::number(mSpinBoxRtuFun->value()));
    RowData.push_back(QString::number(mSpinBoxRtuStartAddr->value()));
    RowData.push_back(QString::number(mSpinBoxRtuRegCount->value()));
    RowData.push_back(QString::number(mSpinBoxRtuDataLen->value()));
    RowData.push_back(QString::number(mSpinBoxRtuType->value()));

    mTableRtuList->fAddRow(RowData);
}

void DialogServiceEdit::fRtuDel()
{
    mTableRtuList->fDelRow(mTableRtuList->fGetCurrentIndex());
}

DialogServiceEdit::~DialogServiceEdit()
{
    if(mService)
    {
        delete mService;
        mService = NULL;
    }
}

void DialogServiceEdit::fSystemChanged(int index)
{
    Q_UNUSED(index);

    QString StartName;

    switch(mComboBoxSystem->currentIndex())
    {
    case 0:
        // Windows
        StartName = tr("COM");
        break;

    case 1:
        // Linux默认
        StartName = tr("ttyS");
        break;

    case 2:
        // Linux定制
        StartName = tr("ttyAT");
        break;

    case 3:
        // Linux1
        StartName = tr("tty");
        break;

    case 4:
        // Linux2
        StartName = tr("ttySAC");
        break;

    default:
        StartName = tr("COM");
        break;
    }

    mLineEditComName->setText(StartName + QString::number(mSpinBoxComNum->value()));
}

void DialogServiceEdit::fPortChanged(int index)
{
    mGroupBoxComMsg->setHidden(true);
    mGroupBoxNetMsg->setHidden(true);
    mGroupBoxClientMsg->setHidden(true);
    mGroupBoxServerMsg->setHidden(true);

    switch(index)
    {
    case TService::Port_Com:
    {
        fCreateCom();

        mGroupBoxComMsg->setHidden(false);

        TComData *ComData = mService->fGetComData();

        if(ComData && mGroupBoxComMsg->layout())
        {
            mLineEditComName->setText(ComData->fGetName());
            mComboBoxSystem->setCurrentIndex(-1);
            mSpinBoxComNum->setValue(0);
            mComboBoxComRate->setCurrentIndex(mComboBoxComRate->findText(QString::number(ComData->fGetRate())));
            mComboBoxComData->setCurrentIndex(mComboBoxComData->findText(QString::number(ComData->fGetDataBits())));
            mComboBoxComStop->setCurrentIndex(ComData->fGetStopBits());
            mComboBoxComCheck->setCurrentIndex(ComData->fGetStopBits());
        }
    }
    break;

    case TService::Port_Net_Client_Tcp:
    case TService::Port_Net_Client_Udp:
    {
        fCreateClient();

        mGroupBoxNetMsg->setHidden(false);
        mGroupBoxClientMsg->setHidden(false);

        TClientData *ClientData = mService->fGetClientData();

        if(ClientData && mGroupBoxNetMsg->layout())
        {
            mLineEditIp->setText(ClientData->fGetIp());
            mSpinBoxNetPort->setValue(ClientData->fGetPort());
        }
    }
    break;

    case TService::Port_Net_Server_Tcp:
    case TService::Port_Net_Server_Udp:
    {
        fCreateServer();

        mGroupBoxNetMsg->setHidden(false);
        mGroupBoxServerMsg->setHidden(false);

        TServerData *ServerData = mService->fGetServerData();

        if(ServerData && mGroupBoxNetMsg->layout() && mGroupBoxServerMsg->layout())
        {
            mLineEditIp->setText(ServerData->fGetIp());
            mSpinBoxNetPort->setValue(ServerData->fGetPort());
            mSpinBoxIoThread->setValue(ServerData->fGetIoThreadSum());
            mSpinBoxProcessThread->setValue(ServerData->fGetProcessThreadSum());
        }
    }
    break;

    default:
        break;
    }
}

void DialogServiceEdit::fAgreeMentChanged(int index)
{
    // 隐藏所有协议相关的组
    if(mGroupBoxIec104Msg)
    {
        mGroupBoxIec104Msg->setHidden(true);
    }

    if(mGroupBoxRtuMsg)
    {
        mGroupBoxRtuMsg->setHidden(true);
    }

    // 只显示当前协议的组
    switch(index)
    {
    case TService::AgreeMent_Iec101:
        break;

    case TService::AgreeMent_Iec104:
    {
        fCreateIec104();
        mGroupBoxIec104Msg->setHidden(false);

        TIec104Data *Iec104Data = mService->fGetIec104Data();

        if(Iec104Data && mGroupBoxIec104Msg->layout())
        {
            mSpinBoxIec104Addr->setValue(Iec104Data->mAddr);
            mSpinBoxIec104UnMaxSum->setValue(Iec104Data->mUnMaxSum);
            mSpinBoxIec104UnMaxTime->setValue(Iec104Data->mUnMaxTime);
            mComboBoxIec104AppAddr->setCurrentIndex(mComboBoxIec104AppAddr->findText(QString::number(Iec104Data->mAppAddr)));
            mComboBoxIec104MsgAddr->setCurrentIndex(mComboBoxIec104MsgAddr->findText(QString::number(Iec104Data->mMsgAddr)));
            mComboBoxIec104SendReason->setCurrentIndex(mComboBoxIec104SendReason->findText(QString::number(Iec104Data->mSendReason)));
            mComboBoxIec104TimeLen->setCurrentIndex(mComboBoxIec104TimeLen->findText(QString::number(Iec104Data->mTimeLen)));
        }
    }
    break;

    case TService::AgreeMent_CDT:
        break;

    case TService::AgreeMent_3761:
        break;

    case TService::AgreeMent_Modebus_RTU:
    {
        fCreateModebusRtu();
        mGroupBoxRtuMsg->setHidden(false);

        TModebusRtuData *ModebusRtuData = mService->fGetModebusRtuData();

        if(ModebusRtuData && mGroupBoxRtuMsg->layout())
        {
            mTableRtuList->fClear();

            foreach(TModebusRtuNode *Node, ModebusRtuData->mList)
            {
                QList<QString>RowData;

                RowData.push_back(QString::number(Node->mHost));
                RowData.push_back(QString::number(Node->mFun));
                RowData.push_back(QString::number(Node->mStartAddr));
                RowData.push_back(QString::number(Node->mRegCount));
                RowData.push_back(QString::number(Node->mDataLen));
                RowData.push_back(QString::number(Node->mType));

                mTableRtuList->fAddRow(RowData);
            }
        }
    }
    break;

    case TService::AgreeMent_SY_GZZS:
        break;

    case TService::AgreeMent_SY_ZNKG:
        break;

    case TService::AgreeMent_SY_REAL:
        break;

    default:
        break;
    }
}

bool DialogServiceEdit::fCheckServiceMsg()
{
    qDebug() << "DialogServiceEdit::fCheckServiceMsg()";
    qDebug() << mLineEditServiceName;
    qDebug() << mComboBoxPort;
    qDebug() << mComboBoxAgreeMent;

    if(mLineEditServiceName->text().length() <= 0
                || mComboBoxPort->currentIndex() < 0
                || mComboBoxAgreeMent->currentIndex() < 0
                )
    {
        return false;
    }
    else
    {
        qDebug() << mComboBoxPort;
        qDebug() << mComboBoxPort->currentIndex();

        switch(mComboBoxPort->currentIndex())
        {
            // 串口
        case TService::Port_Com:
            if(mLineEditComName->text().length() <= 0
                    || mComboBoxComRate->currentIndex() < 0
                    || mComboBoxComData->currentIndex() < 0
                    || mComboBoxComStop->currentIndex() < 0
                    || mComboBoxComCheck->currentIndex() < 0)
            {
                return false;
            }
            else
            {
                return true;
            }
            break;

            // 网络
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Client_Udp:
        case TService::Port_Net_Server_Udp:
            qDebug() << mLineEditIp;
            qDebug() << mSpinBoxNetPort;
            qDebug() << mSpinBoxNetPort->value();
            if(mLineEditIp->text().length() <= 0
                    || mSpinBoxNetPort->value() < 0 || mSpinBoxNetPort->value() > 65535)
            {
                return false;
            }
            else
            {
                return true;
            }
            break;

            default:
                return false;
                break;
        }
    }

    return false;
}

void DialogServiceEdit::fSetServiceMsg()
{
    mService->fSetName(mLineEditServiceName->text());
    qDebug()<<mComboBoxPort->currentIndex();
    mService->fSetPort(mComboBoxPort->currentIndex());
    mService->fSetConnectMode(mComboBoxPortMode->currentIndex());
    mService->fSetRecvMode(mComboBoxRecvMode->currentIndex());
    mService->fSetAgreeMent(mComboBoxAgreeMent->currentIndex());

    qDebug() << "DialogServiceEdit::fSetServiceMsg fGetPort" << mService->fGetPort();

    switch(mService->fGetPort())
    {
    case TService::Port_Com:
    {
        TComData *ComData = mService->fGetComData();
        if(ComData)
        {
            ComData->fSetName(mLineEditComName->text());
            ComData->fSetRate(mComboBoxComRate->currentText().toInt());
            ComData->fSetDataBits(mComboBoxComData->currentText().toInt());
            ComData->fSetStopBits(mComboBoxComStop->currentIndex());
            ComData->fSetCheck(mComboBoxComCheck->currentIndex());
        }
    }
    break;

    case TService::Port_Net_Client_Tcp:
    case TService::Port_Net_Client_Udp:
    {
        TClientData *ClientData = mService->fGetClientData();
        if(ClientData)
        {
            ClientData->fSetIp(mLineEditIp->text());
            ClientData->fSetPort(mSpinBoxNetPort->value());
        }
    }
    break;

    case TService::Port_Net_Server_Tcp:
    case TService::Port_Net_Server_Udp:
    {
        qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSS";
        TServerData *ServerData = mService->fGetServerData();
        if(ServerData)
        {
            qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSS";
            ServerData->fSetIp(mLineEditIp->text());
            ServerData->fSetPort(mSpinBoxNetPort->value());
            ServerData->fSetIoThreadSum(mSpinBoxIoThread->value());
            ServerData->fSetProcessThreadSum(mSpinBoxProcessThread->value());
        }
    }
    break;

    default:
        break;
    }

    qDebug() << "DialogServiceEdit::fSetServiceMsg fGetAgreeMent" << mService->fGetAgreeMent();

    switch(mService->fGetAgreeMent())
    {
    case TService::AgreeMent_Iec104:
    {
        TIec104Data *Iec104Data = mService->fGetIec104Data();

        //qDebug() << "Iec104Data" << Iec104Data;

        if(Iec104Data)
        {
            Iec104Data->mAddr = mSpinBoxIec104Addr->value();
            Iec104Data->mUnMaxSum = mSpinBoxIec104UnMaxSum->value();
            Iec104Data->mUnMaxTime = mSpinBoxIec104UnMaxTime->value();
            Iec104Data->mAppAddr = mComboBoxIec104AppAddr->currentText().toInt();
            Iec104Data->mMsgAddr = mComboBoxIec104MsgAddr->currentText().toInt();
            Iec104Data->mSendReason = mComboBoxIec104SendReason->currentText().toInt();
            Iec104Data->mTimeLen = mComboBoxIec104TimeLen->currentText().toInt();
        }
    }
    break;

    case TService::AgreeMent_Modebus_RTU:
    {
        TModebusRtuData *ModebusRtuData = mService->fGetModebusRtuData();

        if(ModebusRtuData && mGroupBoxRtuMsg->layout())
        {
            // 释放原数据
            ModebusRtuData->fClearHost();

            // 添加新数据
            for(int i = 0; i < mTableRtuList->fGetRowSum(); i++)
            {
                ModebusRtuData->fAddHost(mTableRtuList->fGetCell(i, 0).toInt(),
                                         mTableRtuList->fGetCell(i, 1).toInt(),
                                         mTableRtuList->fGetCell(i, 2).toInt(),
                                         mTableRtuList->fGetCell(i, 3).toInt(),
                                         mTableRtuList->fGetCell(i, 4).toInt(),
                                         mTableRtuList->fGetCell(i, 5).toInt());
            }
        }
    }
    break;

    default:
        break;
    }
}

void DialogServiceEdit::fOk()
{
    if(fCheckServiceMsg())
    {
        mSave = true;
        ipNum=mLineEditIpNum->text().toInt();
        fSetServiceMsg();
        close();
    }
    else
    {
        mLabelResult->setText(tr("<font color=red><b>字段错误，请重新填写</b></font>"));
    }
}
