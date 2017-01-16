#include "GongConfig/DialogGongConfig.h"
#include "../Scada/Dialog/ComConfig/DialogComConfig.h"
#include "../Scada/Src/Protocol/Modbus/Modbus.h"
#include "gong.h"

#include <QDebug>

Gong::Gong(QWidget *parent) : QMainWindow(parent), mPi(3.141592653), mRadius(400)
{
    mClock = NULL;
    mModbus = NULL;
    mConnection = NULL;
    mThreadConnection = NULL;

    QWidget *mWidget = new QWidget(this);
    mWidget->setObjectName(QStringLiteral("mWidget"));
    setCentralWidget(mWidget);    

    mView = new TView();
    mView->fZoom(0.7);
    //mView->setFixedSize(700, 400);
    mView->setObjectName(tr("mView"));
    //mView->setFixedSize(1000, 610);

    mScene = new TScene(-400, -500, 800, 500);
    mScene->setBackgroundBrush(Qt::black);
    mView->setScene(mScene);

    mZuan = new TRect(-4, -470, 4, -462, Qt::SolidLine, 1, Qt::red, Qt::red);
    mGan = new TLine(0, -462, 0, -370, Qt::SolidLine, 6, Qt::blue, Qt::blue);
    mHuaKuai = new TRect(-10, -410, 10, -390, Qt::SolidLine, 1, Qt::blue, Qt::blue);
    mHuaKuaiXiao = new TRect(-15, -370, 15, -350, Qt::SolidLine, 1, Qt::blue, Qt::blue);

    mSheBei = new TGroup();
    mSheBei->fAddItem(mHuaKuai);
    mSheBei->fAddItem(mGan);
    mSheBei->fAddItem(mZuan);
    mSheBei->fAddItem(mHuaKuaiXiao);
    mSheBei->setZValue(2);
    mSheBei->fSetMoveAble(false);
    mSheBei->fSetSelectAble(false);

    mGong = new TArc(-400, -400, 400, 400, Qt::SolidLine, 15, Qt::white, Qt::white);
    mGong->fSetMode(TArc::MODE_ARC);
    mGong->fSetStartAngle(10);
    mGong->fSetCoverAngle(160);
    mGong->setZValue(1);
    mGong->fSetMoveAble(false);
    mGong->fSetDragAble(false);
    mGong->fSetSelectAble(false);
    mGongXiao = new TArc(-360, -360, 360, 360, Qt::SolidLine, 15, Qt::white, Qt::white);
    mGongXiao->fSetMode(TArc::MODE_ARC);
    mGongXiao->fSetStartAngle(10);
    mGongXiao->fSetCoverAngle(160);
    mGongXiao->setZValue(1);
    mGongXiao->fSetMoveAble(false);
    mGongXiao->fSetDragAble(false);
    mGongXiao->fSetSelectAble(false);

    ////////////////////////////
    mTextLength = new TText(-400, -500, -380, -480, tr("给进距离：150mm"), Qt::SolidLine, 1, Qt::white, Qt::black);
    mTextLength->fSetFontSize(20);
    mTextLength->setZValue(10);
    mTextSpeed = new TText(-400, -470, -360, -450, tr("给进速度：100mm/h"), Qt::SolidLine, 1, Qt::white, Qt::black);
    mTextSpeed->fSetFontSize(20);
    mTextSpeed->setZValue(10);

    mScene->addItem(mSheBei);
    mScene->addItem(mGong);
    mScene->addItem(mGongXiao);
    mScene->addItem(mTextLength);
    mScene->addItem(mTextSpeed);

    ///////////////////////////////////////////////
    mLabelAngle = TBase::fLabel(tr("当前角度"), tr("mLabelAngle"));
    mLineEditAngle = TBase::fLineEdit(tr("mLineEditAngle"));
    mLineEditAngle->setText(tr("0"));
    mLabelLength1 = TBase::fLabel(tr("当前位移1"), tr("mLabelLength1"));
    mLabelLength2 = TBase::fLabel(tr("当前位移2"), tr("mLabelLength2"));
    mLineEditLength1 = TBase::fLineEdit(tr("mLineEditLength1"));
    mLineEditLength1->setText(tr("0.0001"));
    mLineEditLength2 = TBase::fLineEdit(tr("mLineEditLength2"));
    mLineEditLength2->setText(tr("0.0002"));
    mLabelStepAngleShow = TBase::fLabel(tr("角度步长"), tr("mLabelStepAngleShow"));
    mSpinBoxAngleStep = TBase::fDoubleSpinBox(tr("mSpinBoxAngleStep"), -80.0, 80.0);
    mSpinBoxAngleStep->setValue(1);
    connect(mSpinBoxAngleStep, SIGNAL(valueChanged(double)), this, SLOT(fStepAngleChanged(double)));

    mLabelStepAngle = TBase::fLabel(tr("度"), tr("mLabelStepAngle"));
    mLabelStepLengthShow = TBase::fLabel(tr("位移步长"), tr("mLabelStepLengthShow"));
    mSpinBoxLengthStep = TBase::fDoubleSpinBox(tr("mSpinBoxAngleStep"), -12000.0, 12000.0);
    mSpinBoxLengthStep->setValue(6.98);
    connect(mSpinBoxLengthStep, SIGNAL(valueChanged(double)), this, SLOT(fStepLengthChanged(double)));

    mLabelStepLength = TBase::fLabel(tr("米"), tr("mLabelStepLength"));

    mButtonLeft = TBase::fButton(tr(""), tr("mButtonLeft"), 80, 80);
    mButtonLeft->setIcon(QIcon(tr(":/root/pic/left.png")));
    mButtonLeft->setIconSize(QSize(80, 80));
    connect(mButtonLeft, SIGNAL(clicked()), this, SLOT(fDoStep()));
    mButtonRight = TBase::fButton(tr(""), tr("mButtonRight"), 80, 80);
    mButtonRight->setIcon(QIcon(tr(":/root/pic/right.png")));
    mButtonRight->setIconSize(QSize(80, 80));
    connect(mButtonRight, SIGNAL(clicked()), this, SLOT(fDoStep()));

    mButtonStop = TBase::fButton(tr(""), tr("mButtonStop"), 80, 80);
    mButtonStop->setIcon(QIcon(tr(":/root/pic/stop.png")));
    mButtonStop->setIconSize(QSize(80, 80));
    connect(mButtonStop, SIGNAL(clicked()), this, SLOT(fStop()));

    mLight = new TLightView(80, 80, this);
    mLight->fSetMode(TLightView::ModeNormal);
    mLabelAlarm = TBase::fLabel(tr("报警信息："), tr("mLabelAlarm"));

    mButtonStart = TBase::fButton(tr(""), tr("mButtonStart"));
    mButtonStart->setIcon(QIcon(tr(":/root/pic/start.png")));
    mButtonStart->setIconSize(QSize(80, 80));
    connect(mButtonStart, SIGNAL(clicked()), this, SLOT(fStart()));

    QGridLayout *mLayout1 = new QGridLayout();
    mLayout1->addWidget(mLabelAngle, 0, 0);
    mLayout1->addWidget(mLineEditAngle, 0, 1);
    mLayout1->addWidget(mLabelLength1, 1, 0);
    mLayout1->addWidget(mLineEditLength1, 1, 1);
    mLayout1->addWidget(mLabelLength2, 2, 0);
    mLayout1->addWidget(mLineEditLength2, 2, 1);

    QGridLayout *mLayout2 = new QGridLayout();
    mLayout2->addWidget(mButtonLeft, 0, 0);
    mLayout2->addWidget(mButtonRight, 0, 1);
    //mLayout2->addWidget(mButtonStop, 0, 2);

    QGridLayout *mLayout3 = new QGridLayout();
    mLayout3->addWidget(mLabelStepAngleShow, 0, 0);
    mLayout3->addWidget(mSpinBoxAngleStep, 0, 1);
    mLayout3->addWidget(mLabelStepAngle, 0, 2);
    mLayout3->addWidget(mLabelStepLengthShow, 1, 0);
    mLayout3->addWidget(mSpinBoxLengthStep, 1, 1);
    mLayout3->addWidget(mLabelStepLength, 1, 2);

    QHBoxLayout *mLayoutHandle = new QHBoxLayout();
    mLayoutHandle->addStretch();
    mLayoutHandle->addWidget(mButtonStart);
    mLayoutHandle->addLayout(mLayout1);
    mLayoutHandle->addSpacing(20);
    mLayoutHandle->addLayout(mLayout2);
    mLayoutHandle->addSpacing(20);
    mLayoutHandle->addLayout(mLayout3);
    mLayoutHandle->addSpacing(20);
    mLayoutHandle->addWidget(mButtonStop);
    mLayoutHandle->addWidget(mLight);
    mLayoutHandle->addWidget(mLabelAlarm);
    mLayoutHandle->addStretch();
    //////////////////////////////////////////////////////////////////////////
    mScale1 = new TScale(60, 200, this);
    //mScale1->fSetText(tr("温度："), tr("℃"));
    //mScale1->fSetValue(45);

    mScale2 = new TScale(60, 200, this);
    //mScale2->fSetText(tr("湿度："), tr("%"));
    //mScale2->fSetValue(30);

    mScale3 = new TScale(60, 200, this);
    //mScale3->fSetText(tr("压力："), tr("Mpa"));
    //mScale3->fSetValue(20);

    mScale4 = new TScale(60, 200, this);
    //mScale4->fSetText(tr("扭矩："), tr("NM"));
    //mScale4->fSetValue(80);

    //QLabel *mLabelSpace1 = TBase::fLabel(tr("123456"), tr("mLabelSpace1"), 50, 50);

    QHBoxLayout *mLayoutBarTop = new QHBoxLayout();
    mLayoutBarTop->addWidget(mScale1);
    mLayoutBarTop->addSpacing(10);
    mLayoutBarTop->addWidget(mScale2);
    mLayoutBarTop->addSpacing(10);

    QHBoxLayout *mLayoutBarButtom = new QHBoxLayout();
    mLayoutBarButtom->addWidget(mScale3);
    mLayoutBarButtom->addSpacing(10);
    mLayoutBarButtom->addWidget(mScale4);
    mLayoutBarButtom->addSpacing(10);

    QVBoxLayout *mLayoutLeft = new QVBoxLayout();
    mLayoutLeft->addLayout(mLayoutBarTop);
    mLayoutLeft->addLayout(mLayoutBarButtom);

    //////////////////////////////////////////////////////////////////////////
    //mLableZuanLength = TBase::fLabel(tr("给进长度："), tr("mLableZuanLength"));
    //mLableZuanSpeed = TBase::fLabel(tr("给进速度："), tr("mLableZuanSpeed"));

    //QGridLayout *mLayoutTop = new QGridLayout();
    //mLayoutTop->addWidget(mLableZuanLength, 0, 0);
    //mLayoutTop->addWidget(mLableZuanSpeed, 1, 0);

    //////////////////////////////////////////////////////////////////////////
    mClock = new TClock(TClock::SetTime, 150, 150);
    mClock->fSetPointerWidth(5);
    mClock->fSetTime(3, 15, 25);
    mClock->fSetPointerVisible(false, true, false);
    mClock->fSetCalibrationVisible(true, false);

    mLableState = TBase::fLabel(tr("当前状态："), tr("mLableState"));

    QGridLayout *mLayoutRight = new QGridLayout();
    mLayoutRight->addWidget(mClock, 0, 0);
    mLayoutRight->addWidget(mLableState, 1, 0);
    //////////////////////////////////////////////////////////////////////////
    QGridLayout *mLayoutMain = new QGridLayout(mWidget);
    //mLayoutMain->addLayout(mLayoutTop, 0, 1);
    mLayoutMain->addLayout(mLayoutLeft, 0, 0);
    mLayoutMain->addWidget(mView, 0, 1);
    mLayoutMain->addLayout(mLayoutRight, 0, 2);
    mLayoutMain->addLayout(mLayoutHandle, 1, 0, 1, 3);

    fCreateMenu();
    setWindowTitle(tr("智能钻机系统"));

    mCurrentAngle = 0;

    mCom = NULL;
    mComData = new TComData();
    mComData->mName = tr("COM3");
    mComData->mRate = 9600;
    mComData->mData = 8;
    mComData->mStop = 2;
    mComData->mCheck = 0;

    TGongConfig::fGetConfigFromFile(QApplication::applicationDirPath() + "/gong.csv", mConfigList);
    fUpdateWidgets();
}

