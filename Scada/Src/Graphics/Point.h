#ifndef TPOINT_H
#define TPOINT_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  点  /////////////////////////////////////////////////////
class TPoint : public TGraphics
{
    Q_OBJECT
public:
    TPoint(double StartX = 0, double StartY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TPoint();

public slots:
    TItem *fCopy();
    void fCopy(TPoint *PointFrom);

    bool fSetDragAble(bool Able);

private:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TPOINT_H
