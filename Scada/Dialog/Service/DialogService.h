#ifndef DIALOG_SERVICE_H
#define DIALOG_SERVICE_H

#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"

class DialogService : public QDialog
{
    Q_OBJECT
public:
    DialogService(QList<TService *> &ServiceList, QWidget *parent = 0);
    ~DialogService();

    QList<TService *> mServiceList;

    bool mSave;

private slots:
    void fOk();

    void fAdd();
    void fEdit();
    void fDel();
    void fServiceListChanged(int Row, int Col);

private:
    int mServiceId;

    // 服务列表
    QGroupBox *mGroupBoxServiceList;
    QLabel *mLabelServiceListShow;
    TTableView *mTableServiceList;

    QTextEdit *mTextEditMsg;

    QGroupBox *mGroupBoxServiceMsg;

    QGroupBox *mGroupBoxControl;
    QPushButton* mButtonAdd;
    QPushButton* mButtonEdit;
    QPushButton* mButtonDel;

    QPushButton* mButtonOk;
    QPushButton* mButtonClose;

    void fInit();

    void fAddServiceToTable(TService *Service);
    QString fGetPortName(int Index);
    QString fGetAgreeName(int Index);
};

#endif // DIALOG_SERVICE_H
