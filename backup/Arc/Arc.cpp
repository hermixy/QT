#include "Arc.h"

TArc::TArc(double StartX, double StartY, double StopX, double StopY, double StartAngle, double CoverAngle,
             const Qt::PenStyle &LineSyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineSyle, LineWidth, LineColor, BackColor)
{
    mStartAngle = StartAngle;
    mCoverAngle = CoverAngle;

    fSetType(TItem::Arc);
}

TArc::~TArc()
{}

TItem *TArc::fCopy()
{
    TArc *mArc = new TArc(this->fGetStartX(), this->fGetStartY(), this->fGetStopX(),
                                this->fGetStopY(), this->fGetStartAngle(), this->fGetCoverAngle(),
                                this->fGetLineStyle(), this->fGetLineWidth(),
                                this->fGetLineColor(), this->fGetBackGroundColor());

    BaseMsgCopy(mArc, this);

    return mArc;
}

void TArc::fSetStartAngle(const double StartAngle)
{
    mStartAngle = StartAngle;
}

double TArc::fGetStartAngle() const
{
    return mStartAngle;
}

void TArc::fSetCoverAngle(const double CoverAngle)
{
    mCoverAngle = CoverAngle;
}

double TArc::fGetCoverAngle() const
{
    return mCoverAngle;
}

void TArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawArc(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
}
