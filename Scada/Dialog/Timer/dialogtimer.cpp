#include "dialogtimer.h"
#include "ui_dialogtimer.h"
#include <QDebug>
#include <QPushButton>
#include  <QDesktopWidget>
#include <QTimeEdit>
#include "Runer.h"
#include <QVBoxLayout>
DialogTimer::DialogTimer(TScene* mSceneList, QWidget *parent) :
    QDialog(parent),myscene(mSceneList),
    ui(new Ui::DialogTimer)
{
    if(mSceneList==nullptr)
        return;
    ui->setupUi(this);

    int i=2;
    int j=0;
    QWidget *page=nullptr;
    QVBoxLayout *hyout=nullptr;
    QHash<QString, QList<TimerOnOff>> timerOnPff=Runer::runner->sceneToTImer.value(mSceneList);
    foreach(TScene::TimerStruct timerItemScene, mSceneList->TImerStructList)
    {
            if(!timerMapToItem.contains(timerItemScene.TimerId))
            {
                if(j%12==0) {
                    if(hyout)
                    {
                        QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

                        hyout->addItem(verticalSpacer);

                    }
                    page=new QWidget();
                    hyout=new QVBoxLayout(page);
                    hyout->setContentsMargins(0, 0, 0, 0);
                    hyout->setSpacing(0);

                    ui->stackedWidget->addWidget(page);
                }
                if(hyout)
                {
                    TimerFrameItem1 *item= new TimerFrameItem1(timerItemScene.TimerId, timerOnPff.value(timerItemScene.TimerId), this);
                    hyout->addWidget(item);

                    timerMapToItem.insert(timerItemScene.TimerId, item);
                }
                j++;
            }
    }
    if(hyout)
    {
        QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        hyout->addItem(verticalSpacer);

    }
    QHBoxLayout *hlayout=new QHBoxLayout();
    QPushButton *button=new QPushButton(this);
    button->setText(tr("应用"));
    button->setMaximumWidth(200);
    connect(button, SIGNAL(clicked()), this, SLOT(applay()));
    hlayout->addStretch();
    hlayout->addWidget(button);



    QPushButton *buttonclose=new QPushButton(this);
    buttonclose->setText(tr("关闭"));
    buttonclose->setMaximumWidth(200);

    connect(buttonclose, SIGNAL(clicked()), this, SLOT(fclose()));
    hlayout->addWidget(buttonclose);

    ui->horizontalLayout_2->insertLayout(i, hlayout);
    setWindowTitle(tr("当前定时表"));
    this->setMinimumSize(QApplication::desktop()->size()-QSize(40, 200));
}

DialogTimer::~DialogTimer()
{
    delete ui;
}

void DialogTimer::applay()
{
    mSave=true;
    timerMapToGroup.clear();
    for(QHash<QString, TimerFrameItem1 *>::iterator i=timerMapToItem.begin(); i!=timerMapToItem.end(); i++)
    {
        qDebug()<<i.value()->getTimer().size();
        timerMapToGroup.insert(i.key(), i.value()->getTimer());
    }
    emit saveDateTimer();
}
void DialogTimer::fclose()
{
    //qDebug()<<"close timer";
    close();
    mSave=false;
    emit saveDateTimer();
}

void DialogTimer::on_pushButton_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(index-1);
}

void DialogTimer::on_pushButton_2_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(index+1);
}
