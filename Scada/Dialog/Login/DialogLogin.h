#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include "../../Src/DataBase/DataBase.h"
#include "../../Src/Base/Base.h"

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    DialogLogin(bool islogin, QWidget *parent = 0);
    ~DialogLogin();

private slots:
    void fLogin();

public:
    TDataBase *mDataBase;
    QLabel *mLabelName;
    QLabel *mLabelNamePic;
    QLabel *mLabelNameNO;
    QLabel *mLabelPassWd;
    QLineEdit *mLineEditName;
    QLineEdit *mLineEditPassWd;
    QLabel *mLabelPassWdNO;
    QPushButton *mButtonLogin;
    QPushButton *mButtonClose;

    bool ok=false;
    bool admin=false;
    bool islogin;
};

#endif // DIALOG_LOGIN_H
