#include "Widgets.h"
#include "QWidget"

#include "../SendTo/SendTo.h"

#ifdef DEF_RUN_AS_RUNER
#include "../../Dialog/InputMessageBox/DialogInputMessageBox.h"
#include <QMessageBox>
#endif

#include <QDebug>

TWidgets::TWidgets(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    TItemEx(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
}

TWidgets::~TWidgets()
{}

void TWidgets::fSendTo(QObject *Sender, int Type, double ValueFloat,
                       int ValueInt, QString Text)
{
    if(fGetServiceId() <= 0)
    {
        qDebug() << "TWidgets::fSendTo fGetServiceId() <= 0 not connect";
        return;
    }

    qDebug() << "TWidgets::fSendTo " << (void *)this
             << "emit" << Sender << Type << ValueFloat << ValueInt << Text;

    bool Sign = true;

    #ifdef DEF_RUN_AS_RUNER
    if(TSendTo::TYPE_MESSAGE_INFO == Type)
    {
        QMessageBox::warning(NULL, tr("提示"), Text);
    }
    else if(TSendTo::TYPE_MESSAGE_INPUT == Type)
    {
        DialogInputMessageBox *Box = new DialogInputMessageBox(tr("请确认操作"),
                                                               Text,
                                                               tr("yes"));

        if(Box)
        {
            Box->exec();

            if(DialogInputMessageBox::RETURN_OK != Box->fGetReturn())
            {
                Sign = false;
            }

            delete Box;
        }
    }
    #endif

    if(Sign)
    {
        qDebug() << "Sign true Send";
        emit mSendTo(Sender, Type, ValueFloat, ValueInt, Text);
    }
    else
    {
        qDebug() << "Sign false do not Send";
    }
}

void TWidgets::fCopy(TWidgets *WidgetsFrom)
{
    if(NULL == WidgetsFrom)
    {
        return;
    }

    TItemEx::fCopy(WidgetsFrom);

    this->mJsWidgets.fSetName(WidgetsFrom->mJsWidgets.fGetName());
}

/*void TWidgets::fMsgCopy(QObject *To, QObject *From)
{
    TItemEx::fMsgCopy(To, From);

    TWidgets *WidgetsTo = dynamic_cast<TWidgets *>(To);
    TWidgets *WidgetsFrom = dynamic_cast<TWidgets *>(From);

    if(NULL == WidgetsTo || NULL == WidgetsFrom)
    {
        return;
    }

    WidgetsTo->mJsWidgets.fSetName(WidgetsFrom->mJsWidgets.fGetName());
}*/


