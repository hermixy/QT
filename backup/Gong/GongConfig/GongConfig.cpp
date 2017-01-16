#include "GongConfig.h"

#include "../Scada/Src/File/File.h"

#include <QMessageBox>
#include <QDebug>

TGongConfig::TGongConfig()
{}

TGongConfig::TGongConfig(const TGongConfig *CopyConfig)
{
    mId = CopyConfig->mId;
    mName = CopyConfig->mName;
    mUnit = CopyConfig->mUnit;
    mAddr = CopyConfig->mAddr;
    mChan = CopyConfig->mChan;
    mZero = CopyConfig->mZero;
    mFull = CopyConfig->mFull;
    mRange = CopyConfig->mRange;
    mAlarmUpper = CopyConfig->mAlarmUpper;
    mAlarmLower = CopyConfig->mAlarmLower;
    mDisplayType = CopyConfig->mDisplayType;
    mCoe = CopyConfig->mCoe;
    mCycle = CopyConfig->mCycle;
    mTime = CopyConfig->mTime;
}

void TGongConfig::fGetConfigFromFile(const QString FileDir, QList<TGongConfig *> &ConfigList)
{
    TFile File;

    if(!File.fOpen(FileDir, QIODevice::ReadOnly))
    {
        qDebug() << "open err:" << FileDir;
        return;
    }

    foreach(TGongConfig *Config, ConfigList)
    {
        ConfigList.removeOne(Config);
        delete Config;
    }

    QString Data;

    while(File.fReadLn(Data))
    {
        //qDebug() << "read file:" << Data;
        QStringList StringList = Data.split(tr(","));

        QList<QString>StrList;

        foreach(QString mStr, StringList)
        {
            //qDebug() << "analy file:" << mStr;
            StrList.push_back(mStr);
        }

        if(StrList.length() >= 14)
        {
            // 添加至list
            TGongConfig *Config = new TGongConfig();

            if(Config)
            {
                Config->mId = StrList.at(0).toInt();
                Config->mName = StrList.at(1);
                Config->mAddr = StrList.at(2).toInt();
                Config->mChan = StrList.at(3).toInt();
                Config->mZero = StrList.at(4).toDouble();
                Config->mFull = StrList.at(5).toDouble();
                Config->mRange = StrList.at(6).toDouble();
                Config->mUnit = StrList.at(7);
                Config->mAlarmUpper = StrList.at(8).toDouble();
                Config->mAlarmLower = StrList.at(9).toDouble();
                Config->mDisplayType = StrList.at(10).toInt();
                Config->mCoe = StrList.at(11).toDouble();
                Config->mCycle = StrList.at(12).toDouble();
                Config->mTime = StrList.at(13).toDouble();

                ConfigList.push_back(Config);
            }
        }
    }

    File.fClose();
}
