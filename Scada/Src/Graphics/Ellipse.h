#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  圆形  ////////////////////////////////////////////////////
class TEllipse : public TGraphics
{
    Q_OBJECT
public:
    TEllipse(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TEllipse();

public slots:
    TItem *fCopy();
    void fCopy(TEllipse *EllipseFrom);

private:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TELLIPSE_H
