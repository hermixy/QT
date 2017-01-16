#ifndef UNDO_H
#define UNDO_H

#include "../Scene/Scene.h"
#include "../Item/Item.h"

#include <QUndoCommand>

class TUndo : public QUndoCommand
{
public:
    TUndo();
    TUndo(TScene * Scene, QList<TItem *> &ItemList, QUndoCommand * Parent = NULL);

    virtual void undo();
    virtual void redo();

    void fSetScene(TScene *Scene);
    void fAddItem(TItem *Item);

protected:
    TScene *mScene;
    QList<TItem *>mList;
};

#endif // UNDO_H
