#include "v6service.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    V6Service mV6Service;

    //mV6Service.showMaximized();
    mV6Service.show();

    return a.exec();
}
