#include "Point.h"

TPoint::TPoint(double StartX, double StartY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StartX, StartY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Point);

    TItem::fSetMoveAble(true);
    TItem::fSetDragAble(false);
}

TPoint::~TPoint()
{}

TItem *TPoint::fCopy()
{
    TPoint *Point = new TPoint();

    Point->fCopy(this);

    return Point;
}

void TPoint::fCopy(TPoint *PointFrom)
{
    if(NULL == PointFrom)
    {
        return;
    }

    TGraphics::fCopy(PointFrom);
}

bool TPoint::fSetDragAble(bool Able)
{
    Q_UNUSED(Able);
    return false;
}

void TPoint::fDraw(QPainter *painter)
{
    painter->drawPoint(mStartX, mStartY);
}

/*void TPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawPoint(mStartX, mStartY);
}*/
