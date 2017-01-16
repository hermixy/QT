#ifndef ACTION_H
#define ACTION_H

#include "../Item/Item.h"
#include "../Scene/Scene.h"

#if 1
#include <QAction>
#else
#include <QtWidgets/QAction>
#endif

class TAction : public QAction
{
    Q_OBJECT

public:
    TAction(const QString &text, QObject* parent = 0);

    int mType;

    QList<TItem *>mItemList;
    TScene *mScene;
};

#endif // ACTION_H
