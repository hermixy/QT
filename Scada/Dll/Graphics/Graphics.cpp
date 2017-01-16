#include "Graphics.h"

#include "../../Src/Base/LibBase.h"
#include <QDebug>

TGraphics::TGraphics(double StartX, double StartY, double StopX, double StopY,
        const Qt::PenStyle &LineSyle, const int LineWidth,
        const QColor &LineColor, const QColor &BackColor) :
        TItem(StartX, StartY, StopX, StopY, LineSyle, LineWidth, LineColor, BackColor)
{
    mValue = 0;
    mCoe = 1;
    mValueUpdateTime = 0;

    mNodeId = 0;
    mServiceId = 0;

    mHint = tr("");
    mLinkScene = 0;

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(fAutoRunRepeat()));

    mAutoType = AUTO_NONE;
    mAutoTime = 0;
    mAutoValue = 0;

    mAlarmEnable = true;

    mLinkageSend = 0;
    mLinkageActive = 0;
    mLinkageRecv = 0;
}

void TGraphics::fMsgCopy(QObject *To, QObject *From)
{
    TItem::fMsgCopy(To, From);

    TGraphics *GraphicsTo = dynamic_cast<TGraphics *>(To);
    TGraphics *GraphicsFrom = dynamic_cast<TGraphics *>(From);

    if(NULL == GraphicsTo || NULL == GraphicsFrom)
    {
        return;
    }

    GraphicsTo->fSetLinkScene(GraphicsFrom->fGetLinkScene());

    GraphicsTo->fSetServiceID(GraphicsFrom->fGetServiceID());
    GraphicsTo->fSetNodeID(GraphicsFrom->fGetNodeID());
    GraphicsTo->fSetValue(GraphicsFrom->fGetValue());

    GraphicsTo->fSetAlarmEnable(GraphicsFrom->fGetAlarmEnable());
    GraphicsTo->fSetAutoType(GraphicsFrom->fGetAutoType());
    GraphicsTo->fSetAutoTime(GraphicsFrom->fGetAutoTime());
    GraphicsTo->fSetAutoValue(GraphicsFrom->fGetAutoValue());

    GraphicsTo->fSetHint(GraphicsFrom->fGetHint());

    GraphicsTo->mJsValue.fSetDir(GraphicsFrom->mJsValue.fGetDir());
    GraphicsTo->mJsValue.fSetName(GraphicsFrom->mJsValue.fGetName());
    GraphicsTo->mJsAlarm.fSetDir(GraphicsFrom->mJsAlarm.fGetDir());
    GraphicsTo->mJsAlarm.fSetName(GraphicsFrom->mJsAlarm.fGetName());
    GraphicsTo->mJsLinkageSend.fSetDir(GraphicsFrom->mJsLinkageSend.fGetDir());
    GraphicsTo->mJsLinkageSend.fSetName(GraphicsFrom->mJsLinkageSend.fGetName());
    GraphicsTo->mJsLinkageActive.fSetDir(GraphicsFrom->mJsLinkageActive.fGetDir());
    GraphicsTo->mJsLinkageActive.fSetName(GraphicsFrom->mJsLinkageActive.fGetName());
}

TGraphics::~TGraphics()
{}

void TGraphics::fSetLinkScene(int LinkId)
{
    mLinkScene = LinkId;
}

int TGraphics::fGetLinkScene() const
{
    return mLinkScene;
}

void TGraphics::fRunJsValue()
{
    // 执行值变化脚本，改变外形
    mJsValue.fRun(this, tr("fPlay"));
}

void TGraphics::fRunJsAlarm()
{
    //qDebug() << "mAlarmEnable" << mAlarmEnable;
    // 执行报警脚本，判断是否启动报警
    if(mAlarmEnable)
    {
        mJsAlarm.fRun(this, tr("fAlarm"));
    }
}

void TGraphics::fStartAuto()
{
    switch(mAutoType)
    {
    case AUTO_ONCE:
        qDebug() << "fStartAlarm AUTO_ONCE";
        QTimer::singleShot(mAutoTime, this, SLOT(fAutoRunOnce()));
        break;

    case AUTO_REPEAT:
        qDebug() << "fStartAlarm AUTO_REPEAT";
        if(mTimer->isActive())
        {
            mTimer->stop();
        }

        if(!mTimer->isActive())
        {
            mTimer->start(mAutoTime);
        }

        break;

    default:
        break;
    }
}

void TGraphics::fRunJsLinkageSend()
{
    // 执行联动启动条件判断
    if(fGetLinkageSend() > 0)
    {
        mJsLinkageSend.fRun(this, tr("fSrc"));
    }
}

void TGraphics::fRunJsLinkageActive()
{
    mJsLinkageActive.fRun(this, tr("fDst"));
}

