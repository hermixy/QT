#ifndef DIALOG_COM_CONFIG_H
#define DIALOG_COM_CONFIG_H

#include "DialogComConfig.h"
#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/Base/Base.h"

class DialogComConfig : public QDialog
{
    Q_OBJECT
public:
    DialogComConfig(TComData *ComData, QWidget *parent = 0);

    TComData *mComData;

private slots:
    void fOK();
    void fSystemChanged(int index);

private:
    // 串口
    QLabel *mLabelSystemShow;
    QComboBox *mComboBoxSystem;
    QLabel *mLabelComNumShow;
    QSpinBox *mSpinBoxComNum;
    QLabel *mLabelComNameShow;
    QLineEdit *mLineEditComName;
    QLabel *mLabelComRateShow;
    QComboBox *mComboBoxComRate;
    QLabel *mLabelComDataShow;
    QComboBox *mComboBoxComData;
    QLabel *mLabelComStopShow;
    QComboBox *mComboBoxComStop;
    QLabel *mLabelComCheckShow;
    QComboBox *mComboBoxComCheck;

    QPushButton *mButtonOK;
    QPushButton *mButtonClose;
};

#endif // DIALOG_COM_CONFIG_H
