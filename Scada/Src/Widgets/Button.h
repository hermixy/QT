#ifndef TBUTTON_H
#define TBUTTON_H

#include "Widgets.h"

#include <QTime>
#include <QApplication>

///////////////////////  控件基类 /////////////////////////////////////
class TButton : public TWidgets
{
    Q_OBJECT
public:
    TButton(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TButton();
    struct link
            {
        int groupId;
        double writeNum;
    };
    struct Control
    {
        QString servername;
        int mainId;
        int devId;
        int startAddr=-1;
        double writeNum;
    };

    void fSetFontSize(int Size);
    int fGetFontSize() const;
public slots:
    TItem *fCopy();
    void fCopy(TButton *ButtonFrom);

    void fSetText(QString Text);
    QString fGetText();

    // 链接图形
    virtual void fSetLinkScene(int LinkId);
    virtual int fGetLinkScene() const;
Q_SIGNALS:
    void mStartAlarm(double Value, qint64 Time);
    void mGotoLink(int LinkId);
    void mSendCommand(int commandNum);  //发送指令
    void sendContral(TButton::Control contral);


public:
    int mcomNum;             //要发送的指令号  0、开   1、关
    int mBeginWeek;
    QTime mBeginTime;        //开始时间

    int mEndWeek;          //结束时间
    QTime mEndTime;          //结束时间
    QList<link > greaterEqual;          //写入指令
    QList<Control > control;          //写入指令
    QString mPixPath;                  //背景图片路径
    int width=0, heigth=0;        //
    bool isShowTimer=false;

private:
    bool mPressed;
    QString mText;
    int mLinkScene;         // 链接的Scene号
    QPixmap currentPixmap;
    int mFontSize;

    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // TBUTTON_H
