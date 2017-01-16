#include "ttimer.h"

TTimer::TTimer(QWidget *parent):QDialog(parent)
{

    QGridLayout *mLayoutMain = new QGridLayout(this);;
    startTime=new QTimeEdit(this);
    stopTime=new QTimeEdit(this);
    mLayoutMain->addWidget(new QLabel(tr("开始时间"), this), 0, 0);
    mLayoutMain->addWidget(startTime, 0, 1);

    mLayoutMain->addWidget(new QLabel(tr("结束时间"), this), 1, 0);
    mLayoutMain->addWidget(stopTime, 1, 1);

    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonLogin"), 75, 36);

    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fLogin()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    mLayoutMain->addWidget(mButtonOk, 2, 0);
    mLayoutMain->addWidget(mButtonClose, 2, 1);
}

TTimer::~TTimer()
{

}
void TTimer::fLogin()
{
    startQTime=startTime->time();
    stopQTime=stopTime->time();
    close();
}
