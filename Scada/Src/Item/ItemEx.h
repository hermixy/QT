#ifndef TITEM_EX_H
#define TITEM_EX_H

#include "Item.h"

class TItemEx : public TItem
{
    Q_OBJECT

public:
    TItemEx(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TItemEx();

    double showStart=0, showStop=0;
    qint64 timertotal=0;
signals:
    void sendData(int groupId, double value);    //往组里面写入值

public slots:
    virtual void setStableValue();
    virtual void setpercent();
    virtual void setLinkStart();
    virtual void fCopy(TItemEx *ItemExFrom);

    virtual void fSetNameId(const QString &NameId);
    virtual QString fGetNameId() const;

    virtual void fSetServiceId(int Id);
    virtual int fGetServiceId() const;

    // 备注信息
    virtual void fSetHint(const QString &Hint);
    virtual QString fGetHint() const;

    virtual void fSetHostScene(QObject *HostScene);
    virtual QObject *fGetHostScene() const;

    virtual void fSetUserDataName1(const QString &UserDataName);
    virtual QString fGetUserDataName1() const;
    virtual void fSetUserDataName2(const QString &UserDataName);
    virtual QString fGetUserDataName2() const;
    virtual void fSetUserData1(const double UserData);
    virtual double fGetUserData1() const;
    virtual void fSetUserData2(const double UserData);
    virtual double fGetUserData2()const;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

protected:

    QString mNameId;
    int mServiceId;
    QString mHint;

    QString mUserDataName1;
    QString mUserDataName2;
    double mUserData1;
    double mUserData2;

    QObject *mHostScene;
};

#endif // TITEM_EX_H
