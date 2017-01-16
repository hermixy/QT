#ifndef TARC_H
#define TARC_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  圆弧  /////////////////////////////////////////////////////
class TArc : public TGraphics
{
    Q_OBJECT
public:
    TArc(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TArc();

    enum
    {
        MODE_MIN = -1,
        MODE_PIE = 0,
        MODE_ARC,
        MODE_CHORD,
        MODE_MAX
    };

public slots:
    TItem *fCopy();
    void fCopy(TArc *ArcFrom);

    bool fSetMode(const int Mode);
    int fGetMode();
    void fSetStartAngle(const double StartAngle);
    double fGetStartAngle() const;
    void fSetCoverAngle(const double CoverAngle);
    double fGetCoverAngle() const;

private:
    int mMode;
    double mStartAngle;
    double mCoverAngle;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};
#endif // TARC_H
