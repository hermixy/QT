#include "battery.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Battery mBattery;

    //mBattery.showMaximized();
    mBattery.show();
    
    return a.exec();
}
