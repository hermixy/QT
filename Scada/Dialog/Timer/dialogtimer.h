#ifndef DIALOGTIMER_H
#define DIALOGTIMER_H
#include "timerframeitem1.h"
#include <QDialog>
#include "../Scada/Src/Scene/Scene.h"
namespace Ui {
class DialogTimer;
}

class DialogTimer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTimer(TScene* mSceneList,QWidget *parent = 0);
    ~DialogTimer();
    bool mSave=false;
signals:
    void saveDateTimer();

public slots:
    void applay();
    void fclose();

public:
    QHash<QString, TimerFrameItem1 *> timerMapToItem;
    QHash<QString, QList<TimerOnOff>> timerMapToGroup;
    TScene* myscene;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogTimer *ui;

};

#endif // DIALOGTIMER_H
