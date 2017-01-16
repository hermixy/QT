#ifndef WIDGETS_H
#define WIDGETS_H

#include "widgets_global.h"

#include "../../Src/Item/Item.h"
#include "../../Src/Js/Js.h"

class WIDGETSSHARED_EXPORT TWidgets : public TItem
{
    Q_OBJECT
public:
    TWidgets(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0);
    virtual ~TWidgets();

    TJs mJsWidgets;

public slots:
    void fSetServId(int Id);
    int fGetServId();
    virtual void fAddWidgets(TItem *Widgets);
    virtual int fGetWidgetsSum();
    virtual TItem* fGetWidgets(int Index);
    virtual QObject* fGetWidgets(const QString &NameId);
    virtual void fClearWidgets();

    virtual void fSendTo(int Type, double Value, int ValueType);

Q_SIGNALS:
    void mSendTo(int Type, double Value, int ValueType);

protected:
    int mServId;
    QList<TItem *>mWidgetsList;

    virtual void fMsgCopy(QObject *To, QObject *From);
};

#endif // WIDGETS_H
