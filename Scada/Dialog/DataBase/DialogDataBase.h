#ifndef DIALOG_DATA_BASE_H
#define DIALOG_DATA_BASE_H

#include "../Scada/Src/DataBase/DataBaseMsg.h"
#include "../Scada/Src/Base/Base.h"

class DialogDataBase : public QDialog
{
    Q_OBJECT

public:
    DialogDataBase(TDataBaseMsg *DBMsg, QWidget *parent = 0);
    ~DialogDataBase();

private slots:
    void fOkClicked();    

private:
    TDataBaseMsg *mDBMsg;
    QLabel *mLabelTypeShow;
    QComboBox *mComboBoxType;

    QLabel *mLabelHost;
    QLineEdit *mLineEditHost;
    QLabel *mLabelDBName;
    QLineEdit *mLineEditDBName;
    QLabel *mLabelUserName;
    QLineEdit *mLineEditUserName;
    QLabel *mLabelPassWd;
    QLineEdit *mLineEditPassWd;

    QPushButton *mButtonOk;
    QPushButton *mButtonClose;
};

#endif // DIALOG_DATA_BASE_H
