#include "showhistory.h"
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include "Runer.h"
#include <QDateTimeEdit>
#include <QDebug>
#include <QAbstractTableModel>

ShowHistory::ShowHistory(QList<TScene*> mSceneList, QWidget *parent, bool isreport):QDialog(parent)
{
    mLayoutMain=new QGridLayout(this);

    int i=0, j=0;
    foreach(TScene *Scene, mSceneList)
    {
        QList<TGraphics *>ItemExList;
        Scene->fGetItemsList(ItemExList);

        foreach(TGraphics *ItemEx, ItemExList)
        {
            if(ItemEx->fGetGroupId()<=0)
            {
                continue;
            }
            if(!groupIdList.contains(ItemEx->fGetGroupId()))
            {
                QCheckBox *button=new QCheckBox(this);

                button->setText(ItemEx->fGetGroupName());
                groupIdList.insert(ItemEx->fGetGroupId(), ItemEx->fGetGroupName());
                mLayoutMain->addWidget(button, i, j);
                if(j++==10)
                {
                    j=0;
                    i++;
                }
            }
        }
        QPushButton *seeButton=TBase::fButton(tr("查看"), tr("seeButton"), 75, 20);
        connect(seeButton, SIGNAL(clicked()), this, SLOT(showHistory()));
        QPushButton *closeButton=TBase::fButton(tr("关闭"), tr("closeButton"), 75, 20);
        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

        QPushButton *reportButton=TBase::fButton(tr("导出报表"), tr("closeButton"), 75, 20);
        connect(reportButton, SIGNAL(clicked()), this, SLOT(report()));

        startTIme=new QDateTimeEdit(this);

        stopTIme=new QDateTimeEdit(this);
        startTIme->setDateTime(QDateTime::currentDateTime());
        stopTIme->setDateTime(QDateTime::currentDateTime());

            if(isreport)
            {
                mLayoutMain->addWidget(startTIme, i+1, 0);
                mLayoutMain->addWidget(stopTIme, i+1, 1);
                mLayoutMain->addWidget(closeButton, i+1, 2);
                mLayoutMain->addWidget(reportButton, i+1, 3);
            }
            else
            {
                mLayoutMain->addWidget(startTIme, i+1, 0);
                mLayoutMain->addWidget(stopTIme, i+1, 1);
                mLayoutMain->addWidget(seeButton, i+1, 2);
                mLayoutMain->addWidget(closeButton, i+1, 3);
            }

    }
    this->setMinimumSize(400, 500);
}

ShowHistory::~ShowHistory()
{

}
void ShowHistory::showHistory()
{
    QTableView *view=nullptr;

    QTableView *tabview=new QTableView(0);
    QList<QCheckBox *> allcheckBox=this->findChildren<QCheckBox *>();
    foreach(QCheckBox *check, allcheckBox)
    {
        if(check->isChecked())
        {
            int groupid=groupIdList.key(check->text());
            if(Runer::runner->mDataBase)
            {
                QStandardItemModel *model=new QStandardItemModel();

                tabview->setMinimumSize(800, 500);
                tabview->setAttribute(Qt::WA_DeleteOnClose);
                tabview->setModel(model);

                TMySQL * mysql=static_cast<TMySQL *>(Runer::runner->mDataBase);
                view=mysql->creatTableView(groupid, startTIme->dateTime(), stopTIme->dateTime());
                view->setWindowTitle(check->text());
                for(int i=0; i<view->model()->rowCount(); i++)
                {
                    /*QMap<int, QVariant> map;
                    map.insert(0,"1");
                    view->setEditTriggers(QAbstractItemView::CurrentChanged);
                    if(!view->model()->setItemData(view->model()->index(i, view->model()->columnCount()-1),map))
                    {
                       qDebug()<<"shezhi shibai"<<view->model()->columnCount()-1;
                    }*/
                    for(int j=0; j<view->model()->columnCount(); j++)
                    {

                        QString name=view->model()->index(i, j).data().toString();
                        if(j==view->model()->columnCount()-1)
                        {
                            name=QDateTime::fromMSecsSinceEpoch(name.toLongLong()).toString(Qt::ISODate);
                        }
                        qDebug()<<name;
                        QStandardItem *item=new QStandardItem;
                        item->setText(name);
                        model->setItem(i, j, item);
                    }
                }
            }
            else
            {
                close();
                return;

            }
        }
    }
    close();
    if(view)
    {
        tabview->show();
        tabview->activateWindow();
    }
}
void ShowHistory::report()
{
    //QTableView *view=nullptr;
    QList<QCheckBox *> allcheckBox=this->findChildren<QCheckBox *>();
    foreach(QCheckBox *check, allcheckBox)
    {
        if(check->isChecked())
        {
            int groupid=groupIdList.key(check->text());
            if(Runer::runner->mDataBase)
            {
                TMySQL * mysql=static_cast<TMySQL *>(Runer::runner->mDataBase);
                //view=mysql->creatTableView(groupid, startTIme->dateTime(), stopTIme->dateTime());
                //view->setWindowTitle(check->text());

                if (Runer::runner->excel(mysql->creatTableView(groupid, startTIme->dateTime(), stopTIme->dateTime()))>0 )//调用导出Excel方法
                {
                    QSqlDatabase::removeDatabase("excelexport");
                }
            }
            else
            {
                close();
                return;

            }
        }
    }
    close();
}
