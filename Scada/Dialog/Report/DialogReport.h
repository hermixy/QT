#ifndef DIALOG_REPORT_H
#define DIALOG_REPORT_H

#include "../../Src/Base/Base.h"

class DialogReport : public QDialog
{
    Q_OBJECT

public:
    DialogReport(QWidget *parent = 0);
    ~DialogReport();

private slots:
    void fCreate();

private:
    QLabel *mLabelShow;
    QPushButton *mButtonCreate;
    QPushButton *mButtonClose;
};

#endif // DIALOG_REPORT_H
