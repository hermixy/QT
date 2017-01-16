#include "Action.h"

TAction::TAction(const QString &text, QObject* parent):
    QAction(text, parent)
{
    mType = -1;
    mScene = NULL;
}
