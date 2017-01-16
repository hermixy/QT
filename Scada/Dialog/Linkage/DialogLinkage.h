#ifndef DIALOG_LINKAGE_H
#define DIALOG_LINKAGE_H

#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/Linkage/Linkage.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"

class DialogLinkage : public QDialog
{
    Q_OBJECT

public:
    DialogLinkage(const QList<TScene *> &SceneList, const QList<TService *> &ServiceList,
                  QList<TLinkage *> &LinkageList, QWidget *parent = 0);
    ~DialogLinkage();

    bool mInit;
    bool mSave;

    QList<TLinkage *> mLinkageList;

private slots:
    void fAdd();
    void fEdit();
    void fDel();

    void fTableClick(int Row, int Col);
    void fOkClicked();

private:
    // 表格
    QLabel *mLabelTableShow;
    TTableView *mTable;

    // src
    QLabel *mLabelSrcServiceShow;
    QComboBox *mComboBoxSrcService;
    QLabel *mLabelSrcNodeId;
    QComboBox *mComboBoxSrcNodeId;
    QLabel *mLabelSrcNodeIdText;

    // dst
    QLabel *mLabelDstServiceShow;
    QComboBox *mComboBoxDstService;
    QLabel *mLabelDstNodeId;
    QComboBox *mComboBoxDstNodeId;
    QLabel *mLabelDstNodeIdText;

    //
    QPushButton *mButtonAdd;
    QPushButton *mButtonEdit;
    QPushButton *mButtonDel;

    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    bool fCheckMsg();
};

#endif // DIALOG_LINKAGE_H