Gong::~Gong()
{
    if(mClock)
    {
        delete mClock;
        mClock = NULL;
    }

    if(mModbus)
    {
        delete mModbus;
        mModbus = NULL;
    }

    if(mConnection)
    {
        delete mConnection;
        mConnection = NULL;
    }

    if(mThreadConnection)
    {
        if(mThreadConnection->isRunning())
        {
            mThreadConnection->fStop();
        }

        mThreadConnection->deleteLater();
        mThreadConnection = NULL;
    }
}

TAction* Gong::fCreateAction(const QString Text)
{
    TAction *Action = new TAction(Text, this);

    return Action;
}

void Gong::fCreateMenu()
{
    mMenuBar = new QMenuBar(this);
    mMenuBar->setObjectName(tr("mMenuBar"));
    setMenuBar(mMenuBar);

    mUserMenu = mMenuBar->addMenu(tr("用户(&U)"));
    mActionLogin = fCreateAction(tr("登录"));
    mUserMenu->addAction(mActionLogin);
    mActionUser = fCreateAction(tr("用户管理"));
    mUserMenu->addAction(mActionUser);
    mActionPermission = fCreateAction(tr("权限管理"));
    mUserMenu->addAction(mActionPermission);
    mActionDuty = fCreateAction(tr("职责管理"));
    mUserMenu->addAction(mActionDuty);

    mConfigMenu = mMenuBar->addMenu(tr("配置(&C)"));
    mActionConfig = fCreateAction(tr("配置管理"));
    connect(mActionConfig, SIGNAL(triggered()), this, SLOT(fConfig()));
    mConfigMenu->addAction(mActionConfig);

    mActionConfigCom = fCreateAction(tr("配置串口"));
    connect(mActionConfigCom, SIGNAL(triggered()), this, SLOT(fConfigCom()));
    mConfigMenu->addAction(mActionConfigCom);
}

