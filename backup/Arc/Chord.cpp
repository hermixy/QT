#include "Chord.h"

TChord::TChord(double StartX, double StartY, double StopX, double StopY, double StartAngle, double CoverAngle,
             const Qt::PenStyle &LineSyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineSyle, LineWidth, LineColor, BackColor)
{
    mStartAngle = StartAngle;
    mCoverAngle = CoverAngle;
    fSetType(TItem::Chord);
}

TChord::~TChord()
{}

TItem *TChord::fCopy()
{
    TChord *mChord = new TChord(this->fGetStartX(), this->fGetStartY(), this->fGetStopX(),
                                this->fGetStopY(), this->fGetStartAngle(), this->fGetCoverAngle(),
                                this->fGetLineStyle(), this->fGetLineWidth(),
                                this->fGetLineColor(), this->fGetBackGroundColor());

    BaseMsgCopy(mChord, this);

    return mChord;
}

void TChord::fSetStartAngle(const double StartAngle)
{
    mStartAngle = StartAngle;
}

double TChord::fGetStartAngle() const
{
    return mStartAngle;
}

void TChord::fSetCoverAngle(const double CoverAngle)
{
    mCoverAngle = CoverAngle;
}

double TChord::fGetCoverAngle() const
{
    return mCoverAngle;
}

void TChord::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget)->drawChord(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
}
