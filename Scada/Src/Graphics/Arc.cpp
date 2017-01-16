#include "Arc.h"

TArc::TArc(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    mMode = MODE_PIE;

    mStartAngle = 45;
    mCoverAngle = 90;

    fSetType(TItem::Arc);
}

TArc::~TArc()
{}

TItem *TArc::fCopy()
{
    TArc *Arc = new TArc();

    Arc->fCopy(this);

    return Arc;
}

void TArc::fCopy(TArc *ArcFrom)
{
    if(NULL == ArcFrom)
    {
        return;
    }

    TGraphics::fCopy(ArcFrom);

    this->fSetMode(ArcFrom->fGetMode());
    this->fSetStartAngle(ArcFrom->fGetStartAngle());
    this->fSetCoverAngle(ArcFrom->fGetCoverAngle());
}

bool TArc::fSetMode(const int Mode)
{
    if(Mode > MODE_MIN && Mode < MODE_MAX)
    {
        mMode = Mode;
        return true;
    }
    else
    {
        return false;
    }
}

int TArc::fGetMode()
{
    return mMode;
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

void TArc::fDraw(QPainter *painter)
{
    switch(mMode)
    {
    case MODE_PIE:
        painter->drawPie(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    case MODE_ARC:
        painter->drawArc(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    case MODE_CHORD:
        painter->drawChord(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    default:
        break;
    }
}

/*void TArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainter *PaintStyle = fBeforePaint(painter, option, widget);

    switch(mMode)
    {
    case MODE_PIE:
        PaintStyle->drawPie(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    case MODE_ARC:
        PaintStyle->drawArc(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    case MODE_CHORD:
        PaintStyle->drawChord(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, mStartAngle * 16, mCoverAngle * 16);
        break;

    default:
        break;
    }
}*/
