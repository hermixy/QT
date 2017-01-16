#include "Line.h"
#include "../Base/LibBase.h"
#include "../Math/Math.h"

#include <QCursor>
#include <QGraphicsSceneEvent>

TLine::TLine(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Line);

    mStartX = StartX;
    mStartY = StartY;
    mStopX = StopX;
    mStopY = StopY;

    fUpdatePos();
}

TLine::~TLine()
{}

QRectF TLine::boundingRect() const
{
    QRectF mRect(mPosX, mPosY, mWidth, mHeight);

    const int padding = (mLineWidth + 1) / 2;
    mRect.adjust(-padding, -padding, +padding, +padding);

    return mRect;
}

void TLine::fUpdatePos()
{
    mPosX = MIN(mStartX, mStopX);
    mPosY = MIN(mStartY, mStopY);
    mWidth = ABS(mStopX - mStartX);
    mHeight = ABS(mStopY - mStartY);
}

void TLine::fSetStartPos(double StartX, double StartY)
{
    TItem::fSetStartPos(StartX, StartY);

    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fUpdatePos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TLine::fSetStopPos(double StopX, double StopY)
{
    TItem::fSetStopPos(StopX, StopY);

    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fUpdatePos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TLine::fSetStartPos(const QPointF &Start)
{
    TItem::fSetStartPos(Start);

    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fUpdatePos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TLine::fSetStopPos(const QPointF &Stop)
{
    TItem::fSetStopPos(Stop);

    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fUpdatePos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

void TLine::fMoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    double SaveAngle = fGetRotateAngle();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(0);
    }

    fMovePos(NewStartX, NewStartY, NewStopX, NewStopY);
    fUpdatePos();

    if(0 != SaveAngle)
    {
        fSetRotateAngle(SaveAngle);
    }
}

TItem *TLine::fCopy()
{
    TLine *Line = new TLine();

    Line->fCopy(this);

    return Line;
}

void TLine::fCopy(TLine *LineFrom)
{
    if(NULL == LineFrom)
    {
        return;
    }

    TGraphics::fCopy(LineFrom);

    this->fUpdatePos();
}

void TLine::UpdateMouse(QPointF &PosNow)
{
    QPointF Start = this->scenePos() + QPointF(mStartX, mStartY);
    QPointF Stop = this->scenePos() + QPointF(mStopX, mStopY);

    int Add = mLineWidth + 2;

    QCursor mCursor;

    if(ABS(PosNow.x() - Start.x()) < Add && ABS(PosNow.y() - Start.y()) < Add)      // 左上
    {
        mDropDirection = 8;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(ABS(PosNow.x() - Stop.x()) < Add && ABS(PosNow.y() - Stop.y()) < Add) // 右下
    {
        mDropDirection = 4;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else
    {
        mDropDirection = 0;
        mCursor.setShape(Qt::ArrowCursor);
    }

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

void TLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "mouseMoveEvent";
    if(mDropScale)
    {
        QPointF dis = event->scenePos() - mDropPos;
        mDropPos = event->scenePos();

        double DisX = dis.x() / 2;
        double DisY = dis.y() / 2;

        switch(mDropDirection)
        {
        case 8: // 起点
            {
                mStartX += DisX;
                mStartY += DisY;
                mStopX -= DisX;
                mStopY -= DisY;

                fUpdatePos();

                update(boundingRect());
                moveBy(DisX, DisY);

                break;
            }

        case 4: // 终点
            {
                mStartX -= DisX;
                mStartY -= DisY;
                mStopX += DisX;
                mStopY += DisY;

                fUpdatePos();

                update(boundingRect());
                moveBy(DisX, DisY);

                break;
            }

        default:
            break;
        }
    }

    fUpdate();

    if(mMoveAble)
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void TLine::fDraw(QPainter *painter)
{
    painter->drawLine(mStartX, mStartY, mStopX, mStopY);
}

/*void TLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawLine(mStartX, mStartY, mStopX, mStopY);
}*/
