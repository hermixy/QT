#include "ProcessBar.h"

#include "../Base/LibBase.h"
#include <QDebug>

TProcessBar::TProcessBar(double StartX, double StartY, double StopX, double StopY,
                         const Qt::PenStyle &LineStyle, const int LineWidth,
                         const QColor &LineColor, const QColor &BackColor) :
                  TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    if(mStopY - mStartY >= mStopX - mStartX)
    {
        mMode = PROCESSBAR_RIGHT;
    }
    else
    {
        mMode = PROCESSBAR_BOTTOM;
    }

    mScaleSum = 10;
    mStartValue = 0;
    mStepValue = 10;
    mValueColor = Qt::green;

    fSetType(TItem::ProcessBar);
}

TProcessBar::~TProcessBar()
{}

TItem *TProcessBar::fCopy()
{
    TProcessBar *ProcessBar = new TProcessBar();

    ProcessBar->fCopy(this);

    return ProcessBar;
}

void TProcessBar::fCopy(TProcessBar *ProcessBarFrom)
{
    if(NULL == ProcessBarFrom)
    {
        return;
    }

    TGraphics::fCopy(ProcessBarFrom);

    this->fSetScaleSum(ProcessBarFrom->fGetScaleSum());
    this->fSetStartValue(ProcessBarFrom->fGetStartValue());
    this->fSetStepValue(ProcessBarFrom->fGetStepValue());
    this->fSetValueColor(ProcessBarFrom->fGetValueColor());
}

void TProcessBar::fSetMode(int Mode)
{
    mMode = Mode;
}

int TProcessBar::fGetMode()
{
    return mMode;
}

void TProcessBar::fSetScaleSum(int Sum)
{
    mScaleSum = Sum;
}

int TProcessBar::fGetScaleSum()
{
    return mScaleSum;
}

void TProcessBar::fSetStartValue(double Value)
{
    mStartValue = Value;
}

double TProcessBar::fGetStartValue()
{
    return mStartValue;
}

void TProcessBar::fSetStepValue(double Value)
{
    mStepValue = Value;
}

double TProcessBar::fGetStepValue()
{
    return mStepValue;
}

void TProcessBar::fSetValueColor(const QColor &Color)
{
    mValueColor = Color;
}

void TProcessBar::fSetValueColor(int R, int G, int B, int A)
{
    mValueColor.setRed(R);
    mValueColor.setGreen(G);
    mValueColor.setBlue(B);
    mValueColor.setAlpha(A);
}

QColor TProcessBar::fGetValueColor()
{
    return mValueColor;
}

/*void TProcessBar::fSetTextName(const QString &Name)
{
    mTextName = Name;
}

QString TProcessBar::fGetTextName()
{
    return mTextName;
}

void TProcessBar::fSetTextUnit(const QString &Unit)
{
    mTextUnit = Unit;
}

QString TProcessBar::fGetTextUnit()
{
    return mTextUnit;
}*/

