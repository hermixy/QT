#ifndef TSCENE_H
#define TSCENE_H

#include "../Item/Item.h"
#include "../Graphics/Graphics.h"
#include "../Widgets/Widgets.h"

#include <QGraphicsScene>
class QGraphicsSceneMouseEvent;

class TScene : public QGraphicsScene
{
    Q_OBJECT

public:
    TScene(QObject *parent = 0);
    TScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    template<typename ItemType> void fGetItemsList(QList<ItemType *> &List)
    {
        List.clear();

        foreach(QGraphicsItem *mGraphicsItem, items())
        {
            ItemType *FindItem = dynamic_cast<ItemType *>(mGraphicsItem);

            if(FindItem)
            {
                List.push_back(FindItem);
            }
        }
    }

    template<typename ItemType> void fGetItemsSelected(QList<ItemType *> &List)
    {
        List.clear();

        foreach(QGraphicsItem *mGraphicsItem, selectedItems())
        {
            ItemType *Item = dynamic_cast<ItemType *>(mGraphicsItem);

            if(Item)
            {
                List.push_back(Item);
            }
        }
    }

    static TItem* fGetItemByNameId(QList<TScene *> &SceneList, const QString &NameId);
    struct TimerStruct{
        QString TimerId;
        int serverId=-1;
        int mainId=-1;
        int devId=-1;
        int startAddr=-1;
        double value=-1;
    };
    QList<TimerStruct> TImerStructList;

public slots:
    QObject* fGetItemByNameId(const QString &NameId);

    void fSetId(int Id);
    int fGetId();
    void fSetName(QString Name);
    QString fGetName();
    void fSetBackColor(const QColor &Color);
    void fSetBackColor(int R, int G, int B);
    QColor fGetBackColor();

    double fGetPosX();
    double fGetPosY();
    double fGetWidth();
    double fGetHeight();

    void fSetUserData(void *UserData);
    void* fGetUserData();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);// 鼠标按下
    //void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);// 鼠标松开
    void keyReleaseEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); //鼠标双击弹出属性


Q_SIGNALS:
    void mDropFinsh(double StartX, double StartY, double StopX, double StopY);
    void mClickItem(TGraphics *Graphics);
    void mMoveItemList(TScene *Scene, QList<TItem *> ItemList,
                       double StartX, double StartY, double StopX, double StopY);

private:
    void *mUserData;
    double mStartDropX;
    double mStartDropY;

    int mId;
    QString mName;
    QColor mBackColor;

    bool mSelectedBeforeDrop;
};

#endif // TSCENE_H
