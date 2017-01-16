#ifndef DATA_BASE_SERVICE_H
#define DATA_BASE_SERVICE_H

#include "DataBase.h"
#include "DataBaseMsg.h"
#include "../Service/Service.h"
#include "../Item/Item.h"
#include "../Graphics/Group.h"
#include "../Scene/Scene.h"
#include "../Linkage/Linkage.h"

#include <QApplication>

class TDataBaseService
{
    Q_DECLARE_TR_FUNCTIONS(TDataBaseService)

public:
    TDataBaseService();
    virtual ~TDataBaseService();

    // 清空数据库
    static bool fClear(TDataBase *DataBase);

    // 保存至数据库
    static bool fSaveService(TDataBase *DataBase, const QList<TService *>ServiceList);
    static bool fSaveScene(TDataBase *DataBase, const QList<TScene *>SceneList);
    static bool fSaveLinkage(TDataBase *DataBase, const QList<TLinkage *> LinkageList);

    // 从数据库读取
    static bool fLoadService(TDataBase *DataBase, QList<TService *> &ServiceList);
    static bool fLoadLinkage(TDataBase *DataBase, QList<TLinkage *> &LinkageList);
    static bool fLoadScene(TDataBase *DataBase, QList<TScene *> &SceneList);

    // 创建数据库对象
    static TDataBase* fCreateDataBase(TDataBaseMsg *DataBaseMsg);

private:
    static bool fSaveItem(TDataBase *DataBase, int SceneId, TItemEx *Item, int ItemIdIndex, TGroup *GroupParent = NULL, int GroupId = 0);

    static QList<TItem *> fLoadItems(TDataBase *DataBase, int SceneId, int GroupId = 0);
    static TItem* fCreateItem(TDataBase *DataBase, int SceneId, const QList<QVariant>& SqlItem);
};

#endif // DATA_BASE_SERVICE_H
