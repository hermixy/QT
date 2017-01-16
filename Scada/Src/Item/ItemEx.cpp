#include "ItemEx.h"
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QDebug>
#include "../Graphics/Graphics.h"
#include "../Graphics/ValueText.h"
#include "QHBoxLayout"
#include <QDialog>
#include <QInputDialog>
#ifdef RUNNER
#include "Runer.h"
#endif

TItemEx::TItemEx(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    TItem(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    mNameId = tr("");
    mUserDataName1 = tr("");
    mUserDataName2 = tr("");
    mUserData1 = 0;
    mUserData2 = 0;

    mHostScene = NULL;
}

TItemEx::~TItemEx()
{}

void TItemEx::fCopy(TItemEx *ItemExFrom)
{
    if(NULL == ItemExFrom)
    {
        return;
    }

    this->fSetNameId(ItemExFrom->fGetNameId());
    this->fSetServiceId(ItemExFrom->fGetServiceId());
    this->fSetHint(ItemExFrom->fGetHint());

    this->fSetUserDataName1(ItemExFrom->fGetUserDataName1());
    this->fSetUserData1(ItemExFrom->fGetUserData1());
    this->fSetUserDataName2(ItemExFrom->fGetUserDataName2());
    this->fSetUserData2(ItemExFrom->fGetUserData2());

    this->fSetStartPos(QPointF(ItemExFrom->fGetCurrentStartX(), ItemExFrom->fGetCurrentStartY()));
    this->fSetStopPos(QPointF(ItemExFrom->fGetCurrentStopX(), ItemExFrom->fGetCurrentStopY()));
    this->fSetBackGroundColor(ItemExFrom->fGetBackGroundColor());
    this->fSetLineColor(ItemExFrom->fGetLineColor());
    this->fSetLineWidth(ItemExFrom->fGetLineWidth());
    this->fSetLineStyle(ItemExFrom->fGetLineStyle());
    qDebug()<<ItemExFrom->fGetZValue();
    this->fSetZValue(ItemExFrom->fGetZValue());
    this->showStart=ItemExFrom->showStart;
    this->showStop=ItemExFrom->showStop;

}

void TItemEx::fSetNameId(const QString &NameId)
{
    mNameId = NameId;
}

QString TItemEx::fGetNameId() const
{
    return mNameId;
}

void TItemEx::fSetServiceId(int Id)
{
    mServiceId = Id;
}

int TItemEx::fGetServiceId() const
{
    return mServiceId;
}


void TItemEx::fSetHint(const QString &Hint)
{
    mHint = Hint;
}

QString TItemEx::fGetHint() const
{
    return mHint;
}

void TItemEx::fSetHostScene(QObject *HostScene)
{
    mHostScene = HostScene;
}

QObject* TItemEx::fGetHostScene() const
{
    return mHostScene;
}

void TItemEx::fSetUserDataName1(const QString &UserDataName)
{
    mUserDataName1 = UserDataName;
}

QString TItemEx::fGetUserDataName1() const
{
    return mUserDataName1;
}

void TItemEx::fSetUserDataName2(const QString &UserDataName)
{
    mUserDataName2 = UserDataName;
}

QString TItemEx::fGetUserDataName2() const
{
    return mUserDataName2;
}

void TItemEx::fSetUserData1(const double UserData)
{
    mUserData1 = UserData;
}

double TItemEx::fGetUserData1() const
{
    return mUserData1;
}

void TItemEx::fSetUserData2(const double UserData)
{
    mUserData2 = UserData;
}

double TItemEx::fGetUserData2() const
{
    return mUserData2;
}

void TItemEx::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
Q_UNUSED(event)
    //if(Runer::runner->userAccess)
    {
    QMenu menu;
    TGraphics *Graphics = dynamic_cast<TGraphics *>(this);
    if(Graphics)
    {
        menu.addAction("参考值设定", this, SLOT(setStableValue()));
        menu.addAction(tr("联动启用时间"), this, SLOT(setLinkStart()));
        TValueText *tValueText = dynamic_cast<TValueText *>(this);
        if(tValueText)
        {
            menu.addAction(tr("百分比设置"), this, SLOT(setpercent()));
        }
    }
    menu.exec(QCursor::pos());
    }

}

void TItemEx::setStableValue()
{

}
void TItemEx::setLinkStart()
{

}
void TItemEx::setpercent()
{

}
