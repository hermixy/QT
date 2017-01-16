#ifndef TABWIDGETREPORT_H
#define TABWIDGETREPORT_H

#include <QDialog>

namespace Ui {
class Form;
}

class TabWidgetReport : public QDialog
{
    Q_OBJECT

public:
    explicit TabWidgetReport(QWidget *parent = 0);
    ~TabWidgetReport();
    QStringList reportName;
    QList<QStringList> content;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Form *ui;
    bool ok=false;
};

#endif // TABWIDGETREPORT_H
