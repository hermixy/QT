#include "Polygon.h"

TPolygon::TPolygon(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Polygon);

    TItem::fSetMoveAble(true);
    TItem::fSetDragAble(false);
}

TPolygon::TPolygon(QPolygonF &PolygonPoints,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor):
        TGraphics(0, 0, 0, 0, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetPolygonPoints(PolygonPoints);

    fSetType(TItem::Polygon);

    TItem::fSetMoveAble(true);
    TItem::fSetDragAble(false);
}

TPolygon::~TPolygon()
{}

TItem *TPolygon::fCopy()
{
    TPolygon *Polygon = new TPolygon();

    Polygon->fCopy(this);

    return Polygon;
}

void TPolygon::fCopy(TPolygon *PolygonFrom)
{
    if(NULL == PolygonFrom)
    {
        return;
    }

    TGraphics::fCopy(PolygonFrom);

    this->fSetPolygonPoints(PolygonFrom->fGetPolygonPoints());
}

void TPolygon::UpdateData(double X, double Y)
{
    double dx = (X - mStartX) / 2;
    double dy = (Y - mStartY) / 2;

    QPolygonF PolygonPoints;

    foreach(QPointF Point, mPolygonPoints)
    {
        PolygonPoints.push_back(QPointF((Point.x() + dx), (Point.y() + dy)));
    }

    fSetPolygonPoints(PolygonPoints);
}

void TPolygon::fSetPolygonPoints(const QPolygonF &PolygonPoints)
{
    mPolygonPoints = PolygonPoints;

    mStartX = mPolygonPoints.boundingRect().topLeft().x();
    mStartY = mPolygonPoints.boundingRect().topLeft().y();
    mStopX = mPolygonPoints.boundingRect().bottomRight().x();
    mStopY = mPolygonPoints.boundingRect().bottomRight().y();
}

QPolygonF TPolygon::fGetPolygonPoints() const
{
    QPolygonF SceneF;

    for(int i = 0; i < mPolygonPoints.count(); ++i)
    {
        SceneF.push_back(mapToScene(mPolygonPoints.at(i)));
    }

    return SceneF;
}

bool TPolygon::fSetDragAble(bool Able)
{
    Q_UNUSED(Able);
    return false;
}

void TPolygon::fDraw(QPainter *painter)
{
    painter->drawPolygon(mPolygonPoints);
}

/*void TPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawPolygon(mPolygonPoints);
}*/