void TGraphics::fStartAlarm()
{
    qint64 AlarmTime = mValueUpdateTime;

    //qDebug() << "emit mStartAlarm" << fGetValue() << AlarmTime;

    emit mStartAlarm(fGetValue(), AlarmTime);

    // 判断是否执行复归
    fStartAuto();
}

void TGraphics::fSetAlarmEnable(bool Enable)
{
    mAlarmEnable = Enable;
}

bool TGraphics::fGetAlarmEnable()
{
    return mAlarmEnable;
}

void TGraphics::fAutoRunOnce()
{
    qDebug() << "fStartAlarm fAutoRunOnce" << mAutoValue;
    fSetValue(mAutoValue);
}

void TGraphics::fAutoRunRepeat()
{
    qDebug() << "fStartAlarm fAutoRunRepeat" << mAutoValue;
    fSetValue(mAutoValue);
}

void TGraphics::fSetAutoTime(int Time)
{
    if(Time >= 0)
    {
        mAutoTime = Time;
    }
}

int TGraphics::fGetAutoTime()
{
    return mAutoTime;
}

void TGraphics::fSetAutoValue(double Value)
{
    mAutoValue = Value;
}

double TGraphics::fGetAutoValue()
{
    return mAutoValue;
}

void TGraphics::fSetAutoType(int Type)
{
    if(Type >= AUTO_NONE && Type <= AUTO_REPEAT)
    {
        mAutoType = Type;
    }
}

int TGraphics::fGetAutoType()
{
    return mAutoType;
}

void TGraphics::fSetServiceID(const int ID)
{
    mServiceId = ID;
}

int TGraphics::fGetServiceID() const
{
    return mServiceId;
}

void TGraphics::fSetNodeID(const int ID)
{
    mNodeId = ID;
}

int TGraphics::fGetNodeID() const
{
    return mNodeId;
}

void TGraphics::fSetCoe(const double Coe)
{
    mCoe = Coe;
}

double TGraphics::fGetCoe() const
{
    return mCoe;
}

void TGraphics::fSetValue(const double Value)
{
    mValue = Value;

    fRunJsValue();
    fRunJsLinkageSend();
}

void TGraphics::fUpdateValue(const double Value, qint64 Time)
{
    //qDebug() << "TGraphics::fUpdateValue" << mValue << Value;
    if(ABS(mValue - Value) > 0.000001)
    {
        fSetValue(Value);

        mValueUpdateTime = Time;

        // 向外发送值变化的信号，用于数据库等其他操作
        emit mValueChanged(mValue, mValueUpdateTime);

        fRunJsAlarm();
    }
}

void TGraphics::fRecvData(int NodeId, double Value, quint64 Time)
{
    //qDebug() << "TGraphics::fRecvData" << QThread::currentThread();
    //qDebug() << "TGraphics::fRecvData****" << NodeId << Value;

    if(NodeId == mNodeId)
    {
        //qDebug() << "TGraphics::fRecvData" << NodeId << Value;
        fUpdateValue(Value, Time);
    }
}

double TGraphics::fGetValue() const
{
    return mValue;
}

void TGraphics::fSetHint(QString Hint)
{
    mHint = Hint;
}

QString TGraphics::fGetHint()
{
    return mHint;
}

void TGraphics::fSetLinkageSend(int _iLinkage)
{
    mLinkageSend = _iLinkage;
}

int TGraphics::fGetLinkageSend()
{
    return mLinkageSend;
}

void TGraphics::fSetLinkageActive(int _iLinkage)
{
    mLinkageActive = _iLinkage;
}

int TGraphics::fGetLinkageActive()
{
    return mLinkageActive;
}

int TGraphics::fGetLinkageRecv()
{
    return mLinkageRecv;
}

void TGraphics::fSendLinkage(bool Enable)
{
    //qDebug() << "emit mSendLinkage(mLinkageSend)" << mLinkageSend;
    emit mSendLinkage(Enable, mLinkageSend);
}

void TGraphics::fRecvLinkage(bool Enable, int _iLinkage)
{
    //qDebug() << "TGraphics::fRecvLinkage" << Enable << _iLinkage;

    if(Enable)
    {
        mLinkageRecv |= _iLinkage;
    }
    else
    {
        mLinkageRecv &= (~_iLinkage);
    }

    //qDebug() << "_iLinkage" << _iLinkage << "mLinkageRecv" << mLinkageRecv << "mLinkageActive" << mLinkageActive;

    // 达到条件，调用脚本
    if(0 != mLinkageActive/* && mLinkageRecv == mLinkageActive*/)
    {
        //mLinkageRecv = 0;

        fRunJsLinkageActive();
    }
}

void TGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "in mousePressEvent" << mLinkScene;

    if(mLinkScene > 0)
    {
        emit mGotoLink(mLinkScene);
    }

    TItem::mousePressEvent(event);
}