void Gong::fConfigCom()
{
    DialogComConfig *mComConfig = new DialogComConfig(mComData, this);

    if(mComConfig)
    {
        mComConfig->exec();

        delete mComConfig;
    }
}

void Gong::fUpdateWidgets()
{
    foreach(TGongConfig *Config, mConfigList)
    {
        TScale *Scale = NULL;

        switch(Config->mId)
        {
        case 1:
            Scale = mScale1;
            break;

        case 2:
            Scale = mScale2;
            break;

        case 3:
            Scale = mScale3;
            break;

        case 4:
            Scale = mScale4;
            break;

        default:
            break;
        }

        if(Scale)
        {
            Scale->fSetText(Config->mName, Config->mUnit);
            Scale->fSetRange(Config->mZero, Config->mFull, Config->mRange);
        }
    }
}

void Gong::fUpdateModebus()
{
    // 更新协议中遍历列表
    if(mModbus)
    {
        mModbus->fClearHost();

        foreach(TGongConfig *Config, mConfigList)
        {
            //qDebug() << "add" << Config->mAddr << Config->mChan;
            mModbus->fAddHost(Config->mAddr, Config->mChan);
        }
    }
}

void Gong::fConfig()
{
    DialogConfigGong *DialogConfig = new DialogConfigGong(mConfigList, this);

    if(DialogConfig)
    {
        DialogConfig->exec();

        if(DialogConfig->mSave)
        {
            foreach(TGongConfig *Config, mConfigList)
            {
                mConfigList.removeOne(Config);
                delete Config;
            }

            foreach(TGongConfig *Config, DialogConfig->mList)
            {
                mConfigList.push_back(new TGongConfig(Config));
            }

            // 更新界面控件
            fUpdateWidgets();

            // 更新协议中遍历列表
            fUpdateModebus();
        }

        delete DialogConfig;
    }
}

