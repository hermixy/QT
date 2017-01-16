#include "Rect.h"

TRect::TRect(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Rect);
}

TRect::~TRect()
{}

TItem *TRect::fCopy()
{
    TRect *Rect = new TRect();

    Rect->fCopy(this);

    return Rect;
}

void TRect::fCopy(TRect *RectFrom)
{
    if(NULL == RectFrom)
    {
        return;
    }

    TGraphics::fCopy(RectFrom);
}

void TRect::fDraw(QPainter *painter)
{
    painter->drawRect(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);
}

/*void TRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawRect(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);
}*/
