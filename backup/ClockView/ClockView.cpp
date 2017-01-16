#include "Clock.h"

#include <QPainter>
#include <QTime>
#include <QTimer>

TClock::TClock(TClockType ClockType, int Width, int Height, QWidget *parent): QWidget(parent)
{
    mHour = 0;
    mMin = 0;
    mSec = 0;
    mVisibleHour = true;
    mVisibleMin = true;
    mVisibleSec = true;

    mVisibleCaliHour = true;
    mVisibleCaliMin = true;

    mPointerWidth = 3;

    mWidth = Width;
    mHeight = Height;

    mType = ClockType;

    if(AutoTime == mType)
    {
        QTimer *timer = new QTimer(this);//声明一个定时器
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));//连接信号槽，定时器超时触发窗体更新
        timer->start(1000);//启动定时器
    }

    //setWindowTitle(tr("Clock"));//设置窗体名称
    //resize(200, 200);//设置窗体大小
    setFixedSize(mWidth, mHeight);
}

bool TClock::fSetTime(int Hour, int Min, int Sec)
{
    if(SetTime == mType)
    {
        mHour = Hour;
        mMin = Min;
        mSec = Sec;

        update();

        return true;
    }
    else
    {
        return false;
    }
}

bool TClock::fSetPointerVisible(bool Hour, bool Min, bool Sec)
{
    mVisibleHour = Hour;
    mVisibleMin = Min;
    mVisibleSec = Sec;

    return true;
}

bool TClock::fSetCalibrationVisible(bool Hour, bool Min)
{
    mVisibleCaliHour = Hour;
    mVisibleCaliMin = Min;

    return true;
}

bool TClock::fSetPointerWidth(int Width)
{
    if(Width > 0)
    {
        mPointerWidth = Width;
        return true;
    }
    else
    {
        return false;
    }
}

void TClock::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    //下面三个数组用来定义表针的三个顶点，以便后面的填充
    static const QPoint hourHand[3] = {
        QPoint(mPointerWidth, 8),
        QPoint(-mPointerWidth, 8),
        QPoint(0, -40)
    };//时针
    static const QPoint minuteHand[3] = {
        QPoint(mPointerWidth, 8),
        QPoint(-mPointerWidth, 8),
        QPoint(0, -70)
    };//分针
    static const QPoint secondHand[3] = {
        QPoint(mPointerWidth, 8),
        QPoint(-mPointerWidth, 8),
        QPoint(0, -90)
    };//秒针

    //填充表针的颜色
    QColor hourColor(127, 0, 127);//时针颜色
    QColor minuteColor(0, 127, 127, 191);//分针颜色
    QColor secondColor(127, 127,0,127);//秒针颜色

    int side = qMin(width(), height());//绘制的范围(宽、高中最小值)

    // 自动模式，根据系统时间，更新
    if(AutoTime == mType)
    {
        QTime time = QTime::currentTime();//获取当前的时间

        mHour = time.hour();
        mMin = time.minute();
        mSec = time.second();
    }

    QPainter painter(this);//声明用来绘图用的painter

    painter.setRenderHint(QPainter::Antialiasing);//绘制的图像反锯齿
    painter.translate(width() / 2, height() / 2);//重新定位坐标起始点点
    painter.scale(side / 200.0, side / 200.0);//设定画布的边界

    if(mVisibleHour)
    {
        painter.setPen(Qt::NoPen);//填充时针，不需要边线所以NoPen
        painter.setBrush(hourColor);//画刷颜色设定

        painter.save();//保存painter的状态

        painter.rotate(30.0 * (mHour + mMin / 60.0));//将painter（的”视角“）根据时间参数转移(30° * (小时 + 分钟 / 60))
        painter.drawConvexPolygon(hourHand, 3);//填充时针的区域

        painter.restore();//恢复填充前“画家”的状态
    }

    if(mVisibleCaliHour)
    {
        painter.setPen(hourColor);//下面画表示小时的刻度，此时要用到画笔（因为要划线）
        QPen pen = painter.pen();
        pen.setWidth(5);
        painter.setPen(pen);

        for (int i = 0; i < 12; ++i) //十二个刻度，循环下就好了
        {
           painter.drawLine(88, 0, 96, 0);//没次都是这样，先画跳线，再转个角
           painter.rotate(30.0);
        }
    }

    // 绘制表盘数字
    painter.drawText(-5, -75, tr("12"));
    painter.drawText(-3, 83, tr("6"));
    painter.drawText(79, 3, tr("3"));
    painter.drawText(-83, 3, tr("9"));

    //后面的跟前面的类似，分别绘制了分针和秒针，及相应的刻度

    if(mVisibleMin)
    {
        painter.setPen(Qt::NoPen);//填充分针，不需要边线所以NoPen
        painter.setBrush(minuteColor);//设置画刷颜色

        painter.save();//保存当前状态
        painter.rotate(6.0 * (mMin + mSec / 60.0));//设旋转(角度 = 6° * (分钟 + 秒 / 60))
        painter.drawConvexPolygon(minuteHand, 3);//填充分针部分
        painter.restore();//恢复保存前状态
    }

    if(mVisibleCaliMin)
    {
        painter.setPen(minuteColor);//设置画刷颜色

        for (int j = 0; j < 60; ++j) //循环60次，绘制表盘
        {
            if ((j % 5) != 0)//判断是否能被5整除(能被5整除表示是正点刻度，暂不绘制)
            {
                painter.drawLine(92, 0, 96, 0);//不是正点刻度，绘制长4个像素的直线
            }
            painter.rotate(6.0);//旋转六度
        }//循环60次，每次旋转6度，所以不用save和restore
    }

    // 绘制秒针
    if(mVisibleSec)
    {
        painter.setPen(Qt::NoPen);//填充分针，不需要边线所以NoPen

        painter.setBrush(secondColor);//设置画刷颜色

        painter.save();//保存当前状态
        painter.rotate(6.0 * mSec);//设置旋转(6° * 秒)
        painter.drawConvexPolygon(secondHand, 3);//设置填充
        painter.restore();//恢复保存前状态
    }
     //painter.end();//释放掉图形所占存储空间
}
