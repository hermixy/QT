#ifndef UNDO_MOVE_DRAG_H
#define UNDO_MOVE_DRAG_H

#include "Undo.h"

class TUndoMoveDrag : public TUndo
{
public:
    TUndoMoveDrag();
    TUndoMoveDrag(TScene * Scene, QList<TItem *> &ItemList, QUndoCommand * Parent = NULL);

    virtual void undo();
    virtual void redo();

    void fSetVector(double StartDx, double StartDy, double StopDx, double StopDy);

private:
    double mStartDx;
    double mStartDy;
    double mStopDx;
    double mStopDy;
};

#endif // UNDO_MOVE_DRAG_H
