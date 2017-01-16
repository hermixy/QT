#ifndef DIALOG_RUN_MODE_H
#define DIALOG_RUN_MODE_H

#include "../Scada/Src/RunMode/RunMode.h"
#include "../Scada/Src/Base/Base.h"

class DialogRunMode : public QDialog
{
    Q_OBJECT
public:
    DialogRunMode(TRunMode *Mode, QWidget *parent = 0);

private slots:
    void fApply();

private:
    TRunMode *mMode;

    QComboBox *mComboBoxMode;
    QLabel *mLabelIpShow;
    QLineEdit *mLineEditIp;
    QLabel* mLabelNetPortShow;
    QSpinBox *mSpinBoxNetPort;

    QPushButton* mButtonApply;
    QPushButton* mButtonClose;
};

#endif // DIALOG_RUN_MODE_H
