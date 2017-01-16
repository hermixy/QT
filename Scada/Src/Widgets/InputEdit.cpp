#include "InputEdit.h"

#include <QKeyEvent>
#include <QDebug>

TInputEdit::TInputEdit(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor) :
       TWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::InputEdit);

    mText = tr("");
    TItem::fSetSelectAble(true);
}

TInputEdit::~TInputEdit()
{}

TItem* TInputEdit::fCopy()
{
    TInputEdit *InputEdit = new TInputEdit();

    InputEdit->fCopy(this);

    return InputEdit;
}

void TInputEdit::fCopy(TInputEdit *InputEditFrom)
{
    if(NULL == InputEditFrom)
    {
        return;
    }

    TWidgets::fCopy(InputEditFrom);

    this->fSetText(InputEditFrom->fGetText());
}

//void TInputEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
void TInputEdit::fDraw(QPainter *painter)
{
    //fBeforePaint(painter, option, widget);

    QFont font = painter->font();
    font.setPointSize(0.7 * (mStopY - mStartY) + 1);
    painter->setFont(font);

    //qDebug() << "TInputEdit::paint" << mText;

    painter->drawRect(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY);
    painter->drawText(mStartX,
                        mStartY,
                        mStopX - mStartX,
                        mStopY - mStartY,
                        Qt::AlignLeft | Qt::AlignVCenter,
                        mText);
}

bool TInputEdit::fSetSelectAble(bool Able)
{
    Q_UNUSED(Able);

    return false;
}

bool TInputEdit::fGetSelectAble()
{
    return mSelectAble;
}

void TInputEdit::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "mText" << mText << event->text() << event->key();

    bool Changed = false;

    if(16777219 != event->key())
    {
        if(event->text().length() > 0)
        {
            mText += event->text();

            Changed = true;
        }
    }
    else
    {
        if(mText.length() > 0)
        {
            mText = mText.left(mText.length() - 1);

            Changed = true;
        }
    }

    fUpdate();

    if(Changed)
    {
        mJsWidgets.fRun(this, tr("fPlay"));

        emit mTextChange(mText);
    }

    QGraphicsItem::keyReleaseEvent(event);
}

void TInputEdit::fSetText(const QString &Text)
{
    mText = Text;
}

QString TInputEdit::fGetText()
{
    return mText;
}

