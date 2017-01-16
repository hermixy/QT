#include "Meter.h"

#include "../Base/LibBase.h"

#ifndef DEF_RUN_IN_EMBEDDED
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif

#include <QDebug>

TMeter::TMeter(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    mMode = MODE_COUNTER_WISE;
    mScaleSum = 8;
    mStartAngle = 216;
    mStepAngle = 36;
    mStartValue = 0;
    mStepValue = 1;

    mPointerColor = QColor(0, 0, 190, 255);//分针颜色

    fSetType(TItem::Meter);
}

TMeter::~TMeter()
{}

TItem *TMeter::fCopy()
{
    TMeter *Meter = new TMeter();

    Meter->fCopy(this);

    return Meter;
}

void TMeter::fCopy(TMeter *MeterFrom)
{
    if(NULL == MeterFrom)
    {
        return;
    }

    TGraphics::fCopy(MeterFrom);

    this->fSetMode(MeterFrom->fGetMode());
    this->fSetScaleSum(MeterFrom->fGetScaleSum());
    this->fSetStartAngle(MeterFrom->fGetStartAngle());
    this->fSetStepAngle(MeterFrom->fGetStepAngle());
    this->fSetStartValue(MeterFrom->fGetStartValue());
    this->fSetStepValue(MeterFrom->fGetStepValue());
    this->fSetPointerColor(MeterFrom->fGetPointerColor());
}

void TMeter::fSetMode(int Mode)
{
    if(Mode >= MODE_CLOSCK_WISE && Mode <= MODE_COUNTER_WISE)
    {
        mMode = Mode;
    }
}

int TMeter::fGetMode()
{
    return mMode;
}

void TMeter::fSetScaleSum(int Sum)
{
    mScaleSum = Sum;
}

int TMeter::fGetScaleSum()
{
    return mScaleSum;
}

void TMeter::fSetStartAngle(double Angle)
{
    mStartAngle = Angle;
}

double TMeter::fGetStartAngle()
{
    return mStartAngle;
}

void TMeter::fSetStepAngle(double Angle)
{
    mStepAngle = Angle;
}

double TMeter::fGetStepAngle()
{
    return mStepAngle;
}

void TMeter::fSetStartValue(double Value)
{
    mStartValue = Value;
}

double TMeter::fGetStartValue()
{
    return mStartValue;
}

void TMeter::fSetStepValue(double Value)
{
    mStepValue = Value;
}

double TMeter::fGetStepValue()
{
    return mStepValue;
}

void TMeter::fSetPointerColor(const QColor &Color)
{
    mPointerColor = Color;
}

QColor TMeter::fGetPointerColor()
{
    return mPointerColor;
}

//void TMeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
void TMeter::fDraw(QPainter *painter)
{
    int fronsize;
    double r;
    QFont font;

    double Width = mStopX - mStartX;
    double Height = mStopY - mStartY;

    r = MIN(Width, Height) / 2;

    //fBeforePaint(painter, option, widget);

    painter->translate(mStartX + Width / 2, mStartY + Height / 2);

    painter->drawEllipse( -r, -r, 2 * r, 2 * r);
    fronsize = 0.1 * r + 1;// 加1，避免为0
    font.setPointSize(fronsize);
    painter->setFont(font);

    double DrawAngle = mStartAngle;
    int Direct = 0;

    if(MODE_COUNTER_WISE == mMode)
    {
        Direct = -1;
    }
    else
    {
        Direct = 1;
    }

    for(int i = 0; i < mScaleSum; i++)
    {
        double Angle = (DrawAngle + i * Direct * mStepAngle) / 180 * 3.1415926535898;
        double x = qCos(Angle) * 0.7 * r;
        double y = -qSin(Angle)* 0.7 * r;

        if(x <= 0)
        {
            x -= (fronsize / 2);
            y -= (fronsize / 2);
            painter->drawText(x,
                            y,
                            r,
                            r,
                            Qt::AlignLeft | Qt::AlignTop,
                            QString::number(mStartValue + i * mStepValue));
        }
        else
        {
            x += (fronsize / 2);
            y += (fronsize * 4 / 5);
            painter->drawText(x - r,
                            y - r,
                            r,
                            r,
                            Qt::AlignRight | Qt::AlignBottom,
                            QString::number(mStartValue + i * mStepValue));
        }
    }

    // 绘制大刻度
    painter->save();
    painter->rotate(-DrawAngle); // 旋转回起点

    for(int i = 0; i < mScaleSum; i++)
    {
        painter->drawLine(0.8 * r, 0, r, 0);
        painter->rotate(-Direct * mStepAngle);
    }
    painter->restore();

    // 绘制小刻度
    painter->save();
    painter->rotate(-DrawAngle); // 旋转回起点

    int Count = mScaleSum - 1;

    if(360 == mScaleSum * mStepAngle)
    {
        Count = mScaleSum;
    }

    for(int i = 0 ; i < Count; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(0 != j)
            {
                painter->drawLine(0.9 * r, 0, r, 0);
            }

            painter->rotate(-Direct * mStepAngle / 10);
        }
    }
    painter->restore();

    // 绘制指针
    double PointerWidth = r * 0.04; // 指针宽度

    const QPoint minuteHand[3] = {
        QPoint(-r * 0.2, -PointerWidth),
        QPoint(r * 0.7, 0),
        QPoint(-r * 0.2, PointerWidth)
    };

    double Value = mValue;

    //qDebug() << "Value" << Value << mStartValue << mStartValue + mStepValue * mScaleSum;

    if(Value < mStartValue)
    {
        Value = mStartValue;
    }
    else if(Value > (mStartValue + mStepValue * mScaleSum))
    {
        Value = mStartValue + mStepValue * mScaleSum;
    }

    int Pointer = DrawAngle + Direct * (Value - mStartValue) / mStepValue * mStepAngle;

    //qDebug() << "TMeter::paint" << Value << Pointer;

    painter->setPen(Qt::NoPen);//填充分针，不需要边线所以NoPen
    painter->setBrush(mPointerColor);//设置画刷颜色

    painter->save();//保存当前状态
    painter->rotate(-Pointer);//设旋转角度
    painter->drawConvexPolygon(minuteHand, 3);//填充分针部分
    painter->restore();//恢复保存前状态
}
