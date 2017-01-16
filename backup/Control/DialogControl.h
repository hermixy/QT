#ifndef DIALOG_CONTROL_H
#define DIALOG_CONTROL_H

#include "../Scada/Src/Widgets/Widgets.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/Base/Base.h"

class DialogControl : public QDialog
{
    Q_OBJECT

public:
    DialogControl(TItem *Item, QList<TScene *> &SceneList,
                  const QList<TService *> &ServiceList, QWidget *parent = 0);
    ~DialogControl();

private slots:
    void fApply();
    void fAdd();
    void fDel();
    void fSceneIndexChanged(int Index);

private:
    TItem *mItem;
    QList<TScene *> mSceneList;
    QList<TService *> mServiceList;

    QGroupBox *mGroupBoxItem;
    QLabel *mLabelNameShow;
    QLineEdit *mEditName;

    QGroupBox *mGroupBoxWidgets;
    QLabel *mLabelServiceShow;
    QComboBox *mComboBoxService;
    QLabel *mLabelJsShow;
    QComboBox *mComboBoxJs;
    QLabel *mLabelSubShow;
    TTableView *mTable;
    QLabel *mLabelSceneShow;
    QComboBox *mComboBoxScene;
    QLabel *mLabelWidgetsShow;
    QComboBox *mComboBoxWidgets;

    QPushButton *mButtonAdd;
    QPushButton *mButtonDel;

    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    void fInit();
    void fCreateItem();
    void fCreateWidgets();
};

#endif // DIALOG_CONTROL_H
