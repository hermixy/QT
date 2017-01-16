#ifndef TTIMER_H
#define TTIMER_H
#include "../../Src/Base/Base.h"
#include "../Scada/Src/Scene/Scene.h"
#include <QObject>
class TTimer : public QDialog
{
    Q_OBJECT
public:
    TTimer(QWidget *parent=0);
    ~TTimer();
    QTimeEdit *startTime;
    QTimeEdit *stopTime;
    QPushButton* mButtonOk;
    QPushButton* mButtonClose;

    QTime startQTime;
    QTime stopQTime;

public slots:
    void fLogin();
};

#endif // TTIMER_H
