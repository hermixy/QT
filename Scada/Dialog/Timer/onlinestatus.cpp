#include "onlinestatus.h"
#include "ui_onlinestatus.h"
#include <QObject>
#include <QMap>
OnlineStatus::OnlineStatus(QList<TService *> mServiceList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlineStatus)
{
    ui->setupUi(this);
    int i=0;
    setWindowTitle(tr("在线状态"));
    QWidget *page=nullptr;
    QVBoxLayout *hlaout=nullptr;
    qDebug()<<mServiceList.size();
    foreach(TService *server, mServiceList)
    {
        if(i%9==0) {
            page=new QWidget();
            hlaout=new QVBoxLayout(page);
            ui->stackedWidget->addWidget(page);
        }
        if(hlaout)
        {
            QHBoxLayout *myhlayout=new QHBoxLayout();
            QLabel *labelip=new QLabel(server->fGetServerData()->fGetIp());
            myhlayout->addWidget(labelip);
            QLabel *labelStatus=new QLabel(tr("在线状态"));
            myhlayout->addWidget(labelStatus);
            QLabel *labelStatuss=new QLabel(tr("离线"));
            labelStatuss->setStyleSheet("color:red");
            myhlayout->addWidget(labelStatuss);
            QLabel *leaveTiem=new QLabel();
            myhlayout->addWidget(leaveTiem);
            QList<QLabel *> labelList;
            labelList.append(labelip);
            labelList.append(labelStatus);
            labelList.append(labelStatuss);
            labelList.append(leaveTiem);
            ipToLabel.insert(server->fGetServerData()->fGetIp(), labelList);
            hlaout->addLayout(myhlayout);
        }
        i++;
    }
}

OnlineStatus::~OnlineStatus()
{
    delete ui;
}

void OnlineStatus::on_pushButton_2_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(index+1);
}

void OnlineStatus::on_pushButton_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(index-1);
}
