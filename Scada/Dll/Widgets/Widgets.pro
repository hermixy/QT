#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T14:00:43
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = Widgets
TEMPLATE = lib

DEFINES += WIDGETS_LIBRARY

SOURCES += Widgets.cpp

HEADERS += Widgets.h\
        widgets_global.h \
    ../../Src/Item/Item.h \
    ../../Src/Js/Js.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Src/Item/ -lItem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Src/Item/ -lItemd
else:unix: LIBS += -L$$PWD/../../Src/Item/ -lItem

INCLUDEPATH += $$PWD/../../Src/Item
DEPENDPATH += $$PWD/../../Src/Item

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Src/Js/ -lJs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Src/Js/ -lJsd
else:unix: LIBS += -L$$PWD/../../Src/Js/ -lJs

INCLUDEPATH += $$PWD/../../Src/Js
DEPENDPATH += $$PWD/../../Src/Js
