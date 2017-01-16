#include "Scene.h"
#include "../Scada/Dialog/Style/DialogStyle.h"
#include "../../../Maker/Maker.h"
#include <QPointF>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

TScene::TScene(QObject *parent) : QGraphicsScene(parent)
{
    mId = 0;
    mName = tr("");

    mStartDropX = 0;
    mStartDropY = 0;

    mUserData = 0;

    mSelectedBeforeDrop = false;
}

TScene::TScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x, y, width, height, parent)
{
    mId = 0;
    mName = tr("");

    mStartDropX = 0;
    mStartDropY = 0;

    mSelectedBeforeDrop = false;
}

void TScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mStartDropX = event->scenePos().x();
    mStartDropY = event->scenePos().y();

    QTransform deviceTransform;

    QGraphicsItem *UnderItem = itemAt(mStartDropX, mStartDropY, deviceTransform);

    if(UnderItem && UnderItem->isSelected())
    {
        mSelectedBeforeDrop = true;
    }

    TGraphics *Graphics = dynamic_cast<TGraphics *>(UnderItem);

    if(Graphics)
    {
        emit mClickItem(Graphics);
    }

    //qDebug() << "TScene:" << QString::number(mStartDropX) << "," << QString::number(mStartDropY);

    QGraphicsScene::mousePressEvent(event);
}

void TScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit mDropFinsh(mStartDropX, mStartDropY, event->scenePos().x(), event->scenePos().y());

    if(mSelectedBeforeDrop && selectedItems().length() > 0)
    {
        QList<TItem *> ItemList;
        fGetItemsSelected(ItemList);

        bool SendEmit = true;

        foreach(TItem *Item, ItemList)
        {
            if(!Item->fGetMoveAble())
            {
                SendEmit = false;
                break;
            }
        }

        if(ItemList.length() > 0 && SendEmit)
        {
            emit mMoveItemList(this, ItemList,
                               mStartDropX, mStartDropY,
                               event->scenePos().x(), event->scenePos().y());
        }
    }

    mSelectedBeforeDrop = false;

    QGraphicsScene::mouseReleaseEvent(event);
}

void TScene::keyReleaseEvent(QKeyEvent *event)
{
    //qDebug() << "TScene mText" << event->text();

    if(selectedItems().length() <= 0)
    {
        return;
    }

    TItem *Item = dynamic_cast<TItem *>(selectedItems().first());

    if(Item)
    {
        Item->keyReleaseEvent(event);
    }
}

void TScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);

#ifdef MAKER
    QList<TItem *> items;

    this->fGetItemsSelected(items);

    if(items.count() <=0)
    {
        return ;
    }

    DialogStyle *mStyle = new DialogStyle(items.first(), Maker::maker->mServiceList);                                                              //新增修改

    if(mStyle->mInit)
    {
        mStyle->exec();
    }

    delete mStyle;
#endif
}

void TScene::fSetId(int Id)
{
    mId = Id;
}

int TScene::fGetId()
{
    return mId;
}

void TScene::fSetName(QString Name)
{
    mName = Name;
}

QString TScene::fGetName()
{
    return mName;
}

void TScene::fSetBackColor(const QColor &Color)
{
    mBackColor = Color;

    setBackgroundBrush(mBackColor);
}

void TScene::fSetBackColor(int R, int G, int B)
{
    fSetBackColor(QColor(R, G, B));
}

QColor TScene::fGetBackColor()
{
    return mBackColor;
}

double TScene::fGetPosX()
{
    return sceneRect().x();
}

double TScene::fGetPosY()
{
    return sceneRect().y();
}

double TScene::fGetWidth()
{
    return sceneRect().width();
}

double TScene::fGetHeight()
{
    return sceneRect().height();
}

void TScene::fSetUserData(void *UserData)
{
    mUserData = UserData;
}

void* TScene::fGetUserData()
{
    return mUserData;
}

TItem* TScene::fGetItemByNameId(QList<TScene *> &SceneList, const QString &NameId)
{
    if(NameId.length() <= 0)
    {
        return NULL;
    }

    foreach(TScene *Scene, SceneList)
    {
        QList<TItemEx *>ItemExList;
        Scene->fGetItemsList(ItemExList);

        foreach(TItemEx *ItemEx, ItemExList)
        {
            if(NameId == ItemEx->fGetNameId())
            {
                return ItemEx;
            }
        }
    }

    return NULL;
}

QObject* TScene::fGetItemByNameId(const QString &NameId)
{
    //qDebug() << "TScene::fGetItemByNameId";

    QList<TItemEx *>ItemExList;
    fGetItemsList(ItemExList);

    foreach(TItemEx *ItemEx, ItemExList)
    {
        if(NameId == ItemEx->fGetNameId())
        {
            //qDebug() << "TScene::fGetItemByNameId" << (void *)ItemEx << (void *)dynamic_cast<TItem *>(ItemEx);
            return ItemEx;
        }
    }

    return NULL;
}
