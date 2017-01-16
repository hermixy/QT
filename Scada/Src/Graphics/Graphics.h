#ifndef TGRAPHICS_H
#define TGRAPHICS_H

#include "../Item/Item.h"
#include "../Item/ItemEx.h"
#include "../Js/Js.h"
#include "../Base/LibBase.h"
#include "../Widgets/Button.h"
#include <QTime>

#include <QTimer>

class QVariant;

///////////////////////  图形基类 /////////////////////////////////////
class TGraphics : public TItemEx
{
    Q_OBJECT
public:
    TGraphics(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TGraphics();
    struct link
            {
        int currntNum;
        int groupId;
        double writeNum;
    };

    enum
    {
        JS_MIN,
        JS_VALUE,
        JS_AUTO,
        JS_MAX
    };

    enum
    {
        AUTO_NONE           = -1,
        AUTO_AFTER_ALARM    = 0,
        AUTO_REPEAT         = 1
    };

public slots:
    virtual void setStableValue();
    virtual void setpercent();
    virtual void setLinkStart();
    virtual void fCopy(TGraphics *GraphicsFrom);

    // 链接图形
    virtual void fSetLinkScene(int LinkId);
    virtual int fGetLinkScene() const;

    // 通道号
    virtual void fSetNodeId(const int Id);
    virtual int fGetNodeId() const;
    // 主地址
    virtual void fSetMainId(const int Id);
    virtual int fGetMainId() const;
    //组Id

    virtual void fSetGroupId(const int Id);
    virtual int fGetGroupId() const;

    //组Name

    virtual void fSetGroupName(const QString Id);
    virtual QString fGetGroupName() const;

    //掉线
    virtual void fSetDisconnect(bool disconnect);
    virtual bool fGetDisconnect();


    // 值
    virtual void fSetCoe(const double Coe);
    virtual double fGetCoe() const;
    virtual void fSetValue(const double Value);                  // 设定值，并运行值脚本，初始化自身外形,联动脚本
    virtual double fGetValue() const;

    virtual inline void fUpdateValue(const double Value, qint64 Time) // 设定值，并运行值脚本、报警脚本、联动脚本等
    {
        double ValueChange = Value * mCoe - mValue;
        //qDebug() << "TGraphics::fUpdateValue" << mValue << Value;
        if(ABS(ValueChange) > 0.000001)
        {
            fSetValue(Value);

            mValueUpdateTime = Time;
        }
    }

    // 报警
    virtual void fStartAlarm();

    void fSetAutoType(int Type);
    int fGetAutoType();
    void fSetAutoTime(int Time);
    int fGetAutoTime();
    //virtual void fStartAuto(int Type, int Time);
    virtual void fStartAuto();

Q_SIGNALS:
    void mStartAlarm(double Value, bool upOrdown, qint64 Time);
    void mGotoLink(int LinkId);
    void sendContral(TButton::Control contral);
public:
    bool crcPosition=false;
    QString threeDevName="modbus rtu";
    QList<link > greaterEqual;          //大于
    QList<link> lessThan;              //小于
    double stableValue=0.0;
    bool isSetstableValue=false;
    double mUpAlarm=0;
    double mDownAlarm=0;
    int setValue=1;
    quint64 mValueUpdateTime;
    int startAddr=-1;

    //联动起作用的时间
    QTime startTime;
    QTime stopTime;

protected:
    int mLinkScene;         // 链接的Scene号

    int mNodeId=-1;            //通道号
    int mGroupId=-1;           //组Id
    int mMainId=0;            //主设备Id
    QString mGroupName;


    double mValue;
    double mCoe;

    bool disconnect=false;

    int mLinkageRecv;       // 当前已经接收到的联动信号相与以后的值

    QTimer *mTimer;
    int mAutoType;
    int mAutoTime;
    bool isALarm=false;
    bool isUpAlarm=false;
    bool isDownAlarm=false;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


private slots:
    //virtual void fAutoRunOnce();
    virtual void fAutoRunRepeat();
};

#endif // TGRAPHICS_H
