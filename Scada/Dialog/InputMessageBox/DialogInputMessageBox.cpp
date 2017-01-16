#include "DialogInputMessageBox.h"

DialogInputMessageBox::DialogInputMessageBox(const QString &Title,
                                             const QString &Text,
                                             const QString &Input,
                                             QWidget *parent) : QDialog(parent)
{
    mReturn = RETURN_ABORT;
    mInputCompare = Input;

    mLabelTextShow = TBase::fLabel(Text, tr("mLabelTextShow"));
    QFont TextFont;
    TextFont.setBold(true);
    TextFont.setPointSize(16);
    mLabelTextShow->setFont(TextFont);
    mLineEditInput = TBase::fLineEdit(tr("mLineEditInput"), 120, 24);
    mLabelInputShow = TBase::fLabel(tr("若要确认操作，请在左侧框中输入:") + mInputCompare, tr("mLabelInputShow"));
    mLabelResult = TBase::fLabel(tr(""), tr("mLabelResult"));

    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));

    QGridLayout *mLayoutName = new QGridLayout();
    mLayoutName->addWidget(mLabelTextShow, 0, 0, 1, 2, Qt::AlignCenter);
    mLayoutName->addWidget(mLineEditInput, 1, 0);
    mLayoutName->addWidget(mLabelInputShow, 1, 1);
    mLayoutName->addWidget(mLabelResult, 2, 1);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new  QVBoxLayout(this);
    mLayoutMain->addLayout(mLayoutName);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(Title);
    setFixedSize(sizeHint());
}

void DialogInputMessageBox::fApply()
{
    if(mInputCompare == mLineEditInput->text())
    {
        mReturn = RETURN_OK;
        close();
    }
    else
    {
        mLabelResult->setText(tr("<font color=red><b>输入错误，请重新输入</b></font>"));
    }
}

int DialogInputMessageBox::fGetReturn()
{
    return mReturn;
}

DialogInputMessageBox::~DialogInputMessageBox()
{}
