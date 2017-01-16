#ifndef LIGHT_VIEW_H
#define LIGHT_VIEW_H

#include <QLabel>
#include <QTimer>

class TLightView : public QLabel
{
    Q_OBJECT

public:
    enum TLightMode
    {
        ModeNormal,
        ModeAlarm
    };

    TLightView(int Width = 100, int Height = 100, QWidget *parent = 0);

    bool fSetMode(TLightMode Mode);

private slots:
    void fTimeDo();

private:
    TLightMode mMode;
    int mAlarmState;

    QTimer *mTimer;
};

#endif // LIGHT_VIEW_H
