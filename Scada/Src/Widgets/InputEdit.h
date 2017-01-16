#ifndef TINPUT_EDIT_H
#define TINPUT_EDIT_H

#include "Widgets.h"

///////////////////////  控件基类 /////////////////////////////////////
class TInputEdit : public TWidgets
{
    Q_OBJECT
public:
    TInputEdit(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TInputEdit();

    void keyReleaseEvent(QKeyEvent *event);

public slots:
    TItem *fCopy();
    void fCopy(TInputEdit *InputEditFrom);

    virtual bool fSetSelectAble(bool Able);
    virtual bool fGetSelectAble();

    void fSetText(const QString &Text);
    QString fGetText();

Q_SIGNALS:
    void mTextChange(QString Text);

private:
    QString mText;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TINPUT_EDIT_H
