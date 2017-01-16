#include "MySQL.h"
#ifdef RUNNER
#include "Runer.h"
#endif
TMySQL::TMySQL() : TDataBase()
{
    qDebug()<<"init database";
    model=new QSqlQueryModel();
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    view=new QTableView();
    view->setMinimumSize(800, 500);
}

TMySQL::~TMySQL()
{
    delete model;
    delete view;
}

bool TMySQL::fInit(QString Host, QString DBName, QString UserName, QString PassWord)
{
    //qDebug()<<"true"<<"1233";
    mDB = QSqlDatabase::addDatabase(QString("QMYSQL"));
    mDB.setHostName(Host);
    mDB.setDatabaseName(DBName);
    mDB.setUserName(UserName);
    mDB.setPassword(PassWord);
    if(mDB.open())
    {
        qDebug()<<"true"<<"1233";
        return true;
    }
    else
    {
        qDebug()<<"true"<<"568";
        mDB.close();
        mDB.removeDatabase(tr("QMYSQL"));
        return false;
    }
}
bool TMySQL::qsqlExec(QString sql)
{
    QSqlQuery query(mDB);
    return query.exec(sql);
}
QTableView *TMySQL::creatTableView(int tableName, QDateTime startTime, QDateTime stopTime)
{
#ifdef RUNNER
    QString sql;
    sql=QString("select * from mysql%1 where date>%2 and date<%3").arg(tableName).arg(startTime.toMSecsSinceEpoch()).arg(stopTime.toMSecsSinceEpoch());
    model->setQuery(sql);

    QList<TGraphics *> * list=Runer::runner->groupToGraphics.value(tableName);

    for(int i=0; i<list->size(); i++)
    {
        if(list->at(i)->fGetNameId().isEmpty())
            model->setHeaderData(i, Qt::Horizontal, QString("value%1").arg(i));
        else
            model->setHeaderData(i, Qt::Horizontal, list->at(i)->fGetNameId());
    }
    view->setModel(model);
#endif
    return view;
}
QTableView *TMySQL::creatTableView(QString tableName)
{
    QString sql;
    sql=QString("select * from %1").arg(tableName);
    model->setQuery(sql);
    view->setModel(model);
    return view;
}
