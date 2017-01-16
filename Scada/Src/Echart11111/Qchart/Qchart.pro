#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T23:35:34
#
#-------------------------------------------------

QT       += core gui
QT += webkitwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qchart
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ChartLine.cpp

HEADERS  += widget.h \
    ChartLine.h

FORMS    += widget.ui
