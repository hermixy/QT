#include "Ellipse.h"

TEllipse::TEllipse(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Ellipse);
}

TEllipse::~TEllipse()
{}

TItem *TEllipse::fCopy()
{
    TEllipse *Ellipse = new TEllipse();

    Ellipse->fCopy(this);

    return Ellipse;
}

void TEllipse::fCopy(TEllipse *EllipseFrom)
{
    if(NULL == EllipseFrom)
    {
        return;
    }

    TGraphics::fCopy(EllipseFrom);
}

void TEllipse::fDraw(QPainter *painter)
{
    painter->drawEllipse(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);
}

/*void TEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawEllipse(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);
}*/
