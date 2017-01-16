#ifndef ACCESS_H
#define ACCESS_H

#include "../DataBase.h"

#include <QApplication>

class TAccess : public TDataBase
{
    Q_DECLARE_TR_FUNCTIONS(TAccess)

public:
    TAccess();
    bool fInit(QString Host, QString DBName, QString UserName, QString PassWord);
    //bool fCheckTabel(QString TabelName);

    //bool fDoSql(QString Sql);
//private:
    //QSqlDatabase mDB;
};

#endif // ACCESS_H
