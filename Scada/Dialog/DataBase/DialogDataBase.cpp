#include "DialogDataBase.h"

#include <QDebug>

DialogDataBase::DialogDataBase(TDataBaseMsg *DBMsg, QWidget *parent) : QDialog(parent)
{
    mDBMsg = DBMsg;

    mLabelTypeShow = TBase::fLabel(tr("数据库类型"), tr("mLabelTypeShow"));
    mComboBoxType = TBase::fComboBox(tr("mComboBoxType"));
    mComboBoxType->addItem(tr("MySQL"));
    mComboBoxType->addItem(tr("Access"));
    mComboBoxType->addItem(tr("Oracle"));
    mComboBoxType->addItem(tr("SQL Server"));

    mLabelHost          = TBase::fLabel(tr("主机"), tr("mLabelHost"));
    mLineEditHost       = TBase::fLineEdit(tr("mLineEditHost"));
    mLabelDBName        = TBase::fLabel(tr("数据库"), tr("mLabelDBName"));
    mLineEditDBName     = TBase::fLineEdit(tr("mLineEditDBName"));
    mLabelUserName      = TBase::fLabel(tr("用户"), tr("mLabelUserName"));
    mLineEditUserName   = TBase::fLineEdit(tr("mLineEditUserName"));
    mLabelPassWd        = TBase::fLabel(tr("密码"), tr("mLabelPassWd"));
    mLineEditPassWd     = TBase::fLineEdit(tr("mLineEditPassWd"));

    if(mDBMsg)
    {
        mComboBoxType->setCurrentIndex(mDBMsg->fGetType());
        mLineEditHost->setText(mDBMsg->fGetHost());
        mLineEditDBName->setText(mDBMsg->fGetDBName());
        mLineEditUserName->setText(mDBMsg->fGetUserName());
        mLineEditPassWd->setText(mDBMsg->fGetPassWord());
    }

    mButtonOk = TBase::fButton(tr("确定"), tr("mPushButtonOK"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOkClicked()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mPushButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addSpacing(20);
    mLayoutButton->addWidget(mButtonClose);

    QGridLayout *mLayoutMain = new QGridLayout();
    mLayoutMain->addWidget(mLabelTypeShow, 0, 0);
    mLayoutMain->addWidget(mComboBoxType, 0, 1);
    mLayoutMain->addWidget(mLabelHost, 1, 0);
    mLayoutMain->addWidget(mLineEditHost, 1, 1);
    mLayoutMain->addWidget(mLabelDBName, 2, 0);
    mLayoutMain->addWidget(mLineEditDBName, 2, 1);
    mLayoutMain->addWidget(mLabelUserName, 3, 0);
    mLayoutMain->addWidget(mLineEditUserName, 3, 1);
    mLayoutMain->addWidget(mLabelPassWd, 4, 0);
    mLayoutMain->addWidget(mLineEditPassWd, 4, 1);
    mLayoutMain->addLayout(mLayoutButton, 5, 0, 1, 2);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("设置数据库"));
    setFixedSize(sizeHint());
}

DialogDataBase::~DialogDataBase()
{}

void DialogDataBase::fOkClicked()
{
    mDBMsg->fSetType(mComboBoxType->currentIndex());
    mDBMsg->fSetHost(mLineEditHost->text());
    mDBMsg->fSetDBName(mLineEditDBName->text());
    mDBMsg->fSetUserName(mLineEditUserName->text());
    mDBMsg->fSetPassWord(mLineEditPassWd->text());

    close();
}
