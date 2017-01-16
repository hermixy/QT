#ifndef TIMERITEMSHOW_H
#define TIMERITEMSHOW_H
#include <QDialog>
#include "../../Src/Service/Service.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"
#include "../Scada/Src/Scene/Scene.h"
class TimerItemShow : public QDialog
{
    Q_OBJECT
public:
    TimerItemShow(QList<TScene::TimerStruct> &timerstruct, QString name=QString(), QWidget *parent = 0);
    ~TimerItemShow();

    QList<TScene::TimerStruct> timerstruct;
    // 服务列表
    QGroupBox *mGroupBoxServiceList;
    QLabel *mLabelServiceListShow;
    TTableView *mTableServiceList;

    QGroupBox *mGroupBoxServiceMsg;

    QGroupBox *mGroupBoxControl;
    QPushButton* mButtonAdd;
    QPushButton* mButtonEdit;
    QPushButton* mButtonDel;

    QPushButton* mButtonOk;
    QPushButton* mButtonClose;
    bool mSave=false;

private slots:
    void fOk();

    void fAdd();
    void fEdit();
    void fDel();
    void fAddServiceToTable(TScene::TimerStruct timerstruct);
};

#endif // TIMERITEMSHOW_H
