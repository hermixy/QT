#include "Access.h"
#include <QDebug>

TAccess::TAccess() : TDataBase()
{}

bool TAccess::fInit(QString Host, QString DBName, QString UserName, QString PassWord)
{
    mDB = QSqlDatabase::addDatabase("QODBC");
    mDB.setDatabaseName(tr("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=")
                        + Host + DBName + tr(";UID='") + UserName + tr("';PWD='") + PassWord + tr("'"));

    if(mDB.open())
    {
        return true;
    }
    else
    {
        mDB.close();
        mDB.removeDatabase("QODBC");
        return false;
    }
}
#if false
bool TAccess::fCheckTabel(QString TabelName)
{
    QSqlQuery mQuery(mDB);

    //select 1 from sysobjects where id = object_id('users'))
    bool qu = mQuery.exec(tr("select 1 from sysobjects where id = object_id('") + TabelName + tr("'))"));
    qDebug() << TabelName;
    qDebug() << tr("select 1 from sysobjects where id = object_id('") + TabelName + tr("'))");
    qDebug() << qu;

    return qu;
}

bool TAccess::fDoSql(QString Sql)
{
    QSqlQuery mQuery(mDB);
    return mQuery.exec(Sql);
}
#endif
