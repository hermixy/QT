#ifndef TARC_H
#define TARC_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  圆弧  /////////////////////////////////////////////////////
class TArc : public TGraphics
{
    //Q_OBJECT
public:
    TArc(double StartX, double StartY, double StopX, double StopY, double StartAngle = 45, double CoverAngle = 90,
          const Qt::PenStyle &LineSyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TArc();

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
#endif // TARC_H
