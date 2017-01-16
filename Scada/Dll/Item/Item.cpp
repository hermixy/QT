#include "Item.h"
#include "../../Src/Base/LibBase.h"

#include <QGraphicsSceneEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

TItem::TItem(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineSyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) : QGraphicsItem()
{
    mNameId = tr("");

    mType = TItem::Base;

    mStartX = MIN(StartX, StopX);
    mStartY = MIN(StartY, StopY);
    mStopX = MAX(StartX, StopX);
    mStopY = MAX(StartY, StopY);

    mLineWidth = LineWidth;
    mLineColor = LineColor;
    mLineStyle = LineSyle;
    mBackGroundColor = BackColor;

    mDropScale = false;
    mDropDirection = Direc_Free;

    fSetSelectAble(true);
    fSetMoveAble(true);
    fSetDragAble(true);

    setAcceptHoverEvents(true);

    mRotateAngle = 0;
    mRotateCenterX = 0;
    mRotateCenterY = 0;
}

void TItem::fTransFormPos()
{
    double StartX   = mStartX;
    double StartY   = mStartY;
    double StopX    = mStopX;
    double StopY    = mStopY;

    mStartX = MIN(StartX, StopX);
    mStartY = MIN(StartY, StopY);
    mStopX = MAX(StartX, StopX);
    mStopY = MAX(StartY, StopY);
}

TItem::~TItem()
{}

void TItem::fMoveEnable(bool Enable)
{
    setFlag(ItemIsMovable, Enable);
}

bool TItem::fSetMoveAble(bool Able)
{
    mMoveAble = Able;
    fMoveEnable(mMoveAble);

    return true;
}

bool TItem::fSetSelectAble(bool Able)
{
    mSelectAble = Able;
    setFlag(ItemIsSelectable, Able);

    return true;
}

bool TItem::fGetSelectAble()
{
    return mSelectAble;
}

bool TItem::fSetDragAble(bool Able)
{
    mDragAble = Able;
    //setAcceptHoverEvents(Able);

    return true;
}

bool TItem::fGetDragAble() const
{
    return mDragAble;
}

bool TItem::fGetMoveAble() const
{
    return mMoveAble;
}

QRectF TItem::boundingRect() const
{
    //qDebug() << "TItem::boundingRect()" << mStartX << mStartY << mStopX - mStartX << mStopY - mStartY;
    QRectF mRect(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);

    const int padding = (mLineWidth + 1) / 2;
    mRect.adjust(-padding, -padding, +padding, +padding);

    return mRect;
}

QPainter* TItem::fBeforePaint(QPainter* &painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPen pen;

    if(option->state & QStyle::State_Selected)
    {
        pen.setColor(Qt::red);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(3);
        painter->setPen(pen);

        QColor RectColor = mBackGroundColor;
        RectColor.setAlpha(0);
        painter->setBrush(RectColor);

        painter->drawRect(boundingRect());
    }

    pen.setColor(mLineColor);
    pen.setStyle(mLineStyle);

    pen.setWidth(mLineWidth);

    painter->setPen(pen);
    painter->setBrush(mBackGroundColor);

    painter->setRenderHint(QPainter::Antialiasing, true);

    return painter;
}

bool TItem::fSetType(const int Type)
{
    if(Type > Base && Type < Max)
    {
        mType = Type;
        return true;
    }
    else
    {
        return false;
    }
}

int TItem::fGetType() const
{
    return mType;
}

void TItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "TItem::mousePressEvent";
    if(fGetDragAble() && Qt::LeftButton == event->button() && mDropDirection > Direc_Free)
    {
        mPressStart = event->scenePos();
        mDropPos = event->scenePos();
        mDropScale = true;
    }
    else
    {
        fMoveEnable(true); // 临时设置为可移动，否则不能触发mouseReleaseEvent
    }

    fUpdate();

    QGraphicsItem::mousePressEvent(event);
}

void TItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "TItem::mouseMoveEvent";
    if(mDropScale)
    {
        QPointF dis = event->scenePos() - mDropPos;
        mDropPos = event->scenePos();

        switch(mDropDirection)
        {
        case Direc_Top: // 上 dis.y < 0
        {
            mStartY += dis.y() / 2;
            mStopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(0, dis.y() / 2);
            break;
        }
        case Direc_RightTop: // 右上
        {
            mStartX -= dis.x() / 2;
            mStartY += dis.y() / 2;
            mStopX += dis.x() / 2;
            mStopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Right: // 右 dis.x > 0
        {
            mStartX -= dis.x() / 2;
            mStopX += dis.x() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, 0);
            break;
        }
        case Direc_RightBottom: // 右下
        {
            mStartX -= dis.x() / 2;
            mStartY -= dis.y() / 2;
            mStopX += dis.x() / 2;
            mStopY += dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Bottom: // 下 dis.y > 0
        {
            mStartY -= dis.y() / 2;
            mStopY += dis.y() / 2;

            update(boundingRect());
            moveBy(0, dis.y() / 2);
            break;
        }
        case Direc_LeftBottom: // 左下
        {
            mStartX += dis.x() / 2;
            mStartY -= dis.y() / 2;
            mStopX -= dis.x() / 2;
            mStopY += dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Left: // 左 dis.x < 0
        {
            mStartX += dis.x() / 2;
            mStopX -= dis.x() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, 0);
            break;
        }
        case Direc_LeftTop: // 左上
        {
            mStartX += dis.x() / 2;
            mStartY += dis.y() / 2;
            mStopX -= dis.x() / 2;
            mStopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }

        default:
            break;
        }
    }

    fUpdate();

    if(mMoveAble) // 避免因为临时设置为可移动，造成不可移动的图形发生移动
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void TItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "TItem::mouseReleaseEvent";
    if(mDropScale)
    {
        double StartDx = 0;
        double StartDy = 0;
        double StopDx = 0;
        double StopDy = 0;

        double Dx = mDropPos.x() - mPressStart.x();
        double Dy = mDropPos.y() - mPressStart.y();

        switch(mDropDirection)
        {
        case Direc_Top: // 上 dis.y < 0
            StartDy = Dy;
            break;

        case Direc_RightTop: // 右上
            StartDy = Dy;
            StopDx = Dx;
            break;

        case Direc_Right: // 右 dis.x > 0
            StopDx = Dx;
            break;

        case Direc_RightBottom: // 右下
            StopDx = Dx;
            StopDy = Dy;
            break;

        case Direc_Bottom: // 下 dis.y > 0
            StopDy = Dy;
            break;

        case Direc_LeftBottom: // 左下
            StartDx = Dx;
            StopDy = Dy;
            break;

        case Direc_Left: // 左 dis.x < 0
            StartDx = Dx;
            break;

        case Direc_LeftTop: // 左上
            StartDx = Dx;
            StartDy = Dy;
            break;

        default:
            break;
        }

        //qDebug() << "emit mDrop" << StartDx << StartDy << StopDx << StopDy;
        emit mDrop(this, StartDx, StartDy, StopDx, StopDy);
    }

    fMoveEnable(mMoveAble); // 关闭临时可移动，恢复正常的移动标志

    fUpdate();

    QGraphicsItem::mouseReleaseEvent(event);
}

