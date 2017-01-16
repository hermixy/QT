#include "Graphics.h"

#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QTimeEdit>
#include <QLabel>
#ifdef RUNNER
#include "Runer.h"
#endif
#include "../Scada/Dialog/Timer/ttimer.h"

TGraphics::TGraphics(double StartX, double StartY, double StopX, double StopY,
                        const Qt::PenStyle &LineStyle, const int LineWidth,
                        const QColor &LineColor, const QColor &BackColor) :
        TItemEx(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    mValue = 0;
    mCoe = 1;
    mValueUpdateTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    mNodeId = 0;
    mGroupId = 0;
    mServiceId = 0;

    mHint = tr("");
    mLinkScene = 0;

    //mTimer = new QTimer(this);
    //connect(mTimer, SIGNAL(timeout()), this, SLOT(fAutoRunRepeat()));

    mAutoType = AUTO_NONE;
    mAutoTime = 0;
    mLinkageRecv = 0;
    mGroupName=QString();
}

void TGraphics::fCopy(TGraphics *GraphicsFrom)
{
    if(NULL == GraphicsFrom)
    {
        return;
    }

    TItemEx::fCopy(GraphicsFrom);

    this->fSetLinkScene(GraphicsFrom->fGetLinkScene());
    this->fSetGroupId(GraphicsFrom->fGetGroupId());
    this->fSetNodeId(GraphicsFrom->fGetNodeId());

    this->fSetValue(GraphicsFrom->fGetValue());

    this->fSetMainId(GraphicsFrom->fGetMainId());
    this->mUpAlarm=GraphicsFrom->mUpAlarm;
    this->mDownAlarm=GraphicsFrom->mDownAlarm;
    this->greaterEqual=GraphicsFrom->greaterEqual;
    this->lessThan=GraphicsFrom->lessThan;

    this->fSetGroupName(GraphicsFrom->fGetGroupName());
    this->stableValue=GraphicsFrom->stableValue;
    this->startAddr=GraphicsFrom->startAddr;
}

TGraphics::~TGraphics()
{

}

void TGraphics::fSetLinkScene(int LinkId)
{
    mLinkScene = LinkId;
}

int TGraphics::fGetLinkScene() const
{
    return mLinkScene;
}

/*void TGraphics::fRunJs(int key)
{
    TJs *Js = fGetJs(key);

    if(Js)
    {
        Js->fRun(this, tr("fPlay"));
    }
}*/

void TGraphics::fSetAutoType(int Type)
{
    mAutoType = Type;
}

int TGraphics::fGetAutoType()
{
    return mAutoType;
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

void TGraphics::fStartAuto()
{
    if(NULL == mTimer || mAutoTime <= 0)
    {
        return;
    }

    if(mTimer->isActive())
    {
        mTimer->stop();
    }

    if(!mTimer->isActive())
    {
        mTimer->start(mAutoTime);
    }
}

/*void TGraphics::fStartAuto(int Type, int Time)
{
    switch(Type)
    {
    case AUTO_ONCE:
        qDebug() << "fStartAlarm AUTO_ONCE";
        QTimer::singleShot(Time, this, SLOT(fAutoRunOnce()));
        break;

    case AUTO_REPEAT:
        qDebug() << "fStartAlarm AUTO_REPEAT";
        if(mTimer->isActive())
        {
            mTimer->stop();
        }

        if(!mTimer->isActive())
        {
            mTimer->start(Time);
        }

        break;

    default:
        break;
    }
}*/

void TGraphics::fStartAlarm()
{
    //qint64 AlarmTime = mValueUpdateTime;

    //emit mStartAlarm(fGetValue(), AlarmTime);
}

/*void TGraphics::fAutoRunOnce()
{
    qDebug() << "fStartAlarm fAutoRunOnce";
    //fSetValue(mAutoValue);
}*/

void TGraphics::fAutoRunRepeat()
{
    qDebug() << "fStartAlarm fAutoRunRepeat";

    //fRunJs(JS_AUTO);
}

void TGraphics::fSetNodeId(const int Id)
{
    mNodeId = Id;
}

int TGraphics::fGetNodeId() const
{
    return mNodeId;
}
void TGraphics::fSetMainId(const int Id)
{
    mMainId = Id;
}

int TGraphics::fGetMainId() const
{
    return mMainId;
}


void TGraphics::fSetGroupId(const int Id)
{
    mGroupId = Id;
}

int TGraphics::fGetGroupId() const
{
    return mGroupId;
}
QString TGraphics::fGetGroupName() const
{
    return mGroupName;
}
void TGraphics::fSetGroupName(const QString name)
{
    mGroupName=name;
}

bool TGraphics::fGetDisconnect()
{
    return disconnect;
}
void TGraphics::fSetDisconnect(bool disconnect)
{
    this->disconnect=disconnect;
}

/*TJs* TGraphics::fCreateJs(int key)
{
    if(mMapJs.count(key) <= 0)
    {
        mMapJs.insert(key, new TJs());
    }

    return fGetJs(key);
}

TJs* TGraphics::fGetJs(int key)
{
    if(mMapJs.count(key) > 0)
    {
        return mMapJs[key];
    }

    return NULL;
}
*/
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
    setValue=setValue+1;

    int tempValue=mValue;
    mValue = Value;
    if(tempValue!=Value)
        fUpdate();

    if((!startTime.isNull()||!stopTime.isNull()))
    {
        if(!((QTime::currentTime()>=startTime) &&(QTime::currentTime()<=stopTime)))
        {
            return;
        }
    }

    link *Mydata;
    double res=0.0;
    int max=20000, min=-20000;

    //qDebug()<<greaterEqual.size();
    foreach(link data, greaterEqual)
    {
        if(Value>data.currntNum&&!isSetstableValue) {
            if(tempValue!=Value)
                emit sendData(data.groupId, data.writeNum);
        }
        if(qAbs(data.currntNum-Value)<res)
        {
            res=qAbs(data.currntNum-Value);
            Mydata=&data;
        }
        if(data.currntNum>max)
        {
            max=data.currntNum;
        }
    }

    foreach(link data, lessThan)
    {
        if(Value<data.currntNum&&!isSetstableValue) {
            if(tempValue!=Value)
                emit sendData(data.groupId, data.writeNum);
        }
        if(qAbs(data.currntNum-Value)<res)
        {
            res=qAbs(data.currntNum-Value);
            Mydata=&data;
        }
        if(data.currntNum<min)
        {
            min=data.currntNum;
        }
    }
    if(Value>max)
    {
        if(!fGetHint().isEmpty()) {
            if(!isUpAlarm)
            {
            emit mStartAlarm(Value, true, QDateTime::currentDateTime().toMSecsSinceEpoch());
            isUpAlarm = true;
            }
        }
    }
    else
    {
        isALarm=false;
    }
    if(Value<min)
    {
        if(!fGetHint().isEmpty() ) {
            if(!isDownAlarm)
            {
            emit mStartAlarm(Value, true, QDateTime::currentDateTime().toMSecsSinceEpoch());
            isDownAlarm = true;
            }
        }
    }
    else
    {
        isDownAlarm=false;
    }
    if(qAbs(Value-stableValue)>1&&isSetstableValue)
    {
        emit sendData(Mydata->groupId, Mydata->writeNum);
    }
}
double TGraphics::fGetValue() const
{
    return mValue;
}

