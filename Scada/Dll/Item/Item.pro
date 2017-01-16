#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T13:45:45
#
#-------------------------------------------------

QT       += widgets

TARGET = Item
TEMPLATE = lib

DEFINES += ITEM_LIBRARY

SOURCES += Item.cpp

HEADERS += Item.h\
        item_global.h \
    ../../Src/Base/LibBase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
