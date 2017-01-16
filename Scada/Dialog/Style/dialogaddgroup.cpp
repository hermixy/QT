#include "dialogaddgroup.h"
#include <QHBoxLayout>
DialogAddGroup::DialogAddGroup(QWidget *parent):QDialog(parent)
{
    groupId=0;
    groupName=QString();
    QGridLayout *mLayoutMain = new QGridLayout(this);
    mgroupName=TBase::fLabel(tr("组名"), tr("mgroupName"));
    mGroupId=TBase::fLabel(tr("组号"), tr("mGroupId"));
    mEditButtonText=TBase::fLineEdit("mEditButtonText");
    mSpinBoxGropu=TBase::fSpinBox("mSpinBoxGropu", 0, 1000);


    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    mLayoutMain->addWidget(mgroupName, 0, 0);

    mLayoutMain->addWidget(mEditButtonText, 0, 1);
    mLayoutMain->addWidget(mGroupId, 1, 0);
    mLayoutMain->addWidget(mSpinBoxGropu, 1, 1);
    mLayoutMain->addWidget(mButtonOk, 2, 0);
    mLayoutMain->addWidget(mButtonClose, 2, 1);
}

DialogAddGroup::~DialogAddGroup()
{

}

void DialogAddGroup::fApply()
{
    groupId=mSpinBoxGropu->value();
    groupName=mEditButtonText->text();
    close();
}
