#ifndef ONLINESTATUS_H
#define ONLINESTATUS_H
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/Service/Service.h"
#include <QDialog>
#include <QHash>
#include <QList>
#include <QLabel>

namespace Ui {
class OnlineStatus;
}

class OnlineStatus : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineStatus(QList<TService *> mServiceList, QWidget *parent = 0);
    ~OnlineStatus();
    QHash<QString , QList<QLabel *>>  ipToLabel;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::OnlineStatus *ui;
};

#endif // ONLINESTATUS_H
