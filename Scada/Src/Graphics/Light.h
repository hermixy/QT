#ifndef TLIGHT_H
#define TLIGHT_H

#include "../Graphics/Graphics.h"

class TLight : public TGraphics
{
    Q_OBJECT
public:
    TLight(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::red);

    virtual ~TLight();

public slots:
    TItem *fCopy();
    void fCopy(TLight *LightFrom);

private:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TLIGHT_H
