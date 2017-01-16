#include "DialogLogin.h"
#include <QSettings>
#include "Runer.h"
#include "customprotocol.h"
#include <QLabel>
DialogLogin::DialogLogin(bool islogin, QWidget *parent) : QDialog(parent)
{
    this->islogin=islogin;
    mLabelName = TBase::fLabel(tr("用户"), tr("mLabelName"));
    mLabelPassWd = TBase::fLabel(tr("密码"), tr("mLabelPassWd"));
    mLabelNameNO=TBase::fLabel(tr(""), tr("mLabelPassWd"));
    mLineEditName = TBase::fLineEdit(tr("mLineEditName"), 200, 24);
    mLineEditPassWd = TBase::fLineEdit(tr("mLineEditPassWd"), 200, 24);
    mLineEditPassWd->setEchoMode(QLineEdit::Password);
    mLineEditPassWd->setStyleSheet("lineedit-password-character: 42");
    mLabelPassWdNO=TBase::fLabel(tr(""), tr("mLabelPassWd"));

    if(islogin)
    {
        mButtonLogin = TBase::fButton(tr("系统登录"), tr("mButtonLogin"), 75, 36);
    }
    else
    {
        mButtonLogin = TBase::fButton(tr("系统注册"), tr("mButtonLogin"), 75, 36);
    }
    connect(mButtonLogin, SIGNAL(clicked()), this, SLOT(fLogin()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonLogin);
    mLayoutButton->addWidget(mButtonClose);

    QGridLayout *mLayoutMain = new QGridLayout(this);
    QLabel *labelPixmap=new QLabel(this);
    labelPixmap->setPixmap(QPixmap("login.png"));
    mLayoutMain->addWidget(labelPixmap, 0, 0, 3, 3);
    mLayoutMain->addWidget(mLabelName, 0, 3);
    mLayoutMain->addWidget(mLineEditName, 0, 4);
    mLayoutMain->addWidget(mLabelNameNO, 0, 5);

    mLayoutMain->addWidget(mLabelPassWd, 1, 3);
    mLayoutMain->addWidget(mLineEditPassWd, 1, 4);
    mLayoutMain->addWidget(mLabelPassWdNO, 1, 5);
    mLayoutMain->addLayout(mLayoutButton, 2, 3, 1, 2);

    // 设置默认
    mButtonLogin->setFocus();
    mButtonLogin->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonLogin->setShortcut(Qt::Key_Enter);
    mButtonLogin->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    if(islogin)
    {
        setWindowTitle(tr("建筑节能系统"));
    }
    else
    {
        setWindowTitle(tr("注册"));
    }
    //setFixedSize(sizeHint());
}

DialogLogin::~DialogLogin()
{}

void DialogLogin::fLogin()
{
    if(!islogin)
    {
        QSettings setting("inis", QSettings::IniFormat);
        setting.setIniCodec("UTF-8");


        MyLoginData data={false, mLineEditName->text(), mLineEditPassWd->text()};
        int num=setting.value("num", 0).toInt();
        setting.beginWriteArray("logins");

            QByteArray bytes;
            bytes.append(data.password);
            bytes=CustomProtocol::getMd5Hash(bytes);
            setting.setArrayIndex(num);
            setting.setValue("permission", data.permission);
            setting.setValue("userName", data.userName);
            setting.setValue("password", bytes);
        setting.endArray();

        setting.setValue("num", num+1);
        qDebug()<<num;
        close();
        return;
    }


    QString mUsrname=mLineEditName->text();
    QString mpasswd=mLineEditPassWd->text();

    QByteArray bytes;
    bytes.append(mpasswd);
    bytes=CustomProtocol::getMd5Hash(bytes);

    QString userName;
    QByteArray password;
    bool permission;

    QSettings setting("inis", QSettings::IniFormat);
    setting.setIniCodec("UTF-8");
    int size=setting.value("num", 0).toInt();
    setting.beginReadArray("logins");

    for(int i=0; i<size; ++i)
    {
        setting.setArrayIndex(i);
        permission=setting.value("permission", false).toBool();
        userName=setting.value("userName", QString()).toString();
        password=setting.value("password", QByteArray()).toByteArray();

        if(userName==mUsrname&&password==bytes)
        {
            qDebug()<<"ok   = tur";
            ok=true;
            admin=permission;
            break;
        }
    }
    setting.endArray();

    if(ok==false)
    {
        mLabelPassWdNO->setText("密码错误");
    }
    else
    {
        close();
    }
}
