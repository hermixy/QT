#include "gong.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gong mGong;

    mGong.showMaximized();

    return a.exec();
}

