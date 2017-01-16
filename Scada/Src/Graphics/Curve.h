#ifndef TCURVE_H
#define TCURVE_H

#include "../Graphics/Graphics.h"
//#include "../RingBuf/RingBuf.h"

#include <QDebug>

class TDataPoint
{
public:
    double mValue;  // 实时值
    double mValid;  // 有效值
};

/////////////////////////////////////////////////  实时曲线  /////////////////////////////////////////////////////
class TCurve : public TGraphics
{
    Q_OBJECT
public:
    TCurve(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TCurve();

public slots:
    TItem *fCopy();
    void fCopy(TCurve *CurveFrom);

    virtual inline void fUpdateValue(const double Value, qint64 Time)
    {
        Q_UNUSED(Time);
        //TGraphics::fUpdateValue(Value, Time);

        if(mDataSum <= 0)
        {
            return;
        }

        //qDebug() << "TCurve::fUpdateValue" << mDataList.length() << Value;

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
    }

    void fSetDataSum(int DataSum);
    int fGetDataSum();
    void fSetInterval(int Interval);
    int fGetInterval();
    void fSetTime(int Time);
    int fGetTime();

private:
    int mDataSum;               // 点数
    int mInterval;           // 间隔，隔几个数 取一个数显示
    QList<TDataPoint *> mDataList;
    //TRingBuf *mRingData;

    QTimer *mTimer;
    int mTime;                  // 刷新间隔ms

    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TCURVE_H
