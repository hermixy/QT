#include "DialogRunMode.h"
#include "../Scada/Src/Math/Math.h"

#include <QMessageBox>
#include <QDebug>

DialogRunMode::DialogRunMode(TRunMode *Mode, QWidget *parent) : QDialog(parent)
{
    mMode = Mode;

    mLabelIpShow = TBase::fLabel(tr("IP"), tr("mLabelIpShow"));
    mLineEditIp = TBase::fLineEdit(tr("mLineEditIp"), 120, 24);

    if(mMode)
    {
        mLineEditIp->setText(mMode->mIp);
    }

    mLabelNetPortShow = TBase::fLabel(tr("端口"), tr("mLabelNetPortShow"));
    mSpinBoxNetPort = TBase::fSpinBox(tr("mSpinBoxNetPort"), 1, 65535);

    if(mMode)
    {
        mSpinBoxNetPort->setValue(mMode->mPort);
    }
    else
    {
        mSpinBoxNetPort->setValue(0);
    }

    mComboBoxMode = TBase::fComboBox(tr("mComboBoxMode"));
    mComboBoxMode->addItem(tr("单机模式"));
    mComboBoxMode->addItem(tr("服务器模式"));
    mComboBoxMode->addItem(tr("客户端模式"));

    if(mMode && mMode->mRunMode >= 0 && mMode->mRunMode < 3)
    {
        mComboBoxMode->setCurrentIndex(mMode->mRunMode);
    }
    else
    {
        mComboBoxMode->setCurrentIndex(-1);
    }

    mButtonApply = TBase::fButton(tr("确定"), tr("mButtonApply"));
    connect(mButtonApply, SIGNAL(clicked()), this, SLOT(fApply()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mButtonClose"));
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mLayoutData = new QGridLayout();
    mLayoutData->addWidget(mComboBoxMode, 0, 0, 1, 2);
    mLayoutData->addWidget(mLabelIpShow, 1, 0);
    mLayoutData->addWidget(mLineEditIp, 1, 1);
    mLayoutData->addWidget(mLabelNetPortShow, 2, 0);
    mLayoutData->addWidget(mSpinBoxNetPort, 2, 1);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonApply);
    mLayoutButton->addWidget(mButtonClose);

    ////////////////////////////////////////
    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);
    mLayoutMain->addLayout(mLayoutData);
    mLayoutMain->addStretch();
    mLayoutMain->addSpacing(5);
    mLayoutMain->addLayout(mLayoutButton);

    mButtonApply->setFocus();
    mButtonApply->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonApply->setShortcut(Qt::Key_Enter);
    mButtonApply->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("管理运行模式"));
    //setFixedSize(sizeHint());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void DialogRunMode::fApply()
{
    if(mMode && mComboBoxMode->currentIndex() >= 0
            && TMath::fCheckIp(mLineEditIp->text())
            && TMath::fCheckPort(mSpinBoxNetPort->value()))
    {
        mMode->mRunMode = mComboBoxMode->currentIndex();
        mMode->mIp = mLineEditIp->text();
        mMode->mPort = mSpinBoxNetPort->value();

        close();
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("请正确输入各信息"));
    }
}
