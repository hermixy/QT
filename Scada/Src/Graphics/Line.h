#ifndef TLINE_H
#define TLINE_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  直线  ////////////////////////////////////////////////////
class TLine : public TGraphics
{
    Q_OBJECT
public:
    TLine(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);
    virtual ~TLine();

    QRectF boundingRect() const;

public slots:
    TItem *fCopy();
    void fCopy(TLine *LineFrom);

    virtual void fSetStartPos(double StartX, double StartY);
    virtual void fSetStopPos(double StopX, double StopY);
    virtual void fSetStartPos(const QPointF &Start);
    virtual void fSetStopPos(const QPointF &Stop);
    virtual void fMoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);

private:
    double mPosX;
    double mPosY;
    double mWidth;
    double mHeight;

    void fUpdatePos();

    void UpdateMouse(QPointF &PosNow);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void fDraw(QPainter *painter);
};

#endif // TLINE_H
