#ifndef TITEM_H
#define TITEM_H

#ifndef DEF_RUN_IN_EMBEDDED

#include <QGraphicsItem>
#else
#include <QtWidgets/QGraphicsItem>
#endif

#include <QtGui/QPen>
#include <QtGui/QPainter>

class TItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    TItem(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TItem();

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
                    Curve       = 17,
                    Button      = 101,
                    InputEdit   = 102,
                    Table       = 103,
                    Max
              };

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QPainter* fBeforePaint(QPainter* &painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void fDraw(QPainter* painter) = 0;

    // 鼠标事件
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // 键盘事件
    virtual void keyReleaseEvent(QKeyEvent *event);

Q_SIGNALS:
    void mDrop(TItem *Item, double StartDx, double StartDy, double StopDx, double StopDy);

public slots:
    virtual TItem* fCopy() = 0;             // 创建一个
    virtual void fCopy(TItem *ItemFrom);  // 根据拷贝已有对象的属性

    virtual bool fSetType(const int Type);
    virtual int fGetType() const;

    virtual bool fGetMoveAble() const;
    virtual bool fSetMoveAble(bool Able);
    virtual bool fSetSelectAble(bool Able);
    virtual bool fGetSelectAble();
    virtual bool fSetDragAble(bool Able);
    virtual bool fGetDragAble() const;

    // 旋转
    bool fSetRotateAngle(double Angle);
    bool fDoRotate(double Angle);
    double fGetRotateAngle();
    bool fSetRotateCenter(double X, double Y);
    double fGetRotateCenterX();
    double fGetRotateCenterY();

    // 坐标，未进行旋转等变换
    virtual double fGetStartX();
    virtual double fGetStartY();
    virtual double fGetStopX();
    virtual double fGetStopY();
    virtual double fGetCenterX();
    virtual double fGetCenterY();
    virtual void fSetStartPos(double StartX, double StartY);
    virtual void fSetStopPos(double StopX, double StopY);
    virtual void fSetStartPos(const QPointF &Start);
    virtual void fSetStopPos(const QPointF &Stop);
    virtual void fMoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);

    // 坐标，旋转等变换以后
    virtual double fGetCurrentStartX();
    virtual double fGetCurrentStartY();
    virtual double fGetCurrentStopX();
    virtual double fGetCurrentStopY();
    virtual double fGetCurrentCenterX();
    virtual double fGetCurrentCenterY();

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

    virtual void fMovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY);
    virtual void fTransFormPos();
};

#endif // TITEM_H
