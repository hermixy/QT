#include "LightView.h"

TLightView::TLightView(int Width, int Height, QWidget *parent): QLabel(parent)
{
    mMode = ModeNormal;
    mAlarmState = 0;

    mTimer = new QTimer(this);//声明一个定时器
    connect(mTimer, SIGNAL(timeout()), this, SLOT(fTimeDo()));//连接信号槽，定时器超时触发窗体更新
    mTimer->start(500);//启动定时器

    setFixedSize(Width, Height);
    setScaledContents(true);
    setPixmap(QPixmap(tr(":/root/pic/normal.png")));
}

bool TLightView::fSetMode(TLightMode Mode)
{
    mMode = Mode;
    return true;
}

void TLightView::fTimeDo()
{
    if(ModeAlarm == mMode)
    {
        if(0 == mAlarmState)
        {
            mAlarmState = 1;
            setPixmap(QPixmap(tr(":/root/pic/red0.png")));
        }
        else
        {
            mAlarmState = 0;
            setPixmap(QPixmap(tr(":/root/pic/red1.png")));
        }
    }
    else
    {
        setPixmap(QPixmap(tr(":/root/pic/normal.png")));
    }
}

