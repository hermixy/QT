#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>

class TClock : public QWidget
{
    Q_OBJECT

public:
    enum TClockType
    {
        AutoTime,
        SetTime
    };

    TClock(TClockType ClockType = AutoTime, int Width = 100, int Height = 100, QWidget *parent = 0);

    bool fSetTime(int Hour, int Min, int Sec);
    bool fSetPointerVisible(bool Hour, bool Min, bool Sec);
    bool fSetCalibrationVisible(bool Hour, bool Min);
    bool fSetPointerWidth(int Width);

protected:
    void paintEvent(QPaintEvent *event);//重绘用的事件处理

private:
    TClockType mType;

    int mWidth;
    int mHeight;

    int mHour;
    int mMin;
    int mSec;

    // 指针可见性
    bool mVisibleHour;
    bool mVisibleMin;
    bool mVisibleSec;

    // 刻度可见性
    bool mVisibleCaliHour;
    bool mVisibleCaliMin;

    // 指针宽度
    int mPointerWidth;
};

#endif // CLOCK_H
