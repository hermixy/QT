#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "../Item/Item.h"
#include "../Graphics/GraphicsPub.h"
#include "../Widgets/WidgetsPub.h"

#include <QtXml/QDomDocument>
#include <QApplication>

class TXmlWriter
{
    Q_DECLARE_TR_FUNCTIONS(TXmlWriter)

public:
    TXmlWriter();
    ~TXmlWriter();
    void fAddHead();
    QDomElement fAddRoot(const QString Root);
    QDomElement fAddChild(QDomElement &Parent, const QString Name);
    bool fAddItem(QDomElement &DomParent, TItem *Item, TGroup *GroupParent = 0);

    bool fSave(QString FileName);

private:
    QDomDocument mDoc;

    bool fInsertItemEx(QDomElement &XmlItem, TItem *Item);
    bool fInsertGraphics(QDomElement &XmlItem, TItem *Item);
    bool fInsertGroup(QDomElement &XmlItem, TItem *Item);
    bool fInsertPolygon(QDomElement &XmlItem, TItem *Item);
    bool fInsertArc(QDomElement &XmlItem, TItem *Item);
    bool fInsertText(QDomElement &XmlItem, TItem *Item);
    bool fInsertValueText(QDomElement &XmlItem, TItem *Item);
    bool fInsertPixmap(QDomElement &XmlItem, TItem *Item);
    bool fInsertCurve(QDomElement &XmlItem, TItem *Item);
    bool fInsertMeter(QDomElement &XmlItem, TItem *Item);
    bool fInsertProcessBar(QDomElement &XmlItem, TItem *Item);

    bool fInsertWidgets(QDomElement &XmlItem, TItem *Item);
    bool fInsertTable(QDomElement &XmlItem, TItem *Item);
    bool fInsertButton(QDomElement &XmlItem, TItem *Item);
    bool fInsertInput(QDomElement &XmlItem, TItem *Item);
};

#endif // XML_WRITER_H
