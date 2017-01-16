#include "UndoMoveDrag.h"
#include <QDebug>

TUndoMoveDrag::TUndoMoveDrag() : TUndo()
{
    mStartDx = 0;
    mStartDy = 0;
    mStopDx = 0;
    mStopDy = 0;
}

TUndoMoveDrag::TUndoMoveDrag(TScene * Scene, QList<TItem *> &ItemList, QUndoCommand * Parent)
    : TUndo(Scene, ItemList, Parent)
{
    mStartDx = 0;
    mStartDy = 0;
    mStopDx = 0;
    mStopDy = 0;
}

void TUndoMoveDrag::undo()
{
    //qDebug() << "TUndoMoveDrag::undo()";

    foreach(TItem *Item, mList)
    {
        /*qDebug() << Item->fGetStartX() - mStartDx
                    << Item->fGetStartY() - mStartDy
                    << Item->fGetStopX() - mStopDx
                    << Item->fGetStopY() - mStopDy;*/

        Item->fMoveTo(Item->fGetStartX() - mStartDx,
                      Item->fGetStartY() - mStartDy,
                      Item->fGetStopX() - mStopDx,
                      Item->fGetStopY() - mStopDy);
    }
}

void TUndoMoveDrag::redo()
{
    //qDebug() << "TUndoMoveDrag::redo()";

    foreach(TItem *Item, mList)
    {
        Item->fMoveTo(Item->fGetStartX() + mStartDx,
                      Item->fGetStartY() + mStartDy,
                      Item->fGetStopX() + mStopDx,
                      Item->fGetStopY() + mStopDy);
    }
}

void TUndoMoveDrag::fSetVector(double StartDx, double StartDy, double StopDx, double StopDy)
{
    mStartDx = StartDx;
    mStartDy = StartDy;
    mStopDx = StopDx;
    mStopDy = StopDy;
}

