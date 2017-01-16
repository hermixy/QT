#ifndef DIALOG_ALARM_H
#define DIALOG_ALARM_H

#include "../../Src/DataBase/DataBase.h"
#include "../../Src/TableView/TableView.h"
#include "../../Src/Base/Base.h"

class DialogAlarm : public QDialog
{
    Q_OBJECT

public:
    DialogAlarm(TDataBase *DataBase, int UserId = 0, QWidget *parent = 0);
    ~DialogAlarm();

private slots:
    void fIgnore();
    void fIgnoreAll();

private:
    int mUserId;
    TDataBase *mDataBase;
    TTableView *mTable;
    int mLineSum;
    QList< QList<QVariant> > mAlarmMsg;

    QPushButton *mButtonIgnoreAll;
    QPushButton *mButtonIgnore;
    QPushButton *mButtonClose;

    void fUpdateTable();
};

#endif // DIALOG_ALARM_H
