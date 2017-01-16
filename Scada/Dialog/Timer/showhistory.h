#ifndef SHOWHISTORY_H
#define SHOWHISTORY_H
#include <QDialog>
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/DataBase/MySQL/MySQL.h"
#include <QGridLayout>
#include <QDateTimeEdit>
class ShowHistory : public QDialog
{
    Q_OBJECT
public:
    ShowHistory(QList<TScene*> mSceneList, QWidget *parent=nullptr, bool isreport=false);
    ~ShowHistory();

    QGridLayout *mLayoutMain;
public slots:
    void showHistory();
    void report();
private:
    QHash<int,QString > groupIdList;
    QDateTimeEdit *startTIme;
    QDateTimeEdit *stopTIme;
};

#endif // SHOWHISTORY_H
