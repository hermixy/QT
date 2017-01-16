#include "ValueText.h"
#include <QDebug>

TValueText::TValueText(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TText(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor, tr("0"))
{
    fSetType(TItem::ValueText);

    mMode = MODE_FLOAT;
    mIntLen = 0;
    mPointLen = 6;
}

TValueText::~TValueText()
{}

TItem *TValueText::fCopy()
{
    TValueText *ValueText = new TValueText();

    ValueText->fCopy(this);

    return ValueText;
}

void TValueText::fCopy(TValueText *ValueTextFrom)
{
    if(NULL == ValueTextFrom)
    {
        return;
    }
    TText::fCopy(ValueTextFrom);
    this->fSetMode(ValueTextFrom->fGetMode());
    this->fSetIntLen(ValueTextFrom->fGetIntLen());
    this->fSetPointLen(ValueTextFrom->fGetPointLen());
    this->setValue=ValueTextFrom->setValue;
}

void TValueText::fSetValue(const double Value)
{

    TGraphics::fSetValue(Value);

    //qDebug() << "TValueText::fSetValue  mode:" << mMode;

    switch(mMode)
    {
    case MODE_INT:
        fSetText(QString::number(mValue));
        break;

    case MODE_FLOAT:
        fSetText(QString::number(mValue, 'g', mPointLen));
        break;

    default:
        break;
    }

    TItem::fUpdate();
}

void TValueText::fSetMode(int _Mode)
{
    mMode = _Mode;
}

int TValueText::fGetMode()
{
    return mMode;
}

void TValueText::fSetIntLen(int _IntLen)
{
    mIntLen = _IntLen;
}

int TValueText::fGetIntLen()
{
    return mIntLen;
}

void TValueText::fSetPointLen(int _PointLen)
{
    mPointLen = _PointLen;
}

int TValueText::fGetPointLen()
{
    return mPointLen;
}

void TValueText::fDraw(QPainter *painter)
{

    //qDebug()<<"valueText";
    QFont Font;
    Font.setFamily(mFontFamily);
    Font.setPointSize(mFontSize);
    Font.setBold(mFontBlod);
    Font.setItalic(mFontItalic);
    Font.setUnderline(mFontUnderLine);
    Font.setStrikeOut(mFontStrikeOut);
    Font.setLetterSpacing(QFont::AbsoluteSpacing, mFontSpacing);
    painter->setFont(Font);

    painter->drawText(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY,
                      Qt::AlignCenter, mText);
}
