#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <QtSql>
#include <QApplication>

class TDataBase
{
    Q_DECLARE_TR_FUNCTIONS(TDataBase)

public:
    TDataBase();
    virtual ~TDataBase();

    virtual bool fInit(QString Host, QString DBName, QString UserName, QString PassWord) = 0;
    virtual bool fIsOpen(); // 数据库是否打开
    virtual bool fClose();
    virtual bool fCheckTabel(QString TabelName);

    virtual bool fAvailable();

    virtual bool fDoSql(QString Sql);
    virtual QList<QVariant> fDoSqlOneResult(QString Sql);
    virtual QList< QList<QVariant> > fDoSqlResult(QString Sql);
protected:
    QSqlDatabase mDB;

private:
    bool KeepConnect();
};

#endif // DATA_BASE_H
