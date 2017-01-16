#ifndef TPOLYGON_H
#define TPOLYGON_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  多边形  /////////////////////////////////////////////////////
class TPolygon : public TGraphics
{
    Q_OBJECT
public:
    TPolygon(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    TPolygon(QPolygonF &PolygonPoints,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TPolygon();

public slots:
    TItem *fCopy();
    void fCopy(TPolygon *PolygonFrom);

    bool fSetDragAble(bool Able);

    void fSetPolygonPoints(const QPolygonF &PolygonPoints);
    QPolygonF fGetPolygonPoints() const;

private:
    QPolygonF mPolygonPoints;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);

    void UpdateData(double X, double Y);
};
#endif // TPOLYGON_H
