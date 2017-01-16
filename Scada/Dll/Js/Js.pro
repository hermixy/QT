#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T13:43:46
#
#-------------------------------------------------

QT       += script

QT       -= gui

TARGET = Js
TEMPLATE = lib

DEFINES += JS_LIBRARY

SOURCES += Js.cpp

HEADERS += Js.h\
        js_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
