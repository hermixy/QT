#include "timeritemedit.h"
#include "Maker.h"

TimerItemEdit::TimerItemEdit(TScene::TimerStruct *timerstructItem, QWidget *parent):QDialog(parent)
{
    if(timerstructItem)
    {
        this->timerstruct=*timerstructItem;
    }

    mLabelResult = TBase::fLabel(tr(""), tr("mLabelResult"));
    mLabelNameIdShow = TBase::fLabel(tr("名称"), tr("mLabelNameIdShow"));
    mLineEditNameId = TBase::fLineEdit(tr("mLineEditNameId"));
    mLineEditNameId->setText(timerstruct.TimerId);

    mLabelServiceShow = TBase::fLabel(tr("选择服务"), tr("mLabelServerShow"));
    mComboBoxService = TBase::fComboBox(tr("mComboBoxService"));

#ifdef MAKER
    if(Maker::maker) {

    foreach(TService *Service, Maker::maker->mServiceList)
    {
        mComboBoxService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
    }
    }
#endif

    mComboBoxService->setCurrentIndex(mComboBoxService->findData(QVariant(timerstruct.serverId)));

    mLabelMainDeviceNum=TBase::fLabel(tr("设备号"), tr("mLabelMainDeviceNum"));
    mLineEditMainDeviceNum=TBase::fSpinBox(tr("mLineEditMainDeviceNum"), 0, 200000);
    mLineEditMainDeviceNum->setValue(timerstruct.mainId);

    mLabelSubDeviceNum=TBase::fLabel(tr("通道"), tr("mLabelSubDeviceNum"));
    mLineEditSubDeviceNum=TBase::fSpinBox(tr("mLineEditSubDeviceNum"), 0, 200000);
    mLineEditSubDeviceNum->setValue(timerstruct.devId);

    mStartAddr=TBase::fLabel(tr("起始地址"), tr("mStartAddr"));
    mLineEditStartAddr=TBase::fLineEdit(tr("mLineEditStartAddr"));
    mLineEditStartAddr->setText(QString::number(timerstruct.startAddr, 16));
    mLabelLessWrite=TBase::fLabel(tr("写入"), tr("mLabelLessWrite"));
    mLineEditLessWrite=TBase::fDoubleSpinBox(tr("mLineEditLessWrite"), -200000, 200000);


    mGroupBoxServiceList = new QGroupBox(tr(""));

    QGridLayout *LayoutConfig = new QGridLayout(mGroupBoxServiceList);


    LayoutConfig->addWidget(mLabelNameIdShow, 0, 0);
    LayoutConfig->addWidget(mLineEditNameId, 0, 1);
    LayoutConfig->addWidget(mLabelServiceShow, 0, 2);
    LayoutConfig->addWidget(mComboBoxService, 0, 3);
    LayoutConfig->addWidget(mLabelMainDeviceNum, 2, 0);
    LayoutConfig->addWidget(mLineEditMainDeviceNum, 2, 1);
    LayoutConfig->addWidget(mLabelSubDeviceNum, 2, 2);
    LayoutConfig->addWidget(mLineEditSubDeviceNum, 2, 3);
    LayoutConfig->addWidget(mStartAddr, 3, 0);
    LayoutConfig->addWidget(mLineEditStartAddr, 3, 1);
    //LayoutConfig->addWidget(mLabelLessWrite, 3, 2);
    //LayoutConfig->addWidget(mLineEditLessWrite, 3, 3);



    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOk()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));


    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonClose);
    mLayoutButton->addStretch();

    QVBoxLayout *LayoutMain = new QVBoxLayout(this);
    LayoutMain->addWidget(mGroupBoxServiceList);
    LayoutMain->addStretch();
    LayoutMain->addWidget(mLabelResult, 0, Qt::AlignCenter);
    LayoutMain->addLayout(mLayoutButton);
    setLayout(LayoutMain);

}

TimerItemEdit::~TimerItemEdit()
{

}
void TimerItemEdit::fOk()
{
    if(fCheckServiceMsg())
    {
        mSave = true;
        this->timerstruct.TimerId=mLineEditNameId->text();
        this->timerstruct.serverId=TBase::fGetComboBoxCurrentData(mComboBoxService);
        this->timerstruct.mainId=mLineEditMainDeviceNum->value();
        this->timerstruct.devId=mLineEditSubDeviceNum->value();
        bool ok;
        this->timerstruct.startAddr=mLineEditStartAddr->text().toInt(&ok, 16);

        close();
    }
    else
    {
        mLabelResult->setText(tr("<font color=red><b>字段错误，请重新填写</b></font>"));
    }
}
bool TimerItemEdit::fCheckServiceMsg()
{
    //qDebug()<<TBase::fGetComboBoxCurrentData(mComboBoxService);
    if(mComboBoxService->currentText().isEmpty()) {
        return false;
    }
    if(mLineEditMainDeviceNum->value()<=0) {
        return false;
    }
    if(mLineEditSubDeviceNum->value()<0) {
        return false;
    }
    return true;
}
