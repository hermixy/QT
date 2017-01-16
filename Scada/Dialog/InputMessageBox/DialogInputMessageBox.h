#ifndef DIALOG_INPUT_MESSAGE_BOX_H
#define DIALOG_INPUT_MESSAGE_BOX_H

#include "../Scada/Src/Base/Base.h"

class DialogInputMessageBox : public QDialog
{
    Q_OBJECT

public:
    DialogInputMessageBox(const QString &Title,
                            const QString &Text,
                            const QString &Input,
                            QWidget *parent = 0);
    ~DialogInputMessageBox();

    enum
    {
        RETURN_ABORT,
        RETURN_OK
    };

    int fGetReturn();


private slots:
    void fApply();

private:
    int mReturn;
    QString mInputCompare;
    QLabel *mLabelTextShow;
    QLineEdit *mLineEditInput;
    QLabel *mLabelInputShow;
    QLabel *mLabelResult;
    QPushButton *mButtonOk;
    QPushButton *mButtonClose;
};

#endif // DIALOG_INPUT_MESSAGE_BOX_H
