#include "Scale.h"

#include "../Base/Base.h"

#include <QPainter>
#include <QDebug>

void TScale::fInit()
{
    setFixedSize(mWidth, mHeight);

    mBar = TBase::fProgressBar(tr("mBar"), mMin, mMax, this);
    mBar->setValue(mValue);
    mBar->setOrientation(Qt::Vertical);
    fSetColor(Qt::darkGreen);

    double ScaleWidth = mWidth / 60;
    double ScaleHeight = mHeight / 200;

    mBar->setFixedSize(20 * ScaleWidth, 150 * ScaleHeight);
    mBar->setGeometry(40 * ScaleWidth, 5 * ScaleHeight, 20 * ScaleWidth, 150 * ScaleHeight);
}

TScale::TScale(int Width, int Height, QWidget *parent) : QWidget(parent)
{
    mMin = 0;
    mMax = 100;
    mStep = 50;
    mWidth = Width;
    mHeight = Height;
    mValue = mMin;
    mHead = tr("当前值");
    mEnd = tr("");

    fInit();
}

TScale::TScale(int Width, int Height, int Min, int Max, int Step, QWidget *parent) : QWidget(parent)
{
    mMin = Min;
    mMax = Max;
    mStep = Step;
    mWidth = Width;
    mHeight = Height;
    mValue = mMin;
    mHead = tr("当前值");
    mEnd = tr("");

    fInit();
}

void TScale::fSetRange(double Min, double Max, double Step)
{
    mMin = Min;
    mMax = Max;
    mStep = Step;

    update();
}

bool TScale::fSetValue(double Value)
{
    if(Value >= mMin && Value <= mMax)
    {
        mValue = Value;
        mBar->setValue(mValue);
        update();
        return true;
    }
    else
    {
        return false;
    }
}

void TScale::fSetText(const QString &Head, const QString &End)
{
    mHead = Head;
    mEnd = End;
    update();
}

void TScale::fSetTextHead(const QString &Head)
{
    mHead = Head;
    update();
}

void TScale::fSetTextEnd(const QString &End)
{
    mEnd = End;
    update();
}

void TScale::fSetColor(const QColor &Color)
{
    if(Color.isValid())
    {
        mBar->setStyleSheet(tr("QProgressBar::chunk { background-color: rgb(")
                            + QString::number(Color.red()) + tr(",")
                                  + QString::number(Color.green()) + tr(",")
                                  + QString::number(Color.blue()) + tr(") }"));

        mBar->setTextVisible(false);

        update();
    }
}

void TScale::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);//声明用来绘图用的painter

    painter.setRenderHint(QPainter::Antialiasing);//绘制的图像反锯齿
    painter.translate(0, 0);//重新定位坐标起始点
    painter.scale(width() / 60.0, height() / 200.0);//设定画布的边界

    int Count = (mMax - mMin) / mStep ;

    painter.setPen(Qt::SolidLine);//填充时针，不需要边线所以NoPen
    //painter.setBrush(QColor());//画刷颜色设定

    //qDebug() << "count" << Count;

    for(int i = 0; i <= Count; i++)
    {
        int x = 30;
        int y = mBar->geometry().y() + mBar->height() * i / Count;
        int Value = mMin + (Count - i) * mStep;
        int TextSpace = 8;

        //qDebug() << "i" << i << ",y" << y;
        painter.drawLine(x, y, x + 5, y);

        if(Value < 0)
        {
            TextSpace = 4;
        }

        painter.drawText(TextSpace, y + 4, QString::number(Value));
    }

    //qDebug() << mHead << QString::number(mValue) << mEnd;
    painter.drawText(0, 185, mHead + QString::number(mValue) + mEnd);
}
