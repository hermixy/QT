#include "alarmdialog.h"
#include <QGridLayout>
#include "../../Src/Base/Base.h"
#include "Runer.h"
AlarmDialog::AlarmDialog(QWidget *parent, QString alarm):QDialog(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    QGridLayout *mLayoutMain = new QGridLayout(this);
    mLabelName = TBase::fLabel(alarm, tr("mLabelName"));

    QPushButton* mButtonLogin = TBase::fButton(tr("处理"), tr("mButtonLogin"), 75, 36);
    connect(mButtonLogin, SIGNAL(clicked()), this, SLOT(handleAlarm()));
    QPushButton * mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonLogin);
    mLayoutButton->addWidget(mButtonClose);

    mLayoutMain->addWidget(mLabelName, 0, 0);
    mLayoutMain->addLayout(mLayoutButton, 1, 0);

}

AlarmDialog::~AlarmDialog()
{

}

void AlarmDialog::handleAlarm()
{
    if(Runer::runner)
        Runer::runner->sound->stop();
    close();
}
void AlarmDialog::setTitle(QString name)
{
    mLabelName->setText(name);
}
