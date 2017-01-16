#include "Pie.h"

TPie::TPie(double StartX, double StartY, double StopX, double StopY, double StartAngle, double CoverAngle,
             const Qt::PenStyle &LineSyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineSyle, LineWidth, LineColor, BackColor)
{
    mStartAngle = StartAngle;
    mCoverAngle = CoverAngle;
    fSetType(TItem::Pie);
}

TPie::~TPie()
{}

TItem *TPie::fCopy()
{
    TPie *mPie = new TPie(this->fGetStartX(), this->fGetStartY(), this->fGetStopX(),
                                this->fGetStopY(), this->fGetStartAngle(), this->fGetCoverAngle(),
                                this->fGetLineStyle(), this->fGetLineWidth(),
                                this->fGetLineColor(), this->fGetBackGroundColor());

    BaseMsgCopy(mPie, this);

    return mPie;
}

void TPie::fSetStartAngle(const double StartAngle)
{
    mStartAngle = StartAngle;
}

double TPie::fGetStartAngle() const
{
    return mStartAngle;
}

void TPie::fSetCoverAngle(const double CoverAngle)
{
    mCoverAngle = CoverAngle;
}

double TPie::fGetCoverAngle() const
{
    return mCoverAngle;
}

void TPie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawPie(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
}
