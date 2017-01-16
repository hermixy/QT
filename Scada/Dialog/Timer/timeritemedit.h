#ifndef TIMERITEMEDIT_H
#define TIMERITEMEDIT_H
#include "../../Src/Service/Service.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"
#include <QDialog>
class TimerItemEdit : public QDialog
{
    Q_OBJECT
public:
    TimerItemEdit(TScene::TimerStruct *timerstructItem, QWidget *parent = 0);
    ~TimerItemEdit();
    QPushButton* mButtonOk;
    QPushButton* mButtonClose;
    bool mSave=false;
    TScene::TimerStruct timerstruct;


    QLabel *mLabelResult;
    QLabel *mLabelNameIdShow;
    QLineEdit *mLineEditNameId;
    QLabel *mLabelServiceShow;
    QComboBox *mComboBoxService;

    QLabel *mLabelSubDeviceNum;
    QSpinBox *mLineEditSubDeviceNum;
    QLabel *mLabelMainDeviceNum;
    QSpinBox *mLineEditMainDeviceNum;

    QLabel *mStartAddr;
    QLineEdit *mLineEditStartAddr;

    QLabel *mLabelLessWrite;
    QDoubleSpinBox *mLineEditLessWrite;

    QGroupBox *mGroupBoxServiceList;

public slots:
    void fOk();
    bool fCheckServiceMsg();
};

#endif // TIMERITEMEDIT_H
