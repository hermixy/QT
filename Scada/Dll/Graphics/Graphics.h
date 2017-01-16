#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_global.h"

#include "../../Src/Item/Item.h"
#include "../../Src/Js/Js.h"

#include <QTimer>

class QVariant;

///////////////////////  图形基类 /////////////////////////////////////
class GRAPHICSSHARED_EXPORT TGraphics : public TItem
{
    Q_OBJECT
public:
    TGraphics(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineSyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TGraphics();

    TJs mJsValue;               // 动作脚本
    TJs mJsAlarm;               // 判断是否启动报警的脚本
    TJs mJsLinkageSend;         // 判断是否发送联动信号
    TJs mJsLinkageActive;       // 执行联动

    /////// AUTO
    enum
    {
        AUTO_NONE       = -1,
        AUTO_ONCE       = 0,
        AUTO_REPEAT     = 1
    };

public slots:
    // 链接图形
    virtual void fSetLinkScene(int LinkId);
    virtual int fGetLinkScene() const;

    // 服务
    virtual void fSetServiceID(const int ID);
    virtual int fGetServiceID() const;
    virtual void fSetNodeID(const int ID);
    virtual int fGetNodeID() const;

    // 值
    virtual void fSetCoe(const double Coe);
    virtual double fGetCoe() const;
    virtual void fSetValue(const double Value);                     // 设定值，并运行值脚本，初始化自身外形,联动脚本
    virtual double fGetValue() const;
    virtual void fUpdateValue(const double Value, qint64 Time); // 设定值，并运行值脚本、报警脚本、联动脚本等
    virtual void fRecvData(int NodeId, double Value, quint64 Time);

    // 报警
    virtual void fStartAlarm();
    virtual void fSetAlarmEnable(bool Enable);
    virtual bool fGetAlarmEnable();

    // 自动复位
    virtual void fSetAutoType(int Type);
    virtual int fGetAutoType();
    virtual void fSetAutoTime(int Time);
    virtual int fGetAutoTime();
    virtual void fSetAutoValue(double Value);
    virtual double fGetAutoValue();
    virtual void fStartAuto();

    // 备注信息
    virtual void fSetHint(QString Hint);
    virtual QString fGetHint();

    // 图形联动
    virtual void fSetLinkageSend(int _iLinkage);
    virtual int fGetLinkageSend();
    virtual void fSetLinkageActive(int _iLinkage);
    virtual int fGetLinkageActive();
    virtual int fGetLinkageRecv();

    virtual void fSendLinkage(bool Enable);                    // 发送联动信号
    virtual void fRecvLinkage(bool Enable, int _iLinkage);       // 接收联动信号

Q_SIGNALS:
    void mValueChanged(double Value, qint64 Time);
    void mStartAlarm(double Value, qint64 Time);
    void mGotoLink(int LinkId);
    void mSendLinkage(bool enable, int _iLinkage);       // 向外发送联动信号

protected:
    int mLinkScene;         // 链接的Scene号

    int mLinkageSend;       // 发送链接号
    int mLinkageActive;     // 激活链接的阈值，达到这个值，就启动动作
    int mLinkageRecv;       // mLinkageActive
    bool mAlarmEnable;      // 是否启用自动报警

    int mNodeId;
    int mServiceId;

    double mValue;
    double mCoe;
    quint64 mValueUpdateTime;

    QTimer *mTimer;
    int mAutoType;
    int mAutoTime;
    double mAutoValue;

    QString mHint;

    virtual void fMsgCopy(QObject *To, QObject *From);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots:
    virtual void fAutoRunOnce();
    virtual void fAutoRunRepeat();

private:
    virtual void fRunJsValue();
    virtual void fRunJsAlarm();
    virtual void fRunJsLinkageSend();
    virtual void fRunJsLinkageActive();
};

#endif // GRAPHICS_H
