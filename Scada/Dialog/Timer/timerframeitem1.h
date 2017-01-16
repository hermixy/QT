#ifndef TIMERFRAMEITEM1_H
#define TIMERFRAMEITEM1_H
#include <QTime>
#include <QWidget>

namespace Ui {
class TimerFrameItem1;
}
struct TimerOnOff
{
    int day;        // 星期一到7
    QTime time;
    int onOff;
};

class TimerFrameItem1 : public QWidget
{
    Q_OBJECT

public:
    explicit TimerFrameItem1(QString name, QList<TimerOnOff> timerOn, QWidget *parent = 0);
    ~TimerFrameItem1();
    QList<TimerOnOff> getTimer();

private:
    Ui::TimerFrameItem1 *ui;
};

#endif // TIMERFRAMEITEM1_H
