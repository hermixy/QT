#ifndef DIALOG_HINT_H
#define DIALOG_HINT_H

#include "../../Src/Base/Base.h"

class DialogHint : public QDialog
{
    Q_OBJECT

public:
    DialogHint(QWidget *parent = 0);
    ~DialogHint();

    void fSetHint(QString Hint);

private:
    QTextEdit *mTextEditHint;
};

#endif // DIALOG_HINT_H
