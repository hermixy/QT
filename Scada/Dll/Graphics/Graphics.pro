#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T13:49:42
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = Graphics
TEMPLATE = lib

DEFINES += GRAPHICS_LIBRARY

SOURCES += Graphics.cpp

HEADERS += Graphics.h\
        graphics_global.h \
    ../../Src/Item/Item.h \
    ../../Src/Js/Js.h \
    ../../Src/Base/LibBase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Src/Js/ -lJs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Src/Js/ -lJsd
else:unix: LIBS += -L$$PWD/../../Src/Js/ -lJs

INCLUDEPATH += $$PWD/../../Src/Js
DEPENDPATH += $$PWD/../../Src/Js

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Src/Item/ -lItem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Src/Item/ -lItemd
else:unix: LIBS += -L$$PWD/../../Src/Item/ -lItem

INCLUDEPATH += $$PWD/../../Src/Item
DEPENDPATH += $$PWD/../../Src/Item
