#ifndef GONG_CONFIG_H
#define GONG_CONFIG_H

#include <QApplication>
#include <QString>

class TGongConfig
{
    Q_DECLARE_TR_FUNCTIONS(TJs)
public:
    TGongConfig();
    TGongConfig(const TGongConfig *CopyConfig);

    static void fGetConfigFromFile(const QString FileDir, QList<TGongConfig *> &ConfigList);

    int mId;
    QString mName;
    QString mUnit;
    int mAddr;
    int mChan;
    double mZero;
    double mFull;
    double mRange;
    double mAlarmUpper;
    double mAlarmLower;
    int mDisplayType;
    double mCoe;
    double mCycle;
    double mTime;
};

#endif // GONG_CONFIG_H
