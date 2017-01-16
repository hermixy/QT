#ifndef DIALOG_DATA_H
#define DIALOG_DATA_H

#include "../../Src/DataBase/DataBase.h"
#include "../../Src/TableView/TableView.h"
#include "../../Src/Base/Base.h"

class DialogData : public QDialog
{
    Q_OBJECT

public:
    DialogData(TDataBase *DataBase, int TabelNum, QWidget *parent = 0);
    ~DialogData();

private slots:
    void fOut();
    void fSearch();
    void fAllTime(bool Checked);

private:
    int mTabelNum;
    QCheckBox *mCheckBoxAll;
    QLabel *mLabelStart;
    QLabel *mLabelStop;
    QDateTimeEdit *mDateTimeStart;
    QDateTimeEdit *mDateTimeStop;
    QPushButton *mButtonSearch;

    TDataBase *mDataBase;
    TTableView *mTable;

    QPushButton *mButtonOut;
    QPushButton *mButtonClose;

    void fUpdateTable(qint64 StartTime, qint64 StopTime);
    void fAllChecked();
};

#endif // DIALOG_DATA_H
