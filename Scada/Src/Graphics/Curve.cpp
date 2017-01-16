#include "Curve.h"

TCurve::TCurve(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Curve);

    mDataSum = 1024;
    mInterval = 1;
    mTime = 5;

    //mRingData = new TRingBuf(1024);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(fUpdate()));
    mTimer->start(mTime);

    /*for(int i = 1; i <= 1000; i++)
    {
        TDataPoint *Point = new TDataPoint();
        Point->mValue = i;

        mDataList.push_back(Point);
    }*/
}

TCurve::~TCurve()
{}

TItem *TCurve::fCopy()
{
    TCurve *Curve = new TCurve();

    Curve->fCopy(this);

    return Curve;
}

void TCurve::fCopy(TCurve *CurveFrom)
{
    if(NULL == CurveFrom)
    {
        return;
    }

    TGraphics::fCopy(CurveFrom);

    this->fSetDataSum(CurveFrom->fGetDataSum());
    this->fSetInterval(CurveFrom->fGetInterval());
    this->fSetTime(CurveFrom->fGetTime());
}

void TCurve::fSetDataSum(int DataSum)
{
    mDataSum = DataSum;
}

int TCurve::fGetDataSum()
{
    return mDataSum;
}

void TCurve::fSetInterval(int Interval)
{
    mInterval = Interval;
}

int TCurve::fGetInterval()
{
    return mInterval;
}

void TCurve::fSetTime(int Time)
{
    mTime = Time;

    if(mTimer->isActive())
    {
        mTimer->stop();
    }

    mTimer->start(mTime);
}

int TCurve::fGetTime()
{
    return mTime;
}

/*void TCurve::fUpdateValue(const double Value, qint64 Time)
{
    Q_UNUSED(Time);
    //TGraphics::fUpdateValue(Value, Time);

    if(mDataSum <= 0)
    {
        return;
    }

    //qDebug() << "TCurve::fUpdateValue" << mDataList.length();

    TDataPoint *Point = NULL;

    if(mDataList.length() >= mDataSum)
    {
        Point = mDataList.first();
        mDataList.removeFirst();
    }
    else
    {
        Point = new TDataPoint();
    }

    Point->mValue = Value;

    mDataList.push_back(Point);
}*/

void TCurve::fDraw(QPainter *painter)
{
    if(mDataSum <= 0)
    {
        return;
    }

    if(mDataList.isEmpty())
    {
        return;
    }

    //qDebug() << "TCurve::paint" << mDataList.length();

    double Len = (mStopX - mStartX) / mDataSum;
    double Height = mStartY + (mStopY - mStartY) / 2;

#if true
    for(int i = mInterval; i < mDataList.length(); i += mInterval)
    {
        //qDebug() << tr("draw point:%1/%2").arg(i).arg(mDataList.length()) << mDataList.at(i)->mValue;

        painter->drawLine(Len * (i - mInterval) + mStartX,
                          mDataList.at(i - mInterval)->mValue + Height,
                          Len * i + mStartX,
                          mDataList.at(i)->mValue + Height);
    }
#else
    QList<TDataPoint *>::const_iterator it = mDataList.constBegin();
    const TDataPoint *last = *it++;
    int i = 1;

    while(it != mDataList.constEnd())
    {
        painter->drawLine(Len * (i - 1) + mStartX,
                          last->mValue + Height,
                          Len * i + mStartX,
                          (*it)->mValue + Height);

        i++;
        last = *it++;
    }
#endif
}

/*void TCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget);

    if(mDataSum <= 0)
    {
        return;
    }

    if(mDataList.isEmpty())
    {
        return;
    }

    //qDebug() << "TCurve::paint" << mDataList.length();

    double Len = (mStopX - mStartX) / mDataSum;
    double Height = mStartY + (mStopY - mStartY) / 2;

#if true
    for(int i = mInterval; i < mDataList.length(); i += mInterval)
    {
        //qDebug() << tr("draw point:%1/%2").arg(i).arg(mDataList.length()) << mDataList.at(i)->mValue;

        painter->drawLine(Len * (i - mInterval) + mStartX,
                          mDataList.at(i - mInterval)->mValue + Height,
                          Len * i + mStartX,
                          mDataList.at(i)->mValue + Height);
    }
#else
    QList<TDataPoint *>::const_iterator it = mDataList.constBegin();
    const TDataPoint *last = *it++;
    int i = 1;

    while(it != mDataList.constEnd())
    {
        painter->drawLine(Len * (i - 1) + mStartX,
                          last->mValue + Height,
                          Len * i + mStartX,
                          (*it)->mValue + Height);

        i++;
        last = *it++;
    }
#endif
}*/
