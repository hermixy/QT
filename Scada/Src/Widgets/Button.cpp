#include "Button.h"
#ifdef RUNNER
#include "Runer.h"
#endif
#include "../Base/LibBase.h"
#include <QDebug>

TButton::TButton(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    TWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Button);
    mText = tr("Button");
    mPressed = false;
    mLinkScene=0;
    mFontSize=12;
}

TButton::~TButton()
{}

TItem* TButton::fCopy()
{
    TButton *Button = new TButton();

    Button->fCopy(this);

    return Button;
}

void TButton::fCopy(TButton *ButtonFrom)
{
    if(NULL == ButtonFrom)
    {
        return;
    }

    TWidgets::fCopy(ButtonFrom);

    this->fSetText(ButtonFrom->fGetText());
    this->fSetLinkScene(ButtonFrom->fGetLinkScene());
    this->mcomNum=ButtonFrom->mcomNum;
    this->mBeginTime=ButtonFrom->mBeginTime;
    this->mEndTime=ButtonFrom->mEndTime;
    this->mBeginWeek=ButtonFrom->mBeginWeek;
    this->mEndWeek=ButtonFrom->mEndWeek;
    this->greaterEqual=ButtonFrom->greaterEqual;
    this->mPixPath=ButtonFrom->mPixPath;
    this->width=ButtonFrom->width;
    this->heigth=ButtonFrom->heigth;
    this->fSetFontSize(ButtonFrom->fGetFontSize());
    this->control=ButtonFrom->control;
    this->isShowTimer=ButtonFrom->isShowTimer;
}

void TButton::fSetText(QString Text)
{
    mText = Text;
}

QString TButton::fGetText()
{
    return mText;
}

//void TButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
void TButton::fDraw(QPainter *painter)
{
    //fBeforePaint(painter, option, widget);
    //qDebug()<<"dram Button";

    if(!mPixPath.isEmpty())
    {   currentPixmap=QPixmap(mPixPath);
        painter->drawPixmap(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, QPixmap(mPixPath));
    }
    else
    {
     painter->save();

    double Width = mStopX - mStartX;        // 绘图区域的宽度
    double Height = mStopY - mStartY;       // 绘图区域的高度
    double CenterX = mStartX + Width / 2;   // 绘图区域的中心X
    //double CenterY = mStartY + Height / 2;  // 绘图区域的中心Y
    painter->setRenderHint(QPainter::Antialiasing); // 去锯齿

    double Roundness = MIN(Width, Height) * 0.1; //圆角

    //painter->setPen(QPen(QBrush(Qt::gray), 2.0));
    QLinearGradient lg1(CenterX, mStartY, CenterX, mStopY);

    if(mPressed)
    {
        //qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
        lg1.setColorAt(0, QColor(0, 0, 0));
        lg1.setColorAt(0.4, QColor(255, 255, 255));
        lg1.setColorAt(1, QColor(100, 100, 100));

        //painter->setBrush(lg1);
        painter->setPen(QPen(QBrush(mBackGroundColor), 5.0));
        //painter->setPen(QPen(QBrush(lg1), 2.0));

    }
    else
    {
        lg1.setColorAt(0, QColor(0, 0, 0));

        lg1.setColorAt(0.4, QColor(255, 255, 255));
        lg1.setColorAt(1, Qt::lightGray);
        painter->setPen(QPen(QBrush(lg1), 5.0));
    }

    //painter->setBrush(lg1);

    QPainterPath OutLine;
    OutLine.addRoundedRect(mStartX, mStartY, Width, Height, Roundness, Roundness);
    //painter->setOpacity(m_opacity);

    painter->fillPath(OutLine, mBackGroundColor);
    if(mPressed)
    {
        painter->setPen(QPen(QBrush(lg1), 2.0));
        painter->fillPath(OutLine, mBackGroundColor);
    }
    painter->drawPath(OutLine);

    //文字
    painter->restore();
    QFont Font;
    Font.setPointSize(mFontSize);
    painter->setFont(Font);

    painter->drawText(mStartX, mStartY, Width, Height, Qt::AlignCenter, mText);
    }
}

void TButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "TButton::mousePressEvent";

    mPressed = true;
    //update();
   // qDebug()<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
    foreach(link data, greaterEqual)
    {
#ifdef RUNNER
        if(!Runer::runner->isAdmins)
            break;
#endif
        emit sendData(data.groupId, data.writeNum);    // 写入数据
    }
    foreach (Control contral, control) {
#ifdef RUNNER
        if(!Runer::runner->isAdmins)
            break;
#endif
        emit sendContral(contral);
    }
    if(mLinkScene > 0)
    {
        emit mGotoLink(mLinkScene);
        emit mSendCommand(mcomNum);

    }
    if(isShowTimer)
    {
        emit mGotoLink(0);
    }
    TItem::mousePressEvent(event);
}

void TButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "TButton::mouseReleaseEvent";
    mPressed = false;
    TItem::mouseReleaseEvent(event);

    //mJsWidgets.fRun(this, tr("fPlay"));
}
void TButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    TItem::hoverEnterEvent(event);
    setCursor(Qt::PointingHandCursor);
    //qDebug()<<"ssssssssssssssss";
}
void TButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

    TItem::hoverLeaveEvent(event);
    setCursor(Qt::ArrowCursor);
}

void TButton::fSetLinkScene(int LinkId)
{
    mLinkScene = LinkId;
}

int TButton::fGetLinkScene() const
{
    return mLinkScene;
}
void TButton::fSetFontSize(int Size)
{
    mFontSize = Size;
}

int TButton::fGetFontSize() const
{
    return mFontSize;
}
