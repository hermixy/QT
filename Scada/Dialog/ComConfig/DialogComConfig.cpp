#include "DialogComConfig.h"

#include <QDebug>

DialogComConfig::DialogComConfig(TComData *ComData, QWidget *parent) : QDialog(parent)
{
    mComData = ComData;

    ///////////////////////////////////////////////
    // 串口
    mLabelComNameShow = TBase::fLabel(tr("串口名称"), tr("mLabelComNameShow"));
    mLineEditComName = TBase::fLineEdit(tr("mLineEditComName"), 120, 24);
    mLineEditComName->setText(mComData->mName);

    mLabelSystemShow = TBase::fLabel(tr("操作系统"), tr("mLabelSystemShow"));
    mComboBoxSystem = TBase::fComboBox(tr("mComboBoxSystem"));
    mComboBoxSystem->addItem(tr("Windows"));
    mComboBoxSystem->addItem(tr("Linux默认"));
    mComboBoxSystem->addItem(tr("Linux定制"));
    mComboBoxSystem->addItem(tr("Linux1"));
    mComboBoxSystem->addItem(tr("Linux2"));
    connect(mComboBoxSystem, SIGNAL(currentIndexChanged(int)), this, SLOT(fSystemChanged(int)));

    mLabelComNumShow = TBase::fLabel(tr("串口号"), tr("mLabelComNameShow"));
    mSpinBoxComNum = TBase::fSpinBox(tr("mSpinBoxComNum"), 1, 100);
    connect(mSpinBoxComNum, SIGNAL(valueChanged(int)), this, SLOT(fSystemChanged(int)));

    mLabelComRateShow = TBase::fLabel(tr("波特率"), tr("mLabelComRateShow"));
    mComboBoxComRate = TBase::fComboBox(tr("mComboBoxComRate"));
    mComboBoxComRate->addItem(tr("110"));
    mComboBoxComRate->addItem(tr("300"));
    mComboBoxComRate->addItem(tr("600"));
    mComboBoxComRate->addItem(tr("1200"));
    mComboBoxComRate->addItem(tr("2400"));
    mComboBoxComRate->addItem(tr("4800"));
    mComboBoxComRate->addItem(tr("9600"));
    mComboBoxComRate->addItem(tr("14400"));
    mComboBoxComRate->addItem(tr("19200"));
    mComboBoxComRate->addItem(tr("38400"));
    mComboBoxComRate->addItem(tr("56000"));
    mComboBoxComRate->addItem(tr("57600"));
    mComboBoxComRate->addItem(tr("115200"));
    mComboBoxComRate->setCurrentIndex(mComboBoxComRate->findText(QString::number(mComData->mRate)));

    mLabelComDataShow = TBase::fLabel(tr("数据位"), tr("mLabelComDataShow"));
    mComboBoxComData = TBase::fComboBox(tr("mComboBoxComData"));
    mComboBoxComData->addItem(tr("4"));
    mComboBoxComData->addItem(tr("5"));
    mComboBoxComData->addItem(tr("6"));
    mComboBoxComData->addItem(tr("7"));
    mComboBoxComData->addItem(tr("8"));
    mComboBoxComData->setCurrentIndex(mComboBoxComData->findText(QString::number(mComData->mData)));

    mLabelComStopShow = TBase::fLabel(tr("停止位"), tr("mLabelComStopShow"));
    mComboBoxComStop = TBase::fComboBox(tr("mComboBoxComStop"));
    mComboBoxComStop->addItem(tr("1"));
    mComboBoxComStop->addItem(tr("1.5"));
    mComboBoxComStop->addItem(tr("2"));
    mComboBoxComStop->setCurrentIndex(mComboBoxComStop->findText(QString::number(mComData->mStop)));

    mLabelComCheckShow = TBase::fLabel(tr("校验位"), tr("mLabelComCheckShow"));
    mComboBoxComCheck = TBase::fComboBox(tr("mComboBoxComCheck"));
    mComboBoxComCheck->addItem(tr("无校验(None)"));
    mComboBoxComCheck->addItem(tr("奇校验(Odd)"));
    mComboBoxComCheck->addItem(tr("偶校验(Even)"));
    mComboBoxComCheck->addItem(tr("1校验(Mark)"));
    mComboBoxComCheck->addItem(tr("0校验(Space)"));
    mComboBoxComCheck->setCurrentIndex(mComData->mCheck);

    QGridLayout *mLayoutPort = new QGridLayout();
    mLayoutPort->addWidget(mLabelSystemShow, 1, 0);
    mLayoutPort->addWidget(mComboBoxSystem, 1, 1);
    mLayoutPort->addWidget(mLabelComNumShow, 2, 0);
    mLayoutPort->addWidget(mSpinBoxComNum, 2, 1);
    mLayoutPort->addWidget(mLabelComNameShow, 3, 0);
    mLayoutPort->addWidget(mLineEditComName, 3, 1);
    mLayoutPort->addWidget(mLabelComRateShow, 4, 0);
    mLayoutPort->addWidget(mComboBoxComRate, 4, 1);
    mLayoutPort->addWidget(mLabelComDataShow, 5, 0);
    mLayoutPort->addWidget(mComboBoxComData, 5, 1);
    mLayoutPort->addWidget(mLabelComStopShow, 6, 0);
    mLayoutPort->addWidget(mComboBoxComStop, 6, 1);
    mLayoutPort->addWidget(mLabelComCheckShow, 7, 0);
    mLayoutPort->addWidget(mComboBoxComCheck, 7, 1);
    ////////////////////////////////////////

    mButtonOK = TBase::fButton(tr("确定"), tr("mButtonOK"), 50, 24);
    connect(mButtonOK, SIGNAL(clicked()), this, SLOT(fOK()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mButtonClose"), 50, 24);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));


    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOK);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);
    mLayoutMain->addLayout(mLayoutPort);
    mLayoutMain->addStretch();
    mLayoutMain->addLayout(mLayoutButton);

    mButtonOK->setFocus();
    mButtonOK->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOK->setShortcut(Qt::Key_Enter);
    mButtonOK->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("串口配置"));
    //setFixedSize(sizeHint());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void DialogComConfig::fSystemChanged(int index)
{
    Q_UNUSED(index);

    switch(mComboBoxSystem->currentIndex())
    {
    case 0:
        // Windows
        mLineEditComName->setText(tr("COM") + QString::number(mSpinBoxComNum->value()));
        break;

    case 1:
        // Linux默认
        mLineEditComName->setText(tr("ttyS") + QString::number(mSpinBoxComNum->value()));
        break;

    case 2:
        // Linux定制
        mLineEditComName->setText(tr("ttyAT") + QString::number(mSpinBoxComNum->value()));
        break;

    case 3:
        // Linux1
        mLineEditComName->setText(tr("tty") + QString::number(mSpinBoxComNum->value()));
        break;

    case 4:
        // Linux2
        mLineEditComName->setText(tr("ttySAC") + QString::number(mSpinBoxComNum->value()));
        break;

    default:
        break;
    }
}

void DialogComConfig::fOK()
{
    mComData->mName = mLineEditComName->text();
    mComData->mRate = mComboBoxComRate->currentText().toInt();
    mComData->mData = mComboBoxComData->currentText().toInt();
    mComData->mStop = mComboBoxComStop->currentText().toInt();
    mComData->mCheck = mComboBoxComCheck->currentIndex();

    close();
}
