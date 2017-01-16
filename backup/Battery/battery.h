#ifndef BATTERY_H
#define BATTERY_H

#include "../base/base.h"
#include <QImage>
#include <QMainWindow>

class Battery : public QMainWindow
{
    Q_OBJECT
public:
    Battery(QWidget *parent = 0);

private slots:
    void fDataChange(double Data);

private:
    QLabel *mLabelArray[16];
    //QImage *mImageArray[16];
    QLabel *mLabelData[16];

    QDoubleSpinBox *mSpinData;
};

#endif // BATTERY_H
