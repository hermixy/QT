#ifndef TPIE_H
#define TPIE_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  扇形  /////////////////////////////////////////////////////
class TPie : public TGraphics
{
    //Q_OBJECT
public:
    TPie(double StartX, double StartY, double StopX, double StopY, double StartAngle = 45, double CoverAngle = 90,
          const Qt::PenStyle &LineSyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TPie();

    TItem *fCopy();

    void fSetStartAngle(const double StartAngle);
    double fGetStartAngle() const;
    void fSetCoverAngle(const double CoverAngle);
    double fGetCoverAngle() const;

private:
    double mStartAngle;
    double mCoverAngle;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // TPIE_H
