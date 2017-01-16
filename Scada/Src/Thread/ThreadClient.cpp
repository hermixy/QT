#include "ThreadClient.h"

#include <QDebug>

TThreadClient::TThreadClient(TDataBase *DataBase, QObject *parent)
    : TThread(parent)
{
    mDataBase = DataBase;
}

TThreadClient::~TThreadClient()
{
}

void TThreadClient::run()
{
    qDebug() << "TThreadClient::run()" << QThread::currentThread();

    QList< QList<QVariant> > ResultData;

    while(!mStopped)
    {
        if(!mDataBase)
        {
            goto NEXT;
        }

        ResultData.clear();

        if(mDataBase)
        {
            ResultData = mDataBase->fDoSqlResult(tr("SELECT service_id,node_id,val FROM data1"));
        }

        foreach(const QList<QVariant>& SqlRow, ResultData)
        {
            if(SqlRow.length() >= 3)
            {
                emit mValueCome(SqlRow.at(0).toInt(),
                                SqlRow.at(1).toInt(),
                                SqlRow.at(2).toDouble(),
                                0);
            }
        }

NEXT:
        sleep(1);
        continue;
    }

    mStopped = false;
}
