#ifndef ALARMDIALOG_H
#define ALARMDIALOG_H
#include <QDialog>
#include <QLabel>
class AlarmDialog : public QDialog
{
    Q_OBJECT
public:
    AlarmDialog(QWidget *parent=0, QString alarm=QString());
    ~AlarmDialog();
    QLabel* mLabelName;
public slots:
    void handleAlarm();
    void setTitle(QString name);
};

#endif // ALARMDIALOG_H
