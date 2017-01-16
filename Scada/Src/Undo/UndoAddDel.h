#ifndef UNDO_ADD_DEL_H
#define UNDO_ADD_DEL_H

#include "Undo.h"

class TUndoAddDel : public TUndo
{
public:
    TUndoAddDel();
    TUndoAddDel(TScene * Scene, QList<TItem *> &ItemList, QUndoCommand * Parent = NULL);

    enum
    {
        OPERATION_ADD,
        OPERATION_DEL
    };

    virtual void undo();
    virtual void redo();

    void fSetOperation(int Operation);
    int fGetOperation();

private:
    int mOperation;
    void fDoAdd();
    void fDoDel();
};

#endif // UNDO_ADD_DEL_H
