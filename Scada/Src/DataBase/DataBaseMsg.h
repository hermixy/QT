#ifndef DATA_BASE_MSG_H
#define DATA_BASE_MSG_H

#include <QHostInfo>
#include <QString>

class TDataBaseMsg
{
public:
    TDataBaseMsg();
    ~TDataBaseMsg();

    bool fIsAvailable();

    void fSetType(int Type);
    int fGetType() const;
    void fSetHost(const QString &Host);
    QString fGetHost() const;
    void fSetDBName(const QString &Name);
    QString fGetDBName() const;
    void fSetUserName(const QString &Name);
    QString fGetUserName() const;
    void fSetPassWord(const QString &PassWord);
    QString fGetPassWord() const;

private:
    int mType;      // 数据库类型
    QString mHost=QHostInfo::localHostName();
    QString mDBName;
    QString mUserName;
    QString mPassWord;
};

#endif // DATA_BASE_MSG_H
