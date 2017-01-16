#include "DataBase.h"

TDataBase::TDataBase()
{
}

TDataBase::~TDataBase()
{
    fClose();
}

bool TDataBase::fIsOpen()
{
    return mDB.isOpen();
}

bool TDataBase::fClose()
{
    if(mDB.isOpen())
    {
        mDB.close();
        return true;
    }

    return false;
}

bool TDataBase::fAvailable()
{
    return mDB.commit();
}

bool TDataBase::KeepConnect()
{
    //qDebug() << "TDataBase::KeepConnect()" << mDB.isOpen();

    if(!mDB.isOpen())
    {
       return false;
    }

    // 判断是否断开了，假如断开了，则重连
    if(!fAvailable())
    {
        mDB.close();
        return mDB.open();
    }

    return true;
}

bool TDataBase::fCheckTabel(QString TabelName)
{
    if(!KeepConnect())
    {
        return false;
    }

    QSqlQuery mQuery(mDB);

    //select 1 from sysobjects where id = object_id('users'))
    bool qu = mQuery.exec(tr("select 1 from sysobjects where id = object_id('") + TabelName + tr("'))"));
    //qDebug() << TabelName;
    //qDebug() << tr("select 1 from sysobjects where id = object_id('") + TabelName + tr("'))");
    //qDebug() << qu;

    return qu;
}

bool TDataBase::fDoSql(QString Sql)
{
    if(!KeepConnect())
    {
        return false;
    }

    QSqlQuery mQuery(mDB);
    return mQuery.exec(Sql);
}

QList<QVariant> TDataBase::fDoSqlOneResult(QString Sql)
{
    QList<QVariant> OneResult;

    if(!KeepConnect())
    {
        return OneResult;
    }

    QSqlQuery mQuery(mDB);

    if(!(Sql.contains(tr("LIMIT")) || Sql.contains(tr("limit"))))
    {
        Sql += tr(" LIMIT 1");
    }

    if(mQuery.exec(Sql))
    {
        if(mQuery.next())
        {
            for(int i = 0; i < mQuery.record().count(); i++)
            {
                OneResult.push_back(mQuery.value(i));
            }
        }
    }

    return OneResult;
}

QList< QList<QVariant> > TDataBase::fDoSqlResult(QString Sql)
{
    QList< QList<QVariant> > Result;

    if(!KeepConnect())
    {
        return Result;
    }

    QSqlQuery mQuery(mDB);

    //qDebug() << "mQuery.exec(Sql)" << Sql;

    if(mQuery.exec(Sql))
    {
        while(mQuery.next())
        {
            QList<QVariant> ResultLine;
            //qDebug() << "mQuery.record().count()" << mQuery.record().count();

            for(int i = 0; i < mQuery.record().count(); i++)
            {
                ResultLine.push_back(mQuery.value(i));
            }

            Result.push_back(ResultLine);
        }
    }

    return Result;
}
