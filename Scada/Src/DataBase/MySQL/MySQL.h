#ifndef MYSQL_H
#define MYSQL_H

#include "../DataBase.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QApplication>
#include <QTableView>

class TMySQL : public TDataBase
{
    Q_DECLARE_TR_FUNCTIONS(TMySQL)

public:
    TMySQL();
    ~TMySQL();
    bool fInit(QString Host, QString DBName, QString UserName, QString PassWord);
    bool qsqlExec(QString sql);      //创建数据库
    QTableView *creatTableView(int tableName, QDateTime startTime, QDateTime stopTime);
    QTableView *creatTableView(QString tableName);


private:
    QSqlQueryModel *model;
    QTableView *view;

};

#endif // MYSQL_H
