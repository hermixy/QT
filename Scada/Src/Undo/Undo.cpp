#include "Undo.h"

TUndo::TUndo()
{
    mScene = NULL;
}

TUndo::TUndo(TScene * Scene, QList<TItem *> &ItemList, QUndoCommand * Parent)
    : QUndoCommand(Parent)
{
    mScene = Scene;
    mList = ItemList;
}

void TUndo::fSetScene(TScene *Scene)
{
    mScene = Scene;
}

void TUndo::fAddItem(TItem *Item)
{
    mList.push_back(Item);
}

void TUndo::undo()
{}

void TUndo::redo()
{}