/*void TGraphics::fUpdateValue(const double Value, qint64 Time)
{
    double ValueChange = Value * mCoe - mValue;
    //qDebug() << "TGraphics::fUpdateValue" << mValue << Value;
    if(ABS(ValueChange) > 0.000001)
    {
        fSetValue(Value);

        mValueUpdateTime = Time;

        // 向外发送值变化的信号，用于数据库等其他操作
        //emit mValueChanged(mServiceId, mNodeId, mValue, mValueUpdateTime, ValueChange);
    }

    //emit mValueCome(mServiceId, mNodeId, Value * mCoe, Time);
}*/


/*int TGraphics::fGetLinkageRecv()
{
    return mLinkageRecv;
}

void TGraphics::fSendLinkage(bool Enable, int _iLinkage)
{
    //qDebug() << "emit mSendLinkage(mLinkageSend)" << mLinkageSend;
    emit mSendLinkage(Enable, _iLinkage);
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

    // 调用脚本
    fRunJs(JS_VALUE);
}
*/
void TGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "in mousePressEvent" << mLinkScene;

    if(mLinkScene > 0)
    {
        emit mGotoLink(mLinkScene);
    }

    TItem::mousePressEvent(event);
}

void TGraphics::setStableValue()
{
    bool ok;
    double d =QInputDialog::getDouble(nullptr,tr("输入参考值"), tr("数值"), stableValue,-20000, 20000, 3, &ok);
    if(ok)
    {
        stableValue=d;
        isSetstableValue=true;
        qDebug()<<d;
    }
}
void TGraphics::setLinkStart()
{
#ifdef RUNNER
    TTimer *time=new TTimer(0);
    time->exec();
    if((time->startTime->time()==QTime(0, 0, 0))&&(time->stopTime->time()==QTime(0, 0, 0)))
    {

    }else {
        this->startTime= time->startQTime;
        this->stopTime=time->stopQTime;
    }
    delete time;
#endif
}
void TGraphics::setpercent()
{
    qDebug()<<"sssssssssssssssssss";
 #ifdef RUNNER
    bool ok;
    double d =QInputDialog::getDouble(nullptr,tr("输入百分比"), tr("百分比"), 0,-20000, 20000, 1, &ok);

    TButton::Control contral;
    if(!Runer::runner)
        return;
    foreach(TService *server, Runer::runner->mServiceList) {
        if(server->fGetId()==fGetServiceId()) {
            contral.servername=server->fGetName();
            contral.mainId=fGetMainId();
            contral.devId=fGetNodeId();
            contral.startAddr=this->startAddr;
            contral.writeNum=Runer::runner->swapShowNum(this->showStart, this->showStop, d);
            emit sendContral(contral);
            return;
    }
    }
#endif
}
