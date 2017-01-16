#ifndef XML_READER_H
#define XML_READER_H

#include "../View/View.h"
#include "../Scene/Scene.h"
#include "../Item/Item.h"
#include "../Graphics/GraphicsPub.h"
#include "../Widgets/WidgetsPub.h"

#include "../DataBase/DataBaseMsg.h"
#include "../Linkage/Linkage.h"
#include "../Service/Service.h"
//#include "../RunMode/RunMode.h"

#include <QtXml/QDomDocument>
#include <QApplication>

class TXmlReader
{
    Q_DECLARE_TR_FUNCTIONS(TXmlWriter)

public:
    TXmlReader();
    ~TXmlReader();


    bool fOpen(QString FileName);
    bool fIsOpen();
    QDomElement fGetRoot();

    TScene* fAnalyScene(QList<TScene *> &SceneList);
    bool fAnalyService(QList<TService *> &ServiceList);
    bool fAnalyLinkage(QList<TLinkage *> &LinkageList);
    bool fAnalyDB(TDataBaseMsg *DBMsg);
    bool fAnalyCurver(QList<TScene::TimerStruct> &CurverStructList);
    bool fAnalyPie(QList<TScene::TimerStruct> &CurverStructList);

    TItem* fAnalyItem(QDomElement mXmlNode);
private:
    QDomElement fFindElement(QString Name);
    QDomDocument mDoc;
    bool mIsOpen;
};

#endif // XML_READER_H
