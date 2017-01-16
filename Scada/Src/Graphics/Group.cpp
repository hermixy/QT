#include "Group.h"
#include "../Base/LibBase.h"

#include <QStyleOptionGraphicsItem>
#include <QDebug>

TGroup::TGroup() : TGraphics(0, 0, 0, 0)
{
    fSetType(TItem::Group);

    TItem::fSetMoveAble(true);
    TItem::fSetDragAble(false);
}

TGroup::~TGroup()
{}

TItem *TGroup::fCopy()
{
    TGroup *Group = new TGroup();

    Group->fCopy(this);

    return Group;
}

void TGroup::fCopy(TGroup *GroupFrom)
{
    if(NULL == GroupFrom)
    {
        return;
    }

    TGraphics::fCopy(GroupFrom);

    this->fAddItems(GroupFrom->fGetItemsList());
}

bool TGroup::fSetDragAble(bool Able)
{
    Q_UNUSED(Able);
    return false;
}

void TGroup::fAddItemToList(TItem *AddItem, TGroup *ParentGroup)
{    
    if(NULL == AddItem)
    {
        return;
    }

    // 设置为不可选中
    AddItem->fSetSelectAble(false);

    // 保存最大线宽为Group的线宽，在boundingRect中确定边界用
    // 不能调用函数fSetLineWidth,否则所有图形的线宽都设置为最大线宽
    mLineWidth = MAX(mLineWidth, AddItem->fGetLineWidth());

    if(ParentGroup)
    {
        AddItem->fSetStartPos(ParentGroup->fGetItemStartPos(AddItem));
        AddItem->fSetStopPos(ParentGroup->fGetItemStopPos(AddItem));
    }

    // 求最外边框，对 线 等进行兼容
    double AddItemStartX = MIN(AddItem->fGetStartX(), AddItem->fGetStopX());
    double AddItemStartY = MIN(AddItem->fGetStartY(), AddItem->fGetStopY());
    double AddItemStopX = MAX(AddItem->fGetStartX(), AddItem->fGetStopX());
    double AddItemStopY = MAX(AddItem->fGetStartY(), AddItem->fGetStopY());

    if(0 == mItemsList.count())
    {
        mItemsList.push_back(AddItem);

        fSetStartPos(AddItemStartX, AddItemStartY);
        fSetStopPos(AddItemStopX, AddItemStopY);
    }
    else
    {
        // 将所有元素按照Z值从小到大排列，Z值小的先绘制
        bool Find = false;
        for(int i = 0; i < mItemsList.length(); ++i)
        {
            if(AddItem->fGetZValue() < mItemsList.at(i)->fGetZValue())
            {
                Find = true;
                mItemsList.insert(i, AddItem);
                break;
            }
        }

        if(!Find)
        {
            mItemsList.push_back(AddItem);
        }

        fSetStartPos(MIN(fGetStartX(), AddItemStartX), MIN(fGetStartY(), AddItemStartY));
        fSetStopPos(MAX(fGetStopX(), AddItemStopX), MAX(fGetStopY(), AddItemStopY));
    }
}

void TGroup::fAddItem(TItem *Item)
{
    if(NULL == Item)
    {
        return;
    }

    if(TItem::Group == Item->fGetType())
    {
        TGroup *Group = dynamic_cast<TGroup *>(Item);

        if(Group)
        {
            // 添加的是Group
            foreach(TItem *AddItem, Group->fGetItemsList())
            {
                fAddItemToList(AddItem->fCopy(), Group);
            }
        }
    }
    else
    {
        fAddItemToList(Item->fCopy());
    }
}

QList<TItem *> TGroup::fGetItemsList() const
{
    return mItemsList;
}

void TGroup::fAddItems(const QList<TItem *> &ItemsList)
{
    foreach(TItem *Item, ItemsList)
    {
        fAddItem(Item);
    }

    update();
}

void TGroup::fSetLineColor(const QColor &color)
{
    foreach(TItem *Item, mItemsList)
    {
        Item->fSetLineColor(color);
    }

    TItem::fSetLineColor(color);
}

void TGroup::fSetLineColor(int r, int g, int b, int a)
{
    this->fSetLineColor(QColor(r, g, b, a));
}

void TGroup::fSetLineStyle(const Qt::PenStyle &style)
{
    foreach(TItem *Item, mItemsList)
    {
        Item->fSetLineStyle(style);
    }

    TItem::fSetLineStyle(style);
}

void TGroup::fSetLineStyle(int style)
{
    this->fSetLineStyle(Qt::PenStyle(style));
}

void TGroup::fSetLineWidth(int width)
{
    foreach(TItem *Item, mItemsList)
    {
        Item->fSetLineWidth(width);
    }

    TItem::fSetLineWidth(width);
}

void TGroup::fSetBackGroundColor(const QColor &color)
{
    foreach(TItem *Item, mItemsList)
    {
        Item->fSetBackGroundColor(color);
    }

    TItem::fSetBackGroundColor(color);
}

void TGroup::fSetBackGroundColor(int r, int g, int b, int a)
{
    this->fSetBackGroundColor(QColor(r, g, b, a));
}

QPointF TGroup::fGetItemStartPos(TItem *Item)
{
    QPointF PointF(0, 0);

    if(Item)
    {
        double SaveAngle = fGetRotateAngle();

        if(0 != SaveAngle)
        {
            fSetRotateAngle(0);
        }

        PointF = mapToScene(Item->fGetStartX(), Item->fGetStartY());

        if(0 != SaveAngle)
        {
            fSetRotateAngle(SaveAngle);
        }
    }

    return PointF;
}

QPointF TGroup::fGetItemStopPos(TItem *Item)
{
    QPointF PointF(0, 0);

    if(Item)
    {
        double SaveAngle = fGetRotateAngle();

        if(0 != SaveAngle)
        {
            fSetRotateAngle(0);
        }

        PointF = mapToScene(Item->fGetStopX(), Item->fGetStopY());

        if(0 != SaveAngle)
        {
            fSetRotateAngle(SaveAngle);
        }
    }

    return PointF;
}

void TGroup::fDraw(QPainter* painter)
{
    Q_UNUSED(painter);
}

void TGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget);

    foreach(TItem *Item, mItemsList)
    {
        Item->paint(painter, option, widget);
    }
}