void TItem::UpdateMouse(QPointF &PosNow)
{
    if(!mDragAble)
    {
        return;
    }

    QCursor mCursor;

    QRectF mRect = boundingRect();

    QPointF lt = this->scenePos() + QPointF(mRect.x(), mRect.y());
    QPointF lb = this->scenePos() + QPointF(mRect.x(), mRect.y() + mRect.height());
    QPointF rt = this->scenePos() + QPointF(mRect.x() + mRect.width(), mRect.y());
    QPointF rb = this->scenePos() + QPointF(mRect.x() + mRect.width(), mRect.y() + mRect.height());

    int Add = mLineWidth + 2;

    if(PosNow.x() <= lt.x() + Add && PosNow.y() <= lt.y() + Add)      // 左上
    {
        mDropDirection = Direc_LeftTop;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(PosNow.x() >= rb.x() - Add && PosNow.y() >= rb.y() - Add) // 右下
    {
        mDropDirection = Direc_RightBottom;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(PosNow.x() <= lb.x() + Add && PosNow.y() >= lb.y() - Add) // 左下
    {
        mDropDirection = Direc_LeftBottom;
        mCursor.setShape(Qt::SizeBDiagCursor);
    }
    else if(PosNow.x() >= rt.x() - Add && PosNow.y() <= rt.y() + Add) // 右上
    {
        mDropDirection = Direc_RightTop;
        mCursor.setShape(Qt::SizeBDiagCursor);
    }
    else if(PosNow.x() < lt.x() + Add /* && PosNow.y() < lb.y() && PosNow.y() > lt.y()*/) // 左
    {
        mDropDirection = Direc_Left;
        mCursor.setShape(Qt::SizeHorCursor);
    }
    else if(PosNow.x() > rt.x() - Add /*&& PosNow.y() < rb.y() && PosNow.y() > rt.y()*/) // 右
    {
        mDropDirection = Direc_Right;
        mCursor.setShape(Qt::SizeHorCursor);
    }
    else if(PosNow.y() < lt.y() + Add /*&& PosNow.x() > lt.x() && PosNow.x() < rt.x()*/) // 上
    {
        mDropDirection = Direc_Top;
        mCursor.setShape(Qt::SizeVerCursor);
    }
    else if(PosNow.y() > lb.y() - Add /*&& PosNow.x() > lt.x() && PosNow.x() < rt.x()*/) // 下
    {
        mDropDirection = Direc_Bottom;
        mCursor.setShape(Qt::SizeVerCursor);
    }
    else
    {
        mDropDirection = Direc_Free;
        mCursor.setShape(Qt::ArrowCursor);
    }

    //qDebug() << "mDropDirection" << mDropDirection;

    if(Direc_Free == mDropDirection)
    {
        fMoveEnable(mMoveAble);
    }
    else
    {
        fMoveEnable(false);
    }

    setCursor(mCursor);
}

void TItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "TItem::hoverEnterEvent";
    QPointF PosNow = event->scenePos();
    UpdateMouse(PosNow);

    fUpdate();
    QGraphicsItem::hoverEnterEvent(event);
}

void TItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "TItem::hoverMoveEvent";
    QPointF PosNow = event->scenePos();
    UpdateMouse(PosNow);

    fUpdate();
    QGraphicsItem::hoverMoveEvent(event);
}

void TItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "TItem::hoverLeaveEvent";
    mDropScale = false;
    mDropDirection = Direc_Free;

    // 当鼠标离开图形，恢复移动状态

    fMoveEnable(mMoveAble);

    fUpdate();

    QCursor mCursor;
    mCursor.setShape(Qt::ArrowCursor);
    setCursor(mCursor);

    QGraphicsItem::hoverLeaveEvent(event);
}

void TItem::fMsgCopy(QObject *To, QObject *From)
{
    TItem *ItemTo = dynamic_cast<TItem *>(To);
    TItem *ItemFrom = dynamic_cast<TItem *>(From);

    if(NULL == ItemTo || NULL == ItemFrom)
    {
        return;
    }

    ItemTo->fSetStartPos(ItemFrom->fGetStartX(), ItemFrom->fGetStartY());
    ItemTo->fSetStopPos(ItemFrom->fGetStopX(), ItemFrom->fGetStopY());
    ItemTo->fSetLineWidth(ItemFrom->fGetLineWidth());
    ItemTo->fSetLineColor(ItemFrom->fGetLineColor());
    ItemTo->fSetLineStyle(ItemFrom->fGetLineStyle());
    ItemTo->fSetBackGroundColor(ItemFrom->fGetBackGroundColor());
    ItemTo->fSetSelectAble(ItemFrom->fGetSelectAble());
    ItemTo->fSetMoveAble(ItemFrom->fGetMoveAble());
    ItemTo->fSetDragAble(ItemFrom->fGetDragAble());
    ItemTo->fSetZValue(ItemFrom->fGetZValue());
}

double TItem::fGetStartX()
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    double Ret = mapToScene(mStartX, mStartY).x();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }

    return Ret;
}

double TItem::fGetStartY()
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    double Ret = mapToScene(mStartX, mStartY).y();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }

    return Ret;

    //return mapToScene(mStartX, mStartY).y();
}

double TItem::fGetStopX()
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    double Ret = mapToScene(mStopX, mStopY).x();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }

    return Ret;
    //return mapToScene(mStopX, mStopY).x();
}

double TItem::fGetStopY()
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    double Ret = mapToScene(mStopX, mStopY).y();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }

    return Ret;
    //return mapToScene(mStopX, mStopY).y();
}