//void TProcessBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
void TProcessBar::fDraw(QPainter *painter)
{
    QFont Font;

    double Width = mStopX - mStartX;
    double Height = mStopY - mStartY;

    //fBeforePaint(painter, option, widget);

    painter->translate(mStartX + Width / 2, mStartY + Height / 2);

    const int FronSize = 0.12 * MIN(Width, Height) + 1;// 加1，避免为0

    double DrawStartX = 0;
    double DrawStartY = 0;
    double DrawWidth = 0;
    double DrawHeight = 0;

    double NoDraw = 0;

    switch(mMode)
    {
    case PROCESSBAR_RIGHT:
        DrawStartX = 0;
        DrawStartY = -Height / 2;
        DrawWidth = Width / 2;
        DrawHeight = Height;

        NoDraw = FronSize / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 2 * NoDraw;
        break;

    case PROCESSBAR_LEFT:
        DrawStartX = -Width / 2;
        DrawStartY = -Height / 2;
        DrawWidth = Width / 2;
        DrawHeight = Height;

        NoDraw = FronSize / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 2 * NoDraw;
        break;

    case PROCESSBAR_TOP:
        DrawStartX = -Width / 2;
        DrawStartY = -Height / 2;
        DrawWidth = Width;
        DrawHeight = Height / 2;

        NoDraw = DrawWidth / mScaleSum / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += 0.1 * NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 0.2 * NoDraw;
        break;

    case PROCESSBAR_BOTTOM:
        DrawStartX = -Width / 2;
        DrawStartY = 0;
        DrawWidth = Width;
        DrawHeight = Height / 2;

        NoDraw = DrawWidth / mScaleSum / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += 0.1 * NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 0.2 * NoDraw;
        break;

    default:
        break;
    }

    // 绘制外边框
    painter->drawRect(DrawStartX, DrawStartY, DrawWidth, DrawHeight);

    if(mScaleSum < 1)
    {
        return;
    }

    // 绘制填充
    if(mValue > mStartValue)
    {
        painter->save();
        QPen pen;
        pen.setStyle(Qt::NoPen);
        painter->setPen(pen);
        painter->setBrush(mValueColor);

        double Value = mValue;
        double StopValue = mStartValue + (mScaleSum - 1) * mStepValue;

        if(Value > StopValue)
        {
            Value = StopValue;
        }

        double Len = (Value - mStartValue) / mStepValue / (mScaleSum - 1);

        if(PROCESSBAR_RIGHT == mMode || PROCESSBAR_LEFT == mMode)
        {
            Len *= DrawHeight;
            painter->drawRect(DrawStartX,
                              DrawStartY + DrawHeight - Len,
                              DrawWidth,
                              Len);
        }
        else
        {
            Len *= DrawWidth;
            painter->drawRect(DrawStartX,
                              DrawStartY,
                              Len,
                              DrawHeight);
        }

        painter->restore();
    }

    // 绘制刻度和文字，同外边框 上下对换或者左右对换
    switch(mMode)
    {
    case PROCESSBAR_RIGHT:
        DrawStartX = -Width / 2;
        DrawStartY = -Height / 2;
        DrawWidth = Width / 2;
        DrawHeight = Height;

        NoDraw = FronSize / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 2 * NoDraw;
        break;

    case PROCESSBAR_LEFT:
        DrawStartX = 0;
        DrawStartY = -Height / 2;
        DrawWidth = Width / 2;
        DrawHeight = Height;

        NoDraw = FronSize / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 2 * NoDraw;
        break;

    case PROCESSBAR_TOP:
        DrawStartX = -Width / 2;
        DrawStartY = 0;
        DrawWidth = Width;
        DrawHeight = Height / 2;

        NoDraw = DrawWidth / mScaleSum / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += 0.1 * NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 0.2 * NoDraw;
        break;

    case PROCESSBAR_BOTTOM:
        DrawStartX = -Width / 2;
        DrawStartY = -Height / 2;
        DrawWidth = Width;
        DrawHeight = Height / 2;

        NoDraw = DrawWidth / mScaleSum / 2 + 1;

        DrawStartX += NoDraw;
        DrawStartY += 0.1 * NoDraw;
        DrawWidth -= 2 * NoDraw;
        DrawHeight -= 0.2 * NoDraw;
        break;

    default:
        break;
    }

    Font.setPointSize(FronSize);
    painter->setFont(Font);

    double StepLen = 0;

    if(PROCESSBAR_RIGHT == mMode)
    {
        StepLen = DrawHeight / (mScaleSum - 1);

        for(int i = 0; i < mScaleSum; i++)
        {
            double PosY = DrawStartY + DrawHeight - i * StepLen;

            painter->drawLine(DrawStartX + 0.7 * DrawWidth,
                              PosY,
                              DrawStartX + DrawWidth,
                              PosY);

            painter->drawText(DrawStartX,
                              PosY - NoDraw,
                              0.6 * DrawWidth,
                              StepLen,
                              Qt::AlignRight | Qt::AlignHCenter,
                              QString::number(mStartValue + i * mStepValue));
        }
    }
    else if(PROCESSBAR_LEFT == mMode)
    {
        StepLen = DrawHeight / (mScaleSum - 1);

        for(int i = 0; i < mScaleSum; i++)
        {
            double PosY = DrawStartY + DrawHeight - i * StepLen;

            painter->drawLine(DrawStartX,
                              PosY,
                              DrawStartX + 0.3 * DrawWidth,
                              PosY);

            painter->drawText(DrawStartX + 0.4 * DrawWidth,
                              PosY - NoDraw,
                              DrawWidth,
                              StepLen,
                              Qt::AlignLeft | Qt::AlignHCenter,
                              QString::number(mStartValue + i * mStepValue));
        }
    }
    else if(PROCESSBAR_BOTTOM == mMode)
    {
        StepLen = DrawWidth / (mScaleSum - 1);

        for(int i = 0; i < mScaleSum; i++)
        {
            double PosX = DrawStartX + i * StepLen;

            painter->drawLine(PosX,
                              DrawStartY + 0.7 * DrawHeight,
                              PosX,
                              DrawStartY + DrawHeight);

            painter->drawText(PosX - NoDraw,
                              DrawStartY,
                              StepLen,
                              0.6 * DrawHeight,
                              Qt::AlignCenter,
                              QString::number(mStartValue + i * mStepValue));
        }
    }
    else
    {
        StepLen = DrawWidth / (mScaleSum - 1);

        for(int i = 0; i < mScaleSum; i++)
        {
            double PosX = DrawStartX + i * StepLen;

            painter->drawLine(PosX,
                              DrawStartY,
                              PosX,
                              DrawStartY + 0.3 * DrawHeight);

            painter->drawText(PosX - NoDraw,
                              DrawStartY + 0.4 * DrawHeight,
                              StepLen,
                              0.6 * DrawHeight,
                              Qt::AlignCenter,
                              QString::number(mStartValue + i * mStepValue));
        }
    }
}
