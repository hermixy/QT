#include "DialogNewScene.h"

DialogNewScene::DialogNewScene(QWidget *parent) : QDialog(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    //mInit = false;
    mSave = false;

    mLabelNameShow = TBase::fLabel(tr("名称"), tr("mLabelNameShow"));
    mLineEdit = TBase::fLineEdit(tr("mLineEdit"), 120, 24);
    mLabelBackColorShow = TBase::fLabel(tr("背景"), tr("mLabelBackColorShow"));
    mLabelBackColor = TBase::fLabel(tr(""), tr("mLabelBackColor"), 50, 36);
    mBackColor = Qt::black;
    TBase::fColorLabel(mLabelBackColor, mBackColor);
    mButtonColor = TBase::fButton(tr("浏览"), tr("mButtonColor"), 50, 36);
    connect(mButtonColor, SIGNAL(clicked()), this, SLOT(fChooseBackColor()));

    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));

    QGridLayout *mLayoutName = new QGridLayout();
    mLayoutName->addWidget(mLabelNameShow, 0, 0);
    mLayoutName->addWidget(mLineEdit, 0, 1, 1, 2);
    mLayoutName->addWidget(mLabelBackColorShow, 2, 0);
    mLayoutName->addWidget(mLabelBackColor, 2, 1);
    mLayoutName->addWidget(mButtonColor, 2, 2);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new  QVBoxLayout(this);
    mLayoutMain->addLayout(mLayoutName);
    mLayoutMain->addSpacing(15);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("新建"));
    setFixedSize(sizeHint());
}

void DialogNewScene::fChooseBackColor()
{
    if(TBase::fChooseColor(mBackColor, this))
    {
        TBase::fColorLabel(mLabelBackColor, mBackColor);
    }
}

void DialogNewScene::fApply()
{
    mSave = true;
    close();
}

void DialogNewScene::fSetTitle(const QString &Title)
{
    setWindowTitle(Title);
}

bool DialogNewScene::fSetName(const QString &Name)
{
    mLineEdit->setText(Name);
    return true;
}

bool DialogNewScene::fSetBackColor(const QColor &Color)
{
    mBackColor = Color;
    TBase::fColorLabel(mLabelBackColor, mBackColor);

    return true;
}

QString DialogNewScene::fGetName() const
{
    return mLineEdit->text();
}

QColor DialogNewScene::fGetBackColor() const
{
    return mBackColor;
}

DialogNewScene::~DialogNewScene()
{}
