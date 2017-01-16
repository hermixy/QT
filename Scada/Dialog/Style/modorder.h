#ifndef MODORDER_H
#define MODORDER_H
#include "../../Src/Service/Service.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "../Scada/Src/Base/Base.h"
class ModOrder : public QDialog
{
    Q_OBJECT
public:
    ModOrder(int  order,double i1,double i2, double i3=0, int i4=0, QString  i5=0, QWidget *parent=0);
    ~ModOrder();


    QLabel *mgroupName;
    QLabel *mGroupId;
    QLineEdit *mEditButton1_1;
    QLineEdit *mEditButton1_2;
    QLineEdit *mEditButton1_3;
    QLineEdit *mEditButton1_4;

    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    QComboBox* mComboBoxService;
public slots:
    void fApply();
public:
    bool isSave=false;
};

#endif // MODORDER_H