void TItem::fSetStartPos(double StartX, double StartY)
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    QPointF Point = mapFromScene(StartX, StartY);
    mStartX = Point.x();
    mStartY = Point.y();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TItem::fSetStopPos(double StopX, double StopY)
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    QPointF Point = mapFromScene(StopX, StopY);
    mStopX = Point.x();
    mStopY = Point.y();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TItem::fSetStartPos(const QPointF &Start)
{
    fSetStartPos(Start.x(), Start.y());
}

void TItem::fSetStopPos(const QPointF &Stop)
{
    fSetStopPos(Stop.x(), Stop.y());
}

void TItem::fMovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    QPointF Point1 = mapFromScene(NewStartX, NewStartY);
    QPointF Point2 = mapFromScene(NewStopX, NewStopY);

    double dx1 = (Point1.x() - mStartX) / 2;
    double dy1 = (Point1.y() - mStartY) / 2;
    double dx2 = (Point2.x() - mStopX) / 2;
    double dy2 = (Point2.y() - mStopY) / 2;

    mStartX += dx1;
    mStartY += dy1;
    mStopX -= dx1;
    mStopY -= dy1;
    moveBy(dx1, dy1);

    mStartX -= dx2;
    mStartY -= dy2;
    mStopX += dx2;
    mStopY += dy2;
    moveBy(dx2, dy2);
}

void TItem::fMoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fMovePos(NewStartX, NewStartY, NewStopX, NewStopY);
    fTransFormPos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TItem::fSetLineColor(const QColor &color)
{
    if(color.isValid())
    {
        mLineColor = color;
    }
}

void TItem::fSetLineColor(int r, int g, int b, int a)
{
    mLineColor = QColor(r, g, b, a);
}

QColor TItem::fGetLineColor() const
{
    return mLineColor;
}

void TItem::fSetLineStyle(const Qt::PenStyle &style)
{
    mLineStyle = style;
}

void TItem::fSetLineStyle(int style)
{
    mLineStyle = Qt::PenStyle(style);
}

Qt::PenStyle TItem::fGetLineStyle() const
{
    return mLineStyle;
}

void TItem::fSetLineWidth(int width)
{
    mLineWidth = width;
}

int TItem::fGetLineWidth() const
{
    return mLineWidth;
}

void TItem::fSetBackGroundColor(const QColor &color)
{
    if(color.isValid())
    {
        mBackGroundColor = color;
    }
}

void TItem::fSetBackGroundColor(int r, int g, int b, int a)
{
    mBackGroundColor = QColor(r, g, b, a);
}

QColor TItem::fGetBackGroundColor() const
{
    return mBackGroundColor;
}

void TItem::fSetNameId(const QString &NameId)
{
    mNameId = NameId;
}

QString TItem::fGetNameId()
{
    return mNameId;
}

bool TItem::fSetRotateAngle(double Angle)
{
    return fDoRotate(Angle - mRotateAngle);
}

bool TItem::fDoRotate(double Angle)
{
    if(ABS(Angle) < 0.000001)
    {
        return false;
    }

    mRotateAngle += Angle;

    QTransform transform(this->transform());
    transform.translate(mRotateCenterX, mRotateCenterY);
    transform.rotate(Angle);
    transform.translate(-mRotateCenterX, -mRotateCenterY);
    this->setTransform(transform);

    return true;
}

double TItem::fGetRotateAngle()
{
    return mRotateAngle;
}

bool TItem::fSetRotateCenter(double X, double Y)
{
#if 1
    QPointF Point = mapFromScene(X, Y);

    mRotateCenterX = Point.x();
    mRotateCenterY = Point.y();
#else
    mRotateCenterX = X;
    mRotateCenterY = Y;
#endif
    return true;
}

double TItem::fGetRotateCenterX()
{
    //return mRotateCenterX;
    return mapToScene(mRotateCenterX, mRotateCenterY).x();
}

double TItem::fGetRotateCenterY()
{
    //return mRotateCenterY;
    return mapToScene(mRotateCenterX, mRotateCenterY).y();
}

void TItem::fSetZValue(double Z)
{
    setZValue(Z);
}

double TItem::fGetZValue()
{
    return zValue();
}

void TItem::fUpdate()
{
    update();
}
