#ifndef TWIDGETS_H
#define TWIDGETS_H

#include "../Item/Item.h"
#include "../Item/ItemEx.h"
#include "../Js/Js.h"

///////////////////////  控件基类 /////////////////////////////////////
class TWidgets : public TItemEx
{
    Q_OBJECT
public:
    TWidgets(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TWidgets();

    TJs mJsWidgets;

public slots:
    virtual void fCopy(TWidgets *WidgetsFrom);
    virtual void fSendTo(QObject *Sender, int Type = 0, double ValueFloat = 0,
                         int ValueInt = 0, QString Text = tr(""));

Q_SIGNALS:
    void mSendTo(QObject *Sender, int Type, double ValueFloat,
                 int ValueInt, QString Text);

//protected:
    //virtual void fMsgCopy(QObject *To, QObject *From);
};

#endif // TWIDGETS_H
