#ifndef TGROUP_H
#define TGROUP_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  组合  ////////////////////////////////////////////////////
class TGroup : public TGraphics
{
    Q_OBJECT
public:
    TGroup();
    virtual ~TGroup();

public slots:
    TItem *fCopy();
    void fCopy(TGroup *GroupFrom);

    bool fSetDragAble(bool Able);

    void fAddItems(const QList<TItem *> &ItemsList);    // 添加列表
    void fAddItem(TItem *Item);                    // 添加项

    QList<TItem *> fGetItemsList() const;

    QPointF fGetItemStartPos(TItem *Item);
    QPointF fGetItemStopPos(TItem *Item);

    // 执行以下选项时，统一采用Grop的样式，丢弃之前的样式
    void fSetLineColor(const QColor &color);
    void fSetLineColor(int r, int g, int b, int a = 255);
    void fSetLineStyle(const Qt::PenStyle &style);
    void fSetLineStyle(int style);
    void fSetLineWidth(int width);
    void fSetBackGroundColor(const QColor &color);
    void fSetBackGroundColor(int r, int g, int b, int a = 255);    

private:
    QList<TItem *> mItemsList;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter* painter);

    void fAddItemToList(TItem *AddItem, TGroup *ParentGroup = 0); // 添加项，不复制，仅坐标变换
};

#endif // TGROUP_H