void Gong::fStepAngleChanged(double Angle)
{
    if(focusWidget() != mSpinBoxLengthStep)
    {
        mSpinBoxLengthStep->setValue(2 * mPi * mRadius * Angle / 360);
    }
}

void Gong::fStepLengthChanged(double Length)
{
    if(focusWidget() != mSpinBoxAngleStep)
    {
        mSpinBoxAngleStep->setValue(Length / (2 * mPi * mRadius) * 360);
    }
}

void Gong::fDoStep()
{
    QPushButton *Button = (QPushButton *)sender();

    int Direction = Button == mButtonLeft ? -1 : 1;
    double StepAngle = mSpinBoxAngleStep->value();
    double DoAngle = (Direction * StepAngle);

    mCurrentAngle += DoAngle;

    if(mCurrentAngle < -80 || mCurrentAngle > 80)
    {
        mCurrentAngle -= DoAngle;
        return;
    }

    mLineEditAngle->setText(QString::number(mCurrentAngle));

    QTransform transform(mSheBei->transform());

    //qDebug() << "fDoStep()" << DoAngle;

    transform.rotate(DoAngle);
    mSheBei->setTransform(transform);
}

void Gong::fStart()
{
    if(NULL == mCom)
    {
        mCom = new TCom(this);
        mConnection = mCom;
    }

    if(TConnection::STATE_RUNING == mCom->fGetState())
    {
        mCom->fStop();
    }

    qDebug() << "Gong::fStart" << mComData->mName << mComData->mRate
                << mComData->mData << mComData->mStop << mComData->mCheck;

    mCom->fSetPortName(mComData->mName);

    mCom->fSetMode(TCom::MODE_RTU);

    mCom->fSetBaudRate(mComData->mRate);
    mCom->fSetDataBits(mComData->mData);
    mCom->fSetStopBits(mComData->mStop);
    mCom->fSetParity(mComData->mCheck);

    mCom->fStart();

    if(TConnection::STATE_RUNING == mCom->fGetState())
    {
        mLight->fSetMode(TLightView::ModeAlarm);
    }

    if(NULL == mModbus)
    {
        mModbus = new TModbus(NULL);

        if(mModbus && mConnection)
        {
            fUpdateModebus();

            mThreadConnection = new TThreadConnection(mModbus, mConnection, this);

            connect(mModbus, SIGNAL(mDataCome(int,double,quint64)), this, SLOT(fDataCome(int,double,quint64)));
        }
    }

    if(mThreadConnection)
    {
        mThreadConnection->start();
    }
}

void Gong::fDataCome(int NodeId, double Value, quint64 Time)
{
    Q_UNUSED(Time);

    qDebug() << "Gong::fDataCome" << NodeId << Value;

    switch(NodeId & 0xFFFF)
    {
    case 0x0400:
        mScale1->fSetValue(Value);
        break;

    case 0x0401:
        mScale2->fSetValue(Value);
        break;

    case 0x0402:
        mScale3->fSetValue(Value);
        break;

    case 0x0403:
        mScale4->fSetValue(Value);
        break;

    case 0x0300:
        mLineEditLength1->setText(QString::number(Value));
        break;

    case 0x0301:
        mLineEditLength2->setText(QString::number(Value));
        break;

    default:
        break;
    }
}

void Gong::fStop()
{
    //qDebug() << mConnection << mLight;

    if(mConnection)
    {
        mConnection->fStop();
    }

    if(mLight)
    {
        mLight->fSetMode(TLightView::ModeNormal);
    }

    if(mThreadConnection)
    {
        mThreadConnection->fStop();
    }

}
