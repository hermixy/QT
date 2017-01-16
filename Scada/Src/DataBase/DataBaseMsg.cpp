#include "DataBaseMsg.h"

#include <QDebug>

TDataBaseMsg::TDataBaseMsg()
{
    mType = -1;
    /*mHost = QString();
    mDBName = QString();
    mUserName = QString();
    mPassWord = QString();*/
}

TDataBaseMsg::~TDataBaseMsg()
{}

bool TDataBaseMsg::fIsAvailable()
{
    //qDebug() << "TDataBaseMsg::fIsAvailable()" << mHost.length() << mHost;

    if(mType >= 0
        && mHost.length() > 0
        && mDBName.length() > 0
        && mUserName.length() > 0
        && mPassWord.length() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TDataBaseMsg::fSetType(int Type)
{
    mType = Type;
}

int TDataBaseMsg::fGetType() const
{
    return mType;
}

void TDataBaseMsg::fSetHost(const QString &Host)
{
    mHost = Host;
}

QString TDataBaseMsg::fGetHost() const
{
    return mHost;
}

void TDataBaseMsg::fSetDBName(const QString &Name)
{
    mDBName = Name;
}

QString TDataBaseMsg::fGetDBName() const
{
    return mDBName;
}

void TDataBaseMsg::fSetUserName(const QString &Name)
{
    mUserName = Name;
}

QString TDataBaseMsg::fGetUserName() const
{
    return mUserName;
}

void TDataBaseMsg::fSetPassWord(const QString &PassWord)
{
    mPassWord = PassWord;
}

QString TDataBaseMsg::fGetPassWord() const
{
    return mPassWord;
}

