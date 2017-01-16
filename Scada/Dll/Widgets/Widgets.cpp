#include "Widgets.h"

#include <QDebug>

TWidgets::TWidgets(double StartX, double StartY, double StopX, double StopY) :
        TItem(StartX, StartY, StopX, StopY)
{
    mServId = 0;
}

TWidgets::~TWidgets()
{}

void TWidgets::fSetServId(int Id)
{
    mServId = Id;
}

int TWidgets::fGetServId()
{
    return mServId;
}

void TWidgets::fAddWidgets(TItem *Widgets)
{
    if(Widgets)
    {
        mWidgetsList.push_back(Widgets);
    }
}

int TWidgets::fGetWidgetsSum()
{
    return mWidgetsList.length();
}

TItem* TWidgets::fGetWidgets(int Index)
{
    //qDebug() << "TWidgets::fGetWidgets(int Index)" << Index;
    if(Index >= 0 && Index < mWidgetsList.length())
    {
        return mWidgetsList.at(Index);
    }

    //qDebug() << "TWidgets::fGetWidgets(int Index) return null";

    return NULL;
}

QObject* TWidgets::fGetWidgets(const QString &NameId)
{
    //qDebug() << "TWidgets::fGetWidgets(const QString &NameId)" << NameId;

    foreach(TItem *Widgets, mWidgetsList)
    {
        if(NameId == Widgets->fGetNameId())
        {
            //qDebug() << "TWidgets::fGetWidgets(const QString &NameId) return" << (void *)Widgets;
            return Widgets;
        }
    }

    //qDebug() << "TWidgets::fGetWidgets(const QString &NameId) return NULL";

    return NULL;
}

void TWidgets::fClearWidgets()
{
    mWidgetsList.clear();
}

void TWidgets::fSendTo(int Type, double Value, int ValueType)
{
    qDebug() << "TWidgets::fSendTo" << Type << Value << ValueType;
    emit mSendTo(Type, Value, ValueType);
}

void TWidgets::fMsgCopy(QObject *To, QObject *From)
{
    TItem::fMsgCopy(To, From);

    TWidgets *WidgetsTo = dynamic_cast<TWidgets *>(To);
    TWidgets *WidgetsFrom = dynamic_cast<TWidgets *>(From);

    if(NULL == WidgetsTo || NULL == WidgetsFrom)
    {
        return;
    }

    WidgetsTo->fSetNameId(WidgetsFrom->fGetNameId());

    WidgetsTo->mJsWidgets.fSetName(WidgetsFrom->mJsWidgets.fGetName());

    for(int i = 0; i < WidgetsFrom->fGetWidgetsSum(); i++)
    {
        WidgetsTo->fAddWidgets(WidgetsFrom->fGetWidgets(i));
    }
}

