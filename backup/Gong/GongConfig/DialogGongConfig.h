#ifndef DIALOG_GONG_CONFIG_H
#define DIALOG_GONG_CONFIG_H

#include "../Scada/Src/Base/Base.h"
#include "../Scada/Src/TableView/TableView.h"
#include "GongConfig.h"

class DialogConfigGong : public QDialog
{
    Q_OBJECT

public:
    DialogConfigGong(QList<TGongConfig *> &ConfigList, QWidget *parent = 0);
    ~DialogConfigGong();

    bool mSave;
    QList<TGongConfig *> mList;

private slots:
    void fAdd();
    void fEdit();
    void fDel();
    void fLoad();
    void fOut();
    void fOK();

    void fTableClick(int Row, int Col);

private:
    TTableView *mTable;
    QLabel *mLabelId;
    QSpinBox *mSpinBoxId;
    QLabel *mLabelName;
    QLineEdit *mLineEditName;
    QLabel *mLabelAddr;
    QSpinBox *mSpinBoxAddr;
    QLabel *mLabelChannel;
    QSpinBox *mSpinBoxChannel;
    QLabel *mLabelZero;
    QDoubleSpinBox *mSpinBoxZero;
    QLabel *mLabelFull;
    QDoubleSpinBox *mSpinBoxFull;
    QLabel *mLabelRange;
    QDoubleSpinBox *mSpinBoxRange;
    QLabel *mLabelUnit;
    QLineEdit *mLineEditUnit;
    QLabel *mLabelAlarmUpper;
    QDoubleSpinBox *mSpinBoxAlarmUpper;
    QLabel *mLabelAlarmLower;
    QDoubleSpinBox *mSpinBoxAlarmLower;
    QLabel *mLabelDisplayType;
    QSpinBox *mSpinBoxDisplayType;
    QLabel *mLabelCoe;
    QDoubleSpinBox *mSpinBoxCoe;
    QLabel *mLabelCycle;
    QDoubleSpinBox *mSpinBoxCycle;
    QLabel *mLabelTime;
    QDoubleSpinBox *mSpinBoxTime;

    QPushButton *mButtonAdd;
    QPushButton *mButtonEdit;
    QPushButton *mButtonDel;
    QPushButton *mButtonLoad;
    QPushButton *mButtonOut;
    QPushButton *mButtonClose;
    QPushButton *mButtonOK;
};

#endif // DIALOG_GONG_CONFIG_H
