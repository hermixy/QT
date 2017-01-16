#ifndef ITEM_H
#define ITEM_H

#include "item_global.h"

#include <QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QStyle>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtGui/QColor>
#include <QtGui/QPen>
#include <QtGui/QPainter>

class ITEMSHARED_EXPORT TItem : public QObject, public QGraphicsItem
{

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    TItem(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineSyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TItem();

    virtual TItem *fCopy() = 0;

    enum Type {
                    Base        = 0,
                    Group       = 1,
                    Point       = 2,
                    Line        = 3,
                    Ellipse     = 4,
                    Rect        = 5,
                    Polygon     = 6,
                    Arc         = 7,
                    Text        = 10,
                    Pixmap      = 11,
                    Path        = 12,
                    ValueText   = 13,
                    Light       = 14,
                    Meter       = 15,
                    ProcessBar  = 16,
                    Button      = 101,
                    InputEdit   = 102,
                    Table       = 103,
                    Max
              };

    virtual bool fSetType(const int Type);
    virtual int fGetType() const;

    virtual bool fGetMoveAble() const;
    virtual bool fSetMoveAble(bool Able);
    virtual bool fSetSelectAble(bool Able);
    virtual bool fGetSelectAble();
    virtual bool fSetDragAble(bool Able);
    virtual bool fGetDragAble() const;

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

Q_SIGNALS:
    void mDrop(TItem *Item, double StartDx, double StartDy, double StopDx, double StopDy);

public slots:
    virtual void fSetNameId(const QString &NameId);
    virtual QString fGetNameId();

    // 旋转
    bool fSetRotateAngle(double Angle);
    bool fDoRotate(double Angle);
    double fGetRotateAngle();
    bool fSetRotateCenter(double X, double Y);
    double fGetRotateCenterX();
    double fGetRotateCenterY();

    // 坐标
    virtual double fGetStartX();
    virtual double fGetStartY();
    virtual double fGetStopX();
    virtual double fGetStopY();
    virtual void fSetStartPos(double StartX, double StartY);
    virtual void fSetStopPos(double StopX, double StopY);
    virtual void fSetStartPos(const QPointF &Start);
    virtual void fSetStopPos(const QPointF &Stop);
    virtual void fMoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);

    // 样式
    virtual void fSetLineColor(const QColor &color);
    virtual void fSetLineColor(int r, int g, int b, int a = 255);
    virtual QColor fGetLineColor() const;

    virtual void fSetLineStyle(const Qt::PenStyle &style);
    virtual void fSetLineStyle(int style);
    virtual Qt::PenStyle fGetLineStyle() const;

    virtual void fSetLineWidth(int width);
    virtual int fGetLineWidth() const;

    virtual void fSetBackGroundColor(const QColor &color);
    virtual void fSetBackGroundColor(int r, int g, int b, int a = 255);
    virtual QColor fGetBackGroundColor() const;

    virtual void fSetZValue(double Z);
    virtual double fGetZValue();

    virtual void fUpdate();

protected:
    QString mNameId;

    int mType;

    double mRotateAngle;
    double mRotateCenterX;
    double mRotateCenterY;

    double mStartX;
    double mStartY;
    double mStopX;
    double mStopY;

    int mLineWidth;
    QColor mLineColor;
    Qt::PenStyle mLineStyle;
    QColor mBackGroundColor;

    bool mSelectAble;
    bool mMoveAble;
    bool mDragAble;
    bool mDropScale;        // 是否开始拖动

    enum
    {
        Direc_Free          = 0,
        Direc_Top           = 1,
        Direc_RightTop      = 2,
        Direc_Right         = 3,
        Direc_RightBottom   = 4,
        Direc_Bottom        = 5,
        Direc_LeftBottom    = 6,
        Direc_Left          = 7,
        Direc_LeftTop       = 8
    };

    int mDropDirection;     // 拖动方向

    QPointF mDropPos;     // 拖动点,会随着拖动过程改变
    QPointF mPressStart;  // 拖动起点

    virtual void fMoveEnable(bool Enable);
    virtual void UpdateMouse(QPointF &PosNow);
    virtual QPainter* fBeforePaint(QPainter* &painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void fMovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY);
    virtual void fTransFormPos();
    virtual void fMsgCopy(QObject *To, QObject *From);
};

#endif // ITEM_H
